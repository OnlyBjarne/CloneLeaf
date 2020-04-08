#include <Arduino.h>
#include <PubSubClient.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <ArduinoOTA.h>

#include <led_config.h>
#include <effects.h>
#include <arduino_secrets.h>
#include <wifi_config.h>
#include <server_config.h>

//wifi and webserver configs

void setup()
{
  Serial.begin(115200);
  setupLeds();
  setupWifi();
  //check if wifi is connected and blink panel if true

  setupServer();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  static CEveryNMilliseconds FPS(1000 / speed);
  ArduinoOTA.handle();
  server.handleClient();
  if (modeString == "Random")
  {
    EVERY_N_SECONDS(10)
    {
      randomEffect();
    }
  }
  EVERY_N_MILLIS(100)
  {
    if (stateString == "on" && FastLED.getBrightness() != brightness)
    {
      FastLED.setBrightness(round((FastLED.getBrightness() + brightness) / 2 + 0.5));
    }
    else if (stateString == "off")
    {
      FastLED.setBrightness(scale8(FastLED.getBrightness(), 64));
    }
  }
  EVERY_N_MILLIS_I(timingObj, 10)
  {
    if (stateString == "on")
    {
      //TODO: Add fade in when powering on

      if (modeString == "Confetti" || (modeString == "Random" && randomString == "Confetti"))
      {
        confetti();
      }
      if (modeString == "Rainbow" || (modeString == "Random" && randomString == "Rainbow"))
      {
        rainbow();
      }
      if (modeString == "Rainbow2" || (modeString == "Random" && randomString == "Rainbow2"))
      {
        rainbowWithGlitter();
      }
      if (modeString == "Solid" || (modeString == "Random" && randomString == "Solid"))
      {
        colorize();
      }
      if (modeString == "Juggle" || (modeString == "Random" && randomString == "Siren"))
      {
        juggle();
      }
      if (modeString == "BPM" || (modeString == "Random" && randomString == "Breathing"))
      {
        bpm();
      }
      if (modeString == "Dot" || (modeString == "Random" && randomString == "Dot"))
      {
        sinelon();
      }
      if (modeString == "Sparkle" || (modeString == "Random" && randomString == "Dot"))
      {
        addGlitter(1);
      }
      gHue++;
    }
  }
  timingObj.setPeriod(1000 / speed);

  FastLED.show();
}
