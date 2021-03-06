#ifndef ONEWHEEL_BLUETOOTH_H
#define ONEWHEEL_BLUETOOTH_H

#define GATTS_TABLE_TAG "GATTS_TABLE_DEMO"

#define ESP_APP_ID                  0x55
#define SAMPLE_DEVICE_NAME          "One Wheel"
#define SVC_INST_ID                 0

/* The max length of characteristic value. When the GATT client performs a write or prepare write operation,
*  the data length must be less than GATTS_DEMO_CHAR_VAL_LEN_MAX. 
*/
#define GATTS_DEMO_CHAR_VAL_LEN_MAX 12
#define PREPARE_BUF_MAX_SIZE        1024
#define CHAR_DECLARATION_SIZE       (sizeof(uint8_t))

#define ADV_CONFIG_FLAG             (1 << 0)
#define SCAN_RSP_CONFIG_FLAG        (1 << 1)

#include <inttypes.h>

enum {
    IDX_MAIN_SVC,
    IDX_CHAR_ANGLES,
    IDX_CHAR_VAL_ANGLES,

    IDX_CHAR_PID,
    IDX_CHAR_VAL_PID,

    IDX_CHAR_INFO,
    IDX_CHAR_VAL_INFO,

    HRS_IDX_NB,
};

void init_ble();
void send_euler(long* euler);
void send_info(uint8_t motor, uint8_t reverse, uint8_t battery, uint8_t speed);
void on_new_pid_gains(uint16_t prop, uint16_t integral, uint16_t deriv);
void get_pid_gains(uint16_t* prop, uint16_t* integral, uint16_t* deriv);

#endif