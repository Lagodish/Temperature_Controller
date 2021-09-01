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

extern float tempC;
extern bool FanFlag;
extern bool RelayFlag;
extern bool CompressorFlag;
extern float TargetTemp;
 

uint8_t R_brightness = 0;
uint8_t G_brightness = 0;
uint8_t B_brightness = 0;
uint8_t W_brightness = 0;

const char *ssid = "Controller";
const char *password = "12345678";

const char *hostname = "controller";

//    !!!! http://controller.local/  !!!!!
 
int statusLabelId=0;
int graphId=0;
int millisLabelId=0;
int SwitchIdF=0;
int SwitchIdC=0;
int SwitchIdR=0;

void numberCall(Control *sender, int type) {
  TargetTemp = sender->value.toInt();
  Serial.print("Target Temp Value: ");
   Serial.println(sender->value); }

void textCall(Control *sender, int type) {
  Serial.print("Text: ID: ");
  Serial.print(sender->id);
  Serial.print(", Value: ");
  Serial.println(sender->value);
}

void slider(Control *sender, int type) {

  int sliderValue = sender->value.toInt();

  if((sliderValue>=0)&&(sliderValue<=255)){

    switch (sender->id) {
      case 6:
        Serial.println("Red brightness");
        R_brightness = sliderValue;
        break;
      case 12:
        Serial.println("Green brightness");
        G_brightness = sliderValue;
        break;
      case 9:
        Serial.println("Blue brightness");
        B_brightness = sliderValue;
        break;
      case 15:
        Serial.println("White brightness");
        W_brightness = sliderValue;
        break;
      }
    }
  

  Serial.print(", Value: ");
  Serial.println(sender->value); 
}

void buttonCallback(Control *sender, int type) {
  switch (type) {
  case B_DOWN:
    Serial.println("Button DOWN");
    break;

  case B_UP:
    Serial.println("Button UP");
    break;
  }
}


void switcherF(Control *sender, int value) {
  switch (value) {
  case S_ACTIVE:
    Serial.print("Active: F");
    FanFlag = true;
    break;

  case S_INACTIVE:
    Serial.print("Inactive: F");
    FanFlag = false;
    break;
  }

  Serial.print(" ");
  Serial.println(sender->id);
}
//Testing
void switcherR(Control *sender, int value) {
  switch (value) {
  case S_ACTIVE:
    Serial.print("Active: R");
    RelayFlag = true;
    break;

  case S_INACTIVE:
    Serial.print("Inactive: R");
    RelayFlag = false;
    break;
  }

  Serial.print(" ");
  Serial.println(sender->id);
}
//Testing..
void switcherC(Control *sender, int value) {
  switch (value) {
  case S_ACTIVE:
    Serial.print("Active: C");
    CompressorFlag = true; 
    break;

  case S_INACTIVE:
    Serial.print("Inactive: C");
    CompressorFlag = false; 
    break;
  }

  Serial.print(" ");
  Serial.println(sender->id);
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

  statusLabelId = ESPUI.label("Status:", ControlColor::Turquoise, "Stop");
  millisLabelId = ESPUI.label("UpTime:", ControlColor::Emerald, "0");
  //ESPUI.button("Push Button", &buttonCallback, ControlColor::Peterriver, "Press");
  //ESPUI.button("Other Button", &buttonExample, ControlColor::Wetasphalt, "Press");
  //ESPUI.padWithCenter("Pad with center", &padExample, ControlColor::Sunflower);
  //ESPUI.pad("Pad without center", &padExample, ControlColor::Carrot);
  SwitchIdF = ESPUI.switcher("Fan #1", &switcherF, ControlColor::None, FanFlag);
  SwitchIdR = ESPUI.switcher("Relay #1", &switcherR, ControlColor::None, RelayFlag);
  SwitchIdC = ESPUI.switcher("Compressor #1", &switcherC, ControlColor::None, CompressorFlag);
  ESPUI.slider("Red brightness", &slider, ControlColor::None, R_brightness,0,255);
  ESPUI.slider("Blue brightness", &slider, ControlColor::None, B_brightness,0,255);
  ESPUI.slider("Green brightness", &slider, ControlColor::None, G_brightness,0,255);
  ESPUI.slider("White brightness", &slider, ControlColor::None, W_brightness,0,255);
  //ESPUI.text("Controller Name:", &textCall, ControlColor::Alizarin, "Bear Boy");
  ESPUI.number("Target Temperature", &numberCall, ControlColor::Alizarin, int(TargetTemp) , 0, 30);

  graphId = ESPUI.graph("Graph Temperature", ControlColor::Wetasphalt);

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
    
   // static bool testSwitchState = false;

    while(1){

        dnsServer.processNextRequest();
        if(CompressorFlag||FanFlag||RelayFlag){
          Serial.println("Status: Work");
          ESPUI.print(statusLabelId, "Work");}
        else{
          ESPUI.print(statusLabelId, "Stop");
          Serial.println("Status: Stop");}
        
        ESPUI.print(millisLabelId, String(float(millis())/(60 * 60 * 1000))+" h");
        ESPUI.updateSwitcher(SwitchIdF, FanFlag);
        ESPUI.updateSwitcher(SwitchIdR, RelayFlag);
        ESPUI.updateSwitcher(SwitchIdC, CompressorFlag);
        ESPUI.addGraphPoint(graphId, round(tempC));

        vTaskDelay(7000/portTICK_PERIOD_MS);

    }
    
    vTaskDelete( NULL );
}