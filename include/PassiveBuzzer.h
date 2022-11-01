#pragma once

#include <Arduino.h>

#define BUZZER_PIN 2
#define SOUND_TONE 1

class PassiveBuzzer
{
private:
    static const int freq = 5000;
    static const int ledChannel = 0;
    static const int resolution = 8;

public:
    PassiveBuzzer();
    void begin();
    void beep(int note, int duration);
};
