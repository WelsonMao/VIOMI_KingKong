
#ifndef      __PROTOCOL_H__
#define  __PROTOCOL_H__

/*******************************************************************
 *******************************************************************/
#ifdef      EXTERN
#undef  EXTERN
#endif

#ifdef   __PROTOCOL_C__
#define EXTERN
#else
#define EXTERN  extern 
#endif
/*******************************************************************
 *******************************************************************/
#define SETB( X, Y )                     ( ( X ) |= ( 0x00000001 << ( Y ) ) ) 
#define CLRB( X, Y )                     ( ( X ) &= ( ~( 0x00000001 << ( Y ) ) ) )
#define CPLB( X, Y )                     ( ( X ) ^= ( 0x00000001 << ( Y ) ) )
#define TESTB( X, Y )                    ( ( X ) & ( 0x00000001 << ( Y ) ) ) 
/*******************************************************************
 *******************************************************************/
#define C_PROTOCOL_FRAME_END                 ( 0xc0 )
#define C_PROTOCOL_FRAME_ESC                 ( 0xdb )
#define C_PROTOCOL_FRAME_ESC_END             ( 0xdc )
#define C_PROTOCOL_FRAME_ESC_ESC             ( 0xdd )

#define C_WIFI_FRAME_START                   ( 0xAA )


#define C_PROTOCOL_MSG_ID_STOP               ( 0 )
#define C_PROTOCOL_MSG_ID_UI_QUERY           ( 1 )
#define C_PROTOCOL_MSG_ID_UI_SEND_STATE		 ( 2 )
#define C_PROTOCOL_MSG_ID_UI_SEND_CYCLE      ( 3 )
#define C_PROTOCOL_MSG_ID_UI_ENTER_OTA       ( 4 )

#define C_PROTOCOL_MSG_ID_USER_MODE          ( 10 )
#define C_PROTOCOL_MSG_ID_SERVE_MODE         ( 11 )
#define C_PROTOCOL_MSG_ID_FACTORY            ( 12 )
#define C_PROTOCOL_MSG_ID_POWER_DOWN_MEMORY  ( 13 )
#define C_PROTOCOL_MSG_ID_WIFI_MSG           ( 14 )
#define C_PROTOCOL_MSG_ID_UI_ACK_MC          ( 0xfe )

/*=======================================================================
  =======================================================================*/
#define C_PROTOCOL_SEND_FRAME_LENGTH         (100 )
#define C_PROTOCOL_RECEIVE_FARME_LENGTH      ( 100 )
#define C_PROTOCOL_WIFI_FRAME_LENGTH          ( 64 )
/*******************************************************************
  锟斤拷锟斤拷锟斤拷锟酵讹拷锟斤拷锟斤拷
 *******************************************************************/
typedef struct _STRUCT_PROTOCOL_{
	volatile char  u8ProtocolFlag;
	uint8_t  u8ReceiveFrameLength;
	uint8_t  u8SendFrameNumber;
	volatile char  u8ReceiveFrameNumber;
	uint8_t  u8aFrameSendArray[C_PROTOCOL_SEND_FRAME_LENGTH];
	volatile uint8_t  u8aFrameReceiveArray[C_PROTOCOL_RECEIVE_FARME_LENGTH];
	char  u8WifiReceiveArray[C_PROTOCOL_WIFI_FRAME_LENGTH];
	uint8_t  u8FrameSendAckArray[10];
}STRUCT_PROTOCOL;

typedef struct _STRUCT_KEY_TO_PROCOTOL_KEY_{
	char eNormalKeyCode;
	char eProcotolKeyCode;
	char eProcotolKeyId;
}STRUCT_KEY_TO_PROCOTOL_KEY;
typedef enum
{
   PROTOCOL_STATE_NULL=0,
   PROTOCOL_STATE_IDLE = 1,
   PROTOCOL_STATE_STANDBY=2,
   PROTOCOL_STATE_RUN = 3,
   PROTOCOL_STATE_PAUSE = 4,
   PROTOCOL_STATE_EOC = 5,
   PROTOCOL_STATE_FAULT = 6,
   PROTOCOL_STATE_DELAY = 7,
   PROTOCOL_STATE_FACTORY=8,
   PROTOCOL_STATE_POWERMODE=9
}PROTOCOL_STATE_e;

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
}Bit_Set;

