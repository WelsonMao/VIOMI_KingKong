#include "ite/itu.h"
extern bool PowerOnLayerOnEnter(ITUWidget* widget, char* param);
extern bool PowerOnLayerOnTimer(ITUWidget* widget, char* param);
extern bool PowerOnLayerOnLeave(ITUWidget* widget, char* param);

extern bool LogoLayerOnEnter(ITUWidget* widget, char* param);
extern bool LogoLayerOnTimer(ITUWidget* widget, char* param);
extern bool LogoLayerOnLeave(ITUWidget* widget, char* param);
extern bool ProcessLayerOnEnter(ITUWidget* widget, char* param);
extern bool LogoPowerBtnOnPress(ITUWidget* widget, char* param);

extern bool MainMenuLayerOnEnter(ITUWidget* widget, char* param);
extern bool MainMenuLayerOnTimer(ITUWidget* widget, char* param);
extern bool MainMenuLayerOnLeave(ITUWidget* widget, char* param);
extern bool MainMenuBtnOnPress(ITUWidget* widget, char* param);
//extern bool MainMenuAICheckBoxOnPress(ITUWidget* widget, char* param);
extern bool TopSideBtnOnPress(ITUWidget* widget, char* param);


extern bool ConfigLayerOnEnter(ITUWidget* widget, char* param);
extern bool ConfigLayerOnTimer(ITUWidget* widget, char* param);
extern bool ConfigLayerOnLeave(ITUWidget* widget, char* param);
extern bool ConfigReturnBtnOnPress(ITUWidget* widget, char* param);
extern bool ResetBtnOnPress(ITUWidget* widget, char* param);
extern bool ConfigOptionBtnOnPress(ITUWidget* widget, char* param);
extern bool ConfigJumpBtnOnPress(ITUWidget* widget, char* param);
extern bool ConfigWiFiRstBtn(ITUWidget* widget, char* param);
extern bool ConfigMainCheckBoxOnPress(ITUWidget* widget, char* param);
extern bool FCTBtnOnPress(ITUWidget* widget, char* param);

//extern bool ScreenSaverLayerOnEnter(ITUWidget* widget, char* param);
//extern bool ScreenSaverLayerOnTimer(ITUWidget* widget, char* param);
//extern bool ScreenSaverLayerOnLeave(ITUWidget* widget, char* param);
//
//extern bool PowerOffLayerOnEnter(ITUWidget* widget, char* param);
//extern bool PowerOffLayerOnTimer(ITUWidget* widget, char* param);
//extern bool PowerOffLayerOnLeave(ITUWidget* widget, char* param);

extern bool FCTLayerOnEnter(ITUWidget* widget, char* param);
extern bool FCTLayerOnTimer(ITUWidget* widget, char* param);
extern bool FCTLayerOnLeave(ITUWidget* widget, char* param);
extern bool GMTTestResultBtnOnPress(ITUWidget* widget, char* param);
extern bool FCTSpeakerBtnOnPress(ITUWidget* widget, char* param);
extern bool FCTBackLightBtnOnPress(ITUWidget* widget, char* param);
extern bool TPDrawPenOnStop(ITUWidget* widget, char* param);
extern bool GMTTestLCDChgBtnOnPress(ITUWidget* widget, char* param);
extern bool SelfCheckReturnBtnOnPress(ITUWidget* widget, char* param);
extern bool GMTTestSelectBtnOnPress(ITUWidget* widget, char* param);

