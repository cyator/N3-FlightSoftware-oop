#include "PassiveBuzzer.h"

PassiveBuzzer::PassiveBuzzer()
{
}
void PassiveBuzzer::begin()
{
    // configure LED PWM functionalitites
    ledcSetup(ledChannel, freq, resolution);
    // attach the channel to the GPIO to be controlled
    ledcAttachPin(BUZZER_PIN, ledChannel);
}
void PassiveBuzzer::beep(int note, int duration)
{
    if (SOUND_TONE == 1)
    {
        ledcWriteTone(ledChannel, note);
        vTaskDelay(duration / portTICK_PERIOD_MS);
        ledcWriteTone(ledChannel, 0);
    }
}