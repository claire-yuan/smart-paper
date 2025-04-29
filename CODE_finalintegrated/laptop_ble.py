import asyncio
from bleak import BleakScanner, BleakClient
import pathlib
import os

# Source: https://getwavecake.com/blog/getting-started-with-bluetooth-and-python-part-2/

class DeviceBle():
    
    def __init__(self):
        self.client = None
        self.SERVICE_UUID = "22222222-2222-2222-2222-222222222222"
        self.CHARACTERISTIC_UUID = "66666666-6666-6666-6666-666666666666"

    async def connect(self):
        print("Scanning for ESP32...")
        devices = await BleakScanner.discover()
        esp = next((d for d in devices if "ESP32C3-BLE" in (d.name or "")), None)
        if esp is not None:
            try:
                print("Found device at address: %s" % (esp))
                print("Attempting to connect...")
                self.client = BleakClient(esp)
                await self.client.connect()
                print("Connected")
            except:
                raise Exception("Failed to connect")
        else:
            raise Exception("Did not find ESP")
    
    async def disconnect(self):
        try:
            print("Disconnecting...")
            await self.client.disconnect()
            print("Disconnected!")
        except:
            raise Exception("Warning: Failed to disconnect. Check for hanging connection")
        
    async def read_characteristic(self):
        while True:
            try:
                data = await self.client.read_gatt_char(self.CHARACTERISTIC_UUID)
                print("Received:", data)
            except:
                raise Exception("Failed to read characteristic.")
            
            await asyncio.sleep(1)


    async def send_message(self, message):
        CHUNK_SIZE = 20  # Number of bytes to send at once

        print(message)
        for i in range(0, len(message), CHUNK_SIZE):
            chunk = message[i:i + CHUNK_SIZE] # Python handles slicing well so don't need to work on index overflow
            print(chunk)

            await self.client.write_gatt_char(self.CHARACTERISTIC_UUID, bytes(chunk))
            await asyncio.sleep(0.01)  # Avoid BLE overload


async def main():

    ## june - i am getting the data to import

    # Get current script's folder
    script_location = pathlib.Path(__file__).parent

    # get data folder
    data_location = script_location / "files"

    # single file example
    output_file = data_location / "output.txt"

    # Open the hex file and read its contents
    with open(output_file, "r") as file:
        hex_data = file.read()

    # Clean the data by removing unwanted characters (commas, newlines, etc.)
    hex_values = hex_data.replace(",", "").split()  # Remove commas and split by space/newline

    img = [int(h, 16) for h in hex_values]  # convert hex string -> int
    print(img)

    ## testing
    pikachu = data_location / "pikachu.txt"
    with open(pikachu, 'w') as f:
        f.write(str(img))

    device = DeviceBle()
    try: 
        await device.connect()
        
        await device.send_message(img)

        await device.read_characteristic()

    except Exception as e:
        print(e)



asyncio.run(main())