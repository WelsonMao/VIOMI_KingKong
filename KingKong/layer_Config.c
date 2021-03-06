#include <assert.h>
#include "scene.h"
#include "ite/itp.h"
#include "bsp/uart_process.h"
#include "bsp/Modbus.h"
static ITULayer*ConfigLayer;
static ITUBackground*ConfigLayerBG, *ConfigReturnBG;
static ITUSprite*ProcessShowSprite, *ConfigPageSprite, *ConfigDetailSprite, *ConfigOptionSprite[CONFIG_SW_MAX], *ConfigWiFiRstSprite, *ConfigWiFiLinkSprite, *ConfigWiFiStatusSprite;
static ITUText*VersionText[4], *ConfigMainText[CONFIG_MENU_MAX], *ConfigBrigtnessText, *ConfigScreenSaverText, *ConfigKeyVolText, *ConfigSetDateText, *ConfigSetTimeText, *ConfigSmartText;
static ITUTextBox*ConfigTitleTextBox;
static ITUCheckBox*ConfigMainCheckBox[CONFIG_MENU_MAX];
static ITUContainer*ScreenSaverContainer, *VolumeContainer, *SmartTemperContainer;
static ITUSprite*ConfigVerSprite, *ConfigRstSprite, *RstPerHSprite, *RstPerLSprite, *ConfigWiFiLinkSprite, *WiFiLinkPerHSprite, *WiFiLinkPerLSprite;
static ITUDigitalClock*DigitalClock;

static bool TextBoxChgFlag = false;
static uint8_t bTypePercent = PERCENT_NULL;
RPT_TICK_t tFCTBtn;
#pragma execution_character_set("utf-8")