extern bool ProcessLayerOnTimer(ITUWidget* widget, char* param);
extern bool ProcessWBBtnOnPress(ITUWidget* widget, char* param);
extern bool ScreenSaveFunc(ITUWidget* widget, char* param);
extern bool OnceTouchFunc(ITUWidget* widget, char* param);
extern bool AIFilterRstBtnOnPress(ITUWidget* widget, char* param);
extern bool AIFilterBtnOnPress(ITUWidget* widget, char* param);
extern bool SSVolRadioBoxOnPress(ITUWidget* widget, char* param);
extern bool ProcessBtnOnPress(ITUWidget* widget, char* param);
extern bool BrightnessWheelOnChange(ITUWidget* widget, char* param);
extern bool LocalSetTimeOnChange(ITUWidget* widget, char* param);
extern bool CustomCycleTrackBarOnChange(ITUWidget* widget, char* param);
extern bool ProcessCycleSetBtnOnPress(ITUWidget* widget, char* param);
extern bool ReserTypeCheckBoxOnPress(ITUWidget* widget, char* param);
extern bool ReservListBtnOnPress(ITUWidget* widget, char* param);
extern bool ReservCtrlOnPress(ITUWidget* widget, char* param);
extern bool ReservSetBtnOnPress(ITUWidget* widget, char* param);
extern bool ReserSetCycleRadioBoxOnPress(ITUWidget* widget, char* param);
extern bool ReservSetTimeSrcOnChange(ITUWidget* widget, char* param);
extern bool ReservSetTimeDstOnChange(ITUWidget* widget, char* param);
extern bool ReservSetTemperOnChange(ITUWidget* widget, char* param);
extern bool ProcessTopInfoBtnOnPress(ITUWidget* widget, char* param);
extern bool ProcessErrDirBtnOnPress(ITUWidget* widget, char* param);
extern bool SmartOnChange(ITUWidget* widget, char* param);
extern bool CycleSetCoverFlowOnChange(ITUWidget* widget, char* param);
ITUActionFunction actionFunctions[] =
{
	"PowerOnLayerOnEnter", PowerOnLayerOnEnter,
	"PowerOnLayerOnTimer", PowerOnLayerOnTimer,
	"PowerOnLayerOnLeave", PowerOnLayerOnLeave,
    "LogoLayerOnEnter", LogoLayerOnEnter,
    "LogoLayerOnTimer", LogoLayerOnTimer,
    "LogoLayerOnLeave", LogoLayerOnLeave,
    "ProcessLayerOnEnter", ProcessLayerOnEnter,
	"LogoPowerBtnOnPress", LogoPowerBtnOnPress,
    "MainMenuLayerOnEnter", MainMenuLayerOnEnter,
    "MainMenuLayerOnTimer", MainMenuLayerOnTimer,
    "MainMenuLayerOnLeave", MainMenuLayerOnLeave,
    "MainMenuBtnOnPress", MainMenuBtnOnPress,
	//"MainMenuAICheckBoxOnPress", MainMenuAICheckBoxOnPress,
    "TopSideBtnOnPress", TopSideBtnOnPress,


    "ConfigLayerOnEnter", ConfigLayerOnEnter,
    "ConfigLayerOnTimer", ConfigLayerOnTimer,
    "ConfigLayerOnLeave", ConfigLayerOnLeave,
    "ConfigReturnBtnOnPress", ConfigReturnBtnOnPress,
    "ResetBtnOnPress", ResetBtnOnPress,
    "ConfigOptionBtnOnPress", ConfigOptionBtnOnPress,
    "ConfigJumpBtnOnPress", ConfigJumpBtnOnPress,
    "ConfigWiFiRstBtn", ConfigWiFiRstBtn,
    "ConfigMainCheckBoxOnPress", ConfigMainCheckBoxOnPress,
	"FCTBtnOnPress", FCTBtnOnPress,

    /*"ScreenSaverLayerOnEnter", ScreenSaverLayerOnEnter,
    "ScreenSaverLayerOnTimer", ScreenSaverLayerOnTimer,
    "ScreenSaverLayerOnLeave", ScreenSaverLayerOnLeave,
    "PowerOffLayerOnEnter", PowerOffLayerOnEnter,
    "PowerOffLayerOnTimer", PowerOffLayerOnTimer,
    "PowerOffLayerOnLeave", PowerOffLayerOnLeave,*/
    "FCTLayerOnEnter", FCTLayerOnEnter,
    "FCTLayerOnTimer", FCTLayerOnTimer,
    "FCTLayerOnLeave", FCTLayerOnLeave,
    "GMTTestResultBtnOnPress", GMTTestResultBtnOnPress,
    "FCTSpeakerBtnOnPress", FCTSpeakerBtnOnPress,
    "FCTBackLightBtnOnPress", FCTBackLightBtnOnPress,
    "TPDrawPenOnStop", TPDrawPenOnStop,
    "GMTTestLCDChgBtnOnPress", GMTTestLCDChgBtnOnPress,
    "SelfCheckReturnBtnOnPress", SelfCheckReturnBtnOnPress,
    "GMTTestSelectBtnOnPress", GMTTestSelectBtnOnPress,
    "ProcessLayerOnTimer", ProcessLayerOnTimer,
	"ProcessWBBtnOnPress", ProcessWBBtnOnPress,
	"ScreenSaveFunc", ScreenSaveFunc,
	"OnceTouchFunc", OnceTouchFunc,
    "AIFilterRstBtnOnPress", AIFilterRstBtnOnPress,
    "AIFilterBtnOnPress", AIFilterBtnOnPress,
    "SSVolRadioBoxOnPress", SSVolRadioBoxOnPress,
	"ProcessBtnOnPress", ProcessBtnOnPress,
	"BrightnessWheelOnChange", BrightnessWheelOnChange,
    "LocalSetTimeOnChange", LocalSetTimeOnChange,
    "CustomCycleTrackBarOnChange", CustomCycleTrackBarOnChange,
    "ProcessCycleSetBtnOnPress", ProcessCycleSetBtnOnPress,
	"ReserTypeCheckBoxOnPress", ReserTypeCheckBoxOnPress,
	"ReservListBtnOnPress", ReservListBtnOnPress,
	"ReservCtrlOnPress", ReservCtrlOnPress,
	"ReservSetBtnOnPress", ReservSetBtnOnPress,
	"ReserSetCycleRadioBoxOnPress", ReserSetCycleRadioBoxOnPress,
	"ReservSetTimeSrcOnChange", ReservSetTimeSrcOnChange,
	"ReservSetTimeDstOnChange", ReservSetTimeDstOnChange,
	"ReservSetTemperOnChange", ReservSetTemperOnChange,
	"ProcessTopInfoBtnOnPress", ProcessTopInfoBtnOnPress,
	"ProcessErrDirBtnOnPress",ProcessErrDirBtnOnPress,
	"SmartOnChange", SmartOnChange,
	"CycleSetCoverFlowOnChange", CycleSetCoverFlowOnChange,
    NULL, NULL
};
