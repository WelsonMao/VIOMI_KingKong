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
#include "Version.h"

#include <stdarg.h>
#include <pthread.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "iic/mmp_iic.h"


TP_VER_t tTPVersion;
/*******************************************
w 24 04 00 05 00 2f 00 03 p  停止扫描
r 24 x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x p
w 24 04 00 05 00 2f 00 02 p读
w 24 04 00 05 00 2f 00 02 p 读取Firmware信息
w 24 04 00 05 00 2f 00 04 p 恢复扫描
R 33+ 00+ 1F+ 00+ 02+ 01+ 02+ 0A+ 00+ 02+ 02+ 00+ 00+ 00+ 00+ 05+ 8F+ 02+ 00+ 9C+ 13+ 13+ 0C+ 12+ F9+ 4A+ 05+ 5B+ 0E+ 05+ 00+ 1E+ 03+ 1A+ 08+ 40+ 4A+ 34+ 17+ 00+ 05+ 90+ 01+ FF+ 00+ 02+ 01+ 00+ 00+ 03+ 01
byte[15][16]
**********************************************/
void TP_SendDelay(int ms)
{
	usleep(ms * 1000);
}
int i2c_read_interface(uint8_t*w_ptr, uint8_t w_len, uint8_t*r_ptr, uint8_t r_len)
{
	int result;
	TP_SendDelay(10);
	result = mmpIicReceiveData(IIC_PORT_1, IIC_MASTER_MODE, TP_I2C_KEY_SLAVER_ADDR, w_ptr, w_len, r_ptr, r_len);
#if 0
	printf("\n");
	printf("i2c read str:[%d]", result);
	for (int i = 0; i < r_len; i++)
	printf("%02X ,", *(r_ptr+i));
	printf("\n");
#endif
	return result;
}
int i2c_write_interface(uint8_t*ptr, uint8_t len)
{
	int result;
	TP_SendDelay(10);
	result = mmpIicSendDataEx(IIC_PORT_1, IIC_MASTER_MODE, TP_I2C_KEY_SLAVER_ADDR, ptr, len);
#if 0
	printf("\n");
	printf("i2c write str[%d]:", result);
	for (int i = 0; i < len; i++)
	printf("%02X ,", *(ptr+i));
	printf("\n");
#endif
	return result;
}
bool TPStepRetryCheck(TP_VER_t *tVer, uint8_t retry, TP_STEP_e StepTo)
{
	bool ret = false;
	if (retry >= TP_RETRY_CNT){
		printf("++++++[%s] ERR!Step Change to[%d]->[%d]\n", STRLOG(tVer->step), tTPVersion.step, StepTo);
		tVer->step = StepTo;
		return true;
	}
	return ret;
}
bool GetTPVersion(void)
{
	bool ret = false;
	int result = -1;
	uint8_t RevBuf[51] = { 0 };
	uint8_t bTPWriteBuf[TP_CMD_LEN] = { 0 };
	uint8_t len = 0;
	uint8_t RetryCnt = 0;
	/*****************************/
	//w 24 04 00 05 00 2f 00 03 p  停止扫描
	memset(bTPWriteBuf, 0, TP_CMD_LEN);
	bTPWriteBuf[0] = 0x04;
	bTPWriteBuf[1] = 0x00;
	bTPWriteBuf[2] = 0x05;
	bTPWriteBuf[3] = 0x00;
	bTPWriteBuf[4] = 0x2f;
	bTPWriteBuf[5] = 0x00;
	bTPWriteBuf[6] = 0x03;
	len = 7;
	/*****************************/
	while (tTPVersion.step)
	{
		switch (tTPVersion.step){
		case TP_STEP_RESUME_DLY:
			TP_SendDelay(1000);
			tTPVersion.step = TP_STEP_START;
			break;
		case TP_STEP_START:
			result = i2c_write_interface(bTPWriteBuf, len);
			if (result >= 0){
				//RetryCnt = 0;
				tTPVersion.step = TP_STEP_START_ACK;
			}
			else{
				RetryCnt++;
				printf("++++++[%s] RetryCnt=%d!\n", STRLOG(TP_STEP_START), RetryCnt);
			}
			if (TPStepRetryCheck(&tTPVersion, RetryCnt, TP_STEP_DEFAULT)){
				RetryCnt = 0;
				return -1;
			}
			break;
		case TP_STEP_START_ACK:
			result = i2c_read_interface(bTPWriteBuf, 0, RevBuf, 10);
			if ((result >= 0) && (0x1f == RevBuf[2]) && (0x3==(RevBuf[4] & 0x7f))){
				RetryCnt = 0;
				tTPVersion.step = TP_STEP_READ_VER;
			}
			else{
				RetryCnt++;
				tTPVersion.step = TP_STEP_START;
				printf("++++++[%s] RetryCnt=%d!\n", STRLOG(TP_STEP_START_ACK), RetryCnt);
			}
			if (TPStepRetryCheck(&tTPVersion, RetryCnt, TP_STEP_START)){
				RetryCnt = 0;
			}
			break;
		case TP_STEP_READ_VER:
			/*****************************/
			//w 24 04 00 05 00 2f 00 02 p 读取Firmware信息
			//w 24 04 00 05 00 2f 00 04 p 恢复扫描
			bTPWriteBuf[6] = 0x02;
			result = i2c_write_interface(bTPWriteBuf, len);
			if (result >= 0){
				RetryCnt = 0;
				tTPVersion.step = TP_STEP_READ_VER_ACK;
			}
			else
			{
				RetryCnt++;
				printf("++++++[%s] RetryCnt=%d!\n", STRLOG(TP_STEP_READ_VER), RetryCnt);
			}
			if (TPStepRetryCheck(&tTPVersion, RetryCnt, TP_STEP_RESUME_SCAN)){
				RetryCnt = 0;
			}
			break;
		case TP_STEP_READ_VER_ACK:
			result = i2c_read_interface(bTPWriteBuf, 0, RevBuf, 51);
			if ((result >= 0) && (RevBuf[2] == 0x1f) && ((RevBuf[4] & 0x7f) == 0x2)) {
				RetryCnt = 0;
				tTPVersion.step = TP_STEP_RESUME_SCAN;
				tTPVersion.TPVer = (uint16_t)(RevBuf[16] << 8 | RevBuf[15] << 0);
			}
			else
			{
				RetryCnt++;
				printf("++++++[%s] RetryCnt=%d!\n", STRLOG(TP_STEP_READ_VER_ACK), RetryCnt);
			}
			if (TPStepRetryCheck(&tTPVersion, RetryCnt, TP_STEP_RESUME_SCAN)){
				RetryCnt = 0;
			}
			break;
		case TP_STEP_RESUME_SCAN:
			bTPWriteBuf[6] = 0x04;
			result = i2c_write_interface(bTPWriteBuf, len);
			if (result >= 0){
				RetryCnt = 0;
				tTPVersion.step = TP_STEP_RESUME_SCAN_ACK;
			}
			else{
				RetryCnt++;
				printf("++++++[%s] RetryCnt=%d!\n", STRLOG(TP_STEP_RESUME_SCAN), RetryCnt);
			}
			if (TPStepRetryCheck(&tTPVersion, RetryCnt, TP_STEP_DEFAULT)){
				RetryCnt = 0;
				return -1;
			}
			break;
		case TP_STEP_RESUME_SCAN_ACK:
			result = i2c_read_interface(bTPWriteBuf, 0, RevBuf, 10);
			if ((result >= 0) && (RevBuf[2] == 0x1f) && ((RevBuf[4] & 0x7f) == 0x4)){
				return 0;
			}
			else{
				RetryCnt++;
				printf("++++++[%s] RetryCnt=%d!\n", STRLOG(TP_STEP_RESUME_SCAN_ACK), RetryCnt);
			}
			if (TPStepRetryCheck(&tTPVersion, RetryCnt, TP_STEP_DEFAULT)){
				RetryCnt = 0;
				return -1;
			}
			break;
		default:
			break;
		}
	}
	return ret;
}
void TouchResetPin(void)
{
	printf("=======start Reset tp======\n");
	ithGpioSetMode(CFG_GPIO_TOUCH_RESET, ITH_GPIO_MODE0);//welson add Power_Standby
	ithGpioSetOut(CFG_GPIO_TOUCH_RESET);
	ithGpioClear(CFG_GPIO_TOUCH_RESET);
	ithGpioEnable(CFG_GPIO_TOUCH_RESET);
	TP_SendDelay(50);
	ithGpioSetMode(CFG_GPIO_TOUCH_RESET, ITH_GPIO_MODE0);//welson add Power_Standby
	ithGpioSetOut(CFG_GPIO_TOUCH_RESET);
	ithGpioSet(CFG_GPIO_TOUCH_RESET);
	ithGpioEnable(CFG_GPIO_TOUCH_RESET);
	printf("=======end Reset tp======\n");
}
void *GetTPVersionFunc(void*arg)
{
	bool ret = false;
	uint32_t TPVerTimeOut = 0;
	uint8_t RevBuf[16] = { 0 };
	uint8_t bTPWriteBuf[TP_CMD_LEN] = { 0 };
	TPVerTimeOut = itpGetTickCount();
	tTPVersion.step = TP_STEP_RESUME_DLY;
	printf("=======start GetTPVersionFunc======\n");
	pthread_detach(pthread_self());
	i2c_read_interface(bTPWriteBuf, 0, RevBuf, 7);
	ret=GetTPVersion();
	if (ret>=0) printf("=======end GetTPVersionFunc:[%#x][%d]======\n", tTPVersion.TPVer, itpGetTickDuration(TPVerTimeOut));
	else{
		TouchResetPin();
		printf("=======end GetTPVersionFunc[NULL][%d]======\n", itpGetTickDuration(TPVerTimeOut));
	}
}
void TPVersionTaskInit(void)
{
#ifndef _WIN32
	pthread_t TPVerTask;
	pthread_attr_t TPVer_attr;

	pthread_attr_init(&TPVer_attr);
	pthread_create(&TPVerTask, &TPVer_attr, GetTPVersionFunc, NULL);
#endif
}