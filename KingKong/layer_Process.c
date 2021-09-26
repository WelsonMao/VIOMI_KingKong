#include <assert.h>
#include "scene.h"
#include "ite/itp.h"
#include "extend/FileCom.h"
#include "extend/CRC.h"
#include "bsp/uart_process.h"
#include "bsp/Modbus.h"
#include "sys/time.h"

static ITULayer*ProcessLayer;
static ITUBackground*AIResetConfirmBG, *ScreenSaverBG, *ProcessLayerBG;
static ITUCoverFlow*CycleSetCoverFlow;
static ITUSprite*ProcessCycleSetSprite[UI_CYCLE_MAX], *ProcessShowSprite, *ProcessWBSprite, *ProcessDateTimeSprite, *ProcessTimeUniteSprite, *PopMenuSprite;
static ITUTrackBar *CustomCycleTrackBar, *BrightnessTrackBar;
static ITUProgressBar*CustomCycleProgressBar;
static ITUButton*TrackBarBtn,*BrightnessBtn;
static ITUText*TrackBarText, *AIFilterValText[2], *ProcessDateTimeText, *SSVolText[6], *ProcessCycleSetText[UI_CYCLE_CUSTOMER];
static ITUIcon*CustomCycleTemperIcon;
static ITURadioBox*SSVolRadioBox[6];
static ITUWheel*LocalYearWheel, *LocalMonthWheel, *LocalDayWheel, *LocalHourWheel, *LocalMinWheel, *SmartWheel, *BrightnessWheel;
#ifdef ENABLE_RESERV
static ITUCheckBox*ReserTypeCheckBox;
static ITUContainer*ReserTypeContainer, *ReservListContainer[3], *ReserTipContainer, *ReservCustomerContainer, *ReservCancelContainer;
static ITUSprite*ReservSetMainSprite, *ReservListSprite[4], *ReservListEditSprite[3], *ReservSetSubSprite, *ReservParamSetSprite, *ReservAddBtnSprite, *ReservEditBtnSprite, *ReserTypeSprite;
static ITUText*ReservTimeText[3], *ReservCycleText[3], *ReserTipText, *ReservEditTitleText, *ReservSetTimeText, *ReservSetCycleText, *ReservTimeCycleText, *ReservSetTemperText, *ReservParamSetText;
static ITUIcon*ReservSetMainMaskIcon, *ReservSelMainMaskIcon, *ReservTimePIcon[12], *ReservTimeWIcon[24];
static ITUWheel*ReservTimeFormWheel, *ReservTimeToWheel, *ReservTemperWheel, *ReservTemperUnitIcon;
static ITUButton*ReservAddBtn, *ReservEditBtn;
static ITURadioBox*ReservModeSelRadioBox[UI_CYCLE_MAX];
static ITUText*ReservModeSelText[UI_CYCLE_MAX];
#endif
#ifdef ENABLE_ERR
static ITUBackground*ProcessTopInfoBG, *InfoErrCntBG, *ProcessErrDirBtnBG[2];
static ITUText*ErrCntText, *TopErrText,*ErrText[3];
static ITUSprite*ProcessErrTypeSprite;
#endif
#ifdef ENABLE_MCUPGRADE
static ITUBackground*UpgradeBG;
static ITUText*UpgradeText;
static ITUAnimation*UpgradeAni;
#endif
uint32_t wConfigSaveCnt,wProcessCnt = 0;
BASE_TIME_t tBaseTime,tBaseSetTime;
BASE_TIME_t tTimeBack = {
	.hour = 0,
	.min = 0,
	.sec = 0,
	.day = 0,
	.month = 0,
	.year = 0
};
#pragma execution_character_set("utf-8")
const int SS_KeyTab[][6] = {
	{15,30,45,60,120,300},
	{20,40,60,80,100,100}
};
void ScreenSaveExit(void)
{
	ITUBackground*bg = ituSceneFindWidget(&theScene, "ScreenSaverBG");
	if (ituWidgetIsVisible(bg)){
		ituWidgetSetVisible(bg, false);
		bit_Label_ScreenSaver = 0;
		ioctl(ITP_DEVICE_BACKLIGHT, ITP_IOCTL_ON, NULL);
	}
	ScreenSaverRefresh();
}

void SS_KeyVolSet(uint8_t sskey,int val)
{
	char buf[40];
	int i;
	/***15s 30s 1min 2min 3min 5min  10min 30min*****/
	printf("%s:sskey=%d,val=%d\n", __func__, sskey, val);
	for (i = 0; i < SCREENSAVER_OPTION_CNT; i++) {
		//ituWidgetSetVisible(SSVolRadioBox[i], false);
		//ituRadioBoxSetChecked(SSVolRadioBox[i], false);
		ituWidgetSetColor(SSVolText[i], 255, 255, 255, 255);
		if (sskey) sprintf(buf, "%d%s", SS_KeyTab[sskey][i], "%");
		else       sprintf(buf, "%d%s", SS_KeyTab[sskey][i],"秒");	
		ituTextSetString(SSVolText[i], buf);
	}
	if (sskey){//keyaudio
		ituWidgetSetVisible(SSVolRadioBox[5], false);
		ituWidgetSetVisible(SSVolText[5], false);
		if (20 == val){
			ituRadioBoxSetChecked(SSVolRadioBox[0], true);
			ituWidgetSetColor(SSVolText[0], 255, 255, 255, 255);
		}
		else if (40 == val){
			ituRadioBoxSetChecked(SSVolRadioBox[1], true);
			ituWidgetSetColor(SSVolText[1], 255, 255, 255, 255);
		}
		else if (60 == val){
			ituRadioBoxSetChecked(SSVolRadioBox[2], true);
			ituWidgetSetColor(SSVolText[2], 255, 255, 255, 255);
		}
		else if (80 == val){
			ituRadioBoxSetChecked(SSVolRadioBox[3], true);
			ituWidgetSetColor(SSVolText[3], 255, 255, 255, 255);
		}
		else if (100 == val){
			ituRadioBoxSetChecked(SSVolRadioBox[4], true);
			ituWidgetSetColor(SSVolText[4], 255, 255, 255, 255);
		}
	}
	else{
		ituWidgetSetVisible(SSVolRadioBox[5], true);
		ituWidgetSetVisible(SSVolText[5], true);
		if (15 == val){
			ituRadioBoxSetChecked(SSVolRadioBox[0], true);
			ituWidgetSetColor(SSVolText[0], 255, 255, 255, 255);
		}
		else if (30 == val){
			ituRadioBoxSetChecked(SSVolRadioBox[1], true);
			ituWidgetSetColor(SSVolText[1], 255, 255, 255, 255);
		}
		else if (45 == val){
			ituRadioBoxSetChecked(SSVolRadioBox[2], true);
			ituWidgetSetColor(SSVolText[2], 255, 255, 255, 255);
		}
		else if (60 == val){
			ituRadioBoxSetChecked(SSVolRadioBox[3], true);
			ituWidgetSetColor(SSVolText[3], 255, 255, 255, 255);
		}
		else if (120 == val){
			ituRadioBoxSetChecked(SSVolRadioBox[4], true);
			ituWidgetSetColor(SSVolText[4], 255, 255, 255, 255);
		}
		else if (300 == val){
			ituRadioBoxSetChecked(SSVolRadioBox[5], true);
			ituWidgetSetColor(SSVolText[5], 255, 255, 255, 255);
		}
	}
}
int SS_KeyVolGet(uint8_t sskey, int val)
{
	int value=0;
	if (sskey){//keyaudio
		if (0 == val)  value = 20;
		else if (1 == val)  value = 40;
		else if (2 == val)  value = 60;
		else if (3 == val)  value = 80;
		else if (4 == val)  value = 100;
	}
	else{
		if (0 == val)  value = 15;
		else if (1 == val)  value = 30;
		else if (2 == val)  value = 45;
		else if (3 == val)  value = 60;
		else if (4 == val)  value = 120;
		else if (5 == val)  value = 300;
	}
	return value;
}
void DelayOptionGet(REV_DELAY_u Delay)
{
	memset(&tConfigFileReserv, 0, sizeof(RESERV_ITEM_t));
	for (int i = 0; i < 3; i++){
		if (Delay.buf[3 + 4 * i]){
			tConfigFileReserv.bEnableBit |= 0x01 << i;
			tConfigFileReserv.bManualTimeSrc[tConfigFileReserv.bManualCnt] = Delay.buf[1 + 4 * i];
			tConfigFileReserv.bManualTimeDst[tConfigFileReserv.bManualCnt] = Delay.buf[2 + 4 * i];
			//tConfigFileReserv.bManualModeID[tConfigFileReserv.bManualCnt]=
			//tConfigFileReserv.bManualTemper[tConfigFileReserv.bManualCnt]=
			tConfigFileReserv.bManualCnt++;
		}
	}
	if (Delay.pkt.uDelayAI.pkt.bEnable){
		tConfigFileReserv.bEnableBit |= 0x10;
		tConfigFileReserv.bSmartTimeSrc = Delay.buf[1 + 4 * 3];
		tConfigFileReserv.bSmartTimeDst = Delay.buf[2 + 4 * 3];
		//tConfigFileReserv.bSmartModeID=
		//tConfigFileReserv.bSmartTemper=
	}
}
#if 0
bool TrackIconShowOnTime(ITUWidget*Iconwidget, int IconwidgetStartXY, int backHWdata, ITUWidget*StopAnywhere, ITUWidget*StopAnywhereBG, bool H_OR_V)
{
	int val;
	bool ret = false;
	static int swapdata = -1;
	if (H_OR_V)// vertical
	{
		swapdata = ituWidgetGetHeight(StopAnywhereBG) - ituWidgetGetHeight(StopAnywhere);
		if (swapdata){
			val = IconwidgetStartXY - ituWidgetGetY(StopAnywhereBG) * (backHWdata - ituWidgetGetHeight(Iconwidget)) / (ituWidgetGetHeight(StopAnywhereBG) - ituWidgetGetHeight(StopAnywhere));
			ituWidgetSetY(Iconwidget, val);
			ret = true;
		}
	}
	else{
		swapdata = ituWidgetGetWidth(StopAnywhereBG) - ituWidgetGetWidth(StopAnywhere);
		if (swapdata){
			val = IconwidgetStartXY - ituWidgetGetX(StopAnywhereBG) * (backHWdata - ituWidgetGetWidth(Iconwidget)) / (ituWidgetGetWidth(StopAnywhereBG) - ituWidgetGetWidth(StopAnywhere));
			ituWidgetSetX(Iconwidget, val);
			ret = true;
		}
	}
	//ret |= TrackIconShowOnTime(IndicatorPosIcon, StepContainerBG, MOVE_ICON_Y_START, BAR_BACKGROUND_HEIGHT, StepStopAnywhere,true);
	//ret |= TrackIconShowOnTime(StepTipsIndicatorPosIcon, StepTipsContainer, 106, BAR_BACKGROUND_HEIGHT, StepTipsStopAnywhere,true);
	return ret;
}
#endif

