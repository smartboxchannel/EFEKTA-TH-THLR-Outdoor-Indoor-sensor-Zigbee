
#include "AF.h"
#include "OSAL.h"
#include "OSAL_Clock.h"
#include "OSAL_PwrMgr.h"
#include "ZComDef.h"
#include "ZDApp.h"
#include "ZDNwkMgr.h"
#include "ZDObject.h"
#include "math.h"

#include "nwk_util.h"
#include "zcl.h"
#include "zcl_app.h"
#include "zcl_diagnostic.h"
#include "zcl_general.h"
#include "zcl_lighting.h"
#include "zcl_ms.h"

#include "bdb.h"
#include "bdb_interface.h"
#include "gp_interface.h"

#include <stdio.h>
#include <stdlib.h>

#include "OnBoard.h"
#include "bme280.h"
#include "hal_adc.h"
#include "hal_drivers.h"
#include "hal_i2c.h"
#include "hal_key.h"
#include "hal_led.h"

#include "battery.h"
#include "commissioning.h"
#include "factory_reset.h"
#include "utils.h"


/*********************************************************************
 * MACROS
 */
#define HAL_KEY_CODE_RELEASE_KEY HAL_KEY_CODE_NOKEY

// use led4 as output pin, osal will shitch it low when go to PM
#define POWER_ON_SENSORS()                                                                                                                 \
    do {                                                                                                                                   \
        HAL_TURN_ON_LED4();                                                                                                                \
        IO_PUD_PORT(OCM_CLK_PORT, IO_PUP);                                                                                                 \
        IO_PUD_PORT(OCM_DATA_PORT, IO_PUP);                                                                                                \
    } while (0)
#define POWER_OFF_SENSORS()                                                                                                                \
    do {                                                                                                                                   \
        HAL_TURN_OFF_LED4();                                                                                                               \
        IO_PUD_PORT(OCM_CLK_PORT, IO_PDN);                                                                                                 \
        IO_PUD_PORT(OCM_DATA_PORT, IO_PDN);                                                                                                \
    } while (0)


extern bool requestNewTrustCenterLinkKey;
byte zclApp_TaskID;
static uint8 currentSensorsReadingPhase = 0;
static bool pushBut = false;
static bool initOut = false;
uint8 SeqNum = 0;
afAddrType_t inderect_DstAddr = {.addrMode = (afAddrMode_t)AddrNotPresent, .endPoint = 0, .addr.shortAddr = 0};
struct bme280_data bme_results;
struct bme280_dev bme_dev = {.dev_id = BME280_I2C_ADDR_PRIM,
                             .intf = BME280_I2C_INTF,
                             .read = I2C_ReadMultByte,
                             .write = I2C_WriteMultByte,
                             .delay_ms = user_delay_ms};

static void zclApp_HandleKeys(byte shift, byte keys);
static void zclApp_Report(void);
static void zclApp_ReadSensors(void);
static void zclSendPres(void);
static void zclSendTemp(void);
static void zclSendHum(void);
static void zclApp_LedOn(void);
static void zclApp_LedOn2(void);
static void zclApp_LedOff(void);
static void zclApp_InitBME280(struct bme280_dev *dev);
static void zclApp_ReadBME280(struct bme280_dev *dev);

static zclGeneral_AppCallbacks_t zclApp_CmdCallbacks = {
    NULL, // Basic Cluster Reset command
    NULL, // Identify Trigger Effect command
    NULL, // On/Off cluster commands
    NULL, // On/Off cluster enhanced command Off with Effect
    NULL, // On/Off cluster enhanced command On with Recall Global Scene
    NULL, // On/Off cluster enhanced command On with Timed Off
    NULL, // RSSI Location command
    NULL  // RSSI Location Response command
};

