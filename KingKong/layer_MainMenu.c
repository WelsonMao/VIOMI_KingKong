#include <assert.h>
#include "scene.h"
#include "ite/itp.h"
#include "extend/FileCom.h"
#include "bsp/uart_process.h"
#include "bsp/Modbus.h"
static ITULayer*MainMenuLayer;
static ITUBackground*MainMenuLayerBG, *SMARTProgressBG;
static ITUText*MainCycleTemperText, *MainModeText, *BotAIFilterText, *MainCycleStatusText, *SMARTProcessBarText,*SmartStatusText;
static ITUSprite*MainMenuBGSprite, *TopSideCtrlSprite[TOPSIDE_MAX], *AISprite, *TemperCatoonSprite, *MainMenuSprite, *ProgressSprite, *RightSprite;
static ITUIcon*TemperIcon[2], *NumIcon[10], *SMARTMaskIcon;
static ITUButton*TopReservBtn;

uint32_t wMainConfigSaveCnt = 0;
uint32_t wMainMenuTick = 0;
#pragma execution_character_set("utf-8")
bool AICatoonShow(uint8_t val1, uint8_t val2)
{
	bool ret = false;
	if ((10 == val1) || (5 == val1) || (0 == val1) || (10 == val2) || (5 == val2) || (0 == val2)){
		if (!AISprite->playing){
			ituTextSetString(BotAIFilterText, "滤芯寿命过低");
			ituSpritePlay(AISprite, 0);
			ret |= true;
		}
	}
	else{
		ituTextSetString(BotAIFilterText, "AI美肤洗滤芯");
		if ((AISprite->playing) || (AISprite->frame)){
			ituSpriteGoto(AISprite, 0);
			ituSpriteStop(AISprite);
			ret |= true;
		}
	}
	return ret;
}
bool MainMenuCycleSync(uint8_t id,uint32_t temper)
{
	bool ret = false;
	static int backid = -1;
	static int32_t backtemper = -1;
	char buf[40];
	if ((backid != id) || (backtemper != temper)){
		backid = id;
		backtemper = temper;
		ituSpriteGoto(MainMenuBGSprite, id);
		if (id <= UI_CYCLE_ECO) sprintf(buf, "%s%d%s", "设定温度",tCycleParamTab[id].bTemperVal, "℃");
		else                    sprintf(buf, "%s%d%s", "设定温度", temper, "℃");
		ituTextSetString(MainCycleTemperText, buf);
		sprintf(buf, "%s", tCycleParamStr[id].Name);
		ituTextSetString(MainModeText, buf);
		printf("MainMenuCycleSync:Modeid=%d,temper=%d\n", id, tCycleParamTab[id].bTemperVal);
		ret = true;
	}
	return ret;
}
bool MainMenuWaterTemperSync(uint16_t temper)
{
	bool ret = false;
	static int temperback = -1;
	if (temperback != temper){
		printf("MainMenuWaterTemperSync:temper[%d]->[%d]\n", temperback, temper);
		temperback = temper;
		if (temper < 100){
			ituIconLinkSurface(TemperIcon[0], NumIcon[temper / 10]);
			ituIconLinkSurface(TemperIcon[1], NumIcon[temper % 10]);
		}
		else{
			ituIconLinkSurface(TemperIcon[0], NumIcon[9]);
			ituIconLinkSurface(TemperIcon[1], NumIcon[9]);
		}
		ituSpriteGoto(TemperCatoonSprite, temper);
		ret |= true;
	}
	return ret;
}
bool MainMenuWiFiShow(uint8_t state)
{
	bool ret = false;
	if (TopSideCtrlSprite[TOPSIDE_WIFI]->frame != state){
		ituSpriteGoto(TopSideCtrlSprite[TOPSIDE_WIFI], state);
		ret = true;
	}
	return ret;
}
bool MainMenuSmartShowProgress(uint8_t enable, uint8_t remain)
{
	bool ret = false;
	char buf[50];
	uint8_t data = 0;
	static uint8_t enableFlag = 0;
	static uint8_t val = 0;
	int maskInitPosx = -7;
	int progressWitdh = 481;
	if (enableFlag != enable){
		enableFlag = enable;
		ituSpriteGoto(MainMenuSprite, enableFlag);
		ituSpriteGoto(ProgressSprite, OperatorParam.CycleID);
		ituSpriteGoto(RightSprite, OperatorParam.CycleID);
		if (enableFlag) ituTextSetString(SmartStatusText, "智能杀菌中");
		ret = true;
	}
	if (enableFlag && (val != remain)){
		val = remain;
		data = 100-val * 100 / 30;
		sprintf(buf, "%02d%s", data, "%");//SMARTProgressBG  SMARTMaskIcon -7
		ituTextSetString(SMARTProcessBarText, buf);
		//ituWidgetSetWidth(SMARTProgressBG, data*progressWitdh / 100);
		ituWidgetSetX(ProgressSprite, data*progressWitdh / 100 + maskInitPosx);
		//ituWidgetSetX(SMARTMaskIcon, data*progressWitdh / 100 + maskInitPosx);
		//ituProgressBarSetValue(SMARTProgressBar, data);
		//printf("data=%d\n", data);
		ret |= true;
	}
	return ret;
}
bool MainMenuStatusShow(uint8_t status)
{
	bool ret = false;
	static int back = -1;
	if (back != status){
		printf("%s:status[%d->%d]\n", __func__, back, status);
		ituTextSetString(MainCycleStatusText, RunStatusStr[status]);
		back = status;
		ret = true;
	}
	return ret;
}
bool MainMenuLayerOnEnter(ITUWidget* widget, char* param)
{
	int i = 0;
	char buf[40];
	if (!MainMenuLayer){
		MainMenuLayer = ituSceneFindWidget(&theScene, "MainMenuLayer");
		MainMenuLayerBG = ituSceneFindWidget(&theScene, "MainMenuLayerBG");
		MainCycleTemperText = ituSceneFindWidget(&theScene, "MainCycleTemperText");
		MainModeText = ituSceneFindWidget(&theScene, "MainModeText");
		BotAIFilterText = ituSceneFindWidget(&theScene, "BotAIFilterText");
		MainMenuBGSprite = ituSceneFindWidget(&theScene, "MainMenuBGSprite");
		AISprite = ituSceneFindWidget(&theScene, "AISprite");
		TopReservBtn = ituSceneFindWidget(&theScene, "TopReservBtn");
		TemperCatoonSprite = ituSceneFindWidget(&theScene, "TemperCatoonSprite");
		MainCycleStatusText = ituSceneFindWidget(&theScene, "MainCycleStatusText");
		SMARTProcessBarText = ituSceneFindWidget(&theScene, "SMARTProcessBarText");
		SMARTProgressBG = ituSceneFindWidget(&theScene, "SMARTProgressBG");
		SMARTMaskIcon = ituSceneFindWidget(&theScene, "SMARTMaskIcon");
		MainMenuSprite = ituSceneFindWidget(&theScene, "MainMenuSprite");
		SmartStatusText = ituSceneFindWidget(&theScene, "SmartStatusText");
		ProgressSprite = ituSceneFindWidget(&theScene, "ProgressSprite");
		RightSprite = ituSceneFindWidget(&theScene, "RightSprite");
		
		for (i = 0; i < TOPSIDE_MAX; i++){
			sprintf(buf, "TopSideCtrlSprite%d", i);
			TopSideCtrlSprite[i] = ituSceneFindWidget(&theScene, buf);
		}
		for (i = 0; i < 2; i++){
			sprintf(buf, "TemperIcon%d", i);
			TemperIcon[i] = ituSceneFindWidget(&theScene, buf);
		}
		for (i = 0; i < 10; i++){
			sprintf(buf, "NumIcon%d", i);
			NumIcon[i] = ituSceneFindWidget(&theScene, buf);
		}
	}
	OperatorParam.LayerID = LAYER_MAIN_MENU;
	wMainMenuTick = itpGetTickCount();
	printf("%s\n", __func__);
    return true;
}

