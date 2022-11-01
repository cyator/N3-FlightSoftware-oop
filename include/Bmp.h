#pragma once

#include <Arduino.h>
#include <Adafruit_BMP085.h>
#include "PassiveBuzzer.h"
#include "pitches.h"

#define SEA_LEVEL_PRESSURE 102400

class Bmp
{
private:
    PassiveBuzzer buzzer;
    Adafruit_BMP085 bmp;
    float altitude;
    float BASE_ALTITUDE;

public:
    Bmp();
    void begin();
    float getBaseAltitude();
    float getAltitude();
    float calculateBaseAltitude(int loopCount);
    void get_readings();
};