void zclApp_Init(byte task_id) {

    HAL_TURN_ON_LED1();
    user_delay_ms(300);
    HAL_TURN_OFF_LED1();
    POWER_OFF_SENSORS();
    HalI2CInit();
  
    requestNewTrustCenterLinkKey = FALSE;
    zclApp_TaskID = task_id;
    
    bdb_RegisterSimpleDescriptor(&zclApp_FirstEP);
    zclGeneral_RegisterCmdCallbacks(zclApp_FirstEP.EndPoint, &zclApp_CmdCallbacks);
    zcl_registerAttrList(zclApp_FirstEP.EndPoint, zclApp_AttrsFirstEPCount, zclApp_AttrsFirstEP);
    
    zcl_registerForMsg(zclApp_TaskID);
    RegisterForKeys(zclApp_TaskID);

    osal_start_reload_timer(zclApp_TaskID, APP_REPORT_EVT, 7000);
    osal_start_reload_timer(zclApp_TaskID, APP_REPORT_BATT_EVT, APP_REPORT_BATT_DELAY);
}

uint16 zclApp_event_loop(uint8 task_id, uint16 events) {
    afIncomingMSGPacket_t *MSGpkt;

    (void)task_id;
    if (events & SYS_EVENT_MSG) {
        while ((MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive(zclApp_TaskID))) {
            switch (MSGpkt->hdr.event) {
            case KEY_CHANGE:
                zclApp_HandleKeys(((keyChange_t *)MSGpkt)->state, ((keyChange_t *)MSGpkt)->keys);
                break;
            case ZCL_INCOMING_MSG:
                if (((zclIncomingMsg_t *)MSGpkt)->attrCmd) {
                    osal_mem_free(((zclIncomingMsg_t *)MSGpkt)->attrCmd);
                }
                break;

            default:
                break;
            }
            osal_msg_deallocate((uint8 *)MSGpkt);
        }
        return (events ^ SYS_EVENT_MSG);
    }

    if (events & APP_REPORT_EVT) {
        if (initOut == false){
          sendInitReportCount++;
          if(sendInitReportCount  >=  5){
            osal_stop_timerEx(zclApp_TaskID, APP_REPORT_EVT);
            osal_clear_event(zclApp_TaskID, APP_REPORT_EVT);
            osal_start_reload_timer(zclApp_TaskID, APP_REPORT_EVT, APP_REPORT_DELAY);
            initOut = true;
          }
          pushBut = true;
          zclApp_Report();
        }else{
         zclApp_Report();  
        }
        return (events ^ APP_REPORT_EVT);
    }

    if (events & APP_READ_SENSORS_EVT) {
        zclApp_ReadSensors();
        return (events ^ APP_READ_SENSORS_EVT);
    }
    if (events & APP_REPORT_BATT_EVT) {
        pushBut = true;
        return (events ^ APP_REPORT_BATT_EVT);
    }
    if (events & APP_LED_EVT) {
           zclApp_LedOff();
        return (events ^ APP_LED_EVT);
    }
    if (events & APP_LED2_EVT) {
           zclApp_LedOff();
           osal_start_timerEx(zclApp_TaskID, APP_LEDON_EVT, 150);
           
        return (events ^ APP_LED2_EVT);
    }
     if (events & APP_LEDON_EVT) {
          zclApp_LedOn(); 
           
        return (events ^ APP_LEDON_EVT);
    }
    return 0;
}


static void zclApp_LedOn(void){
   HAL_TURN_ON_LED1();
  osal_start_timerEx(zclApp_TaskID, APP_LED_EVT, 50);
}

static void zclApp_LedOn2(void){
   HAL_TURN_ON_LED1();
  osal_start_timerEx(zclApp_TaskID, APP_LED2_EVT, 50);
}

static void zclApp_LedOff(void){
   HAL_TURN_OFF_LED1();
}

static void zclApp_HandleKeys(byte portAndAction, byte keyCode) {
    
#if APP_COMMISSIONING_BY_LONG_PRESS == TRUE
    if (bdbAttributes.bdbNodeIsOnANetwork == 1) {
      zclFactoryResetter_HandleKeys(portAndAction, keyCode);
    }
#else
    zclFactoryResetter_HandleKeys(portAndAction, keyCode);
#endif
  
    zclCommissioning_HandleKeys(portAndAction, keyCode);
    
    if (portAndAction & HAL_KEY_PRESS) {
      if (bdbAttributes.bdbNodeIsOnANetwork == 1){
       
      }else{
      
      }
    }
    if (portAndAction & HAL_KEY_RELEASE) {
  
        pushBut = true;
        zclApp_Report();     

        HAL_TURN_OFF_LED1();
    } 
}