void ProcessLayerJump(uint8_t layerID, uint8_t reset)
{
	printf("%s:layerID=[%d->%d]\n", __func__, OperatorParam.LayerID, layerID);
	ITULayer*toLayer = NULL;
	switch (layerID){
	case LAYER_POWER_ON:
		toLayer = ituSceneFindWidget(&theScene, "PowerOnLayer");
		ituLayerGoto(toLayer);
		break;
	case LAYER_LOGO:
		toLayer = ituSceneFindWidget(&theScene, "LogoLayer");
		ituLayerGoto(toLayer);
		break;
	case LAYER_MAIN_MENU:
		toLayer = ituSceneFindWidget(&theScene, "MainMenuLayer");
		ituLayerGoto(toLayer);
		break;
	case LAYER_CONFIG:
		toLayer = ituSceneFindWidget(&theScene, "ConfigLayer");
		ituLayerGoto(toLayer);
		break;
	case LAYER_FCT:
		toLayer = ituSceneFindWidget(&theScene, "FCTLayer");
		ituLayerGoto(toLayer);
		break;
	case LAYER_PROCESS:break;
	default:break;
	}
	bit_Reset_Layer = reset;
}
bool MainBoardStatusProc(void)
{
	bool ret = false;
	static int powerback = -1;
	static int Modeback = -1;
	static int WiFiReady = -1;
	uint8_t power = ACUGet_PowerStatus();
	uint8_t mode = mapConvertMiiotAndMachine(CONVERT_IOT_2_MACHINE, IO_CMD_MODE, ACUGet_Mode());
	uint8_t WiFiState = ACUGet_WiFiStatus();
	if (false == tComState.RevEnable) return ret;
	if (powerback != power){
		printf("%s:bPowerOnOff[%d->%d-%d]\n", __func__, powerback, power, uTransmitDev.pkt.bPowerOnOff);
		powerback = power;
		uTransmitDev.pkt.bPowerOnOff = power;
		OperatorParam.PowerStatus = power;
		if ((LAYER_LOGO == OperatorParam.LayerID) && (OperatorParam.PowerStatus)){
			printf("MainBoardPowerStateProc:ProcessLayerJump(LAYER_MAIN_MENU)!!\n");
			ProcessLayerJump(LAYER_MAIN_MENU, 1);
		}
		else if (!OperatorParam.PowerStatus && (OperatorParam.LayerID != LAYER_LOGO)){
			printf("MainBoardPowerStateProc:ProcessLayerJump(LAYER_LOGO)!!\n");
			ScreenSaveExit();
			ProcessLayerJump(LAYER_LOGO, 1);
		}
		ret |= true;
	}
	if (Modeback != mode){
		printf("%s:bMode[%d->%d-%d]\n", __func__, Modeback, mode, uTransmitDev.pkt.bMode);
		Modeback = mode;
		uTransmitDev.pkt.bMode = mapConvertMiiotAndMachine(CONVERT_MACHINE_2_IOT, IO_CMD_MODE, mode);
		OperatorParam.CycleID = mode;
		ret |= true;
	}
	if (WiFiReady != WiFiState){
		printf("%s:bWiFiStatus[%d->%d]\n", __func__, WiFiReady, WiFiState);
		WiFiReady = WiFiState;
		bit_Label_WiFi = WiFiState ? true : false;
		uTransmitDev.pkt.bWiFiStatus = bit_Label_WiFi;
		ret |= true;
	}
	if ((tConfig.Enable_TemperResevOut != uRevDev.pkt.bKeepWarmEnable)||\
		(tConfig.Enable_WaterOutPowerOff != uRevDev.pkt.bPowerBreakEnable)||\
		(tConfig.Enable_Smart != uRevDev.pkt.bBacterEnable) || \
		(tConfig.SmartVal != uRevDev.pkt.bBacterCycle)\
		){
		printf("%s:ConfigOption[%d->%d][%d->%d][%d->%d]\n", __func__, tConfig.Enable_TemperResevOut, uRevDev.pkt.bKeepWarmEnable,\
			tConfig.Enable_WaterOutPowerOff, uRevDev.pkt.bPowerBreakEnable, tConfig.Enable_Smart, uRevDev.pkt.bBacterEnable);
		tConfig.Enable_TemperResevOut = uRevDev.pkt.bKeepWarmEnable ? true : false;
		tConfig.Enable_WaterOutPowerOff = uRevDev.pkt.bPowerBreakEnable ? true : false;
		tConfig.Enable_Smart = uRevDev.pkt.bBacterEnable;
		tConfig.SmartVal = uRevDev.pkt.bBacterCycle;
		/*if (LAYER_CONFIG == OperatorParam.LayerID) ProcessLayerJump(LAYER_CONFIG, 1);
		else                                       wConfigSaveCnt = itpGetTickCount();
		*/
		if (OperatorParam.LayerID != LAYER_CONFIG) wConfigSaveCnt = itpGetTickCount();
		ret |= true;
	}
}
#ifdef ENABLE_RESERV
uint8_t ReservTimeWheelRangeGet(int index)
{
	int i,j, val,id;
	uint8_t TimSrc[24] = { 0 };
	uint8_t itemCnt = 0;
	uint8_t period = 0;
	uint8_t offset = 0;
	for (i = 0; i < 24; i++){
		TimSrc[i] = i;
	}

	for (i = 0; i < tConfigFileReserv.bManualCnt; i++){
		if (index != i){
			for (j = tConfigFileReserv.bManualTimeSrc[i]; j < tConfigFileReserv.bManualTimeDst[i]; j++){
				//printf("i=%d,j=%d\n", i, j);
				TimSrc[j] = 25;
			}
		}
	}
	//printf("TimSrc:");
	for (i = 0; i < 24; i++){
		//printf("[%d]%d ,",i,TimSrc[i]);
		if (TimSrc[i] != 25){
			tReservConfig.TimeRangeTab[itemCnt] = TimSrc[i];	
			itemCnt++;
			//printf("!=%d-%d-%d-%d\n", offset, period, i, TimSrc[i]);
			if (!offset){
				tReservConfig.TimeMinTab[period] = TimSrc[i];
				offset=1;
			}
			tReservConfig.TimeMaxTab[period] = TimSrc[i]+1;
		}
		else{
			//printf("=%d-%d-%d-%d\n", offset, period, i, TimSrc[i]);
			if (offset){
				tReservConfig.TimeMaxTab[period] =i;
				period++;
				offset = 0;
			}
			tReservConfig.TimeMinTab[period] = TimSrc[i];
		}
	}
	//printf("\n");
	if (offset) period++;
	tReservConfig.TimePeriodCnt = period;
	tReservConfig.TimeRangeCnt = itemCnt;
	if (tReservConfig.EditIndex){
		id = tReservConfig.EditIndex - 1;
		tReservTmpItem.bTimeSrc = tConfigFileReserv.bManualTimeSrc[id];
		tReservTmpItem.bTimeDst = tConfigFileReserv.bManualTimeDst[id];
	}
	else{
		tReservTmpItem.bTimeSrc = tReservConfig.TimeMinTab[0];
		tReservTmpItem.bTimeDst = tReservConfig.TimeMaxTab[0];
	}
#if 1
	printf("*******************\n");
	//printf("ReservTimeWheelRangeGet:TimeRangeCnt=%d,bTimeSrc=%d,bTimeDst=%d\n", tReservConfig.TimeRangeCnt, tReservTmpItem.bTimeSrc, tReservTmpItem.bTimeDst);
	for (i = 0; i < itemCnt; i++) printf("%d ", tReservConfig.TimeRangeTab[i]);
	printf("\n");
	//printf("ReservTimeWheelRangeGet:TimePeriodCnt=%d\n", tReservConfig.TimePeriodCnt);
	for (i = 0; i < period; i++) printf("[%d->%d] ", tReservConfig.TimeMinTab[i], tReservConfig.TimeMaxTab[i]);
	printf("\n");
#endif
	return period;
}
int ReserTimeIndexGet(uint8_t val)
{
	int i = 0;
	for (i = 0; i < tReservConfig.TimeRangeCnt; i++){
		if (tReservConfig.TimeRangeTab[i] == val) return i;
	}
	return -1;
}
int ReserTimePeriodGet(uint8_t val)
{
	int i = 0;
	for (i = 0; i < tReservConfig.TimePeriodCnt; i++){
		if ((val >= tReservConfig.TimeMinTab[i]) && (val <= tReservConfig.TimeMaxTab[i])) return i;
	}
	printf("========ERR:%s%d\n", __func__, __LINE__);
	return -1;
}
void ReservParamWheelInit(void)
{
	int i, focus,val;
	static char*ItemStr[60] = { 0 };
	char buf[10];
	uint8_t TimSrc[24] = { 0 };
	uint8_t TimDst[24] = { 0 };
	uint8_t itemCnt = tReservConfig.TimeRangeCnt;
	uint8_t dstVal = 0;
	/************************************************/
	for (i = 0; i < itemCnt; i++){
		sprintf(buf, "%d", tReservConfig.TimeRangeTab[i]);
		ItemStr[i] = malloc(strlen(buf) + 1);
		strcpy(ItemStr[i], buf);
	}
	focus = ReserTimeIndexGet(tReservTmpItem.bTimeSrc);
	ReservTimeFormWheel->itemCount = itemCnt;
	ituWheelSetItemTree(ReservTimeFormWheel, ItemStr, itemCnt);
	if (focus < 0) focus = 0;
	ReservTimeFormWheel->focusIndex = focus;
	for (i = 0; i < itemCnt; i++) free(ItemStr[i]);
	/************************************************/
	itemCnt = 0;
	focus = 0;
	val = ReserTimePeriodGet(tReservTmpItem.bTimeSrc);
	dstVal = tReservConfig.TimeMaxTab[val];
	//tReservTmpItem.bTimeDst = tReservConfig.TimeMaxTab[val];
	printf("%d-%d-%d\n", tReservTmpItem.bTimeSrc, dstVal, tReservTmpItem.bTimeDst);
	for (i = tReservTmpItem.bTimeSrc + 1; i <= dstVal; i++){
		sprintf(buf, "%d", i);
		ItemStr[itemCnt] = malloc(strlen(buf) + 1);
		strcpy(ItemStr[itemCnt], buf);
		if (tReservConfig.EditIndex){
			if (tReservTmpItem.bTimeDst == i) focus = itemCnt;
		}
		itemCnt++;
	}
	if (itemCnt){
		if (itemCnt > 5) ReservTimeToWheel->cycle_tor = 1;
		else             ReservTimeToWheel->cycle_tor = 0;
		ReservTimeToWheel->itemCount = itemCnt;
		ituWheelSetItemTree(ReservTimeToWheel, ItemStr, itemCnt);
		if (tReservConfig.EditIndex <= 0) focus = itemCnt - 1;	
		//if (tReservConfig.EditIndex)
		printf("%d:%d-%d-%d\n", tReservConfig.EditIndex, tReservTmpItem.bTimeSrc, tReservTmpItem.bTimeDst, focus);
		ituWheelGoto(ReservTimeToWheel, focus);
	}
	//ReservTimeToWheel->focusIndex = focus;
	for (i = 0; i < itemCnt; i++) free(ItemStr[i]);
}
void ReservParamWheelLink(uint8_t data)
{
	static char*ItemStr[60] = { 0 };
	char buf[10];
	uint8_t itemCnt=0;
	uint8_t focus = 0;
	uint8_t val = 0;
	int i;
	val = ReserTimePeriodGet(data);
	for (i = data + 1; i <= tReservConfig.TimeMaxTab[val]; i++){
		sprintf(buf, "%d", i);
		ItemStr[itemCnt] = malloc(strlen(buf) + 1);
		strcpy(ItemStr[itemCnt], buf);
		itemCnt++;
	}
	if (itemCnt){
		if (itemCnt > 5) ReservTimeToWheel->cycle_tor = 1;
		else             ReservTimeToWheel->cycle_tor = 0;
		ReservTimeToWheel->itemCount = itemCnt;
		ituWheelSetItemTree(ReservTimeToWheel, ItemStr, itemCnt);
		focus = itemCnt - 1;
	}
	//ReservTimeToWheel->focusIndex = focus;
	ituWheelGoto(ReservTimeToWheel, focus);
	for (i = 0; i < itemCnt; i++){
		//printf("ItemStr[%d]=%s\n", i,ItemStr[i]);
		free(ItemStr[i]);
	}
}
void ReservTimeCycleClr(void)
{
	int i = 0;
	for (i = 0; i < 12; i++) ituWidgetSetVisible(ReservTimePIcon[i], false);
	for (i = 0; i < 24; i++) ituWidgetSetVisible(ReservTimeWIcon[i], false);	
}
void ReservTmpItemSet(RESERV_TEMP_ITEM_t *item, uint8_t mode, uint8_t temper)
{
	item->bModeID = mode;
	item->bTemper = temper;
	printf("ReservTmpItemSet:bModeID=%d,bTemper=%d\n", mode, temper);
}
void ReservEditShow(RESERV_TEMP_ITEM_t *item)
{
	char buf[50];
	sprintf(buf, "%02d:00-%02d:00", item->bTimeSrc, item->bTimeDst);
	ituTextSetString(ReservSetTimeText, buf);
	ituTextSetString(ReservSetCycleText, tCycleParamStr[item->bModeID].Name);
	sprintf(buf, "%d%s", item->bTemper, "℃");
	ituTextSetString(ReservSetTemperText, buf);
	ituWidgetSetVisible(ReservCustomerContainer, ((UI_CYCLE_CUSTOMER == item->bModeID) ? true : false));
	ituWidgetSetVisible(ReservSetMainMaskIcon, ((UI_CYCLE_CUSTOMER == item->bModeID) ? true : false));
}
bool ReservProcessShow(bool enable)
{
	bool ret = false;
	static int mainIndex = -1, subIndex = -1, mainId = -1, mainSelIndex = -1,editIndex=0;
	int i,offset,index = 0;
	uint8_t point = 0;
	uint8_t line = 0;
	uint8_t begin, end = 0, thour=0;
	char buf[40];
	if (enable){
		mainIndex = -1;
		mainId = -1;
		mainSelIndex= -1;
		editIndex = -1;
		subIndex = -1;
		ituCheckBoxSetChecked(ReserTypeCheckBox, false);
		memset(&tReservConfig, 0, sizeof(RESERV_CONFIG_t));

	}

	if ((mainIndex != tReservConfig.MainIndex) || (subIndex != tReservConfig.SubIndex)){
		printf("ReservProcessShow:MainIndex[%d->%d],SubIndex[%d->%d]\n", mainIndex, tReservConfig.MainIndex, subIndex, tReservConfig.SubIndex);
		mainIndex = tReservConfig.MainIndex;
		subIndex = tReservConfig.SubIndex;
		ituSpriteGoto(ReservSetMainSprite, mainIndex);
		ituSpriteGoto(ReservSetSubSprite, subIndex);
		ret |= true;
	}
	if ((mainId != tReservConfig.MainID) ){ //|| (editIndex != tReservConfig.EditIndex)
		printf("ReservProcessShow:MainID[%d->%d]\n", mainId, tReservConfig.MainID);
		mainId = tReservConfig.MainID;
		editIndex = -1;
		ituSpriteGoto(ReserTypeSprite, 0);
		if (RESERV_INIT == mainId){
			mainSelIndex = -1;
			/****************************************************************************/
			for (i = 0; i <RESERV_ITEM_MAX; i++){
				ituSpriteGoto(ReservListEditSprite[i], 0);
				ituSpriteGoto(ReservListSprite[i], 0);
				if (i <tConfigFileReserv.bManualCnt){
					ituWidgetSetVisible(ReservListContainer[i], true);
					if ((tConfigFileReserv.bEnableBit & 1 << i)) ituSpriteGoto(ReservListSprite[i], true);
					else                                         ituSpriteGoto(ReservListSprite[i], false);
					sprintf(buf, "%02d:00-%02d:00", tConfigFileReserv.bManualTimeSrc[i], tConfigFileReserv.bManualTimeDst[i]);
					ituTextSetString(ReservTimeText[i], buf);
					sprintf(buf, "%s", tCycleParamStr[tConfigFileReserv.bManualModeID[i]].Name);
					ituTextSetString(ReservCycleText[i], buf);
				}
				else{
					ituWidgetSetVisible(ReservListContainer[i], false);
				}
			}
			/****************************************************************************/
			if (ReservListEditSprite[0]->frame)
			for (i = 0; i < tConfigFileReserv.bManualCnt; i++) ituSpriteGoto(ReservListEditSprite[i], 0);
			/****************************************************************************/
			ituTextSetString(ReserTipText, "最多支持3段时间预约，不可设置重复时间");
			if (tConfigFileReserv.bManualCnt){
				offset = ituWidgetGetHeight(ReserTypeContainer)*(tConfigFileReserv.bManualCnt);
				ituWidgetSetY(ReserTipContainer, offset);
				ituWidgetSetY(ReservCancelContainer, offset);
				if (tConfigFileReserv.bManualCnt >= 3){
					ituTextSetString(ReserTipText, "预约时间段已达3段，无法添加新预约");
					ituWidgetDisable(ReservAddBtn);
				}
				else{
					ituWidgetEnable(ReservAddBtn);
				}
				ituWidgetEnable(ReservEditBtn);
			}
			else{
				ituWidgetEnable(ReservAddBtn);
				ituWidgetDisable(ReservEditBtn);
			}
			ituSpriteGoto(ReservAddBtnSprite, (ituWidgetIsEnabled(ReservAddBtn) ? 1 : 0));
			ituSpriteGoto(ReservEditBtnSprite, (ituWidgetIsEnabled(ReservEditBtn) ? 1 : 0));
			/****************************************************************************/
			ituWidgetSetVisible(ReserTipContainer, true);
			ituWidgetSetVisible(ReservCancelContainer, false);
			ituWidgetSetVisible(ReservSelMainMaskIcon, false);
			/****************************************************************************/
		}
		else if (RESERV_SET == mainId){
			/****************************************************************************/
			if (!ReservListEditSprite[0]->frame)
			for (i = 0; i < tConfigFileReserv.bManualCnt; i++) ituSpriteGoto(ReservListEditSprite[i], 1);
			ituWidgetSetVisible(ReserTipContainer, false);
			ituWidgetSetVisible(ReservCancelContainer, false);
			ituWidgetSetVisible(ReservSelMainMaskIcon, false);
			/*************************************/
			ituSpriteGoto(ReservAddBtnSprite, 2);////welson for bugsNo 20  and return-save btn for edit page
			ituSpriteGoto(ReservEditBtnSprite, 2);
			ituWidgetEnable(ReservAddBtn);
			ituWidgetEnable(ReservEditBtn);
			/***************************************/
		}
		else if (RESERV_DEL == tReservConfig.MainID){
			/****************************************************************************/
			ituWidgetSetVisible(ReservCancelContainer, true);
			ituWidgetSetY(ReservSelMainMaskIcon, ituWidgetGetHeight(ReserTypeContainer)*(tReservConfig.EditIndex-1));
			ituWidgetSetVisible(ReservSelMainMaskIcon, true);
		}
		else{
			if (RESERV_SMART == tReservConfig.MainID){
				 ituSpriteGoto(ReserTypeSprite, 1);
				 ituSpriteGoto(ReservListSprite[3], (tConfigFileReserv.bEnableBit & 0x10));
				 mainSelIndex = -1;
			}
			/****************************************************************************/
			ituWidgetSetVisible(ReserTipContainer, false);
			ituWidgetSetVisible(ReservCancelContainer, false);
			ituWidgetSetVisible(ReservSelMainMaskIcon, false);
		}
		ret |= true;
	}
	if (mainSelIndex != tConfigFileReserv.bEnableBit){
		/****************************************************************************/
		printf("ReservProcessShow:MainSelIndex[%d->%d]\n", mainSelIndex, tConfigFileReserv.bEnableBit);
		mainSelIndex = tConfigFileReserv.bEnableBit;
		//if (mainSelIndex)
		{
			ReservTimeCycleClr();
			if (!ReserTypeSprite->frame){//手动预约
				if (tConfigFileReserv.bManualCnt){
					for (i = 0; i < RESERV_ITEM_MAX; i++){
						if (tConfigFileReserv.bEnableBit & (1 << i)){
							begin = tConfigFileReserv.bManualTimeSrc[i];
							end = tConfigFileReserv.bManualTimeDst[i];
							thour += (end - begin);
							printf("%d:%d-%d,%d\n", i, begin, end, thour);
							for (point = (begin % 2 + begin / 2); point <= (end / 2); point++)
								ituWidgetSetVisible(ReservTimePIcon[point % 12], true);
							for (line = begin; line < end; line++)
								ituWidgetSetVisible(ReservTimeWIcon[line], true);
							ituWidgetSetColor(ReservTimeText[i], 255, 255, 255, 255);
							ituWidgetSetColor(ReservCycleText[i], 255, 255, 255, 255);
						}
						else{
							ituWidgetSetColor(ReservTimeText[i], 255, 153, 153, 153);
							ituWidgetSetColor(ReservCycleText[i], 255, 153, 153, 153);
						}
					}
					if (thour){
						sprintf(buf, "%d%s", thour, "小时");
						ituTextSetString(ReservTimeCycleText, buf);
						ituWidgetSetColor(ReservTimeCycleText, 255, 255, 255, 255);
					}
					else{
						ituTextSetString(ReservTimeCycleText, "未设置");
						ituWidgetSetColor(ReservTimeCycleText, 255, 153, 153, 153);
					}
				}
				else{
					ituTextSetString(ReservTimeCycleText, "未设置");
					ituWidgetSetColor(ReservTimeCycleText, 255, 153, 153, 153);
				}
			}
			else{
				if (tConfigFileReserv.bEnableBit & (1 << 3)){
					begin = tConfigFileReserv.bSmartTimeSrc;
					end = tConfigFileReserv.bSmartTimeDst;
					thour += (end - begin);
					printf("%d:%d-%d,%d\n", 4, begin, end, thour);
					for (point = (begin % 2 + begin / 2); point <= (end / 2); point++)
						ituWidgetSetVisible(ReservTimePIcon[point % 12], true);
					for (line = begin; line < end; line++)
						ituWidgetSetVisible(ReservTimeWIcon[line], true);
					sprintf(buf, "%d%s", thour, "小时");
					ituTextSetString(ReservTimeCycleText, buf);
					ituWidgetSetColor(ReservTimeCycleText, 255, 255, 255, 255);
				}
				else{
					ituTextSetString(ReservTimeCycleText, "未设置");
					ituWidgetSetColor(ReservTimeCycleText, 255, 153, 153, 153);
				}
			}
		}
		/****************************************************************************/
		ret |= true;
	}
	//if (!ReservSetMainSprite->frame) tReservConfig.EditIndex = -1;
	//printf("ReservProcessShow:EditIndex[%d->%d]\n",editIndex, tReservConfig.EditIndex);
	if ((editIndex != tReservConfig.EditIndex)){
		/****************************************************************************/
		printf("ReservProcessShow:EditIndex[%d->%d]\n", editIndex, tReservConfig.EditIndex);
		editIndex = tReservConfig.EditIndex;
		if (tReservConfig.EditIndex >= 0){
			if (tReservConfig.EditIndex){
				index = tReservConfig.EditIndex - 1;
				ReservTmpItemSet(&tReservTmpItem, tConfigFileReserv.bManualModeID[index], tConfigFileReserv.bManualTemper[index]);
			}
			else{
				ReservTmpItemSet(&tReservTmpItem, RESERV_DEFALT_MODE_ID, atoi(tCycleParamStr[RESERV_DEFALT_MODE_ID].Temper));
			}
			//ReservTmpItemSet(&tReservTmpItem, UI_CYCLE_SINGLE,0);
			ReservEditShow(&tReservTmpItem);
			if (ituWidgetIsVisible(ReservCancelContainer)){
				ituWidgetSetY(ReservSelMainMaskIcon, ituWidgetGetHeight(ReserTypeContainer)*(tReservConfig.EditIndex - 1));
				ituWidgetSetVisible(ReservSelMainMaskIcon, true);
			}
		}
		/****************************************************************************/
		ret |= true;
	}
	return ret;
}
#endif
void LocalDateTimeInit(uint8_t type)
{
	struct timeval tv;
	struct timezone tzone;
	struct tm* tm;
    time_t timeshow;
	int i, itemCnt,focus;
	static char*ItemStr[60] = {0};
	char buf[10];
	//int tBaseTime.year, tBaseTime.month, tBaseTime.day,tBaseTime.hour,tBaseTime.min;
	gettimeofday(&tv, NULL);
	tm = localtime(&tv.tv_sec);
	tBaseTime.year = tBaseSetTime.year = tm->tm_year + 1900;
	tBaseTime.month = tBaseSetTime.month = tm->tm_mon + 1;
	tBaseTime.day = tBaseSetTime.day = tm->tm_mday;
	tBaseTime.hour = tBaseSetTime.hour = tm->tm_hour;
	tBaseTime.min = tBaseSetTime.min = tm->tm_min;
	tBaseTime.sec = tBaseSetTime.sec = tm->tm_sec;
    timeshow=tv.tv_sec;
	printf("Local Time Init:%s\n",ctime(&timeshow));
	if (!type){//INFO_ID_DATE
		/************************************************/
		itemCnt = 12;
		for (i = 0; i < itemCnt; i++){
			sprintf(buf, "%02d", i + 2019);
			ItemStr[i] = malloc(strlen(buf) + 1);
			strcpy(ItemStr[i], buf);
		}
		focus = tBaseTime.year - 2019;
		LocalYearWheel->itemCount = itemCnt;
		ituWheelSetItemTree(LocalYearWheel, ItemStr, itemCnt);
		ituWheelGoto(LocalYearWheel, focus);
		for (i = 0; i < itemCnt; i++) free(ItemStr[i]);
		/************************************************/
		itemCnt = 12;
		for (i = 0; i < itemCnt; i++){
			sprintf(buf, "%02d", i + 1);
			ItemStr[i] = malloc(strlen(buf) + 1);
			strcpy(ItemStr[i], buf);
		}
		focus = tBaseTime.month-1;
		LocalMonthWheel->itemCount = itemCnt;
		ituWheelSetItemTree(LocalMonthWheel, ItemStr, itemCnt);
		ituWheelGoto(LocalMonthWheel, focus);
		for (i = 0; i < itemCnt; i++) free(ItemStr[i]);
		/************************************************/	
		if ((1 == tBaseTime.month) || (3 == tBaseTime.month) || (5 == tBaseTime.month) || (7 == tBaseTime.month) || (8 == tBaseTime.month) || (10 == tBaseTime.month) || (12 == tBaseTime.month)) itemCnt = 31;
		else if ((4 == tBaseTime.month) || (6 == tBaseTime.month) || (9 == tBaseTime.month) || (11 == tBaseTime.month))  itemCnt = 30;
		else {
			if (tBaseTime.year % 4) itemCnt = 28; //平年
			else              itemCnt = 29; //闰年
		}
		for (i = 0; i < itemCnt; i++){
			sprintf(buf, "%02d", i + 1);
			ItemStr[i] = malloc(strlen(buf) + 1);
			strcpy(ItemStr[i], buf);
		}
		focus = tBaseTime.day-1;
		LocalDayWheel->itemCount = itemCnt;
		ituWheelSetItemTree(LocalDayWheel, ItemStr, itemCnt);
		ituWheelGoto(LocalDayWheel, focus);
		for (i = 0; i < itemCnt; i++) free(ItemStr[i]);
	}
	else {//INFO_ID_TIME
		/************************************************/
		itemCnt = 24;
		for (i = 0; i < itemCnt; i++){
			sprintf(buf, "%02d", i + 1);
			ItemStr[i] = malloc(strlen(buf) + 1);
			strcpy(ItemStr[i], buf);
		}
		focus = tBaseTime.hour - 1;
		LocalHourWheel->itemCount = itemCnt;
		ituWheelSetItemTree(LocalHourWheel, ItemStr, itemCnt);
		ituWheelGoto(LocalHourWheel, focus);
		for (i = 0; i < itemCnt; i++) free(ItemStr[i]);
		/************************************************/
		itemCnt = 60;
		for (i = 0; i < itemCnt; i++){
			sprintf(buf, "%02d", i);
			ItemStr[i] = malloc(strlen(buf) + 1);
			strcpy(ItemStr[i], buf);
		}
		focus = tBaseTime.min;
		LocalMinWheel->itemCount = itemCnt;
		ituWheelSetItemTree(LocalMinWheel, ItemStr, itemCnt);
		ituWheelGoto(LocalMinWheel,focus);
		for (i = 0; i < itemCnt; i++) free(ItemStr[i]);
	}
}
bool LocalTimeSyc(void)
{
	struct timeval tv;
	struct tm *tm, mytime;
	struct timezone tzone;
	static int MinBack = -1;
	time_t timeshow;
	bool ret = false;
	gettimeofday(&tv, NULL);
	tm = localtime(&tv.tv_sec);
	memcpy(&mytime, tm, sizeof(struct tm));
	tzone.tz_minuteswest = 8 * 60;
	tzone.tz_dsttime = 0;
	timeshow = tv.tv_sec;
	//if (!tComState.RevEnable) return ret;
	tTimeBack.year = uRevDev.pkt.bYear;// (uint16_t)(uRevDev.pkt.bYear[0] << 8 | uRevDev.pkt.bYear[1]);
	tTimeBack.month = uRevDev.pkt.bMonth;
	tTimeBack.day =  uRevDev.pkt.bDay;
	tTimeBack.hour = uRevDev.pkt.bHour; //1900
	tTimeBack.min = uRevDev.pkt.bMin;
	tTimeBack.sec = uRevDev.pkt.bSec;
	/*if (((mytime.tm_year +0 ) != tTimeBack.year) || ((mytime.tm_mon + 1) != tTimeBack.month) || (mytime.tm_mday != tBaseSetTime.day) || \
		(mytime.tm_hour != tTimeBack.hour) || (mytime.tm_min != tBaseSetTime.min)){*/
	//if (mytime.tm_min != tTimeBack.min){
	if (MinBack != tTimeBack.min){
		MinBack = tTimeBack.min;
		mytime.tm_year = tTimeBack.year - 0;
		mytime.tm_mon  =  tTimeBack.month - 1;
		mytime.tm_mday = tTimeBack.day;
		mytime.tm_hour = tTimeBack.hour;// clockHourWheel->focusIndex;
		mytime.tm_min  =  tTimeBack.min;// clockMinuteWheel->focusIndex;
		mytime.tm_sec = tTimeBack.sec;
		tv.tv_sec = mktime(&mytime);
		tv.tv_usec = 0;
		settimeofday(&tv, &tzone);
		timeshow = tv.tv_sec;
		printf("Sync Time Set:%s\n", ctime(&timeshow));
		ret = true;
	}
	return ret;
}
void LocalDateTimeSet(uint8_t type)
{
	struct timeval tv;
	struct tm *tm, mytime;
	struct timezone tzone;
    time_t timeshow;
	bool ret = false;
	uint8_t buf[3] = { 0 };
	gettimeofday(&tv, NULL);
	tm = localtime(&tv.tv_sec);
	memcpy(&mytime, tm, sizeof(struct tm));
	tzone.tz_minuteswest = 8 * 60;
	tzone.tz_dsttime = 0;
    timeshow =tv.tv_sec;
    printf("Local Time Set:%s\n",ctime(&timeshow));
	if (!type){//INFO_ID_DATE
		if ((tBaseTime.year != tBaseSetTime.year) || (tBaseTime.month != tBaseSetTime.month) || (tBaseTime.day != tBaseSetTime.day)){
			mytime.tm_year = tBaseSetTime.year - 1900;
			mytime.tm_mon = tBaseSetTime.month - 1;
			mytime.tm_mday = tBaseSetTime.day;
#ifdef ENABLE_MODBUS
			buf[0] = tBaseSetTime.year - 1900;
			buf[1] = tBaseSetTime.month;
			buf[2] = tBaseSetTime.day;
			ProtocolModBus_Send(SET_DATA, REG_ADDRESS_YEAR, 3, buf);
#endif
			ret = true;
		}
	}
	else{//INFO_ID_TIME
		if ((tBaseTime.hour != tBaseSetTime.hour) || (tBaseTime.min != tBaseSetTime.min)){
			mytime.tm_hour = tBaseSetTime.hour;// clockHourWheel->focusIndex;
			mytime.tm_min = tBaseSetTime.min;// clockMinuteWheel->focusIndex;
#ifdef ENABLE_MODBUS
			buf[0] = tBaseSetTime.hour;
			buf[1] = tBaseSetTime.min;
			buf[2] = tBaseSetTime.sec;
			ProtocolModBus_Send(SET_DATA, REG_ADDRESS_HOUR, 3, buf);
#endif
			ret = true;
		}
	}
	if (ret){
		tv.tv_sec = mktime(&mytime);
		tv.tv_usec = 0;
		settimeofday(&tv, &tzone);
        timeshow =tv.tv_sec;
        printf("Dst Time Set:%s\n",ctime(&timeshow));
	}
}
bool ProcessFilterShow(bool init)
{
	bool ret = false;
	char buf[100];
	static int FilterVal[2] = { 0 };
	if (init){
		FilterVal[0] = -1;
		FilterVal[1] = -1;
	}
	if ((FilterVal[0] != uRevDev.pkt.bFilterLife[0]) || (FilterVal[1] != uRevDev.pkt.bFilterLife[1])){
		ituTextSetString(AIFilterValText[0], " ");
		ituTextSetString(AIFilterValText[1], " ");
		FilterVal[0] = uRevDev.pkt.bFilterLife[0];
		FilterVal[1] = uRevDev.pkt.bFilterLife[1];
		sprintf(buf, "%s  %d%s", "AI美肤阻垢滤芯", FilterVal[0], "%");
		ituTextSetString(AIFilterValText[0], buf);
		sprintf(buf, "%s  %d%s", "AI美肤除氯滤芯", FilterVal[1], "%");
		ituTextSetString(AIFilterValText[1], buf);
		ret = true;
	}
	return ret;
}
bool ProcessShow(uint8_t Process_ID, uint8_t Info_ID)
{
	bool ret = false;
	static int ProcessID = PROCESS_ID_NULL;
	static int InfoID = -1;
	char buf[100];
	if ((ProcessID != Process_ID) || (InfoID != Info_ID)){
		printf("%s:Process:%d->%d,Info:%d->%d,\n", __func__, ProcessID, Process_ID, InfoID, Info_ID);
		ProcessID = Process_ID;
		InfoID = Info_ID;
		if (ProcessID){
			if (PROCESS_ID_SET_CYCLE == ProcessID) ituSpriteGoto(ProcessWBSprite, WB_ID_SET_CYCLE);
			else if (PROCESS_ID_RESERV == ProcessID)ituSpriteGoto(ProcessWBSprite, WB_ID_RESERV);
			else                                   ituSpriteGoto(ProcessWBSprite, WB_ID_MIX);
		}
		else{
			ituSpriteGoto(ProcessWBSprite, WB_ID_NULL);
			AudioKeySound(SOUND_FUNCTION_OUT);
		}
			
		if (PROCESS_ID_SET_CYCLE == ProcessID){
			ituTrackBarSetValue(CustomCycleTrackBar, tCycleParamTab[OperatorParam.CycleID].bTemperVal);
			ituProgressBarSetValue(CustomCycleProgressBar, tCycleParamTab[OperatorParam.CycleID].bTemperVal);
			//CustomCycleProgressBar->min = 31;
			//CustomCycleProgressBar->max = 79;
			//CustomCycleTrackBar->gap = 64;
			ituWidgetSetX(TrackBarText, ituWidgetGetX(TrackBarBtn));
			ituSpriteGoto(PopMenuSprite, 0);
		}
		else if (PROCESS_ID_DATE_TIME == ProcessID){
			ituSpriteGoto(ProcessDateTimeSprite, InfoID);
			ituSpriteGoto(ProcessTimeUniteSprite, InfoID);
			if (InfoID)ituTextSetString(ProcessDateTimeText, "时间设置");
			else       ituTextSetString(ProcessDateTimeText, "日期设置");
			LocalDateTimeInit(InfoID);
		}
		else if (PROCESS_ID_SCREEN == ProcessID){
			ituWheelGoto(BrightnessWheel, tConfig.BrightnessVal - 1);
			bit_Label_ParamOnce = 1;
		}
		else if (PROCESS_ID_SS_VOL == ProcessID){
			if (InfoID) SS_KeyVolSet(InfoID, tConfig.KeyVolumeVal);
			else     	SS_KeyVolSet(InfoID, tConfig.ScreenSaverTimeSec);
		}
		else if (PROCESS_ID_AI_FILTER == ProcessID){
			ituSpriteGoto(PopMenuSprite, 1);
		}
		else if (PROCESS_ID_RESERV == ProcessID){
			ituSpriteGoto(PopMenuSprite, 2);
			ReservProcessShow(true);
			//bit_Label_ReservSetUpdate = 1;
		}
		else if (PROCESS_ID_ERR == ProcessID){
		}
		else if (PROCESS_ID_SMART == ProcessID){
			if (1 == tConfig.SmartVal) ituWheelGoto(SmartWheel, 0);
			else if (2 == tConfig.SmartVal) ituWheelGoto(SmartWheel, 1);
			else if (3 == tConfig.SmartVal) ituWheelGoto(SmartWheel, 2);
			else if (4 == tConfig.SmartVal) ituWheelGoto(SmartWheel, 3);
		}			
		ituWidgetSetVisible(AIResetConfirmBG, false);
		/*******************************************************************************************************************/
		if ((PROCESS_ID_SET_CYCLE == ProcessID) || (PROCESS_ID_AI_FILTER == ProcessID) || (PROCESS_ID_RESERV == ProcessID)){
			ituCoverFlowNext(CycleSetCoverFlow);
		}
		/*******************************************************************************************************************/
		ituSpriteGoto(ProcessShowSprite, ProcessID);

		OperatorParam.ProcessID = Process_ID;
		OperatorParam.InfoID = Info_ID;
		ret = true;
	}
	return ret;
}
bool ProcessCycleSync(uint8_t id)
{
	bool ret = false;
	static int backid = -1;
	if (backid != id){
		backid = id;
		for (int i = 0; i < UI_CYCLE_MAX; i++){
			if (i < UI_CYCLE_CUSTOMER) ituWidgetSetColor(ProcessCycleSetText[i], 255, 153, 153, 153);
			ituSpriteGoto(ProcessCycleSetSprite[i], 0);	 
		}
		if (OperatorParam.CycleID < UI_CYCLE_CUSTOMER) ituWidgetSetColor(ProcessCycleSetText[OperatorParam.CycleID], 255, 255, 255, 255);
		ituSpriteGoto(ProcessCycleSetSprite[OperatorParam.CycleID], 1);
		ret = true;
	}
	return ret;
}
bool ProcessMCUUpgradeChk(void)
{

	bool ret = false;
	static uint8_t percent = 0;
	char buf[100];
	if (ACUChk_MCUUpgrade()){
		bit_Label_Upgrade = 1;
		ScreenSaveExit();
		if (!ituWidgetIsVisible(UpgradeBG)){
			ituWidgetSetVisible(UpgradeBG, true);
			ituTextSetString(UpgradeText, " ");
			percent = 0;
			ret = true;
		}
		else{
			if (percent != ACUGet_UpgradeStep()){
				printf("ProcessMCUUpgradeChk:%d->%d\n", percent, ACUGet_UpgradeStep());
				percent = ACUGet_UpgradeStep();
				if (percent<100) sprintf(buf, "更新已完成 %d%s", percent,"%");
				else             sprintf(buf, "更新已完成,请等待数据刷新!\n");
				ituTextSetString(UpgradeText, buf);
				ret = true;
			}
		}
	}
	else{
		bit_Label_Upgrade = 0;
		if (ituWidgetIsVisible(UpgradeBG)){
			ituWidgetSetVisible(UpgradeBG, false);
			ret = true;
		}
	}
	return ret;
}
void CycleParamInit(void)
{
	int i = 0;
	char buf[50];
	for (i = 0; i < UI_CYCLE_MAX; i++){
		if (i < UI_CYCLE_CUSTOMER){
			sprintf(buf, "%s %s %s", tCycleParamStr[i].Temper, tCycleParamStr[i].Capacity, tCycleParamStr[i].Power);
			ituTextSetString(ProcessCycleSetText[i], buf);
		}
		tCycleParamTab[i].bTemperVal = atoi(tCycleParamStr[i].Temper);
		tCycleParamTab[i].bPowerVal  = atoi(tCycleParamStr[i].Power);
		if (!strcmp(tCycleParamStr[i].Capacity, "单胆")) tCycleParamTab[i].bCapacityVal = 0;
		else                                             tCycleParamTab[i].bCapacityVal = 1;
		//printf("Cycle[%d]:%d-%d-%d\n", i,tCycleParamTab[i].bTemperVal, tCycleParamTab[i].bCapacityVal, tCycleParamTab[i].bPowerVal);
	}
}
bool ProcessLayerOnEnter(ITUWidget* widget, char* param)
{
	int i = 0;
	char buf[100];
	if (!ProcessLayer){
		ProcessLayer = ituSceneFindWidget(&theScene, "ProcessLayer");
		ProcessLayerBG = ituSceneFindWidget(&theScene, "ProcessLayerBG");
		AIResetConfirmBG = ituSceneFindWidget(&theScene, "AIResetConfirmBG");
		ScreenSaverBG = ituSceneFindWidget(&theScene, "ScreenSaverBG");
		ProcessShowSprite = ituSceneFindWidget(&theScene, "ProcessShowSprite");
		ProcessWBSprite = ituSceneFindWidget(&theScene, "ProcessWBSprite");
		CustomCycleProgressBar = ituSceneFindWidget(&theScene, "CustomCycleProgressBar");
		CustomCycleTrackBar = ituSceneFindWidget(&theScene, "CustomCycleTrackBar");
		TrackBarBtn = ituSceneFindWidget(&theScene, "TrackBarBtn");
		TrackBarText = ituSceneFindWidget(&theScene, "TrackBarText");
		CustomCycleTemperIcon = ituSceneFindWidget(&theScene, "CustomCycleTemperIcon");
		ProcessDateTimeSprite = ituSceneFindWidget(&theScene, "ProcessDateTimeSprite");
		ProcessTimeUniteSprite = ituSceneFindWidget(&theScene, "ProcessTimeUniteSprite");
		ProcessDateTimeText = ituSceneFindWidget(&theScene, "ProcessDateTimeText");
		BrightnessWheel = ituSceneFindWidget(&theScene, "BrightnessWheel");
		BrightnessTrackBar = ituSceneFindWidget(&theScene, "BrightnessTrackBar");
		BrightnessBtn = ituSceneFindWidget(&theScene, "BrightnessBtn");
		LocalYearWheel = ituSceneFindWidget(&theScene, "LocalYearWheel");
		LocalMonthWheel = ituSceneFindWidget(&theScene, "LocalMonthWheel");
		LocalDayWheel = ituSceneFindWidget(&theScene, "LocalDayWheel");
		LocalHourWheel = ituSceneFindWidget(&theScene, "LocalHourWheel");
		LocalMinWheel = ituSceneFindWidget(&theScene, "LocalMinWheel");
		SmartWheel = ituSceneFindWidget(&theScene, "SmartWheel");
		PopMenuSprite = ituSceneFindWidget(&theScene, "PopMenuSprite");
		CycleSetCoverFlow = ituSceneFindWidget(&theScene, "CycleSetCoverFlow");
#ifdef ENABLE_MCUPGRADE
		UpgradeBG = ituSceneFindWidget(&theScene, "UpgradeBG");
		UpgradeAni = ituSceneFindWidget(&theScene, "UpgradeAni");
		UpgradeText = ituSceneFindWidget(&theScene, "UpgradeText");
		UpgradeAni->delay = 10;
#endif
#ifdef ENABLE_RESERV
		ReserTypeContainer = ituSceneFindWidget(&theScene, "ReserTypeContainer");
		ReserTypeCheckBox = ituSceneFindWidget(&theScene, "ReserTypeCheckBox");
		ReserTypeSprite = ituSceneFindWidget(&theScene, "ReserTypeSprite");
		ReserTipContainer = ituSceneFindWidget(&theScene, "ReserTipContainer");
		ReservCustomerContainer = ituSceneFindWidget(&theScene, "ReservCustomerContainer");
		ReservCancelContainer = ituSceneFindWidget(&theScene, "ReservCancelContainer");
		ReservSetMainSprite = ituSceneFindWidget(&theScene, "ReservSetMainSprite");
		ReservSetSubSprite = ituSceneFindWidget(&theScene, "ReservSetSubSprite");
		ReservParamSetSprite = ituSceneFindWidget(&theScene, "ReservParamSetSprite");
		ReserTipText = ituSceneFindWidget(&theScene, "ReserTipText");
		ReservEditTitleText = ituSceneFindWidget(&theScene, "ReservEditTitleText");
		ReservSetTimeText = ituSceneFindWidget(&theScene, "ReservSetTimeText");
		ReservSetCycleText = ituSceneFindWidget(&theScene, "ReservSetCycleText");
		ReservTimeCycleText = ituSceneFindWidget(&theScene, "ReservTimeCycleText");
		ReservSetTemperText = ituSceneFindWidget(&theScene, "ReservSetTemperText");
		ReservParamSetText = ituSceneFindWidget(&theScene, "ReservParamSetText");
		ReservSelMainMaskIcon = ituSceneFindWidget(&theScene, "ReservSelMainMaskIcon");
		ReservSetMainMaskIcon = ituSceneFindWidget(&theScene, "ReservSetMainMaskIcon");
		ReservTimeFormWheel = ituSceneFindWidget(&theScene, "ReservTimeFormWheel");
		ReservTimeToWheel = ituSceneFindWidget(&theScene, "ReservTimeToWheel");		
		ReservTemperWheel = ituSceneFindWidget(&theScene, "ReservTemperWheel");
		ReservTemperUnitIcon = ituSceneFindWidget(&theScene, "ReservTemperUnitIcon");
		ReservAddBtn = ituSceneFindWidget(&theScene, "ReservAddBtn");
		ReservEditBtn = ituSceneFindWidget(&theScene, "ReservEditBtn");
		ReservAddBtnSprite = ituSceneFindWidget(&theScene, "ReservAddBtnSprite");
		ReservEditBtnSprite = ituSceneFindWidget(&theScene, "ReservEditBtnSprite");
		for (i = 0; i < 4; i++){
			sprintf(buf, "ReservListSprite%d", i);
			ReservListSprite[i] = ituSceneFindWidget(&theScene, buf);
		}
		for (i = 0; i < 3; i++){
			sprintf(buf, "ReservListContainer%d", i);
			ReservListContainer[i] = ituSceneFindWidget(&theScene, buf);
			sprintf(buf, "ReservListEditSprite%d", i);
			ReservListEditSprite[i] = ituSceneFindWidget(&theScene, buf);
			sprintf(buf, "ReservTimeText%d", i);
			ReservTimeText[i] = ituSceneFindWidget(&theScene, buf);
			sprintf(buf, "ReservCycleText%d", i);
			ReservCycleText[i] = ituSceneFindWidget(&theScene, buf);
		}
		for (i = 0; i < 12; i++){
			sprintf(buf, "ReservTimePIcon%d", i);
			ReservTimePIcon[i] = ituSceneFindWidget(&theScene, buf);
		}
		for (i = 0; i < 24; i++){
			sprintf(buf, "ReservTimeWIcon%d", i);
			ReservTimeWIcon[i] = ituSceneFindWidget(&theScene, buf);
		}
		
		for (i = 0; i < UI_CYCLE_MAX; i++){
			sprintf(buf, "ReservModeSelRadioBox%d", i);
			ReservModeSelRadioBox[i]= ituSceneFindWidget(&theScene, buf);
			sprintf(buf, "ReservModeSelText%d", i);
			ReservModeSelText[i] = ituSceneFindWidget(&theScene, buf);
		}
#endif
#ifdef ENABLE_ERR
		ProcessTopInfoBG = ituSceneFindWidget(&theScene, "ProcessTopInfoBG");
		InfoErrCntBG = ituSceneFindWidget(&theScene, "InfoErrCntBG");
		ProcessErrTypeSprite = ituSceneFindWidget(&theScene, "ProcessErrTypeSprite");
		ErrCntText = ituSceneFindWidget(&theScene, "ErrCntText");
		TopErrText = ituSceneFindWidget(&theScene, "TopErrText");
		for (i = 0; i < 2; i++){
			sprintf(buf, "ProcessErrDirBtnBG%d", i);
			ProcessErrDirBtnBG[i] = ituSceneFindWidget(&theScene, buf);
		}
		for (i = 0; i < 3; i++){
			sprintf(buf, "ErrText%d", i);
			ErrText[i] = ituSceneFindWidget(&theScene, buf);
		}
#endif
		for (i = 0; i < UI_CYCLE_MAX; i++){
			sprintf(buf, "ProcessCycleSetSprite%d", i);
			ProcessCycleSetSprite[i] = ituSceneFindWidget(&theScene, buf);
		}
		for (i = 0; i < UI_CYCLE_CUSTOMER; i++){
			sprintf(buf, "ProcessCycleSetText%d", i);
			ProcessCycleSetText[i] = ituSceneFindWidget(&theScene, buf);
		}
		for (i = 0; i < 2; i++){
			sprintf(buf, "AIFilterValText%d", i);
			AIFilterValText[i] = ituSceneFindWidget(&theScene, buf);
		}
		for (i = 0; i < 6; i++){
			sprintf(buf, "SSVolRadioBox%d", i);
			SSVolRadioBox[i] = ituSceneFindWidget(&theScene, buf);
			sprintf(buf, "SSVolText%d", i);
			SSVolText[i] = ituSceneFindWidget(&theScene, buf);
		}		
	}
#ifdef ENABLE_MCUPGRADE
	ituWidgetSetVisible(UpgradeBG, false);
#endif
	ituWidgetSetVisible(ScreenSaverBG, false);
	CycleParamInit();
	ProcessFilterShow(true);
	wProcessCnt = itpGetTickCount();
	printf("%s\n", __func__);
    return true;
}
bool ProcessLayerOnTimer(ITUWidget* widget, char* param)
{
	bool ret = false;
#ifdef _WIN32
	if (LAYER_LOGO == OperatorParam.LayerID) uRevDev.pkt.bUpgrade |= 0x80;
	if ((itpGetTickDuration(wProcessCnt) >= 1 * 100) && ACUChk_MCUUpgrade()){
		if (uRevDev.pkt.bUpgrade<0xFF)uRevDev.pkt.bUpgrade++;
		wProcessCnt = itpGetTickCount();
	}
#endif
#if 1
	char buf[40];
	/**************************************************************************/
	if (bit_Label_ScreenSaver){
		if (ituWidgetIsEnabled(ProcessLayerBG)) ituWidgetDisable(ProcessLayerBG);
	}
	else{
		if (!ituWidgetIsEnabled(ProcessLayerBG)) ituWidgetEnable(ProcessLayerBG);
	}
	if (((tConfig.Enable_TemperResevOut != bit_Config_TemperResevOut)\
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
		&& (itpGetTickDuration(wConfigSaveCnt) >= CONFIG_SAVE_TIME)){
		ConfigSave();
	}
	/**************************************************************************/
#endif
	ret |= LocalTimeSyc();

	ret |= ProcessCycleSync(OperatorParam.CycleID);
#if 1
	ret |= ProcessFilterShow(false);
	if (PROCESS_ID_RESERV==OperatorParam.ProcessID)
	ret |= ReservProcessShow(false);
	ret |= MainBoardStatusProc();
#ifdef ENABLE_MCUPGRADE
	ret |= ProcessMCUUpgradeChk();
#endif
	if (bit_Label_ReservSetUpdate){
		bit_Label_ReservSetUpdate = 0;
	}
#endif
	//printf("Idle=%d\n", LocalHourWheel->idle);
	return ret;
}
bool ProcessWBBtnOnPress(ITUWidget* widget, char* param)
{
	if ((PROCESS_ID_SET_CYCLE == OperatorParam.ProcessID) || (PROCESS_ID_AI_FILTER == OperatorParam.ProcessID) || (PROCESS_ID_RESERV == OperatorParam.ProcessID))
		ituCoverFlowPrev(CycleSetCoverFlow);
	else  
		ProcessShow(PROCESS_ID_NULL, INFO_ID_NULL);
	//AudioKeySound(SOUND_CLICK2);
	printf("%s\n", __func__);
	return true;
}
bool ProcessCycleSetBtnOnPress(ITUWidget* widget, char* param)
{
	int val = atoi(param);
	OperatorParam.CycleID = val;
#ifdef ENABLE_MODBUS
	uint8_t buf[2] = { 0 };
	uint8_t Temperbuf[4] = { 0 };
	buf[0] = mapConvertMiiotAndMachine(CONVERT_MACHINE_2_IOT, IO_CMD_MODE, OperatorParam.CycleID);
	ProtocolModBus_Send(SET_DATA, REG_ADDRESS_MODE, 1, buf);
	if (UI_CYCLE_CUSTOMER == OperatorParam.CycleID){
#if 0
		Float_To_NByte((float)(CustomCycleTrackBar->value*1.0), Temperbuf);
		ProtocolModBus_Send(SET_DATA, REG_ADDRESS_TARGET_TEMPER, 4, Temperbuf);
#else
		Temperbuf[0] = CustomCycleTrackBar->value;
		//ProtocolModBus_Send(SET_DATA, REG_ADDRESS_TARGET_TEMPER, 1, Temperbuf);
#endif
	}		
#endif
	else Temperbuf[0] = atoi(tCycleParamStr[OperatorParam.CycleID].Temper);
	ProtocolModBus_Send(SET_DATA, REG_ADDRESS_TARGET_TEMPER, 1, Temperbuf);
	AudioKeySound(SOUND_CLICK2);
	printf("ProcessCycleSetBtnOnPress:%d\n", val);
    return true;
}
bool CustomCycleTrackBarOnChange(ITUWidget* widget, char* param)
{
	int val = atoi(param);
	if (tCycleParamTab[UI_CYCLE_CUSTOMER].bTemperVal != val)
	{
		ituWidgetSetX(TrackBarText, ituWidgetGetX(TrackBarBtn));
#ifdef ENABLE_MODBUS
		uint8_t buf[4] = { 0 };
#if 0
		Float_To_NByte((float)(val*1.0), buf);
		ProtocolModBus_Send(SET_DATA, REG_ADDRESS_TARGET_TEMPER, 4, buf);
#else
		buf[0] = val;
		ProtocolModBus_Send(SET_DATA, REG_ADDRESS_TARGET_TEMPER, 1, buf);
#endif
#endif
		tCycleParamTab[UI_CYCLE_CUSTOMER].bTemperVal = val;
		printf("%s:val=%d\n", __func__, val);
		return true;
	}
	return false;
}
bool AIFilterRstBtnOnPress(ITUWidget* widget, char* param)
{
	int val = atoi(param);
	uint8_t buf[4] = { 0 };
#ifdef ENABLE_MODBUS
	if (val){
		buf[0] = 100;
		ProtocolModBus_Send(SET_DATA, REG_ADDRESS_FILTER + OperatorParam.AIFilterID, 1, buf);
	}
#endif
	//if (val) tProcessVal.bAIFilterVal[OperatorParam.AIFilterID] = 100;	
	ituWidgetSetVisible(AIResetConfirmBG, false);
	AudioKeySound(SOUND_CLICK2);
	return true;
}
bool AIFilterBtnOnPress(ITUWidget* widget, char* param)
{
	int val = atoi(param);
	if (val) ituWidgetSetY(AIResetConfirmBG, 269);
	else     ituWidgetSetY(AIResetConfirmBG, 168);
	ituWidgetSetVisible(AIResetConfirmBG, true);
	OperatorParam.AIFilterID = val;
	AudioKeySound(SOUND_CLICK2);
	printf("%s:%d\n", __func__, val);
	return true;
}
bool ProcessBtnOnPress(ITUWidget* widget, char* param)
{
	int val = atoi(param);
	int index = val / 2;
	int enable = val % 2;
	bool ret = false;
	printf("%s:Index=%d,enable=%d\n", __func__, index, enable);
	switch (index){
	case PROCESS_ID_SET_CYCLE:
		break;
	case PROCESS_ID_DATE_TIME:
		ret = true;
		LocalDateTimeSet(OperatorParam.InfoID);
		break;
	case PROCESS_ID_SCREEN:
		if (enable){
			tConfig.BrightnessVal = BrightnessWheel->focusIndex+1;
			wConfigSaveCnt = itpGetTickCount();
		}
		ScreenSetBrightness(tConfig.BrightnessVal);
		ret = true;
		break;
	case PROCESS_ID_SS_VOL:
		break;
	case PROCESS_ID_AI_FILTER:
		ituCoverFlowPrev(CycleSetCoverFlow);
		break;
	case PROCESS_ID_RESERV:
		break;
	case PROCESS_ID_ERR:
		break;
	case PROCESS_ID_SMART:
		if (enable){
			tConfig.SmartVal = SmartWheel->focusIndex % 4 + 1;
			wConfigSaveCnt = itpGetTickCount();
		}
		ret = true;
		break;
	default :
		break;
	}
	if (ret) ProcessShow(PROCESS_ID_NULL, INFO_ID_NULL);
	//AudioKeySound(SOUND_CLICK2);
	printf("%s:%d,SmartVal=%d\n", __func__, val, tConfig.SmartVal);
	return true;
}
bool BrightnessWheelOnChange(ITUWidget* widget, char* param)
{
	int val = atoi(param);
	ScreenSetBrightness(val+1);
	printf("%s,%d,%d-%d\n", __func__, val, BrightnessWheel->focusIndex, BrightnessWheel->frame);
	return true;
}
bool LocalSetTimeOnChange(ITUWidget* widget, char* param)
{
	//LocalYearWheel, *LocalMonthWheel, *LocalDayWheel, *LocalHourWheel, *LocalMinWheel
	ITUWheel*wheel = (ITUWheel*)widget;
	int val = atoi(param);
	if (!strcmp(widget->name, "LocalYearWheel")) tBaseSetTime.year = val + 2019;
	else if (!strcmp(widget->name, "LocalMonthWheel")) tBaseSetTime.month = val + 1;
	else if (!strcmp(widget->name, "LocalDayWheel")) tBaseSetTime.day = val + 1;
	else if (!strcmp(widget->name, "LocalHourWheel")) tBaseSetTime.hour = val;
	else if (!strcmp(widget->name, "LocalMinWheel")) tBaseSetTime.min = val;
	printf("year=%d,%d-%d,%d-%d,%d,%s\n", tBaseSetTime.year, tBaseSetTime.month, tBaseSetTime.day, tBaseSetTime.hour, tBaseSetTime.min, wheel->idle, widget->name);
	return true;
}
bool SSVolRadioBoxOnPress(ITUWidget* widget, char* param)
{
	int val = atoi(param);
	int data = SS_KeyVolGet(OperatorParam.InfoID, val);
	if (OperatorParam.InfoID) tConfig.KeyVolumeVal = data;
	else                      tConfig.ScreenSaverTimeSec = data;
	//AudioSetKeyLevel(tConfig.KeyVolumeVal);
	//AudioSetVolume(tConfig.KeyVolumeVal);
	wConfigSaveCnt = itpGetTickCount();
	AudioKeySound(SOUND_CLICK2);
	printf("%s:%d-%d\n", __func__, val, data);
	return true;
}

bool ScreenSaveFunc(ITUWidget* widget, char* param)
{
	if (!ituWidgetIsVisible(ScreenSaverBG)){
		if ((LAYER_FCT != OperatorParam.LayerID) && (LAYER_LOGO != OperatorParam.LayerID)){
			bit_Label_ScreenSaver = 1;
			ituWidgetSetVisible(ScreenSaverBG, true);
			ioctl(ITP_DEVICE_BACKLIGHT, ITP_IOCTL_OFF, NULL);  //welson add for bugs 14
		}
		else
			ScreenSaverRefresh();
		//ITULayer*golayer = ituSceneFindWidget(&theScene, "ScreenSaverLayer");
		//ituLayerGoto(golayer);
		//if (OperatorParam.ProcessID) ProcessShow(PROCESS_ID_NULL, INFO_ID_NULL);
	}
	return true;
}
bool OnceTouchFunc(ITUWidget* widget, char* param)
{
	ScreenSaveExit();
	//printf("==%s\n", __func__);
	return true;
}
bool ReserTypeCheckBoxOnPress(ITUWidget* widget, char* param)
{
	int val = atoi(param);
	if (ituCheckBoxIsChecked(ReserTypeCheckBox)) tReservConfig.MainID = RESERV_SMART;
	else                                         tReservConfig.MainID = RESERV_INIT;
	AudioKeySound(SOUND_CLICK2);
	printf("%s:%d\n", __func__, val);
	return true;
}
bool ReservListBtnOnPress(ITUWidget* widget, char* param)
{
	int val = atoi(param);
	if (0 == val % 10){
		if (ReservListSprite[val / 10]->frame){
			ituSpriteGoto(ReservListSprite[val / 10], 0);
		    tConfigFileReserv.bEnableBit &= ~(1 << (val / 10)); 		
		}
		else{
			ituSpriteGoto(ReservListSprite[val / 10], 1);
			tConfigFileReserv.bEnableBit |= 1 << (val / 10);
		}
		bit_Label_ReservSetUpdate = 1;
	}
	else if (1 == val % 10){
		tReservConfig.EditIndex = val / 10 + 1;
		tReservConfig.MainID = RESERV_DEL;
		//ituWidgetSetVisible(ReservCancelContainer, true);
		bit_Label_ReservSetUpdate = 1;
	}
	else if (2 == val % 10){
		tReservConfig.EditIndex = val / 10 + 1;
		ReservTimeWheelRangeGet(val/10);
		tReservConfig.MainIndex = 1;
		tReservConfig.SubIndex = 1;
		//ituSpriteGoto(ReservSetMainSprite, 1);
		//ituSpriteGoto(ReservSetSubSprite, 1);
		bit_Label_ReservSetUpdate = 1;
		
		tReservConfig.MainID = RESERV_EDIT;
	}
	AudioKeySound(SOUND_CLICK2);
	printf("%s:%d\n", __func__, val);
	return true;
}
bool ReservCtrlOnPress(ITUWidget* widget, char* param)
{
	int val = atoi(param);
	int index = 0;
	bool ret = false;
	printf("%s:MainID=%d,MainIndex=%d\n", __func__, tReservConfig.MainID, tReservConfig.MainIndex);
	if (0 == val){//添加预约
		//ituSpriteGoto(ReservSetMainSprite, 1);
		//ituSpriteGoto(ReservSetSubSprite, 1);
		if (RESERV_INIT == tReservConfig.MainID){
			tReservConfig.MainIndex = 1;
			tReservConfig.SubIndex = 1;
			tReservConfig.EditIndex = 0;
			ReservTimeWheelRangeGet(tConfigFileReserv.bManualCnt);
			bit_Label_ReservSetUpdate = 1;
			tReservConfig.MainID = RESERV_ADD;
			ituTextSetString(ReservEditTitleText, "添加预约");
		}
		else  tReservConfig.MainID = RESERV_INIT; ////welson for bugsNo 20  and return-save btn for edit page
		
	}
	else if (1 == val){//编辑预约
		 if (RESERV_INIT == tReservConfig.MainID){
			tReservConfig.MainID = RESERV_SET;
			ituTextSetString(ReservEditTitleText, "编辑预约");
			bit_Label_ReservSetUpdate = 1;
		}
		else tReservConfig.MainID = RESERV_INIT;
	}
	else if (2 == val){//删除预约
		CustomConfigDelIndex(CONFIG_FILE_RESERV, tReservConfig.EditIndex - 1);//welson for bugsNo 20  and return-save btn for edit page
		//if (!tConfigFileReserv.bManualCnt) ituSpriteGoto(ReservSetMainSprite, 0);
		//bit_Label_ReservSetUpdate = 1;
		//ituWidgetSetVisible(ReservCancelContainer, false);
		tReservConfig.MainID = RESERV_INIT;
	}
	else if (3 == val){//保存预约
		tReservConfig.MainIndex = 0;
		tReservConfig.SubIndex = 0;
		//ituSpriteGoto(ReservSetMainSprite, 0);
		//ituSpriteGoto(ReservSetSubSprite, 0);
		if (tReservConfig.EditIndex){
			index = tReservConfig.EditIndex - 1;
			if (tConfigFileReserv.bManualModeID[index] != tReservTmpItem.bModeID){
				tConfigFileReserv.bManualModeID[index] = tReservTmpItem.bModeID;
				ret = true;
			}
			if (tConfigFileReserv.bManualTimeSrc[index] != tReservTmpItem.bTimeSrc){
				tConfigFileReserv.bManualTimeSrc[index] = tReservTmpItem.bTimeSrc;
				ret = true;
			}
			if (tConfigFileReserv.bManualTimeDst[index] != tReservTmpItem.bTimeDst){
				tConfigFileReserv.bManualTimeDst[index] = tReservTmpItem.bTimeDst;
				ret = true;
			}
			if (tConfigFileReserv.bManualTemper[index] != tReservTmpItem.bTemper){
				tConfigFileReserv.bManualTemper[index] = tReservTmpItem.bTemper;
				ret = true;
			}
			if (ret) CustomConfigRW(CONFIG_FILE_RESERV, 0);
		}
		else{
			CustomConfigaAdd(CONFIG_FILE_RESERV, tReservTmpItem.bModeID, tReservTmpItem.bTimeSrc, tReservTmpItem.bTimeDst, tReservTmpItem.bTemper);
		}
		tReservConfig.MainID = RESERV_INIT;
		bit_Label_ReservSetUpdate = 1;
	}
	else if (4 == val){//返回
		tReservConfig.MainIndex = 0;
		tReservConfig.SubIndex = 0;
		//ituSpriteGoto(ReservSetMainSprite, 0);
		//ituSpriteGoto(ReservSetSubSprite, 0);
		tReservConfig.MainID = RESERV_INIT;
		bit_Label_ReservSetUpdate = 1;
	}
	AudioKeySound(SOUND_CLICK2);
	printf("%s:%d\n", __func__, val);
	return true;
}
void ReservParamShow(uint8_t index)
{
	int i = 0;
	ITUText*text;
	ituSpriteGoto(ReservParamSetSprite, index);
	if (RESERV_PARAM_TIME == index){
		//text = (ITUText*)ituWheelGetFocusItem(ReserSetDayWheel);
		ituTextSetString(ReservParamSetText, "预约时间");
		ReservParamWheelInit();
	}
	else if (RESERV_PARAM_MODE == index){
		for (i = 0; i < UI_CYCLE_MAX; i++){
			//ituRadioBoxSetChecked(ReservModeSelRadioBox[i], false);
			ituWidgetSetColor(ReservModeSelText[i], 255, 160, 160, 160);
		}
		ituRadioBoxSetChecked(ReservModeSelRadioBox[tReservTmpItem.bModeID], true);
		ituWidgetSetColor(ReservModeSelText[tReservTmpItem.bModeID], 255, 255, 255, 255);
		ituTextSetString(ReservParamSetText, "加热模式");
	}
	else if (RESERV_PARAM_TEMPER == index){
		//ituWidgetSetY(ReservTemperWheel, 94);
		printf("tReservTmpItem.bTemper=%d\n", tReservTmpItem.bTemper);
		ituWheelGoto(ReservTemperWheel, (tReservTmpItem.bTemper - 35) / 5);
		ituTextSetString(ReservParamSetText, "加热温度");
	}
	printf("ReservParamShow:index=%d,bModeID=%d\n", index, tReservTmpItem.bModeID);
}
bool ReservSetBtnOnPress(ITUWidget* widget, char* param)
{
	int val = atoi(param);
	if (tReservConfig.SubIndex != 2) tReservConfig.SubIndex = 2;
	if (tReservConfig.OptionIndex != (val + 1)){
		tReservConfig.OptionIndex = val + 1;
		ReservParamShow(val);
	}
	ituWidgetSetY(ReservSetMainMaskIcon, 81 + val * 60);
	//ituWidgetSetVisible(ReservSetMainMaskIcon, true);
	//ituSpriteGoto(ReservSetSubSprite, 2);
	AudioKeySound(SOUND_CLICK2);
	printf("%s:%d\n", __func__, val);
	return true;
}
bool ReserSetCycleRadioBoxOnPress(ITUWidget* widget, char* param)
{
	int val = atoi(param);
	tReservTmpItem.bModeID = val;
	tReservTmpItem.bTemper = atoi(tCycleParamStr[tReservTmpItem.bModeID].Temper);
	//bit_Label_ReservSetUpdate = 1;
	ReservEditShow(&tReservTmpItem);
	AudioKeySound(SOUND_CLICK2);
	printf("%s:%d\n", __func__, val);
	return true;
}
bool ReservSetTimeSrcOnChange(ITUWidget* widget, char* param)
{
	int val = atoi(param);
	ITUText*text;
	text = (ITUText*)ituWheelGetFocusItem(ReservTimeFormWheel);
	int data = 0;
	data = atoi(ituTextGetString(text));
	tReservTmpItem.bTimeSrc = data;
	ReservParamWheelLink(data);
	printf("%s:%d-%d-%d\n", __func__, val, data, ReservTimeFormWheel->idle);
	return true;
}
bool ReservSetTimeDstOnChange(ITUWidget* widget, char* param)
{
	int val = atoi(param);
	ITUText*text;
	text = (ITUText*)ituWheelGetFocusItem(ReservTimeToWheel);
	tReservTmpItem.bTimeDst = atoi(ituTextGetString(text));
	//bit_Label_ReservSetUpdate = 1;
	ReservEditShow(&tReservTmpItem);
	printf("%s:%d-%d\n", __func__, val, ReservTimeToWheel->idle);
	return true;
}
 bool ReservSetTemperOnChange(ITUWidget* widget, char* param)
 {
	 int val = atoi(param);
	 //bit_Label_ReservSetUpdate = 1;
	 ITUText*text = (ITUText*)ituWheelGetFocusItem(ReservTemperWheel);
	 tReservTmpItem.bTemper = atoi(ituTextGetString(text));
	 ReservEditShow(&tReservTmpItem);
	 printf("%s:%d-%d\n", __func__, val, ReservTemperWheel->idle);
	 return true;
 }
bool ProcessTopInfoBtnOnPress(ITUWidget* widget, char* param)
{
	int val = atoi(param);
	AudioKeySound(SOUND_CLICK2);
	printf("%s:%d\n", __func__, val);
	return true;
}
bool ProcessErrDirBtnOnPress(ITUWidget* widget, char* param)
{
	int val = atoi(param);
	AudioKeySound(SOUND_CLICK2);
	printf("%s:%d\n", __func__, val);
	return true;
}
bool SmartOnChange(ITUWidget* widget, char* param)
{
	return true;
}
bool CycleSetCoverFlowOnChange(ITUWidget* widget, char* param)
{
	int val = atoi(param);
	if (0 == val) ProcessShow(PROCESS_ID_NULL, INFO_ID_NULL);
	printf("%s:%d\n", __func__, atoi(param));
	return true;
}