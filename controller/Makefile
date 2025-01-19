OPTIONS=-Wall -Werror

INC+= -Iinclude -Iexternal/nanocobs

sample: packet.o nanocobs.o serial_simulator.o dishwasher.o
	$(CC) $(INC) sample.c $^ -o $@ $(OPTIONS)

tests: test_serial test_packet test_dishwasher

test_serial: serial_simulator.o
	$(CC) $(INC) tests/serial.c $< -o $@

test_packet: packet.o nanocobs.o
	$(CC) tests/packet.c $^ -o $@

test_dishwasher: packet.o nanocobs.o serial_simulator.o dishwasher.o
	$(CC) $(INC) tests/dishwasher.c $^ -o $@ $(OPTIONS)

serial_simulator.o:
	$(CC) $(INC) -c serial/simulator.c -o $@ $(OPTIONS)

packet.o: nanocobs.o
	$(CC) $(INC) -c packet/packet.c -o $@ $(OPTIONS)

nanocobs.o:
	$(CC) -c external/nanocobs/cobs.c -o $@ 

dishwasher.o: packet.o serial_simulator.o
	$(CC) $(INC) -c dishwasher.c -o $@ $(OPTIONS)

clean:
	rm -rf *.o sample test_serial test_dishwasher test_packet