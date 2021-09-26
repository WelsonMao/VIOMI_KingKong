#include <assert.h>
#include "../scene.h"
/*global define*/
// need to install VS90SP1-KB980263-x86.exe for vs2008
#pragma execution_character_set("utf-8")
#ifdef _WIN32
#define WIN32_COM           1
#define CFG_UART0_BAUDRATE 115200
#else
#define TFT_UART_NO         ITP_DEVICE_UART1
#endif
#define VTIME_ENABLE

#define TIMEOUT_MS          1   
#define TIMEOUT_RETRY       (20*TIMEOUT_MS)   //ms
#ifndef NULL
#define NULL	(void *)0
#endif



#ifdef VTIME_ENABLE
typedef void(*T_pfVoidFuncVoid)(void);
typedef struct
{
	uint16_t  msec;
	void* pCallback;
} Vtimer_t;
typedef enum
{
	VTIME_FUNC_GET_PARAM = 0
	, VTIME_FUNC_MAX
}VTIME_FUNC_e;
#endif

#define COM_ERR_ID     27
typedef struct
{
	bool ComState;
	uint32_t  wComCnt;
}COM_ERR_t;
extern COM_ERR_t  tComErr;
typedef enum
{
	GET_UART_FRAME_HEADER = 0,
	GET_UART_FRAME_ADDRESS_H,
	GET_UART_FRAME_ADDRESS_L,
	GET_UART_FRAME_LEN,
	GET_UART_FRAME_PAYLOAD,
	GET_UART_FRAME_CHECKSUM,
}UARTREV_STATE_e;
typedef struct
{
	//uint8_t inDataBuf[20];
	uint8_t cmdPos;
	//uint8_t Rev_Num;
	//uint8_t Data_Length;
	//uint8_t payloadLen;
	//uint8_t payloadCount;
	//uint8_t checkSum;
	uint8_t RevBuf[256];
	//UARTREV_STATE_e eGetState;
}UARTREV_t;


extern void UART_ProtocolInit(void);
extern void TimeBaseThreadInit(void);

