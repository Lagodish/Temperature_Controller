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
extern double TargetTemp;
extern double TempSensor_0;
extern double TempSensor_1;
extern double TempSensor_2;
extern double TempSensor_3;
extern double TempSensor_4;
extern double CalibTemp_0;
extern double CalibTemp_1;
extern double CalibTemp_2;
extern double CalibTemp_3;
extern double CalibTemp_4;
extern int DefrostWorkTime;
extern int MinDefreeze;
extern uint8_t contrast;
extern bool FanFlag;
extern bool RelayFlag;
extern bool CompressorFlag;
extern bool Debug;
extern bool TenthsFlag;
extern bool LockFlag;
extern bool DefreezeFlag;
extern int DisplayUIFlag;
extern uint8_t R_brightness;
extern uint8_t G_brightness;
extern uint8_t B_brightness;
extern uint8_t W_brightness;
extern int DisplayUIFlag;
extern int TempSensorLocation_0;
extern int TempSensorLocation_1;
extern int TempSensorLocation_2;
extern int TempSensorLocation_3;
extern int TempSensorLocation_4;
extern int numberOfDevices;
extern bool ManualMode;
extern int DefreezeTempTrig;

//TODO save storage!
extern bool FanOnWork;
extern bool FanOnDefr;
extern int SPdiff;
extern int SPmin;
extern int SPmax;
extern int OnDelay;
extern int BetweenDelay;
extern int TempIndValue;
extern int TempIndValueDefr;
extern int DefrTrig;


// Var.


//WiFi  !!!! http://controller.local/  !!!!!
const char *ssid = "Controller";
const char *password = "12345678";
const char *hostname = "controller";

// Elements IDs
uint16_t statusLabelId=0;
uint16_t C0LabelId=0;
uint16_t C1LabelId=0;
uint16_t C2LabelId=0;
uint16_t C3LabelId=0;
uint16_t C4LabelId=0;
//uint16_t graphId=0;
uint16_t millisLabelId=0;
uint16_t SwitchIdF=0;
uint16_t SwitchIdC=0;
uint16_t SwitchIdR=0;
uint16_t SliderIdR=0;
uint16_t SliderIdG=0;
uint16_t SliderIdB=0;
uint16_t SliderIdW=0;
uint16_t SliderIdBrt=0;
uint16_t NumberIdST=0;
uint16_t NumberIdTW=0;
uint16_t NumberIdTD=0;
uint16_t NumberIdC0=0;
uint16_t NumberIdC1=0;
uint16_t NumberIdC2=0;
uint16_t NumberIdC3=0;
uint16_t NumberIdC4=0;
uint16_t NumberIdN=0;
uint16_t SwitchIdM = 0;
uint16_t ButtonId0 = 0;
uint16_t SwitchId1 = 0;
uint16_t SwitchId2 = 0;
uint16_t SwitchId3 = 0;
uint16_t SwitchIdDFW = 0;
uint16_t select0 = 0;
uint16_t select1 = 0;
uint16_t select2 = 0;
uint16_t select3 = 0;
uint16_t select4 = 0;
uint16_t select5 = 0;
uint16_t select6 = 0;
uint16_t select7 = 0;
uint16_t select8 = 0;
uint16_t select9 = 0;
uint16_t select10 = 0;
uint16_t SwitchIdCW = 0;
uint16_t NumberIdDiff = 0;
uint16_t NumberIdMin = 0;
uint16_t NumberIdMax = 0;
uint16_t NumberIdDW = 0;
uint16_t NumberIdDB = 0;
uint16_t NumberIdTDS = 0;


void sliderCallback(Control *sender, int type) {

  int sliderValue = sender->value.toInt();
  int id = int(sender->id);

  if((sliderValue>=0)&&(sliderValue<=255)){
    if(id==SliderIdBrt){contrast = int(round(map(sliderValue, 0, 100, 60, 255)));}
    sliderValue = int(round(map(sliderValue, 0, 100, 0, 255)));
    if(id==SliderIdR){R_brightness = sliderValue;}
    if(id==SliderIdG){G_brightness = sliderValue;}
    if(id==SliderIdB){B_brightness = sliderValue;}
    if(id==SliderIdW){W_brightness = sliderValue;}
    }
  
}

void buttonCallback(Control *sender, int type) {
  int id = int(sender->id);
  if(ButtonId0==id){
  switch (type) {
  case B_DOWN:
    Serial.println("Software reset!");
    break;

  case B_UP:
    ESP.restart();
    break;
  }}

}


