#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h> 

#include <sys/time.h>

#include "serial.h"

#define MAX_CONNECTIONS 1

/* Declaraction of private types */
typedef struct serial_internal_s {
    bool initialized;
    int socket;
} serial_internal_t;

/* Private variables */
static const char socket_path[] = "/tmp/dishwasher.socket";
static serial_internal_t m_internal[MAX_CONNECTIONS];
static uint8_t m_number_of_connections = 0;

/* Public functions */

int serial_init(serial_t *p_instance){
    if(p_instance == NULL){
        return -EINVAL;
    }

    if(m_number_of_connections >= MAX_CONNECTIONS){
        return -EMLINK;
    }

    serial_internal_t *p_internal = &m_internal[m_number_of_connections];

    if(p_internal->initialized){
        return -EINVAL;
    }

    /* Create socket and connect to simulator */
    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;
    memcpy(addr.sun_path, socket_path, sizeof(socket_path));
    p_internal->socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if(p_internal->socket < 0){
        return -EIO;
    }

    /* Set timeout for 100ms */
    struct timeval timeout={0,100000};
    setsockopt(p_internal->socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout,sizeof(struct timeval));

    if(connect(p_internal->socket, (struct sockaddr *)&addr, SUN_LEN(&addr)) < 0){
        return -EIO;
    };

    /* Update instance state and pointer to internal data structure */
    p_internal->initialized = true;
    p_instance->p_internal = (void *)p_internal;
    m_number_of_connections++;

    return 0;
}

int serial_write(serial_t *p_instance, uint8_t *data, int data_len){

    if(p_instance == NULL || data == NULL || data_len == 0){
        return -EINVAL;
    }

    serial_internal_t *p_internal = (serial_internal_t *)p_instance->p_internal;

    if(!p_internal->initialized){
        return -EINVAL;
    }

    if (send(p_internal->socket, data, data_len, 0) < 0){
        return -EIO;
    };

    return 0;
}

int serial_read_until(serial_t *p_instance, uint8_t delimiter, uint8_t *buffer, int buffer_len){

    if(p_instance == NULL || buffer == NULL || buffer_len == 0){
        return -EINVAL;
    }

    serial_internal_t *p_internal = (serial_internal_t *)p_instance->p_internal;

    if(!p_internal->initialized){
        return -EINVAL;
    }

    uint8_t byte;
    int i = 0;

    do {
        if(recv(p_internal->socket, &byte, sizeof(byte), 0) < 0){
            return -EIO;
        };
        buffer[i++] = byte;
        if(i > buffer_len){
            return -ENOMEM;
        }
    } while(byte != delimiter);

    return i;
}

int serial_uninit(serial_t *p_instance){
    if(p_instance == NULL){
        return -EINVAL;
    }

    serial_internal_t *p_internal = (serial_internal_t *)p_instance->p_internal;

    if(!p_internal->initialized){
        return -EINVAL;
    }

    close(p_internal->socket);

    p_internal->initialized = false;
    m_number_of_connections--;

    return 0;
}
