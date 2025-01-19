OPTIONS=-Wall -Werror

INC+= -Iexternal/nanocobs

tests: test_serial test_packet

test_serial: serial_simulator.o
	$(CC) $(INC) tests/serial.c $< -o $@

test_packet: packet.o nanocobs.o
	$(CC) tests/packet.c $^ -o $@

serial_simulator.o:
	$(CC) $(INC) -c serial/simulator.c -o $@ $(OPTIONS)

packet.o: nanocobs.o
	$(CC) $(INC) -c packet/packet.c -o $@ $(OPTIONS)

nanocobs.o:
	$(CC) -c external/nanocobs/cobs.c -o $@ 

clean:
	rm -rf *.o test_serial