void switcherCallback(Control *sender, int value) {
  int id = int(sender->id);
  switch (value) {
  case S_ACTIVE:
    if(SwitchIdF==id){ FanFlag = true;}
    if(SwitchIdR==id){ RelayFlag = true;}
    if(SwitchIdC==id){ CompressorFlag = true;}
    if(SwitchId1==id){TenthsFlag = true;}
    if(SwitchId2==id){LockFlag = true;}
    if(SwitchId3==id){DefreezeFlag = true;}
    if(SwitchIdM==id){ManualMode = true;}
    if(SwitchIdCW==id){FanOnWork = true;}
    if(SwitchIdDFW){FanOnDefr = true;}
    break;

  case S_INACTIVE:
    if(SwitchIdF==id){ FanFlag = false;}
    if(SwitchIdR==id){ RelayFlag = false;}
    if(SwitchIdC==id){ CompressorFlag = false;}
    if(SwitchId1==id){TenthsFlag = false;}
    if(SwitchId2==id){LockFlag = false;}
    if(SwitchId3==id){DefreezeFlag = false;}
    if(SwitchIdM==id){ManualMode = false;}
    if(SwitchIdCW==id){FanOnWork = false;}
    if(SwitchIdDFW){FanOnDefr = false;}
    break;
  }
}

void selectCallback( Control* sender, int value ) {
  int id = int(sender->id);
  int data = (sender->value).toInt();

  if(select0==id){TempSensorLocation_0 = data;}
  if(select1==id){TempSensorLocation_1 = data;}
  if(select2==id){TempSensorLocation_2 = data;}
  if(select3==id){TempSensorLocation_3 = data;}
  if(select4==id){TempSensorLocation_4 = data;}
  if(select6==id){TempIndValue= data;}
  if(select9==id){TempIndValueDefr= data;}
  if(select10==id){DefrTrig= data;}
  if(select5==id){DisplayUIFlag = data;}
}

void numberCall( Control* sender, int type ) {
  int id = int(sender->id);
    
  if(NumberIdTW==id){DefrostWorkTime = (sender->value).toInt();}
  if(NumberIdTD==id){MinDefreeze = (sender->value).toInt();}
  if(NumberIdC0==id){CalibTemp_0 = (sender->value).toDouble();}
  if(NumberIdC1==id){CalibTemp_1 = (sender->value).toDouble();}
  if(NumberIdC2==id){CalibTemp_2 = (sender->value).toDouble();}
  if(NumberIdC3==id){CalibTemp_3 = (sender->value).toDouble();}
  if(NumberIdC4==id){CalibTemp_4 = (sender->value).toDouble();}
  if(NumberIdN==id){numberOfDevices = (sender->value).toInt();}
  if(NumberIdTDS==id){DefreezeTempTrig = (sender->value).toInt();}
  if(NumberIdDiff==id){SPdiff= (sender->value).toInt();}
  if(NumberIdMin==id){SPmin= (sender->value).toInt();}
  if(NumberIdMax==id){SPmax= (sender->value).toInt();}
  if(NumberIdDW==id){OnDelay= (sender->value).toInt();}
  if(NumberIdDB==id){BetweenDelay= (sender->value).toInt();}
  if(NumberIdST==id){TargetTemp = (sender->value).toDouble();}

}

void WebServer( void * parameter)
{
    if(Debug) Serial.println("WebServer");

    /*    #1 GUI oled control       #2 Buttons control    */

      ESPUI.setVerbosity(Verbosity::Quiet);

#if defined(ESP32)
  WiFi.setHostname(hostname);
#else
  WiFi.hostname(hostname);
#endif

    // not connected -> create hotspot
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("\n\nSmart Temperature Controller");
      Serial.print("Loading");

      WiFi.mode(WIFI_AP);
      WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
      WiFi.softAP(ssid, password);

      int timeout = 5;

      do {
        vTaskDelay(500/portTICK_PERIOD_MS);
        Serial.print(".");
        timeout--;
      } while (timeout);
    }


  dnsServer.start(DNS_PORT, "*", apIP);

  Serial.println("\n\nWiFi parameters: Controller:12345678");
  Serial.print("Mode: ");
  Serial.println("Station");
  Serial.print("IP address: ");
  Serial.println(WiFi.getMode() == WIFI_AP ? WiFi.softAPIP() : WiFi.localIP());

  ESPUI.jsonUpdateDocumentSize = 2000;
  ESPUI.jsonInitialDocumentSize = 11000;

  //statusLabelId = ESPUI.addControl(ControlType::Label, "Status:", "Stop", ControlColor::Turquoise);
  

  uint16_t tab1 = ESPUI.addControl(ControlType::Tab, "Light&Display", "Light&Display");
  uint16_t tab2 = ESPUI.addControl(ControlType::Tab, "Main", "Main");
  uint16_t tab3 = ESPUI.addControl(ControlType::Tab, "Additional", "Additional");
  uint16_t tab4 = ESPUI.addControl(ControlType::Tab, "Sensors", "Sensors");

