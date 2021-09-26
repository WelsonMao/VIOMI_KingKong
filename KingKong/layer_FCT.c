#include <assert.h>
#include "scene.h"
#include "bsp/COM.h"
#pragma execution_character_set("utf-8")
#define INFO_MAX_NO 8

static ITULayer*FCTLayer;
static ITUButton*GMTSelectBtn[GMT_TEST_MAX], *GMTTestLCDChgBtn, *TPOKButton, *TPFailButton;
static ITUDrawPen*TPDrawPen;
static ITUSprite*FCTDisplaySprite, *GMTTestLCDSprite;
static ITUText*GMTTestVerText[4], *GMTTestUartResultText, *WiFiResultText0, *WiFiResultText1;

FCT_TYPE_e eGMT_ID;
uint32_t iFailID,wCheckTickCnt;
UPGRADE_VER_t tUpgradeVer;
void DrawPenShow(ITUDrawPen*drawpen)
{
	static uint8_t drawpenFlags = false;
	int minX, minY, maxX, maxY, beginX, beginY, endX, endY, curX, curY;
	int index;
	static int ret = 0;

	//index = GMTTestTPSprite->frame;
	curX = drawpen->px0;
	curY = drawpen->py0;
	printf("drawpenFlags=%d\n", drawpen->drawpenFlags);
	if (drawpen->drawpenFlags)
	{
		if (!drawpenFlags)
		{
			drawpenFlags = true;
			ituWidgetSetVisible(TPOKButton, false);
			ituWidgetSetVisible(TPFailButton, false);
			printf("down0:x=%d,y=%d\n", curX, curY);
			//if (!(curX <= WidthOffset && (Heigth - StandOffSet) <= curY)) ret = 1;
		}
		else
		{
			printf("move0:x=%d,y=%d\n", curX, curY);
			//if (!(curX <= WidthOffset)) ret = 1;
		}
	}
	else if (drawpenFlags)
	{
		printf("up0:x=%d,y=%d\n", curX, curY);
		ituWidgetSetVisible(TPOKButton, true);
		ituWidgetSetVisible(TPFailButton, true);
		ituDrawPenStop(drawpen);
		drawpenFlags = false;
		ret = 0;
	}
}
bool FCTLayerOnEnter(ITUWidget* widget, char* param)
{
	char buf[40];
	int i=0;
	if (!FCTLayer){
		FCTLayer = ituSceneFindWidget(&theScene, "FCTLayer");
		FCTDisplaySprite = ituSceneFindWidget(&theScene, "FCTDisplaySprite");
		GMTTestLCDSprite = ituSceneFindWidget(&theScene, "GMTTestLCDSprite");
		GMTTestLCDChgBtn = ituSceneFindWidget(&theScene, "GMTTestLCDChgBtn");
		TPOKButton = ituSceneFindWidget(&theScene, "TPOKButton");
		TPFailButton = ituSceneFindWidget(&theScene, "TPFailButton");
		TPDrawPen = ituSceneFindWidget(&theScene, "TPDrawPen");
		GMTTestUartResultText = ituSceneFindWidget(&theScene, "GMTTestUartResultText");
		WiFiResultText0 = ituSceneFindWidget(&theScene, "WiFiResultText0");
		WiFiResultText1 = ituSceneFindWidget(&theScene, "WiFiResultText1");
		for (i = 0; i < GMT_TEST_MAX; i++){
			sprintf(buf, "GMTSelectBtn%d", i);
			GMTSelectBtn[i] = ituSceneFindWidget(&theScene, buf);
		}
		for (i = 0; i < 4; i++){
			sprintf(buf, "GMTTestVerText%d", i);
			GMTTestVerText[i] = ituSceneFindWidget(&theScene, buf);
		}
	}
	OperatorParam.LayerID = LAYER_FCT;
	ituSpriteGoto(FCTDisplaySprite, 0);
    return true;
}

