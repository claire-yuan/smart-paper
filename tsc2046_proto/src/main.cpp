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
  byte dataToSend = 0b10111001;  // Example byte to send
  byte receivedData = 0x11;  // Byte to receive
    
  // Select the SPI device (pull CS low)
  digitalWrite(PIN_NUM_CS, LOW);
  // delay(10);

  // Send and receive data via SPI
  SPI.transfer(dataToSend);
  delay(10);
  receivedData = SPI.transfer(0);
  
  // Deselect the SPI device (pull CS high)
  digitalWrite(PIN_NUM_CS, HIGH);
  
  // Print the received data
  // Serial.print("Sent Data: 0x");
  // Serial.print(dataToSend, HEX);
  // Serial.print(" | Received Data: 0x");
  Serial.println(receivedData);

  delay(100); 
}