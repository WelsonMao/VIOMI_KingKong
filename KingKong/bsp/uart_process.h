#include <assert.h>
#include "../scene.h"

//#define ENABLE_COM_RETRY
#ifdef ENABLE_COM_RETRY
#define RETRY_MAX_CNT                   3
#endif

#define FRAME_OFFSET_FRAMETYPE              1
#define FRAME_OFFSET_REGADDRESS_H           2
#define FRAME_OFFSET_REGADDRESS_L           3
#define FRAME_OFFSET_BYTECNT                2
#define FRAME_OFFSET_PAYLOAD                3

#define REG_ADDRESS_DEVINFO                 0
#define REG_ADDRESS_DEVINFO_END             95
#define REG_ADDRESS_DEVINFO_LEN             (REG_ADDRESS_DEVINFO_END-REG_ADDRESS_DEVINFO+1)

#define REG_ADDRESS_WIFI_RST                130
#define REG_ADDRESS_UPGRADE                 131

#define REG_ADDRESS_DEV                     132
#define REG_ADDRESS_DEV_END                 160
#define REG_ADDRESS_DEV_LEN                 (REG_ADDRESS_DEV_END-REG_ADDRESS_UPGRADE+1)
#define REG_ADDRESS_TARGET_TEMPER           136
#define REG_ADDRESS_REAL_TEMPER             137
#define REG_ADDRESS_MODE                    138
#define REG_ADDRESS_POWER                   139
#define REG_ADDRESS_STATUS                  140
#define REG_ADDRESS_WIFI                    141
#define REG_ADDRESS_POWER_BREAK             142
#define REG_ADDRESS_KEEP_WARM               145
#define REG_ADDRESS_YEAR                    146
#define REG_ADDRESS_HOUR                    149
#define REG_ADDRESS_FILTER                  152
#define REG_ADDRESS_SMART                   157

#define REG_ADDRESS_DELAY                   161
#define REG_ADDRESS_DELAY_END               176
#define REG_ADDRESS_DELAY_LEN              (REG_ADDRESS_DELAY_END-REG_ADDRESS_DELAY+1)
#define REG_ADDRESS_DELAY_M0                161
#define REG_ADDRESS_DELAY_M1                165
#define REG_ADDRESS_DELAY_M2                169
#define REG_ADDRESS_DELAY_AI                173
#define REG_ADDRESS_DELAY_SINGLE_LEN         3
#define REG_ADDRESS_TOTAL_LEN               (REG_ADDRESS_DELAY_END-REG_ADDRESS_DEV+1)

/***************************************************************/
typedef	 volatile  union{
	struct{
		unsigned char bit0 : 1;
		unsigned char bit1 : 1;
		unsigned char bit2 : 1;
		unsigned char bit3 : 1;
		unsigned char bit4 : 1;
		unsigned char bit5 : 1;
		unsigned char bit6 : 1;
		unsigned char bit7 : 1;
	}bits;
	unsigned char byte;
}Bit_Set;

typedef enum
{
	STEP_ID_NULL = 0,
	STEP_ID_WEIGH,
	STEP_ID_WASH,
	STEP_ID_SPIN,
	STEP_ID_RINSE,
	STEP_ID_DRY,
	STEP_ID_FINISH,
	STEP_ID_DELAY,
	STEP_ID_MAX
}TYPE_ID_e;
typedef union
{
	char buf[REG_ADDRESS_DEVINFO_LEN];
	 struct{
		 char cManufacturer[32];
		 char cMode[16];
		 char cSerialNO[16];
		 char cFWVer[32];
	}pkt;
}REV_DEVINFO_u;
typedef union
{
	uint8_t buf[4];
	struct{
		uint8_t bStatus;
		uint8_t bStartTime;
		uint8_t bEndTime;
		uint8_t bEnable;
	}pkt;
}DELAY_FORMAT_u;
typedef union
{
	uint8_t buf[16];
	struct{
		DELAY_FORMAT_u uDelayManual[3];//20
		DELAY_FORMAT_u uDelayAI; //21
	}pkt;
}REV_DELAY_u;

typedef union
{
	uint8_t buf[REG_ADDRESS_DEV_LEN];
	 struct{
		uint8_t bUpgrade;
		uint8_t bFault[4];            //1
		uint8_t bTargetTemper;        //2
		uint8_t bTemper;              //3
		uint8_t bMode;                //4
		uint8_t bPowerOnOff;          //5
		uint8_t bStatus;              //6
		uint8_t bWiFiStatus;          //7
		uint8_t bPowerBreakEnable;    //8
		uint8_t bMagneRodEnable;      //9
		uint8_t bSurplusHotWater;     //10
		uint8_t bKeepWarmEnable;      //11 
		uint8_t bYear;                //12
		uint8_t bMonth;               //13
		uint8_t bDay;                 //14
		uint8_t bHour;                //15
		uint8_t bMin;                 //16
		uint8_t bSec;                 //17
		uint8_t bFilterLife[2];       //18
		uint8_t bTemperDown;          //19
		uint8_t bFlowMeter;			  //20
		uint8_t HeatRelay;			  //21
		uint8_t bBacterEnable;        //22
		uint8_t bBacterCycle;		  //23
		uint8_t bBacterStatus;		  //24
		uint8_t bBacterRemainTime;	  //25
	}pkt;
}REV_DEV_u;
typedef struct
{	
	uint8_t TransmitCMD;
	uint32_t TimeBaseTick;
	uint8_t ResponseState;
	uint8_t RetryCnt;
	uint8_t InOutState;
	bool    TransmitEnable;
	bool    RevEnable;
	uint16_t BaseAddress;
}COM_STATE_t;
extern REV_DEVINFO_u uRevDevInfo;
extern REV_DEV_u   uTransmitDev,uRevDev ;
extern REV_DELAY_u uRevDelay;
extern COM_STATE_t tComState;
/***************************************************************/
uint32_t ACUGet_Fault(void);
uint32_t ACUGet_TargetTemper(void);
uint32_t ACUGet_Temper(void);
uint8_t ACUGet_Mode(void);
uint8_t ACUGet_PowerStatus(void);
uint8_t ACUGet_DevStatus(void);
uint8_t ACUGet_WiFiStatus(void);
uint8_t ACUGet_SmartStatus(void);
uint8_t ACUGet_SmartRemainTime(void);
bool ACUChk_MCUUpgrade(void);
uint8_t ACUGet_UpgradeStep(void);
/**************************************************************/
extern void RevFrameAnalysis(uint8_t *buf, uint8_t len);
/**************************************************************/