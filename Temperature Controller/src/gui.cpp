#include <Arduino.h> 
#include "Wire.h"
#include "SH1106.h"
#include <pins_define.h>
// Include the UI lib
#include "OLEDDisplayUi.h"
// Include custom images
#include "GUI/images.h"
#include "GUI/font.h"
#include "GUI/iconset.h"


// Extern var.
extern double Zone_1;
extern double TargetTemp;
extern int contrast;
extern int DisplayUIFlag;
extern uint8_t HourNow;
extern uint8_t MinNow;
extern bool FanFlag;
extern bool RelayFlag;
extern bool CompressorFlag;
extern bool LightFlag;
extern bool PowerOnFlag;
extern bool LockFlag;
extern bool Debug;
extern bool TenthsFlag;
extern bool DefreezeFlag;
extern bool Warning;
extern int SPmin;
extern int SPmax;
extern int TempIndValue;
extern bool TempIndSet;

// Var.
double TempDisplay = 0;

// Extern.
extern SemaphoreHandle_t i2c_line;

// Initialize the OLED display using Wire library
SH1106Wire display(0x3c, 21, 22, GEOMETRY_128_64, I2C_ONE, 400000); //set I2C frequency to 400kHz

OLEDDisplayUi ui     (&display);

void msOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
  //display->setTextAlignment(TEXT_ALIGN_LEFT);
  //display->setFont(ArialMT_Plain_10);
  //display->drawString(0, 0, "UpTime: "+String(float(millis())/(60 * 60 * 1000))+"h");

  String Time = "";
  if(HourNow<10){Time="0"+String(HourNow);}
  else{Time=String(HourNow);}
  Time+=":";
  if(MinNow<10){Time=Time+"0"+String(MinNow);}
  else{Time+=String(MinNow);}
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(128, 0, Time);

}

void Frame_1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {

  int IntegerPart = (int)(TempDisplay);
  int DecimalPart = 10 * abs(TempDisplay - IntegerPart);

  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->setFont(DSEG7_Classic_Regular_50);
  display->drawString(77 + x, 8 + y, String(IntegerPart));

  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);
  display->drawString(80 + x, 48 + y, "C"); 
  display->drawCircle(78 + x, 49 + y, 2);

  if(TenthsFlag){
  display->setFont(DSEG7_Classic_Regular_15);
  display->drawString(77 + x, 15 + y, String(DecimalPart));}
  
  display->drawIco16x16(97 + x, 14 + y, LockFlag ? lock_closed_icon16x16 : lock_open_icon16x16); //Power powerbutton_icon16x16   || lock_closed_icon16x16 or lock_open_icon16x16 or door_icon16x16 if door is open || warning_icon16x16
  display->drawIco16x16(97 + x, 31 + y, LightFlag ? bulb_off_icon16x16 : bulb_icon16x16);    //Light    || or bulb_icon16x16
  display->drawIco16x16(97 + x, 48 + y, DefreezeFlag ? humidity_icon16x16 : humidity2_icon16x16);    //Water    || or humidity2_icon16x16

  
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->setFont(ArialMT_Plain_10);
  //TODO Flag changer! Fix names!
  display->drawString(128 + x, 16 + y, LockFlag ? "On":"Off"); //Lock
  display->drawString(128 + x, 33 + y, LightFlag ? "On":"Off"); //Light
  display->drawString(128 + x, 50 + y, DefreezeFlag ? "On":"Off");  //Defreeze
}

void Frame_2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {

  int IntegerPart = (int)(TempDisplay);
  int DecimalPart = 10 * abs(TempDisplay - IntegerPart);

  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->setFont(DSEG7_Classic_Regular_50);
  display->drawString(80 + x, 8 + y, String(IntegerPart));

  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);
  display->drawString(83 + x, 48 + y, "C"); 
  display->drawCircle(81 + x, 49 + y, 2);

  if(TenthsFlag){
  display->setFont(DSEG7_Classic_Regular_15);
  display->drawString(80 + x, 15 + y, String(DecimalPart));}
  
  display->drawIco16x16(107 + x, 14 + y, LockFlag ? lock_closed_icon16x16 : lock_open_icon16x16); //Power powerbutton_icon16x16   || lock_closed_icon16x16 or lock_open_icon16x16 or door_icon16x16 if door is open || warning_icon16x16
  if(Warning) display->drawIco16x16(107 + x, 31 + y, warning_icon16x16);    //Light    || or bulb_icon16x16
  else display->drawIco16x16(107 + x, 31 + y, LightFlag ? bulb_off_icon16x16 : bulb_icon16x16);
  display->drawIco16x16(107 + x, 48 + y, DefreezeFlag ? humidity_icon16x16 : humidity2_icon16x16);    //Water    || or humidity2_icon16x16
}

