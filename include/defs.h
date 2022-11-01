#pragma once
#include <Arduino.h>

struct SendValues
{
    uint64_t timestamp;
    float altitude;
    float ax;
    float ay;
    float az;
    float gx;
    float gy;
    float gz;
    float filtered_s;
    float filtered_v;
    float filtered_a;
    int state;
    float latitude;
    float longitude;
    float gpsAltitude;
    float temperature;
};

struct FilteredValues
{
    float displacement;
    float velocity;
    float acceleration;
};
