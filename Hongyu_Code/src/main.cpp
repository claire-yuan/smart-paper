#include <Arduino.h>
#include <SPI.h>
#include "epd7in5_V2.h"
#include "imagedata.h"
#include "touch.h"
#include "epdpaint.h"

bool calibrate = false;
int BL_X = 0;
int BL_Y = 0;

int BR_X = 0;
int BR_Y = 0;

int TL_X = 0;
int TL_Y = 0;

int TR_X = 0;
int TR_Y = 0;

double h11 = 0.9734;
double h12 = -0.2997;
double h13 = 61.1876;
double h21 = 0.2997;
double h22 = 0.9734;
double h23 = -859.1649;
double h31 = 0;
double h32 = 0;
double h33 = 1;

int counter = 0;
int x_offset = 0;
int y_offset = 0;
int x_prev = 0;
int y_prev = 0;
int radius = 0;
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

  delay(500);

  Serial.print("e-Paper Clear\r\n ");
  epd.Clear();
  
  touch.touch_init();
  circ.DrawFilledCircle(radius+1, radius+1, radius, 0);
  paint.Clear(1);
  
  if(calibrate == true){
    //paint.DrawFilledCircle(10, 10, 5, 0);
    paint.DrawFilledCircle(10, 470, 5, 0);
    paint.DrawFilledCircle(790, 10, 5, 0);
    //paint.DrawFilledCircle(790, 470, 5, 0);
    epd.Displaypart(image, 0, 0, EPD_WIDTH, EPD_HEIGHT);
    
    Serial.print("Touch Top Left Corner \r\n");
    while(touch.get_X_position() == 0){
    }
    TL_X = touch.get_X_position();
    TL_Y = touch.get_Y_position();
    delay(1000);

    Serial.print("Touch Top Right Corner \r\n");
    while(touch.get_X_position() == 0){
    }
    TR_X = touch.get_X_position();
    TR_Y = touch.get_Y_position();
    delay(1000);

    Serial.print("Touch Bottom Left Corner \r\n");
    while(touch.get_X_position() == 0){
    }
    BL_X = touch.get_X_position();
    BL_Y = touch.get_Y_position();
    delay(1000);

    Serial.print("Touch Bottom Right Corner \r\n");
    while(touch.get_X_position() == 0){
    }
    BR_X = touch.get_X_position();
    BR_Y = touch.get_Y_position();
    delay(1000);

    Serial.print("TL: ");
    Serial.print(TL_X);
    Serial.print(",");
    Serial.println(TL_Y);

    Serial.print("TR: ");
    Serial.print(TR_X);
    Serial.print(",");
    Serial.println(TR_Y);

    Serial.print("BL: ");
    Serial.print(BL_X);
    Serial.print(",");
    Serial.println(BL_Y);

    Serial.print("BR: ");
    Serial.print(BR_X);
    Serial.print(",");
    Serial.println(BR_Y);
    paint.Clear(1);
    epd.Clear();
  }
  Serial.print("End of setup function\r\n ");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int x = touch.get_X_position();
  int y = touch.get_Y_position();
  int norm = (h31*x + h32*y + h33);
   int x_pos = (h11*x + h12*y + h13)/norm;
   int y_pos = (h21*x + h22*y + h23)/norm;

   /*
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
*/
 if (calibrate == false) {
    //Serial.print(x_pos);
    //Serial.print(",");
    //Serial.println(y_pos);
  }

  if(x != 0 && (x_pos != x_prev || y_pos != y_prev)){
    paint.DrawFilledCircle(x_pos, y_pos, radius, 0);
    //epd.Displaypart(circle, x_pos-x_offset-radius, y_pos-y_offset-radius, 2*radius+1, 2*radius+1);
    refreshed = false;
    x_prev = x_pos;
    y_prev = y_pos;
    counter++;
  }

  if(x == 0 && refreshed == false){
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
