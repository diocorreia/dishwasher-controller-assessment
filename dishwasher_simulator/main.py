#!/usr/bin/env python

import asyncio

from fakeSerial import FakeSerial

serial = FakeSerial()

async def main():
    print("Dishwasher Simulator")

    communicationTask = asyncio.create_task(serial.run())

    await communicationTask


if __name__ == "__main__":
    asyncio.run(main())