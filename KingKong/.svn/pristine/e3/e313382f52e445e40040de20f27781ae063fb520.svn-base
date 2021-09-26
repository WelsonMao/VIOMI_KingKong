#include <assert.h>
#include "../scene.h"
#ifndef     __UART_TASK_H__
#define __UART_TASK_H__
#define TIME_TICK                            (50)//MS
#define MS                                   (1)
#define SECOND                               (1000*MS/TIME_TICK)
#define MINUTE                               (60*SECOND)

#define C_UART_RECEIVE_LENGTH                ( 2 )
#define C_UART_SEND_LENGTH                   ( 2 )
#define C_UART_TRY_MAX_COUNT                 ( 5 )
#define C_UART_TRY_ACK_SPACE_TIME            ( 200 * MS / TIME_TICK )
#define C_UART_SEND_INFO_SPACE_TIME          ( 200 * MS / TIME_TICK )//800
#define C_UART_QUERY_NO_RESPONE_TIME         ( 1 * MINUTE )
#define C_UART_FAULT_CLEAR_TIME              ( 10 * SECOND )
#define C_UART_WAIT_MC_READY_TIME            ( 1 * SECOND / 10 )
#define C_UART_LINK_FAULT_TIME               ( 10 * SECOND )
#define C_UART_RECEIVE_FRAME_OVERFLOW_TIME   ( 1 * SECOND / 5 )
#define C_UART_FRAME_SEND_SPACE_TIME         ( 1 * SECOND / 10 )
 
#define C_UART_INIT_LINK_FAULT               ( 0x80 )
#define C_UART_NORMAL_FAULT                  ( 0x80 )
#define UART_USE_QUEUE                       (1)
#if 0
enum  ENUM_UART_FAULT{
	               E_UART_NO_FAULT,
		          E_UART_INIT_LINK_FAULT,
		          E_UART_NORMAL_FAULT
	             };
#endif

/*******************************************************************
                                         锟斤拷锟斤拷锟斤拷锟酵讹拷锟斤拷锟斤拷
*******************************************************************/
typedef struct _STRUCT_UART_TASK_{
				   char   u8SendFrameID;
                  //   char   eUartFaultType;
				   char   u8UartTryCount;
				   char   u8UartTryAckSpaceTime;
				   char   u8NextSendFrameID[2];
				   uint16_t   u8UartSendInfoSpaceTime;
				   uint16_t   u8UartReceiveTimeOverFlow;
				   uint16_t  u16UartFaultCheckTime;
				   uint16_t  u16UartStartOrPauseKeyClrFaultTime;
                                 }STRUCT_UART_TASK;

/*******************************************************************
                                          锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
*******************************************************************/
 STRUCT_UART_TASK sgUartTask;


/*******************************************************************
                                         锟疥函锟斤拷锟斤拷锟斤拷锟斤拷
*******************************************************************/
 extern uint32_t TaskCnt;
/*******************************************************************
                                       锟斤拷锟斤拷涌诤锟斤拷锟斤拷锟斤拷锟斤拷锟?
*******************************************************************/
extern void UartInit( void );
extern void TransmitSpaceTimeZero(void);
extern void SetUartNextSendFrameID( char u8PortocolFrameID );
extern void TransmitDataInit(void);
extern bool JudgeUartInitLinkOK( void );
extern void UartTask( void );
//extern BOOL ReadUartFaultStatus( void );
//extern void ClearUartStartOrPauseKeyClrFaultTime( void );
/*********************************************************/
#endif /* __UART_TASK_H__ */
/*******************************************************************
                                          END OF FILE
*******************************************************************/

