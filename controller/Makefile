OPTIONS=-Wall -Werror

tests: test_serial

test_serial: serial_simulator.o
	$(CC) $(INC) tests/serial.c $< -o $@

serial_simulator.o:
	$(CC) $(INC) -c serial/simulator.c -o $@ $(OPTIONS)

clean:
	rm -rf *.o test_serial