#!/usr/bin/env python

import asyncio

from dishwasher import Machine
from fakeSerial import FakeSerial
from comProtocol import comProtocol
from dishwasher import Machine
from shell import Shell

dishwasher = Machine()
serial = FakeSerial()
com = comProtocol(dishwasher)
shell = Shell(dishwasher, com)

async def main():
    print("Dishwasher Simulator")

    # Set function to be called upon serial packets reception
    serial.set_callback(com.process_packet)

    # Set function to be called by the communication module to send packets
    com.set_response_callback(serial.send)

    shellTask = asyncio.create_task(shell.run())
    communicationTask = asyncio.create_task(serial.run())

    await communicationTask
    await shellTask


if __name__ == "__main__":
    asyncio.run(main())