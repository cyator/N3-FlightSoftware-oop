#include "Mqtt.h"

Mqtt::Mqtt()
{
}

void Mqtt::begin()
{
    // set up parachute pin
    pinMode(EJECTION_PIN, OUTPUT);
    // configure pubsub client
    client.setBufferSize(MQTT_PACKET_SIZE);
    client.setServer(MQTT_SERVER, MQTT_PORT);
    client.setCallback(mqttCallback);

    if (!client.connected())
    {
        reconnect();
    }
    client.loop();
}

void Mqtt::mqttCallback(char *topic, byte *message, unsigned int length)
{
    debug("Message arrived on topic: ");
    debug(topic);
    debug(". Message: ");

    String messageTemp;

    for (int i = 0; i < length; i++)
    {
        debug((char)message[i]);
        messageTemp += (char)message[i];
    }
    debugln();

    if (String(topic) == "esp32/ejection")
    {
        debug("Changing output to ");
        if (messageTemp == "on")
        {
            debug("on");
            digitalWrite(EJECTION_PIN, HIGH);
        }
        else if (messageTemp == "off")
        {
            debug("off");
            digitalWrite(EJECTION_PIN, LOW);
        }
    }
}

void Mqtt::reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        debugln("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("ESP32Client"))
        {
            debugln("Mqtt connected");
            // Subscribe
            bool subscriptionStatus = client.subscribe("esp32/ejection");
            if (subscriptionStatus)
            {
                debugln("subscribed to esp32/ejection");
            }
            else
            {
                debugln("Failed to subscribe to esp32/ejection");
            }
        }
        else
        {
            debug("failed, rc=");
            debug(client.state());
            debugln(" try again in 50 milliseconds");
            buzzer.beep(NOTE_A5, 100);
            // Wait 50ms seconds before retrying
            vTaskDelay(50 / portTICK_PERIOD_MS);
        }
    }
}

void Mqtt::sendTelemetryWiFi(SendValues sv)
{

    // publish json message
    char mqttMessage[300];
    sprintf(mqttMessage, "{\"timestamp\":%lld,\"altitude\":%.3f,\"temperature\":%.3f,\"ax\":%.3f,\"ay\":%.3f,\"az\":%.3f,\"gx\":%.3f,\"gy\":%.3f,\"gz\":%.3f,\"filtered_s\":%.3f,\"filtered_v\":%.3f,\"filtered_a\":%.3f,\"state\":%d,\"longitude\":%.8f,\"latitude\":%.8f}", sv.timestamp, sv.altitude, sv.temperature, sv.ax, sv.ay, sv.az, sv.gx, sv.gy, sv.gz, sv.filtered_s, sv.filtered_v, sv.filtered_a, sv.state, sv.longitude, sv.latitude);
    client.publish("esp32/message", mqttMessage);

    debugln(mqttMessage);
}
