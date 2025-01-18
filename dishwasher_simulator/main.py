#!/usr/bin/env python

import asyncio

from fakeSerial import FakeSerial
from comProtocol import comProtocol

serial = FakeSerial()
com = comProtocol()

async def main():
    print("Dishwasher Simulator")

    # Set function to be called upon serial packets reception
    serial.set_callback(com.process_packet)

    # Set function to be called by the communication module to send packets
    com.set_response_callback(serial.send)

    communicationTask = asyncio.create_task(serial.run())

    await communicationTask


if __name__ == "__main__":
    asyncio.run(main())