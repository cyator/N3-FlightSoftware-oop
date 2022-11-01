#include "TransmitWifi.h"

// constructor

TransmitWifi::TransmitWifi()
{
}

void TransmitWifi::setup_wifi()
{
    // We start by connecting to a WiFi network
    debugln();
    debug("Connecting to ");
    debug(SSID);
    debugln();

    WiFi.begin(SSID, PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        buzzer.beep(NOTE_A5, 500);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        Serial.print(".");
    }

    debugln("");
    debugln("WiFi connected");
    debugln("IP address: ");
    debugln(WiFi.localIP());
}

void TransmitWifi::setup_AP()
{
    debugln();
    debugln("Configuring access point...");

    // You can remove the password parameter if you want the AP to be open.
    WiFi.softAP(SSID, PASSWORD);
    IPAddress myIP = WiFi.softAPIP();
    debug("AP IP address: ");
    debugln(myIP);
}
