import asyncio

SOCKET_PATH = "/tmp/dishwasher.socket"

class FakeSerial:

    def __init__(self):
        self.socket_path = SOCKET_PATH

    def set_callback(self, callback):
        self.callback = callback

    async def handle_request(self, reader, writer):
        self.__writer = writer
        while True:
            try:
                data = await reader.readuntil(separator=b'\x00')
            except:
                break
            else:
                if(len(data) == 1 or len(data) == 0):
                    continue
                await self.callback(data)

    async def send(self, packet):
        try:
            self.__writer.write(packet)
            await self.__writer.drain()    
        except:
            return

    async def run(self):
        server = await asyncio.start_unix_server(self.handle_request, path=self.socket_path)
