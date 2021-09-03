#include <Arduino.h> 
//#include "SSD1306Wire.h"
#include "SH1106Wire.h"
// Include the UI lib
#include "OLEDDisplayUi.h"

// Include custom images
#include "GUI/images.h"
#include "GUI/font.h"
#include "GUI/iconset.h"

extern float tempC;
extern uint8_t contrast;
extern bool FanFlag;
extern bool RelayFlag;
extern bool CompressorFlag;
extern bool LightFlag;
extern bool LockFlag;

// Initialize the OLED display using Wire library
SH1106Wire display(0x3c, 21, 22, GEOMETRY_128_64, I2C_ONE, 400000); //set I2C frequency to 400kHz
//SH1106Wire display(0x3c, 21, 22);

OLEDDisplayUi ui     ( &display );

void msOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(0, 0, "UpTime: "+String(float(millis())/(60 * 60 * 1000))+"h");

  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(128, 0, "09:36");

}

void MainFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {

  int IntegerPart = (int)(tempC);
  int DecimalPart = 10 * (tempC - IntegerPart);

  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->setFont(DSEG7_Classic_Regular_50);
  display->drawString(77 + x, 8 + y, String(IntegerPart));

  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);
  display->drawString(80 + x, 48 + y, "C"); 
  display->drawCircle(78 + x, 49 + y, 2);

  display->setFont(DSEG7_Classic_Regular_15);
  display->drawString(77 + x, 15 + y, String(DecimalPart));
  
  display->drawIco16x16(97 + x, 14 + y, LockFlag ? lock_open_icon16x16 : lock_closed_icon16x16); //Power powerbutton_icon16x16   || lock_closed_icon16x16 or lock_open_icon16x16 or door_icon16x16 if door is open || warning_icon16x16
  display->drawIco16x16(97 + x, 31 + y, LightFlag ? bulb_off_icon16x16 : bulb_icon16x16);    //Light    || or bulb_icon16x16
  display->drawIco16x16(97 + x, 48 + y, FanFlag ? humidity_icon16x16 : humidity2_icon16x16);    //Water    || or humidity2_icon16x16

  
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->setFont(ArialMT_Plain_10);
  //TODO Flag changer! Fix names!
  display->drawString(128 + x, 16 + y, LockFlag ? "On":"Off"); //Lock
  display->drawString(128 + x, 33 + y, LightFlag ? "On":"Off"); //Light
  display->drawString(128 + x, 50 + y, FanFlag ? "On":"Off");  //Water
}

void ExtraFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  // Text alignment demo
  display->setFont(ArialMT_Plain_10);
  // The coordinates define the left starting point of the text
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(0 + x, 11 + y, "Left aligned (0,10)");
  // The coordinates define the center of the text
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 22 + y, "Center aligned (64,22)");
  // The coordinates define the right end of the text
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->drawString(128 + x, 33 + y, "Right aligned (128,33)");
}

FrameCallback frames[] = {MainFrame, ExtraFrame};

int frameCount = 2;
OverlayCallback overlays[] = { msOverlay };
int overlaysCount = 1;

void GUI( void * parameter)
{
    Serial.println("GUI");

    /*    #1 GUI oled control       #2 Buttons control    */
  
    ui.setTargetFPS(30);

    ui.disableAutoTransition();
    ui.disableAllIndicators();

    ui.setFrames(frames, frameCount);
    ui.setOverlays(overlays, overlaysCount);
    
    ui.init();
    //display.flipScreenVertically();

    int remainingTimeBudget = 0;
    uint8_t oldData = 0;

    while(1){        
        remainingTimeBudget = ui.update();
        if (remainingTimeBudget > 0){  vTaskDelay(remainingTimeBudget/portTICK_PERIOD_MS); 
        if(oldData!=contrast){oldData=contrast; display.setBrightness(contrast);}}
    }
    
    vTaskDelete( NULL );
}