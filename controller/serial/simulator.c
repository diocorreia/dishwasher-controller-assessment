#include <stdio.h>

#include "serial.h"

int serial_init(serial_t *p_instance){
    return 0;
}

int serial_write(serial_t *p_instance, uint8_t *data, int data_len){
    return 0;
}

int serial_read_until(serial_t *p_instance, uint8_t delimiter, uint8_t *buffer, int buffer_len){
    return 0;
}

int serial_uninit(serial_t *p_instance){
    return 0;
}
