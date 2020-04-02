#include <Arduino.h>
#include <PubSubClient.h>
#include <DNSServer.h>

#include <ArduinoOTA.h>

#include <led_config.h>
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
