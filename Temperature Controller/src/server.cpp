#include <Arduino.h>

#include <DNSServer.h>
#include <ESPUI.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;

#if defined(ESP32)
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

// Extern var.
extern float tempC;
extern double CalibTemp_0;
extern double CalibTemp_1;
extern double CalibTemp_2;
extern double CalibTemp_3;
extern double CalibTemp_4;
extern int MinWorkComp;
extern int MinDefreeze;
extern uint8_t contrast;
extern bool FanFlag;
extern bool RelayFlag;
extern bool CompressorFlag;

// Var.
uint8_t R_brightness = 0;
uint8_t G_brightness = 0;
uint8_t B_brightness = 0;
uint8_t W_brightness = 0;

//WiFi  !!!! http://controller.local/  !!!!!
const char *ssid = "Controller";
const char *password = "12345678";
const char *hostname = "controller";

// Elements IDs
uint8_t statusLabelId=0;
uint8_t graphId=0;
uint8_t millisLabelId=0;
uint8_t SwitchIdF=0;
uint8_t SwitchIdC=0;
uint8_t SwitchIdR=0;
uint8_t SliderIdR=0;
uint8_t SliderIdG=0;
uint8_t SliderIdB=0;
uint8_t SliderIdW=0;
uint8_t SliderIdBrt=0;
uint8_t NumberIdTW=0;
uint8_t NumberIdTD=0;
uint8_t NumberIdC0=0;
uint8_t NumberIdC1=0;
uint8_t NumberIdC2=0;
uint8_t NumberIdC3=0;
uint8_t NumberIdC4=0;


void sliderCallback(Control *sender, int type) {

  int sliderValue = sender->value.toInt();
  sliderValue = map(sliderValue, 0, 100, 0, 255);
  int id = int(sender->id);

  if((sliderValue>=0)&&(sliderValue<=255)){
    if(id==SliderIdR){Serial.print("Red"); R_brightness = sliderValue;}
    if(id==SliderIdG){Serial.print("Green"); G_brightness = sliderValue;}
    if(id==SliderIdB){Serial.print("Blue"); B_brightness = sliderValue;}
    if(id==SliderIdW){Serial.print("White"); W_brightness = sliderValue;}
    if(id==SliderIdBrt){Serial.print("Display"); if(sliderValue<60){sliderValue=60;} contrast = sliderValue;}
    }
  

  Serial.print(", Value: ");
  Serial.println(sender->value); 
}

void buttonCallback(Control *sender, int type) {
  switch (type) {
  case B_DOWN:
    Serial.println("Software reset!");
    break;

  case B_UP:
    ESP.restart();
    break;
  }
}


void switcherCallback(Control *sender, int value) {
  int id = int(sender->id);
  switch (value) {
  case S_ACTIVE:
    if(SwitchIdF==id){ FanFlag = true;}
    if(SwitchIdR==id){ RelayFlag = true;}
    if(SwitchIdC==id){ CompressorFlag = true;}

    break;

  case S_INACTIVE:
    if(SwitchIdF==id){ FanFlag = false;}
    if(SwitchIdR==id){ RelayFlag = false;}
    if(SwitchIdC==id){ CompressorFlag = false;}

    break;
  }
}

void numberCall( Control* sender, int type ) {
  int id = int(sender->id);
  double data = (sender->value).toDouble();
    
  if(id==NumberIdTW){MinWorkComp = int(data);}
  if(id==NumberIdTD){MinDefreeze = int(data);}
  if(id==NumberIdC0){CalibTemp_0 = data;}
  if(id==NumberIdC1){CalibTemp_1 = data;}
  if(id==NumberIdC2){CalibTemp_2 = data;}
  if(id==NumberIdC3){CalibTemp_3 = data;}
  if(id==NumberIdC4){CalibTemp_4 = data;}

  Serial.print(id);
  Serial.print(" , Value: ");
  Serial.println(data); 
}

