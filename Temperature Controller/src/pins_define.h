#include <Arduino.h>

//Pins Config for my dev board (VineBox)
const uint8_t R = 16; 
const uint8_t G = 5;  //	outputs PWM signal at boot 
const uint8_t Bl = 17;
const uint8_t W = 18;
const uint8_t F1 = 33;
const uint8_t F2 = 27;   // ADC2 !
const uint8_t PIN_BUTTON2 = 35;   //input only DHT
const uint8_t Relay = 19;
const uint8_t Heater = 11; // NOT WORKING 	connected to the uint8_tegrated SPI flash
const uint8_t PIN_BUTTON1 = 34;  //input only EndStop
const uint8_t Comp = 32;
const uint8_t Beeper = 8;  // NOT WORKING 	connected to the uint8_tegrated SPI flash
const uint8_t PIN_BUTTON3 = 36;    //input only
const uint8_t PIN_BUTTON4 = 39;    //input only
const uint8_t ESP32_TX = 1;  //debug output at boot 
const uint8_t ESP32_RX = 22;
const uint8_t DS18B20 = 23;