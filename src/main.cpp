#include <Arduino.h>
#include <PubSubClient.h>
#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FastLED.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>

#include <arduino_secrets.h>
#include <html_index.h>

//wifi and webserver configs

//panels config
#define NUMPANELS 9
#define NUM_LEDS_IN_PANEL 3
const int NUM_LEDS = NUMPANELS * NUM_LEDS_IN_PANEL;

//fade/flash
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

//RAINBOW
uint8_t thishue = 0; // Starting hue value.
uint8_t deltahue = 10;

//LED -> PANEL mapping
const int PANEL_LEDS[NUMPANELS][NUM_LEDS_IN_PANEL] =
    {
        {17, 16, 18}, //panel 0
        {15, 11, 19}, //panel 1
        {14, 13, 12}, //panel 2
        {20, 10, 21}, //panel 3
        {9, 8, 22},   //panel 4
        {23, 7, 24},  //panel 5
        {6, 2, 25},   //panel 6
        {5, 4, 3},    //panel 7
        {26, 1, 0},   //panel 8
};

//Fast led settings
#define DATA_PIN D1
CRGB leds[NUMPANELS * NUM_LEDS_IN_PANEL];
int speed = 50;
int brightness = 100;
const char *mode = "Random";
String modeString = "Random";
String oldModeString = "Random";

void setPanelColor(int panel, int size, CRGB color)
{
  for (int led = 0; led < size; led++)
  {
    leds[PANEL_LEDS[panel][led]] = color;
  }
}
ESP8266WebServer server(80);

void handleIndex()
{
  server.send(200, "text/html", MAIN_page);
}
void handleGetSettings()
{
  String settings;
  StaticJsonDocument<256> doc;
  doc["speed"] = speed;
  doc["brightness"] = brightness;
  doc["mode"] = modeString.c_str();
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
void setColor(int inR, int inG, int inB)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i].red = inR;
    leds[i].green = inG;
    leds[i].blue = inB;
  }

  FastLED.show();

  Serial.println("Setting LEDs:");
  Serial.print("r: ");
  Serial.print(inR);
  Serial.print(", g: ");
  Serial.print(inG);
  Serial.print(", b: ");
  Serial.println(inB);
}
void setup()
{
  Serial.begin(115200);
  FastLED.addLeds<WS2812B, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(150);
  WiFi.begin(SECRET_SSID, SECRET_PWD);
  WiFi.hostname(AP_HOSTNAME);

  //check if wifi is connected and blink panel if true
  while (WiFi.status() != WL_CONNECTED)
  {
    setPanelColor(3, 3, CRGB::Red);
    FastLED.show();
    delay(250);
    setPanelColor(3, 3, CRGB::Black);
    delay(250);
  }
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

void loop()
{
  ArduinoOTA.handle();
  server.handleClient();

  //effects

  EVERY_N_MILLISECONDS(1000 / speed)
  {
    if (modeString == "Rainbow")
    { //Single Dot Down
      rainbow();
    }
    if (modeString == "Dot")
    {
      sinelon();
    }
    if (modeString == "Rainbow2")
    {
      rainbowWithGlitter();
    }
  }
}