bool FCTLayerOnTimer(ITUWidget* widget, char* param)
{
	bool ret = false;
	static uint32_t uartshowtick = 0;
	static int uartstate = -1;
	if (GMT_TEST_TP == eGMT_ID) DrawPenShow(TPDrawPen);
	else if (GMT_TEST_UART == eGMT_ID) {
		if (!tComErr.ComState){
			uartshowtick++;
			if (uartshowtick >= 100) uartstate = 0;
		}
		else{
			uartshowtick = 0;
			uartstate = 1;
		}
		if ((itpGetTickDuration(wCheckTickCnt) >= 3 * 1000)){
			if (uartstate)  ituTextSetString(GMTTestUartResultText, "OK");
			else{
				ituTextSetString(GMTTestUartResultText, "NG ");
				uartshowtick = 0;
			}
		}
	}
	/*else if (GMT_TEST_WiFi == eGMT_ID){
		if (itpGetTickDuration(wCheckTickCnt) >= 3 * 1000){
			if ((T_Wifi_Net_State_Type_Local == vBEH__Wifi__Read__Net__State()) || (T_Wifi_Net_State_Type_Cloud == vBEH__Wifi__Read__Net__State()))
				ituTextSetString(WiFiResultText0, "OK");
			else
				ituTextSetString(WiFiResultText0, "NG");

		}
		else{
			tempval = itpGetTickDuration(wCheckTickCnt) / 200;
			if (0 == (tempval % 3)) ituTextSetString(WiFiResultText0, ".");
			else if (1 == (tempval % 3)) ituTextSetString(WiFiResultText0, "..");
			else if (2 == (tempval % 3)) ituTextSetString(WiFiResultText0, "...");
		}
	}
	else if (GMT_TEST_VER == eGMT_ID){
		static uint8_t MCVer = 0;
		static uint8_t MoterVerH = 0;
		static uint8_t MoterVerL = 0;
		if ((MCVer != tDataRev.u8MCVer) || (MoterVerH != tDataRev.u8MotorVerH) || (MoterVerL != tDataRev.u8MotorVerL)){
			sprintf(buf, "电源板版本：%02x.%02x.%02x", tDataRev.u8MCVer, tDataRev.u8MotorVerH, tDataRev.u8MotorVerL);
			ituTextSetString(GMTTestVerText[1], buf);
			MCVer = tDataRev.u8MCVer;
			MoterVerH = tDataRev.u8MotorVerH;
			MoterVerL = tDataRev.u8MotorVerL;
			ret |= true;
		}
	}*/
    return ret;
}

bool FCTLayerOnLeave(ITUWidget* widget, char* param)
{
    return true;
}

bool GMTTestResultBtnOnPress(ITUWidget* widget, char* param)
{
	int val = atoi(param);
	if (val){
		iFailID &= (uint16_t)(~(3 << (FCTDisplaySprite->frame - 1) * 2));
		iFailID |= (uint16_t)(1 << (FCTDisplaySprite->frame - 1) * 2);
	}
	else{
		iFailID &= (uint16_t)(~(3 << (FCTDisplaySprite->frame - 1) * 2));
		iFailID |= (uint16_t)(2 << (FCTDisplaySprite->frame - 1) * 2);
	}
	if (GMT_TEST_BACKLIGHT == eGMT_ID)  ScreenSetBrightness(theConfig.brightness);
	//else if (GMT_TEST_LAMP == eGMT_ID)   LampOn(false);
	eGMT_ID = GMT_TEST_NULL;
	//for (int i = 0; i < 8; i++)
	{
		if (val)
		{
			printf("red\n");
			GMTSelectBtn[FCTDisplaySprite->frame - 1]->bgColor.blue = 0;
			GMTSelectBtn[FCTDisplaySprite->frame - 1]->bgColor.red = 0xFF;
			GMTSelectBtn[FCTDisplaySprite->frame - 1]->bgColor.green = 0;
		}
		else
		{
			printf("green\n");
			GMTSelectBtn[FCTDisplaySprite->frame - 1]->bgColor.blue = 0;
			GMTSelectBtn[FCTDisplaySprite->frame - 1]->bgColor.red = 0;
			GMTSelectBtn[FCTDisplaySprite->frame - 1]->bgColor.green = 0xFF;
		}

	}
	AudioKeySound(SOUND_CLICK2);
	printf("OK/FAIL=%d,iFailID=%X,sprite=%d\n", val, iFailID, FCTDisplaySprite->frame);
	ituSpriteGoto(FCTDisplaySprite, GMT_TEST_NULL);
	return true;
}

bool FCTSpeakerBtnOnPress(ITUWidget* widget, char* param)
{
	AudioKeySound(SOUND_ERROR);
    return true;
}

