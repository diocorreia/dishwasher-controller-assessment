#include <stdbool.h>

#ifndef _DISHWASHER_H_
#define _DISHWASHER_H_

#define MAX_STRING_LEN 128

/*! Dishwasher Program */
typedef enum dishwasher_program_type_e {
    DW_PROG_INVALID,        /*!< Invalid program. */
    DW_PROG_50DEG_ECO,      /*!< ECO program at 50 ºC */
    DW_PROG_60DEG_NORMAL,   /*!< Normal program at 60 ºC */
    DW_PROG_60DEG_HIGH,     /*!< High program at 60 ºC */
    DW_PROG_70DEG_HIGH      /*!< High program at 70 ºC */
} dishwasher_program_type_t;

/*! Dishwasher Step */
typedef enum dishwasher_step_type_e {
    DW_STEP_INVALID,    /*!< Invalid step. */
    DW_STEP_PRESOAK,    /*!< Machine is on pre-soaking. */
    DW_STEP_WARMUP,     /*!< Machine is warming up. */
    DW_STEP_MAIN_WASH,  /*!< Machine is washing the clothes. */
    DW_STEP_CLEAN_WASH, /*!< Machine is cleaning. */
    DW_STEP_DRY         /*!< Machine is drying. */
} dishwasher_step_type_t;

/*! Dishwasher State */
typedef enum dishwasher_state_type_e {
    DW_STATE_IDLE,          /*!< Machine is on IDLE. */
    DW_STATE_PROG_RUNNING,  /*!< Machine is running a program. */
    DW_STATE_ERROR          /*!< Error was detected. */
} dishwasher_state_type_t;

/*! Dishwasher Status */
typedef struct dishwasher_status_s {
    dishwasher_program_type_t program; /*!< Program currently set. */
    dishwasher_step_type_t step;   /*!< Machine program step. */
    dishwasher_state_type_t state; /*!< Machine state. */
    struct {
        bool salt_level_low;            /*!< Salt level is low. */
        bool rinse_agent_level_low;     /*!< Rinse agent level is low. */
    } warnings;
    struct {
        bool pump_jammed;               /*!< Pump is jammed. */
        bool water_supply_closed;       /*!< Water supply is closed. */
    } errors;
} dishwasher_status_t;

/*! Dishwasher Info */
typedef struct dishwasher_info_s {
    char machine_type[MAX_STRING_LEN];  /*!< Machine type. */
    char hw_version[MAX_STRING_LEN];    /*!< Hardware version. */
    char fw_version[MAX_STRING_LEN];    /*!< Firmware version. */
    char serial[MAX_STRING_LEN];        /*!< Serial number. */
} dishwasher_info_t;

/** @brief Initializes dishwasher module
 * 
 *  @retval 0 dishwasher module successfully intialized.
 *  @retval -DISHWASHER_ERROR_REQUEST failed to get version of communication protocol.
 *  @retval -DISHWASHER_ERROR_VERSION machine is using different version of communication protocol.
*/
int dishwasher_init();

/** @brief Uninitializes dishwasher module
 *  
 *  @retval 0 serial connection successfully uninitialized.
*/
int dishwasher_uninit();

/** @brief Get information
 * 
 *  @details Information includes: type, hardware version, firmware version and serial number
 *  
 *  @param[out] p_info Pointer to \ref dishwasher_info_t structure to store response.
 * 
 *  @retval 0 operation was a success.
 *  @retval -DISHWASHER_ERROR_EINVAL \p p_info is NULL or module is uninitialized.
 *  @retval -DISHWASHER_ERROR_REQUEST failed to send request to the machine.
 *  @retval -DISHWASHER_INVALID_RESPONSE failed to get response from the machine.
*/
int dishwasher_get_machine_info(dishwasher_info_t *p_info);

/** @brief Get status
 * 
 *  @details Status includes: set program, current step, current state, warnings and errors.
 *  
 *  @param[out] p_status Pointer to \ref dishwasher_status_t structure to store response.
 * 
 *  @retval 0 operation was a success.
 *  @retval -DISHWASHER_ERROR_EINVAL \p p_status is NULL or module is uninitialized.
 *  @retval -DISHWASHER_ERROR_REQUEST failed to send request to the machine.
 *  @retval -DISHWASHER_INVALID_RESPONSE failed to get response from the machine.
*/
int dishwasher_get_status(dishwasher_status_t *p_status);

/** @brief Start program
 * 
 *  @details Status includes: set program, current step, current state, warnings and errors.
 *  
 *  @param[out] program Washing program.
 * 
 *  @retval 0 program started successfully.
 *  @retval -DISHWASHER_ERROR_EINVAL \p program is not valid or module is uninitialized.
 *  @retval -DISHWASHER_ERROR_REQUEST failed to send request to the machine.
 *  @retval -DISHWASHER_INVALID_RESPONSE failed to get response from the machine.
*/
int dishwasher_start_program(dishwasher_program_type_t program);

/** @brief Start program
 * 
 *  @details Status includes: set program, current step, current state, warnings and errors.
 *  
 *  @param[out] program Washing program.
 * 
 *  @retval 0 program aborted successfully.
 *  @retval -DISHWASHER_ERROR_EINVAL module is uninitialized.
 *  @retval -DISHWASHER_ERROR_REQUEST failed to send request to the machine.
 *  @retval -DISHWASHER_INVALID_RESPONSE failed to get response from the machine.
*/
int dishwasher_abort_program();

#endif /* _DISHWASHER_H_ */
