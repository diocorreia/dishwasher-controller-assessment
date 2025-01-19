#include <stdint.h>

#ifndef _DISHWASHER_CONTROLLER_SERIAL_SERIAL_H_
#define _DISHWASHER_CONTROLLER_SERIAL_SERIAL_H_

typedef struct serial_s {
    void *p_internal;
} serial_t;

/** @brief Initializes serial connection
 * 
 *  @param[out] p_instance Pointer to serial connection instance.
 * 
 *  @retval 0 serial connection successfully initialized.
 *  @retval -EINVAL \p p_instance is NULL or already initialized.
 *  @retval -EMLINK no connections currently available. 
*/
int serial_init(serial_t *p_instance);

/** @brief Writes data to serial connection 
 * 
 *  @param[in] p_instance Pointer to serial connection instance.
 *  @param[in] data Buffer with data to be written.
 *  @param[in] data_len Length (in bytes) of data to be written.
 * 
 *  @retval 0 data was successfully sent.
 *  @retval -EINVAL \p p_instance or \p p_data is NULL, \p data_len is 0, or \p instance is not initialized. 
 *  @retval otherwise operation was not successful.
*/
int serial_write(serial_t *p_instance, uint8_t *data, int data_len);

/** @brief Writes data to serial connection 
 * 
 *  @param[in] p_instance Pointer to serial connection instance.
 *  @param[in] delimiter Byte to search for.
 *  @param[out] buffer Buffer in which read data will be stored.
 *  @param[in] buffer_len Length (in bytes) of data buffer.
 * 
 *  @retval Number of read bytes.
 *  @retval -EINVAL \p p_instance or \p p_buffer is NULL, \p buffer_len is 0, or \p instance is not initialized. 
 *  @retval otherwise operation was not successful.
*/
int serial_read_until(serial_t *p_instance, uint8_t delimiter, uint8_t *buffer, int buffer_len);

/** @brief Uninitializes serial connection
 * 
 *  @param[in] p_instance Pointer to serial connection instance.
 * 
 *  @retval 0 serial connection successfully uninitialized.
 *  @retval -EINVAL \p p_instance is NULL or not initialized.
 *  @retval otherwise operation was not successful.
*/
int serial_uninit(serial_t *p_instance);

#endif /* _DISHWASHER_CONTROLLER_SERIAL_SERIAL_H_ */
