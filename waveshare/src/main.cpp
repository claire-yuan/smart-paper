#include <Arduino.h>
#include <SPI.h>
#include "epd7in5_V2.h"
#include "imagedata.h"


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Epd epd;
  Serial.print("e-Paper init \r\n ");
  if (epd.Init() != 0) {
      Serial.print("e-Paper init failed\r\n ");
      return;
  }

  delay(1000);

  Serial.print("e-Paper Display\r\n ");
  epd.Displaypart(IMAGE_DATA,250, 200,240,103);
  
  Serial.print("Displayed\r\n ");

  delay(5000);

  Serial.print("e-Paper Clear\r\n ");
  epd.Clear();

  epd.Sleep();

  Serial.print("End of setup function\r\n ");
}

void loop() {
}