//------------------ TAB 1
  SliderIdR = ESPUI.addControl(ControlType::Slider, "Red brightness<br>Яркость красный", String(int(double(R_brightness)/2.55)), ControlColor::Alizarin, tab1, &sliderCallback);
  SliderIdG = ESPUI.addControl(ControlType::Slider, "Blue brightness<br>Яркость синий", String(int(double(G_brightness)/2.55)), ControlColor::Peterriver, tab1, &sliderCallback);
  SliderIdB = ESPUI.addControl(ControlType::Slider, "Green brightness<br>Яркость зеленый", String(int(double(B_brightness)/2.55)), ControlColor::Emerald, tab1, &sliderCallback);
  SliderIdW = ESPUI.addControl(ControlType::Slider, "White brightness<br>Яркость белый", String(int(double(W_brightness)/2.55)), ControlColor::None, tab1, &sliderCallback);
  SliderIdBrt = ESPUI.addControl(ControlType::Slider, "Display brightness<br>Яркость экрана", String(int(double(contrast)/2.55)), ControlColor::Sunflower, tab1, &sliderCallback);
  select5 = ESPUI.addControl( ControlType::Select, "Display UI<br>Вид главного экрана", String(DisplayUIFlag), ControlColor::None, tab1, &selectCallback);
  ESPUI.addControl( ControlType::Option, "Temperature / Температура", "0", ControlColor::Alizarin, select5);
  ESPUI.addControl( ControlType::Option, "Temp&Icons / Темп. и значки", "1", ControlColor::Alizarin, select5);
  ESPUI.addControl( ControlType::Option, "Temp&Icons+ / Темп. и значки+", "2", ControlColor::Alizarin, select5);
  ESPUI.addControl( ControlType::Option, "WiFi show / Показать WiFi", "3", ControlColor::Sunflower, select5);

