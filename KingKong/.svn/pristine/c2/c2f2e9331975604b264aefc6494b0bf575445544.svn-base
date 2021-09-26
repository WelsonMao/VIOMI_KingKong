#include <assert.h>
#include "../scene.h"
/*global define*/
// need to install VS90SP1-KB980263-x86.exe for vs2008
#pragma execution_character_set("utf-8")
/***********************************************************************************************
*1. 串口发送波特率：9600Bit/S。	
*2. 字节发送格式为：起始1位+数据8位+终止1位。	
*3. 数据发送格式：头帧0x6B（1Byte）+ 主从（1Byte）+ 命令（1Byte）+命令具体指示（1Byte）+求和取反校验（1Byte）+结束符0x88（1Byte）。	
*4. 主从指令：	
*0x01:主设备	0x02：从设备	
************************************************************************************************/
#define    SPEECH_ID_MASTER              0x01
#define    SPEECH_ID_SLAVE               0x02
#define    SPEECH_DATA_HEAD             0x6B
#define    SPEECH_DATA_END              0x88
#define    SPEECH_BUFF_LEN              0x06
/*******************************struct define ****************************************************/
typedef union
{
	uint8_t buf[SPEECH_BUFF_LEN];
	struct{
		uint8_t head;
		uint8_t id;
		uint8_t cmd;
		uint8_t data;
		uint8_t checksum;
		uint8_t end;
	}pkt;
}SPEECH_DATA_u;
typedef enum
{
	SPEECH_GET_HEADER_DLE =0,
	SPEECH_GET_ID,
	SPEECH_PAYLOAD,
	SPEECH_CHECKSUM,
	SPEECH_GET_END,
}SPEECH_REV_STEP_e;
typedef struct
{
	SPEECH_REV_STEP_e eRevState;
	uint8_t CmdPos;
	uint8_t DataLen;
	uint8_t PayloadCnt;
	uint8_t CheckSum;
	uint8_t Cnt;
}UARTREV_FORMAT_t;
typedef enum
{
	STATE_SPEECH_CMD_IDLE=0,
	STATE_SPEECH_CMD_PLAY=1,
	STATE_SPEECH_CMD_PAUSE=2,
	STATE_SPEECH_CMD_CANCEL=3,
	STATE_SPEECH_CMD_POWER_OFF=4,
	STATE_SPEECH_CMD_POWER_ON = 5,
	STATE_SPEECH_CMD_DOOR_OPEN = 6
}SPEECH_CMD_STATE;
typedef enum
{
	DIRECT_CMD_NULL = 0,
	DIRECT_CMD_END = 1,
	DIRECT_CMD_VALID = 2
}DIRECTCMD_e;
typedef struct
{
	int CycleID;
	int State;
}SPEECH_DATA_t;
extern SPEECH_DATA_t tSpeech;
extern UARTREV_FORMAT_t tSpeechRev;
extern SPEECH_DATA_u uSpeechTransmit, uSpeechAppRev, uSpeechRev;
/*************************************Func******************************************************/
void Speech_ComInit(void);
void SpeechDirectSend(DIRECTCMD_e index);