
#define  __UART_TASK_C__
#include <assert.h>
#include "ite/itp.h"
#include "UART_Task.h"
#include "UART_DataTrans.h"
#include "COM.h"
#include "task_queue.h"

#include "../WiFi/BEH_Wifi_pub.h"

#undef   __UART_TASK_C__
uint32_t TaskCnt = 0;
uint32_t TaskTick =0;
#if 1// ( defined( PRJ_OPTION_UART_TASK ) && ( PRJ_OPTION_UART_TASK == ON ) )
/* ******************************************************************
 ****************************************************************** */
enum ENUM_UART_LINK_STATUS{
	E_UART_INIT_LINKING,
	E_UART_INIT_LINK_OK
}; 

static char eUartInitLinkStatus = E_UART_INIT_LINKING;
/* ******************************************************************
 ****************************************************************** */
static void UartStartFrameSend( void );
static void SetUartNextSendFrame( void );
/* ******************************************************************
 ****************************************************************** */
/* =======================================================================
   ================================================================== */
void UartInit( void )
{
    uartTaskQueueInit();
    
	sgUartTask.u8SendFrameID = C_PROTOCOL_MSG_ID_STOP;
	sgUartTask.u8NextSendFrameID[0] = C_PROTOCOL_MSG_ID_STOP;
	sgUartTask.u8NextSendFrameID[1] = C_PROTOCOL_MSG_ID_STOP;
	sgUartTask.u8UartTryCount = 0;
}
void TransmitDataInit(void)
{
	memset(&tDataTransmitCycle, 0, sizeof(DataTransmitCycle_t));
}

void TransmitSpaceTimeZero(void)
{
    sgUartTask.u8UartSendInfoSpaceTime = 0;
}
/* =======================================================================
   ================================================================== */
void SetUartNextSendFrameID( char u8PortocolFrameID )
{
	if( C_PROTOCOL_MSG_ID_UI_ACK_MC == u8PortocolFrameID )
	{
		sgUartTask.u8NextSendFrameID[1] = u8PortocolFrameID;
	}
	else
	{	
		//if( (C_PROTOCOL_MSG_ID_UI_QUERY == sgUartTask.u8NextSendFrameID[0] || C_PROTOCOL_MSG_ID_STOP == sgUartTask.u8NextSendFrameID[0]))
		{
			//printf("=====%#x->%#x\n", sgUartTask.u8NextSendFrameID[0], u8PortocolFrameID);
#if UART_USE_QUEUE
            uartTaskQueueIn(TYPE_CONTROL, u8PortocolFrameID, NULL, 0);
            //wsf_log("uartTaskQueueIn:[%d]", u8PortocolFrameID);
            //uartTaskPrintTest();
#else
            sgUartTask.u8NextSendFrameID[0] = u8PortocolFrameID;
#endif
            if (u8PortocolFrameID != C_PROTOCOL_MSG_ID_UI_QUERY){
                sgUartTask.u8UartSendInfoSpaceTime = 0;
            }
		}
	}
}
/* =======================================================================
   ================================================================== */
static void SetUartNextSendFrame( void )
{
	char  u8PortocolFrameID;
	//printf("SetUartNextSendFrame:data0=%#x,data1=%#x,try=%#x,status=%#x\n",\
		sgUartTask.u8NextSendFrameID[0], sgUartTask.u8NextSendFrameID[1], sgUartTask.u8UartTryCount, GetUartSendReadyStatus());
	if( C_PROTOCOL_MSG_ID_UI_ACK_MC == sgUartTask.u8NextSendFrameID[1] )
	{
		if ( GetUartSendReadyStatus() )
		{
			u8PortocolFrameID = C_PROTOCOL_MSG_ID_UI_ACK_MC;
			sgUartTask.u8NextSendFrameID[1] = C_PROTOCOL_MSG_ID_STOP;
			SetProtocolSendFrame( u8PortocolFrameID );
			StartUartSend( sgProtocol.u8FrameSendAckArray, C_PROTOCOL_FRAME_END );
			printf("1:SetProtocolSendFrame%d\n", u8PortocolFrameID);
		}
	}
	else
	{
		if( 0 != sgUartTask.u8UartTryCount )
		{
			//printf("RETRY:%d\n", sgUartTask.u8UartTryCount);
			return;
		}
#if UART_USE_QUEUE
        if( C_PROTOCOL_MSG_ID_STOP == sgUartTask.u8NextSendFrameID[0] )
        {
            sgUartTask.u8NextSendFrameID[0] = uartTaskQueueOut();
            if(C_PROTOCOL_MSG_ID_STOP != sgUartTask.u8NextSendFrameID[0]){
                //wsf_log("uartTaskQueueOut:[%d]", sgUartTask.u8NextSendFrameID[0]);
                u8PortocolFrameID = sgUartTask.u8NextSendFrameID[0];
                sgUartTask.u8UartTryCount = C_UART_TRY_MAX_COUNT + 1;
			    sgUartTask.u8NextSendFrameID[0] = C_PROTOCOL_MSG_ID_STOP;
            }
            else{
                return;
            }
        }
#else
		if( C_PROTOCOL_MSG_ID_STOP != sgUartTask.u8NextSendFrameID[0] )
		{
			u8PortocolFrameID = sgUartTask.u8NextSendFrameID[0];
            sgUartTask.u8UartTryCount = C_UART_TRY_MAX_COUNT + 1;
			sgUartTask.u8NextSendFrameID[0] = C_PROTOCOL_MSG_ID_STOP;
		}
#endif
		else
		{
			return;
		}
		sgUartTask.u8UartTryAckSpaceTime = C_UART_TRY_ACK_SPACE_TIME;
		SetProtocolSendFrame( u8PortocolFrameID );
		//printf("12:SetProtocolSendFrame%d\n", u8PortocolFrameID);
		sgUartTask.u8SendFrameID = u8PortocolFrameID;
		sgUartTask.u8UartSendInfoSpaceTime = 0;
	}
}
/* ==================================================================
   ================================================================== */
