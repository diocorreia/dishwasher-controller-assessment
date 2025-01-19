#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <string.h>

#include "../packet/packet.h"

void test_packet_pack_on_sucess(){
    uint8_t payload[2] = {0x04, 0x03};
    uint8_t expected_packet[7] = {0x03, 0x13, 0x05, 0x03, 0x04, 0x03, 0x00};
    uint8_t packet[7] = {0};
    
    assert(packet_pack(payload, 2, packet, 7) == 7);
    assert(memcmp(packet, expected_packet, sizeof(expected_packet)) == 0);
}

void test_packet_unpack_on_sucess(){
    uint8_t payload[2] = {0x04, 0x03};
    uint8_t packet[8] = {0};
    uint8_t unpacked[8] = {0};

    int packet_len = packet_pack(payload, sizeof(payload), packet, sizeof(packet));

    assert(packet_unpack(packet, packet_len, unpacked, sizeof(unpacked)) == 2);

    assert(memcmp(payload, unpacked, sizeof(payload)) == 0);
}

int main(){
    test_packet_pack_on_sucess();
    test_packet_unpack_on_sucess();
    printf("ALL TESTS PASSED!\n");
}
