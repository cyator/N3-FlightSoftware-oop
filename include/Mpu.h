#pragma once

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include "pitches.h"
#include "debug.h"
#include "PassiveBuzzer.h"

class Mpu
{

private:
    PassiveBuzzer buzzer;
    Adafruit_MPU6050 mpu;
    float ax;
    float ay;
    float az;
    float gx;
    float gy;
    float gz;
    float temperature;

public:
    Mpu();
    float getAx();
    float getAy();
    float getAz();
    float getGx();
    float getGy();
    float getGz();
    float getTemp();
    void begin();
    void get_readings();
};
