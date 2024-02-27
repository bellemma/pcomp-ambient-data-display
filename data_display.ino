#include "FastLED.h"
#include <SparkFun_Qwiic_OLED.h>

#define PIN_NEO_PIXEL_ONE  10   // Arduino pin that connects to first NeoPixel
#define NUM_LEDS_ONE 28 // LEDs on first NeoPix
#define YEARS 15 // years for which we have housing data


CRGB leds[NUM_LEDS_ONE];
QwiicNarrowOLED myOLED;

int yearPercentageMap[YEARS][2] = {
    {2005, 64},
    {2006, 74},
    {2007, 83},
    {2008, 81},
    {2009, 86},
    {2010, 78},
    {2011, 89},
    {2012, 86},
    {2013, 92},
    {2014, 90},
    {2015, 95},
    {2016, 96},
    {2017, 97},
    {2018, 95},
    {2019, 86},
};

// Function to retrieve percentage value based on the year
int getPercentage(int year) {
    for (int i = 0; i < YEARS; i++) {
        if (year == yearPercentageMap[i][0]) {
            return yearPercentageMap[i][1];
        }
    }
    // If year not found
    return 100;
}

void display(int year, int percentage) {

  myOLED.erase();
  String housing_data = String(year) + ": "+ String(100 - percentage)+ "% affordable"; // our message

  // Center our message on the screen. Get the screen size of the string
  // starting x position - screen width minus string width  / 2
  int x0 = (myOLED.getWidth() - myOLED.getStringWidth(housing_data)) / 2;

  // starting y position - screen height minus string height / 2 
  int y0 = (myOLED.getHeight() - myOLED.getStringHeight(housing_data)) / 2;

  // Draw the text
  myOLED.text(x0, y0, housing_data, 1);

  // send the graphics to the device
  myOLED.display();
}

void setup() { 
  FastLED.addLeds<NEOPIXEL, PIN_NEO_PIXEL_ONE>(leds, NUM_LEDS_ONE);

  Wire.begin();
  Serial.begin(115200);

     if (myOLED.begin() == false)
    {
        Serial.println("Device begin failed. Freezing...");
        while (true)
            ;
    }
    Serial.println("Begin success");
  }

void loop() {
  int potValue = analogRead(A0); // Read potentiometer value
  int year = map(potValue, 0, 1023, 2005, 2020); // Map potentiometer value to year range

  if (potValue > 1015) {
    year = 2019; // Set to 2019 if potentiometer value is above threshold
  }

  // Retrieve percentage value from array based on the year
  int percentage = getPercentage(year);
  Serial.println(percentage);

  display(year, percentage);

  // Calculate number of LEDs for each color
  int numRedLEDs = map(percentage, 0, 100, 0, NUM_LEDS_ONE);
  int numBlueLEDs = NUM_LEDS_ONE - numRedLEDs;

  // Display the colors on the LED strip
  for (int i = 0; i < NUM_LEDS_ONE; i++) {
      if (i < numRedLEDs) {
          leds[i] = CRGB::Red; // Set LED color to red
      } else {
          leds[i] = CRGB::Blue; // Set LED color to blue
      }
  }
  FastLED.show(); // Update LED strip
}
