#include <assert.h>
#include "../scene.h"
#include "UART_DataTrans.h"
#include "UART_Task.h"
#include "ite/itp.h"
#define  __PROTOCOL_C__
#undef   __PROTOCOL_C__

#if 1//( defined( PRJ_OPTION_PROTOCOL ) && ( PRJ_OPTION_PROTOCOL == ON ) )
/***************************************************************************************************
 ***************************************************************************************************/
char KeyCodeWatch;
char wifiReplyReceiveFlag;
//EXTERN uint8_t cyclevalue;
extern  uint16_t key_value;


void ProtocolArrayClear( char* pu8Org, char u8Length )
{
	char  u8tmp = 0;
	for( ; u8tmp < u8Length; u8tmp++ )
	{
		*pu8Org = 0;
		pu8Org++;
	}
}

void ProtocolArrayCopy( char* pu8Src, char* pu8Aim, char u8Length )
{
	for( ; u8Length > 0; u8Length-- )
	{
		*pu8Aim++ = *pu8Src++ ;
	}
}

static char GetProtocolFrameChecksum( char* pu8Org, char u8Length )
{
	char  u8Checksum = 0;
	char  u8tmp = 0;

	for( ; u8tmp < u8Length; u8tmp++ )
	{
		u8Checksum += *pu8Org;
		pu8Org++;
	}
	return( u8Checksum );
}

static void ProtocolFrameNumberAddOne( void )
{
	if( sgProtocol.u8SendFrameNumber != 0xff )
	{
		sgProtocol.u8SendFrameNumber++;
	}
	else
	{
		sgProtocol.u8SendFrameNumber = 1;
	}
}


static void ProtocolFrameSpecialCharToReplaceChar(uint8_t* pu8ProtocolArray, uint8_t u8Length)
{
	uint8_t  u8tmp0;

	for( u8tmp0 = 2; u8tmp0 < ( u8Length - 1 ); u8tmp0++ )
	{
		uint8_t u8tmp1 = *(pu8ProtocolArray + u8tmp0);

		do{
			if( C_PROTOCOL_FRAME_END == u8tmp1 )
			{
				*( pu8ProtocolArray + u8tmp0 ) = C_PROTOCOL_FRAME_ESC;
				u8tmp0++;
				u8Length++;
				u8tmp1 = u8Length;

				for( ; u8tmp1 > u8tmp0 ; u8tmp1-- )
				{
					*( pu8ProtocolArray + u8tmp1 ) = *( pu8ProtocolArray + u8tmp1 - 1 );
				}
				*( pu8ProtocolArray + u8tmp0 ) = C_PROTOCOL_FRAME_ESC_END;
				break;
			}

			if( C_PROTOCOL_FRAME_ESC == u8tmp1 )
			{
				*( pu8ProtocolArray + u8tmp0 ) = C_PROTOCOL_FRAME_ESC;
				u8tmp0++;
				u8Length++;
				u8tmp1 = u8Length;

				for( ; u8tmp1 > u8tmp0 ; u8tmp1-- )
				{
					*( pu8ProtocolArray + u8tmp1 ) = *( pu8ProtocolArray + u8tmp1 - 1 );
				}
				*( pu8ProtocolArray + u8tmp0 ) = C_PROTOCOL_FRAME_ESC_ESC;
			}

		}while( 0 );
	}
}

