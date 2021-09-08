#include "include.h"
#include "demos_config.h"

#include "cJsontest.h"

//#if CJSON_TEST_DEMO
#include <stdio.h>
#include "cJSON.h"
#include "mem_pub.h"
#include "uart_pub.h"
#include "stdarg.h"
#include "include.h"
#include "rtos_pub.h"
#include "error.h"
#include "portmacro.h"
// #include"iot_pro.h"
// #include"iot_https.h"

// #include "iot_aws_mqtt.h"
// #include "iot_uart_pro.h"
#include "iot_ble.h"
#include "portmacro.h"
#include "string.h"
#include "common.h"
#include "net.h"
#include "wlan_ui_pub.h"
#include "iot_ble_service.h"
#define BLE_NUM 10

beken_thread_t  iot_handle = NULL;
devstate gDevstate;
devstate *gDevstatep = &gDevstate;
xQueueHandle ble_queue = NULL;
xQueueHandle ble_notify = NULL;
static iotBleInfo BleInfo;
iotBleInfo * gIotBleInfo = &BleInfo;


void testconforzero(void){
	// int socket;
	// socket = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);
	// return;

}

void iot_ble_start(bleBleStatus flag){
	bleNotify data;
	BaseType_t qret;
	data.id = 0;
	data.status = flag;
	data.type = WifiStart;
	qret = xQueueSend(ble_queue, &data, 1000);
	if (qret != pdPASS){
		bk_printf("error\n");
	}
	return;	
}

void doThings(devstate *status){
    iot_ble_start(BleStatusStart);
}

int pro_main(void){
    doThings(gDevstatep);
    return 0;
}


static void init_app_thread3( void *arg )
{
	pro_main();
	while(1){
		bk_printf("available heap %d\r\n", xPortGetFreeHeapSize());
		vTaskDelay(10000);
	}
	//vTaskDelay(10000);
	rtos_delete_thread( NULL );

}

void handleBleNet(void *pvParameters){
	bleNotify data;
	BaseType_t qret;

	while(1){
		qret = xQueueReceive(ble_queue, &data, 1000);
		if (qret == pdPASS){
			bk_printf("data.type=%d,status=%d\n",data.type,data.status);
			switch(data.type){
				case WifiStart:
					bk_printf("to BleStatusStart\n");
					i4_start_ble_service();
					break;
				default:
					break;
			}
		}
	}
}

void ioit_ble_int(void){
#if 1	
	ble_queue = xQueueCreate(BLE_NUM, sizeof(bleNotify));
	ble_notify  = xQueueCreate(1, 1);
	os_memset(gIotBleInfo, 0, sizeof(iotBleInfo));
	memcpy(gIotBleInfo->wifi_mac_buf,gWifiMac,sizeof(gIotBleInfo->wifi_mac_buf));
	bk_printf("mac" MACSTR "-hhh\n",MAC2STR(gIotBleInfo->wifi_mac_buf));
	#define MAC2STRT(a) (a)[4], (a)[5]
	#define MACSTRT "%02x%02x"
	//char name3[32];
	os_snprintf(gIotBleInfo->name,sizeof(gIotBleInfo->name),"Dreo DR-HTF001-" MACSTRT,MAC2STRT(gWifiMac));

	rtos_create_thread(NULL,
						THD_EXTENDED_APP_PRIORITY,
						   "handleBleNet",
						   (beken_thread_function_t)handleBleNet,
						   1024,
						   (beken_thread_arg_t)0);	

	// TimerInit(&badNetTimer);
	// TimerInit(&badNetSuccesTimer);
	// TimerInit(&resetNetTimer);
	// TimerInit(&gIotBleInfo->bletTimer);
#endif
		
}

void iot_init(void){
#if 0
    if(initflag != 0)
		return;
	initflag = 1;
	iot_printf("available heap %d\r\n", xPortGetFreeHeapSize());
	mqtt_queue_back = xQueueCreate(PUBLISH_NUM, sizeof(void *));
	mqtt_pub_queue = xQueueCreate(PUBLISH_NUM, sizeof(void *));
	main_notify = xQueueCreate(1, 1);
	mqtt_sub_queue = xQueueCreate(PUBLISH_NUM, sizeof(void *));



	os_memset(&firminfo,0,sizeof(firminfo));
	os_memset(mcuinfop,0,sizeof(struct mcuInfo));
	arr_strcpy(firminfo.ver,firmver);
	arr_strcpy(firminfo.mode,"iot");
	os_snprintf(firminfo.devmac,sizeof(firminfo.devmac),MACSTR22,MAC2STR(gWifiMac));
	firminfo.wifi_rssi = 100;
	firminfo.network_latency = 101;
	firminfo.connected = 1;
	awsData_init();
	aws_data2_init();
	//vTaskDelay(500);
	
#endif
	vTaskDelay(1000);
	ioit_ble_int();
	vTaskDelay(500);
	// iot_uartpro_init(notifyCmd);

}

static void iot_cgi_thread( void *arg ){

	// testconforzero();
	vTaskDelay(1000);
	iot_init();
#if 0
	rtos_create_thread(NULL,
							 4,
							 "app_tcp",
							 (beken_thread_function_t)app_tcp_main,
							 2048,
							 (beken_thread_arg_t)NULL); 
#endif

	rtos_create_thread(NULL,
			THD_INIT_PRIORITY,
			"mqtt3",
			(beken_thread_function_t)init_app_thread3,
			4096,
			(beken_thread_arg_t)0);


#if 0
	rtos_create_thread(NULL,
			THD_EXTENDED_APP_PRIORITY,
			"mqtt4",
			(beken_thread_function_t)amazon_awsiot_thread6,
			4096,
			(beken_thread_arg_t)0);
#endif
	vTaskDelay(3000);
	while(1){
		vTaskDelay(50000);
	}
	rtos_delete_thread( NULL );
}

void iot_cgi(void){
	rtos_create_thread(&iot_handle,
			BEKEN_DEFAULT_WORKER_PRIORITY,
			"iot_cgi",
			(beken_thread_function_t)iot_cgi_thread,
			4096,
			(beken_thread_arg_t)0);
	return;

}