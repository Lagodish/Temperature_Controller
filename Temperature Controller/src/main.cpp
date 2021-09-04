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

void setup() {
  // Code here, to run once:
  Serial.begin(115200); //UART setup

  i2c_line = xSemaphoreCreateMutex();
  xTaskCreate(Storage, "Storage", 10000, NULL, 2, NULL);
  xTaskCreate(Sensors, "Sensors", 8000, NULL, 1, NULL);
  xTaskCreate(ClockRTC, "ClockRTC", 7000, NULL, 1, NULL);
  xTaskCreate(Light, "Light", 5000, NULL, 1, NULL);
  xTaskCreate(Compressor, "Compressor", 5000, NULL, 1, NULL);
  xTaskCreate(Ventilator, "Ventilator", 5000, NULL, 1, NULL);
  xTaskCreate(Additional, "Additional", 5000, NULL, 1, NULL);
  xTaskCreate(GUI, "GUI", 10000, NULL, 1, NULL);
  //xTaskCreate(WebServer, "WebServer", 7000, NULL, 1, NULL);

  xTaskCreatePinnedToCore(WebServer, "WebServer", 10000, NULL, 2, NULL,  1); 

}

void loop() {
  // Code here, not run!
}