void SetProtocolSendFrame( char u8MsgID )
{
	char index,DataLength, key;
	//char* wifiBuffer;

	char *pu8tmp;
	if( C_PROTOCOL_MSG_ID_UI_ACK_MC == u8MsgID )
	{
		pu8tmp = sgProtocol.u8FrameSendAckArray;

		ProtocolArrayClear( sgProtocol.u8FrameSendAckArray, sizeof( sgProtocol.u8FrameSendAckArray ) );
		*pu8tmp++ = C_PROTOCOL_FRAME_END;
		*pu8tmp++ = C_PROTOCOL_MSG_ID_UI_ACK_MC;
		*pu8tmp++ = sgProtocol.u8ReceiveFrameNumber;
		*pu8tmp++ = 0 - GetProtocolFrameChecksum( ( char* )&sgProtocol.u8FrameSendAckArray[1], 2 );
		*pu8tmp = C_PROTOCOL_FRAME_END;
		ProtocolFrameSpecialCharToReplaceChar( sgProtocol.u8FrameSendAckArray, 5 );
	}
	else
	{
		pu8tmp = sgProtocol.u8aFrameSendArray;

		ProtocolArrayClear( sgProtocol.u8aFrameSendArray, sizeof( sgProtocol.u8aFrameSendArray ) );
		*pu8tmp++ = C_PROTOCOL_FRAME_END;

		if( C_PROTOCOL_MSG_ID_UI_ENTER_OTA == u8MsgID )  //300ms loop
		{
			*pu8tmp++ = C_PROTOCOL_MSG_ID_UI_ENTER_OTA;
			*pu8tmp++ = bit_Protocol_Lamp;
			*pu8tmp++ = 0;
			//*pu8tmp++ = 0;// ReadProjectSoftwareVerson();
			ProtocolFrameNumberAddOne();
			*pu8tmp++ = sgProtocol.u8SendFrameNumber;
			*pu8tmp++ = 0 - GetProtocolFrameChecksum( ( char* )&sgProtocol.u8aFrameSendArray[1], 4 );
			*pu8tmp = C_PROTOCOL_FRAME_END;
			ProtocolFrameSpecialCharToReplaceChar( sgProtocol.u8aFrameSendArray, 7 );
		}
		else if (C_PROTOCOL_MSG_ID_UI_SEND_STATE == u8MsgID)
		{    
			*pu8tmp++ = C_PROTOCOL_MSG_ID_UI_SEND_STATE;
			*pu8tmp++ = eTransmitCycleState;
			*pu8tmp++ = tDataTransmitCycle.FactoryStep;
			ProtocolFrameNumberAddOne();
			*pu8tmp++ =  sgProtocol.u8SendFrameNumber;
			*pu8tmp++ = 0 - GetProtocolFrameChecksum( ( char* )&sgProtocol.u8aFrameSendArray[1], 4 );
			*pu8tmp = C_PROTOCOL_FRAME_END;
			//printf("BuildState:%#x-%#x\n", eTransmitCycleState, tDataTransmitCycle.FactoryStep);
			ProtocolFrameSpecialCharToReplaceChar( sgProtocol.u8aFrameSendArray, 7 );		
		}
		else if (C_PROTOCOL_MSG_ID_UI_SEND_CYCLE == u8MsgID)
		{    
			*pu8tmp++ = C_PROTOCOL_MSG_ID_UI_SEND_CYCLE;
			*pu8tmp++ = tDataTransmitCycle.Cycle_ID;// ReadCycleSendKeyCode();
			*pu8tmp++ = tDataTransmitCycle.Wash_Temper;
			*pu8tmp++ = tDataTransmitCycle.Speed;
			*pu8tmp++ = tDataTransmitCycle.PreWash;
			*pu8tmp++ = tDataTransmitCycle.ReservH;
			*pu8tmp++ = tDataTransmitCycle.ReservL;
			*pu8tmp++ = tDataTransmitCycle.Degerm;
			*pu8tmp++ = tDataTransmitCycle.Softer;
			*pu8tmp++ = tDataTransmitCycle.WaterLevel;
			*pu8tmp++ = tDataTransmitCycle.Dry;
			*pu8tmp++ = tDataTransmitCycle.Rinse_Num;
			*pu8tmp++ = tDataTransmitCycle.FrogWash;
			*pu8tmp++ = tDataTransmitCycle.MuteWash;
			*pu8tmp++ = tDataTransmitCycle.AcWash;
			*pu8tmp++ = tDataTransmitCycle.StainsWash;
			*pu8tmp++ = tDataTransmitCycle.Total_H;
			*pu8tmp++ = tDataTransmitCycle.Total_L;
			*pu8tmp++ = tDataTransmitCycle.Func.byte;
			ProtocolFrameNumberAddOne();
			*pu8tmp++ = sgProtocol.u8SendFrameNumber;
			*pu8tmp++ = 0 - GetProtocolFrameChecksum( ( char* )&sgProtocol.u8aFrameSendArray[1], 20 );
			*pu8tmp = C_PROTOCOL_FRAME_END;
			/*printf("Build:%x{", tDataTransmitCycle.Func.byte);
			for (int i = 0; i < 23; i++) printf("%#x ,", sgProtocol.u8aFrameSendArray[i]);
			printf("}\n");*/
			ProtocolFrameSpecialCharToReplaceChar( sgProtocol.u8aFrameSendArray, 23 );	
			tDataTransmitCycle.FactoryStep = 0;
			tDataTransmitCycle.ReservH = 0;
			tDataTransmitCycle.ReservL = 0;
		}
		else if( C_PROTOCOL_MSG_ID_UI_QUERY == u8MsgID )  //300ms loop
		{
			*pu8tmp++ = C_PROTOCOL_MSG_ID_UI_QUERY;
			*pu8tmp++ = bit_Protocol_Lamp;
			*pu8tmp++ = 0;
			//*pu8tmp++ = 0;// ReadProjectSoftwareVerson();
			ProtocolFrameNumberAddOne();
			*pu8tmp++ = sgProtocol.u8SendFrameNumber;
			*pu8tmp++ = 0 - GetProtocolFrameChecksum( ( char* )&sgProtocol.u8aFrameSendArray[1], 4 );
			*pu8tmp = C_PROTOCOL_FRAME_END;
			ProtocolFrameSpecialCharToReplaceChar( sgProtocol.u8aFrameSendArray, 7 );
		}
	}
}


