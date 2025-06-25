#include <Arduino.h>
#include <FastLED.h>
// #include <NimBLEDevice.h>
#include "cube_data.h"
#include "icohedron_data.h"
#include "tower_data.h"
#include "fighter_data.h"
#include "dino_data.h"

// NimBLECharacteristic *pCharacteristic;

#define REED_PIN 4
#define NUM_LEDS 256
#define DATA_PIN 1

// Define the array of leds
CRGB leds[NUM_LEDS];

long int numRotations = 0;
long int startTime;
long int lastTime = 0;

double rpms;

boolean firstModel = true;

double getRPM()
{
    long int elapsedTime = millis() - startTime;
    if (elapsedTime == 0)
        return 0;
    return (double(numRotations) / double(elapsedTime) * 60000.0); // Convert to minutes
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
        firstModel = !firstModel; // Toggle between cmodels
    }
}

// class MyBLECallbacks : public NimBLECharacteristicCallbacks
// {
//     void onWrite(NimBLECharacteristic *pCharacteristic)
//     {
//         std::string value = pCharacteristic->getValue();
//         Serial.print("Received BLE message: ");
//         Serial.println(value.c_str());
//     }
// };

// void bleTask(void *pvParameters)
// {
//     NimBLEDevice::init("VolumetricDisplay");
//     NimBLEServer *pServer = NimBLEDevice::createServer();

//     NimBLEService *pService = pServer->createService("12345678-1234-1234-1234-123456789abc");
//     NimBLECharacteristic *pCharacteristic = pService->createCharacteristic(
//         "abcdefab-1234-1234-1234-abcdefabcdef",
//         NIMBLE_PROPERTY::WRITE);

//     // Use the named callback class
//     pCharacteristic->setCallbacks(new MyBLECallbacks());

//     pService->start();
//     NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
//     pAdvertising->start();

//     // BLE loop
//     while (1)
//     {
//         vTaskDelay(1000 / portTICK_PERIOD_MS); // Keep task alive
//     }
// }

void setup()
{
    Serial.begin(115200);
    // xTaskCreatePinnedToCore(
    //     bleTask,   // Task function
    //     "bleTask", // Name
    //     8192,      // Stack size
    //     NULL,      // Parameters
    //     1,         // Priority
    //     NULL,      // Task handle
    //     0          // Core 0
    // );

    // REED SWITCH
    pinMode(REED_PIN, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(REED_PIN), ISR, RISING);

    // LEDS
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(255);

    delay(3000);

    startTime = millis();
    numRotations = 0;
    rpms = getRPMS();
}

uint8_t lastFrameNum = 0;
uint8_t frameNum = 0;

void loop()
{
    frameNum = floor(rpms * (millis() - lastTime) * 180.0);

    if (frameNum >= 180)
    {
        frameNum = 179;
    }

    if (frameNum != lastFrameNum)
    {
        if (firstModel)
        {
            FastLED.addLeds<NEOPIXEL, DATA_PIN>(cube_data[frameNum], NUM_LEDS);
            // FastLED.addLeds<NEOPIXEL, DATA_PIN>(icohedron_data[frameNum], NUM_LEDS);
        }
        else
        {
            FastLED.addLeds<NEOPIXEL, DATA_PIN>(tower_data[frameNum], NUM_LEDS);
            // FastLED.addLeds<NEOPIXEL, DATA_PIN>(fighter_data[frameNum], NUM_LEDS);
            // FastLED.addLeds<NEOPIXEL, DATA_PIN>(dino_data[frameNum], NUM_LEDS);
        }
        FastLED.show();
        lastFrameNum = frameNum;
    }
}