//------------------ TAB 2
  NumberIdTW = ESPUI.addControl(ControlType::Number, "Defrost Work Time<br>Время работы оттайки (Min)", String(DefrostWorkTime), ControlColor::Alizarin, tab2, &numberCall);
  NumberIdTD = ESPUI.addControl(ControlType::Number, "Defrost interval - D0<br>Интервал между процессами оттайки (Min)", String(MinDefreeze), ControlColor::Peterriver, tab2, &numberCall);
  NumberIdTDS = ESPUI.addControl(ControlType::Number, "Defrost Trig Temp - D1<br>Температура запуска процесса оттайки (°C)", String(DefreezeTempTrig), ControlColor::Peterriver, tab2, &numberCall);
  SwitchId1 = ESPUI.addControl(ControlType::Switcher, "Display tenths<br>Отображать десятые", TenthsFlag ? "1" : "0", ControlColor::None, tab2, &switcherCallback);
  SwitchIdCW = ESPUI.addControl(ControlType::Switcher, "Fan ON during normal operation<br>Вентилятор ВКЛ при нормальной работе", FanOnWork ? "1" : "0", ControlColor::None, tab2, &switcherCallback);  // FIX "0" TODO 
  SwitchIdDFW = ESPUI.addControl(ControlType::Switcher, "Fan ON during Defrost<br>Вентилятор ВКЛ при оттайке", FanOnDefr ? "1" : "0", ControlColor::None, tab2, &switcherCallback);  // FIX "0" TODO 
  NumberIdDiff = ESPUI.addControl(ControlType::Number, "Working set point differential<br>Дифференциал рабочей контрольной точки", String(SPdiff), ControlColor::Turquoise, tab2, &numberCall);
  NumberIdMin = ESPUI.addControl(ControlType::Number, "Min working point value<br>Мин значение рабочей точки (°C)", String(SPmin), ControlColor::Turquoise, tab2, &numberCall);
  NumberIdMax = ESPUI.addControl(ControlType::Number, "Max working point value<br>Макс значение рабочей точки (°C)", String(SPmax), ControlColor::Turquoise, tab2, &numberCall);
  NumberIdDW = ESPUI.addControl(ControlType::Number, "Compressor turn-on delay at power-up (min.)<br>Задержка включения компрессора при подаче питания (минуты)", String(OnDelay), ControlColor::Turquoise, tab2, &numberCall);
  NumberIdDB = ESPUI.addControl(ControlType::Number, "Interval compressor work (min.)<br>Интервал цикла компрессора (минуты)", String(BetweenDelay), ControlColor::Turquoise, tab2, &numberCall);

  select6 = ESPUI.addControl( ControlType::Select, "Temperature indicator value<br>Значение индикатора температуры", String(TempIndValue), ControlColor::Emerald, tab2, &selectCallback);
    ESPUI.addControl( ControlType::Option, "Chamber temperature / Температура камеры", "0", ControlColor::Alizarin, select6);
    ESPUI.addControl( ControlType::Option, "Working set point / Рабочая точка", "1", ControlColor::Alizarin, select6);
  select9 = ESPUI.addControl( ControlType::Select, "Temperature value on the display during defrost<br>Температура на экране во время оттайки", String(TempIndValueDefr), ControlColor::Emerald, tab2, &selectCallback);
    ESPUI.addControl( ControlType::Option, "Температура с датчика", "0", ControlColor::Alizarin, select9);
    ESPUI.addControl( ControlType::Option, "Температура  'установленная'/'рабочая точкa'", "1", ControlColor::Alizarin, select9);
  select10 = ESPUI.addControl( ControlType::Select, "Defrost activation method<br>Способ активации процесса оттайки", String(DefrTrig), ControlColor::Emerald, tab2, &selectCallback);
    ESPUI.addControl( ControlType::Option, "Периодический – активирован по интервалу D0", "0", ControlColor::Alizarin, select10);
    ESPUI.addControl( ControlType::Option, "Периодический – температура испарителя установится ниже температуры D1 и по интервалу D0", "1", ControlColor::Alizarin, select10);

//------------------ TAB 3
  millisLabelId = ESPUI.addControl(ControlType::Label, "UpTime:", "0.0 h", ControlColor::Carrot,tab3);
  ButtonId0 = ESPUI.addControl(ControlType::Button, "Reboot Controller<br>Перезагрузить", "Reboot now<br>Сейчас", ControlColor::Alizarin, tab3, &buttonCallback);
  NumberIdST = ESPUI.addControl(ControlType::Number, "Target temp (°C)<br>Установленная температура (°C)", String(TargetTemp), ControlColor::Turquoise, tab3, &numberCall);
  SwitchIdM = ESPUI.addControl(ControlType::Switcher, "Manual Mode *!*<br>Ручное управление *!*", ManualMode ? "1" : "0", ControlColor::None, tab3, &switcherCallback);  
  SwitchIdF = ESPUI.addControl(ControlType::Switcher, "Fan<br>Вентилятор", "0", ControlColor::None, tab3, &switcherCallback);
  SwitchIdR = ESPUI.addControl(ControlType::Switcher, "Relay<br>Доп. выход", "0", ControlColor::None, tab3, &switcherCallback);
  SwitchIdC = ESPUI.addControl(ControlType::Switcher, "Compressor<br>Компрессор", "0", ControlColor::None, tab3, &switcherCallback);
  SwitchId3 = ESPUI.addControl(ControlType::Switcher, "Defreeze<br>Оттайка", "0", ControlColor::None, tab3, &switcherCallback);
  SwitchId2 = ESPUI.addControl(ControlType::Switcher, "Key lock<br>Блокировка кнопок", "0", ControlColor::None, tab3, &switcherCallback);

