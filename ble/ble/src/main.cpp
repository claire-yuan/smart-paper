#include <Arduino.h>
#include <imagedata.h>
#include <ble.h>

// Source: https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/

#define SERVICE_UUID        "22222222-2222-2222-2222-222222222222"
#define CHARACTERISTIC_UUID "66666666-6666-6666-6666-666666666666"


void setup() {
  Serial.begin(115200);
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

  Serial.println("BLE Server started, waiting for client...");
}


void loop() {
  pCharacteristic->setValue("Hello from the ESP32C3"); // send message from ESP32 to laptop
  
  if (chars_received == 3090) {
    chars_received = 0;
    Serial.println("All chars received: "); 

    for (int i = 0; i < 3090; i++) {
      Serial.printf("%02X ", IMAGE_DATA[i]);
    }

    // Call function to update screen
  }

  delay(1000);
}