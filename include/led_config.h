#include <FastLED.h>
//panels config
#define NUMPANELS 9
#define NUM_LEDS_IN_PANEL 6
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
const char *state = "on";
String stateString = "on";
String oldStateString = "on";
int speed = 50;
int brightness = 100;
const char *mode = "Rainbow";
String modeString = "Rainbow";
String oldModeString = "Rainbow";

void setPanelColor(int panel, int size, CRGB color)
{
    for (int led = 0; led < size; led++)
    {
        leds[PANEL_LEDS[panel][led]] = color;
    }
}