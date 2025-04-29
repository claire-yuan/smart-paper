#include <ble.h>
#include <imagedata.h>

class MyCallbacks: public BLECharacteristicCallbacks {

    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();
      
      for (size_t i = 0; i < value.length(); i++) {
        IMAGE_DATA[chars_received] = (uint8_t)value[i];
        chars_received++;
      }
    }

};
   
  