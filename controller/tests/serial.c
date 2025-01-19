#include <stdio.h>
#include <errno.h>
#include <assert.h>

#include "../serial/serial.h"

void test_serial_init_success(void){
    serial_t inst;
    assert(serial_init(&inst) == 0);
    serial_uninit(&inst);
}

void test_serial_uninit_success(void){
    serial_t inst;
    assert(serial_init(&inst) == 0);
    assert(serial_uninit(&inst) == 0);
    assert(serial_init(&inst) == 0);
    serial_uninit(&inst);
}

void test_serial_init_instance_is_null(void){
    serial_t *inst = NULL;
    assert(serial_init(inst) == -EINVAL);    
}

void test_serial_init_instance_too_many_connections(void){
    serial_t instances[2];
    assert(serial_init(&instances[0]) == 0);
    assert(serial_init(&instances[1]) == -EMLINK);
    serial_uninit(&instances[0]);
}

void test_serial_write_sucess(void){
    serial_t inst;
    uint8_t data[] = {0x06, 0xd3, 0x05, 0x05, 0x04, 0x03, 0x00};

    serial_init(&inst);

    assert(serial_write(&inst, data, sizeof(data)) == 0);

    serial_uninit(&inst);
}

void test_serial_write_with_serial_not_init(void){
    serial_t inst;
    uint8_t data[] = {0x06, 0xd3, 0x05, 0x05, 0x04, 0x03, 0x00};

    assert(serial_write(&inst, data, sizeof(data)) == -EINVAL);
}

void test_serial_read_sucess(void){
    serial_t inst;
    uint8_t request[] = {0x06, 0xd3, 0x05, 0x05, 0x04, 0x03, 0x00};
    uint8_t response[128] = {0};

    serial_init(&inst);

    serial_write(&inst, request, sizeof(request));
    assert(serial_read_until(&inst, 0x00, response, 128) > 0);

    serial_uninit(&inst);
}

void test_serial_read_buffer_to_small(void){
    serial_t inst;
    uint8_t request[] = {0x06, 0xd3, 0x05, 0x05, 0x04, 0x03, 0x00};
    uint8_t response[1] = {0};

    serial_init(&inst);

    serial_write(&inst, request, sizeof(request));
    assert(serial_read_until(&inst, 0x00, response, 1) == -ENOMEM);

    serial_uninit(&inst);
}

int main(){
    test_serial_init_success();
    test_serial_uninit_success();
    test_serial_init_instance_too_many_connections();
    test_serial_init_instance_is_null();
    test_serial_write_sucess();
    test_serial_write_with_serial_not_init();
    test_serial_read_sucess();
    test_serial_read_buffer_to_small();
    printf("ALL TESTS PASSED!\n");
}