void ConfigLayerInit(void)
{
	char buf[50];
	int i = 0;
	bTypePercent = PERCENT_NULL;
	ituSpriteGoto(ConfigDetailSprite, 0);
	ituSpriteGoto(ConfigWiFiRstSprite, 0);
	ituSpriteGoto(ConfigWiFiStatusSprite, bit_Label_WiFi);
	ituCheckBoxSetChecked(ConfigMainCheckBox[0], true);
	OperatorParam.ConfigID = 0;
	for (i = 1; i < CONFIG_MENU_MAX; i++)  ituCheckBoxSetChecked(ConfigMainCheckBox[i], false);
	ituSpriteGoto(ConfigOptionSprite[CONFIG_SW_TEMPER_RESEVOUT], tConfig.Enable_TemperResevOut);
	ituSpriteGoto(ConfigOptionSprite[CONFIG_SW_WATEROUT_POWEROFF], tConfig.Enable_WaterOutPowerOff);
	ituSpriteGoto(ConfigOptionSprite[CONFIG_SW_SCREENSAVER], tConfig.Enable_ScreenSaver);
	ituSpriteGoto(ConfigOptionSprite[CONFIG_SW_KEY_VOICE], tConfig.Enable_KeyVoice);
	ituSpriteGoto(ConfigOptionSprite[CONFIG_SW_SMART], tConfig.SmartVal?1:0);//tConfig.Enable_Smart
	ituWidgetSetVisible(ScreenSaverContainer, tConfig.Enable_ScreenSaver);
	ituWidgetSetVisible(VolumeContainer, tConfig.Enable_KeyVoice);
	ituWidgetSetVisible(SmartTemperContainer, tConfig.SmartVal ? true : false);// tConfig.Enable_Smart
	if (0 == tConfig.SmartVal) sprintf(buf, "%s", "关闭");
	else if (1 == tConfig.SmartVal) sprintf(buf, "%s", "一周");
	else if (2 == tConfig.SmartVal) sprintf(buf, "%s", "二周");
	else if (3 == tConfig.SmartVal) sprintf(buf, "%s", "三周");
	else if (4 == tConfig.SmartVal) sprintf(buf, "%s", "一个月");
	ituTextSetString(ConfigSmartText, buf);
	sprintf(buf, "%d%s", tConfig.BrightnessVal*10, "%");
	ituTextSetString(ConfigBrigtnessText, buf);
	sprintf(buf, "%s%d%s", "无操作",tConfig.ScreenSaverTimeSec,"秒后");
	ituTextSetString(ConfigScreenSaverText, buf);
	sprintf(buf, "%d%s", tConfig.KeyVolumeVal, "%");
	ituTextSetString(ConfigKeyVolText, buf);
}
bool ConfigSetValUpdate(void)
{
	char buf[50];
	bool ret = false;
	static int backval = 0;
	static int smartval = 0;
	if (tConfig.BrightnessVal != theConfig.brightness){
		if (backval != tConfig.BrightnessVal){
			backval = tConfig.BrightnessVal;
			ret |= true;
		}
		if (ret){
			printf("%s:[BrightnessVal]%d->%d\n", __func__, theConfig.brightness, tConfig.BrightnessVal);
			sprintf(buf, "%d%s", tConfig.BrightnessVal * 10, "%");
			ituTextSetString(ConfigBrigtnessText, buf);
			ret = false;
		}	
	}
	if (tConfig.ScreenSaverTimeSec != theConfig.screensaver_time){
		if (backval != tConfig.ScreenSaverTimeSec){
			backval = tConfig.ScreenSaverTimeSec;
			ret |= true;
		}
		if (ret){
			printf("%s:[ScreenSaver]%d->%d\n", __func__, theConfig.screensaver_time, tConfig.ScreenSaverTimeSec);
			sprintf(buf, "%s%d%s", "无操作", tConfig.ScreenSaverTimeSec, "秒后");
			ituTextSetString(ConfigScreenSaverText, buf);
			ret = false;
		}
	}
	if (tConfig.KeyVolumeVal != theConfig.keylevel){
		if (backval != tConfig.KeyVolumeVal){
			backval = tConfig.KeyVolumeVal;
			ret |= true;
		}
		if (ret){
			printf("%s:[KeyVolume]%d->%d\n", __func__, theConfig.keylevel, tConfig.KeyVolumeVal);
			sprintf(buf, "%d%s", tConfig.KeyVolumeVal, "%");
			ituTextSetString(ConfigKeyVolText, buf);
			ret = false;
		}
	}
	//printf("%d-%d\n", tConfig.SmartVal, theConfig.SmartVal);
	//if (tConfig.SmartVal != theConfig.SmartVal)
	{
		if (smartval != tConfig.SmartVal){
			smartval = tConfig.SmartVal;
			ret |= true;
		}
		if (ret){
			printf("%s:[SmartVal]%d->%d\n", __func__, theConfig.SmartVal, tConfig.SmartVal);
			if (0 == tConfig.SmartVal) sprintf(buf, "%s", "关闭");
			else if (1 == tConfig.SmartVal) sprintf(buf, "%s", "一周");
			else if (2 == tConfig.SmartVal) sprintf(buf, "%s", "二周");
			else if (3 == tConfig.SmartVal) sprintf(buf, "%s", "三周");
			else if (4 == tConfig.SmartVal) sprintf(buf, "%s", "一个月");
			ituTextSetString(ConfigSmartText, buf);
			ret = false;
		}
	}
	return ret;
}