bool MainMenuLayerOnTimer(ITUWidget* widget, char* param)
{
	bool ret = false;
	int ypos = 0;
	static uint8_t tone = SOUND_FUNCTION_IN;
#if 0
	if (itpGetTickDuration(wMainMenuTick) >= 3 * 1000){
		AudioKeySound(tone);
		if (SOUND_FUNCTION_IN == tone) tone = SOUND_FUNCTION_OUT;
		else if (SOUND_FUNCTION_OUT == tone) tone = SOUND_CLICK;
		else if (SOUND_CLICK == tone) tone = SOUND_CLICK2;
		else if (SOUND_CLICK2 == tone) tone = SOUND_FUNCTION_IN;
		wMainMenuTick = itpGetTickCount();
	}
#endif
#ifdef _WIN32
	//uRevDev.pkt.bBacterStatus = 1;
	if (itpGetTickDuration(wMainMenuTick) >= 5 * 100){
		//if (!uRevDev.pkt.bTemper[3]) uRevDev.pkt.bTemper[3] = 99;
		//else                         uRevDev.pkt.bTemper[3] = 0;
		/*if (uRevDev.pkt.bTemper[3] < 100) uRevDev.pkt.bTemper[3]++;
		else                              uRevDev.pkt.bTemper[3] = 0;*/
		//uRevDev.pkt.bTemper++;
		//if (uRevDev.pkt.bTemper > 1) uRevDev.pkt.bTemper = 0;
		//if (!uRevDev.pkt.bTemper) uRevDev.pkt.bTemper = 100;
		//else                      uRevDev.pkt.bTemper = 0;
		if (uRevDev.pkt.bBacterRemainTime)uRevDev.pkt.bBacterRemainTime--;
		else                              uRevDev.pkt.bBacterRemainTime=30;
		wMainMenuTick = itpGetTickCount();
	}
#endif
	/*****************************************************************************/
	if (OperatorParam.ProcessID || bit_Label_ScreenSaver){
		if (ituWidgetIsEnabled(MainMenuLayerBG)) ituWidgetDisable(MainMenuLayerBG);	
	}
	else{
		if (!ituWidgetIsEnabled(MainMenuLayerBG)) ituWidgetEnable(MainMenuLayerBG);
	}
	/*****************************************************************************/
	ret |= MainMenuCycleSync(OperatorParam.CycleID,ACUGet_TargetTemper());
	ret |= MainMenuWaterTemperSync(ACUGet_Temper());
	ret |= MainMenuStatusShow(ACUGet_DevStatus());
	ret |= AICatoonShow(uRevDev.pkt.bFilterLife[0], uRevDev.pkt.bFilterLife[1]);
	ret |= MainMenuWiFiShow(bit_Label_WiFi);
	ret |= MainMenuSmartShowProgress(ACUGet_SmartStatus(), ACUGet_SmartRemainTime());
	//ret |= MainMenuSmartShowProgress(1, ACUGet_SmartRemainTime());
	//if ((bit_Config_AISurface != tConfig.Enable_AISurface) && (itpGetTickDuration(wMainConfigSaveCnt) >= CONFIG_SAVE_TIME))
	//	ConfigSave();
	return ret;
}

