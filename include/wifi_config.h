#include <ESP8266WiFi.h>
#include <arduino_secrets.h>
#include <FastLED.h>

void setupWifi()
{
    WiFi.begin(SECRET_SSID, SECRET_PWD);
    WiFi.hostname(AP_HOSTNAME);

    while (WiFi.status() != WL_CONNECTED)
    {
        setPanelColor(3, 3, CRGB::Red);
        FastLED.show();
        delay(250);
        setPanelColor(3, 3, CRGB::Black);
        delay(250);
    }
}