bool CycleProcessPercentShow(uint8_t type, bool init)
{
	static uint32_t pertick = 0;
	static uint8_t per = 0;
	uint8_t val = 0;
	bool ret = false;
	if (init){
		pertick = itpGetTickCount();
		per = 0;
		ret = true;
	}
	if (PERCENT_WIFI == type){
		if (bit_Label_WiFi) val = 100;
		else{
			if (per<99) val = itpGetTickDuration(pertick) * 100 / WIFI_RST_TIMEOUT;
			else        val = 99;
		}
	}
	else if (PERCENT_RST == type)  val = itpGetTickDuration(pertick) * 100 / SYS_RST_TIMEOUT;
	if (per != val){
		printf("%d-%d\n", val, per);
		per = val;
		if (PERCENT_WIFI == type){
			if (per >= 100){
				bTypePercent = PERCENT_NULL;
				ituSpriteGoto(ConfigWiFiRstSprite, 0);
				ituSpriteGoto(ConfigWiFiStatusSprite, 1);//0 fail 1 sucess
			}
			else{
				ituSpriteGoto(ConfigWiFiLinkSprite, per*ConfigWiFiLinkSprite->childCount / 100);
				ituSpriteGoto(WiFiLinkPerHSprite, per / 10);
				ituSpriteGoto(WiFiLinkPerLSprite, per % 10);
			}
		}
		else if (PERCENT_RST == type){
			if (per>=100){
				bTypePercent = PERCENT_NULL;
				exit(0);
			}
			else{
				ituSpriteGoto(ConfigRstSprite, per*ConfigRstSprite->childCount / 100);
				ituSpriteGoto(RstPerHSprite, per / 10);
				ituSpriteGoto(RstPerLSprite, per % 10);
				ret = true;
			}
		}	
	}
	return ret;
}
bool ConfigDataTimeUpdate(void)
{
	bool ret = false;
	struct timeval tv;
	struct timezone tzone;
	struct tm* tm;
	time_t auqian;
	static uint8_t timeSetFlag = 0;
	char buf[50];
	if ((!OperatorParam.ProcessID) && (CONFIG_MENU_DATE_TIME == ConfigDetailSprite->frame)&&(0 == timeSetFlag++)){
		gettimeofday(&tv, NULL);
		tm = localtime(&tv.tv_sec);
		tzone.tz_minuteswest = 8 * 60;
		tzone.tz_dsttime = 0;
		//settimeofday(&tv, &tzone);
		auqian = tv.tv_sec;
		//ConfigSetDateText  ConfigSetTimeText
		sprintf(buf, "%d%s%2d%s%2d%s", (tm->tm_year + 1900), "年", (tm->tm_mon + 1), "月", tm->tm_mday, "日");
		ituTextSetString(ConfigSetDateText, buf);
		sprintf(buf, "%02d%:%02d", tm->tm_hour, tm->tm_min);
		ituTextSetString(ConfigSetTimeText, buf);
		printf("==Local Time:%s", ctime(&auqian));
		//printf("tv_sec=%d,%d-%d,%d-%d,%d-%d-%d\n", auqian, tm->tm_year, tm->tm_yday, tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
		//printf("DigitalClock:%d:%d,%d:%d:%d\n", DigitalClock->year, DigitalClock->month, DigitalClock->day, DigitalClock->hour, DigitalClock->minute, DigitalClock->second);
		ret = true;
	}
	return ret;
}
bool ConfigLayerOnEnter(ITUWidget* widget, char* param)
{
	int i = 0;
	char buf[40];
	if (!ConfigLayer){
		ConfigLayer = ituSceneFindWidget(&theScene, "ConfigLayer");
		ConfigLayerBG = ituSceneFindWidget(&theScene, "ConfigLayerBG");
		ConfigReturnBG = ituSceneFindWidget(&theScene, "ConfigReturnBG");
		ProcessShowSprite = ituSceneFindWidget(&theScene, "ProcessShowSprite");
		ConfigDetailSprite = ituSceneFindWidget(&theScene, "ConfigDetailSprite");
		ConfigWiFiRstSprite = ituSceneFindWidget(&theScene, "ConfigWiFiRstSprite");
		ConfigWiFiLinkSprite = ituSceneFindWidget(&theScene, "ConfigWiFiLinkSprite");
		ConfigWiFiStatusSprite = ituSceneFindWidget(&theScene, "ConfigWiFiStatusSprite");
		ConfigTitleTextBox = ituSceneFindWidget(&theScene, "ConfigTitleTextBox");
		ScreenSaverContainer = ituSceneFindWidget(&theScene, "ScreenSaverContainer");
		VolumeContainer = ituSceneFindWidget(&theScene, "VolumeContainer");
		ConfigBrigtnessText = ituSceneFindWidget(&theScene, "ConfigBrigtnessText");
		ConfigScreenSaverText = ituSceneFindWidget(&theScene, "ConfigScreenSaverText");
		ConfigKeyVolText = ituSceneFindWidget(&theScene, "ConfigKeyVolText");
		ConfigVerSprite = ituSceneFindWidget(&theScene, "ConfigVerSprite");
		ConfigRstSprite = ituSceneFindWidget(&theScene, "ConfigRstSprite");
		RstPerHSprite = ituSceneFindWidget(&theScene, "RstPerHSprite");
		RstPerLSprite = ituSceneFindWidget(&theScene, "RstPerLSprite");
		ConfigWiFiLinkSprite = ituSceneFindWidget(&theScene, "ConfigWiFiLinkSprite");
		WiFiLinkPerHSprite = ituSceneFindWidget(&theScene, "WiFiLinkPerHSprite");
		WiFiLinkPerLSprite = ituSceneFindWidget(&theScene, "WiFiLinkPerLSprite");
		ConfigSetDateText = ituSceneFindWidget(&theScene, "ConfigSetDateText");
		ConfigSetTimeText = ituSceneFindWidget(&theScene, "ConfigSetTimeText");
		DigitalClock = ituSceneFindWidget(&theScene, "DigitalClock");
		ConfigSmartText = ituSceneFindWidget(&theScene, "ConfigSmartText");
		SmartTemperContainer = ituSceneFindWidget(&theScene, "SmartTemperContainer");
		for (i = 0; i < CONFIG_SW_MAX; i++){
			sprintf(buf, "ConfigOptionSprite%d", i);
			ConfigOptionSprite[i] = ituSceneFindWidget(&theScene, buf);
		}
		for (i = 0; i < 4; i++){
			sprintf(buf, "VersionText%d", i);
			VersionText[i] = ituSceneFindWidget(&theScene, buf);
		}
		for (i = 0; i < CONFIG_MENU_MAX; i++){
			sprintf(buf, "ConfigMainText%d", i);
			ConfigMainText[i] = ituSceneFindWidget(&theScene, buf);
			sprintf(buf, "ConfigMainCheckBox%d", i);
			ConfigMainCheckBox[i] = ituSceneFindWidget(&theScene, buf);
		}
	}
	ConfigLayerInit();
	TextBoxChgFlag = true;
	OperatorParam.LayerID = LAYER_CONFIG;
    return true;
}

