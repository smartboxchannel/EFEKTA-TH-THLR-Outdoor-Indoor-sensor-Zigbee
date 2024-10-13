#include "AF.h"
#include "OSAL.h"
#include "ZComDef.h"
#include "ZDConfig.h"
#include "zcl.h"
#include "zcl_general.h"
#include "zcl_ms.h"
#include "zcl_ha.h"
#include "zcl_app.h"
#include "battery.h"

#define APP_DEVICE_VERSION                                                      2
#define APP_FLAGS                                                               0

#define APP_HWVERSION                                                           1
#define APP_ZCLVERSION                                                          1

const uint16 zclApp_clusterRevision_all = 0x0001;

int16 zclApp_Temperature_Sensor_MeasuredValue = 0;
int16 zclApp_PressureSensor_MeasuredValue = 0;
int16 zclApp_PressureSensor_ScaledValue = 0;
int8 zclApp_PressureSensor_Scale = -1;
uint16 zclApp_HumiditySensor_MeasuredValue = 0;
int16 sendInitReportCount = 0;

const uint8 zclApp_HWRevision = APP_HWVERSION;
const uint8 zclApp_ZCLVersion = APP_ZCLVERSION;
const uint8 zclApp_ApplicationVersion = 3;
const uint8 zclApp_StackVersion = 4;

//{lenght, 'd', 'a', 't', 'a'}
const uint8 zclApp_ManufacturerName[] = {9, 'E', 'f', 'e', 'k', 't', 'a', 'L', 'a', 'b'};
#ifdef OUTDOOR_LONG_RANGE
const uint8 zclApp_ModelId[] = {13, 'E', 'F', 'E', 'K', 'T', 'A', '_', 'T', 'H', 'P', '_', 'L', 'R'};
#else
const uint8 zclApp_ModelId[] = {10, 'E', 'F', 'E', 'K', 'T', 'A', '_', 'T', 'H', 'P'};
#endif
const uint8 zclApp_DateCode[] = { 12, '2', '0', '2', '4', '1', '0', '1', '4', ' ', '6', '4', '3'};
const uint8 zclApp_SWBuildID[] = {5, '1', '.', '0', '.', '3'};

const uint8 zclApp_PowerSource = POWER_SOURCE_BATTERY;

CONST zclAttrRec_t zclApp_AttrsFirstEP[] = {
    {BASIC, {ATTRID_BASIC_ZCL_VERSION, ZCL_UINT8, R, (void *)&zclApp_ZCLVersion}},
    {BASIC, {ATTRID_BASIC_APPL_VERSION, ZCL_UINT8, R, (void *)&zclApp_ApplicationVersion}},
    {BASIC, {ATTRID_BASIC_STACK_VERSION, ZCL_UINT8, R, (void *)&zclApp_StackVersion}},
    {BASIC, {ATTRID_BASIC_HW_VERSION, ZCL_UINT8, R, (void *)&zclApp_HWRevision}},
    {BASIC, {ATTRID_BASIC_MANUFACTURER_NAME, ZCL_DATATYPE_CHAR_STR, R, (void *)zclApp_ManufacturerName}},
    {BASIC, {ATTRID_BASIC_MODEL_ID, ZCL_DATATYPE_CHAR_STR, R, (void *)zclApp_ModelId}},
    {BASIC, {ATTRID_BASIC_DATE_CODE, ZCL_DATATYPE_CHAR_STR, R, (void *)zclApp_DateCode}},
    {BASIC, {ATTRID_BASIC_POWER_SOURCE, ZCL_DATATYPE_ENUM8, R, (void *)&zclApp_PowerSource}},
    {BASIC, {ATTRID_BASIC_SW_BUILD_ID, ZCL_DATATYPE_CHAR_STR, R, (void *)zclApp_DateCode}},
    {BASIC, {ATTRID_CLUSTER_REVISION, ZCL_DATATYPE_UINT16, R, (void *)&zclApp_clusterRevision_all}},
    {POWER_CFG, {ATTRID_POWER_CFG_BATTERY_VOLTAGE, ZCL_UINT8, RR, (void *)&zclBattery_Voltage}},
    {POWER_CFG, {ATTRID_POWER_CFG_BATTERY_PERCENTAGE_REMAINING, ZCL_UINT8, RR, (void *)&zclBattery_PercentageRemainig}},
    {POWER_CFG, {ATTRID_POWER_CFG_BATTERY_VOLTAGE_RAW_ADC, ZCL_UINT16, RR, (void *)&zclBattery_RawAdc}},
    {TEMP, {ATTRID_MS_TEMPERATURE_MEASURED_VALUE, ZCL_INT16, RR, (void *)&zclApp_Temperature_Sensor_MeasuredValue}},
    {PRESSURE, {ATTRID_MS_PRESSURE_MEASUREMENT_MEASURED_VALUE, ZCL_INT16, RR, (void *)&zclApp_PressureSensor_MeasuredValue}},
    {PRESSURE, {ATTRID_MS_PRESSURE_MEASUREMENT_SCALED_VALUE, ZCL_INT16, RR, (void *)&zclApp_PressureSensor_ScaledValue}},
    {PRESSURE, {ATTRID_MS_PRESSURE_MEASUREMENT_SCALE, ZCL_INT8, R, (void *)&zclApp_PressureSensor_Scale}},
    {HUMIDITY, {ATTRID_MS_RELATIVE_HUMIDITY_MEASURED_VALUE, ZCL_UINT16, RR, (void *)&zclApp_HumiditySensor_MeasuredValue}}
};

uint8 CONST zclApp_AttrsFirstEPCount = (sizeof(zclApp_AttrsFirstEP) / sizeof(zclApp_AttrsFirstEP[0]));

const cId_t zclApp_InClusterListFirstEP[] = {ZCL_CLUSTER_ID_GEN_BASIC, POWER_CFG, TEMP, PRESSURE, HUMIDITY};
#define APP_MAX_INCLUSTERS (sizeof(zclApp_InClusterListFirstEP) / sizeof(zclApp_InClusterListFirstEP[0]))

const cId_t zclApp_OutClusterList[] = {POWER_CFG, TEMP, PRESSURE, HUMIDITY};
#define APP_MAX_OUT_CLUSTERS (sizeof(zclApp_OutClusterList) / sizeof(zclApp_OutClusterList[0]))

SimpleDescriptionFormat_t zclApp_FirstEP = {
    1,
    ZCL_HA_PROFILE_ID,
    ZCL_HA_DEVICEID_SIMPLE_SENSOR,
    APP_DEVICE_VERSION,
    APP_FLAGS,
    APP_MAX_INCLUSTERS,
    (cId_t *)zclApp_InClusterListFirstEP,
    APP_MAX_OUT_CLUSTERS,
    (cId_t *)zclApp_OutClusterList
};
