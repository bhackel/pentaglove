# Tests a bluetooth connection for a given characteristic and MAC address

import asyncio
import platform
import sys

from bleak import BleakClient
from bleak.backends.characteristic import BleakGATTCharacteristic


# you can change these to match your device or override them from the command line
CHARACTERISTIC_UUID = "00002a19-0000-1000-8000-00805f9b34fb"
ADDRESS = "07:65:06:7F:9D:02"


def notification_handler(characteristic: BleakGATTCharacteristic, data: bytearray):
    """Simple notification handler which prints the data received."""
    print(f"{characteristic.description}: {int.from_bytes(data, 'little')}")


async def main(address, char_uuid):
    async with BleakClient(address) as client:
        print(f"Connected: {client.is_connected}")

        await client.start_notify(char_uuid, notification_handler)
        await asyncio.sleep(5.0)
        await client.stop_notify(char_uuid)


if __name__ == "__main__":
    asyncio.run(main(ADDRESS, CHARACTERISTIC_UUID))