bool ConfigLayerOnTimer(ITUWidget* widget, char* param)
{
	bool ret = false;
	int len = 0;
	int beginY = 0;
	int height = 0;
	if (ProcessShowSprite->frame || bit_Label_ScreenSaver) ituWidgetDisable(ConfigLayerBG);
	else                          ituWidgetEnable(ConfigLayerBG);
	/**********************************************/

		
	/**********************************************/
	if (TextBoxChgFlag){
		len = strlen(ituTextGetString(ConfigMainText[ConfigDetailSprite->frame]));
		if (1 == ConfigDetailSprite->frame){
			ituTextBoxSetString(ConfigTitleTextBox, "网络");
			len = 9;
		}
		else
			ituTextBoxSetString(ConfigTitleTextBox, ituTextGetString(ConfigMainText[ConfigDetailSprite->frame]));
		len /= 3;
		height = len * 38 + (len - 1) * 48;
		beginY = (600 - height) / 2;
		ituWidgetSetHeight(ConfigTitleTextBox, height);
		ituWidgetSetY(ConfigTitleTextBox, beginY);
		TextBoxChgFlag = false;
	}
	ret |= ConfigSetValUpdate();
	ret |= CycleProcessPercentShow(bTypePercent, false);
	ret |= ConfigDataTimeUpdate();
	/**********************************************/
	if (tFCTBtn.enable){
		if (itpGetTickDuration(tFCTBtn.tick) >= 1500){
			memset(&tFCTBtn, 0, sizeof(RPT_TICK_t));
		}
	}
    return ret;
}