Bit_Set _fFuncInfo;
#define	_fFuncByte    			_fFuncInfo.byte
#define	bit_Protocol_Lamp		_fFuncInfo.bits.bit0
#define	bit_Protocol_Bubble     _fFuncInfo.bits.bit1
#define	bit_Protocol_Spray      _fFuncInfo.bits.bit2
#define	bit_Protocol_Repeat     _fFuncInfo.bits.bit3
#define	bit_Protocol_UV         _fFuncInfo.bits.bit4
#define	bit_Protocol_Taste      _fFuncInfo.bits.bit5
#define	bit_Protocol_Dry		_fFuncInfo.bits.bit6
#define	bit_Protocol_Resev7     _fFuncInfo.bits.bit7
/*************************************************************/
/*param define */
#define CTRL_ON                  1
#define CTRL_OFF                 0

#define STATUS_LIQ_LEVEL         0x01   
#define STATUS_PREWASH           0x02
#define STATUS_FROGWASH          0x04
#define STATUS_ACWASH            0x08
#define STATUS_NIGHTWASH         0x10
#define STATUS_CHILDLOCK         0x20
#define STATUS_DOOR              0x40
#define STATUS_SOFT_LEVEL        0x80

#define DIALOG_DRY               0x01
#define DIALOG_SPIN              0x02
#define DIALOG_RINSE             0x04
#define DIALOG_WASH              0x08
#define DIALOG_PREWASH           0x10
#define DIALOG_CHECK             0x20
#define DIALOG_WEIGHT            0x40