void SetReceiveFrameLength( char u8Length )
{
	sgProtocol.u8ReceiveFrameLength = u8Length;
}

void JudgeProtocolFrameOK( void )
{
	uint8_t u8tmp0;

	CLRB( sgProtocol.u8ProtocolFlag, B_PF_ACK_FRAME_OK );
	CLRB( sgProtocol.u8ProtocolFlag, B_PF_INFORMATION_FRAME_OK );

	//  END
	//printf("JudgeProtocolFrameOK:arry[0]=%#x,arry[%d]=%#x\n", sgProtocol.u8aFrameReceiveArray[0], sgProtocol.u8ReceiveFrameLength, sgProtocol.u8aFrameReceiveArray[sgProtocol.u8ReceiveFrameLength]);
	if( (C_PROTOCOL_FRAME_END != sgProtocol.u8aFrameReceiveArray[0]) || 
			(C_PROTOCOL_FRAME_END != sgProtocol.u8aFrameReceiveArray[sgProtocol.u8ReceiveFrameLength])  )
	{
		return;
	}
	//   
	u8tmp0 = 0 - GetProtocolFrameChecksum( 
			( char* )&sgProtocol.u8aFrameReceiveArray[1], 
			sgProtocol.u8ReceiveFrameLength - 2
			);
	//printf("JudgeProtocolFrameOK:u8tmp0=%#x->%#x\n", u8tmp0, sgProtocol.u8aFrameReceiveArray[sgProtocol.u8ReceiveFrameLength - 1]);
	if( sgProtocol.u8aFrameReceiveArray[sgProtocol.u8ReceiveFrameLength - 1] != u8tmp0 )
	{
		return;
	}

	// ACKFRAME_OK
	//printf("JudgeProtocolFrameOK:%#x,%#x->%#x\n", sgProtocol.u8aFrameReceiveArray[1], sgProtocol.u8SendFrameNumber, sgProtocol.u8aFrameReceiveArray[2]);
	if( C_PROTOCOL_MSG_ID_UI_ACK_MC == sgProtocol.u8aFrameReceiveArray[1] )
	{
		if( sgProtocol.u8SendFrameNumber == sgProtocol.u8aFrameReceiveArray[2] )
		{
			SETB( sgProtocol.u8ProtocolFlag, B_PF_ACK_FRAME_OK );
		}
	}
	else
	{
		SETB( sgProtocol.u8ProtocolFlag, B_PF_INFORMATION_FRAME_OK );
	}
}
bool ReadReceiveInformationFrameOKStatus( void )
{
	//printf("ReadReceiveInformationFrameOKStatus:%d\n", TESTB(sgProtocol.u8ProtocolFlag, B_PF_INFORMATION_FRAME_OK));
	if( TESTB( sgProtocol.u8ProtocolFlag, B_PF_INFORMATION_FRAME_OK ) )
		return(true);
	else
		return( false );
}
bool ReadReceiveAckFrameOKStatus(void)
{
	if( TESTB( sgProtocol.u8ProtocolFlag, B_PF_ACK_FRAME_OK ) )
		return(true);
	else
		return(false);
}
/* =======================================================================
   ClearReceiveAckFrameOK
   OK



   2011-01-10
   ================================================================== */
