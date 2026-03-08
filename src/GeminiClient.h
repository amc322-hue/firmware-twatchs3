#ifndef GEMINI_CLIENT_H
#define GEMINI_CLIENT_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <base64.h>

class GeminiClient {
public:
    static String postAudio(int16_t* buffer, size_t size);

private:
    static const char* API_URL;
};

#endif // GEMINI_CLIENT_H
