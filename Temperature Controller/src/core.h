#include <Arduino.h>

void Storage( void * parameter)
{
    Serial.println("Storage task start!");

    /*    #1 Get data from flash storage    */

    while(1){

        vTaskDelay(5000/portTICK_PERIOD_MS);

    }

    vTaskDelete( NULL );
}

void Light( void * parameter)
{
    Serial.println("Light task start!");

    /*    #1 RGB led conrtol    #2 additional WRGB led    */

    while(1){

        vTaskDelay(5000/portTICK_PERIOD_MS);

    }

    vTaskDelete( NULL );
}

void Compressor( void * parameter)
{
    Serial.println("Compressor task start!");
    
    /*    #1 Compressor freq. on/off control    */

    while(1){

        vTaskDelay(5000/portTICK_PERIOD_MS);

    }

    vTaskDelete( NULL );
}

void Ventilator( void * parameter)
{
    Serial.println("Ventilator task start!");

    /*    #1 Ventilator(s) speed(?) or on/off control    */

    while(1){

        vTaskDelay(5000/portTICK_PERIOD_MS);

    }
    
    vTaskDelete( NULL );
}

void Sensors( void * parameter)
{
    Serial.println("Sensors task start!");

    /*    #1 Open door switch get data    #2 Temp sersor(s) ds18b20 get data    */

    while(1){

        vTaskDelay(5000/portTICK_PERIOD_MS);

    }

    vTaskDelete( NULL );
}
