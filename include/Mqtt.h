#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "debug.h"
#include "defs.h"
#include "PassiveBuzzer.h"
#include "pitches.h"

#define MQTT_PACKET_SIZE 300
#define EJECTION_PIN 13
#define MQTT_SERVER "192.168.43.104"
#define MQTT_PORT 1883

class Mqtt
{
private:
    WiFiClient espClient;
    PubSubClient client{espClient};
    PassiveBuzzer buzzer;

public:
    Mqtt();
    void begin();
    static void mqttCallback(char *topic, byte *message, unsigned int length);
    void reconnect();
    void sendTelemetryWiFi(SendValues sv);
};
