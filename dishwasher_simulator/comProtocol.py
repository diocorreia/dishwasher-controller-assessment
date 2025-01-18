PROTOCOL_VERSION_MAJOR = 1
PROTOCOL_VERSION_MINOR = 0

from dishwasher import Machine, Error, Warning, Program
from cobs import cobs
from crc8 import crc8
from enum import Enum

class requestType(Enum):
    HELLO = 1
    GET_MACHINE_INFO = 2
    GET_STATUS = 3
    START_PROGRAM = 4
    ABORT_PROGRAM = 5

class comProtocol:
    def __init__(self, machine:Machine):
        self.sequence = 0
        self.machine = machine
        self.ignore_sequence_number = False
        self.insert_crc_error = False
        self.no_response = False

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
            case requestType.HELLO.value:
                await self.__on_hello_request()
            case requestType.GET_MACHINE_INFO.value:
                await self.__on_get_machine_info_request()
            case requestType.GET_STATUS.value:
                await self.__on_get_status_request()
            case requestType.START_PROGRAM.value:
                await self.__on_start_program_request(data[4])
            case _:
                await self.__response_callback(b'\x00')

        if(not self.ignore_sequence_number):
            if(self.sequence == 255):
                self.sequence = 0
            else:
                self.sequence = sequence + 1

    def __build_packet(self, payload):
        
        packet = b''

        packet_len = len(payload) + 3
        packet = packet_len.to_bytes() + self.sequence.to_bytes() + payload

        if(self.insert_crc_error is True):
            crc = crc8(packet)
            crc.update(b'\x01')
            crc = crc.digest()
        else:
            crc = crc8(packet).digest()

        packet = crc + packet
        packet = cobs.encode(packet) + b'\x00' 

        return packet

    async def __on_hello_request(self):

        payload = bytearray()
        payload.append(PROTOCOL_VERSION_MAJOR)
        payload.append(PROTOCOL_VERSION_MINOR)
        
        packet = self.__build_packet(payload)

        if(self.__response_callback is None):
            return
        
        if(self.no_response):
            return
        
        await self.__response_callback(packet)

    async def __on_get_machine_info_request(self):

        payload = b''
        info = self.machine.getInfo()
        payload += info[0].encode("ascii")
        payload += b'\x00'
        payload += info[1].encode("ascii")
        payload += b'\x00'
        payload += info[2].encode("ascii")
        payload += b'\x00'
        payload += info[3].encode("ascii")
        payload += b'\x00'
    
        packet = self.__build_packet(payload)

        if(self.__response_callback is None):
            return
        
        if(self.no_response):
            return
        
        await self.__response_callback(packet)

    async def __on_get_status_request(self):

        status = self.machine.getStatus()

        payload_msb = ((status['program'].value & 7) << 5)
        payload_msb |= ((status['step'].value & 7) << 2)
        payload_msb |= (status['state'].value & 3)

        payload_lsb = 0
        for warning in status['warnings']:
            if(warning == Warning.SALT_LEVEL_LOW):
                payload_lsb |= 128
            if(warning == Warning.RINSE_AGENT_LOW):
                payload_lsb |= 64

        for error in status['errors']:
            if(error == Error.PUMP_JAMMED):
                payload_lsb |= 8
            if(error == Error.WATER_SUPPLY_CLOSED):
                payload_lsb |= 4

        payload = bytearray([payload_msb, payload_lsb])
        # print(payload.hex())

        packet = self.__build_packet(payload)

        if(self.__response_callback is None):
            return
        
        if(self.no_response):
            return
        
        await self.__response_callback(packet)

    async def __on_start_program_request(self, program):

        self.machine.setProgram(Program(program))
        self.machine.start()

        packet = self.__build_packet(b'\x00')

        if(self.__response_callback is None):
            return

        if(self.no_response):
            return

        await self.__response_callback(packet)
