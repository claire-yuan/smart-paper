#include <Arduino.h>
#include <SPI.h>

#define PIN_NUM_MISO 2
#define PIN_NUM_MOSI 7
#define PIN_NUM_CLK 6
#define PIN_NUM_CS 10

void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);
  delay(1000);
  
  // Set up SPI pins
  SPI.begin(PIN_NUM_CLK, PIN_NUM_MISO, PIN_NUM_MOSI, PIN_NUM_CS);  // Using custom pins
  pinMode(PIN_NUM_CS, OUTPUT);

  // ~CS pulled high when inactive
  digitalWrite(PIN_NUM_CS, HIGH);

  // Set SPI settings: SPI mode 0, max clock speed 1 MHz
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

  Serial.println("SPI Initialized");
}

void loop() {
  // ------------------- REQUESTING Y-POSITION --------------------------------------
  byte request_Y = 0b10010001;  // Example byte to send
  byte receivedData = 0x11;  // Byte to receive
    
  // Select the SPI device (pull CS low)
  digitalWrite(PIN_NUM_CS, LOW);
  // delay(10);

  // Send and receive data via SPI
  SPI.transfer(request_Y);
  byte byte1 = SPI.transfer(0);
  byte byte2 = SPI.transfer(0);
  
  // Deselect the SPI device (pull CS high)
  digitalWrite(PIN_NUM_CS, HIGH);
  // Serial.println(byte1);
  // Serial.println(byte2);

  int y_position = (byte1 << 4) | (byte2 >> 4);

  // ------------------- REQUESTING X-POSITION --------------------------------------

  byte request_X = 0b11010001;  // Example byte to send
  // Select the SPI device (pull CS low)
  digitalWrite(PIN_NUM_CS, LOW);
  // delay(10);

  // Send and receive data via SPI
  SPI.transfer(request_Y);
  byte1 = SPI.transfer(0);
  byte2 = SPI.transfer(0);
  
  // Deselect the SPI device (pull CS high)
  digitalWrite(PIN_NUM_CS, HIGH);
  // Serial.println(byte1);
  // Serial.println(byte2);

  int x_position = (byte1 << 4) | (byte2 >> 4);

  Serial.print(x_position);
  Serial.print(", ");
  Serial.println(y_position);

  delay(10); 
}