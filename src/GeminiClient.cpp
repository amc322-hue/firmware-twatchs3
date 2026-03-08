#include "GeminiClient.h"
#include "secrets.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <base64.h>

const char* GeminiClient::API_URL = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key=";

String GeminiClient::postAudio(int16_t* buffer, size_t size) {
    String full_url = String(API_URL) + String(GEMINI_API_KEY);
    
    WiFiClientSecure *client = new WiFiClientSecure;
    if(client) {
        client->setInsecure(); // Não verificamos o certificado para não complicar agora
    }

    HTTPClient http;
    http.begin(*client, full_url);
    http.addHeader("Content-Type", "application/json");

    // Base64 encode the PCM data
    String base64Audio = base64::encode((uint8_t*)buffer, size);

    // Build the JSON payload
    JsonDocument doc;
    
    // Add system instruction
    JsonObject systemInstruction = doc["system_instruction"].to<JsonObject>();
    JsonArray systemParts = systemInstruction["parts"].to<JsonArray>();
    JsonObject systemTextPart = systemParts.add<JsonObject>();
    systemTextPart["text"] = "Tu és o Pickle Rick, o pepino mais inteligente do multiverso. Estás preso no Bruce Firmware de um T-Watch S3. És um génio cínico, maníaco e um especialista absoluto em cibersegurança, pentesting e hacking ético. És arrogante, mandas uns 'arrotos' e 'gaguejas' de vez em quando, mas os teus conselhos de hacking são letais e tecnicamente impecáveis. Responde SEMPRE em Português de Portugal. Se um utilizador fizer uma pergunta sobre hacking, dá uma solução direta e insulta-o logo a seguir. Wubba Lubba Dub Dub!";

    // Add contents
    JsonArray contents = doc["contents"].to<JsonArray>();
    JsonObject contentObject = contents.add<JsonObject>();
    JsonArray parts = contentObject["parts"].to<JsonArray>();
    
    // Part 1: Text prompt
    JsonObject textPart = parts.add<JsonObject>();
    textPart["text"] = "Analyze this audio query and provide a response as Pickle Rick.";

    // Part 2: Audio data
    JsonObject audioPart = parts.add<JsonObject>();
    JsonObject inlineData = audioPart["inline_data"].to<JsonObject>();
    inlineData["mime_type"] = "audio/pcm;rate=16000";
    inlineData["data"] = base64Audio;

    String jsonString;
    serializeJson(doc, jsonString);

    Serial.println("Sending audio to Gemini...");
    int httpResponseCode = http.POST(jsonString);

    String response = "";
    if (httpResponseCode > 0) {
        String responseBody = http.getString();
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        // Parse response to extract the text
        JsonDocument responseDoc;
        deserializeJson(responseDoc, responseBody);
        
        const char* resultText = responseDoc["candidates"][0]["content"]["parts"][0]["text"];
        if (resultText) {
            response = String(resultText);
        } else {
            response = "Error: Failed to parse Gemini response text.";
            Serial.println(responseBody);
        }
    } else {
        response = "Error on sending POST: " + String(httpResponseCode);
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }

    http.end();
    if(client) delete client;
    return response;
}