#define DIALOG_BEGIN_INDEX             0x00
#define DIALOG_DRY_INDEX               0x01
#define DIALOG_SPIN_INDEX              0x02
#define DIALOG_RINSE_INDEX             0x03
#define DIALOG_WASH_INDEX              0x04
#define DIALOG_PREWASH_INDEX           0x05
#define DIALOG_CHECK_INDEX             0x06
#define DIALOG_WEIGHT_INDEX            0x07
#define DIALOG_TASTE_INDEX             0x08
#define DIALOG_END_INDEX               0x09
/*************************************************************/
typedef union
{
	uint8_t byte;
	struct 
	{
		uint8_t Lamp : 1;
		uint8_t Bubble : 1;
		uint8_t Spray : 1;
		uint8_t Repeat : 1;
		uint8_t NewUV : 1;
		uint8_t NewTaste : 1;
		uint8_t NewDry : 1;
		uint8_t Reserv1 : 1;
	}bit;
}FUNC_u;
typedef struct
{
	uint8_t Cycle_ID;
	uint8_t Wash_Temper;
	uint8_t Speed;
	uint8_t PreWash;
	uint8_t ReservH;
	uint8_t ReservL;
	uint8_t Degerm;
	uint8_t Softer;
	uint8_t WaterLevel;
	uint8_t Dry;
	uint8_t Rinse_Num;
	uint8_t FrogWash;
	uint8_t MuteWash;
	uint8_t AcWash;
	uint8_t StainsWash;
	uint8_t Total_H;
	uint8_t Total_L;
	FUNC_u  Func;
	uint8_t FactoryStep;
}DataTransmitCycle_t;
typedef struct
{
	uint16_t  iWashTimeSet;
	uint16_t  iSoakTimeSet;
	uint16_t  iSpinTimeSet;
	uint16_t  iDryTimeSet;
	uint16_t  iRinseTimeSet;
	uint16_t  iReserTimeSet;
}DataTransmitTime_t;
typedef struct _STRUCT_USER_MODE_{
	uint8_t eWorkMode;                         /* 协议中DATA1  */
	uint8_t eWorkStatus;                       /* 协议中DATA2  */
	uint8_t eCycleProgram;                     /* 协议中DATA3  */
	uint8_t eErrorCode;                        /* 协议中DATA4  */
	uint8_t u8DelayTimeH;                      /* 协议中DATA5  */
	uint8_t u8DelayTimeL;                      /* 协议中DATA6  */
	uint8_t u8DryAndRinseCount;                /* 协议中DATA7  */
	uint8_t u8SetTemperatureLevel;             /* 协议中DATA8  */
	uint8_t u8SetSpeed;                        /* 协议中DATA9  */
	uint8_t u8FunctionOption;                  /* 协议中DATA10  */
	uint8_t u8RemainTimeHigh;                  /* 协议中DATA11 */
	uint8_t u8RemainTimeLow;                   /* 协议中DATA12 */
	uint8_t u8CycleFlag;                       /* 协议中DATA13 */
	uint8_t u8Weight;                          /* 协议中DATA14 */
	uint8_t u8Stains;                          /* 协议中DATA15 */
	uint8_t u8DryInWindSensorAD;               /* 协议中DATA16 */
	uint8_t u8DryOutWindSensorAD;              /* 协议中DATA17 */
	uint8_t eReturnKeyCode;                    /* 协议中DATA18 */
	uint8_t u8MachineStatus;                   /* 协议中DATA19 */
	uint8_t u8NowStep;                         /* 协议中DATA20 */
	uint8_t u8WaterLevelFrequencyHigh;         /* 协议中DATA21 */
	uint8_t u8WaterLevelFrequencyLow;          /* 协议中DATA22 */
	uint8_t u8WaterLevelH;                     /* 协议中DATA23 */
	uint8_t u8WaterLevelL;                     /* 协议中DATA24 */
	uint8_t u8SetWaterLevel;                   /* 协议中DATA25 */
	uint8_t u8NowSpeedHigh;                    /* 协议中DATA26 */
	uint8_t u8NowSpeedLow;                     /* 协议中DATA27 */
	uint8_t u8OOBValue;                        /* 协议中DATA28 */
	uint8_t u8WashAllTimeHigh;                 /* 协议中DATA29 */
	uint8_t u8WashAllTimeLow;                  /* 协议中DATA30 */
	uint8_t u8InjectState;                     /* 协议中DATA31 */
	uint8_t u8SetWashTime;                     /* 协议中DATA32 */
	uint8_t u8SetSpinTime;                     /* 协议中DATA33 */
	uint8_t u8DegermLevel;                     /* 协议中DATA34 */
	uint8_t u8StotenerLevel;                   /* 协议中DATA35 */
	FUNC_u  uFuncOption2;                      /* 协议中DATA36 */
	uint8_t u8MCVer;                           /* 协议中DATA37 */
	uint8_t u8MotorVerH;                       /* 协议中DATA38 */
	uint8_t u8MotorVerL;                       /* 协议中DATA39 */
	uint8_t u8WaterTemper;                     /* 协议中DATA40 */
	uint8_t u8TargetSpeedH;                    /* 协议中DATA41 */
	uint8_t u8TargetSpeedL;                    /* 协议中DATA42 */
	uint8_t u8TurbidityH;                      /* 协议中DATA43 */
	uint8_t u8TurbidityL;                      /* 协议中DATA44 */
	uint8_t u8WNTC;                            /* 协议中DATA45 */
	uint8_t u8WNTC_30S;                        /* 协议中DATA46 */
	uint8_t u8FatoryCStep;                     /* 协议中DATA47 */
	uint8_t u8WorkTimeTotalH;                  /* 协议中DATA48 */
	uint8_t u8WorkTimeTotalL;                  /* 协议中DATA49 */
	uint8_t u8PowerTotalH;                     /* 协议中DATA50 */
	uint8_t u8PowerTotalL;                     /* 协议中DATA51 */
	uint8_t u8WaterTotalH;                     /* 协议中DATA52 */
	uint8_t u8WaterTotalL;					   /* 协议中DATA53 */
	uint8_t u8WasherTotalH;                    /* 协议中DATA54 */
	uint8_t u8WasherTotalL;                    /* 协议中DATA55 */
	uint8_t u8SofterTotalH;                    /* 协议中DATA56 */
	uint8_t u8SofterTotalL;                    /* 协议中DATA57 */
	uint8_t u8TurbidityValH;                   /* 协议中DATA58*/
	uint8_t u8TurbidityValL;                   /* 协议中DATA59 */
}STRUCT_USER_MODE_DATA;
/*******************************************************************
  锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
 *******************************************************************/
