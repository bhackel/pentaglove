# Desktop application for the glove project
# Reads the current glove state over bluetooth, then clicks in a specific
# position on the screen to activate some predefined command

import asyncio
import platform
import sys
import keyboard
import mouse

from bleak import BleakClient
from bleak.backends.characteristic import BleakGATTCharacteristic


# Set UUID values for the device and the characteristic
ADDRESS = "07:65:06:7F:9D:02"
CHARACTERISTIC_UUID = "00002a19-0000-1000-8000-00805f9b34fb"


def notification_handler(characteristic: BleakGATTCharacteristic, data: bytearray):
    """Receives the data, parses it, and calls a method with the number"""
    value = int.from_bytes(data, 'little')
    print(f"Glove State: {value}")
    action(value)


async def main(address, char_uuid):
    async with BleakClient(address) as client:
        print(f"Connected: {client.is_connected}")

        await client.start_notify(char_uuid, notification_handler)
        await asyncio.sleep(123456789.0)
        await client.stop_notify(char_uuid)


def action(num):
    """Performs a mouse movement based on the given number"""
    match num:
        case 1:
            mouse.move("270", "80")
            mouse.click()
        case 2:
            mouse.move("230", "80")
            mouse.click()
        case 3:
            mouse.move("1000", "80")
            mouse.click()
        case 4:
            mouse.move("1200", "80")
            mouse.click()
        case 5:
            mouse.move("25", "80")
            mouse.click()
        case 6:
            mouse.move("69", "80")
            mouse.click()


if __name__ == "__main__":
    asyncio.run(main(ADDRESS, CHARACTERISTIC_UUID))