void Frame_3(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {

  int IntegerPart = (int)(TempDisplay);
  int DecimalPart = 10 * abs(TempDisplay - IntegerPart);

  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->setFont(DSEG7_Classic_Regular_50);
  display->drawString(95 + x, 8 + y, String(IntegerPart));

  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);
  display->drawString(98 + x, 48 + y, "C"); 
  display->drawCircle(96 + x, 49 + y, 2);

  if(TenthsFlag){
  display->setFont(DSEG7_Classic_Regular_15);
  display->drawString(95 + x, 15 + y, String(DecimalPart));}

  if(Warning) display->drawIco16x16(110 + x, 31 + y, warning_icon16x16);
}

void Frame_0(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {

  display->drawXbm(34, 0, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
  
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 32 + y, "Connect to Controller!");
  display->drawString(64 + x, 42 + y, "WiFi - Controller:12345678");
  display->drawString(64 + x, 52 + y, "http://192.168.1.1/");
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(0 + x, 0 + y, "Settings");

}

FrameCallback frames[] = {Frame_3, Frame_2, Frame_1, Frame_0};

int frameCount = 4;
OverlayCallback overlays[] = { msOverlay };
int overlaysCount = 1;

void GUI( void * parameter)
{
    if(Debug) Serial.println("GUI");

    /*    #1 GUI oled control       #2 Buttons control    */
    xSemaphoreTake(i2c_line, portMAX_DELAY);
    ui.setTargetFPS(5);

    ui.disableAutoTransition();
    ui.disableAllIndicators();

    ui.setFrames(frames, frameCount);
    //ui.setOverlays(overlays, overlaysCount);

    ui.switchToFrame(DisplayUIFlag);
    ui.init();
    //display.flipScreenVertically();
    display.setBrightness(contrast);
    xSemaphoreGive(i2c_line);
    
    uint8_t oldData = contrast;
    int remainingTimeBudget = 0;
    int switchToFrameFlag = DisplayUIFlag;
    bool power = true;
    
    while(1){
        xSemaphoreTake(i2c_line, portMAX_DELAY);
        remainingTimeBudget = ui.update();
        if(switchToFrameFlag != DisplayUIFlag){switchToFrameFlag = DisplayUIFlag; ui.switchToFrame(DisplayUIFlag);}
        if(oldData!=contrast){oldData=contrast; display.setBrightness(contrast);}
        if(power!=PowerOnFlag){power=PowerOnFlag; if(PowerOnFlag){display.displayOn();}else{display.displayOff();}}
        xSemaphoreGive(i2c_line);

        if((TempIndValue==0)&&(TempIndSet==false)){
          TempDisplay = Zone_1;
          if(DefreezeFlag==1){TempDisplay = TargetTemp;}
        } else{TempDisplay = TargetTemp; }

        if (remainingTimeBudget > 0){
          vTaskDelay(remainingTimeBudget/portTICK_PERIOD_MS);}
        
    }
    
    vTaskDelete( NULL );
}

void Buttons( void * parameter)
{

  bool state_btn1 = false, state_btn2 = false, state_btn3 = false, state_btn4 = false;

  pinMode(PIN_BUTTON1,INPUT);
  pinMode(PIN_BUTTON2,INPUT);
  pinMode(PIN_BUTTON3,INPUT);
  pinMode(PIN_BUTTON4,INPUT);
 
  while(true){

         bool st1 = digitalRead(PIN_BUTTON1);
         bool st2 = digitalRead(PIN_BUTTON2);
         bool st3 = digitalRead(PIN_BUTTON3);
         bool st4 = digitalRead(PIN_BUTTON4);
 
         if( st1 != state_btn1 ){     //Minus Button pressed
            state_btn1 = st1;
            if( st1 == HIGH ){ 
              TempIndSet=true;
              if((TargetTemp-0.1)>double(SPmin)){TargetTemp = TargetTemp-0.5;}
            }
         }        
              
         if( st2 != state_btn2 ){   //Power Button pressed
            state_btn2 = st2;
            if( st2 == HIGH ){PowerOnFlag = !PowerOnFlag;}
         }        
              
         if( st3 != state_btn3 ){     //Plus Button pressed
            state_btn3 = st3;
            if( st3 == HIGH ){
              TempIndSet=true;
              if((TargetTemp+0.1)<double(SPmax)){TargetTemp = TargetTemp+0.5;}
            }
         }   
              
         if( st4 != state_btn4 ){  // Light Button pressed
            state_btn4 = st4;
            if( st4 == HIGH ){ LightFlag = !LightFlag;}
         }

         vTaskDelay(100);
      }
   
    
    vTaskDelete( NULL );
}
