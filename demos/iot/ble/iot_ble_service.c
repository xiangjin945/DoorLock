/*
 * @Author: i4season.xiang
 * @Date: 2021-08-03 22:05:18
 * @LastEditTime: 2021-09-07 18:07:34
 * @FilePath: /ProJect_DIR/7231u_ble_Config_Network/beken_freertos_sdk_release-SDK_BL2028N_3.0.33/demos/iot/ble/iot_ble_service.c
 */
#include "string.h"
// #include "ble_api_5_x.h"
#include "ble_api.h"
#include "iot_ble_service.h"
#include "generic.h"
#include "str_pub.h"
#include "iot_ble_scan.h"
#include "rtos_pub.h"
#include "iot_wifi_fun.h"
#include "iot_ble.h"
#include "ble.h"
#include "application.h"

#define BK_ATT_DECL_PRIMARY_SERVICE_128     {0x00,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define BK_ATT_DECL_CHARACTERISTIC_128      {0x03,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define BK_ATT_DESC_CLIENT_CHAR_CFG_128     {0x02,0x29,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

#define WRITE_REQ_CHARACTERISTIC_128        {0x01,0xFF,0,0,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}
#define INDICATE_CHARACTERISTIC_128         {0x02,0xFF,0,0,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}
#define NOTIFY_CHARACTERISTIC_128           {0x03,0xFF,0,0,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}

#define DSN_CHARACTERISTIC_128              {0x0a,0x18,0xb3,0xff,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}
#define DSN_MODULE_STATUS                   {0x0b,0x18,0xb3,0xff}
#define DSN_WIFI_MAC_ADDR                   {0x0c,0x18,0xb3,0xff}

#define VERSION_CHARACTERISTIC_128          {0x01,0x18,0xb4,0xff,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}
#define SCAN_CHARACTERISTIC_128             {0x02,0x18,0xb4,0xff,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}
#define SCANRESULT_CHARACTERISTIC_128       {0x03,0x18,0xb4,0xff,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}
#define CONNECT_CHARACTERISTIC_128          {0x04,0x18,0xb4,0xff,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}
#define STATUS_CHARACTERISTIC_128           {0x05,0x18,0xb4,0xff,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}
#define WIFI_CONFIG_USER_ID                 {0x06,0x18,0xb4,0xff,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}
#define WIFI_CONFIG_API_INFO                {0x07,0x18,0xb4,0xff,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}

#define EVENT_NTF_API_RESULT                {0x0a,0x18,0xb5,0xff,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}
#define EVENT_NTF_PAIRING                   {0x0b,0x18,0xb5,0xff,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}
#define EVENT_NTF_WIFI_STATUS               {0x0C,0x18,0xb5,0xff,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}

static const uint8_t test_svc_uuid[16] = {0xb3,0xFF,0,0,0,0,0,0,0x28,0x37,0,0,0,0};
static const uint8_t scan_svc_uuid[16] = {0xb4,0xff,0,0,0x05,0x18};
static const uint8_t ctrl_svc_uuid[16] = {0xb5,0xFF,0,0,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0};

enum
{
	ATT_IDX_SVC,
	ATT_IDX_FF01_VAL_CHAR,
	ATT_IDX_FF01_VAL_VALUE,
	ATT_IDX_FF02_VAL_CHAR,
	ATT_IDX_FF02_VAL_VALUE,
	ATT_IDX_FF02_VAL_IND_CFG,
	ATT_IDX_FF03_VAL_CHAR,
	ATT_IDX_FF03_VAL_VALUE,

    ATT_IDX_FF04_VAL_CHAR,
	ATT_IDX_FF04_VAL_VALUE,
    ATT_IDX_FF05_VAL_CHAR,
	ATT_IDX_FF05_VAL_VALUE,
    ATT_IDX_FF05_VAL_IND_CFG,
    ATT_IDX_FF06_VAL_CHAR,
	ATT_IDX_FF06_VAL_VALUE,
    ATT_IDX_FF07_VAL_CHAR,
	ATT_IDX_FF07_VAL_VALUE,

