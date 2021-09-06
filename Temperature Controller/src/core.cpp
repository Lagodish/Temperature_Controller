#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "RTClib.h"
#include <Preferences.h>
#include <pins_define.h>

// Var.
double tempC = 0.0;

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

double Zone_1 = 0.0;
double Evaporator_1 = 0.0;
double Zone_2 = 0.0;
double Evaporator_2 = 0.0;

int TempSensorLocation_0 = 0;
int TempSensorLocation_1 = 0;
int TempSensorLocation_2 = 0;
int TempSensorLocation_3 = 0;
int TempSensorLocation_4 = 0;

int contrast = 0;
int numberOfDevices = 0;
int DefrostWorkTime = 0;
int MinDefreeze = 0;
int DisplayUIFlag = 0;
uint8_t R_brightness = 0;
uint8_t G_brightness = 0;
uint8_t B_brightness = 0;
uint8_t W_brightness = 0;
uint8_t HourNow = 0;
uint8_t MinNow = 0;
int warnCount = 0;

bool FanFlag = false;
bool DefreezeFlag = false;
bool RelayFlag = false;
bool CompressorFlag = false;
bool LightFlag = false;
bool LockFlag = false;
bool TenthsFlag = true;

bool Warning = false;
bool Debug = false;
bool ChangesToSaveFlag = false; //TODO Logic use!
bool ManualMode = false;

// Extern.
extern SemaphoreHandle_t i2c_line;

// Dev.
OneWire oneWire(DS18B20);
DallasTemperature sensors(&oneWire);
DeviceAddress tempDeviceAddress; 
Preferences preferences;
RTC_DS3231 rtc;

// function to print a device address

