#include <Arduino.h>

void GUI( void * parameter)
{
    Serial.println("GUI");

    /*    #1 GUI oled control    */

    while(1){

    vTaskDelay(5000/portTICK_PERIOD_MS);

    }
    vTaskDelete( NULL );
}