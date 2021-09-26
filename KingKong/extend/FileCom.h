#ifndef __FILE_COM_H__
#define __FILE_COM_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*****************************************************************/
#define ENABLE_CONFIG_FILE
#define RESERV_ITEM_MAX                 3
/*****************************************************************/
typedef enum
{
	CONFIG_FILE_RESERV=0,
	CONFIG_FILE_MAX
}CONFIG_TYPE_e;
typedef struct
{
	uint8_t bEnableBit;//1-cnt1 2-cnt2 4-cnt3 0x10-smart
	uint8_t bManualCnt;
	uint8_t bManualTimeSrc[RESERV_ITEM_MAX];
	uint8_t bManualTimeDst[RESERV_ITEM_MAX];
	uint8_t bManualModeID[RESERV_ITEM_MAX];
	uint8_t bManualTemper[RESERV_ITEM_MAX];
	uint8_t bSmartDayCnt;
	uint8_t bSmartTimeSrc;
	uint8_t bSmartTimeDst;
	uint8_t bSmartModeID;
	uint8_t bSmartTemper;
}RESERV_ITEM_t;
extern RESERV_ITEM_t tConfigFileReserv;

/*****************************************************************/
#define RESERV_INFO_CONFIG_PATH   "B://config_file//ReservItem.config"
/*****************************************************************/
#ifdef ENABLE_CONFIG_FILE
void CustomConfigRW(uint8_t index, bool RW);
void CustomConfigDelIndex(uint8_t type, uint8_t index);
void ConfigReservItemSort(void);
void CustomConfigaAdd(uint8_t type, uint8_t modeid, uint8_t timesrc, uint8_t timesdst, uint8_t temper);
#endif
#endif