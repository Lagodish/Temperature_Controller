#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "RTClib.h"
#include <Preferences.h>
#include <pins_define.h>

// Var.
float tempC = 0.0f;

double TargetTemp = 0.0;

double TempSensor_0 = 0.0;
double TempSensor_1 = 0.0;
double TempSensor_2 = 0.0;
double TempSensor_3 = 0.0;
double TempSensor_4 = 0.0;
double CalibTemp_0 = 0.0;
double CalibTemp_1 = 0.0;
double CalibTemp_2 = 0.0;
double CalibTemp_3 = 0.0;
double CalibTemp_4 = 0.0;

int contrast = 0;
int numberOfDevices = 0;
int MinWorkComp = 0;
int MinDefreeze = 0;
int DisplayUIFlag = 0;
uint8_t R_brightness = 0;
uint8_t G_brightness = 0;
uint8_t B_brightness = 0;
uint8_t W_brightness = 0;
uint8_t HourNow = 0;
uint8_t MinNow = 0;

bool FanFlag = false;
bool DefreezeFlag = false;
bool RelayFlag = false;
bool CompressorFlag = false;
bool LightFlag = false;
bool LockFlag = false;
bool TenthsFlag = true;

bool Warning = false;
bool Debug = false;

// Extern.
extern SemaphoreHandle_t i2c_line;

// Dev.
OneWire oneWire(DS18B20);
DallasTemperature sensors(&oneWire);
DeviceAddress tempDeviceAddress; 
Preferences preferences;
RTC_DS3231 rtc;

// function to print a device address
void printAddress(DeviceAddress deviceAddress);

void Storage( void * parameter)
{
    if(Debug) Serial.println("Storage task start!");

    /*    #1 Get data from flash storage    */

    preferences.begin("data-space", false);

    TargetTemp = preferences.getDouble("TargetTemp", 10.0);
    CalibTemp_0 = preferences.getDouble("CalibTemp_0", 0.0);
    CalibTemp_1 = preferences.getDouble("CalibTemp_1", 0.0);
    CalibTemp_2 = preferences.getDouble("CalibTemp_2", 0.0);
    CalibTemp_3 = preferences.getDouble("CalibTemp_3", 0.0);
    CalibTemp_4 = preferences.getDouble("CalibTemp_4", 0.0);
    contrast = preferences.getInt("contrast", 110);
    MinWorkComp = preferences.getInt("MinWorkComp", 30);
    MinDefreeze = preferences.getInt("MinDefreeze", 10);
    R_brightness = preferences.getInt("R_brightness", 0);
    G_brightness = preferences.getInt("G_brightness", 0);
    B_brightness = preferences.getInt("B_brightness", 0);
    W_brightness = preferences.getInt("W_brightness", 255);
    DisplayUIFlag = preferences.getInt("DisplayUIFlag", 1);
    LockFlag = preferences.getBool("LockFlag", false);
    TenthsFlag = preferences.getBool("TenthsFlag", true);

    preferences.end();
    vTaskDelay(100/portTICK_PERIOD_MS);

    double TargetTemp_Old = TargetTemp;
    double CalibTemp_0_Old = CalibTemp_0;
    double CalibTemp_1_Old = CalibTemp_1;
    double CalibTemp_2_Old = CalibTemp_2;
    double CalibTemp_3_Old = CalibTemp_3;
    double CalibTemp_4_Old = CalibTemp_4;
    int contrast_Old = contrast;
    int MinWorkComp_Old = MinWorkComp;
    int MinDefreeze_Old = MinDefreeze;
    int DisplayUIFlag_old = DisplayUIFlag;
    uint8_t R_brightness_Old = R_brightness;
    uint8_t G_brightness_Old = G_brightness;
    uint8_t B_brightness_Old = B_brightness;
    uint8_t W_brightness_Old = W_brightness;
    bool LockFlag_Old = LockFlag;
    bool TenthsFlag_old = TenthsFlag;
    
    while(1){
        
        //any change in var 
        if((LockFlag_Old != LockFlag)||(contrast_Old != contrast)||(R_brightness_Old != R_brightness)||(G_brightness_Old != G_brightness)||
        (B_brightness_Old != B_brightness)||(W_brightness_Old != W_brightness)||(MinWorkComp_Old != MinWorkComp)||(MinDefreeze_Old != MinDefreeze)||
        (abs(TargetTemp_Old-TargetTemp)>0.1)||(abs(CalibTemp_0_Old - CalibTemp_0)>0.01)||(TenthsFlag_old != TenthsFlag)||(DisplayUIFlag_old != DisplayUIFlag)||
        (abs(CalibTemp_1_Old - CalibTemp_1)>0.01)||(abs(CalibTemp_2_Old - CalibTemp_2)>0.01)
        ||(abs(CalibTemp_3_Old - CalibTemp_3)>0.01)||(abs(CalibTemp_4_Old - CalibTemp_4)>0.01)){

            TargetTemp_Old = TargetTemp;
            CalibTemp_0_Old = CalibTemp_0;
            CalibTemp_1_Old = CalibTemp_1;
            CalibTemp_2_Old = CalibTemp_2;
            CalibTemp_3_Old = CalibTemp_3;
            CalibTemp_4_Old = CalibTemp_4;
            contrast_Old = contrast;
            MinWorkComp_Old = MinWorkComp;
            MinDefreeze_Old = MinDefreeze;
            R_brightness_Old = R_brightness;
            G_brightness_Old = G_brightness;
            B_brightness_Old = B_brightness;
            W_brightness_Old = W_brightness;
            LockFlag_Old = LockFlag;
            TenthsFlag_old = TenthsFlag;
            DisplayUIFlag_old = DisplayUIFlag;
            
            preferences.begin("data-space", false);
            
            preferences.putDouble("TargetTemp", TargetTemp);
            preferences.putDouble("CalibTemp_0", CalibTemp_0);
            preferences.putDouble("CalibTemp_1", CalibTemp_1);
            preferences.putDouble("CalibTemp_2", CalibTemp_2);
            preferences.putDouble("CalibTemp_3", CalibTemp_3);
            preferences.putDouble("CalibTemp_4", CalibTemp_4);
            preferences.putInt("contrast", contrast);
            preferences.putInt("MinWorkComp", MinWorkComp);
            preferences.putInt("MinDefreeze", MinDefreeze);
            preferences.putInt("R_brightness", R_brightness);
            preferences.putInt("G_brightness", G_brightness);
            preferences.putInt("B_brightness", B_brightness);
            preferences.putInt("DisplayUIFlag", DisplayUIFlag);
            preferences.putInt("W_brightness", W_brightness);
            preferences.putBool("LockFlag", LockFlag);
            preferences.putBool("TenthsFlag", TenthsFlag);
            

            preferences.end();

            if(Debug) Serial.println("Data Updated!");
        }

        vTaskDelay(5000/portTICK_PERIOD_MS);

    }

    vTaskDelete( NULL );
}

