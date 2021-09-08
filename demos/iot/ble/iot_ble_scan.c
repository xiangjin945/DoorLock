/*
 * @Author: i4season.xiang
 * @Date: 2021-08-02 14:13:47
 * @LastEditTime: 2021-08-06 13:54:47
 * @FilePath: \beken_freertos_sdk_release-SDK_BL2028N_3.0.33_belon\beken_freertos_sdk_release-SDK_BL2028N_3.0.33\demos\ble\scan\ble_scan.c
 */
#include "iot_ble_scan.h"
#include "generic.h"
#include "mem_pub.h"
#include "wlan_ui_pub.h"
//#include "stdint-uintn.h"

#include "ble_api.h"
#include "iot_ble_service.h"
#include "ble_api_5_x.h"
#include "iot_wifi_fun.h"
static uint16_t id_index[2]={0};

extern ble_err_t bk_ble_send_ntf_value(uint32_t len, uint8_t *buf, uint16_t prf_id, uint16_t att_idx);


int ble_scanresultid_save(uint16_t pre_id, uint16_t att_id)
{
    id_index[0] = pre_id;
    id_index[1] = att_id;
    return 0;
}

int get_preid_attid(uint16_t *pre_id, uint16_t *att_id)
{
    *pre_id = id_index[0];
    *att_id = id_index[1];
    return 0;
}

//返回扫描得热点列表
int ble_scan_result(uint16_t pre_id, uint16_t att_id)
{
    iotwifilist *value = NULL;
    uint16_t i,length = 43;
    uint8_t len = 0;
    get_wifi_ap_list(&value, &len);

   // bk_printf("value = %s ,len = %d\r\n",value,len);
    for(i = 0; i < len; i++)
    {
        if(ERR_SUCCESS != bk_ble_send_ntf_value(length, (uint8_t *)&value[i], pre_id, att_id-1))
        {
            bk_printf("ERROR\r\n");
        }
    }

     return 0;   
}

//返回通知得结果
int event_status_result(uint16_t pre_id, uint16_t att_id, uint8_t *value, uint16_t length)
{
    bk_printf("value = %d ,len = %d\r\n",value[0],length);
    if(ERR_SUCCESS != bk_ble_send_ntf_value(length, value, pre_id, att_id-1))
    {
        bk_printf("ERROR\r\n");
    }
    return 0;
}


