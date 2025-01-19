OPTIONS=-Wall -Werror

tests: test_serial test_packet

test_serial: serial_simulator.o
	$(CC) $(INC) tests/serial.c $< -o $@

test_packet: packet.o
	$(CC) tests/packet.c $^ -o $@

serial_simulator.o:
	$(CC) $(INC) -c serial/simulator.c -o $@ $(OPTIONS)

packet.o:
	$(CC) $(INC) -c packet/packet.c -o $@ $(OPTIONS)

clean:
	rm -rf *.o test_serial