	ATT_IDX_NB,
};


/***********************声明*****************************/
uint8_t ble_read_callback(read_req_t *read_req);
void ble_write_callback(write_req_t *write_req);
void ble_event_callback(ble_event_t event, void *param);
extern void ble_advertise(void);
// void WiFi_parsing_ssid_key(uint8_t *value,uint16_t len);
/***********************声明*****************************/

uint8_t wifi_connect_status[2]  = {0};
uint8_t event_ntf_api[2]        = {0};
uint8_t event_ntf_priring[2]    = {0};
uint8_t event_ntf_wifistatus[2] = {0};



bk_attm_desc_t att_db[ATT_IDX_NB] =
{
	//  Service Declaration
	[ATT_IDX_SVC]              = {BK_ATT_DECL_PRIMARY_SERVICE_128, BK_PERM_SET(RD, ENABLE), 0, 0},

	//  Level Characteristic Declaration
	[ATT_IDX_FF01_VAL_CHAR]    = {BK_ATT_DECL_CHARACTERISTIC_128,  BK_PERM_SET(RD, ENABLE), 0, 0},
	//  Level Characteristic Value
	[ATT_IDX_FF01_VAL_VALUE]   = {VERSION_CHARACTERISTIC_128,      BK_PERM_SET(NTF, ENABLE)|BK_PERM_SET(RD, ENABLE), BK_PERM_SET(RI, ENABLE)|BK_PERM_SET(UUID_LEN, UUID_32), 128},

	[ATT_IDX_FF02_VAL_CHAR]    = {BK_ATT_DECL_CHARACTERISTIC_128,  BK_PERM_SET(RD, ENABLE), 0, 0},
	//  Level Characteristic Value
	[ATT_IDX_FF02_VAL_VALUE]   = {SCAN_CHARACTERISTIC_128,         BK_PERM_SET(NTF, ENABLE)|BK_PERM_SET(RD, ENABLE)|BK_PERM_SET(WRITE_REQ, ENABLE), BK_PERM_SET(RI, ENABLE)|BK_PERM_SET(UUID_LEN, UUID_32), 128},
	// //  Level Characteristic - Client Characteristic Configuration Descriptor
    [ATT_IDX_FF02_VAL_IND_CFG] = {BK_ATT_DESC_CLIENT_CHAR_CFG_128, BK_PERM_SET(RD, ENABLE)|BK_PERM_SET(WRITE_REQ, ENABLE), 0, 0},


    [ATT_IDX_FF03_VAL_CHAR]    = {BK_ATT_DECL_CHARACTERISTIC_128,  BK_PERM_SET(RD, ENABLE), 0, 0},
	//  Level Characteristic Value
	[ATT_IDX_FF03_VAL_VALUE]   = {SCANRESULT_CHARACTERISTIC_128,   BK_PERM_SET(WRITE_REQ, ENABLE)|BK_PERM_SET(WRITE_COMMAND, ENABLE), BK_PERM_SET(RI, ENABLE)|BK_PERM_SET(UUID_LEN, UUID_32), 128},

    [ATT_IDX_FF04_VAL_CHAR]    = {BK_ATT_DECL_CHARACTERISTIC_128,  BK_PERM_SET(RD, ENABLE), 0, 0},
	[ATT_IDX_FF04_VAL_VALUE]   = {CONNECT_CHARACTERISTIC_128,      BK_PERM_SET(WRITE_REQ, ENABLE)|BK_PERM_SET(WRITE_COMMAND, ENABLE), BK_PERM_SET(RI, ENABLE)|BK_PERM_SET(UUID_LEN, UUID_32), 300},

	[ATT_IDX_FF05_VAL_CHAR]    = {BK_ATT_DECL_CHARACTERISTIC_128,  BK_PERM_SET(RD, ENABLE), 0, 0},
	[ATT_IDX_FF05_VAL_VALUE]   = {STATUS_CHARACTERISTIC_128,       BK_PERM_SET(NTF, ENABLE)|BK_PERM_SET(RD, ENABLE), BK_PERM_SET(RI, ENABLE)|BK_PERM_SET(UUID_LEN, UUID_32), 128},
    [ATT_IDX_FF05_VAL_IND_CFG] = {BK_ATT_DESC_CLIENT_CHAR_CFG_128, BK_PERM_SET(RD, ENABLE)|BK_PERM_SET(WRITE_REQ, ENABLE), 0, 0},

    [ATT_IDX_FF06_VAL_CHAR]    = {BK_ATT_DECL_CHARACTERISTIC_128,  BK_PERM_SET(RD, ENABLE), 0, 0},
	[ATT_IDX_FF06_VAL_VALUE]   = {WIFI_CONFIG_USER_ID,             BK_PERM_SET(WRITE_REQ, ENABLE)|BK_PERM_SET(WRITE_COMMAND, ENABLE), BK_PERM_SET(RI, ENABLE)|BK_PERM_SET(UUID_LEN, UUID_32), 20},

    [ATT_IDX_FF07_VAL_CHAR]    = {BK_ATT_DECL_CHARACTERISTIC_128,  BK_PERM_SET(RD, ENABLE), 0, 0},
	[ATT_IDX_FF07_VAL_VALUE]   = {WIFI_CONFIG_API_INFO,            BK_PERM_SET(WRITE_REQ, ENABLE)|BK_PERM_SET(WRITE_COMMAND, ENABLE), BK_PERM_SET(RI, ENABLE)|BK_PERM_SET(UUID_LEN, UUID_32), 300},
};



