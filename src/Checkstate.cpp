#include "Checkstate.h"

// constructor
Checkstate::Checkstate()
{
    state = 0;
}
void Checkstate::begin(float baseAltitude)
{
    BASE_ALTITUDE = baseAltitude;
}
int Checkstate::getState()
{
    return state;
}
int Checkstate::checkInflight()
{
    float displacement = altitude - BASE_ALTITUDE;
    if (displacement > 20)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int Checkstate::checkApogee()
{

    if (velocity < 0)
    {
        MAX_ALTITUDE = altitude;
        return 2;
    }
    else
    {
        return 1;
    }
}

int Checkstate::checkDescent()
{
    float displacement = altitude - MAX_ALTITUDE;
    if (displacement < 20)
    {
        return 3;
    }
    else
    {
        return 2;
    }
}

int Checkstate::checkGround()
{
    float displacement = altitude - BASE_ALTITUDE;
    if (displacement < 20)
    {
        return 4;
    }
    else
    {
        return 3;
    }
}

int Checkstate::checkstate(float altitude, float velocity)
{

    this->altitude = altitude;
    this->velocity = velocity;

    switch (state)
    {
    case 0:
        state = checkInflight();
        break;
    case 1:
        state = checkApogee();
        break;
    case 2:
        state = checkDescent();
        break;
    case 3:
        state = checkGround();
        break;
    default:
        break;
    }
    return state;
}
