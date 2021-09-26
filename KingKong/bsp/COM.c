#include <assert.h>
#include "../scene.h"
#include <sys/ioctl.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "../ctrlboard.h"
#include "ite/itp.h"
#include "ite/ith.h"
#include "COM.h"
#include "../extend/CRC.h"
#include  "uart_process.h"
#include "task_queue.h"
#include "Modbus.h"
#ifndef _WIN32
#include "uart/uart.h"
#endif
#ifdef VTIME_ENABLE
Vtimer_t tVtimer[VTIME_FUNC_MAX];
#endif
static sem_t UartSem;
static uint8_t inDataBuf[256] = { 0 };
static UARTREV_t tUartRev;
COM_ERR_t  tComErr;
/*******************frame format*********************************************/
static void ComUartCallback(void* arg1, uint32_t arg2)
{
	sem_post(&UartSem);
}
/*******************frame format*********************************************/
/****************************************************************************/
void MainBoardUartRev(uint8_t *recvdata, uint8_t readLen)
{
	uint8_t count = 0;
	if (readLen){
		while (readLen--){   
			//printf("cmdPos=%d,val=%d\n", tUartRev.cmdPos, *(recvdata + count));
			tUartRev.RevBuf[tUartRev.cmdPos++] = *(recvdata + count);
			count++;
		}
	}
}
/*******************frame format*********************************************/
void* BSP_UartSendThreadCallback(void* arg)
{
	int ret = -1;
	printf("=======Start BSP_UartSendThreadCallback!=======\n");
	uart_event_msg_t queueEvent;
	uint32_t wSendTickCnt = itpGetTickCount();
	while (1){
#ifdef ENABLE_COM_RETRY
		if ((tComState.ResponseState) && (itpGetTickDuration(tComState.TimeBaseTick) >= TIMEOUT_RETRY)){
			tComState.RetryCnt++;
			if (tComState.RetryCnt <RETRY_MAX_CNT) uartTaskPrevQueueOut();
			else                                   tComState.RetryCnt = 0;
			tComState.ResponseState = 0;	
			tComState.TransmitEnable = true;
			printf("!!!Retry:%d\n", tComState.RetryCnt);
		}
#endif
		//if (tComState.TransmitEnable)// && (itpGetTickDuration(tComState.TimeBaseTick) >= 80)
		usleep(80 * 1000);
		{
			ret = uartTaskQueueOut(&queueEvent);
			if (0 == ret){
				//printf("====TimeBaseTick=%d,ret=%d-%d\n", itpGetTickDuration(tComState.TimeBaseTick), ret, tComState.TransmitEnable);
				ret = -1;
				tComState.TransmitCMD = queueEvent.payloadBuf[FRAME_OFFSET_FRAMETYPE];
				tComState.BaseAddress = (uint16_t)(queueEvent.payloadBuf[FRAME_OFFSET_REGADDRESS_H] << 8 | queueEvent.payloadBuf[FRAME_OFFSET_REGADDRESS_L]);
				tComState.InOutState = 0;
				tComState.ResponseState = 1;
				//tComState.TransmitEnable = false;
				tComState.TimeBaseTick = itpGetTickCount();
#ifdef _WIN32
				//UartSend(WIN32_COM, queueEvent.payloadBuf, queueEvent.payloadLen);
#else
				write(TFT_UART_NO, queueEvent.payloadBuf, queueEvent.payloadLen);
#endif
#if 1
				if (FUN_CODE_10H == tComState.TransmitCMD)
				{
					printf("Send[%d]:{", queueEvent.payloadLen);
					for (int i = 0; i < queueEvent.payloadLen; i++)
						printf("%02X ,", queueEvent.payloadBuf[i]);
					printf("};\n");
				}
#endif
			}
		}
	}
	printf("=======End BSP_UartSendThreadCallback!=======\n");
	return 0;
}
/*******************frame format*********************************************/
void* BSP_UartRevThreadCallback(void* arg)
{
	int len = 0;
	printf("=======Start BSP_UartRevThreadCallback!=======\n");
	sem_init(&UartSem, 0, 0);
	memset(&uRevDev, 0, sizeof(REV_DEV_u));
	memset(&uTransmitDev, 0, sizeof(REV_DEV_u));
	while (1)
	{
#ifdef _WIN32
		len = UartReceive(WIN32_COM, inDataBuf, 256);
#else
		sem_wait(&UartSem);
		len = read(TFT_UART_NO, inDataBuf, 256);
#endif
		if (len>0){
			tComState.InOutState = 1;
			tComState.TimeBaseTick = itpGetTickCount();
			MainBoardUartRev(inDataBuf, len);
			len = 0;
		}
	}
	printf("=======end BSP_UartRevThreadCallback======\n");
	return 0;
}
/*******************frame format*********************************************/
void BSP_UartInit(void)
{
#ifdef _WIN32
	ComInit(WIN32_COM, BAUDRATE);
#else
	UART_OBJ *pComUartInfo = (UART_OBJ*)malloc(sizeof(UART_OBJ));
	pComUartInfo->parity = 0;
	pComUartInfo->port = ITH_UART1;
	pComUartInfo->txPin = CFG_GPIO_UART1_TX;
	pComUartInfo->rxPin = CFG_GPIO_UART1_RX;
	pComUartInfo->baud = BAUDRATE;
	pComUartInfo->timeout = 0;
	pComUartInfo->mode = UART_INTR_MODE;
	pComUartInfo->forDbg = false;
	printf("Start uart Interrupt mode test!\n");

	itpRegisterDevice(TFT_UART_NO, &itpDeviceUart1);

	ioctl(TFT_UART_NO, ITP_IOCTL_INIT, (void*)pComUartInfo);
	ioctl(TFT_UART_NO, ITP_IOCTL_REG_UART_CB, (void*)ComUartCallback);
#endif
}
/*******************frame format*********************************************/
/*******************frame format*********************************************/
void UART_ProtocolInit(void)
{
#if 1//ndef _WIN32
	static  volatile bool  AcitonOnce = false;
	pthread_t MainBoard_RevTask, MainBoard_SendTask;
	pthread_attr_t MainBoardT_RevAttr, MainBoardT_SendAttr;

	if ((!ScreenIsOff()) && (AcitonOnce == false))
	{
		//printf("=======Start UART_ProtocolInit!=======\n");
		BSP_UartInit();
		pthread_attr_init(&MainBoardT_RevAttr);
		pthread_attr_setdetachstate(&MainBoardT_RevAttr, PTHREAD_CREATE_DETACHED);
		pthread_create(&MainBoard_RevTask, &MainBoardT_RevAttr, BSP_UartRevThreadCallback, NULL);
		uartTaskQueueInit();
		pthread_attr_init(&MainBoardT_SendAttr);
		pthread_attr_setdetachstate(&MainBoardT_SendAttr, PTHREAD_CREATE_DETACHED);
		pthread_create(&MainBoard_SendTask, &MainBoardT_SendAttr, BSP_UartSendThreadCallback, NULL);
		AcitonOnce = true;
	}
#endif
}
#ifdef VTIME_ENABLE
void VTime_Set(uint16_t time, VTIME_FUNC_e name, void*pCallback)
{
	tVtimer[name].msec = time;
	tVtimer[name].pCallback = pCallback;
}
void VTimeLoopFunc_GetParam(void)
{   
	ProtocolModBus_Send(GET_DATA, REG_ADDRESS_DEV, REG_ADDRESS_DEV_LEN, NULL);//REG_ADDRESS_TOTAL_LEN REG_ADDRESS_DEV_LEN
	VTime_Set(TIMEOUT_MS * 500, VTIME_FUNC_GET_PARAM, VTimeLoopFunc_GetParam);
#if 0//def _WIN32
	tComState.TransmitEnable = true;
#endif
}
const T_pfVoidFuncVoid VTimeLoopFunc[] =
{
	VTimeLoopFunc_GetParam,
};
void VTimer_Init(void)
{
	for (uint8_t i = 0; i < VTIME_FUNC_MAX; i++)
	{
		tVtimer[i].msec = 0;
		tVtimer[i].pCallback = 0;
	}
}
void VTime_LoopTask(void)
{
	static uint32_t msCnt = 0;
	static bool doOnece = true;
	T_pfVoidFuncVoid pfFunc;
	static bool bSendState = false;
	for (uint8_t i = 0; i < VTIME_FUNC_MAX; i++){
		if (tVtimer[i].msec != 0){
			tVtimer[i].msec--;
			if (tVtimer[i].pCallback != 0){
				if (tVtimer[i].msec == 0){
					//printf("WIN_LoopTask:i=%d\n", i);
					((T_pfVoidFuncVoid)tVtimer[i].pCallback)();
				}
			}
		}
	}
}
#endif
void *TimeBaseThreadCallback(void* arg)
{
	int BaseTime = 1000;//us//*50
	static uint32_t BaseTick = 0;
	static uint16_t Cnt = 0;
	uint8_t exception_code = 0;
	BaseTick = itpGetTickCount();
	VTimer_Init();
	VTimeLoopFunc_GetParam();
	memset(&tComState, 0, sizeof(COM_STATE_t));
	printf("=======Start TimeBaseThreadCallback!=======\n");
	while (1){
		usleep(BaseTime);
		if (tComState.InOutState&&itpGetTickDuration(tComState.TimeBaseTick) >=4){//TIMEOUT_CHAR35 / 1000
			tComState.InOutState = 0;
			tComState.RevEnable = true;
			tComState.TimeBaseTick = itpGetTickCount();
			if (MB_Parse_Data(tUartRev.RevBuf,tUartRev.cmdPos))
			{
				//exception_code = MB_Exception_Check(tPdu);
				/*if (exception_code) MB_ResponseException(tComState.TransmitCMD, exception_code);
				else                RevFrameAnalysis(tUartRev.RevBuf, tUartRev.cmdPos);*/
				RevFrameAnalysis(tUartRev.RevBuf, tUartRev.cmdPos);
			}
			
			tUartRev.cmdPos = 0;
		}
		VTime_LoopTask();
	}
	printf("=======End TimeBaseThreadCallback!=======\n");
	return 0;
}
/*******************frame format*********************************************/
void TimeBaseThreadInit(void)
{
	static  volatile bool  AcitonOnce = false;
	pthread_t VTimetask;
	pthread_attr_t VTimeattr;

	if ((!ScreenIsOff()) && (AcitonOnce == false))
	{
		printf("=======Start TimeBaseThreadInit!=======\n");
		pthread_attr_init(&VTimeattr);
		pthread_attr_setdetachstate(&VTimeattr, PTHREAD_CREATE_DETACHED);
		pthread_create(&VTimetask, &VTimeattr, TimeBaseThreadCallback, NULL);//
		AcitonOnce = true;
	}
}
