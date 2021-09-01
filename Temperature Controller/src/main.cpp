/*
   Project "Temperature Controller"
   Sources on GitHub: https://github.com/Lagodish/Temperature_Controller
   By Lagodish Tech, 2021
*/

#include <Arduino.h>
#include <core.h>
#include <gui.h>
#include <server.h>


void setup() {
  // Code here, to run once:
  Serial.begin(115200); //UART setup
  while(!Serial);

  xTaskCreate(Storage, "Storage", 5000, NULL, 1, NULL);
  xTaskCreate(Light, "Light", 5000, NULL, 1, NULL);
  xTaskCreate(Compressor, "Compressor", 5000, NULL, 1, NULL);
  xTaskCreate(Ventilator, "Ventilator", 5000, NULL, 1, NULL);
  xTaskCreate(Sensors, "Sensors", 5000, NULL, 1, NULL);
  xTaskCreate(GUI, "GUI", 5000, NULL, 2, NULL);
  //xTaskCreatePinnedToCore(GUI, "GUI", 10000, NULL, 1, NULL,  0); 
  xTaskCreate(WebServer, "WebServer", 5000, NULL, 1, NULL);

}

void loop() {
  // Code here, not run!
}