bool ConfigLayerOnLeave(ITUWidget* widget, char* param)
{
	/**********************************************/
	/*if (((tConfig.Enable_TemperResevOut != bit_Config_TemperResevOut)\
		|| (tConfig.Enable_WaterOutPowerOff != bit_Config_WaterOutPowerOff)\
		|| (tConfig.Enable_NetTime != bit_Config_NetTime)\
		|| (tConfig.Enable_ScreenSaver != bit_Config_ScreenSaver)\
		|| (tConfig.Enable_KeyVoice != bit_Config_KeyVoice)\
		|| (tConfig.BrightnessVal != theConfig.brightness)\
		|| (tConfig.ScreenSaverTimeSec != theConfig.screensaver_time)\
		|| (tConfig.KeyVolumeVal != theConfig.keylevel)\
		|| (tConfig.Enable_Smart != bit_Config_Smart)\
		|| (tConfig.SmartVal != theConfig.SmartVal)\
		)\
		){
		ConfigSave();
	}	*/
	/**********************************************/
    return true;
}

bool ConfigReturnBtnOnPress(ITUWidget* widget, char* param)
{
	ITULayer*golayer = ituSceneFindWidget(&theScene, "MainMenuLayer");
	ituLayerGoto(golayer);
	AudioKeySound(SOUND_CLICK2);
    return true;
}

bool ConfigJumpBtnOnPress(ITUWidget* widget, char* param)
{
	char buf[40];
	int val = atoi(param);
	if (0 == val)      ProcessShow(PROCESS_ID_AI_FILTER, INFO_ID_NULL);
	else if (1 == val) ProcessShow(PROCESS_ID_DATE_TIME,INFO_ID_DATE);
	else if (2 == val) ProcessShow(PROCESS_ID_DATE_TIME, INFO_ID_TIME);
	else if (3 == val) ProcessShow(PROCESS_ID_SCREEN, INFO_ID_NULL);
	else if (4 == val) ProcessShow(PROCESS_ID_SS_VOL, INFO_ID_SCREEN_SAVER);
	else if (5 == val) ProcessShow(PROCESS_ID_SS_VOL, INFO_ID_KEY_AUDIO);
	else if (6 == val) ProcessShow(PROCESS_ID_SMART, INFO_ID_NULL);
	AudioKeySound(SOUND_CLICK2);
	printf("ConfigJumpBtnOnPress:%d\n", val);
    return true;
}

bool ConfigOptionBtnOnPress(ITUWidget* widget, char* param)
{
	int val = atoi(param);
	int cur_frame = ConfigOptionSprite[val]->frame;
	int to_frame = (ConfigOptionSprite[val]->frame?0:1);

	if (CONFIG_SW_TEMPER_RESEVOUT == val) tConfig.Enable_TemperResevOut = to_frame;
	else if (CONFIG_SW_WATEROUT_POWEROFF == val) tConfig.Enable_WaterOutPowerOff = to_frame;
	else if (CONFIG_SW_NET_TIME == val){
		tConfig.Enable_NetTime = to_frame;
		if (tConfig.Enable_NetTime) LocalTimeSyc();
	}
	else if (CONFIG_SW_SCREENSAVER == val) tConfig.Enable_ScreenSaver = to_frame;
	else if (CONFIG_SW_KEY_VOICE == val) tConfig.Enable_KeyVoice = to_frame;
	else if (CONFIG_SW_SMART == val){
		if (to_frame) tConfig.SmartVal = 1;
		else          tConfig.SmartVal = 0;
		tConfig.Enable_Smart = to_frame;
	}
	if (CONFIG_SW_SCREENSAVER == val){
		if (tConfig.Enable_ScreenSaver){
			if (!ituWidgetIsVisible(ScreenSaverContainer)) ituWidgetShow(ScreenSaverContainer, ITU_EFFECT_SCROLL_DOWN, 5);
		}
		else{
			if (ituWidgetIsVisible(ScreenSaverContainer))  ituWidgetHide(ScreenSaverContainer, ITU_EFFECT_SCROLL_UP, 5);
		}
	}
	else if (CONFIG_SW_KEY_VOICE == val){
		if (tConfig.Enable_KeyVoice){
			if (!ituWidgetIsVisible(VolumeContainer))      ituWidgetShow(VolumeContainer, ITU_EFFECT_SCROLL_DOWN, 5);
		}
		else{
			if (ituWidgetIsVisible(VolumeContainer))       ituWidgetHide(VolumeContainer, ITU_EFFECT_SCROLL_UP, 5);
		}
	}
	else if (CONFIG_SW_SMART == val){
		if (tConfig.Enable_Smart){
			if (!ituWidgetIsVisible(SmartTemperContainer))      ituWidgetShow(SmartTemperContainer, ITU_EFFECT_SCROLL_DOWN, 5);
		}
		else{
			if (ituWidgetIsVisible(SmartTemperContainer))       ituWidgetHide(SmartTemperContainer, ITU_EFFECT_SCROLL_UP, 5);
		}
	}
	ituSpriteGoto(ConfigOptionSprite[val], to_frame);
	wConfigSaveCnt = itpGetTickCount();
	AudioKeySound(SOUND_CLICK2);
	printf("ConfigOptionBtnOnPress:%d[%d]->[%d],%#x\n", val, cur_frame, to_frame, _fConfigOption.byte);
    return true;
}

