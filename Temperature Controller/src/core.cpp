#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <pins_define.h>

// Globals
float tempC = 0.0f;
uint8_t R_brightness = 0;
uint8_t G_brightness = 0;
uint8_t B_brightness = 0;
uint8_t W_brightness = 0;

OneWire oneWire(DS18B20);
DallasTemperature sensors(&oneWire);
int numberOfDevices;
DeviceAddress tempDeviceAddress; 

// function to print a device address
void printAddress(DeviceAddress deviceAddress);

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
    //PWM setup
    const int freq = 10000;
    const uint8_t resolution = 8;

    

    ledcSetup(1, freq, resolution);
    ledcSetup(2, freq, resolution);
    ledcSetup(3, freq, resolution);
    ledcSetup(4, freq, resolution);

    ledcAttachPin(R, 1);
    ledcAttachPin(G, 2);
    ledcAttachPin(Bl, 3);
    ledcAttachPin(W, 4);

    while(1){

        ledcWrite(1, R_brightness);
        ledcWrite(2, G_brightness);
        ledcWrite(3, B_brightness);
        ledcWrite(4, W_brightness);
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

    sensors.begin();
    // Grab a count of devices on the wire
    numberOfDevices = sensors.getDeviceCount()+1;
    
    // locate devices on the bus
    Serial.print("Locating devices...");
    Serial.print("Found ");
    Serial.print(numberOfDevices, DEC);
    Serial.println(" devices.");

    // Loop through each device, print out address
    for(int i=0;i<numberOfDevices; i++){
        // Search the wire for address
        if(sensors.getAddress(tempDeviceAddress, i)){
        Serial.print("Found device ");
        Serial.print(i, DEC);
        Serial.print(" with address: ");
        printAddress(tempDeviceAddress);
        Serial.println();
        } else {
        Serial.print("Found ghost device at ");
        Serial.print(i, DEC);
        Serial.print(" but could not detect address. Check power and cabling");
        }
    }

    while(1){
          sensors.requestTemperatures(); // Send the command to get temperatures
  
        // Loop through each device, print out temperature data
        for(int i=0;i<numberOfDevices; i++){
            // Search the wire for address
            if(sensors.getAddress(tempDeviceAddress, i)){
            // Output the device ID
            Serial.print("Temperature for device: ");
            Serial.println(i,DEC);
            // Print the data
            float temp = sensors.getTempC(tempDeviceAddress);
            if((temp>-50.0f)&&(temp<50.0f)){
                tempC = temp;
            }
            Serial.print("Temp C: ");
            Serial.print(tempC);
            Serial.print(" Temp F: ");
            Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit
            }
        }
        vTaskDelay(10000/portTICK_PERIOD_MS);

    }

    vTaskDelete( NULL );
}

void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++){
    if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
  }
}
