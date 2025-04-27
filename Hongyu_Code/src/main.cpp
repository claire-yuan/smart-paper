#include <Arduino.h>
#include <SPI.h>
#include "epd7in5_V2.h"
#include "imagedata.h"
#include "touch.h"
#include "epdpaint.h"

int counter = 0;
int x_offset = 0;
int y_offset = 0;
int x_prev = 0;
int y_prev = 0;
int radius = 2;
bool refreshed = true;

unsigned char* image = (unsigned char *)malloc(EPD_WIDTH * EPD_HEIGHT / 8);
unsigned char* circle = (unsigned char*)malloc((2*radius + 1)*(2*radius + 1)/8);
Paint paint(image, EPD_WIDTH, EPD_HEIGHT);
Paint circ(circle, 2*radius+1, 2*radius+1);

Touch touch; 
Epd epd;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(500);

 
  Serial.print("e-Paper init \r\n ");
  if (epd.Init() != 0) {
      Serial.print("e-Paper init failed\r\n ");
      return;
  }

  delay(500);

  Serial.print("e-Paper Display\r\n ");
  epd.Displaypart(IMAGE_DATA,250, 200,240,103);
  
  Serial.print("Displayed\r\n ");

  delay(1000);

  Serial.print("e-Paper Clear\r\n ");
  epd.Clear();

  epd.Sleep();

  
  touch.touch_init();
  circ.DrawFilledCircle(radius+1, radius+1, radius, 0);
  paint.Clear(1);

  Serial.print("e-Paper init \r\n ");
  if (epd.Init() != 0) {
      Serial.print("e-Paper init failed\r\n ");
      return;
  }

  Serial.print("End of setup function\r\n ");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int x_pos = touch.get_X_position();
  int y_pos = touch.get_Y_position();

    x_pos = x_pos - x_offset - radius;
    y_pos = y_pos - y_offset - radius;

if(x_pos > EPD_WIDTH){
    x_pos = EPD_WIDTH;
  }
if(y_pos > EPD_HEIGHT){
    y_pos = EPD_HEIGHT;
  }
if(x_pos < 0){
  x_pos = 0;
}
if(y_pos < 0){
  y_pos = 0;
}

 //if (MAP_POSITION_VALUES == false) {
    Serial.print(x_pos);
    Serial.print(",");
    Serial.println(y_pos);
  //}

  if(x_pos != 0 && (x_pos != x_prev || y_pos != y_prev)){
    paint.DrawFilledCircle(x_pos, y_pos, radius, 0);
    //epd.Displaypart(circle, x_pos-x_offset-radius, y_pos-y_offset-radius, 2*radius+1, 2*radius+1);
   refreshed = false;
    x_prev = x_pos;
    y_prev = y_pos;
    counter++;
  }

  if(x_pos == 0 && y_pos == 480 && refreshed == false){
     epd.Displaypart(image, 0, 0, EPD_WIDTH, EPD_HEIGHT);
     refreshed = true;
     counter = 0;
  }
  /*
  if(counter > 10){
    epd.Clear();
    epd.Displaypart(image, 0, 0, EPD_WIDTH, EPD_HEIGHT);
    counter = 0;
  }
  */

}