extern char eUISendKeyCode;
extern uint16_t ekeyPhyPosition;
STRUCT_PROTOCOL sgProtocol;
STRUCT_USER_MODE_DATA tDataRev;
DataTransmitCycle_t tDataTransmitCycle, tDataTransmitCycleBack;
PROTOCOL_STATE_e  eTransmitCycleState;
//uint8_t eTransmitFactoryNext=0;
char  u8WifiSendArray[C_PROTOCOL_WIFI_FRAME_LENGTH];
char  u8WifiSendArraySize;

#define B_PF_ACK_FRAME_OK                   ( 0 )
#define B_PF_INFORMATION_FRAME_OK           ( 1 )
#define B_PF_USER_MODE_DECODE_OK            ( 2 )
/*******************************************************************
  锟斤拷锟捷憋拷锟斤拷
 *******************************************************************/
#ifdef  __PROTOCOL_C__
/*=======================================================================
  锟斤拷锟斤拷锟斤拷tProtocolKeyCode
  锟斤拷锟杰ｏ拷锟斤拷锟斤拷协锟斤拷锟接︼拷锟?
  =======================================================================*/
#endif /* __PROTOCOL_C__ */
/*******************************************************************
 *******************************************************************/
extern void SetUISendKeyCode( char eKeyCode );
extern void ProtocolArrayClear(char* pu8Org, char u8Length);
extern void ProtocolArrayCopy(char* pu8Src, char* pu8Aim, char u8Length);
extern void SetProtocolSendFrame(char u8MsgID);
extern void DecodeReceiveProtocolFrame(void);
extern void JudgeProtocolFrameOK(void);
extern void GetWifiCheckSumOK(void);
extern void ClearReceiveAckFrameOK(void);
extern void ClearReceiveInfromationFrameOK(void);
extern void SetReceiveFrameLength(char u8Length);

extern bool ReadReceiveInformationFrameOKStatus(void);
extern bool ReadReceiveAckFrameOKStatus(void);
extern bool JudgeReturnKeyCodeValidOrInvalid(char eKeyCode);
extern bool JudgeReturnKeyCodeValidOrInvalidTEST(char eKeyCode);
extern char GetWifiReceiveFlag(void);
extern uint8_t wifi_mark;
extern void SetWifiReceiveFlag(char flag);

//extern void ResponseHandle_CMD_Ctrl_ON_OFF(bool enable);
extern void ResponseHandle_CMD_SetCycle(void);
extern void ResponseHandle_CMD_CycleState(uint8_t cmd);
extern uint8_t DeviceStatusGet(uint8_t bit);
extern uint8_t GetACUState(void);
extern uint8_t GetACUStep(void);
extern uint8_t GetACUErr(void);
extern void SetACUErr(int id);
extern uint32_t GetCycleRemainTime(void);
extern uint32_t GetCycleDelayTime(void);
extern uint8_t GetACUMCVer(void);
extern uint16_t GetACUMoterVer(void);
extern uint8_t ProcessDialogGet(uint8_t step);
/*********************************************************/
#endif /* __PROTOCOL_H__ */


