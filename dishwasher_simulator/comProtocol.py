PROTOCOL_VERSION_MAJOR = 1
PROTOCOL_VERSION_MINOR = 0

from cobs import cobs
from crc8 import crc8

class comProtocol:
    def __init__(self):
        self.sequence = 0
        self.ignore_sequence_number = False

    def set_response_callback(self, callback):
        self.__response_callback = callback

    async def process_packet(self, data):
        data = cobs.decode(data[:-1])

        # Check CRC
        crc = data[0]
        to_check = data[1:]
        calculated = crc8(to_check).digest()
        calculated = int.from_bytes(calculated)
        if(crc != calculated):
            print("CRC Failed: " + str(crc) + " != " + str(calculated))
            await self.__response_callback(b'\x00')
            return

        # Check sequence
        if(not self.ignore_sequence_number):
            sequence = data[2]
            if(sequence < self.sequence):
                await self.__response_callback(b'\x00')
                return

        # Check request
        match data[3]:
            case _:
                await self.__response_callback(b'\x00')

        if(not self.ignore_sequence_number):
            if(self.sequence == 255):
                self.sequence = 0
            else:
                self.sequence = sequence + 1