bool FCTBackLightBtnOnPress(ITUWidget* widget, char* param)
{
	int val = atoi(param);
	if (0 == val) ScreenSetBrightness(9);
	else if (1 == val)     ScreenSetBrightness(5);
	//else if (2 == val)  LampOn(true);
	//else if (3 == val)  LampOn(false);
	AudioKeySound(SOUND_CLICK2);
	printf("%s%d,%d\n", __func__, __LINE__, val);
    return true;
}

bool TPDrawPenOnStop(ITUWidget* widget, char* param)
{
    return true;
}

bool GMTTestLCDChgBtnOnPress(ITUWidget* widget, char* param)
{
	int val = GMTTestLCDSprite->frame;
	if (val <= GMTTestLCDSprite->childCount - 2)
	{
		val++;
		ituSpriteGoto(GMTTestLCDSprite, val);
		if (val == GMTTestLCDSprite->childCount - 1) ituWidgetDisable(GMTTestLCDChgBtn);
	}
	AudioKeySound(SOUND_CLICK2);
    return true;
}

bool SelfCheckReturnBtnOnPress(ITUWidget* widget, char* param)
{
	ITULayer*MainMenuLayer = ituSceneFindWidget(&theScene, "MainMenuLayer");
	//ResponseHandle_CMD_Ctrl_ON_OFF(CTRL_ON);
	bit_Flag_FCT = 0;
	ituLayerGoto(MainMenuLayer);
	AudioKeySound(SOUND_CLICK2);
	printf("%s%d\n", __func__, __LINE__);
    return true;
}
void GMT_DISPLAY_FCT_SHOW(int index)
{
	char buf[100];
	switch (index)
	{
	case GMT_TEST_VER:
		sprintf(buf, "显示板版本：%s", SW_VERSION_STR);
		ituTextSetString(GMTTestVerText[0], buf);
		printf("[0]%s\n", buf);
		/*sprintf(buf, "电源板版本：%02x.%02x.%02x", tDataRev.u8MCVer, tDataRev.u8MotorVerH, tDataRev.u8MotorVerL);
		ituTextSetString(GMTTestVerText[1], buf);
		MCVer = tDataRev.u8MCVer;
		MoterVerH = tDataRev.u8MotorVerH;
		MoterVerL = tDataRev.u8MotorVerL;
		printf("[1]%s\n", buf);
		*/
#ifdef UPGRADE_NEW
		sprintf(buf, "触摸屏版本：%X", bTPVerGet);
		ituTextSetString(GMTTestVerText[2], buf);
		sprintf(buf, "触摸键版本：%X", bTouchKeyVerGet);
		ituTextSetString(GMTTestVerText[3], buf);
#else
#ifdef TP_VERSION
		if (tTPVersion.TPVer) ituWidgetSetVisible(GMTTestVerText[2], true);
		else                  ituWidgetSetVisible(GMTTestVerText[2], false);
		sprintf(buf, "触摸屏版本：%X", tTPVersion.TPVer);
		ituTextSetString(GMTTestVerText[2], buf);
#endif
		sprintf(buf, "触摸键版本：%X", tUpgradeVer.iTouchVer);
		ituTextSetString(GMTTestVerText[3], buf);
#endif

		break;
	case GMT_TEST_LCD:
		ituWidgetEnable(GMTTestLCDChgBtn);
		ituSpriteGoto(GMTTestLCDSprite, 0);
		break;
	case GMT_TEST_TP:
		ituWidgetSetVisible(TPOKButton, false);
		ituWidgetSetVisible(TPFailButton, false);

		break;
	}
}
bool GMTTestSelectBtnOnPress(ITUWidget* widget, char* param)
{
	int val = atoi(param) + 1;
	eGMT_ID = val;
	ituSpriteGoto(FCTDisplaySprite, val);
	if (GMT_TEST_WiFi == val){
		//vBEH__Wifi__Factory__Apply();
		//wCheckTickCnt = itpGetTickCount();
	}
	else if (GMT_TEST_UART == val)
		wCheckTickCnt = itpGetTickCount();
	printf("%s,val=%d \n", __func__, val);
	GMT_DISPLAY_FCT_SHOW(val);
	AudioKeySound(SOUND_CLICK2);
	return true;
}

