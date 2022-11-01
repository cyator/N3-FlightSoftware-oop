#include <Arduino.h>

#include "../include/defs.h"
#include "../include/debug.h"
#include "../include/PassiveBuzzer.h"
#include "../include/TransmitWifi.h"
#include "../include/Mqtt.h"
#include "../include/Mpu.h"
#include "../include/Bmp.h"
#include "../include/Gps.h"
#include "../include/Checkstate.h"
#include "../include/KalmanFilter.h"

// define tasks
void TaskHandleSensors(void *pvParameters);
void TaskHandleTransmit(void *pvParameters);
void TaskHandleSDWrite(void *pvParameters);

// define queue
static uint16_t wifi_queue_length = 100;
static QueueHandle_t wifi_telemetry_queue;

// inits
TransmitWifi txwifi;
Mqtt mqtt;
Mpu mpu;
Bmp bmp;
Gps gps;
Checkstate checkstate;
PassiveBuzzer buzzer;
KalmanFilter kalmanfilter;

// the setup function runs once when you press reset or power the board
void setup()
{

  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS); // wait for serial monitor to open

  // init buzzer
  buzzer.begin();

  // init sensors
  mpu.begin();
  bmp.begin();
  gps.begin();
  kalmanfilter.begin();

// setup wifi
#if SETUP_AP == 1
  txwifi.setup_AP();
#else
  txwifi.setup_wifi();
#endif

  // init mqtt
  mqtt.begin();

  // init checkstate
  float baseAltitude = bmp.calculateBaseAltitude(10);
  checkstate.begin(baseAltitude);

  // queue
  wifi_telemetry_queue = xQueueCreate(wifi_queue_length, sizeof(SendValues));

  if (wifi_telemetry_queue == NULL)
  {
    debugln("Error creating wifi telemetry queue");
  }

  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(
      TaskHandleSensors, "TaskHandleSensors" // A name just for humans
      ,
      2048 // This stack size can be checked & adjusted by reading the Stack Highwater
      ,
      NULL, 2 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
      ,
      NULL, 0);

  xTaskCreatePinnedToCore(
      TaskHandleTransmit, "TaskHandleTransmit", 3072 // Stack size
      ,
      NULL, 1 // Priority
      ,
      NULL, 1);

  debugln("setup complete");

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.

  // delete setup and loop task
  vTaskDelete(NULL);
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskHandleSensors(void *pvParameters) // This is a task.
{
  (void)pvParameters;
  UBaseType_t uxHighWaterMark;
  static int droppedWiFiPackets = 0;

  struct SendValues sv;
  struct FilteredValues filtered_values;

  uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
  debugln();
  debug("TaskHandleSensorsStack: ");
  debugln(uxHighWaterMark);

  for (;;) // A Task shall never return or exit.
  {
    bmp.get_readings();
    sv.altitude = bmp.getAltitude();
    mpu.get_readings();

    sv.ax = mpu.getAx();
    sv.ay = mpu.getAy();
    sv.az = mpu.getAz();
    sv.gx = mpu.getGx();
    sv.gy = mpu.getGy();
    sv.gz = mpu.getGz();
    sv.temperature = mpu.getTemp();

    gps.get_gps_readings();
    sv.latitude = gps.getLatitude();
    sv.longitude = gps.getLongitude();

    // TODO: very important to know the orientation of the altimeter
    filtered_values = kalmanfilter.update(sv);

    sv.filtered_a = filtered_values.acceleration;
    sv.filtered_s = filtered_values.displacement;
    sv.filtered_v = filtered_values.velocity;

    checkstate.checkstate(sv.altitude, sv.filtered_v);
    sv.state = checkstate.getState();
    sv.timestamp = millis();

    if (xQueueSend(wifi_telemetry_queue, (void *)&sv, 0) != pdTRUE)
    {
      droppedWiFiPackets++;
    }

    debugf("Dropped WiFi Packets : %d\n", droppedWiFiPackets);

    vTaskDelay(50 / portTICK_PERIOD_MS);
    uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    debugln();
    debug("TaskHandleSensorsStack: ");
    debugln(uxHighWaterMark);
  }
}

void TaskHandleTransmit(void *pvParameters) // This is a task.
{
  (void)pvParameters;

  UBaseType_t uxHighWaterMark;

  uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
  debugln();
  debug("TaskHandleTransmitStack: ");
  debugln(uxHighWaterMark);

  struct SendValues sv;

  for (;;)
  {
    xQueueReceive(wifi_telemetry_queue, (void *)&sv, 0);

    mqtt.sendTelemetryWiFi(sv);

    vTaskDelay(50 / portTICK_PERIOD_MS);

    uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    debugln();
    debug("TaskHandleTransmitStack: ");
    debugln(uxHighWaterMark);
  }
}
