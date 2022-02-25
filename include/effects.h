String effectList[8] = {
    "Random", "Rainbow", "Rainbow2", "Dot", "Confetti", "BPM", "Juggle", "Solid all"};

void rainbow()
{
    // FastLED's built-in rainbow generator
    fill_rainbow(leds, NUM_LEDS, gHue, 7);
}

void addGlitter(fract8 chanceOfGlitter)
{
    if (random8() < chanceOfGlitter)
    {
        leds[random16(NUM_LEDS)] += CRGB::White;
    }
}

void rainbowWithGlitter()
{
    // built-in FastLED rainbow, plus some random sparkly glitter
    rainbow();
    addGlitter(80);
}

void confetti()
{
    // random colored speckles that blink in and fade smoothly
    fadeToBlackBy(leds, NUM_LEDS, 10);
    int pos = random16(NUM_LEDS);
    leds[pos] += CHSV(gHue + random8(64), 200, 255);
}

void sinelon()
{
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy(leds, NUM_LEDS, 20);
    int pos = beatsin16(speed, 0, NUM_LEDS - 1);
    leds[pos] += CHSV(gHue, 255, 192);
}

void circleDot() 
{
    fadeToBlackBy(leds, NUM_LEDS,20);
    int pos = beatsin8(speed, 0 ,NUM_LEDS - 1);
    leds[pos] += CHSV(gHue,255, 192);
}

void bpm()
{
    // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
    uint8_t BeatsPerMinute = speed;
    CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
    for (int i = 0; i < NUM_LEDS; i++)
    { //9948
        leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    }
}

void juggle()
{
    // eight colored dots, weaving in and out of sync with each other
    fadeToBlackBy(leds, NUM_LEDS, 20);
    byte dothue = 0;
    for (int i = 0; i < 8; i++)
    {
        leds[beatsin16(i + speed, 0, NUM_LEDS - 1)] |= CHSV(dothue, 200, 255);
        dothue += 32;
    }
}

void colorize()
{
    fill_solid(leds, NUM_LEDS, CRGB::Green);
}

void randomEffect()
{
    randomString = effectList[random8(1, sizeof effectList - 1)];
}