bool ConfigWiFiRstBtn(ITUWidget* widget, char* param)
{
	int val = atoi(param);
	uint8_t buf[2] = { 0 };
	ituSpriteGoto(ConfigWiFiRstSprite, val + 1);
#ifdef ENABLE_MODBUS
	if (!val){
		buf[0] =1;
		ProtocolModBus_Send(SET_DATA, REG_ADDRESS_WIFI_RST, 1, buf);
	}
#endif
	if (3 == ConfigWiFiRstSprite->frame){
		bTypePercent = PERCENT_WIFI;
		CycleProcessPercentShow(bTypePercent, true);
	}
	AudioKeySound(SOUND_CLICK2);
    return true;
}

bool ConfigMainCheckBoxOnPress(ITUWidget* widget, char* param)
{
	int val = atoi(param);
	if (OperatorParam.ConfigID != val){
		for (int i = 0; i < CONFIG_MENU_MAX; i++) ituCheckBoxSetChecked(ConfigMainCheckBox[i], false);
		ituCheckBoxSetChecked(ConfigMainCheckBox[val], true);
		ituSpriteGoto(ConfigDetailSprite, val);
		OperatorParam.ConfigID = val;
		TextBoxChgFlag = true;
		if (CONFIG_MENU_SW_VER == OperatorParam.ConfigID){
			ituTextSetString(VersionText[0], uRevDevInfo.pkt.cFWVer);//固件版本
			ituTextSetString(VersionText[1], SW_VERSION_STR);//系统版本
			ituTextSetString(VersionText[3], uRevDevInfo.pkt.cMode);//固件版本
		}
		//printf("ConfigMainCheckBoxOnPress:%d,ConfigID=%d\n", val, OperatorParam.ConfigID);
	}
	else
		ituCheckBoxSetChecked(ConfigMainCheckBox[val], true);
	AudioKeySound(SOUND_CLICK2);
    return true;
}

bool ResetBtnOnPress(ITUWidget* widget, char* param)
{
	ituSpriteGoto(ConfigVerSprite, 1);
	bTypePercent = PERCENT_RST;
	ConfigReset();
	CycleProcessPercentShow(bTypePercent, true);
	AudioKeySound(SOUND_CLICK2);
	return true;
}
bool FCTBtnOnPress(ITUWidget* widget, char* param)
{
	if (!tFCTBtn.enable){
		tFCTBtn.enable = true;
		tFCTBtn.cnt = 0;
	}
	tFCTBtn.cnt++;
	tFCTBtn.tick = itpGetTickCount();
	if (tFCTBtn.cnt >= 3){
		ITULayer*goLayer = ituSceneFindWidget(&theScene, "FCTLayer");
		ituLayerGoto(goLayer);
		memset(&tFCTBtn, 0, sizeof(RPT_TICK_t));
	}
	AudioKeySound(SOUND_CLICK2);
	return true;
}