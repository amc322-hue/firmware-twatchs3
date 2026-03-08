#include "AudioCapture.h"

bool AudioCapture::begin() {
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 128,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_PIN_NO_CHANGE,
        .ws_io_num = MIC_CLK_PIN,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = MIC_DATA_PIN
    };

    esp_err_t err = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
    if (err != ESP_OK) {
        Serial.printf("Failed to install I2S driver: %d\n", err);
        return false;
    }

    err = i2s_set_pin(I2S_PORT, &pin_config);
    if (err != ESP_OK) {
        Serial.printf("Failed to set I2S pins: %d\n", err);
        return false;
    }

    err = i2s_set_clk(I2S_PORT, SAMPLE_RATE, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
    if (err != ESP_OK) {
        Serial.printf("Failed to set I2S clock: %d\n", err);
        return false;
    }

    Serial.println("I2S PDM Microphone initialized.");
    return true;
}

size_t AudioCapture::read(int16_t* buffer, size_t size) {
    size_t bytes_read = 0;
    esp_err_t err = i2s_read(I2S_PORT, buffer, size, &bytes_read, portMAX_DELAY);
    if (err != ESP_OK) {
        Serial.printf("Error reading from I2S: %d\n", err);
        return 0;
    }
    return bytes_read;
}

void AudioCapture::stop() {
    i2s_driver_uninstall(I2S_PORT);
    Serial.println("I2S Audio stopped.");
}
