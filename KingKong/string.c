#include <stdio.h>
#include <string.h>
#include "ite/ith.h"
#include "ctrlboard.h"

// need to install VS90SP1-KB980263-x86.exe for vs2008
#pragma execution_character_set("utf-8")

CYCLE_PARAM_LIST_t tCycleParamStr[] = {
	{ "瞬热洗","75℃", "单胆", "5KW" },//
	{ "单人洗","75℃", "单胆", "3KW" },//
	{ "多人洗","75℃", "双胆", "5KW" },//
	{ "节能洗","45℃", "双胆", "3KW" },//
	//{ "智能杀菌","75℃", "双胆", "3KW" },//
	{ "自定义","50℃", "双胆", "5KW" }//
};
const char*WeekTab[] = { "", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六", "星期日" };
const char*RunStatusStr[] = { "已关机", "加热中", "保温中", "用水中" };
ERROR_LIST_t  tUIErrorLsit[] =
{
	/*0-19*/
	{ 0, 0, "", "", "" },
	/*1-10*/
	{ 1, 1, "E2", "干烧故障",       "检查内胆是否注满水，断电冷却后，重新注水，如不能消除，请联系售后上门维修。" },
	{ 1, 1, "E3", "温度传感器故障", "请联系售后上门维修" },
	{ 1, 1, "E4", "超温故障",       "请联系售后上门维修" },
	{ 0, 1, "/",  "电子镁棒故障",   "请联系售后上门维修" },
	/*27*/
	{ 0, 1, "0060", "主控通讯异常" }
};
const IO_PARAM_MAP TransParamMapTable[] = {
	/********************************************************
	IO_CMD_EVENT_e io_cmd;
	uint8_t localparam;
	uint8_t miiotparam;
	********************************************************/
	{ IO_CMD_MODE, 0, 4 },//瞬热洗  //
	{ IO_CMD_MODE, 1, 5 },//单人洗  //
	{ IO_CMD_MODE, 2, 6 },//双人洗  //
	{ IO_CMD_MODE, 3, 8 },//节能洗 
	{ IO_CMD_MODE, 4, 7 },//自定义
	//{ IO_CMD_MODE, 0, 0 },//xx
};
uint16_t mapConvertMiiotAndMachine(CONVERT_TOWARD_e toward, IO_CMD_EVENT_e event, uint16_t data)
{
	uint16_t i, bRet, mapSize;

	bRet = 0;
	mapSize = sizeof(TransParamMapTable) / sizeof(TransParamMapTable[0]);
	for (i = 0; i< mapSize; i++){
		if (event == TransParamMapTable[i].io_cmd){
			if ((CONVERT_MACHINE_2_IOT == toward) && (data == TransParamMapTable[i].localparam)){
				bRet = TransParamMapTable[i].miiotparam;
				break;
			}
			if ((CONVERT_IOT_2_MACHINE == toward) && (data == TransParamMapTable[i].miiotparam)){
				bRet = TransParamMapTable[i].localparam;
				break;
			}
		}
	}
	return bRet;
}