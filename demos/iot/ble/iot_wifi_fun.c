/*
 * @Author: i4season.xiang
 * @Date: 2021-07-29 18:42:00
 * @LastEditTime: 2021-08-03 17:32:40
 * @FilePath: \beken_freertos_sdk_release-SDK_BL2028N_3.0.33_belon\beken_freertos_sdk_release-SDK_BL2028N_3.0.33\demos\wifi\wifi_fun\wifi_fun.c
 */
#include "iot_common.h"
#include "iot_wifi_fun.h"
#include "rw_msg_rx.h"
static int aplen = 0;
static int8_t *scanid;

static iotwifilist wifilist[10];



static ScanResult_adv apList;
//ScanResult_adv *apList;
static const char *crypto_str[] = {
		"None",
		"WEP",
		"WPA_TKIP",
		"WPA_AES",
		"WPA2_TKIP",
		"WPA2_AES",
		"WPA2_MIXED",		////BK_SECURITY_TYPE_WPA3_SAE
		"WPA3_SAE",	  		/**< WPA3 SAE */
		"WPA3_WPA2_MIXED",	/** WPA3 SAE or WPA2 AES */
		"AUTO",
};

static void scan_result(void *ctxt, uint8_t param)
{
	int i,ssidlen=0;
	aplen = 0;
	if (wlan_sta_scan_result(&apList) == 0) {
		int ap_num = apList.ApNum;
		
		bk_printf("Got ap count: %d\r\n", ap_num);
		if(ap_num > 10){
			aplen = 10;
		}else{
			aplen = ap_num;
		}
		os_memset(wifilist,0,sizeof(wifilist));
		for (i = 0; i < aplen; i++){
			apList.ApNum = i;
			bk_printf("  %d  \"%s\", " MACSTR "\b, %d, %s, %d\n",
					apList.ApNum, apList.ApList[i].ssid, MAC2STR(apList.ApList[i].bssid),
					apList.ApList[i].ApPower, crypto_str[apList.ApList[i].security],
					apList.ApList[i].channel);
			wifilist[i].ApNum = apList.ApNum;
			wifilist[i].ssidlen = strlen(apList.ApList[i].ssid);
			arr_strcpy(wifilist[i].SSID, apList.ApList[i].ssid);
			os_memcpy(wifilist[i].BSSID, apList.ApList[i].bssid, sizeof(wifilist[i].BSSID));
			wifilist[i].RSSI = apList.ApList[i].ApPower;
			wifilist[i].Security = apList.ApList[i].security;
		}
		os_free(apList.ApList);
		
	}
	*scanid = *scanid + 1;
	return;
}


void wifiscan(int8_t *id){
	scanid = id;
	mhdr_scanu_reg_cb(scan_result, 0);
	bk_wlan_start_scan();
	return;
}

void get_wifi_ap_list(iotwifilist **plist , uint8_t *num)
{
	*plist = wifilist;
	*num = aplen;
	return;

}

