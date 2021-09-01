#include <Arduino.h> 
//#include "SSD1306Wire.h"
#include "SH1106Wire.h"
// Include the UI lib
#include "OLEDDisplayUi.h"

// Include custom images
#include "images.h"
#include "font.h"

extern float tempC;
extern uint8_t contrast;

// Initialize the OLED display using Wire library
SH1106Wire display(0x3c, 21, 22, GEOMETRY_128_64, I2C_ONE, 400000); //set I2C frequency to 400kHz
//SH1106Wire display(0x3c, 21, 22);

OLEDDisplayUi ui     ( &display );

void msOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(0, 0, "UpTime: "+String(float(millis())/(60 * 60 * 1000))+" h");
}

void drawFrame1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  // draw an xbm image.
  // Please note that everything that should be transitioned
  // needs to be drawn relative to x and y

  display->drawXbm(x + 34, y + 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}

void drawFrame2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  //display->setFont(DSEG14_Classic_Regular_15);
  //display->drawString(0 + x, 10 + y, "Arial 10");

  int IntegerPart = (int)(tempC);
  int DecimalPart = 10 * (tempC - IntegerPart);

  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->setFont(DSEG7_Classic_Regular_50);
  display->drawString(80 + x, 10 + y, String(IntegerPart));

  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(DSEG14_Classic_Regular_10);
  display->drawString(85 + x, 50 + y, "C"); 
  display->drawCircle(81 + x, 49 + y, 2);

  display->setFont(DSEG7_Classic_Regular_15);
  display->drawString(80 + x, 15 + y, String(DecimalPart));
  // display->setFont(ArialMT_Plain_24);
  // display->drawString(0 + x, 34 + y, "Arial 24");
}

void drawFrame3(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
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

void drawFrame4(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  display->drawStringMaxWidth(0 + x, 10 + y, 128, "Lorem ipsum\n dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore.");
}

// This array keeps function pointers to all frames
// frames are the single views that slide in
FrameCallback frames[] = { drawFrame2, drawFrame3, drawFrame1};

int frameCount = 3;
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