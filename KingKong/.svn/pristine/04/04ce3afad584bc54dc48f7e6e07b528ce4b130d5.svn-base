#include <assert.h>
#include "../scene.h"
#include "FileCom.h"
#include "ite/itp.h"
#pragma execution_character_set("utf-8")

RESERV_ITEM_t tConfigFileReserv;
#ifdef ENABLE_CONFIG_FILE
/*******************************************************************************/
int sys_read_configFile(uint8_t * pathname, uint8_t *config, uint32_t file_len)
{
	size_t readlen = 0;
	size_t sumlen = 0;
	if (NULL == pathname || NULL == config){
		printf("[%s] argv null!\n", __FUNCTION__);
		return -1;
	}
	FILE *file = fopen(pathname, "rb");
	if (file == NULL){
		printf("[%s] file open failed!\n", __FUNCTION__);
		return (-1);
	}
	else{
		readlen = 0;
		sumlen = 0;
		do{
			readlen = fread(config + sumlen, 1, (file_len - sumlen), file);
			sumlen += readlen;
		} while (sumlen < file_len || readlen);
	}
	fclose(file);
	return 0;
}
/*******************************************************************************/
int sys_write_configFile(uint8_t * pathname, uint8_t *config, uint32_t file_len)
{
	size_t writelen = 0;
	size_t sumlen = 0;
	if (NULL == pathname || NULL == config){
		printf("[%s] argv null!\n", __FUNCTION__);
		return -1;
	}
	FILE *file = fopen(pathname, "wb");
	if (file == NULL){
		printf("[%s] file open failed!\n", __FUNCTION__);
		return (-1);
	}
	else{
		writelen = 0;
		sumlen = 0;

		do{
			writelen = fwrite(config + sumlen, 1, (file_len - sumlen), file);
			sumlen += writelen;
		} while (sumlen < file_len);
	}
	fclose(file);
	return 0;
}
/*******************************************************************************/
void ConfigFileInit(uint8_t index)
{
	if (CONFIG_FILE_RESERV == index){
		memset(&tConfigFileReserv, 0, sizeof(RESERV_ITEM_t));
		tConfigFileReserv.bManualCnt = 1;
		tConfigFileReserv.bManualModeID[0] = 1;
		tConfigFileReserv.bManualTemper[0] = atoi(tCycleParamStr[tConfigFileReserv.bManualModeID[0]].Temper);
		tConfigFileReserv.bManualTimeSrc[0] = 9;
		tConfigFileReserv.bManualTimeDst[0] = 13;
		tConfigFileReserv.bSmartDayCnt = 0;
		tConfigFileReserv.bSmartModeID = 1;
		tConfigFileReserv.bSmartTemper = atoi(tCycleParamStr[tConfigFileReserv.bSmartModeID].Temper);
		tConfigFileReserv.bSmartTimeSrc = 6;
		tConfigFileReserv.bSmartTimeDst = 12;
	}
	printf("%s:%d\n", __func__, index);
}
void CustomConfigRW(uint8_t index, bool RW)
{
	int ret = -1;
	switch (index)
	{
	case CONFIG_FILE_RESERV:
		if (RW) ret = sys_read_configFile(RESERV_INFO_CONFIG_PATH, &tConfigFileReserv, sizeof(RESERV_ITEM_t));
		else    ret = sys_write_configFile(RESERV_INFO_CONFIG_PATH, &tConfigFileReserv, sizeof(RESERV_ITEM_t));
		if (ret == -1){
			printf("[%s][%d] failed!\r\n", RESERV_INFO_CONFIG_PATH, RW);
			ConfigFileInit(CONFIG_FILE_RESERV);
			CustomConfigRW(CONFIG_FILE_RESERV, 0);
		}
		else		   printf("[%s][%d] sucessed!\r\n", RESERV_INFO_CONFIG_PATH, RW);
#if 1
		if (ret != -1){
			/**********************************************************************/
			printf("Reserv:ManualCnt=%d,bEnableBit=%d\n", tConfigFileReserv.bManualCnt, tConfigFileReserv.bEnableBit);
			for (int i = 0; i < tConfigFileReserv.bManualCnt; i++){
				printf("[%d:%d][%d->%d,%d,%d] ,", i, tConfigFileReserv.bEnableBit&(1<<i), tConfigFileReserv.bManualTimeSrc[i], tConfigFileReserv.bManualTimeDst[i], tConfigFileReserv.bManualModeID[i], tConfigFileReserv.bManualTemper[i]);
				printf("\n");
			}
			printf("ReservSmart[%d]:day=%d,time=[%d->%d],mode=%d,temper=%d\n", tConfigFileReserv.bEnableBit&0x08, tConfigFileReserv.bSmartDayCnt, \
				tConfigFileReserv.bSmartTimeSrc, tConfigFileReserv.bSmartTimeDst, tConfigFileReserv.bSmartModeID, tConfigFileReserv.bSmartTemper);
			/**********************************************************************/
		}
#endif
		break;
	default:
		break;
	}
} 
void CustomConfigDelIndex(uint8_t type, uint8_t index)
{
	int i = 0;
	/**********************************************************************/
	printf("ReservDel:ManualCnt=%d\n", tConfigFileReserv.bManualCnt);
	for (int i = 0; i < RESERV_ITEM_MAX; i++){
		printf("[%d:%d][%d->%d,%d,%d] ,", i, tConfigFileReserv.bEnableBit&(1 << i), tConfigFileReserv.bManualTimeSrc[i], tConfigFileReserv.bManualTimeDst[i], tConfigFileReserv.bManualModeID[i], tConfigFileReserv.bManualTemper[i]);
		printf("\n");
	}
	if (CONFIG_FILE_RESERV == type){
		if (tConfigFileReserv.bManualCnt > 1){
			for (i = index; i < tConfigFileReserv.bManualCnt-1; i++){
				tConfigFileReserv.bManualModeID[i] = tConfigFileReserv.bManualModeID[i + 1];
				tConfigFileReserv.bManualTemper[i] = tConfigFileReserv.bManualTemper[i + 1];
				tConfigFileReserv.bManualTimeSrc[i] = tConfigFileReserv.bManualTimeSrc[i + 1];
				tConfigFileReserv.bManualTimeDst[i] = tConfigFileReserv.bManualTimeDst[i + 1];
				if (tConfigFileReserv.bEnableBit & 1 << (i + 1)) tConfigFileReserv.bEnableBit |= 1 << i;
				else                                             tConfigFileReserv.bEnableBit &= ~(1 << i);
			}
			tConfigFileReserv.bManualModeID[i] = tConfigFileReserv.bManualTemper[i] = 0;
			tConfigFileReserv.bManualTimeSrc[i] = tConfigFileReserv.bManualTimeDst[i] = 0;
			tConfigFileReserv.bEnableBit &= ~(1 << i);
			tConfigFileReserv.bManualCnt -= 1;
		}
		else{
			memset(tConfigFileReserv.bManualModeID, 0, RESERV_ITEM_MAX);
			memset(tConfigFileReserv.bManualTemper, 0, RESERV_ITEM_MAX);
			memset(tConfigFileReserv.bManualTimeSrc, 0, RESERV_ITEM_MAX);
			memset(tConfigFileReserv.bManualTimeDst, 0, RESERV_ITEM_MAX);
			tConfigFileReserv.bEnableBit &= 0xF0;
			tConfigFileReserv.bManualCnt = 0;
		}
		printf("ReservDel:ManualCnt=%d\n", tConfigFileReserv.bManualCnt);
		for (int i = 0; i < RESERV_ITEM_MAX; i++){
			printf("[%d:%d][%d->%d,%d,%d] ,", i, tConfigFileReserv.bEnableBit&(1 << i), tConfigFileReserv.bManualTimeSrc[i], tConfigFileReserv.bManualTimeDst[i], tConfigFileReserv.bManualModeID[i], tConfigFileReserv.bManualTemper[i]);
			printf("\n");
		}
		CustomConfigRW(CONFIG_FILE_RESERV, 0);
	}

}