void ClearReceiveAckFrameOK( void )
{
	CLRB( sgProtocol.u8ProtocolFlag, B_PF_ACK_FRAME_OK ); 
}
/* =======================================================================
   ClearReceiveInfromationFrameOK
   OK



   2011-01-10
   ================================================================== */
void ClearReceiveInfromationFrameOK( void )
{
	CLRB( sgProtocol.u8ProtocolFlag, B_PF_INFORMATION_FRAME_OK );
}
/*---------------------------------------------------------------------*/
/************************************************************************/
uint8_t DeviceStatusGet(uint8_t bit)
{
	return  1;//(tDataRev.u8FunctionOption&bit);
}
uint8_t GetACUState(void)
{
	return (tDataRev.eWorkStatus);
}
uint8_t GetACUStep(void)
{
	static uint8_t step = 0;
	if (step != tDataRev.u8CycleFlag){
		printf("GetACUStep:%d-%d\n", step, tDataRev.u8CycleFlag);
		step = tDataRev.u8CycleFlag;
	}
	return (tDataRev.u8CycleFlag);
}
uint32_t GetCycleRemainTime(void)
{
#if 0//def _WIN32
	tDataRev.u8RemainTimeHigh = (uint8_t)(180 >> 8)&0xFF;
	tDataRev.u8RemainTimeLow = (uint8_t)(180 >> 0) & 0xFF;
#endif
	return (uint32_t)(tDataRev.u8RemainTimeHigh << 8 | tDataRev.u8RemainTimeLow) * 60;
}
uint32_t GetCycleDelayTime(void)
{
	return (uint32_t)(tDataRev.u8DelayTimeH << 8 | tDataRev.u8DelayTimeL);
}
uint8_t GetACUErr(void)
{
	return (tDataRev.eErrorCode);
}
void SetACUErr(int id)
{
	tDataRev.eErrorCode = id;
}
uint8_t ProcessDialogGet(uint8_t step)
{
	/*int pos = 0;
	if (step){
		for (pos = 0; pos < 8; pos++){
			if (step&(1 << pos)){
				pos++;
				break;
			}
		}
	}
	return pos;*/
	return step;
}
uint8_t GetACUMCVer(void)
{
	return (tDataRev.u8MCVer);
}
uint16_t GetACUMoterVer(void)
{
	uint16_t val = (uint16_t)(tDataRev.u8MotorVerH << 8 | tDataRev.u8MotorVerL);
	return val;
}
uint32_t period = 0;
void ResponseHandle_CMD_SetCycle(void)
{
	printf("====%s\n", __func__);
	period = itpGetTickCount();
	SetUartNextSendFrameID(C_PROTOCOL_MSG_ID_UI_SEND_CYCLE);
}
void ResponseHandle_CMD_CycleState(uint8_t cmd)
{
	printf("====%s:%#x-%d\n", __func__, cmd, itpGetTickDuration(period));
	eTransmitCycleState = cmd;
	SetUartNextSendFrameID(C_PROTOCOL_MSG_ID_UI_SEND_STATE);
}
/*---------------------------------------------------------------------*/
/* ************************************************************************************************ */
#endif /*  PRJ_OPTION_PROTOCOL  */
/* ******************************************************************
   END OF FILE
 ****************************************************************** */

