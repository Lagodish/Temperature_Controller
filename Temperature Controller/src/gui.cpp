#include <Arduino.h> 
#include "Wire.h"
#include "SH1106.h"
// Include the UI lib
#include "OLEDDisplayUi.h"

// Include custom images
#include "GUI/images.h"
#include "GUI/font.h"
#include "GUI/iconset.h"

// Extern var.
extern float tempC;
extern int contrast;
extern int DisplayUIFlag;
extern bool FanFlag;
extern bool RelayFlag;
extern bool CompressorFlag;
extern bool LightFlag;
extern bool LockFlag;
extern bool Debug;
extern bool TenthsFlag;
extern bool DefreezeFlag;
extern uint8_t HourNow;
extern uint8_t MinNow;
extern bool Warning;
extern bool SensorReadyFlag;
// Extern.
extern SemaphoreHandle_t i2c_line;

//Var.


// Initialize the OLED display using Wire library
SH1106Wire display(0x3c, 21, 22, GEOMETRY_128_64, I2C_ONE, 400000); //set I2C frequency to 400kHz
//SH1106Wire display(0x3c, 21, 22);

OLEDDisplayUi ui     ( &display );

void msOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(0, 0, "UpTime: "+String(float(millis())/(60 * 60 * 1000))+"h");

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

  int IntegerPart = (int)(tempC);
  int DecimalPart = 10 * (tempC - IntegerPart);

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

  int IntegerPart = (int)(tempC);
  int DecimalPart = 10 * (tempC - IntegerPart);

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

  int IntegerPart = (int)(tempC);
  int DecimalPart = 10 * (tempC - IntegerPart);

  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->setFont(DSEG7_Classic_Regular_50);
  display->drawString(100 + x, 8 + y, String(IntegerPart));

  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);
  display->drawString(103 + x, 48 + y, "C"); 
  display->drawCircle(101 + x, 49 + y, 2);

  if(TenthsFlag){
  display->setFont(DSEG7_Classic_Regular_15);
  display->drawString(100 + x, 15 + y, String(DecimalPart));}

}

void Frame_0(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {

  display->drawXbm(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);

}

//TODO WiFi Frame and set mode and normal mode!
FrameCallback frames[] = {Frame_3, Frame_2, Frame_1, Frame_0};

int frameCount = 4;
OverlayCallback overlays[] = { msOverlay };
int overlaysCount = 1;

void GUI( void * parameter)
{
    if(Debug) Serial.println("GUI");

    /*    #1 GUI oled control       #2 Buttons control    */
    xSemaphoreTake(i2c_line, portMAX_DELAY);
    ui.setTargetFPS(1);

    ui.disableAutoTransition();
    ui.disableAllIndicators();

    ui.setFrames(frames, frameCount);
    ui.setOverlays(overlays, overlaysCount);
    ui.init();
    ui.switchToFrame(DisplayUIFlag);
    //display.flipScreenVertically();

    //uint8_t oldData = 0;
    //if(oldData!=contrast){oldData=contrast; display.setBrightness(contrast);}
    display.setBrightness(contrast);

    delay(100);
    xSemaphoreGive(i2c_line);
    
    int remainingTimeBudget = 0;
    
    while(1){
        xSemaphoreTake(i2c_line, portMAX_DELAY);
        remainingTimeBudget = ui.update();

        //if(SensorReadyFlag) ui.switchToFrame(3);
        //else ui.switchToFrame(DisplayUIFlag);
    
   
        if (remainingTimeBudget > 0){xSemaphoreGive(i2c_line); vTaskDelay(remainingTimeBudget/portTICK_PERIOD_MS); }

    }
    
    vTaskDelete( NULL );
}