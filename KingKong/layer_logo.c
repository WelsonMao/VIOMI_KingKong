#include <sys/time.h>
#include <assert.h>
#include "ite/itp.h"
#include "scene.h"
#include "extend/FileCom.h"
#include "buzzer/Sound.h"
#include "bsp/COM.h"
#include "bsp/uart_process.h"
#include "bsp/Modbus.h"

static ITULayer*LogoLayer,*MainMenuLayer;
static ITUBackground*LogoLayerBG, *LogoPowerOffShowBG;
static ITUSprite*LogoTimeSprite[4], *LogoTimeDotSprite, *MainMenuBGSprite;
static ITUText*LogoDayText;
OPERATE_t OperatorParam;
RESERV_CONFIG_t tReservConfig;
CONFIG_t tConfig;
CYCLE_PARAM_t tCycleParamTab[UI_CYCLE_MAX];
RESERV_TEMP_ITEM_t tReservTmpItem;
PROCESSVAL_t tProcessVal;
Flag_set _fConfigOption, _fLabel, _fFlagBit,  _fResetBit;

#pragma execution_character_set("utf-8")
bool LogoTimeShow(bool init)
{
	bool ret = false;
	static uint8_t hour, min, month, day, week = 0;
	struct timeval tv;
	struct tm* tm;
	time_t timeshow;
	static uint32_t tick= 0;
	//printf("%d-%d-%d-%d-%d\n",tm->tm_hour,tm->tm_min,tm->tm_mon,tm->tm_mday,tm->tm_wday);
	char buf[50];
	if (init){
		hour = 0;
		min = 0;
		month = 0;
		day = 0;
		week = 0;
		tick = itpGetTickCount();
	}
	if (OperatorParam.PowerStatus) return ret;
	gettimeofday(&tv, NULL);
	tm = localtime(&tv.tv_sec);
	timeshow = tv.tv_sec;
	//printf("LogoTimeShow:%s\n", ctime(&timeshow));
	if (itpGetTickDuration(tick) >= 1000){
		if (LogoTimeDotSprite->frame) ituSpriteGoto(LogoTimeDotSprite, 0);
		else                          ituSpriteGoto(LogoTimeDotSprite, 1);	
		tick = itpGetTickCount();
	}
	if ((hour != tm->tm_hour) || (min != tm->tm_min)){
		hour = tm->tm_hour;
		min = tm->tm_min;
		ituSpriteGoto(LogoTimeSprite[0], hour / 10);
		ituSpriteGoto(LogoTimeSprite[1], hour % 10);
		ituSpriteGoto(LogoTimeSprite[2], min / 10);
		ituSpriteGoto(LogoTimeSprite[3], min % 10);
		ret |=true;
	}
	if ((month != tm->tm_mon+1) || (day != tm->tm_mday) || (week != tm->tm_wday)){
		month = tm->tm_mon+1;
		day = tm->tm_mday;
		week = tm->tm_wday;
		sprintf(buf, "%02d%s%02d%s %s", month, "月", day, "日", WeekTab[week]);
		ituTextSetString(LogoDayText, buf);
		ret |= true;
	}
	return ret;
}
bool PowerStateShow(bool init,uint8_t power_status)
{
	bool ret = false;
	static int status = -1;
	if (init){
		status = -1;
	}
	if (status != power_status){
		printf("PowerStateShow:[%d]->[%d]\n", status, power_status);
		status = power_status;
		if (status){
			ituWidgetSetVisible(LogoPowerOffShowBG, false);
			ProcessLayerJump(LAYER_MAIN_MENU,1);
			//ituLayerGoto(MainMenuLayer);
		}
		else{
			ituWidgetSetVisible(LogoPowerOffShowBG, true);
			LogoTimeShow(false);
		}
		ret |= true;
	}
	return ret;
}
bool LogoLayerOnEnter(ITUWidget* widget, char* param)
{
	int i = 0;
	char buf[40];
	if (!LogoLayer){
		LogoLayer = ituSceneFindWidget(&theScene, "LogoLayer");
		MainMenuLayer = ituSceneFindWidget(&theScene, "MainMenuLayer");
		LogoLayerBG = ituSceneFindWidget(&theScene, "LogoLayerBG");
		LogoPowerOffShowBG = ituSceneFindWidget(&theScene, "LogoPowerOffShowBG");
		LogoDayText = ituSceneFindWidget(&theScene, "LogoDayText");
		LogoTimeDotSprite = ituSceneFindWidget(&theScene, "LogoTimeDotSprite");
		MainMenuBGSprite = ituSceneFindWidget(&theScene, "MainMenuBGSprite");
		for (i = 0; i < 4; i++){
			sprintf(buf, "LogoTimeSprite%d", i);
			LogoTimeSprite[i] = ituSceneFindWidget(&theScene, buf);
		}
	}
	STRUCT_CLR(tProcessVal);
	/**************************************************/
	tProcessVal.bAIFilterVal[0] = 85;
	tProcessVal.bAIFilterVal[1] = 85;
	//tProcessVal.bCurTemperVal = 65;
	/**************************************************/
	CustomConfigRW(CONFIG_FILE_RESERV, 1);
	PowerStateShow(true, OperatorParam.PowerStatus);
	AudioKeySound(SOUND_ERROR);
	OperatorParam.LayerID = LAYER_LOGO;
    return true;
}

bool LogoLayerOnTimer(ITUWidget* widget, char* param)
{
	bool ret = false;
	ret |= PowerStateShow(false, OperatorParam.PowerStatus);
	ret |= LogoTimeShow(false);
	return ret;
}

bool LogoLayerOnLeave(ITUWidget* widget, char* param)
{
    return true;
}
bool LogoPowerBtnOnPress(ITUWidget* widget, char* param)
{
	uint8_t buf[2] = { 0 };
	OperatorParam.PowerStatus = 1;
#ifdef ENABLE_MODBUS
	//uTransmitDev.pkt.bPowerOnOff = 1;
	buf[0] = OperatorParam.PowerStatus;
	ProtocolModBus_Send(SET_DATA, REG_ADDRESS_POWER, 1, buf);
#endif
	AudioKeySound(SOUND_CLICK2);
	ituSpriteGoto(MainMenuBGSprite, OperatorParam.CycleID);   //welson for bugsNo 15  switch from logo to mainmenu screen flash for change cycleID
	return true;
}
