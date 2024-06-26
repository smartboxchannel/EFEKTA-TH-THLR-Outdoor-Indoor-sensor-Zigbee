#define TC_LINKKEY_JOIN
#define NV_INIT
#define NV_RESTORE


#define TP2_LEGACY_ZC
//patch sdk
// #define ZDSECMGR_TC_ATTEMPT_DEFAULT_KEY TRUE

#define NWK_AUTO_POLL
#define MULTICAST_ENABLED FALSE

#define ZCL_READ
#define ZCL_BASIC
#define ZCL_IDENTIFY
#define ZCL_REPORTING_DEVICE

#define ZSTACK_DEVICE_BUILD (DEVICE_BUILD_ENDDEVICE)

#define DISABLE_GREENPOWER_BASIC_PROXY
#define BDB_FINDING_BINDING_CAPABILITY_ENABLED 1
#define BDB_REPORTING TRUE


#define ISR_KEYINTERRUPT
#define HAL_BUZZER FALSE

#define HAL_LED TRUE
#define HAL_I2C TRUE
#define BLINK_LEDS TRUE


#if !defined HAL_BOARD_THP
#error "Board type must be defined"
#endif

#define BDB_MAX_CLUSTERENDPOINTS_REPORTING 10

#define BME280_32BIT_ENABLE

#define POWER_SAVING

// #define DO_DEBUG_UART


//i2c bme280
#define OCM_CLK_PORT 0
#define OCM_DATA_PORT 0
#define OCM_CLK_PIN 5
#define OCM_DATA_PIN 6


#ifdef DO_DEBUG_UART
#define HAL_UART TRUE
#define HAL_UART_DMA 1
#define INT_HEAP_LEN (2685 - 0x4B - 0xBB)
#endif


#define HAL_KEY_P2_INPUT_PINS BV(0)

//#define OUTDOOR_LONG_RANGE
//#define BMP

#include "hal_board_cfg.h"

#include "stdint.h"
#include "stddef.h"
