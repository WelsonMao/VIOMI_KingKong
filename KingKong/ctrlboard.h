/** @file
 * ITE Display Control Board Modules.
 *
 * @author Jim Tan
 * @version 1.0
 * @date 2015
 * @copyright ITE Tech. Inc. All Rights Reserved.
 */
/** @defgroup ctrlboard ITE Display Control Board Modules
 *  @{
 */
#ifndef CTRLBOARD_H
#define CTRLBOARD_H

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup ctrlboard_audio Audio Player
 *  @{
 */
#define  HW_VERSION_STR               "UI7061"
#define  SW_VERSION_STR               "7.0.00.210926"
#define  CUSTOM_ID_STR                "VIMVO"
#define  PRODUCT_ID_STR               "KINGKONG"

#define  CONFIG_SAVE_TIME          (5 * 1000)
#define  SCREENSAVER_OPTION_CNT     6
#define  WIFI_RST_TIMEOUT          (15*1000)
#define  SYS_RST_TIMEOUT           (5 * 1000)

#define  ENABLE_MCUPGRADE
#define  ENABLE_MODBUS
#define  ENABLE_RESERV 
#define  ENABLE_ERR
#define  RESERV_DEFALT_MODE_ID      0

#define STRUCT_CLR(x)          memset((uint8_t*)&(x), 0, (uint32_t)(sizeof(x)))
#define STR(s)     #s
#define USER_DEBUG
#ifdef  USER_DEBUG
#define LOG(M, ...) do{fprintf(stdout,"##%s|%d## ",  __FUNCTION__, __LINE__);fprintf(stdout, M, ##__VA_ARGS__);fprintf(stdout, "\n");fflush(stdout);}while(0)
#define LOG_ERR(M, ...) do{fprintf(stderr,"##%s|%d## ",  __FUNCTION__, __LINE__);fprintf(stderr, M, ##__VA_ARGS__);fprintf(stderr, "\n");fflush(stderr);}while(0)
#define LOG_DEB(M, ...) do{fprintf(stdout,"##%s|%d## ",  __FUNCTION__, __LINE__);fprintf(stdout, M, ##__VA_ARGS__);fprintf(stdout, "\n");fflush(stdout);}while(0)
#else
#define LOG(M, ...)
#define LOG_ERR(M, ...) do{fprintf(stderr,"##%s|%d## ", __FUNCTION__, __LINE__);fprintf(stderr, M, ##__VA_ARGS__);fprintf(stderr, "\n");fflush(stderr);}while(0)
#define LOG_DEB(M, ...)
#endif
typedef	 volatile  union{
	struct{
		unsigned char bit0 : 1;
		unsigned char bit1 : 1;
		unsigned char bit2 : 1;
		unsigned char bit3 : 1;
		unsigned char bit4 : 1;
		unsigned char bit5 : 1;
		unsigned char bit6 : 1;
		unsigned char bit7 : 1;
	}bits;
	unsigned char byte;
}Flag_Set;
#define Flag_set		Flag_Set
extern Flag_set _fConfigOption;
#define	_fConfigOptionByte    		_fConfigOption.byte
#define	bit_Config_TemperResevOut   _fConfigOption.bits.bit0
#define bit_Config_WaterOutPowerOff	_fConfigOption.bits.bit1 
#define bit_Config_NetTime          _fConfigOption.bits.bit2
#define bit_Config_ScreenSaver      _fConfigOption.bits.bit3
#define bit_Config_KeyVoice         _fConfigOption.bits.bit4
#define bit_Config_ChildLock        _fConfigOption.bits.bit5
#define bit_Config_AISurface        _fConfigOption.bits.bit6
#define bit_Config_Smart            _fConfigOption.bits.bit7
extern Flag_set _fLabel;
#define	_fLabelByte    		        _fLabel.byte
#define	bit_Label_ReservSetUpdate   _fLabel.bits.bit0
#define bit_Label_ParamOnce	        _fLabel.bits.bit1 
#define bit_Label_WiFi              _fLabel.bits.bit2
#define bit_Label_ScreenSaver		_fLabel.bits.bit3
#define bit_Label_Upgrade           _fLabel.bits.bit4
#define bit_Label_Reserv5           _fLabel.bits.bit5
#define bit_Label_Reserv6           _fLabel.bits.bit6
#define bit_Label_Reserv7           _fLabel.bits.bit7
extern Flag_set _fFlagBit;
#define	_fFlagByte    			_fFlagBit.byte
#define	bit_Flag_SuperMode      _fFlagBit.bits.bit0
#define bit_Flag_FCT	        _fFlagBit.bits.bit1 
extern Flag_set _fResetBit;
#define	_fResetByte    			_fResetBit.byte
#define	bit_Reset_Layer         _fResetBit.bits.bit0

typedef struct
{
	bool Enable_TemperResevOut;
	bool Enable_WaterOutPowerOff;
	bool Enable_NetTime;
	bool Enable_ScreenSaver;
	bool Enable_KeyVoice;
	bool Enable_ChildLock;
	bool Enable_AISurface;
	bool Enable_Smart;

	uint8_t BrightnessVal;
	uint8_t KeyVolumeVal;
	uint8_t otaMode;
	uint8_t McuVersion;
	uint8_t SmartVal;
	int ScreenSaverTimeSec;
}CONFIG_t;
extern CONFIG_t tConfig;
typedef enum
{
	TOPSIDE_WIFI=0,
	TOPSIDE_CONFIG=1,
	TOPSIDE_MAX=2
};
typedef enum
{
	PERCENT_NULL=0,
	PERCENT_WIFI=1,
	PERCENT_RST=2,
	PERCENT_MAX
};
typedef enum
{
	LAYER_POWER_ON = 0,
	LAYER_LOGO = 1,
	LAYER_MAIN_MENU = 2,
	LAYER_CONFIG = 3,
	LAYER_FCT = 4,
	LAYER_PROCESS=5,
	LAYER_MAX
};
typedef enum
{
	UI_CYCLE_FAST = 0,//瞬热洗
	UI_CYCLE_SINGLE = 1,
	UI_CYCLE_DOUBLE = 2,
	UI_CYCLE_ECO = 3,
	UI_CYCLE_CUSTOMER=4,
	UI_CYCLE_MAX
}UI_CYCLE_e;
typedef enum
{
	CONFIG_SW_TEMPER_RESEVOUT = 0,
	CONFIG_SW_WATEROUT_POWEROFF = 1,
	CONFIG_SW_NET_TIME=2,
	CONFIG_SW_SCREENSAVER = 3,
	CONFIG_SW_KEY_VOICE = 4,
	CONFIG_SW_SMART=5,
	CONFIG_SW_MAX = 6
};
typedef enum
{
	CONFIG_MENU_GENERAL = 0,
	CONFIG_MENU_WLAN = 1,
	CONFIG_MENU_DATE_TIME = 2,
	CONFIG_MENU_SCRREN_AUDIO = 3,
	CONFIG_MENU_SW_VER = 4,
	CONFIG_MENU_MAX=5
};
typedef enum
{
	RESERV_PARAM_TIME=0,
	RESERV_PARAM_MODE = 1,
	RESERV_PARAM_TEMPER = 2,
	RESERV_PARAM_MAX
};
typedef enum{
	SOUND_START = 0,
	SOUND_STOP,
	SOUND_LOCK_IN,
	SOUND_LOCK_OUT,
	SOUND_WATEROUT_START,
	SOUND_WATEROUT_STOP,
	SOUND_LOCK_ALARM,
	SOUND_WATERLOW_ALARM,
	SOUND_FUNCTION_IN,
	SOUND_FUNCTION_OUT,
	SOUND_WARNING,
	SOUND_ERROR,
	SOUND_REMEMBER,
	SOUND_CLICK,
	SOUND_CLICK2,
	SOUND_MAX
}SOUND_e;
typedef enum
{
	GMT_TEST_NULL = 0,
	GMT_TEST_VER,
	GMT_TEST_LCD,
	GMT_TEST_TP,
	GMT_TEST_WiFi,
	GMT_TEST_UART,
	GMT_TEST_BACKLIGHT,
	GMT_TEST_SPEAKER,
	GMT_TEST_MAX
}FCT_TYPE_e;
typedef struct
{
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t day;
	uint8_t month;
	uint16_t year;
}BASE_TIME_t;
extern BASE_TIME_t tBaseTime;
typedef enum
{
	RESERV_INIT=0,
	RESERV_ADD = 1,
	RESERV_SET=2,
	RESERV_EDIT=3,
	RESERV_DEL=4,
	RESERV_SMART
};
typedef struct
{
	uint8_t MainIndex;
	uint8_t MainID;
	uint8_t MainSelIndex;
	uint8_t EditIndex;
	uint8_t SubIndex;
	uint8_t OptionIndex;
	uint8_t TimeRangeTab[24];
	uint8_t TimeRangeCnt;
	uint8_t TimePeriodCnt;
	uint8_t TimeMinTab[4];
	uint8_t TimeMaxTab[4];
}RESERV_CONFIG_t;

typedef struct
{
	uint8_t LayerID;
	uint8_t ConfigID;
	uint8_t ErrID;
	uint8_t PowerStatus;
	UI_CYCLE_e CycleID;
	uint8_t OptionSetID;
	uint8_t ProcessID;
	uint8_t InfoID;
	uint16_t CycleTime;
	uint8_t CyclePageID;
	uint8_t FavorID;
	uint8_t SSLayerFromID;
	uint8_t WiFiLayerFromID;
	uint8_t TipInfoPage;
	uint8_t AIFilterID;
	int    LayerJump;
	int    FingerX;
	int    FingerY;

}OPERATE_t;
extern RESERV_CONFIG_t tReservConfig;
extern OPERATE_t OperatorParam;
typedef struct
{
	char *Name;
	char *Temper;
	char *Capacity;
	char *Power;
}CYCLE_PARAM_LIST_t;
typedef struct
{
	uint8_t bTemperVal;
	uint8_t bCapacityVal;
	uint8_t bPowerVal;
}CYCLE_PARAM_t;
typedef struct
{
	uint8_t bTimeSrc;
	uint8_t bTimeDst;
	uint8_t bModeID;
	uint8_t bTemper;
}RESERV_TEMP_ITEM_t;
extern RESERV_TEMP_ITEM_t tReservTmpItem;
extern CYCLE_PARAM_LIST_t tCycleParamStr[];
extern CYCLE_PARAM_t tCycleParamTab[UI_CYCLE_MAX];
extern const char*WeekTab[];
typedef enum
{
	HEATER_STATE_IDLE=0,
	HEATER_STATE_RUN,
	HEATER_STATE_MAX
}HEATER_STATE_e;
typedef struct
{
	uint8_t bMode;
	uint8_t bSetTemperVal;
	uint8_t bAIFilterVal[2];
	HEATER_STATE_e eHeaterState;
}PROCESSVAL_t;
extern PROCESSVAL_t tProcessVal;
typedef enum
{
	PROCESS_ID_NULL = 0,
	PROCESS_ID_SET_CYCLE = 1,
	PROCESS_ID_DATE_TIME=2,
	PROCESS_ID_SCREEN=3,
	PROCESS_ID_SS_VOL=4,
	PROCESS_ID_AI_FILTER=5,
	PROCESS_ID_RESERV=6,
	PROCESS_ID_ERR=7,
	PROCESS_ID_SMART=8,
	PROCESS_ID_MAX
}INFO_e;
typedef enum
{
	WB_ID_NULL = 0,
	WB_ID_SET_CYCLE = 1,
	WB_ID_MIX= 2,
	WB_ID_RESERV = 3,
	WB_ID_MAX
}WB_e;
typedef enum
{
	INFO_ID_NULL = 0,
	INFO_ID_DATE = 0,
	INFO_ID_TIME = 1,
	INFO_ID_SCREEN_SAVER = 0,
	INFO_ID_KEY_AUDIO = 1,
	INFO_ID_MAX
}ID_e;
typedef enum
{
	TEMPER_CATOON_KEEP_WARM=0,
	TEMPER_CATOON_HEAT=1,
	TEMPER_CATOON_MAX
};
typedef struct
{
	bool recover;
	uint8_t type;
	char *errid;
	char *errorchs;
	char *errorchs2;
}ERROR_LIST_t;
typedef struct
{
	uint32_t tick;
	uint8_t  cnt;
	bool enable;
}RPT_TICK_t;
typedef struct
{
	uint16_t iTPVer;
	uint16_t iTouchVer;
}UPGRADE_VER_t;
typedef enum
{
	CONVERT_IOT_2_MACHINE = 0,
	CONVERT_MACHINE_2_IOT
}CONVERT_TOWARD_e;
typedef enum
{
	IO_CMD_NONE = 0,
	IO_CMD_MODE = 1,
	IO_CMD_MODE_TIME = 2,
	IO_CMD_STAINS = 3,
}IO_CMD_EVENT_e;
typedef struct{
	IO_CMD_EVENT_e io_cmd;
	uint8_t localparam;
	uint8_t miiotparam;
}IO_PARAM_MAP;

extern const char*RunStatusStr[];
extern uint16_t mapConvertMiiotAndMachine(CONVERT_TOWARD_e toward, IO_CMD_EVENT_e event, uint16_t data);
extern  UPGRADE_VER_t tUpgradeVer;
extern  ERROR_LIST_t  tUIErrorLsit[];
extern bool ProcessShow(uint8_t Process_ID, uint8_t Info_ID);
extern void ProcessLayerJump(uint8_t layerID, uint8_t reset);
extern bool LocalTimeSyc(void);
extern uint32_t wConfigSaveCnt;
typedef int (*AudioPlayCallback)(int state);

/**
 * Initializes audio module.
 */
void AudioInit(void);

/**
 * Exits audio module.
 */
void AudioExit(void);

/**
 * Plays the specified wav file.
 *
 * @param filename The specified wav file to play.
 * @param func The callback function.
 * @return 0 for success, otherwise failed.
 */
int AudioPlay(char* filename, AudioPlayCallback func);

/**
 * Stops playing sound.
 */
void AudioStop(void);

int AudioPlayMusic(char* filename, AudioPlayCallback func);

/**
 * Plays keypad sound.
 */
void AudioPlayKeySound(void);
void AudioPauseKeySound(void);
void AudioResumeKeySound(void);
void AudioKeySound(uint8_t index);
/**
 * Sets the volume of keypad sound.
 *
 * @param level the percentage of volume.
 */
void AudioSetKeyLevel(int level);

/**
 * Mutes all audio.
 */
void AudioMute(void);

/**
 * Un-mutes all audio.
 */
void AudioUnMute(void);

/**
 * Determines whether this audio is muted or not.
 *
 * @return true muted, false otherwise.
 */
bool AudioIsMuted(void);

bool AudioIsPlaying(void);

void AudioSetVolume(int level);
void AudioSetLevel(int level);
int AudioGetVolume(void);

/** @} */ // end of ctrlboard_audio

/** @defgroup ctrlboard_config Configuration
 *  @{
 */
/**
 * Language type definition.
 */
typedef enum
{
    LANG_ENG,   ///< English
    LANG_CHT,   ///< Traditional Chinese
    LANG_CHS    ///< Simplified Chinese
} LangType;

typedef enum
{
    WIFI_CLIENT = 0,   ///< Client mode
    WIFI_SOFTAP,   ///< SoftAP mode
} WifiType;

bool disableHotspot;
/**
 * Main menu type definition.
 */
typedef enum
{
    MAINMENU_COVERFLOW,             ///< Cover flow
    MAINMENU_COVERFLOW_REFLECTION,  ///< Cover flow with reflection effect
    MAINMENU_PAGEFLOW_FLIP,         ///< Flip page flow
    MAINMENU_PAGEFLOW_FLIP2,        ///< Flip2 page flow
    MAINMENU_PAGEFLOW_FOLD,         ///< Fold page flow
    MAINMENU_COVERFLOW_RIPPLE       ///< Cover flow with ripple effect
} MainMenuType;

typedef struct
{
    // network
    int dhcp;                                                           ///< Enable DHCP or not
    char ipaddr[16];                                                    ///< IP address
    char netmask[16];                                                   ///< Netmask
    char gw[16];                                                        ///< Gateway address
    char dns[16];                                                       ///< DNS address

    // display
    int brightness;                                                     ///< Brightness, the range is 0~9
    int screensaver_time;                                               ///< Time to enter screen saver mode, unit is minute
    int screensaver_type;                                               ///< Screen saver type @see ScreensaverType
    int lang;                                                           ///< Language type @see LangType
    int mainmenu_type;                                                  ///< Main menu type @see MainMenuType

    // sound
    char      keysound[PATH_MAX];                                       // /< Key sound file path
    int       keylevel;                                                 // /< Key volume percentage, range is 0~100
    int       audiolevel;                                               // /< Audio volume percentage, range is 0~100

    // photo
    int       photo_interval;                                           // /< Photo change interval (second)

    // setting
    int touch_calibration;                                              ///< Need to do the touch calibration or not

    // wifi
    char ssid[64];
    char password[256];
    char secumode[3];
    int wifi_mode;
    int wifi_on_off;
    // wifi
    char ap_ssid[64];  // ap mode ssid
    char ap_password[256]; // ap mode password
    

	// login
    char user_id[64];
    char user_password[64];

	int order[2][6];

	//user
	uint8_t option;
	uint8_t otaMode;
	uint8_t McuVersion;
	uint8_t SmartVal;
	int fctresult;
} Config;

/**
 * Global instance variable of configuration.
 */
extern Config theConfig;

/**
 * Loads configuration file.
 */
void ConfigInit(void);

/**
 * Exits configuration.
 */
void ConfigExit(void);

/**
 * Updates CRC files.
 *
 * @param filepath The file path to update the CRC value. NULL for ini file on public drive.
 */
void ConfigUpdateCrc(char* filepath);

/**
 * Saves the public part of configuration to file.
 */
void ConfigSave(void);
void ConfigReset(void);
/** @} */ // end of ctrlboard_config

/** @defgroup ctrlboard_external External
 *  @{
 */
#define EXTERNAL_BUFFER_SIZE 64 ///< External buffer size

typedef enum
{
    EXTERNAL_SHOW_MSG,  ///< Show message
    EXTERNAL_TEST0,     ///< Test #0
    EXTERNAL_TEST1,     ///< Test #1
    EXTERNAL_TEST2,     ///< Test #2
    EXTERNAL_TEST3,     ///< Test #3
    EXTERNAL_TEST4,     ///< Test #4
    EXTERNAL_TEST5      ///< Test #5
} ExternalEventType;

typedef struct
{
    ExternalEventType type;
    int arg1;
    int arg2;
    int arg3;
    uint8_t buf1[EXTERNAL_BUFFER_SIZE];

} ExternalEvent;

/**
 * Initializes external module.
 */
void ExternalInit(void);

/**
 * Exits external module.
 */
void ExternalExit(void);

/**
 * Receives external module event.
 *
 * @param ev The external event.
 * @return 0 for no event yet, otherwise for event occured.
 */
int ExternalReceive(ExternalEvent* ev);

/**
 * Sends external module event.
 *
 * @param ev The external event.
 * @return 0 for success, otherwise for failure.
 */
int ExternalSend(ExternalEvent* ev);

/**
 * Initializes external process module.
 */
void ExternalProcessInit(void);

/**
 * Processes external module event.
 *
 * @param ev The external event.
 * @return 0 for no event yet, otherwise for event occured.
 */
void ExternalProcessEvent(ExternalEvent* ev);

/** @} */ // end of ctrlboard_external

/** @defgroup ctrlboard upgarde_uart
 *  @{
 */
#if defined(CFG_UPGRADE_FROM_UART_RUN_TIME)
#if defined(CFG_UPGRADE_UART0)
#define UPGRADE_UART_PORT	ITP_DEVICE_UART0
#elif defined(CFG_UPGRADE_UART1)
#define UPGRADE_UART_PORT	ITP_DEVICE_UART1
#else
#define UPGRADE_UART_PORT	ITP_DEVICE_UART0
#endif
#define UPGRADE_PATTERN				0x1A

#define ACK20						0x14
#define ACK50						0x32
#define ACK100						0x64
#define ACK150						0x96
#define ACK200						0xC8
#define ACK210						0xD2
#define ACK211						0xD3
#define ACK220						0xDC
#define ACK221						0xDD

//the total check times is CHECK_NUM or CHECK_NUM+1
#define CHECK_NUM			4		
#define RETRY_SIZE			5
#define RETRY_CHECKSUM		1
#define RETRY_DATA			1

/**
 * Initializes Upgrade Fw by Uart module.
 */
void UpgradeUartInit(void);

/**
 * Exits Upgrade Fw by Uart module.
 */
void UpgradeUartExit(void);
#endif
/** @} */ // end of ctrlboard upgarde_uart

/** @defgroup ctrlboard_network Network
 *  @{
 */
/**
 * Initializes network module.
 */
void NetworkInit(void);
void NetworkWifiModeSwitch(void);

/**
 * Network pre-setting.
 */
void NetworkPreSetting(void);
void NetworkWifiPreSetting(void);

/**
 * Network function process.
 */
void NetworkEthernetProcess(void);
void NetworkWifiProcess(void);

/**
 * Exits network module.
 */
void NetworkExit(void);
bool NetworkIsExit(void);

/**
 * Resets network module.
 */
void NetworkReset(void);

/**
 * Determines whether the network(Ethernet) is ready or not.
 *
 * @return true for ready; false for net ready yet.
 */
bool NetworkIsReady(void);

/**
 * Determines whether the network(Ethernet) and Center Server are both ready or not.
 *
 * @return true for ready; false for net ready yet.
 */
bool NetworkServerIsReady(void);

/**
 * Determines whether the network(WIFI) is ready or not.
 *
 * @return true for ready; false for net ready yet.
 */
bool NetworkWifiIsReady(void);

/** @} */ // end of ctrlboard_network

/** @defgroup ctrlboard_photo Photo Loader
 *  @{
 */

typedef void (*PhotoLoadCallback)(uint8_t* data, int size);

void PhotoInit(void);

void PhotoExit(void);

int PhotoLoad(char* filename, PhotoLoadCallback func);

/** @} */ // end of ctrlboard_photo

/** @defgroup ctrlboard_screen Screen
 *  @{
 */
/**
 * Screensaver type definition.
 */
typedef enum
{
    SCREENSAVER_NONE,   ///< No screensaver
    SCREENSAVER_CLOCK,  ///< Clock sreensaver
    SCREENSAVER_BLANK,  ///< Black screen screensaver
    SCREENSAVER_PHOTO   ///< Photo screensaver
} ScreensaverType;

/**
 * Initializes screen module.
 */
void ScreenInit(void);

/**
 * Turns off screen.
 */
void ScreenOff(void);

/**
 * Turns on screen.
 */
void ScreenOn(void);

/**
 * Is the screen off or on.
 *
 * @return true for off; false for on.
 */
bool ScreenIsOff(void);

/**
 * Sets the brightness.
 *
 * @param value The brightness value.
 */
void ScreenSetBrightness(int value);

/**
 * Gets the maximum brightness level.
 *
 * @return the maximum brightness level.
 */
int ScreenGetMaxBrightness(void);

/**
 * Re-counts the time to enter screensaver.
 */
void ScreenSaverRefresh(void);

/**
 * Checks whether it is about time to enter screensaver mode.
 *
 * @return 0 for not yet, otherwise for entering screensaver mode currently.
 */
int ScreenSaverCheck(void);

/**
 * Is on screensaver mode or not.
 */
bool ScreenSaverIsScreenSaving(void);

/**
 * Takes a screenshot to USB drive.
 *
 * @param lcdSurf The LCD surface widget.
 */
void Screenshot(void* lcdSurf);

/**
 * Clears screen.
 *
 */
void ScreenClear(void);

/** @} */ // end of ctrlboard_screen

/** @defgroup ctrlboard_storage Storage
 *  @{
 */

typedef enum
{
    STORAGE_NONE = -1,
    STORAGE_USB,
    STORAGE_SD,
    STORAGE_INTERNAL,

    STORAGE_MAX_COUNT
} StorageType;

typedef enum
{
    STORAGE_UNKNOWN,
    STORAGE_SD_INSERTED,
    STORAGE_SD_REMOVED,
    STORAGE_USB_INSERTED,
    STORAGE_USB_REMOVED,
    STORAGE_USB_DEVICE_INSERTED,
    STORAGE_USB_DEVICE_REMOVED
} StorageAction;

/**
 * Initializes storage module.
 */
void StorageInit(void);

StorageAction StorageCheck(void);
StorageType StorageGetCurrType(void);
void StorageSetCurrType(StorageType type);
char* StorageGetDrivePath(StorageType type);
bool StorageIsInUsbDeviceMode(void);

/** @} */ // end of ctrlboard_storage

/** @defgroup ctrlboard_string String
 *  @{
 */
/**
 * Guard sensors definition.
 */
typedef enum
{
    GUARD_EMERGENCY,    ///< Emergency sensor
    GUARD_INFRARED,     ///< Infrared sensor
    GUARD_DOOR,         ///< Door sensor
    GUARD_WINDOW,       ///< Window sensor
    GUARD_SMOKE,        ///< Smoke sensor
    GUARD_GAS,          ///< Gas sensor
    GUARD_AREA,         ///< Area sensor
    GUARD_ROB,          ///< Rob sensor

    GUARD_SENSOR_COUNT  ///< Total sensor count
} GuardSensor;

/**
 * Gets the description of guard sensor.
 *
 * @param sensor The guard sensor.
 * @return the string of guard sensor.
 */
const char* StringGetGuardSensor(GuardSensor sensor);

/**
 * Gets the description of WiFi connected.
 *
 * @return the string of WiFi connected.
 */
const char* StringGetWiFiConnected(void);

/** @} */ // end of ctrlboard_string

/** @defgroup ctrlboard_upgrade Upgrade
 *  @{
 */
/**
 * Quit value definition.
 */
typedef enum
{
    QUIT_NONE,                  ///< Do not quit
    QUIT_DEFAULT,               ///< Quit for nothing
    QUIT_RESET_FACTORY,         ///< Quit to reset to factory setting
    QUIT_UPGRADE_FIRMWARE,      ///< Quit to upgrade firmware
    QUIT_UPGRADE_WEB,           ///< Quit to wait web upgrading
    QUIT_RESET_NETWORK,         ///< Quit to reset network
    QUIT_UPGRADE_UART			///< Quit to upgrade firmware by Uart
} QuitValue;

/**
 * Initializes upgrade module.
 *
 * @return 0 for initializing success, non-zero for initializing failed and the value will be the QuitValue.
 */
int UpgradeInit(void);

/**
 * Sets the CRC value of the specified file path.
 *
 * @param filepath The file path to update the CRC value.
 */
void UpgradeSetFileCrc(char* filepath);

/**
 * Sets the URL to upgrade.
 *
 * @param url The url to download and upgrade.
 */
void UpgradeSetUrl(char* url);

/**
 * Sets the stream to upgrade.
 *
 * @param stream The stream to upgrade.
 */
void UpgradeSetStream(void* stream);

/**
 * Processes the upgrade procedure by QuitValue.
 *
 * @param code The QuitValue.
 * @return 0 for process success; otherwise failed.
 */
int UpgradeProcess(int code);

/**
 * Is upgrading ready or not.
 *
 * @return true for ready; otherwise not ready yet.
 */
bool UpgradeIsReady(void);

/**
 * Is upgrading finished or not.
 *
 * @return true for finished; otherwise not finish yet.
 */
bool UpgradeIsFinished(void);

/**
 * Gets the upgrading result.
 *
 * @return 0 for success, failed otherwise.
 */
int UpgradeGetResult(void);

/** @} */ // end of ctrlboard_upgrade

/** @defgroup ctrlboard_webserver Web Server
 *  @{
 */
/**
 * Initializes web server module.
 */
void WebServerInit(void);

/**
 * Exits web server module.
 */
void WebServerExit(void);

/** @} */ // end of ctrlboard_webserver

void PlayVideo(int x, int y, int width, int height, int bgColor, int volume);
void WaitPlayVideoFinish(void);

void PlayMjpeg(int x, int y, int width, int height, int bgColor, int volume);
void WaitPlayMjpegFinish(void);

#ifdef	CFG_DYNAMIC_LOAD_TP_MODULE
void DynamicLoadTpModule(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* CTRLBOARD_H */
/** @} */ // end of ctrlboard