ble_err_t bk_ble_connect_init(void)
{
    ble_err_t status = ERR_SUCCESS;

    struct bk_ble_db_cfg ble_db_cfg;

    ble_db_cfg.att_db = att_db;
    ble_db_cfg.att_db_nb = ATT_IDX_NB;
    ble_db_cfg.prf_task_id = 0;
    ble_db_cfg.start_hdl = 0;
    ble_db_cfg.svc_perm = BK_PERM_SET(SVC_UUID_LEN, UUID_32);
    memcpy(&(ble_db_cfg.uuid[0]), &scan_svc_uuid[0], 16);

	do{
   		status = bk_ble_create_db(&ble_db_cfg);
		vTaskDelay(100);
		bk_printf("status=%d\n",status);
	}while(status != ERR_SUCCESS);

    return status;
}

// void i4_ble_cmd_cb(ble_cmd_t cmd, ble_cmd_param_t *param)
// {
// 	//cmd:14 idx:0 status:0 连接上
// 	//cmd:3 idx:0 status:0 断开
// 	bk_printf("cmd:%d idx:%d status:%d\r\n", cmd, param->cmd_idx, param->status);
// 	if(cmd == 14){
		
// 		i4bleid++;
// 		i4wifistatus[1] = 0;
// 	}
// 	if(cmd == 3){
// 		i4bleid++;
// 	}
// }
#if 0
const char dsn_buf[52] = "1401783383044132866-cced7e54dd8c4bcc8679447b3255d3f1";
void read_event_cbs(void *param)
{
    read_req_t *red = (read_req_t *)param;
    if(red->att_idx == 2){
        memcpy(red->value,dsn_buf,strlen(dsn_buf));
        red->size = 52;
        red->length = 52;
    }
    red->value[0] = 1;
    red->value[1] = 2;
    red->length = 2;
}
#endif
static int i4bleinit = 0;
static int i4blestart = 0;

void i4_init_ble_service(void)
{
	if(i4bleinit){
		return;
	}    
    i4bleinit = 1;
	ble_set_write_cb(ble_write_callback);
    ble_set_read_cb(ble_read_callback);
    ble_set_event_cb(ble_event_callback);

	ble_activate(NULL);
	bk_printf(" init ble end %d\r\n!", xPortGetFreeHeapSize());
	vTaskDelay(1000);

}
/**
 * @description: 开启蓝牙服务
 */