static void UartStartFrameSend( void )
{
	//printf("UartStartFrameSend:space=%#x,try=%#x,%d\n", sgUartTask.u8UartTryAckSpaceTime, sgUartTask.u8UartTryCount, GetUartSendReadyStatus());
	if( sgUartTask.u8UartTryAckSpaceTime < C_UART_TRY_ACK_SPACE_TIME )
	{
		sgUartTask.u8UartTryAckSpaceTime++;
	}
	//printf("UartStartFrameSend:space=%#x,try=%#x,flag=%#x\n", sgUartTask.u8UartTryAckSpaceTime, sgUartTask.u8UartTryCount, sgUartDrive.u8UartDriveFlag);
	if (!GetUartSendReadyStatus()){
		//printf("SendState=%d\n", GetUartSendReadyStatus());
		return;
	}
	if( sgUartTask.u8UartTryCount > C_UART_TRY_MAX_COUNT ){//first package.
	}
	else if( 0 == sgUartTask.u8UartTryCount ){
		return;
	}
	else{
		if( sgUartTask.u8UartTryAckSpaceTime < C_UART_TRY_ACK_SPACE_TIME ){//retry duty.
			return;
		}
	}
    sgUartTask.u8UartSendInfoSpaceTime = 0;
	sgUartTask.u8UartTryAckSpaceTime = 0;
    sgUartTask.u8UartReceiveTimeOverFlow = 0;
    if(C_PROTOCOL_MSG_ID_UI_QUERY == sgProtocol.u8aFrameSendArray[1]){
	    sgUartTask.u8UartTryCount = 0;
    }
    else{
        sgUartTask.u8UartTryCount--;
    }

	{                                           /*  ���²���Ϊ��ֹ��������ʼ���߽�����־���Ե�����  */
		char u8tmp0 = 0;
		char u8tmp1 = 0;
		for( ; u8tmp0 < sizeof( sgProtocol.u8aFrameSendArray ); u8tmp0++ )
		{
			if( C_PROTOCOL_FRAME_END == sgProtocol.u8aFrameSendArray[u8tmp0] )
			{
				u8tmp1++;

				if( 2 == u8tmp1 )
				{
					sgUartDrive.u8UartSendLen = u8tmp0+1;
#if 1
					StartUartSend( sgProtocol.u8aFrameSendArray, C_PROTOCOL_FRAME_END ); //֡ͷ END
					//printf("test\n");
#endif
					StartUartReceive( ( uint8_t* )sgProtocol.u8aFrameReceiveArray, C_PROTOCOL_FRAME_END );
					break;
				}
			}
		}
	}

}

/* =======================================================================
   ================================================================== */
bool JudgeUartInitLinkOK( void )
{
	if (E_UART_INIT_LINK_OK == eUartInitLinkStatus)  return(true);
	else                                             return(false);
}

void DecodeReceiveProtocolFrame(void)
{
	uint8_t  u8Index, u8tmp = 0;
	char* pu8tmp;
	sgProtocol.u8ReceiveFrameNumber = sgProtocol.u8aFrameReceiveArray[sgProtocol.u8ReceiveFrameLength - 2];
	if (C_PROTOCOL_MSG_ID_USER_MODE == sgProtocol.u8aFrameReceiveArray[1]){
		pu8tmp = (uint8_t*)&tDataRev;
		u8tmp = sizeof(tDataRev);
	}
	for (u8Index = 2; u8Index < (u8tmp + 2); u8Index++){
		*pu8tmp = sgProtocol.u8aFrameReceiveArray[u8Index];
		pu8tmp++;
	}
	//printf("DecodeReceiveProtocolFrame:ERR[%#x],STATE[%#x],time=%d-%d\n", tDataRev.eErrorCode, tDataRev.u8CycleFlag, tDataRev.u8RemainTimeHigh, tDataRev.u8RemainTimeLow);
}
/* =======================================================================
   ================================================================== */
