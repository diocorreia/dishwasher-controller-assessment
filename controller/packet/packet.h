#include <stdint.h>

#ifndef _DISHWASHER_CONTROLLER_PACKET_PACKET_H_
#define _DISHWASHER_CONTROLLER_PACKET_PACKET_H_

#define MAX_PAYLOAD_LEN 128
#define MAX_PACKET_LEN (MAX_PAYLOAD_LEN + 4)

/** @brief Builds packet accordingly to the Dishwasher communication protocol
 *  
 *  @param[in] payload Payload of the packet.
 *  @param[in] payload_len Length of the payload.
 *  @param[out] buffer Buffer in which the built packet will be stored.
 *  @param[out] buffer_len Size of the buffer. This information is used to prevent overflow.
 * 
 *  @retval If positive, corresponds to the size of the built packet.
 *  @retval -EINVAL \p payload or \p buffer is NULL, or \p payload_len or \p buffer_len is 0.  
 *  @retval -EMSGSIZE \p buffer is too small.
 */
int packet_pack(uint8_t *payload, int payload_len, uint8_t *buffer, int buffer_len);

/** @brief Unpacks accordingly to the Dishwasher communication protocol
 *  
 *  @param[in] packet Packet to unpack.
 *  @param[in] packet_len Length of the packet.
 *  @param[out] buffer Buffer in which the built unpacked data will be stored.
 *  @param[out] buffer_len Size of the buffer. This information is used to prevent overflow.
 * 
 *  @retval If positive, corresponds to the size of the unpacked data.
 *  @retval -EINVAL \p packet or \p buffer is NULL, or \p packet_len or \p buffer_len is 0.  
 *  @retval -EMSGSIZE \p buffer is too small.
 */
int packet_unpack(uint8_t *packet, int packet_len, uint8_t *buffer, int buffer_len);

#endif /* _DISHWASHER_CONTROLLER_PACKET_PACKET_H_ */
