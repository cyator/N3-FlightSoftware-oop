#pragma once

#include <Arduino.h>
#include <TinyGPSPlus.h>
#include "HardwareSerial.h"
#include "debug.h"
#include "PassiveBuzzer.h"
#include "pitches.h"

#define TX_PIN 17
#define RX_PIN 16

class Gps
{

private:
    PassiveBuzzer buzzer;
    TinyGPSPlus tinygps;
    byte RXPin;
    byte TXPin;
    static const uint32_t GPSBaud = 9600;
    float latitude;
    float longitude;

public:
    Gps();
    float getLongitude();
    float getLatitude();
    void begin();
    void get_gps_readings();
};