void WebServer( void * parameter)
{
    Serial.println("WebServer");

    /*    #1 GUI oled control       #2 Buttons control    */

      ESPUI.setVerbosity(Verbosity::VerboseJSON);

#if defined(ESP32)
  WiFi.setHostname(hostname);
#else
  WiFi.hostname(hostname);
#endif

    // not connected -> create hotspot
    if (WiFi.status() != WL_CONNECTED) {
      Serial.print("\n\nCreating hotspot");

      WiFi.mode(WIFI_AP);
      WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
      WiFi.softAP(ssid, password);

      int timeout = 5;

      do {
        delay(500);
        Serial.print(".");
        timeout--;
      } while (timeout);
    }


  dnsServer.start(DNS_PORT, "*", apIP);

  Serial.println("\n\nWiFi parameters: Name - Controller");
  Serial.print("Mode: ");
  Serial.println(WiFi.getMode() == WIFI_AP ? "Station" : "Client");
  Serial.print("IP address: ");
  Serial.println(WiFi.getMode() == WIFI_AP ? WiFi.softAPIP() : WiFi.localIP());

  statusLabelId = ESPUI.addControl(ControlType::Label, "Status:", "Stop", ControlColor::Turquoise );
  millisLabelId = ESPUI.addControl(ControlType::Label, "UpTime:", "0.0 h", ControlColor::Carrot );

  uint16_t tab1 = ESPUI.addControl(ControlType::Tab, "Light", "Light" );
  uint16_t tab2 = ESPUI.addControl(ControlType::Tab, "Main", "Main" );
  uint16_t tab3 = ESPUI.addControl(ControlType::Tab, "Additional", "Additional" );

  SliderIdR = ESPUI.addControl(ControlType::Slider, "Red brightness", String(double(R_brightness)/2.55), ControlColor::Alizarin, tab1, &sliderCallback);
  SliderIdG = ESPUI.addControl(ControlType::Slider, "Blue brightness", String(double(G_brightness)/2.55), ControlColor::Peterriver, tab1, &sliderCallback);
  SliderIdB = ESPUI.addControl(ControlType::Slider, "Green brightness", String(double(B_brightness)/2.55), ControlColor::Emerald, tab1, &sliderCallback);
  SliderIdW = ESPUI.addControl(ControlType::Slider, "White brightness", String(double(W_brightness)/2.55), ControlColor::None, tab1, &sliderCallback);
  SliderIdBrt = ESPUI.addControl(ControlType::Slider, "Display brightness", String(double(contrast)/2.55), ControlColor::Sunflower, tab1, &sliderCallback);

  NumberIdTW = ESPUI.addControl(ControlType::Number, "Compressor Work Time (Min):", "0", ControlColor::Alizarin, tab2, &numberCall);
  NumberIdTD = ESPUI.addControl(ControlType::Number, "Defreeze Duration (Min):", "0", ControlColor::Alizarin, tab2, &numberCall);
  NumberIdC0 = ESPUI.addControl(ControlType::Number, "Calibration Sensor #0 (°C):", "0", ControlColor::Sunflower, tab2, &numberCall);
  NumberIdC1 = ESPUI.addControl(ControlType::Number, "Calibration Sensor #1 (°C)", "0", ControlColor::Sunflower, tab2, &numberCall);
  NumberIdC2 = ESPUI.addControl(ControlType::Number, "Calibration Sensor #2 (°C)", "0", ControlColor::Sunflower, tab2, &numberCall);
  NumberIdC3 = ESPUI.addControl(ControlType::Number, "Calibration Sensor #3 (°C)", "0", ControlColor::Sunflower, tab2, &numberCall);
  NumberIdC4 = ESPUI.addControl(ControlType::Number, "Calibration Sensor #4 (°C)", "0", ControlColor::Sunflower, tab2, &numberCall);

  ESPUI.addControl(ControlType::Button, "Reboot Controller", "Reboot now", ControlColor::Alizarin, tab3, &buttonCallback );
  SwitchIdF = ESPUI.addControl(ControlType::Switcher, "Fan #1", "0", ControlColor::None, tab3, &switcherCallback);
  SwitchIdR = ESPUI.addControl(ControlType::Switcher, "Relay #1", "0", ControlColor::None, tab3, &switcherCallback);
  SwitchIdC = ESPUI.addControl(ControlType::Switcher, "Compressor #1", "0", ControlColor::None, tab3, &switcherCallback);
  graphId = ESPUI.addControl(ControlType::Graph, "Graph Temperature", "0", ControlColor::Sunflower, tab3);

  //NumberIdTempTarg = ESPUI.number("Target Temperature", &numberCall, ControlColor::None, int(TargetTemp) , 0, 30);
  //ESPUI.button("Push Button", &buttonCallback, ControlColor::Peterriver, "Press");

  /*
   * .begin loads and serves all files from PROGMEM directly.
   * If you want to serve the files from SPIFFS use ESPUI.beginSPIFFS
   * (.prepareFileSystem has to be run in an empty sketch before)
   */
 
  // Enable this option if you want sliders to be continuous (update during move) and not discrete (update on stop)
  // ESPUI.sliderContinuous = true;

  /*
   * Optionally you can use HTTP BasicAuth. Keep in mind that this is NOT a
   * SECURE way of limiting access.
   * Anyone who is able to sniff traffic will be able to intercept your password
   * since it is transmitted in cleartext. Just add a string as username and
   * password, for example begin("ESPUI Control", "username", "password")
   */


  ESPUI.begin("Temperature Controller","admin","admin");
  ESPUI.clearGraph(graphId);
   // static bool testSwitchState = false;
  ESPUI.updateControlValue(NumberIdTW, String(MinWorkComp));
  ESPUI.updateControlValue(NumberIdTD, String(MinDefreeze));
  ESPUI.updateControlValue(NumberIdC0, String(CalibTemp_0));
  ESPUI.updateControlValue(NumberIdC1, String(CalibTemp_1));
  ESPUI.updateControlValue(NumberIdC2, String(CalibTemp_2));
  ESPUI.updateControlValue(NumberIdC3, String(CalibTemp_3));
  ESPUI.updateControlValue(NumberIdC4, String(CalibTemp_4));

    while(1){

      dnsServer.processNextRequest();

      if(CompressorFlag||FanFlag||RelayFlag){
        Serial.println("Status: Operate");
        ESPUI.updateControlValue(statusLabelId, "Operate");}
      else{
        ESPUI.updateControlValue(statusLabelId, "Stop");
        Serial.println("Status: Stop");}
        
      ESPUI.updateControlValue(millisLabelId, String(float(millis())/(60 * 60 * 1000))+" h");
      ESPUI.addGraphPoint(graphId, int(round(tempC)));

      //update stages
      ESPUI.updateControlValue(SwitchIdF, FanFlag ? "1" : "0");
      ESPUI.updateControlValue(SwitchIdR, RelayFlag ? "1" : "0");
      ESPUI.updateControlValue(SwitchIdC, CompressorFlag ? "1" : "0");
      
      vTaskDelay(3000/portTICK_PERIOD_MS);
    }
    
    vTaskDelete( NULL );
}