void Light( void * parameter)
{
    if(Debug) Serial.println("Light task start!");

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

        if((R_brightness==0)&&(G_brightness==0)&&(B_brightness==0)&&(W_brightness==0)){LightFlag = false;}
        else{LightFlag = true;}
        
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
    if(Debug) Serial.println("Compressor task start!");
    
    /*    #1 Compressor freq. on/off control    */
    pinMode (Comp, OUTPUT);
    while(1){
        if(TargetTemp<tempC) CompressorFlag = true;
        else CompressorFlag = false;
        
        digitalWrite(Comp, CompressorFlag);

        vTaskDelay(5000/portTICK_PERIOD_MS);

    }

    vTaskDelete( NULL );
}

void Ventilator( void * parameter)
{
    if(Debug) Serial.println("Ventilator task start!");

    /*    #1 Ventilator(s) speed(?) or on/off control    */

    pinMode (F1, OUTPUT);
    pinMode (Relay, OUTPUT);
    while(1){
        
        //TODO if(defreze)
        digitalWrite(F1, FanFlag);

        //Relay controll
        digitalWrite(Relay, RelayFlag);

        vTaskDelay(5000/portTICK_PERIOD_MS);

    }
    
    vTaskDelete( NULL );
}

void Sensors( void * parameter)
{
    if(Debug) Serial.println("Sensors task start!");

    /*    #1 Open door switch get data    #2 Temp sersor(s) ds18b20 get data    */

    sensors.begin();
    sensors.setResolution(10);
    // Grab a count of devices on the wire
    numberOfDevices = 4;                                // !!!!!!!!!!!!!!!!!!!!!!!TODO WEB Number for sensors!
    if(Debug) Serial.println(numberOfDevices);
    // Loop through each device, print out address
    for(int i=0;i<=numberOfDevices; i++){
        // Search the wire for address
        if(sensors.getAddress(tempDeviceAddress, i)){
        if(Debug) Serial.print("Found device ");
        if(Debug) Serial.print(i, DEC);
        if(Debug) Serial.print(" with address: ");
        printAddress(tempDeviceAddress);
        if(Debug) Serial.println();
        } else {
        if(Debug) Serial.print("Found ghost device at ");
        if(Debug) Serial.print(i, DEC);
        if(Debug) Serial.print(" but could not detect address. Check power and cabling");
        }
    }

    while(1){
        sensors.requestTemperatures(); // Send the command to get temperatures
        // Loop through each device, print out temperature data
        for(int i=0;i<=numberOfDevices; i++){
            // Search the wire for address
            double temp = double(sensors.getTempCByIndex(i));
                if((temp>-50.0f)&&(temp<90.0f)){
                    tempC = temp;

                    if(i==0) TempSensor_0 = temp;
                    if(i==1) TempSensor_1 = temp;
                    if(i==2) TempSensor_2 = temp;
                    if(i==3) TempSensor_3 = temp;
                    if(i==4) TempSensor_4 = temp;
                }
                else{ if(Debug) Serial.println("Temp Sensor Error #1"); Warning = true;}             

            }
        vTaskDelay(10000/portTICK_PERIOD_MS);
        
    }
    vTaskDelete( NULL );
}

void Additional( void * parameter)
{
    if(Debug) Serial.println("Additional task start!");

    /*    #1 Additional tasks    */

    while(1){
        
        vTaskDelay(5000/portTICK_PERIOD_MS);

    }
    
    vTaskDelete( NULL );
}

void ClockRTC( void * parameter)
{
    if(Debug) Serial.println("ClockRTC task start!");

    /*    #1 Additional tasks    */
    xSemaphoreTake(i2c_line, portMAX_DELAY);
    Wire.setPins(21,22);
    Wire.begin(21,22,400000);
    if (! rtc.begin()) {
    Warning = true;
    Serial.println("Error RTC!");
    }
 
    if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    Warning = true;
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    //rtc.adjust(DateTime(2021, 9, 4, 20, 0, 0));
    }

    xSemaphoreGive(i2c_line);

    while(1){
        xSemaphoreTake(i2c_line, portMAX_DELAY);
        DateTime now = rtc.now();
        HourNow = now.hour();
        MinNow = now.minute();
        xSemaphoreGive(i2c_line);
        vTaskDelay(5000/portTICK_PERIOD_MS);
    }
    
    vTaskDelete( NULL );
}

void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++){
    if (deviceAddress[i] < 16) if(Debug) Serial.print("0");
      if(Debug) Serial.print(deviceAddress[i], HEX);
  }
}