bool MainMenuLayerOnLeave(ITUWidget* widget, char* param)
{
	//printf("%s\n", __func__);
	//if ((bit_Config_AISurface != tConfig.Enable_AISurface))
	//	ConfigSave();
    return true;
}

bool MainMenuBtnOnPress(ITUWidget* widget, char* param)
{
	int val = atoi(param);
	uint8_t buf[2] = { 0 };
	if (0 == val) ProcessShow(PROCESS_ID_SET_CYCLE, INFO_ID_NULL);
	else if (1 == val){
		ProcessLayerJump(LAYER_LOGO, 1);
		OperatorParam.PowerStatus = 0;
#ifdef ENABLE_MODBUS
		//uTransmitDev.pkt.bPowerOnOff = 0;
		buf[0] = OperatorParam.PowerStatus;
		ProtocolModBus_Send(SET_DATA, REG_ADDRESS_POWER, 1, buf);
#endif
	}
	else if (2 == val){
#ifdef ENABLE_MODBUS
		ProtocolModBus_Send(GET_DATA, REG_ADDRESS_DELAY, REG_ADDRESS_DELAY_LEN, NULL);
#endif
		ProcessShow(PROCESS_ID_AI_FILTER, INFO_ID_NULL);
	}
	else if (3 == val) ProcessShow(PROCESS_ID_RESERV, INFO_ID_NULL);
	AudioKeySound(SOUND_FUNCTION_IN);
    return true;
}
bool TopSideBtnOnPress(ITUWidget* widget, char* param)
{
	int val = atoi(param);
	if (TOPSIDE_CONFIG == val){
#ifdef ENABLE_MODBUS
		ProtocolModBus_Send(GET_DATA, REG_ADDRESS_DEVINFO, REG_ADDRESS_DEVINFO_LEN, NULL);
#endif
		ProcessLayerJump(LAYER_CONFIG, 1);
	}
	AudioKeySound(SOUND_CLICK2);
    return true;
}

