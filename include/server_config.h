#include <WiFi.h>
#include <WebServer.h>

#include <ArduinoJson.h>
#include <html_index.h>

WebServer server(80);

void handleIndex()
{
    server.send(200, "text/html", INDEX_HTML);
}

void handleGetSettings()
{
    String settings;
    StaticJsonDocument<256> doc;
    doc["speed"] = speed;
    doc["brightness"] = brightness;
    doc["mode"] = modeString.c_str();
    doc["state"] = stateString.c_str();
    JsonArray effects = doc.createNestedArray("effects");
    for (size_t i = 0; i < sizeof effectList; i++)
    {
        effects.add(effectList[i]);
    }

    serializeJson(doc, settings);
    server.send(200, "application/json", settings);
}

void handleSetSettings()
{
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, server.arg("plain"));
    Serial.println(server.arg("plain"));
    Serial.println("Received message from client");
    if (error)
    {
        Serial.println("Failed to parse json");
        server.send(400);
    }
    if (doc.containsKey("state"))
    {
        state = doc["state"];
        oldStateString = stateString;
        stateString = state;
    }
    if (doc.containsKey("speed"))
    {
        speed = doc["speed"];
    }
    if (doc.containsKey("brightness"))
    {
        brightness = doc["brightness"];
    }
    if (doc.containsKey("mode"))
    {
        oldModeString = modeString;
        mode = doc["mode"];
        modeString = mode;
    }
    server.send(200);
}


void handleSetDefaults()
{
    server.send(200);
}

void setupServer()
{
    server.on("/", handleIndex);
    server.on("/getsettings", handleGetSettings);
    server.on("/setsettings", HTTP_POST, handleSetSettings);
    server.on("/reset", handleSetDefaults);
    server.begin();
    Serial.println("Server started");
}