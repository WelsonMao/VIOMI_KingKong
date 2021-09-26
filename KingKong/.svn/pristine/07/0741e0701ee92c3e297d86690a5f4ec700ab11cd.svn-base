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
#include "CRC.h"
#include "COM.h"
#include "Speech.h"
#include "uart/uart.h"
#include "../mailbox/infoMailBox_Pub.h"
#include "../bsp\UART_DataTrans.h"

UARTREV_FORMAT_t tSpeechRev;
static uint8_t SpeechinDataBuf[256]={0};
SPEECH_DATA_u uSpeechTransmit, uSpeechAppRev, uSpeechRev;
static sem_t SpeechUartSem;
SPEECH_DATA_t tSpeech;
bool Speech_REV(int readLen)
{
	bool isCmdCompleted = false;
	uint8_t count = 0;
	if (readLen)
	{
		while (readLen--)
		{
			//printf("Speech_REV[%#x]:%#x\n", readLen, SpeechinDataBuf[count]);
			switch (tSpeechRev.eRevState)
			{
			case SPEECH_GET_HEADER_DLE:
				memset(uSpeechAppRev.buf, 0, SPEECH_BUFF_LEN);
				memset(&tSpeechRev, 0, sizeof(UARTREV_FORMAT_t));
				if (SPEECH_DATA_HEAD == SpeechinDataBuf[count])
				{
					uSpeechAppRev.buf[tSpeechRev.CmdPos++] = SpeechinDataBuf[count];
					tSpeechRev.eRevState = SPEECH_GET_ID;
				}
				else
				{	
					printf("[SPEECH_GET_HEADER_DLE]=%#x\n", SpeechinDataBuf[count]);
					tSpeechRev.eRevState = SPEECH_GET_HEADER_DLE;
				}
				break;
			case SPEECH_GET_ID:
				if (SPEECH_ID_MASTER == SpeechinDataBuf[count])
				{
					uSpeechAppRev.buf[tSpeechRev.CmdPos++] = SpeechinDataBuf[count];
					tSpeechRev.eRevState = SPEECH_PAYLOAD;
				}
				else
				{
					printf("[SPEECH_GET_ID] Wrong[%#x],inDataBuf=%#x\n", uSpeechAppRev.pkt.id, SpeechinDataBuf[count]);
					tSpeechRev.eRevState = SPEECH_GET_HEADER_DLE;
				}
				break;
			case SPEECH_PAYLOAD:
				if (tSpeechRev.DataLen < 1)
				{
					tSpeechRev.eRevState = SPEECH_PAYLOAD;
					tSpeechRev.DataLen++;
				}
				else
				{
					tSpeechRev.eRevState = SPEECH_CHECKSUM;
					tSpeechRev.DataLen = 0;
				}
				uSpeechAppRev.buf[tSpeechRev.CmdPos++] = SpeechinDataBuf[count];
				break;

			case SPEECH_CHECKSUM:
				tSpeechRev.CheckSum = CRCSumGet(&uSpeechAppRev.buf[0], (SPEECH_BUFF_LEN - 2));
				if (tSpeechRev.CheckSum == SpeechinDataBuf[count])
				{
					// Get one command
					
					uSpeechAppRev.buf[tSpeechRev.CmdPos++] = SpeechinDataBuf[count];
					tSpeechRev.eRevState = SPEECH_GET_END;
				}
				else
				{
					printf("[SPEECH_CHECKSUM] Wrong[%#x],checkSum=0x%x \n", SpeechinDataBuf[count], tSpeechRev.CheckSum);
					tSpeechRev.eRevState = SPEECH_GET_HEADER_DLE;
				}			
				break;
			case SPEECH_GET_END:
				if (SPEECH_DATA_END == SpeechinDataBuf[count]){
					uSpeechAppRev.buf[tSpeechRev.CmdPos++] = SpeechinDataBuf[count];
					tSpeechRev.Cnt = tSpeechRev.CmdPos;
					memcpy(uSpeechRev.buf, uSpeechAppRev.buf, SPEECH_BUFF_LEN);
					isCmdCompleted = true;
#if 1
					printf("Speech RevData={\n");
					for (int i = 0; i < SPEECH_BUFF_LEN; i++){
						printf("%#x ,", uSpeechRev.buf[i]);
					}
					printf("}\n");
#endif
				}
				else{
					printf("[SPEECH_GET_END] Wrong[%#x] \n", SpeechinDataBuf[count]);
				}
				tSpeechRev.eRevState = SPEECH_GET_HEADER_DLE;
				break;
             default:
				 tSpeechRev.eRevState = SPEECH_GET_HEADER_DLE;
					printf("===default\n");
                   break;
			}
			count++;
		}
		return  isCmdCompleted;
	}
}
void SpeechDirectSend(DIRECTCMD_e index)
{
	bool ret = false;
	SPEECH_DATA_u directcmd = {0};
	if (DIRECT_CMD_END == index){
		directcmd.pkt.cmd = 0x35;		
		ret = true;
	}
	else if (DIRECT_CMD_VALID == index){
		directcmd.pkt.cmd = 0x36;
		ret = true;
	}
	if (ret){
		directcmd.pkt.head = SPEECH_DATA_HEAD;
		directcmd.pkt.id = SPEECH_ID_SLAVE;
		directcmd.pkt.data = 0;
		directcmd.pkt.end = SPEECH_DATA_END;
		memset(uSpeechTransmit.buf, 0, SPEECH_BUFF_LEN);
		memcpy(uSpeechTransmit.buf, directcmd.buf, SPEECH_BUFF_LEN);
		uSpeechTransmit.pkt.checksum = CRCSumGet(uSpeechTransmit.buf, (SPEECH_BUFF_LEN - 2));
#ifndef _WIN32
#ifdef CFG_UART2_ENABLE
		write(ITP_DEVICE_UART2, uSpeechTransmit.buf, SPEECH_BUFF_LEN);
#endif
#endif
#ifdef ENABLE_WAKEUP_SENSOR
		wScreenSaverTimOut = itpGetTickCount();
		ScreenSaverRefresh();
		bit_Label_SensorLink = 1;
		if (LAYER_POWEROFF == OperatorParam.LayerID) bit_Label_SpeechLink = 1;
		else                                         bit_Label_SpeechLink = 0;
		printf("SpeechDirectSend:LayerID=%d,index=%d\n", OperatorParam.LayerID, index);
#endif
#if 1//def WIN_DEBUG
		printf("DirectSpeechSend={");
		for (uint8_t i = 0; i < SPEECH_BUFF_LEN; i++)
			printf("%#x,", uSpeechTransmit.buf[i]);
		printf("}\n");
#endif
	}
}
void SpeechSend(SPEECH_DATA_u*buf)
{
	memset(uSpeechTransmit.buf, 0, SPEECH_BUFF_LEN);
	memcpy(uSpeechTransmit.buf, buf, SPEECH_BUFF_LEN);
	uSpeechTransmit.pkt.id = SPEECH_ID_SLAVE;
	if ((0x31 == uSpeechTransmit.pkt.cmd) && (0x1 == uSpeechTransmit.pkt.data))
		uSpeechTransmit.pkt.data=0;
	uSpeechTransmit.pkt.checksum = CRCSumGet(uSpeechTransmit.buf, (SPEECH_BUFF_LEN - 2));
#ifndef _WIN32
#ifdef CFG_UART2_ENABLE
	write(ITP_DEVICE_UART2, uSpeechTransmit.buf, SPEECH_BUFF_LEN);
#endif
#endif
#ifdef ENABLE_WAKEUP_SENSOR
    wScreenSaverTimOut=itpGetTickCount();
    ScreenSaverRefresh();
	bit_Label_SensorLink = 1;
	if (LAYER_POWEROFF == OperatorParam.LayerID) bit_Label_SpeechLink = 1;
	else                                         bit_Label_SpeechLink = 0;
    printf("SpeechSend:LayerID=%d\n",OperatorParam.LayerID);
#endif
#if 1//def WIN_DEBUG
	printf("SpeechRes={");
	for (uint8_t i = 0; i < SPEECH_BUFF_LEN; i++)
		printf("%#x,", uSpeechTransmit.buf[i]);
	printf("}\n");
#endif
}

