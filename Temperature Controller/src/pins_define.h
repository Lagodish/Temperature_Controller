//Pins Config for my dev board (VineBox)
#define R 16  
#define G 5  //	outputs PWM signal at boot 
#define Bl 17
#define W 18
#define F1 33
#define F2 27   // ADC2 !
#define DHT 35   //input only
#define Relay 19
#define Heater 11 // NOT WORKING 	connected to the integrated SPI flash
#define EndStop 34  //input only
#define Comp 32
#define Beeper 8  // NOT WORKING 	connected to the integrated SPI flash
#define GP1 36    //input only
#define GP2 39    //input only
#define GP3 2   // ADC2 !  connected to on-board LED
#define GP4 4   // ADC2 !
#define Dac1 26 // ADC2 !
#define Dac2 25 // ADC2 !
#define CLK 14  // ADC2 !  outputs PWM signal at boot
#define MISO 12 // ADC2 !  boot fail if pulled high
#define MOSI 13 // ADC2 !
#define CS 15   // ADC2 !  outputs PWM signal at boot
#define RX 10  // NOT WORKING  	connected to the integrated SPI flash
#define TX 9   // NOT WORKING 	connected to the integrated SPI flash
#define FS1 7  // NOT WORKING   connected to the integrated SPI flash
#define FS2 6  // NOT WORKING  	connected to the integrated SPI flash
#define SCL 22
#define SDA 21
#define ESP32_TX 1   //debug output at boot 
#define ESP32_RX 22
#define DS18B20 23

/*
        ADC2_CH0 (GPIO 4)
        ADC2_CH1 (GPIO 0)
        ADC2_CH2 (GPIO 2)
        ADC2_CH3 (GPIO 15)
        ADC2_CH4 (GPIO 13)
        ADC2_CH5 (GPIO 12)
        ADC2_CH6 (GPIO 14)
        ADC2_CH7 (GPIO 27)
        ADC2_CH8 (GPIO 25)
        ADC2_CH9 (GPIO 26)

    Note: ADC2 pins cannot be used when Wi-Fi is used.
    So, if you’re using Wi-Fi and you’re having trouble getting the value from an ADC2 GPIO, you may consider using an ADC1 GPIO instead.
    That should solve your problem.
*/

//Light
    uint8_t briR = 0;
    uint8_t briG = 0;
    uint8_t briB = 0;
    uint8_t briW = 0;