void Storage( void * parameter)
{
    if(Debug) Serial.println("Storage task start!");

    /*    #1 Get data from flash storage    */

    preferences.begin("data-space", false);
    
    contrast = preferences.getInt("contrast", 110);
    DefrostWorkTime = preferences.getInt("WorkTDef", 30);
    MinDefreeze = preferences.getInt("Defreeze", 10);
    R_brightness = preferences.getInt("R_bright", 0);
    G_brightness = preferences.getInt("G_bright", 0);
    B_brightness = preferences.getInt("B_bright", 0);
    W_brightness = preferences.getInt("W_bright", 255);
    DisplayUIFlag = preferences.getInt("DisplayUI", 1);
    TempSensorLocation_0 = preferences.getInt("SensorLoc0", 0);
    TempSensorLocation_1 = preferences.getInt("SensorLoc1", 0);
    TempSensorLocation_2 = preferences.getInt("SensorLoc2", 0);
    TempSensorLocation_3 = preferences.getInt("SensorLoc3", 0);
    TempSensorLocation_4 = preferences.getInt("SensorLoc4", 0);
    LockFlag = preferences.getBool("LockFlag", false);
    TenthsFlag = preferences.getBool("TenthsFlag", true);
    TargetTemp = preferences.getDouble("TargetTemp", 10.0f);
    CalibTemp_0 = preferences.getDouble("CalTemp0", 0.0f);
    CalibTemp_1 = preferences.getDouble("CalTemp1", 0.0f);
    CalibTemp_2 = preferences.getDouble("CalTemp2", 0.0f);
    CalibTemp_3 = preferences.getDouble("CalTemp3", 0.0f);
    CalibTemp_4 = preferences.getDouble("CalTemp4", 0.0f);
    numberOfDevices = preferences.getInt("numDevices", 1);

    preferences.end();

    double TargetTemp_Old = TargetTemp;
    double CalibTemp_0_Old = CalibTemp_0;
    double CalibTemp_1_Old = CalibTemp_1;
    double CalibTemp_2_Old = CalibTemp_2;
    double CalibTemp_3_Old = CalibTemp_3;
    double CalibTemp_4_Old = CalibTemp_4;
    int contrast_Old = contrast;
    int DefrostWorkTime_Old = DefrostWorkTime;
    int MinDefreeze_Old = MinDefreeze;
    int DisplayUIFlag_old = DisplayUIFlag;
    int TempSensorLocation_0_Old = TempSensorLocation_0;
    int TempSensorLocation_1_Old = TempSensorLocation_1;
    int TempSensorLocation_2_Old = TempSensorLocation_2;
    int TempSensorLocation_3_Old = TempSensorLocation_3;
    int TempSensorLocation_4_Old = TempSensorLocation_4;
    uint8_t R_brightness_Old = R_brightness;
    uint8_t G_brightness_Old = G_brightness;
    uint8_t B_brightness_Old = B_brightness;
    uint8_t W_brightness_Old = W_brightness;
    bool LockFlag_Old = LockFlag;
    bool TenthsFlag_old = TenthsFlag;
    int numberOfDevices_old = numberOfDevices;
    
   
    while(1){
        
        //any change in var 
        if((LockFlag_Old != LockFlag)||(contrast_Old != contrast)||(R_brightness_Old != R_brightness)||(G_brightness_Old != G_brightness)||
        (B_brightness_Old != B_brightness)||(W_brightness_Old != W_brightness)||(DefrostWorkTime_Old != DefrostWorkTime)||(MinDefreeze_Old != MinDefreeze)||
        (abs(TargetTemp_Old-TargetTemp)>0.1)||(abs(CalibTemp_0_Old - CalibTemp_0)>0.01)||(TenthsFlag_old != TenthsFlag)||(DisplayUIFlag_old != DisplayUIFlag)||
        (abs(CalibTemp_1_Old - CalibTemp_1)>0.01)||(abs(CalibTemp_2_Old - CalibTemp_2)>0.01)||(abs(CalibTemp_3_Old - CalibTemp_3)>0.01)||(numberOfDevices_old != numberOfDevices)||
        (abs(CalibTemp_4_Old - CalibTemp_4)>0.01)||(TempSensorLocation_0_Old != TempSensorLocation_0)||(TempSensorLocation_1_Old != TempSensorLocation_1)||
        (TempSensorLocation_2_Old != TempSensorLocation_2)||(TempSensorLocation_3_Old != TempSensorLocation_3)||(TempSensorLocation_4_Old != TempSensorLocation_4)){

            TargetTemp_Old = TargetTemp;
            CalibTemp_0_Old = CalibTemp_0;
            CalibTemp_1_Old = CalibTemp_1;
            CalibTemp_2_Old = CalibTemp_2;
            CalibTemp_3_Old = CalibTemp_3;
            CalibTemp_4_Old = CalibTemp_4;
            contrast_Old = contrast;
            DefrostWorkTime_Old = DefrostWorkTime;
            MinDefreeze_Old = MinDefreeze;
            R_brightness_Old = R_brightness;
            G_brightness_Old = G_brightness;
            B_brightness_Old = B_brightness;
            W_brightness_Old = W_brightness;
            LockFlag_Old = LockFlag;
            TenthsFlag_old = TenthsFlag;
            DisplayUIFlag_old = DisplayUIFlag;
            TempSensorLocation_0_Old = TempSensorLocation_0;
            TempSensorLocation_1_Old = TempSensorLocation_1;
            TempSensorLocation_2_Old = TempSensorLocation_2;
            TempSensorLocation_3_Old = TempSensorLocation_3;
            TempSensorLocation_4_Old = TempSensorLocation_4;
            numberOfDevices_old = numberOfDevices;
            
            preferences.begin("data-space", false);
            
            preferences.putDouble("TargetTemp", TargetTemp);
            preferences.putDouble("CalTemp0", CalibTemp_0);
            preferences.putDouble("CalTemp1", CalibTemp_1);
            preferences.putDouble("CalTemp2", CalibTemp_2);
            preferences.putDouble("CalTemp3", CalibTemp_3);
            preferences.putDouble("CalTemp4", CalibTemp_4);
            preferences.putInt("contrast", contrast);
            preferences.putInt("WorkTDef", DefrostWorkTime);
            preferences.putInt("Defreeze", MinDefreeze);
            preferences.putInt("R_bright", R_brightness);
            preferences.putInt("G_bright", G_brightness);
            preferences.putInt("B_bright", B_brightness);
            preferences.putInt("DisplayUI", DisplayUIFlag);
            preferences.putInt("W_bright", W_brightness);
            preferences.putInt("SensorLoc0", TempSensorLocation_0);
            preferences.putInt("SensorLoc1", TempSensorLocation_1);
            preferences.putInt("SensorLoc2", TempSensorLocation_2);
            preferences.putInt("SensorLoc3", TempSensorLocation_3);
            preferences.putInt("SensorLoc4", TempSensorLocation_4);
            preferences.putInt("numDevices", numberOfDevices);
            preferences.putBool("LockFlag", LockFlag);
            preferences.putBool("TenthsFlag", TenthsFlag);
            

            preferences.end();

            if(1) Serial.println("Data Updated!");
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
    sensors.setResolution(12);
    
    while(1){
        sensors.requestTemperatures(); // Send the command to get temperatures
        // Loop through each device, print out temperature data
        vTaskDelay(800/portTICK_PERIOD_MS);
        for(int i=0;i<numberOfDevices; i++){
            // Search the wire for address
            float dataRes = double(sensors.getTempCByIndex(i));
            if(dataRes==DEVICE_DISCONNECTED_C||dataRes==DEVICE_DISCONNECTED_RAW||(dataRes==85)){
            warnCount++;

            if(Debug){
                Serial.print(i);
                Serial.print("# WARNING! DS18B20 - ");
                Serial.println(dataRes);
            }
            }else{

                    if(i==0) TempSensor_0 = dataRes;
                    if(i==1) TempSensor_1 = dataRes;
                    if(i==2) TempSensor_2 = dataRes;
                    if(i==3) TempSensor_3 = dataRes;
                    if(i==4) TempSensor_4 = dataRes;       
            }
        }
        vTaskDelay(10000/portTICK_PERIOD_MS);
    }

    vTaskDelete( NULL );
}

void Check( void * parameter)
{
    if(Debug) Serial.println("Check task start!");

    /*    #1 Additional tasks    */

    while(1){
        if(Warning)
            Serial.println("######### WARNING #########");
        
        if(warnCount>3)
            Warning = true;
        else
        warnCount = 0;

        vTaskDelay(30000/portTICK_PERIOD_MS);

    }
    
    vTaskDelete( NULL );
}


void Additional( void * parameter)
{
    if(Debug) Serial.println("Additional task start!");

    /*    #1 Additional tasks    */

    vTaskDelay(5000/portTICK_PERIOD_MS);

    while(1){

        uint8_t Z1Count = 0;
        uint8_t Z2Count = 0;
        uint8_t E1Count = 0;
        uint8_t E2Count = 0;
        double TempZone_1 = 0.0;
        double TempEvaporator_1 = 0.0;
        double TempZone_2 = 0.0;
        double TempEvaporator_2 = 0.0;

        if(TempSensor_0!=0.0){
            if(TempSensorLocation_0==0){TempEvaporator_1 += TempSensor_0+CalibTemp_0; E1Count++;}
            if(TempSensorLocation_0==1){TempZone_1 += TempSensor_0+CalibTemp_0; Z1Count++;}
            if(TempSensorLocation_0==2){TempEvaporator_2 += TempSensor_0+CalibTemp_0; E2Count++;}
            if(TempSensorLocation_0==3){TempZone_2 += TempSensor_0+CalibTemp_0; Z2Count++;}}
        if(TempSensor_1!=0.0){
            if(TempSensorLocation_1==0){TempEvaporator_1 += TempSensor_1+CalibTemp_1; E1Count++;}
            if(TempSensorLocation_1==1){TempZone_1 += TempSensor_1+CalibTemp_1; Z1Count++;}
            if(TempSensorLocation_1==2){TempEvaporator_2 += TempSensor_1+CalibTemp_1; E2Count++;}
            if(TempSensorLocation_1==3){TempZone_2 += TempSensor_1+CalibTemp_1; Z2Count++;}}
        if(TempSensor_2!=0.0){
            if(TempSensorLocation_2==0){TempEvaporator_1 += TempSensor_2+CalibTemp_2; E1Count++;}
            if(TempSensorLocation_2==1){TempZone_1 += TempSensor_2+CalibTemp_2; Z1Count++;}
            if(TempSensorLocation_2==2){TempEvaporator_2 += TempSensor_2+CalibTemp_2; E2Count++;}
            if(TempSensorLocation_2==3){TempZone_2 += TempSensor_2+CalibTemp_2; Z2Count++;}}
        if(TempSensor_3!=0.0){
            if(TempSensorLocation_3==0){TempEvaporator_1 += TempSensor_3+CalibTemp_3; E1Count++;}
            if(TempSensorLocation_3==1){TempZone_1 += TempSensor_3+CalibTemp_3; Z1Count++;}
            if(TempSensorLocation_3==2){TempEvaporator_2 += TempSensor_3+CalibTemp_3; E2Count++;}
            if(TempSensorLocation_3==3){TempZone_2 += TempSensor_3+CalibTemp_3; Z2Count++;}}
        if(TempSensor_4!=0.0){
            if(TempSensorLocation_4==0){TempEvaporator_1 += TempSensor_4+CalibTemp_4; E1Count++;}
            if(TempSensorLocation_4==1){TempZone_1 += TempSensor_4+CalibTemp_4; Z1Count++;}
            if(TempSensorLocation_4==2){TempEvaporator_2 += TempSensor_4+CalibTemp_4; E2Count++;}
            if(TempSensorLocation_4==3){TempZone_2 += TempSensor_4+CalibTemp_4; Z2Count++;}}

        if(Z1Count != 0) Zone_1 = TempZone_1/Z1Count;
        if(Z2Count != 0) Zone_2 = TempZone_2/Z2Count;  
        if(E1Count != 0) Evaporator_1 = TempEvaporator_1/E1Count;
        if(E2Count != 0) Evaporator_2 = TempEvaporator_2/E2Count;

        if(Debug) Serial.println("Z1:" + String(Zone_1) + " Z2:" + String(Zone_2) + " E1:" + String(Evaporator_1) + " E2:"+String(Evaporator_2));
        
        //TODO Show Temp NOW or Target Temp
        tempC = Zone_1;

        vTaskDelay(5000/portTICK_PERIOD_MS);

    }
    
    vTaskDelete( NULL );
}

void ClockRTC( void * parameter)
{
    if(Debug) Serial.println("ClockRTC task start!");

    /*    #1 ClockRTC tasks    */
    xSemaphoreTake(i2c_line, portMAX_DELAY);
    Wire.setPins(21,22);
    
    if (! rtc.begin()) {
    Warning = true;
    if(Debug) Serial.println("Error RTC!");
    }
 
    if (rtc.lostPower()) {
    if(Debug) Serial.println("RTC lost power, let's set the time!");
    Warning = true;
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