void i4_start_ble_service(void)
{
	ble_err_t status = ERR_SUCCESS;
	if(i4blestart)
		return;
	i4blestart = 1;
    //(3000);

	i4_init_ble_service();


	ble_advertise();

	bk_printf("bk_ble_start_advertising=%d\n",status);


	
    //bk_ble_start_advertising(os_strtoul(0, NULL, 10), 0, i4_ble_cmd_cb);
	bk_printf(" start end heap %d\r\n", xPortGetFreeHeapSize());
	vTaskDelay(1000);

}
/**
 * @description: 停止蓝牙服务
 */
void i4_stop_ble_service(void){
   bk_printf(" heap %d\r\n", xPortGetFreeHeapSize());
	if(i4blestart == 0)
		return;
   i4blestart = 0;
   appm_disconnect();
   appm_stop_advertising();
   //bk_ble_delete_advertising(os_strtoul(0, NULL, 10), i4_ble_cmd_cb);
}

/**
 * @description: 蓝牙读取事件回调
 * @param {read_req_t} *read_req
 * @return {*}返回读出的数据长度
 */
uint8_t ble_read_callback(read_req_t *read_req)
{
    bk_printf("read_cb[prf_id:%d, att_idx:%d]\r\n", read_req->prf_id, read_req->att_idx);
    read_req->value[0] = 0x10;
    read_req->value[1] = 0x20;
    read_req->value[2] = 0x30;
    return 3;
}
/**
 * @description: 写事件回调
 * @param {write_req_t} *write_req
 */
void ble_write_callback(write_req_t *write_req)
{
    bk_printf("write_cb[prf_id:%d, att_idx:%d, len:%d]\r\n", write_req->prf_id, write_req->att_idx, write_req->len);
    for(int i = 0; i < write_req->len; i++)
    {
        bk_printf("0x%x ", write_req->value[i]);
    }
    bk_printf("\r\n");
}

/**
 * @description: 事件回调处理
 * @param {ble_event_t} event
 * @param {void} *param
 */
void ble_event_callback(ble_event_t event, void *param)
{
    switch(event)
    {
        case BLE_STACK_OK:
        {
            os_printf("STACK INIT OK\r\n");
            bk_ble_connect_init();
        }
        break;
        case BLE_STACK_FAIL:
        {
            os_printf("STACK INIT FAIL\r\n");
        }
        break;
        case BLE_CONNECT:
        {
            os_printf("BLE CONNECT\r\n");
        }
        break;
        case BLE_DISCONNECT:
        {
            os_printf("BLE DISCONNECT\r\n");
        }
        break;
        case BLE_MTU_CHANGE:
        {
            os_printf("BLE_MTU_CHANGE:%d\r\n", *(uint16_t *)param);
        }
        break;
        case BLE_TX_DONE:
        {
            os_printf("BLE_TX_DONE\r\n");
        }
        break;
        case BLE_GEN_DH_KEY:
        {
            os_printf("BLE_GEN_DH_KEY\r\n");
            os_printf("key_len:%d\r\n", ((struct ble_gen_dh_key_ind *)param)->len);
            for(int i = 0; i < ((struct ble_gen_dh_key_ind *)param)->len; i++)
            {
                os_printf("%02x ", ((struct ble_gen_dh_key_ind *)param)->result[i]);
            }
            os_printf("\r\n");
        }
        break;
        case BLE_GET_KEY:
        {
            os_printf("BLE_GET_KEY\r\n");
            os_printf("pub_x_len:%d\r\n", ((struct ble_get_key_ind *)param)->pub_x_len);
            for(int i = 0; i < ((struct ble_get_key_ind *)param)->pub_x_len; i++)
            {
                os_printf("%02x ", ((struct ble_get_key_ind *)param)->pub_key_x[i]);
            }
            os_printf("\r\n");
        }
        break;
        case BLE_CREATE_DB_OK:
        {
            os_printf("CREATE DB SUCCESS\r\n");
        }
        break;
        default:
            os_printf("UNKNOW EVENT\r\n");
        break;
    }
}

