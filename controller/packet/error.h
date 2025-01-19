#ifndef _DISHWASHER_CONTROLLER_PACKET_ERROR_H_
#define _DISHWASHER_CONTROLLER_PACKET_ERROR_H_

#define ERROR_PARAM_INVAL 1 /*!< Invalid parameter */
#define ERROR_BUFFER_OVERFLOW 2 /*!< Buffer is too small */
#define ERROR_INVALID_SEQUENCE 3 /*!< Invalid sequence number */
#define ERROR_INVALID_CRC 4 /*!< Error detected on tha packet */
#define ERROR_FAILED_TO_ENCODE 5 /*!< Error while encoding packet to COBS */
#define ERROR_FAILED_TO_DECODE 6 /*!< Error while decoding COBS packet */

#endif /* _DISHWASHER_CONTROLLER_PACKET_ERROR_H_ */
