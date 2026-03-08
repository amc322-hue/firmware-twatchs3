#ifndef AUDIO_CAPTURE_H
#define AUDIO_CAPTURE_H

#include <Arduino.h>
#include <driver/i2s.h>

class AudioCapture {
public:
    static bool begin();
    static size_t read(int16_t* buffer, size_t size);
    static void stop();

private:
    static const i2s_port_t I2S_PORT = I2S_NUM_0;
    static const int SAMPLE_RATE = 16000;
    static const int MIC_CLK_PIN = 44;
    static const int MIC_DATA_PIN = 47;
};

#endif // AUDIO_CAPTURE_H
