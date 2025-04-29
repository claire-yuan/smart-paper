#include <Arduino.h>
#include <SPI.h>

#define PIN_NUM_MISO 2
#define PIN_NUM_MOSI 7
#define PIN_NUM_CLK 6
#define PIN_NUM_TSC_CS 10
#define PIN_NUM_EPD_CS 3

//
const bool MAP_POSITION_VALUES = false;

// SETTINGS FOR TSC2046 SAMPLING
const int TOUCH_N_SAMPLES = 3;
const int TOUCH_SAMPLE_DELAY = 1; // ms

// Function declarations
int get_battery();
int get_X_position();
int get_Y_position();

// TOUCH OVERLAY ORIGINAL COORDINATES BEFORE MAPPING
int BL_X = 790;
int BL_Y = 790;

int BR_X = 1190;
int BR_Y = 650;

int TL_X = 580;
int TL_Y = 1300;

int TR_X = 1410;
int TR_Y = 1430;



void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);
  delay(1000);
  
  // Set up SPI pins
  SPI.begin(PIN_NUM_CLK, PIN_NUM_MISO, PIN_NUM_MOSI, PIN_NUM_TSC_CS);  // Using custom pins
  pinMode(PIN_NUM_TSC_CS, OUTPUT);
  pinMode(PIN_NUM_EPD_CS, OUTPUT);

  // ~CS pulled high when inactive
  digitalWrite(PIN_NUM_TSC_CS, HIGH);
  digitalWrite(PIN_NUM_EPD_CS, HIGH);

  // Set SPI settings: SPI mode 0, max clock speed 1 MHz
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0));

  Serial.println("SPI Initialized");
}

void loop() {
  unsigned long milliseconds = millis();
  unsigned long seconds = milliseconds / 1000;
  Serial.print(seconds);
  Serial.print(",");
  Serial.print(get_battery());
  Serial.print(",");
  Serial.print(get_X_position());
  Serial.print(",");
  Serial.println(get_Y_position());
  delay(10000);
}

/* Function to get battery voltage from the TSC2046*/
int get_battery() {
  digitalWrite(PIN_NUM_TSC_CS, LOW);

  // Send and receive data via SPI
  SPI.transfer(0b10100111);
  byte byte1 = SPI.transfer(0);
  byte byte2 = SPI.transfer(0);
  
  // Deselect the SPI device (pull CS high)
  digitalWrite(PIN_NUM_TSC_CS, HIGH);

  // Add position value to sum (later to be averaged)
  return ((uint16_t)byte1 << 4) | (byte2 >> 4); 
}

/* Function to get X position from the TSC2046*/
int get_X_position() {
  // Select the SPI device (pull CS low)
  int x_sum = 0;
  for (int i = 0; i < TOUCH_N_SAMPLES; i++) {
    digitalWrite(PIN_NUM_TSC_CS, LOW);

    // Send and receive data via SPI
    SPI.transfer(0b11010000);
    byte byte1 = SPI.transfer(0);
    byte byte2 = SPI.transfer(0);
    
    // Deselect the SPI device (pull CS high)
    digitalWrite(PIN_NUM_TSC_CS, HIGH);

    // Add position value to sum (later to be averaged)
    x_sum += ((uint16_t)byte1 << 4) | (byte2 >> 4); 

    delay(TOUCH_SAMPLE_DELAY); 
  }
  return x_sum/TOUCH_N_SAMPLES;

}

/* Function to get Y position from the TSC2046*/
int get_Y_position() {
  int y_sum = 0;
  for (int i = 0; i < TOUCH_N_SAMPLES; i++) {
  
    // Select the SPI device (pull CS low)
    digitalWrite(PIN_NUM_TSC_CS, LOW); 

    // Send and receive data via SPI
    SPI.transfer(0b10010000);
    byte byte1 = SPI.transfer(0);
    byte byte2 = SPI.transfer(0);
    
    // Deselect the SPI device (pull CS high)
    digitalWrite(PIN_NUM_TSC_CS, HIGH);

    // Add position value to sum (later to be averaged)
    y_sum += ((uint16_t)byte1 << 4) | (byte2 >> 4);

    delay(TOUCH_SAMPLE_DELAY);
  }  
  
  return y_sum/TOUCH_N_SAMPLES;
}
