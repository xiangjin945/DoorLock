/*
 * @Author: i4season.xiang
 * @Date: 2021-08-02 15:05:42
 * @LastEditTime: 2021-09-06 18:16:24
 * @FilePath: /ProJect_DIR/7231u_ble_Config_Network/beken_freertos_sdk_release-SDK_BL2028N_3.0.33/demos/iot/ble/include/iot_ble_service.h
 */
#ifndef _BLE_SERVICE_H_
#define _BLE_SERVICE_H_
// #include "iot_flash.h"

#define ATTID_SCANRESULT_NTF    5
#define ATTID_SCAN_WRITE        7
#define ATTID_WIFIKEY_WRITE     9
#define ATTID_WIFIKEY_NTF       12
#define ATTID_WIFI_USERID       14
#define ATTID_WIFI_API          16

void ioit_ble_int(void);
void i4_init_ble_service(void);
void i4_start_ble_service(void);
void i4_stop_ble_service(void);





//int i4_ble_status_get(void);
//void i4_send_(void);
//void i4_stop_ble_service(void);
#endif