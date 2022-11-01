#include "Bmp.h"

Bmp::Bmp()
{
}

void Bmp::begin()
{
    if (!bmp.begin())
    {
        Serial.println("Could not find a valid BMP085 sensor, check wiring!");
        while (1)
        {
            buzzer.beep(NOTE_A5, 500);
            vTaskDelay(250 / portTICK_PERIOD_MS);
            buzzer.beep(NOTE_B5, 500);
            vTaskDelay(250 / portTICK_PERIOD_MS);
            buzzer.beep(NOTE_C5, 500);
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }
    }
}

float Bmp::getAltitude()
{
    return altitude;
}

float Bmp::getBaseAltitude()
{
    return BASE_ALTITUDE;
}

float Bmp::calculateBaseAltitude(int loopCount)
{
    // average because of variation
    // FixMe: use a filtered altitude
    float sum = 0;

    for (int i = 0; i < loopCount; i++)
    {
        get_readings();
        sum += getAltitude();
    }
    float average = sum / loopCount;
    BASE_ALTITUDE = average;
    return BASE_ALTITUDE;
}

void Bmp::get_readings()
{
    altitude = bmp.readAltitude(SEA_LEVEL_PRESSURE);
}