#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Source: https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/

#define SERVICE_UUID        "22222222-2222-2222-2222-222222222222"
#define CHARACTERISTIC_UUID "66666666-6666-6666-6666-666666666666"

class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    Serial.print("Received: ");
    for (size_t i = 0; i < value.length(); i++) {
      Serial.printf("%02X ", (uint8_t)value[i]);
    }
    Serial.println();
  }
};

BLECharacteristic *pCharacteristic;

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
  pCharacteristic->setValue("Hello from the ESP32C3. 11000000101000001001000010001000100001001000001011000000"); // send message from ESP32 to laptop
  delay(1000);
}