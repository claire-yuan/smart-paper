#include <Arduino.h>
#include <SPI.h>
#include "epd7in5_V2.h"
#include "imagedata.h"
#include "touch.h"
#include "epdpaint.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
// #include <ble.h>


// E-PAPER GLOBAL VARIABLES
int BL_X = 0;
int BL_Y = 0;

int BR_X = 0;
int BR_Y = 0;

int TL_X = 0;
int TL_Y = 0;

int TR_X = 0;
int TR_Y = 0;

int counter = 0;
int x_offset = 450;
int y_offset = 500;
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

// BLE VARIABLES
#define SERVICE_UUID        "22222222-2222-2222-2222-222222222222"
#define CHARACTERISTIC_UUID "66666666-6666-6666-6666-666666666666"

BLECharacteristic *pCharacteristic;
uint16_t chars_received = 0;


class MyCallbacks: public BLECharacteristicCallbacks {

  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    
    for (size_t i = 0; i < value.length(); i++) {
      IMAGE_DATA[chars_received] = (uint8_t)value[i];
      chars_received++;
    }
  }

};
 

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
  
  touch.touch_init();
  circ.DrawFilledCircle(radius+1, radius+1, radius, 0);
  paint.Clear(1);
  /*
  Serial.print("Touch Top Left Corner \r\n");
  while(touch.get_X_position() == 0 && touch.get_Y_position() == 2047){
  }
  TL_X = touch.get_X_position();
  TL_Y = touch.get_Y_position();
  delay(1000);

  Serial.print("Touch Top Right Corner \r\n");
  while(touch.get_X_position() == 0 && touch.get_Y_position() == 2047){
  }
  TR_X = touch.get_X_position();
  TR_Y = touch.get_Y_position();
  delay(1000);

  Serial.print("Touch Bottom Left Corner \r\n");
  while(touch.get_X_position() == 0 && touch.get_Y_position() == 2047){
  }
  BL_X = touch.get_X_position();
  BL_Y = touch.get_Y_position();
  delay(1000);

  Serial.print("Touch Bottom Right Corner \r\n");
  while(touch.get_X_position() == 0 && touch.get_Y_position() == 2047){
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
  */
  
  // ------------------- BLE INIT ----------------------------
  Serial.println("BLE Init");
  
  BLEDevice::init("ESP32C3-BLE");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
  );

  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->start();

  // pCharacteristic->setValue("Hello from the ESP32C3"); // send message from ESP32 to laptop
  Serial.println("BLE Server started, waiting for client...");


  // END
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



  // BLE
  if (chars_received == 3090) {
    chars_received = 0;
    Serial.println("All chars received: "); 

    for (int i = 0; i < 3090; i++) {
      Serial.printf("%02X ", IMAGE_DATA[i]);
    }

    // Call function to update screen
    epd.Displaypart(IMAGE_DATA,250, 200,240,103);
  }
}