void SpeechRevProcess(SPEECH_DATA_u*RevBuf)
{
	//printf("%s:cmd=%#x\n", RevBuf->pkt.cmd);
	//miiotCmdMailBoxIn(MIIOT_CMD_on, TRUE);
	//memset(&tSpeech, 0, sizeof(SPEECH_DATA_t));
	bool ret = false;
	tSpeech.CycleID = -1;
	tSpeech.State = -1;
	uint8_t cmdDisable = 0;
	switch (RevBuf->pkt.cmd)
	{
	case 0x01 :
		tSpeech.CycleID = UI_CYCLE_GOLDEN_WASH; 
		break;
	case 0x02:
		tSpeech.CycleID = UI_CYCLE_DRUM_CLEAN;
		break;
	case 0x03:
		tSpeech.CycleID = UI_CYCLE_SPIN; 
		break;
	case 0x04:
		tSpeech.CycleID = UI_CYCLE_SUPER_QUICK; 
		break;
	case 0x05:
		tSpeech.CycleID = UI_CYCLE_COTTON; 
		break;
	case 0x06:
		tSpeech.CycleID = UI_CYCLE_SHIRT; 
		break;
	case 0x07:
		tSpeech.CycleID = UI_CYCLE_WOOL; 
		break;
	case 0x08:
		tSpeech.CycleID = UI_CYCLE_DOWN; 
		break;
	case 0x09:
#ifdef ENABLE_WORKMODE_CTRL
		if (_fFlagByte) tSpeech.CycleID = -1;
		else
#endif
		{
			tSpeech.State = STATE_SPEECH_CMD_PLAY;
			miiotCmdMailBoxIn(MIIOT_CMD_start_wash, true);
		}
		break;
	case 0x0A:
		tSpeech.CycleID = UI_CYCLE_STERILIZE; 
		break;
	case 0x0B:
		tSpeech.CycleID = UI_CYCLE_JEANS; 
		break;
	case 0x0C:
		tSpeech.CycleID = UI_CYCLE_UNDER_WEAR; 
		break;
	case 0x0D:
		tSpeech.CycleID = UI_CYCLE_CHILD; 
		break;
	case 0x0E:
		tSpeech.CycleID = UI_CYCLE_COTTON_PLUS; 
		break;
	case 0x15:
#ifdef ENABLE_WORKMODE_CTRL
		if (_fFlagByte) tSpeech.CycleID = -1;
		else
#endif
		{
			tSpeech.State = STATE_SPEECH_CMD_POWER_ON;
			miiotCmdMailBoxIn(MIIOT_CMD_on, true);
		}

		break;
	case 0x16:
#ifdef ENABLE_WORKMODE_CTRL
		if (_fFlagByte) tSpeech.CycleID = -1;
		else
#endif
		{
			tSpeech.State = STATE_SPEECH_CMD_POWER_OFF;
			miiotCmdMailBoxIn(MIIOT_CMD_on, false);
		}
		break;
	case 0x17:
#ifdef ENABLE_WORKMODE_CTRL
		if (_fFlagByte) tSpeech.CycleID = -1;
		else
#endif
		{
			tSpeech.State = STATE_SPEECH_CMD_CANCEL;
			miiotCmdMailBoxIn(MIIOT_CMD_CancelWash, TRUE);
		}
		break;
	case 0x18:
#ifdef ENABLE_WORKMODE_CTRL
		if (_fFlagByte) tSpeech.CycleID = -1;
		else
#endif
		{
			if (STATE_CYCLE_RUN == OperatorParam.CycleState){
				if (GetACUStep() != DIALOG_DRY_INDEX){
					tSpeech.State = STATE_SPEECH_CMD_PAUSE;
					miiotCmdMailBoxIn(MIIOT_CMD_pause, true);
				}
				else
					cmdDisable = 1;
			}
			else
				cmdDisable = 1;
		}
		break;
	case 0x20:
		tSpeech.CycleID = UI_CYCLE_RINSE_SPIN; 
		break;
	/*case 0x24:
		tSpeech.CycleID = UI_CYCLE_GOLDEN_DRY;
		break;*/
	case 0x25:
		tSpeech.CycleID = UI_CYCLE_AIR_CARE; 
		break;
	case 0x26:
		tSpeech.CycleID = UI_CYCLE_BIG; 
		break;
	case 0x27:
		tSpeech.CycleID = UI_CYCLE_WASH_DRY; 
		break;
	case 0x2D:
#ifdef ENABLE_WORKMODE_CTRL
		if (_fFlagByte) tSpeech.CycleID = -1;
		else
#endif
		{
			tSpeech.State = STATE_SPEECH_CMD_DOOR_OPEN;
			miiotCmdMailBoxIn(MIIOT_CMD_door, true);
		}
		break;
	/*case 0x2E:
		tSpeech.CycleID = UI_CYCLE_COTTON_DRY;
		break;*/
	case 0x2F:
		tSpeech.CycleID = UI_CYCLE_DOWN_DRY; 
		break;
	case 0x30:
		tSpeech.CycleID = UI_CYCLE_STERILIZE_HOT; 
		break;
	case 0x32:
		tSpeech.CycleID = UI_CYCLE_WARM; 
		break;
	case 0x33:
		tSpeech.CycleID = UI_CYCLE_GOLDEN_DRY;
		break;
	case 0x34:
		tSpeech.CycleID = UI_CYCLE_COTTON_DRY;
		break;
	default:
		printf("%s:Default cmd=%#x\n", RevBuf->pkt.cmd);
		//cmdDisable = 1;
		break;
	}
	printf("+++++CycleID=%d,CycleState=%d\n", tSpeech.CycleID, OperatorParam.CycleState);
	if (!cmdDisable){
#ifdef ENABLE_WORKMODE_CTRL
		if ((tSpeech.CycleID >= 0) && ((OperatorParam.CycleState != STATE_IDLE) || _fFlagByte))
#else
		if ((tSpeech.CycleID >= 0) && ((OperatorParam.CycleState != STATE_IDLE)))
#endif
		{
			tSpeech.CycleID = -1;
#ifdef ENABLE_SPEECH_NEWCMD
			SpeechDirectSend(DIRECT_CMD_VALID);
#endif
		}
		else
			SpeechSend(RevBuf);
		if (tSpeech.CycleID >= 0) miiotCmdMailBoxIn(MIIOT_CMD_mode_speech, tSpeech.CycleID);
	}
	else{
#ifdef ENABLE_SPEECH_NEWCMD
		SpeechDirectSend(DIRECT_CMD_VALID);
#endif
	}
}
static void SpeechUartCallback(void* arg1, uint32_t arg2)
{
	sem_post(&SpeechUartSem);
}
void Speech_UartInit(void)
{
	#ifdef _WIN32
		ComInit(ITP_DEVICE_UART2, 9600);
	#else
	UART_OBJ *pSpeechUartInfo	= (UART_OBJ*)malloc(sizeof(UART_OBJ));
	pSpeechUartInfo->parity	= 0;
	pSpeechUartInfo->port = ITH_UART2;
	pSpeechUartInfo->txPin = CFG_GPIO_UART2_TX;
	pSpeechUartInfo->rxPin = CFG_GPIO_UART2_RX;
	pSpeechUartInfo->baud = CFG_UART2_BAUDRATE;
	pSpeechUartInfo->timeout = 0;
	pSpeechUartInfo->mode = UART_INTR_MODE;
	pSpeechUartInfo->forDbg = false;

	itpRegisterDevice(ITP_DEVICE_UART2, &itpDeviceUart2);
	ioctl(ITP_DEVICE_UART2, ITP_IOCTL_INIT, (void*)pSpeechUartInfo);
	ioctl(ITP_DEVICE_UART2, ITP_IOCTL_REG_UART_CB, (void*)SpeechUartCallback);
	#endif
}
void* Speech_UartFunc(void* arg)
{
	int len = 0;
	printf("=======Start Speech_UartFunc!=======\n");
	Speech_UartInit();
	sem_init(&SpeechUartSem, 0, 0);
	while (1)
	{
		sem_wait(&SpeechUartSem);
		#ifdef CFG_UART2_ENABLE
		len = read(ITP_DEVICE_UART2, SpeechinDataBuf, 256);;
		#endif
		if (tConfig.Speech_Enable){//tConfig.Speech_Enable
			if (Speech_REV(len)){
				SpeechRevProcess(uSpeechRev.buf);
				tSpeechRev.Cnt = 0;
			}
		}
		else
			tSpeechRev.Cnt = 0;
	}
	printf("=======end Speech_UartFunc======\n");
	return 0;
}
void Speech_ComInit(void)
{
#ifndef _WIN32
    static  volatile bool  AcitonOnce=false;
	pthread_t SpeechTask;
	pthread_attr_t SpeechAttr;
	
    if((!ScreenIsOff())&&(AcitonOnce==false))
    {
		printf("=======Start Speech_ComInit!=======\n");
		pthread_attr_init(&SpeechAttr);
		pthread_create(&SpeechTask, &SpeechAttr, Speech_UartFunc, NULL);
		AcitonOnce=true;
    }
#endif
}