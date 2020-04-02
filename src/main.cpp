#include <Arduino.h>
#include <PubSubClient.h>
#include <DNSServer.h>

#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>

#include <led_config.h>
#include <arduino_secrets.h>
#include <wifi.h>
#include <html_index.h>

//wifi and webserver configs

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
    mode = doc["mode"];
    modeString = mode;
    Serial.println(modeString);
  }
}

void setup()
{
  Serial.begin(115200);
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(150);

  setupWifi();
  //check if wifi is connected and blink panel if true

  ArduinoOTA.onStart([]() {
    Serial.println("OTA Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("OTA End");
    Serial.println("Rebooting...");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r\n", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
      Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)
      Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR)
      Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR)
      Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)
      Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  if (WiFi.status() == WL_CONNECTED)
  {
    for (int led = 0; led < NUM_LEDS; led++)
    {
      leds[led] = CRGB::Blue;
      FastLED.show();
      leds[led] = CRGB::Black;
      delay(30);
    }
  }
  //setup server
  server.on("/", handleIndex);
  server.on("/getsettings", handleGetSettings);
  server.on("/setsettings", handleSetSettings);
  server.begin();
  Serial.println("Server started");
}

#include <effects.h>
int oldMillis = 0;
void loop()
{
  static CEveryNMilliseconds FPS(1000 / speed);
  ArduinoOTA.handle();
  server.handleClient();
  EVERY_N_MILLIS(20) { gHue++; }
  EVERY_N_MILLIS_I(timingObj, 0)
  {
    timingObj.setPeriod(1000 / speed);
    if (modeString == "Random")
    {
      confetti();
    }
    if (modeString == "Rainbow")
    {
      rainbow();
    }
    if (modeString == "RainbowGlitter")
    {
      rainbowWithGlitter();
    }
    if (modeString == "Solid")
    {
      colorize();
    }
    if (modeString == "Siren")
    {
      juggle();
    }
    if (modeString == "Breathing")
    {
      bpm();
    }
    if (modeString == "Running dot")
    {
      sinelon();
    }
    if (modeString == "Sparkle")
    {
      addGlitter(1);
    }
  }

  FastLED.show();
}
