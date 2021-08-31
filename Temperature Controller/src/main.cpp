/*
   Project "Temperature Controller"
   Sources on GitHub: https://github.com/Lagodish/Temperature_Controller
   By Lagodish Tech, 2021
*/

#include <Arduino.h>
#include <pins_define.h>
#include <core.h>
#include <gui.h>
/*
void setup() {
  // Code here, to run once:
  Serial.begin(9600); //UART setup
  while(!Serial);

  xTaskCreate(Storage, "Storage", 5000, NULL, 1, NULL);
  xTaskCreate(Light, "Light", 5000, NULL, 1, NULL);
  xTaskCreate(Compressor, "Compressor", 5000, NULL, 1, NULL);
  xTaskCreate(Ventilator, "Ventilator", 5000, NULL, 1, NULL);
  xTaskCreate(Sensors, "Sensors", 5000, NULL, 1, NULL);
  xTaskCreate(GUI, "GUI", 5000, NULL, 1, NULL);

}

void loop() {
  // Code here, not run!
}
*/