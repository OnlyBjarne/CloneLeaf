#include <ESP8266WiFi.h>

void setupWifi()
{
    WiFi.hostname(AP_HOSTNAME);
    WiFi.begin(SECRET_SSID, SECRET_PWD);

    while (WiFi.status() != WL_CONNECTED)
    {
        if (debug)
            Serial.print(".");
        setPanelColor(3, 3, CRGB::Red);
        FastLED.show();
        digitalWrite(LED_BUILTIN, HIGH);
        delay(250);
        digitalWrite(LED_BUILTIN, LOW);
        setPanelColor(3, 3, CRGB::Black);
        delay(250);
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Wifi Connected");
        for (int led = 0; led < NUM_LEDS; led++)
        {
            leds[led] = CRGB::Blue;
            FastLED.show();
            leds[led] = CRGB::Black;
            delay(30);
        }
        FastLED.show();
    }
}

void setupOTA()
{
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

    ArduinoOTA.setHostname(AP_HOSTNAME);
    ArduinoOTA.setPassword(SECRET_OTA_PWD);
    ArduinoOTA.begin();
}