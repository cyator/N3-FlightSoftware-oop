#pragma once

#include <Arduino.h>

class Checkstate
{
private:
    int state;
    float BASE_ALTITUDE;
    float MAX_ALTITUDE;
    float altitude;
    float velocity;
    int checkInflight();
    int checkApogee();
    int checkDescent();
    int checkGround();

public:
    Checkstate();
    void begin(float baseAltitude);
    int getState();
    int checkstate(float altitude, float velocity);
};