static void zclApp_ReadSensors(void) {
    switch (currentSensorsReadingPhase++) {
 
    case 0:
        if(pushBut){
        zclBattery_Report();
      }
      break;
            
    case 1:
        POWER_ON_SENSORS();
        zclApp_InitBME280(&bme_dev);
        POWER_OFF_SENSORS();
        break;
        
    case 2:
        zclSendTemp();
        break;
        
    case 3:
        zclSendHum();
        break;
        
   case 4:
        zclSendPres();
        break;     
        
    default:
        currentSensorsReadingPhase = 0;
        if(pushBut == true){
        pushBut = false;
        }
        break;
    }
    if (currentSensorsReadingPhase != 0) {
        osal_start_timerEx(zclApp_TaskID, APP_READ_SENSORS_EVT, 20);
    }
}



static void _delay_us(uint16 microSecs) {
    while (microSecs--) {
    asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); 
    asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); 
    asm("nop");
  }
}

void user_delay_ms(uint32_t period) { _delay_us(1000 * period); }

static void zclApp_InitBME280(struct bme280_dev *dev) {
    int8_t rslt = bme280_init(dev);
    if (rslt == BME280_OK) {
        uint8_t settings_sel;
        dev->settings.osr_h = BME280_OVERSAMPLING_1X;
        dev->settings.osr_p = BME280_OVERSAMPLING_16X;
        dev->settings.osr_t = BME280_OVERSAMPLING_2X;
        dev->settings.filter = BME280_FILTER_COEFF_16;
        dev->settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

        settings_sel = BME280_OSR_PRESS_SEL;
        settings_sel |= BME280_OSR_TEMP_SEL;
        settings_sel |= BME280_OSR_HUM_SEL;
        settings_sel |= BME280_STANDBY_SEL;
        settings_sel |= BME280_FILTER_SEL;
        rslt = bme280_set_sensor_settings(settings_sel, dev);
        rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE, dev);

        uint32_t req_delay = bme280_cal_meas_delay(&dev->settings);
        dev->delay_ms(req_delay);

        zclApp_ReadBME280(dev);
    } else {
        HAL_TURN_ON_LED1();
    }
}
static void zclApp_ReadBME280(struct bme280_dev *dev) {
    int8_t rslt = bme280_get_sensor_data(BME280_ALL, &bme_results, dev);
    if (rslt == BME280_OK) {
        zclApp_Temperature_Sensor_MeasuredValue = (int16)bme_results.temperature;
        zclApp_PressureSensor_ScaledValue = (int16)(pow(10.0, (double)zclApp_PressureSensor_Scale) * (double)bme_results.pressure);
        zclApp_PressureSensor_MeasuredValue = bme_results.pressure / 100;
        zclApp_HumiditySensor_MeasuredValue = (uint16)(bme_results.humidity * 100 / 1024);
    } 
}

static void zclApp_Report(void) { 
  if(devState == DEV_END_DEVICE && pushBut == true){
  zclApp_LedOn();
  }else if(devState == DEV_NWK_ORPHAN && pushBut == true || devState == DEV_NWK_BACKOFF && pushBut == true || devState == DEV_NWK_KA && pushBut == true){
   zclApp_LedOn2(); 
  }
  if(sendInitReportCount == 0){
     initOut = false;
     osal_stop_timerEx(zclApp_TaskID, APP_REPORT_EVT);
     osal_clear_event(zclApp_TaskID, APP_REPORT_EVT);
     osal_start_reload_timer(zclApp_TaskID, APP_REPORT_EVT, 7000);
  }
  if (initOut == false){
    NLME_SetPollRate(400);
  }else{
    NLME_SetPollRate(0);
  }
  
  osal_start_timerEx(zclApp_TaskID, APP_READ_SENSORS_EVT, 20); 
}

