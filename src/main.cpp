#include <Arduino.h>
#include <LilyGoLib.h>
#include "PMUManager.h"
#include "AudioCapture.h"
#include "WiFiManager.h"
#include "GeminiClient.h"
#include "UIManager.h"

#define BUFFER_SIZE 512
int16_t capture_buffer[BUFFER_SIZE];

#define TOTAL_SAMPLES 80000 // 5 seconds @ 16kHz
int16_t* recording_buffer = nullptr;
size_t samples_captured = 0;
bool is_recording = false;

void setup() {
    Serial.begin(115200);

    // 1. Initialize Watch Hardware (Display, Power, Touch)
    watch.begin();
    
    // 2. Initialize LVGL
    lv_init();
    watch.lvgl_begin(); 

    // 3. Initialize UI
    UIManager::begin();

    // 4. Initialize Peripherals
    if (!PMUManager::begin()) return;
    PMUManager::enableMicrophonePower();

    if (!AudioCapture::begin()) return;
    WiFiManager::connect();

    recording_buffer = (int16_t*)(psramFound() ? ps_malloc(TOTAL_SAMPLES * sizeof(int16_t)) : malloc(TOTAL_SAMPLES * sizeof(int16_t)));
    if (!recording_buffer) {
        Serial.println("Memory allocation failed!");
        UIManager::setResponse("Memory allocation failed! Jerry made this.");
    } else {
        Serial.println("PickleRick AI Ready.");
    }
}

void processGeminiResponse() {
    UIManager::setState(UI_STATE_THINKING);
    
    if (!WiFiManager::isConnected() && !WiFiManager::connect()) {
        UIManager::setResponse("WiFi error. Fix your router, Morty.");
        return;
    }

    String response = GeminiClient::postAudio(recording_buffer, samples_captured * sizeof(int16_t));
    Serial.println("\n--- Response ---\n" + response + "\n----------------\n");
    
    UIManager::setResponse(response.c_str());
}

void loop() {
    // Handle UI tasks
    UIManager::update();

    // Handle recording trigger (from UI button)
    if (UIManager::isButtonPressed()) {
        if (recording_buffer && !is_recording) {
            Serial.println("Recording started by UI...");
            samples_captured = 0;
            is_recording = true;
        }
    } else {
        if (is_recording) {
            Serial.println("Recording stopped by UI.");
            is_recording = false;
            processGeminiResponse();
        }
    }

    if (is_recording) {
        size_t bytes_read = AudioCapture::read(capture_buffer, BUFFER_SIZE * sizeof(int16_t));
        if (bytes_read > 0) {
            size_t samples_read = bytes_read / sizeof(int16_t);
            for (size_t i = 0; i < samples_read && samples_captured < TOTAL_SAMPLES; i++) {
                recording_buffer[samples_captured++] = capture_buffer[i];
            }

            if (samples_captured >= TOTAL_SAMPLES) {
                is_recording = false;
                processGeminiResponse();
            }
        }
    }
    yield();
}
