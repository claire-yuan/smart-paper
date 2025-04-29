#include <Arduino.h>
#include <SPI.h>
#include "epd7in5_V2.h"
#include "imagedata.h"
#include "touch.h"
#include "epdpaint.h"

bool calibrate = false;
int BL_X = 0;
int BL_Y = 0;
int BM_X = 0;
int BM_Y = 0;
int BR_X = 0;
int BR_Y = 0;

int ML_X = 0;
int ML_Y = 0;
int MM_X = 0;
int MM_Y = 0;
int MR_X = 0;
int MR_Y = 0;


int TL_X = 0;
int TL_Y = 0;
int TM_X = 0;
int TM_Y = 0;
int TR_X = 0;
int TR_Y = 0;

double h11 = 1.191431653564400;
double h12 = 0.176681037844232;
double h13 = -1745.667881077949;
double h21 = 0.262048140503375;
double h22 = 1.169083518440465;
double h23 = -2362.615906127927;
double h31 = 0.00007121571771299078;
double h32 = 0.0003443752248933555;
double h33 = 1;

int counter = 0;
int x_offset = 100;
int y_offset = -50;
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

  delay(500);

  Serial.print("e-Paper Clear\r\n ");
  epd.Clear();
  
  touch.touch_init();
  circ.DrawFilledCircle(radius+1, radius+1, radius, 0);
  paint.Clear(1);
  
  if(calibrate == true){
    paint.DrawFilledCircle(50, 50, 5, 0); //BL
    paint.DrawFilledCircle(50, 240, 5, 0); //BM
    paint.DrawFilledCircle(50, 430, 5, 0); //BR

    paint.DrawFilledCircle(400, 50, 5, 0); //ML
    paint.DrawFilledCircle(400, 240, 5, 0); //MM
    paint.DrawFilledCircle(400, 430, 5, 0); //MR

    paint.DrawFilledCircle(750, 50, 5, 0); //TL
    paint.DrawFilledCircle(750, 240, 5, 0); //TM
    paint.DrawFilledCircle(750, 430, 5, 0); //TR
    epd.Displaypart(image, 0, 0, EPD_WIDTH, EPD_HEIGHT);
    
    Serial.print("Touch Top Left Corner \r\n");
    while(touch.get_X_position() == 0){
    }
    TL_X = touch.get_X_position();
    TL_Y = touch.get_Y_position();
    delay(1000);

    Serial.print("Touch Top Middle \r\n");
    while(touch.get_X_position() == 0){
    }
    TM_X = touch.get_X_position();
    TM_Y = touch.get_Y_position();
    delay(1000);

    Serial.print("Touch Top Right Corner \r\n");
    while(touch.get_X_position() == 0){
    }
    TR_X = touch.get_X_position();
    TR_Y = touch.get_Y_position();
    delay(1000);

    Serial.print("Touch Middle Left \r\n");
    while(touch.get_X_position() == 0){
    }
    ML_X = touch.get_X_position();
    ML_Y = touch.get_Y_position();
    delay(1000);

    Serial.print("Touch Middle Middle \r\n");
    while(touch.get_X_position() == 0){
    }
    MM_X = touch.get_X_position();
    MM_Y = touch.get_Y_position();
    delay(1000);

    Serial.print("Touch Middle Right \r\n");
    while(touch.get_X_position() == 0){
    }
    MR_X = touch.get_X_position();
    MR_Y = touch.get_Y_position();
    delay(1000);

    Serial.print("Touch Bottom Left Corner \r\n");
    while(touch.get_X_position() == 0){
    }
    BL_X = touch.get_X_position();
    BL_Y = touch.get_Y_position();
    delay(1000);

    Serial.print("Touch Bottom Middle \r\n");
    while(touch.get_X_position() == 0){
    }
    BM_X = touch.get_X_position();
    BM_Y = touch.get_Y_position();
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

    Serial.print("TM: ");
    Serial.print(TM_X);
    Serial.print(",");
    Serial.println(TM_Y);

    Serial.print("TR: ");
    Serial.print(TR_X);
    Serial.print(",");
    Serial.println(TR_Y);

    Serial.print("ML: ");
    Serial.print(ML_X);
    Serial.print(",");
    Serial.println(ML_Y);

    Serial.print("MM: ");
    Serial.print(MM_X);
    Serial.print(",");
    Serial.println(MM_Y);
    
    Serial.print("MR: ");
    Serial.print(MR_X);
    Serial.print(",");
    Serial.println(MR_Y);

    Serial.print("BL: ");
    Serial.print(BL_X);
    Serial.print(",");
    Serial.println(BL_Y);

    Serial.print("BM: ");
    Serial.print(BM_X);
    Serial.print(",");
    Serial.println(BM_Y);

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
  double norm = h31*x + h32*y + h33;
  int x_pos = static_cast<int>((h11*x + h12*y + h13) / norm + 0.5);
  int y_pos = static_cast<int>((h21*x + h22*y + h23) / norm + 0.5);
  x_pos = x_pos - x_offset;
  y_pos = y_pos - y_offset;
    //x_pos = x_pos - x_offset;
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
if(calibrate == false){
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print(",");
  Serial.print(x_pos);
  Serial.print(",");
  Serial.println(y_pos);
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
   if ((x_pos<-50 && x_pos>-200) && (y_pos>520 && y_pos<600)){
    paint.Clear(1);
    epd.Clear();
   }
/*
  if(counter > 10){
    epd.Clear();
    epd.Displaypart(image, 0, 0, EPD_WIDTH, EPD_HEIGHT);
    counter = 0;
  }
 */
}
