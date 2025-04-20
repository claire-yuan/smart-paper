#include <Arduino.h>
#include <SPI.h>
#include "epd7in5_V2.h"
#include "imagedata.h"
#include "touch.h"

Touch touch;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);

  
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
  
  touch.touch_init();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int x_pos = touch.get_X_position();
  int y_pos = touch.get_Y_position();
  //if (MAP_POSITION_VALUES == false) {
    Serial.print(x_pos);
    Serial.print(",");
    Serial.println(y_pos);
  //}

}
