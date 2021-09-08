#ifndef IOT_BLE_H
#define IOT_BLE_H
#ifdef __cplusplus
extern "C"
{
#endif
// #include "iot_common.h"
#include "MQTTFreeRTOS.h"

#ifndef arr_strcpy
#define arr_strcpy(dst,src) do{os_strncpy((char*)dst,(char*)src,sizeof(dst)-1);dst[sizeof(dst)-1]=0;}while(0)
#endif

#define IOT_TYPE_MQTT 1
#define IOT_TYPE_UART 2
#define MCUMODES "mcu_hardware_model"
#define MCUFIRMS "mcu_firmware_version"
#define MCUON "mcuon"
#define  FIRMMODES "module_hardware_model"
#define  FIRFIRMS "module_firmware_version"
#define WIFIRSSI "wifi_rssi"
#define WIFICONNECT "connected"
#define NLATENCY "network_latency"
#define MODELMAC "module_hardware_mac"

	#define ONLY_UPLOAD 0
	#define UPLOAD_DESIRED 1
	struct mcuChange
	{
		/** The name of the CLI command */
		const char *name;
		int8_t id;
		int8_t type;
		int8_t len;
		int8_t sendtype;
		int8_t timetype;
	};

	enum {
		S_MCU_DISCONNECT,
		S_MCU_CONNECT,
		S_MCU_GET_DATAOK,
	};
	typedef union {
		void *raw;
		int8_t *bvalue;
		int *value;
		char *str;
		int8_t *enumValue;
	}iotData;
	struct mcuInfoData{
		int8_t id;
		uint8_t type;
		int16_t len;
		iotData value;
		//uint8_t *value;
		int8_t change;

		int16_t desiredLen;
		iotData desiredValue;
		int8_t desiredChang;
		int8_t desiredInit;
		int8_t sendtype;
		int8_t timetype;
		int desiredTime;
		int reportTime;
		struct mcuInfoData *next;
	};
	struct mcuFirmInfo{
		char mcumode[16];
		int8_t mcumode_diff;//mqtt

		char mcuver[16];
		int8_t mcuver_diff;//mqtt

		char mcutype[5];
		int8_t mcutype_diff;//
		uint8_t mcuok;
		int8_t mcuok_diff;//mqtt
	};

	struct mcuWifiInfo{
		uint8_t wifimode;
		int8_t wifimode_diff;//mcu
		uint8_t wifistatus;
		int8_t wifistatus_diff;//mcu
	};

	struct mcuInfo{
		struct mcuFirmInfo firmInfo;
		struct mcuWifiInfo wifiInfo;
	};
	typedef struct {
		int8_t netState;
		Timer netTimer;
		int8_t netcount;
		int8_t blueStart;
		int8_t reset;
		int8_t mcureset;
		int8_t mcuState;
		Timer mcuTimer;
		int8_t muccount;		
		int8_t testbadcount;
		int8_t productInfoAck;
		int8_t WifiModeAck;
		int8_t WifiNotifyAck;
		int8_t DataPointNotifyAck;
		Timer uuidTimer;
		char mqttuuid[64];
		char devmac[18];
		char mqttsendflag;
		int mqttuuidnum;
	}devstate;
	enum {
		IOT_MSG_BUTTON = 1,
		IOT_MSG_OTA = 2,
		IOT_MSG_OTAERROR = 3,
		IOT_MSG_RECONNECT = 4,
		IOT_MSG_RESTART = 5,
		IOT_MSG_UARTERROR = 6,

	};

	typedef struct{
		int8_t src;
		int8_t ts;
		int8_t button;
		int8_t type4;
	}msginfo;
	typedef union {
		void *data;
		msginfo info;
	}msgdata;
	
	typedef struct{
		int8_t type;
		int timestamp;
		msgdata mdata;
	}MSGDATA;

enum {
		IOT_TYPE_MQTT_STOP =0X01,
		IOT_TYPE_MQTT_START =0X02,
		IOT_TYPE_MQTT_PUB  =0X03,
		IOT_TYPE_MQTT_MSG  =0X04,
		IOT_TYPE_MQTT_GET  =0X05,
		IOT_TYPE_MQTT_PUBACK =0X06,
		IOT_TYPE_MQTT_GETACK =0X07,
		IOT_TYPE_MQTT_SUB =0X08,
		IOT_TYPE_MQTT_IOT =0X09,
	
	};
#define TimeNotConnect 1000
#define TimeConnect 60000
extern char *firmver;
extern char *otastr;
extern char *otastr2;

//#define IOT_IPERFTEST
extern int8_t testmcuflag;

struct firmInfo{
	char ver[16];
	int8_t ver_diff;//mqtt
	char mode[16];
	int8_t mode_diff;//mqtt
	int wifi_rssi;
	int8_t wifi_rssi_diff;//mqtt
	int network_latency;
	int8_t network_latency_diff;//mqtt
	int8_t connected;
	int8_t connected_diff;//mqtt
	char devmac[20];
	int8_t devmac_diff;//mqtt

};
extern struct firmInfo *firminfop;
//#define MCUTEST 1








#ifndef IOT_FREE
#define IOT_FREE(x) do{if(x != NULL)os_free(x); x= NULL;}while(0)
#endif
extern void iot_printf(const char *fmt, ...);
#define IODEBUG iot_printf
void iot_init(void);


typedef enum
{
	PairingID = 0,
	wifiStatus = 1,
	NetStatus = 2,
	WifiScan = 3,
	WifiConnect = 4,
	WifiStart = 5,
	WifiConnectWifi = 6,

} bleNotifyType;
typedef enum
{
	PairingFalse = 0,
	PairingTrue = 1,
} blePairingStatus;

typedef enum
{
	WifiStatusNone = 0,
	WifiStatusFail = 1,
	WifiStatusSuccess = 2,
} bleWifiStatus;

typedef enum
{
	NetRegisterFail = 0x00,
	NetRegisterSuccess = 0x01,
	NetMqttFail = 0x10,
	NetMqttSuccess = 0x11,
} bleNetStatus;

typedef enum
{
	BleStatusStop = 0,
	BleStatusStart = 1,
} bleBleStatus;

typedef enum
{
	BleDisConnect = 0,
	BleConnect = 1,
} bleBleConnectStatus;






typedef struct{
	int8_t  type;//bleNotifyType
	int8_t  status;
	int8_t  id;
}bleNotify;
typedef struct{
	blePairingStatus pairingStatus;
	bleWifiStatus wifiStatus;
	bleNetStatus netStatus;
	bleBleStatus bleStatus;
	bleBleConnectStatus bleConnectStatus;
	int8_t bleconnect;
	int8_t  id;
	int8_t  scanid;
	int8_t  connetWifiFlag;
	Timer bletTimer;
	uint8_t wifi_mac_buf[6];
	char name[32];

}iotBleInfo;
extern iotBleInfo * gIotBleInfo;

void ioit_ble_int(void);

//bk_wlan_stop(BK_STATION);
//1字节，设备注册/AWS接入结果：
//设备注册结果: 0x00:失败 0x01:成功
//服务接入结果: 0x10:失败 0x11:成功
void sendWifiChangeStatus(uint8_t status , int bleid);
//1字节，WiFi模组状态：0x00:未配置(无WiFi配网信息) 0x01:未连接 0x02:已连接
void sendNetChangeStatus(uint8_t status,int bleid);
//1字节，模组是否允许配网标识：0x00:false 0x01:true
void sendPairChangeStatus(uint8_t status,int bleid);
int getBleid(void);
void iot_scanWifi(void);
void iot_connectWifi(void);
int getwifiStatus(void);
void iot_ble_start(bleBleStatus flag);
uint16_t getwifistate(void);
extern unsigned char gWifiMac[6];
int getpair(void);

void testwifi(uint8_t index);
#define KEEPAP 300000

void ioit_keepap(void);



#ifdef __cplusplus
}
#endif


#endif

