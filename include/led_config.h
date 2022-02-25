#include <FastLED.h>
//panels config
#define NUMPANELS 8
#define NUM_LEDS_IN_PANEL 6
const int NUM_LEDS = NUMPANELS * NUM_LEDS_IN_PANEL;

//fade/flash
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

//RAINBOW
uint8_t thishue = 0; // Starting hue value.
uint8_t deltahue = 10;

//LED -> PANEL mapping

//Fast led settings
#define DATA_PIN 16

CRGB leds[NUMPANELS * NUM_LEDS_IN_PANEL];
const char *state = "on";
String stateString = "on";
String oldStateString = "on";
int speed = 50;
uint8_t brightness = 100;
const char *mode = "Rainbow";
String modeString = "Rainbow";
String oldModeString = "Rainbow";
String randomString = "Random";
void setupLeds()
{
    FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(150);
}
/*
void setPanelColor(int panel, int size, CRGB color)
{
    for (int led = 0; led < size; led++)
    {
        leds[PANEL_LEDS[panel][led]] = color;
    }
}
*/