void ConfigReservItemSort(void)
{
	uint8_t tempsrc,tempdst= 0,bit=0;
	uint8_t id, temper;
	int i, j = 0;
#if 1
	printf("Before Sort:Cnt=%d,Enable=%d\n", tConfigFileReserv.bManualCnt, tConfigFileReserv.bEnableBit);
	for (i = 0; i < tConfigFileReserv.bManualCnt; i++) printf("%d ,", tConfigFileReserv.bManualTimeSrc[i]);
	printf("\n");
#endif
	for (i = 0; i < tConfigFileReserv.bManualCnt - 1; i++)
	{
		for (j = 1; j < tConfigFileReserv.bManualCnt; j++)
		{
			if (tConfigFileReserv.bManualTimeSrc[j] < tConfigFileReserv.bManualTimeSrc[j - 1])
			{
				tempsrc = tConfigFileReserv.bManualTimeSrc[j];
				tempdst = tConfigFileReserv.bManualTimeDst[j];
				id = tConfigFileReserv.bManualModeID[j];
				temper = tConfigFileReserv.bManualTemper[j];
				bit = tConfigFileReserv.bEnableBit&(1 << j);

				tConfigFileReserv.bManualTimeSrc[j] = tConfigFileReserv.bManualTimeSrc[j - 1];
				tConfigFileReserv.bManualTimeDst[j] = tConfigFileReserv.bManualTimeDst[j - 1];
				tConfigFileReserv.bManualModeID[j] = tConfigFileReserv.bManualModeID[j-1];
				tConfigFileReserv.bManualTemper[j] = tConfigFileReserv.bManualTemper[j-1];
				if (tConfigFileReserv.bEnableBit&(1 << (j - 1))) tConfigFileReserv.bEnableBit |= (1 << j);
				else                                             tConfigFileReserv.bEnableBit &= ~(1 << j);

				tConfigFileReserv.bManualTimeSrc[j - 1] = tempsrc;
				tConfigFileReserv.bManualTimeDst[j - 1] = tempdst;
				tConfigFileReserv.bManualModeID[j - 1] = id;
				tConfigFileReserv.bManualTemper[j - 1] = temper;
				if (bit) tConfigFileReserv.bEnableBit |= (1 << (j - 1));
				else     tConfigFileReserv.bEnableBit &= ~(1 << (j - 1));
			}
		}
	}

#if 1
	printf("After Sort:Cnt=%d,Enable=%d\n", tConfigFileReserv.bManualCnt, tConfigFileReserv.bEnableBit);
	for (i = 0; i < tConfigFileReserv.bManualCnt; i++) printf("%d ,", tConfigFileReserv.bManualTimeSrc[i]);
	printf("\n");
#endif	
}
void CustomConfigaAdd(uint8_t type, uint8_t modeid, uint8_t timesrc, uint8_t timesdst, uint8_t temper)
{
	tConfigFileReserv.bManualModeID[tConfigFileReserv.bManualCnt] = modeid;
	tConfigFileReserv.bManualTemper[tConfigFileReserv.bManualCnt] = temper;
	tConfigFileReserv.bManualTimeSrc[tConfigFileReserv.bManualCnt] = timesrc;
	tConfigFileReserv.bManualTimeDst[tConfigFileReserv.bManualCnt] = timesdst;
	tConfigFileReserv.bManualCnt++;
	ConfigReservItemSort();
	CustomConfigRW(CONFIG_FILE_RESERV, 0);
}
/*******************************************************************************/
#endif