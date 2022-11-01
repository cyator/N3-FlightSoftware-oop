#include "Gps.h"

Gps::Gps()
{
}

// getters

float Gps::getLongitude()
{
    return longitude;
}

float Gps::getLatitude()
{
    return latitude;
}

void Gps::begin()
{
    Serial2.begin(GPSBaud, SERIAL_8N1, RX_PIN, TX_PIN);
    while (!Serial2.available())
    {
        Serial.println("Waiting for GPS Serial ...");
        buzzer.beep(NOTE_A5, 500);
        vTaskDelay(250 / portTICK_PERIOD_MS);
        buzzer.beep(NOTE_B5, 500);
        vTaskDelay(250 / portTICK_PERIOD_MS);
        buzzer.beep(NOTE_C5, 500);
        vTaskDelay(250 / portTICK_PERIOD_MS);
        buzzer.beep(NOTE_D5, 500);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void Gps::get_gps_readings()
{
    debugln();
    debug("reading gps");
    while (Serial2.available())
    {
        if (tinygps.encode(Serial2.read()))
        {
            if (tinygps.location.isValid())
            {
                latitude = tinygps.location.lat();
                longitude = tinygps.location.lng();
                debugln();
                debug("latitude: ");
                debugln(latitude);
                debug("longitude: ");
                debugln(longitude);
            }
            else
            {
                debugln();
                debugln("gps not valid");
            }
        }
    }
}