//------------------ TAB 4
  NumberIdN = ESPUI.addControl(ControlType::Number, "Number of sensor(s)<br>Кол-во термодатчиков (шт)", String(numberOfDevices), ControlColor::Carrot, tab4, &numberCall);
  
  
  select0 = ESPUI.addControl( ControlType::Select, "Sensor located  #1<br>Расположение термодатчика #1", String(TempSensorLocation_0), ControlColor::Peterriver, tab4, &selectCallback);
  C0LabelId = ESPUI.addControl(ControlType::Label, "Temperature Sensor #1<br>Термодатчик #1", "", ControlColor::Peterriver, tab4);
  NumberIdC0 = ESPUI.addControl(ControlType::Number, "Calibration Sensor #1<br>Калибровка термодатчика #1 (°C):", String(CalibTemp_0), ControlColor::Peterriver, tab4, &numberCall);
    ESPUI.addControl( ControlType::Option, "Evaporator 1 / Испаритель 1", "0", ControlColor::None, select0);
    ESPUI.addControl( ControlType::Option, "Zone 1 / Зона 1", "1", ControlColor::None, select0);
    ESPUI.addControl( ControlType::Option, "Evaporator 2 / Испаритель 2", "2", ControlColor::None, select0);
    ESPUI.addControl( ControlType::Option, "Zone 2 / Зона 2", "3", ControlColor::None, select0);
    ESPUI.addControl( ControlType::Option, "None / Нету", "127", ControlColor::None, select0);
  
  select1 = ESPUI.addControl( ControlType::Select, "Sensor located #2<br>Расположение термодатчика #2", String(TempSensorLocation_1), ControlColor::Turquoise, tab4, &selectCallback);
  C1LabelId = ESPUI.addControl(ControlType::Label, "Temperature Sensor #2<br>Термодатчик #2", "", ControlColor::Turquoise, tab4);
  NumberIdC1 = ESPUI.addControl(ControlType::Number, "Calibration Sensor #2<br>Калибровка термодатчика #2 (°C)", String(CalibTemp_1), ControlColor::Turquoise, tab4, &numberCall);
    ESPUI.addControl( ControlType::Option, "Evaporator 1 / Испаритель 1", "0", ControlColor::None, select1);
    ESPUI.addControl( ControlType::Option, "Zone 1 / Зона 1", "1", ControlColor::None, select1);
    ESPUI.addControl( ControlType::Option, "Evaporator 2 / Испаритель 2", "2", ControlColor::None, select1);
    ESPUI.addControl( ControlType::Option, "Zone 2 / Зона 2", "3", ControlColor::None, select1);
    ESPUI.addControl( ControlType::Option, "None / Нету", "127", ControlColor::None, select1); 
 
  select2 = ESPUI.addControl( ControlType::Select, "Sensor located  #3<br>Расположение термодатчика #3", String(TempSensorLocation_2), ControlColor::Sunflower, tab4, &selectCallback);
  C2LabelId = ESPUI.addControl(ControlType::Label, "Temperature Sensor #3<br>Термодатчик #3", "", ControlColor::Sunflower, tab4);
  NumberIdC2 = ESPUI.addControl(ControlType::Number, "Calibration Sensor #3<br>Калибровка термодатчика #3 (°C)", String(CalibTemp_2), ControlColor::Sunflower, tab4, &numberCall);
    ESPUI.addControl( ControlType::Option, "Evaporator 1 / Испаритель 1", "0", ControlColor::None, select2);
    ESPUI.addControl( ControlType::Option, "Zone 1 / Зона 1", "1", ControlColor::None, select2);
    ESPUI.addControl( ControlType::Option, "Evaporator 2 / Испаритель 2", "2", ControlColor::None, select2);
    ESPUI.addControl( ControlType::Option, "Zone 2 / Зона 2", "3", ControlColor::None, select2);
    ESPUI.addControl( ControlType::Option, "None / Нету", "127", ControlColor::None, select2);
  
  select3 = ESPUI.addControl( ControlType::Select, "Sensor located #4<br>Расположение термодатчика #4", String(TempSensorLocation_3), ControlColor::Emerald, tab4, &selectCallback);
  C3LabelId = ESPUI.addControl(ControlType::Label, "Temperature Sensor #4<br>Термодатчик #4", "", ControlColor::Emerald, tab4);
  NumberIdC3 = ESPUI.addControl(ControlType::Number, "Calibration Sensor #4<br>Калибровка термодатчика #4 (°C)", String(CalibTemp_3), ControlColor::Emerald, tab4, &numberCall);
    ESPUI.addControl( ControlType::Option, "Evaporator 1 / Испаритель 1", "0", ControlColor::None, select3);
    ESPUI.addControl( ControlType::Option, "Zone 1 / Зона 1", "1", ControlColor::None, select3);
    ESPUI.addControl( ControlType::Option, "Evaporator 2 / Испаритель 2", "2", ControlColor::None, select3);
    ESPUI.addControl( ControlType::Option, "Zone 2 / Зона 2", "3", ControlColor::None, select3);
    ESPUI.addControl( ControlType::Option, "None / Нету", "127", ControlColor::None, select3);