void UartTask( void )
{
	static char u8UartFaultResetTime = 0;
	
	
   /*if (100 == TaskCnt){
		printf("TaskCnt=%d,TaskTick=%d\n", TaskCnt, itpGetTickDuration(TaskTick));
		TaskCnt = 0;
		TaskTick = itpGetTickCount();
	}
	TaskCnt++;*/
	//static char u8UartRemoteFaultResetTime = 0;
	//printf("u16UartFaultCheckTime=%d,eUartInitLinkStatus=%d,u8UartSendInfoSpaceTime=%d\n", sgUartTask.u16UartFaultCheckTime, eUartInitLinkStatus, sgUartTask.u8UartSendInfoSpaceTime);
	if (sgUartTask.u16UartFaultCheckTime <= C_UART_QUERY_NO_RESPONE_TIME) sgUartTask.u16UartFaultCheckTime++;

	if( E_UART_INIT_LINKING == eUartInitLinkStatus ){
		if( sgUartTask.u16UartFaultCheckTime < C_UART_WAIT_MC_READY_TIME ){
			sgUartTask.u8UartSendInfoSpaceTime = C_UART_SEND_INFO_SPACE_TIME; 
			return;
		}
	}

	if (sgUartTask.u8UartSendInfoSpaceTime < C_UART_SEND_INFO_SPACE_TIME) sgUartTask.u8UartSendInfoSpaceTime++;
	//printf("%d-%d\n", sgUartTask.u8UartSendInfoSpaceTime, C_UART_SEND_INFO_SPACE_TIME);
	if (sgUartTask.u8UartSendInfoSpaceTime >= C_UART_SEND_INFO_SPACE_TIME) {
		SetUartNextSendFrameID(C_PROTOCOL_MSG_ID_UI_QUERY);
		//printf("UartTask:C_PROTOCOL_MSG_ID_UI_QUERY:[%d]\n", sgUartTask.u8UartSendInfoSpaceTime);
		//sgUartTask.u8UartSendInfoSpaceTime = 0;
		
	}

	if( GetUartReceiveStatus()){
		sgUartTask.u8UartReceiveTimeOverFlow++;
		if( sgUartTask.u8UartReceiveTimeOverFlow >= C_UART_RECEIVE_FRAME_OVERFLOW_TIME ){
			ResetUartReceive();
			sgUartTask.u8UartReceiveTimeOverFlow = 0;
		}
	}
	else{
		sgUartTask.u8UartReceiveTimeOverFlow = 0;
	}
	/*  ACkFrameOK + InfomationFrameOK  */
	if( ReadReceiveInformationFrameOKStatus() ){

		ClearReceiveAckFrameOK();
		sgUartTask.u8UartTryAckSpaceTime = 0;
		sgUartTask.u8UartTryCount = 0;

		ProtocolArrayClear((uint8_t*)sgProtocol.u8aFrameSendArray, sizeof(sgProtocol.u8aFrameSendArray));
    	sgUartTask.u8UartReceiveTimeOverFlow = 0;
		ClearReceiveInfromationFrameOK();
		sgUartTask.u16UartFaultCheckTime = 0;
		u8UartFaultResetTime = 0;	
		DecodeReceiveProtocolFrame();
		//printf("Ver:%#x-%#x-%#x\n", tDataRev.u8MCVer, tDataRev.u8MotorVerH, tDataRev.u8MotorVerL);
		//ProtocolArrayClear( ( uint8_t* )sgProtocol.u8aFrameReceiveArray,  sizeof( sgProtocol.u8aFrameReceiveArray ) );
		SetReceiveFrameLength( 0 );
		// sgUartTask.eUartFaultType = E_UART_NO_FAULT;
		eUartInitLinkStatus = E_UART_INIT_LINK_OK;
		//SetUartNextSendFrameID( C_PROTOCOL_MSG_ID_UI_ACK_MC );       /*  ��������Ϣ��,����Ӧ���ź�  */
	}
	else{
		if( E_UART_INIT_LINKING == eUartInitLinkStatus )
		{
			if( sgUartTask.u16UartFaultCheckTime >= C_UART_LINK_FAULT_TIME ){
				//SetErrorCode( C_UART_INIT_LINK_FAULT );
			}
		}
		else {
			if( sgUartTask.u16UartFaultCheckTime >= C_UART_QUERY_NO_RESPONE_TIME ){
				//SetErrorCode( C_UART_NORMAL_FAULT );
			}
		}

		if( sgUartTask.u16UartFaultCheckTime >= 8 * SECOND ){
			if( 0 == u8UartFaultResetTime ){
				u8UartFaultResetTime = 0;
				DisableUart();
				ResetUartReceive();
			}
			u8UartFaultResetTime++;
			if (u8UartFaultResetTime >= 2 * SECOND) u8UartFaultResetTime = 0;
		}
		else{
			u8UartFaultResetTime = 0;
		}
	}
	//printf("Send\n");
	SetUartNextSendFrame(); 
#if 1
	UartStartFrameSend();
#endif
}


#if 0
/* =======================================================================
   ================================================================== */
bool ReadUartFaultStatus( void )
{
	if( E_UART_NO_FAULT == sgUartTask.eUartFaultType )  return( true );
	else                                                return( false );
}
#endif

/* ************************************************************************************************ */
#endif
/* ******************************************************************
   END OF FILE
 ****************************************************************** */

