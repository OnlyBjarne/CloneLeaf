#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <html_index.h>

ESP8266WebServer server(80);

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
    server.send(200);
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, server.arg("plain"));
    if (error)
    {
        Serial.println("Failed to parse json");
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
}

void handleSetDefaults()
{
    server.send(200);
}

void setupServer()
{
    server.on("/", handleIndex);
    server.on("/getsettings", handleGetSettings);
    server.on("/setsettings", handleSetSettings);
    server.on("/serdefaults", handleSetDefaults);
    server.begin();
    Serial.println("Server started");
}