//if(numberOfDevices==5){}
  select4 = ESPUI.addControl( ControlType::Select, "Sensor located #5<br>Расположение термодатчика #5", String(TempSensorLocation_4), ControlColor::Alizarin, tab4, &selectCallback);
  C4LabelId = ESPUI.addControl(ControlType::Label, "Temperature Sensor #5<br>Термодатчик #5", "", ControlColor::Alizarin, tab4);
  NumberIdC4 = ESPUI.addControl(ControlType::Number, "Calibration Sensor #5<br>Калибровка термодатчика #5 (°C)", String(CalibTemp_4), ControlColor::Alizarin, tab4, &numberCall);
    ESPUI.addControl( ControlType::Option, "Evaporator 1 / Испаритель 1", "0", ControlColor::Alizarin, select4);
    ESPUI.addControl( ControlType::Option, "Zone 1 / Зона 1", "1", ControlColor::None, select4);
    ESPUI.addControl( ControlType::Option, "Evaporator 2 / Испаритель 2", "2", ControlColor::None, select4);
    ESPUI.addControl( ControlType::Option, "Zone 2 / Зона 2", "3", ControlColor::None, select4);
    ESPUI.addControl( ControlType::Option, "None / Нету", "127", ControlColor::None, select4);

  //ButtonId1 = ESPUI.addControl(ControlType::Button, "Clear graph<br>Очистить график", "Clear<br>Очистить", ControlColor::Alizarin, tab3, &buttonCallback);
   
  //graphId = ESPUI.addControl(ControlType::Graph, "Graph Temperature (x10)", "0", ControlColor::Sunflower, tab3);

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
  //ESPUI.clearGraph(graphId);

  bool FFOld = false;
  bool RFOld = false;
  bool CFOld = false; 
  double TempOld_0 = 0;
  double TempOld_1 = 0;
  double TempOld_2 = 0;
  double TempOld_3 = 0;
  double TempOld_4 = 0;
  int Millis_Old = 0;

    while(1){

      dnsServer.processNextRequest();

      //if(CompressorFlag||FanFlag||RelayFlag)  Serial.println("Status: Operate");
      //else  ESPUI.updateControlValue(statusLabelId, "Stop");
              
      int MillisNow = millis()/(60 * 600); //TODO fix! Not working
      if(Millis_Old!=MillisNow){Millis_Old=MillisNow; ESPUI.updateControlValue(millisLabelId, String(double(millis())/(60 * 60 * 1000))+" h");}
          
      //ESPUI.addGraphPoint(graphId, int(round(tempC*10)));

      //update stages
      if(FFOld != FanFlag){FFOld = FanFlag; ESPUI.updateControlValue(SwitchIdF, FanFlag ? "1" : "0");}
      if(RFOld != RelayFlag){RFOld = RelayFlag; ESPUI.updateControlValue(SwitchIdR, RelayFlag ? "1" : "0");}
      if(CFOld != CompressorFlag){CFOld = CompressorFlag; ESPUI.updateControlValue(SwitchIdC, CompressorFlag ? "1" : "0");}
      if(TempOld_0!= TempSensor_0){TempOld_0 = TempSensor_0; ESPUI.updateControlValue(C0LabelId, String(TempSensor_0)+" °C");}
      if(TempOld_1 != TempSensor_1){TempOld_1 = TempSensor_1; ESPUI.updateControlValue(C1LabelId, String(TempSensor_1)+" °C");}
      if(TempOld_2 != TempSensor_2){TempOld_2 = TempSensor_2; ESPUI.updateControlValue(C2LabelId, String(TempSensor_2)+" °C");}
      if(TempOld_3 != TempSensor_3){TempOld_3 = TempSensor_3; ESPUI.updateControlValue(C3LabelId, String(TempSensor_3)+" °C");}
      if(TempOld_4 != TempSensor_4){TempOld_4 = TempSensor_4; ESPUI.updateControlValue(C4LabelId, String(TempSensor_4)+" °C");}

      vTaskDelay(3000/portTICK_PERIOD_MS);
    }
    
    vTaskDelete( NULL );
}