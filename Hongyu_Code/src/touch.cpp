#include <Arduino.h>
#include <SPI.h>
#include "touch.h"

#ifndef PIN_NUM_MISO
#define PIN_NUM_MISO 2
#endif

#ifndef PIN_NUM_MOSI
#define PIN_NUM_MOSI 7
#endif

#ifndef PIN_NUM_CLK
#define PIN_NUM_CLK 6
#endif

#ifndef CS_TOUCH
#define CS_TOUCH 10
#endif

//
const bool MAP_POSITION_VALUES = false;

// SETTINGS FOR TSC2046 SAMPLING
const int TOUCH_N_SAMPLES = 10;
const int TOUCH_SAMPLE_DELAY = 0; // ms

// TOUCH OVERLAY ORIGINAL COORDINATES BEFORE MAPPING
/*
int BL_X = 790;
int BL_Y = 790;

int BR_X = 1190;
int BR_Y = 650;

int TL_X = 580;
int TL_Y = 1300;

int TR_X = 1410;
int TR_Y = 1430;

int BL_X = 726;
int BL_Y = 826;

int BR_X = 608;
int BR_Y = 1192;

int TL_X = 1162;
int TL_Y = 662;

int TR_X = 1345;
int TR_Y = 1407;
*/

// Constructor
Touch::Touch() {
};

// Deconstructor
Touch::~Touch() {
};


/* Function to get X position from the TSC2046*/
int Touch::get_X_position(void) {
  // Select the SPI device (pull CS low)
  int x_sum = 0;
  for (int i = 0; i < TOUCH_N_SAMPLES; i++) {
    digitalWrite(CS_TOUCH, LOW);
    // delay(10);

    // Send and receive data via SPI
    SPI.transfer(0b11010000);
    byte byte1 = SPI.transfer(0);
    byte byte2 = SPI.transfer(0);
    
    // Deselect the SPI device (pull CS high)
    digitalWrite(CS_TOUCH, HIGH);

    // Add position value to sum (later to be averaged)
    x_sum += (byte1 << 4) | (byte2 >> 4); 

   //delay(TOUCH_SAMPLE_DELAY); 
  }
  return x_sum/TOUCH_N_SAMPLES;

}

/* Function to get Y position from the TSC2046*/
int Touch::get_Y_position(void) {
  int y_sum = 0;
  for (int i = 0; i < TOUCH_N_SAMPLES; i++) {
  
    // Select the SPI device (pull CS low)
    digitalWrite(CS_TOUCH, LOW); 

    // Send and receive data via SPI
    SPI.transfer(0b10010000);
    byte byte1 = SPI.transfer(0);
    byte byte2 = SPI.transfer(0);
    
    // Deselect the SPI device (pull CS high)
    digitalWrite(CS_TOUCH, HIGH);

    // Add position value to sum (later to be averaged)
    y_sum += (byte1 << 4) | (byte2 >> 4);

    //delay(TOUCH_SAMPLE_DELAY);
  }  
  
  return y_sum/TOUCH_N_SAMPLES;
}

void Touch::touch_init(void){
     // Set up SPI pins
   SPI.begin(PIN_NUM_CLK, PIN_NUM_MISO, PIN_NUM_MOSI, CS_TOUCH);  // Using custom pins
   pinMode(CS_TOUCH, OUTPUT);
 
   // ~CS pulled high when inactive
   digitalWrite(CS_TOUCH, HIGH);
 
   // Set SPI settings: SPI mode 0, max clock speed 1 MHz
   //SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
 
   Serial.println("SPI Initialized");
}
