/*
 * This file is part of Volumetric-Display-Demo.
 *
 * Volumetric-Display-Demo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Volumetric-Display-Demo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include <Arduino.h>
#include <FastLED.h>
#include "cube_data.h"
#include "icohedron_data.h"
#include "tower_data.h"
#include "fighter_data.h"
#include "dino_data.h"

#define REED_PIN 4
#define NUM_LEDS 256
#define DATA_PIN 1

// Define the array of leds
CRGB leds[NUM_LEDS];

long int numRotations = 0;
long int startTime;
long int lastTime = 0;

double rpms;

volatile boolean firstModel = true;

double getRPM()
{
    long int elapsedTime = millis() - startTime;
    if (elapsedTime == 0)
        return 0;
    return (double(numRotations) / double(elapsedTime) * 60000.0);
}

double getRPMS()
{
    long int elapsedTime = millis() - startTime;
    if (elapsedTime == 0)
        return 0;
    return static_cast<double>(numRotations) / elapsedTime;
}

void IRAM_ATTR ISR() // IRAM_ATTR makes it put it in the ram rather than flash to make it run faster
{
    if (millis() - lastTime < 10)
        return; // Debounce
    numRotations++;
    lastTime = millis();
    rpms = getRPMS();
    if (numRotations % 250 == 0)
    {
        firstModel = !firstModel; // Toggle between the 3d models
    }
}

volatile uint8_t lastFrameNum = 0;
volatile uint8_t frameNum = 0;

void testTask(void *pvParameters)
{
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(255);

    delay(3000);

    for (;;)
    {
        uint8_t localFrameNum = frameNum;
        uint8_t localLastFrameNum = lastFrameNum;
        boolean localFirstModel = firstModel;

        if (localFrameNum != localLastFrameNum)
        {
            lastFrameNum = localFrameNum;
            if (firstModel)
            {
                FastLED.addLeds<NEOPIXEL, DATA_PIN>(cube_data[frameNum], NUM_LEDS);
                // FastLED.addLeds<NEOPIXEL, DATA_PIN>(icohedron_data[frameNum], NUM_LEDS);
            }
            else
            {
                FastLED.addLeds<NEOPIXEL, DATA_PIN>(tower_data[frameNum], NUM_LEDS);
                //     // FastLED.addLeds<NEOPIXEL, DATA_PIN>(fighter_data[frameNum], NUM_LEDS);
                //     // FastLED.addLeds<NEOPIXEL, DATA_PIN>(dino_data[frameNum], NUM_LEDS);
            }
            FastLED.show();
        }
    }
}

void setup()
{

    // set up multitasking
    xTaskCreatePinnedToCore(
        testTask,   // Task function
        "testTask", // Name
        8192,       // Stack size
        NULL,       // Parameters
        1,          // Priority
        NULL,       // Task handle
        0           // Core 0
    );

    // Reed Switch Setup
    pinMode(REED_PIN, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(REED_PIN), ISR, RISING);

    delay(3000);

    startTime = millis();
    numRotations = 0;
    rpms = getRPMS();
}

void loop()
{
    frameNum = floor(rpms * (millis() - lastTime) * 180.0);

    if (frameNum >= 180)
    {
        frameNum = 179;
    }
}