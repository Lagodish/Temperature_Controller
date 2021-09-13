/*
   Project "Temperature Controller"
   Sources on GitHub: https://github.com/Lagodish/Temperature_Controller
   By Lagodish Tech, 2021
*/

#include <Arduino.h>
#include <core.h>
#include <gui.h>
#include <server.h>

SemaphoreHandle_t i2c_line;
extern bool DataReady;

void setup() {
  // Code here, to run once:
  Serial.begin(115200); //UART setup

  i2c_line = xSemaphoreCreateMutex();

  xTaskCreate(Storage, "Storage", 5000, NULL, 2, NULL);
    while (!DataReady) {vTaskDelay(100/portTICK_PERIOD_MS);}
  xTaskCreatePinnedToCore(Sensors, "Sensors", 5000, NULL, 1, NULL, 1);
  //xTaskCreatePinnedToCore(ClockRTC, "ClockRTC", 5000, NULL, 1, NULL, 1);
  xTaskCreate(Light, "Light", 5000, NULL, 1, NULL);
  xTaskCreate(Operate, "Compressor", 5000, NULL, 1, NULL);
  xTaskCreate(Defreeze, "Defreeze", 5000, NULL, 1, NULL);
  xTaskCreate(Additional, "Additional", 5000, NULL, 1, NULL);
  xTaskCreate(Check, "Check", 5000, NULL, 1, NULL);
  xTaskCreatePinnedToCore(Buttons, "Buttons", 5000, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(GUI, "GUI", 5000, NULL, 1, NULL, 1);
  xTaskCreate(WebServer, "WebServer", 5000, NULL, 1, NULL);

  //xTaskCreatePinnedToCore(WebServer, "WebServer", 10000, NULL, 2, NULL,  1); 

}

void loop() {
vTaskDelete(NULL);  
  // Code here, not run!
}