static void zclSendPres(void) { 
  if(pushBut == true){
  zclReportCmd_t *pReportCmd;
  const uint8 NUM_ATTRIBUTES = 2;
  
  pReportCmd = osal_mem_alloc(sizeof(zclReportCmd_t) + (NUM_ATTRIBUTES * sizeof(zclReport_t)));
  if (pReportCmd != NULL) {
    pReportCmd->numAttr = NUM_ATTRIBUTES;
    pReportCmd->attrList[0].attrID = ATTRID_MS_PRESSURE_MEASUREMENT_MEASURED_VALUE;
    pReportCmd->attrList[0].dataType = ZCL_INT16;
    pReportCmd->attrList[0].attrData = (void *)(&zclApp_PressureSensor_MeasuredValue);
    
    pReportCmd->attrList[1].attrID = ATTRID_MS_PRESSURE_MEASUREMENT_SCALED_VALUE;
    pReportCmd->attrList[1].dataType = ZCL_INT16;
    pReportCmd->attrList[1].attrData = (void *)(&zclApp_PressureSensor_ScaledValue);

     zcl_SendReportCmd(zclApp_FirstEP.EndPoint, &inderect_DstAddr, PRESSURE, pReportCmd, ZCL_FRAME_SERVER_CLIENT_DIR, TRUE, SeqNum++);
  }
    osal_mem_free(pReportCmd);
  }else{
    bdb_RepChangedAttrValue(zclApp_FirstEP.EndPoint, PRESSURE, ATTRID_MS_PRESSURE_MEASUREMENT_MEASURED_VALUE);
  }
}

static void zclSendTemp(void) {
  if(pushBut == true){
  zclReportCmd_t *pReportCmd;
  const uint8 NUM_ATTRIBUTES = 1;
  
  pReportCmd = osal_mem_alloc(sizeof(zclReportCmd_t) + (NUM_ATTRIBUTES * sizeof(zclReport_t)));
  if (pReportCmd != NULL) {
    pReportCmd->numAttr = NUM_ATTRIBUTES;
    
    pReportCmd->attrList[0].attrID = ATTRID_MS_TEMPERATURE_MEASURED_VALUE;
    pReportCmd->attrList[0].dataType = ZCL_DATATYPE_INT16;
    pReportCmd->attrList[0].attrData = (void *)(&zclApp_Temperature_Sensor_MeasuredValue);
    
    zcl_SendReportCmd(zclApp_FirstEP.EndPoint, &inderect_DstAddr, TEMP, pReportCmd, ZCL_FRAME_SERVER_CLIENT_DIR, true, SeqNum++);
  }
    osal_mem_free(pReportCmd);
  }else{
    bdb_RepChangedAttrValue(zclApp_FirstEP.EndPoint, TEMP, ATTRID_MS_TEMPERATURE_MEASURED_VALUE);
  }
}

static void zclSendHum(void) {
 if(pushBut == true){
  zclReportCmd_t *pReportCmd;
  const uint8 NUM_ATTRIBUTES = 1;
  
  pReportCmd = osal_mem_alloc(sizeof(zclReportCmd_t) + (NUM_ATTRIBUTES * sizeof(zclReport_t)));
  if (pReportCmd != NULL) {
    pReportCmd->numAttr = NUM_ATTRIBUTES;
    
    pReportCmd->attrList[0].attrID = ATTRID_MS_RELATIVE_HUMIDITY_MEASURED_VALUE;
    pReportCmd->attrList[0].dataType = ZCL_DATATYPE_UINT16;
    pReportCmd->attrList[0].attrData = (void *)(&zclApp_HumiditySensor_MeasuredValue);

    zcl_SendReportCmd(zclApp_FirstEP.EndPoint, &inderect_DstAddr, HUMIDITY, pReportCmd, ZCL_FRAME_SERVER_CLIENT_DIR, true, SeqNum++);
  }
    osal_mem_free(pReportCmd);
 }else{
    bdb_RepChangedAttrValue(zclApp_FirstEP.EndPoint, HUMIDITY, ATTRID_MS_RELATIVE_HUMIDITY_MEASURED_VALUE); 
  }
}