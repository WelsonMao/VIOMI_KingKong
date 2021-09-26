#include "uart_process.h"
#include "task_queue.h"
#include "Modbus.h"
#include "ite/itp.h"
#include "ite/ith.h"
#include "../extend/CRC.h"
REV_DEVINFO_u uRevDevInfo;
REV_DEV_u   uTransmitDev, uRevDev;
REV_DELAY_u uRevDelay;
COM_STATE_t tComState = {
	.TransmitCMD = 0,
	.TimeBaseTick = 0,
	.ResponseState = 0,
	.RetryCnt = 0,
	.InOutState = 0,
	.TransmitEnable = true,
	.RevEnable = false,
	.BaseAddress =0
};
/*************************************************************************************************************/
uint32_t ACUGet_Fault(void)
{
	uint32_t bRet = 0;
	bRet = (uint32_t)((uRevDev.pkt.bFault[0] << 24) | \
					  (uRevDev.pkt.bFault[1]) << 16 | \
					   (uRevDev.pkt.bFault[2] << 8) | \
					  (uRevDev.pkt.bFault[3] << 0));
	return bRet;
}
uint32_t ACUGet_TargetTemper(void)
{
	uint32_t bRet = 0;
#if 1
	bRet=uRevDev.pkt.bTargetTemper;
#else
	bRet = (uint32_t)((uRevDev.pkt.bTargetTemper[0] << 24) | \
					  (uRevDev.pkt.bTargetTemper[1]) << 16 | \
					   (uRevDev.pkt.bTargetTemper[2] << 8) | \
					  (uRevDev.pkt.bTargetTemper[3] << 0));
	bRet = (uint32_t)Hex_To_Decimal(uRevDev.pkt.bTargetTemper, 4);
#endif
	return bRet;
}
uint32_t ACUGet_Temper(void)
{
	uint32_t bRet = 0;
#if 1
	bRet = uRevDev.pkt.bTemper;
#else
	bRet = (uint32_t)((uRevDev.pkt.bTemper[0] << 24)| \
		              (uRevDev.pkt.bTemper[1])<< 16 | \
					  (uRevDev.pkt.bTemper[2] << 8) | \
					  (uRevDev.pkt.bTemper[3] << 0));
	bRet = (uint32_t)Hex_To_Decimal(uRevDev.pkt.bTemper, 4);
#endif
	return bRet;
}
uint8_t ACUGet_Mode(void)
{
	uint8_t bRet = 0;
	bRet = uRevDev.pkt.bMode;
	return bRet;
}
uint8_t ACUGet_PowerStatus(void)
{
	uint8_t bRet = 0;
	bRet = uRevDev.pkt.bPowerOnOff;
	return bRet;
}
uint8_t ACUGet_DevStatus(void)
{
	uint8_t bRet = 0;
	bRet = uRevDev.pkt.bStatus;
	return bRet;
}
uint8_t ACUGet_WiFiStatus(void)
{
	uint8_t bRet = 0;
	bRet = uRevDev.pkt.bWiFiStatus;
	return bRet;
}
uint8_t ACUGet_SmartStatus(void)
{
	uint8_t bRet = 0;
	bRet = uRevDev.pkt.bBacterStatus;
	return bRet;
}
uint8_t ACUGet_SmartRemainTime(void)
{
	uint8_t bRet = 0;
	bRet = uRevDev.pkt.bBacterRemainTime;
	return bRet;
}
bool ACUChk_MCUUpgrade(void)
{
	bool bRet = false;
	if (uRevDev.pkt.bUpgrade & 0xF0) bRet = true;
	return bRet;
}
uint8_t ACUGet_UpgradeStep(void)
{
	uint8_t bRet = 0;
	bRet = uRevDev.pkt.bUpgrade&0x7F;
	return bRet;
}

/*************************************************************************************************************/
void TransmitQueueCtrl(COM_STATE_t *com,uint8_t cmd)
{
    //printf("TransmitQueueCtrl:cmd=%x->%x,%d\n",cmd,com->TransmitCMD,itpGetTickDuration(tComState.TimeBaseTick));
	if ((com->TransmitCMD) && (cmd == com->TransmitCMD)){
		com->ResponseState = 0;
		com->TransmitCMD = 0;
		com->TransmitEnable = 1;
	}
}
/*************************************************************************************************************/
void RevFrameAnalysis(uint8_t *buf,uint8_t len)
{
	uint8_t RevCmdType = *(buf + FRAME_OFFSET_FRAMETYPE);
	uint16_t offset = 0,size=0;
	int i=0;
	switch(RevCmdType){
	case FUN_CODE_04H:      //状态帧
		offset = tComState.BaseAddress;
		size = *(buf + FRAME_OFFSET_BYTECNT);
		for (i = 0; i < size; i++){
			if ((offset >= REG_ADDRESS_DEVINFO) && (offset <= REG_ADDRESS_DEVINFO_END)) uRevDevInfo.buf[offset - REG_ADDRESS_DEVINFO + i] = *(buf + FRAME_OFFSET_PAYLOAD + i);
			else if ((offset >= REG_ADDRESS_DEV) && (offset <= REG_ADDRESS_DEV_END))    uRevDev.buf[offset - REG_ADDRESS_DEV + i] = *(buf + FRAME_OFFSET_PAYLOAD + i);
			else if ((offset >= REG_ADDRESS_DELAY) && (offset <= REG_ADDRESS_DELAY_END))uRevDelay.buf[offset - REG_ADDRESS_DELAY + i] = *(buf + FRAME_OFFSET_PAYLOAD + i);
		}
		TransmitQueueCtrl(&tComState, RevCmdType);
		break;
	case FUN_CODE_01H:
	case FUN_CODE_02H:
	case FUN_CODE_03H:
	case FUN_CODE_05H:
	case FUN_CODE_06H:
	case FUN_CODE_10H:   
		TransmitQueueCtrl(&tComState, RevCmdType);
		//printf("<-RevFrameAnalysis:FuncID=%#x\n", RevCmdType);
		break;
	default:
		printf("<-RevFrameAnalysis:[Default]FuncID=%#x\n", RevCmdType);
		//uartTaskPrevQueueOut();
		break;
	}
#if 0
	printf("REV[%d]:{", len);
	for (i = 0; i < len; i++) printf("%02X,", buf[i]);
	printf("};\n");
#endif
}
/*************************************************************************************************************/

