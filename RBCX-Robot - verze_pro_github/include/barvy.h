#include <Wire.h>
#include "Adafruit_TCS34725.h"
/* Example code for the Adafruit TCS34725 breakout library */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
bool cervena(){
    float r, g, b;
    tcs.getRGB(&r, &g, &b);
    float red_avg = r;
    float green_avg = g;
    float blue_avg = b;
    printf("red: %f, green: %f, blue: %f\n", red_avg, green_avg, blue_avg);
    delay(10);
    if (red_avg > green_avg && red_avg > blue_avg && red_avg > 100)
    {
        printf("RED\n");
        rkLedRed(true);
        return true;
    }
    else {
        return false;
    }
}