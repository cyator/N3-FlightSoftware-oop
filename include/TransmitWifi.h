#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include "debug.h"
#include "pitches.h"
#include "PassiveBuzzer.h"

#define SETUP_AP 0
#define SSID "S7 edge"
#define PASSWORD "almg76061"

class TransmitWifi
{
private:
    PassiveBuzzer buzzer;

public:
    TransmitWifi();
    void setup_wifi();
    void setup_AP();
};
