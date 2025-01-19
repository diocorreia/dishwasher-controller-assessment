#include <stdio.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <dishwasher.h>
#include <dishwasher/error.h>

#include "serial/serial.h"
#include "packet/packet.h"

#define PROTOCOL_VERSION_MAJOR 0x01
#define PROTOCOL_VERSION_MINOR 0x00

typedef enum dishwasher_request_type_e {
    DW_REQ_HELLO = 0x01,
    DW_REQ_GET_MACHINE_INFO,
    DW_REQ_GET_STATUS,
    DW_REQ_START_PROGRAM,
    DW_REQ_ABORT_PROGRAM
} dishwasher_request_type_t;

/* Forward declaration of internal functions */
static int send_request(uint8_t *request, int request_len);
static int receive_response(uint8_t *buffer, int buffer_len);
static uint8_t *parse_string(uint8_t *buffer, uint8_t *string);

/* Internal variables */
static bool m_initialized = false;
static serial_t m_serial;

int dishwasher_init(){
    if(m_initialized){
        return -DISHWASHER_ERROR_EINVAL;
    }

    if(serial_init(&m_serial) < 0){
        return -DISHWASHER_ERROR_INIT;
    };

    uint8_t request = DW_REQ_HELLO;
    uint8_t response[MAX_PAYLOAD_LEN] = {0};
    int response_len = 0;

    if(send_request(&request, 1) < 0){
        return -DISHWASHER_ERROR_REQUEST;
    };

    response_len = receive_response(response, MAX_PAYLOAD_LEN);

    if(response_len != 2){
        return -DISHWASHER_ERROR_VERSION;
    }

    if(response[0] != PROTOCOL_VERSION_MAJOR || response[1] != PROTOCOL_VERSION_MINOR){
        return -DISHWASHER_ERROR_VERSION;
    }

    m_initialized = true;

    return 0;
}

int dishwasher_uninit(){
    if(m_initialized){
        serial_uninit(&m_serial);
        m_initialized = false;
    }
    return 0;
}

int dishwasher_get_machine_info(dishwasher_info_t *p_info){
    if(!m_initialized){
        return -DISHWASHER_ERROR_INIT;
    }

    if(p_info == NULL){
        return -DISHWASHER_ERROR_EINVAL;
    }

    uint8_t request = DW_REQ_GET_MACHINE_INFO;
    uint8_t response[MAX_PAYLOAD_LEN] = {0};
    

    if(send_request(&request, 1) < 0){
        return -DISHWASHER_ERROR_REQUEST;
    };

    if(receive_response(response, MAX_PAYLOAD_LEN) < 0){
        return -DISHWASHER_INVALID_RESPONSE;
    };

    uint8_t *p_buffer = response;

    p_buffer = parse_string(p_buffer, (uint8_t*)p_info->machine_type);
    p_buffer = parse_string(p_buffer, (uint8_t*)p_info->hw_version);
    p_buffer = parse_string(p_buffer, (uint8_t*)p_info->fw_version);
    p_buffer = parse_string(p_buffer, (uint8_t*)p_info->serial);

    return 0;
}

int dishwasher_get_status(dishwasher_status_t *p_status){
    if(!m_initialized){
        return -DISHWASHER_ERROR_INIT;
    }

    if(p_status == NULL){
        return -DISHWASHER_ERROR_EINVAL;
    }

    uint8_t request = DW_REQ_GET_STATUS;
    uint8_t response[2] = {0};

    if(send_request(&request, 1) < 0){
        return -DISHWASHER_ERROR_REQUEST;
    };

    if(receive_response(response, 2) != 2){
        return -DISHWASHER_INVALID_RESPONSE;
    };
    
    p_status->program = (response[0] >> 5) & 0x07;
    p_status->step = (response[0] >> 2) & 0x07;
    p_status->state = (response[0] & 0x03);
    p_status->warnings.salt_level_low = (response[1] & 0x80);
    p_status->warnings.rinse_agent_level_low = (response[1] & 0x40);
    p_status->errors.pump_jammed = (response[1] & 0x08);
    p_status->errors.water_supply_closed = (response[1] & 0x04);

    return 0;
}

int dishwasher_start_program(dishwasher_program_type_t program){
    if(!m_initialized){
        return -DISHWASHER_ERROR_INIT;
    }

    if(program == DW_PROG_INVALID || program > DW_PROG_70DEG_HIGH){
        return -DISHWASHER_ERROR_EINVAL;
    }

    uint8_t request[2] = {DW_REQ_START_PROGRAM, 0};
    uint8_t response;

    request[1] = program;

    if(send_request(request, 2) < 0){
        return -DISHWASHER_ERROR_REQUEST;
    };

    if(receive_response(&response, 1) != 1){
        return -DISHWASHER_INVALID_RESPONSE;
    };

    return response;
}

int dishwasher_abort_program(){
    if(!m_initialized){
        return -DISHWASHER_ERROR_INIT;
    }

    uint8_t request = DW_REQ_ABORT_PROGRAM;
    uint8_t response;

    if(send_request(&request, 1) < 0){
        return -DISHWASHER_ERROR_REQUEST;
    };

    if(receive_response(&response, 1) != 1){
        return -DISHWASHER_INVALID_RESPONSE;
    };

    return response;
}

/* Private functions */
static int send_request(uint8_t *request, int request_len){
    static uint8_t packet[MAX_PACKET_LEN] = {0};
    static int packet_len;

    memset(packet, 0, MAX_PACKET_LEN);
    packet_len = packet_pack(request, request_len, packet, MAX_PACKET_LEN);

    if(packet_len < 0){
        return -1;
    }

    return serial_write(&m_serial, packet, packet_len);
}

static int receive_response(uint8_t *response, int response_len){
    static uint8_t buffer[MAX_PACKET_LEN];
    static int packet_len;

    packet_len = serial_read_until(&m_serial, 0x00, buffer, MAX_PACKET_LEN);

    if(packet_len < 0){
        return packet_len;
    }

    return packet_unpack(buffer, packet_len, response, response_len);
}

static uint8_t *parse_string(uint8_t *buffer, uint8_t *string){
    static int str_len = 0;

    if(buffer == NULL || string == NULL){
        return NULL;
    }

    str_len = strlen((char *)buffer);

    if(str_len == 0){
        return NULL;
    }

    memcpy(string, buffer, str_len + 1);

    return (uint8_t*)(buffer + str_len + 1);
}
