#include <assert.h>
#include "../scene.h"
#include <sys/ioctl.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "../ctrlboard.h"
#include "ite/itp.h"
#include "ite/ith.h"
#include "Modbus.h"
#include "../extend/CRC.h"
#include  "uart_process.h"
#include "task_queue.h"

uint8_t Tx_Buf[256] = { 0 };
uint8_t Rx_Buf[256] = { 0 };
PDU_t tPdu;
/*==========================HostFunc=====================================*/
/*************************************************************************
* 函数功能: 读N个线圈状态(CoilStatue)
* 输入参数: _addr:从站地址,_reg:寄存器地址,_num:待读取的数量
*************************************************************************/
void MB_ReadCoil_01H(uint8_t _addr, uint16_t _reg, uint16_t _num)
{
	uint16_t TxCount = 0;
	uint16_t crc = 0;
	Tx_Buf[TxCount++] = _addr;		    /* 从站地址 */
	Tx_Buf[TxCount++] = 0x01;		      /* 功能码 */
	Tx_Buf[TxCount++] = _reg >> 8;	  /* 寄存器地址 高字节 */
	Tx_Buf[TxCount++] = _reg;		      /* 寄存器地址 低字节 */
	Tx_Buf[TxCount++] = _num >> 8;	  /* 线圈(bit)个数 高字节 */
	Tx_Buf[TxCount++] = _num;		      /* 线圈(bit)个数 低字节 */
	crc = CRC16_MODBUS((uint8_t*)&Tx_Buf, TxCount);
	Tx_Buf[TxCount++] = (uint8_t)(crc >> 8 & 0xFF); /* crc 高字节 */
	Tx_Buf[TxCount++] = (uint8_t)(crc >> 0 & 0xFF);	/* crc 低字节 */
	//HAL_UART_Transmit(&husart_debug, (uint8_t *)&Tx_Buf, TxCount, 0xffff);
}
/**************************************************************************
* 函数功能: 读输入状态状态(InputStatue)
* 输入参数: _addr:从站地址,_reg:寄存器地址,_num:待读取的输入数量
*************************************************************************/
void MB_ReadStatus_02H(uint8_t _addr, uint16_t _reg, uint16_t _num)
{
	uint16_t TxCount = 0;
	uint16_t crc = 0;
	Tx_Buf[TxCount++] = _addr;		    /* 从站地址 */
	Tx_Buf[TxCount++] = 0x02;		      /* 功能码 */
	Tx_Buf[TxCount++] = _reg >> 8;	  /* 寄存器地址 高字节 */
	Tx_Buf[TxCount++] = _reg;		      /* 寄存器地址 低字节 */
	Tx_Buf[TxCount++] = _num >> 8;	  /* 开关(Input)个数 高字节 */
	Tx_Buf[TxCount++] = _num;		      /* 开关(Input)个数 低字节 */
	crc = CRC16_MODBUS((uint8_t*)&Tx_Buf, TxCount);
	Tx_Buf[TxCount++] = (uint8_t)(crc >> 8 & 0xFF); /* crc 高字节 */
	Tx_Buf[TxCount++] = (uint8_t)(crc >> 0 & 0xFF);	/* crc 低字节 */
	//HAL_UART_Transmit(&husart_debug, (uint8_t *)&Tx_Buf, TxCount, 0xffff);
}
/**************************************************************************
* 函数功能: 读保持寄存器(HoldingRegister)
* 输入参数: _addr:从站地址,_reg:寄存器地址,_num:待读取的寄存器数量
*************************************************************************/
void MB_ReadHoldingReg_03H(uint8_t _addr, uint16_t _reg, uint16_t _num)
{
	uint16_t TxCount = 0;
	uint16_t crc = 0;
	Tx_Buf[TxCount++] = _addr;		    /* 从站地址 */
	Tx_Buf[TxCount++] = 0x03;		      /* 功能码 */
	Tx_Buf[TxCount++] = _reg >> 8;	  /* 寄存器地址 高字节 */
	Tx_Buf[TxCount++] = _reg;		      /* 寄存器地址 低字节 */
	Tx_Buf[TxCount++] = _num >> 8;	  /* 寄存器(16bits)个数 高字节 */
	Tx_Buf[TxCount++] = _num;		      /* 低字节 */
	crc = CRC16_MODBUS((uint8_t*)&Tx_Buf, TxCount);
	Tx_Buf[TxCount++] = (uint8_t)(crc >> 8 & 0xFF); /* crc 高字节 */
	Tx_Buf[TxCount++] = (uint8_t)(crc >> 0 & 0xFF);	/* crc 低字节 */
	//HAL_UART_Transmit(&husart_debug, (uint8_t *)&Tx_Buf, TxCount, 0xffff);
}
/*************************************************************************
* 函数功能: 读N个输入寄存器(InputRegister)
* 输入参数: _addr:从站地址,_reg:寄存器地址,_num:待读取的寄存器数量
*************************************************************************/
void MB_ReadInputReg_04H(uint8_t _addr, uint16_t _reg, uint16_t _num)
{
	uint16_t TxCount = 0;
	uint16_t crc = 0;
	Tx_Buf[TxCount++] = _addr;		    /* 从站地址 */
	Tx_Buf[TxCount++] = 0x04;		      /* 功能码 */
	Tx_Buf[TxCount++] = _reg >> 8;	  /* 寄存器地址 高字节 */
	Tx_Buf[TxCount++] = _reg;		      /* 寄存器地址 低字节 */
	Tx_Buf[TxCount++] = _num >> 8;	  /* 寄存器(16bits)个数 高字节 */
	Tx_Buf[TxCount++] = _num;		      /*  低字节 */
	crc = CRC16_MODBUS((uint8_t*)&Tx_Buf, TxCount);
	Tx_Buf[TxCount++] = (uint8_t)(crc >> 8 & 0xFF); /* crc 高字节 */
	Tx_Buf[TxCount++] = (uint8_t)(crc >> 0 & 0xFF);	/* crc 低字节 */
	//HAL_UART_Transmit(&husart_debug, (uint8_t *)&Tx_Buf, TxCount, 0xffff);
}
/**************************************************************************
* 函数功能: 写单个线圈状态(CoilStatue)
* 输入参数: _addr:从站地址,_reg:寄存器地址,_sta:待写入的线圈状态(0,1)
*************************************************************************/
void MB_WriteCoil_05H(uint8_t _addr, uint16_t _reg, uint16_t _sta)
{
	uint16_t TxCount = 0;
	uint16_t crc = 0;
	Tx_Buf[TxCount++] = _addr;		    /* 从站地址 */
	Tx_Buf[TxCount++] = 0x05;		      /* 功能码 */
	Tx_Buf[TxCount++] = _reg >> 8;	  /* 寄存器地址 高字节 */
	Tx_Buf[TxCount++] = _reg;		      /* 寄存器地址 低字节 */
	Tx_Buf[TxCount++] = _sta >> 8;	  /* 线圈(bit)个数 高字节 */
	Tx_Buf[TxCount++] = _sta;		      /* 线圈(bit)个数 低字节 */
	crc = CRC16_MODBUS((uint8_t*)&Tx_Buf, TxCount);
	Tx_Buf[TxCount++] = (uint8_t)(crc >> 8 & 0xFF); /* crc 高字节 */
	Tx_Buf[TxCount++] = (uint8_t)(crc >> 0 & 0xFF);	/* crc 低字节 */
	//HAL_UART_Transmit(&husart_debug, (uint8_t *)&Tx_Buf, TxCount, 0xffff);
}
/**************************************************************************
* 函数功能: 写单个保持寄存器(HoldingRegister)
* 输入参数: _addr:从站地址,_reg:寄存器地址,_data:待写入的寄存器数据
*************************************************************************/
void MB_WriteHoldingReg_06H(uint8_t _addr, uint16_t _reg, uint16_t _data)
{
	uint16_t TxCount = 0;
	uint16_t crc = 0;
	Tx_Buf[TxCount++] = _addr;		    /* 从站地址 */
	Tx_Buf[TxCount++] = 0x06;		      /* 功能码 */
	Tx_Buf[TxCount++] = _reg >> 8;	  /* 寄存器地址 高字节 */
	Tx_Buf[TxCount++] = _reg;		      /* 寄存器地址 低字节 */
	Tx_Buf[TxCount++] = _data >> 8;	  /* 寄存器(16bits)个数 高字节 */
	Tx_Buf[TxCount++] = _data;		    /*  低字节 */
	crc = CRC16_MODBUS((uint8_t*)&Tx_Buf, TxCount);
	Tx_Buf[TxCount++] = (uint8_t)(crc >> 8 & 0xFF); /* crc 高字节 */
	Tx_Buf[TxCount++] = (uint8_t)(crc >> 0 & 0xFF);	/* crc 低字节 */
	//HAL_UART_Transmit(&husart_debug, (uint8_t *)&Tx_Buf, TxCount, 0xffff);
}
/*************************************************************************
* 函数功能: 写N个保持寄存器(HoldingRegister)
* 输入参数: _addr:从站地址,_reg:寄存器地址,_num:待写入的寄存器数量,_databuf:待写入的寄存器数据
* 说    明: 填充数据发送缓存区,然后发送._databuf的长度需 >= _num*2
*************************************************************************/
void MB_WriteNumHoldingReg_10H(uint8_t _addr, uint16_t _reg, uint16_t _num, uint8_t *_databuf)
{
	uint16_t i;
	uint16_t TxCount = 0;
	uint16_t crc = 0;
	Tx_Buf[TxCount++] = _addr;		    /* 从站地址 */
	Tx_Buf[TxCount++] = 0x10;		      /* 功能码 */
	Tx_Buf[TxCount++] = _reg >> 8;	  /* 寄存器地址 高字节 */
	Tx_Buf[TxCount++] = _reg;		      /* 寄存器地址 低字节 */
	Tx_Buf[TxCount++] = _num >> 8;	  /* 寄存器(16bits)个数 高字节 */
	Tx_Buf[TxCount++] = _num;		      /*  低字节 */
	Tx_Buf[TxCount++] = _num << 1;		  /* 数据个数 */
	for (i = 0; i < 2 * _num; i++)
	Tx_Buf[TxCount++] = _databuf[i];		/* 后面的数据长度 */
	crc = CRC16_MODBUS((uint8_t*)&Tx_Buf, TxCount);
	Tx_Buf[TxCount++] = (uint8_t)(crc >> 8 & 0xFF); /* crc 高字节 */
	Tx_Buf[TxCount++] = (uint8_t)(crc >> 0 & 0xFF);	/* crc 低字节 */
	//HAL_UART_Transmit(&husart_debug, (uint8_t *)&Tx_Buf, TxCount, 0xffff);
}
/*==========================AppFunc=====================================*/
void ProtocolModBus_Send(uint8_t type, uint16_t _reg, uint16_t _num, uint8_t *_databuf)
{
	uint16_t i;
	uint16_t len = 0;
	uint16_t crc = 0;
	uint8_t Payload[256] = { 0 };
	//printf("ProtocolModBus_Send\n");
	Payload[len++] = MODBUS_SLAVE_ADDRESS;      /* 从站地址 */
	if (GET_DATA == type) Payload[len++] = 0x04;  /* 功能码 */
	else                  Payload[len++] = 0x10;
	Payload[len++] = _reg >> 8;	  /* 寄存器地址 高字节 */
	Payload[len++] = _reg;		      /* 寄存器地址 低字节 */
	Payload[len++] = _num >> 8;	  /* 寄存器(16bits)个数 高字节 */
	Payload[len++] = _num;		      /*  低字节 */
	if (SET_DATA == type){
		Payload[len++] = _num;// << 1;		  /* 数据个数 */
		for (i = 0; i <  _num; i++)//2 *
			Payload[len++] = _databuf[i];		/* 后面的数据长度 */
	}
	crc = CRC16_MODBUS((uint8_t*)&Payload, len);
	Payload[len++] = (uint8_t)(crc >> 8 & 0xFF); /* crc 高字节 */
	Payload[len++] = (uint8_t)(crc >> 0 & 0xFF);	/* crc 低字节 */
	if (len){
		tComState.TransmitEnable = true; //if (SET_DATA==type) 
		uartTaskQueueIn(UART_FORWARD_MAINBOARD, 0, Payload, len);
	}
}

/**
* 函数功能: 判断地址是否符合协议范围
* 输入参数: _Addr:起始地址,_RegNum:寄存器数量,_FunCode:功能码
* 返 回 值: 异常码:02H或NONE
* 说    明: 地址范围是0x0000~0xFFFF,可操作的空间范围不能超过这个区域
*/
uint8_t MB_JudgeAddr(uint16_t _Addr, uint16_t _RegNum)
{
	uint8_t Excode = EXCEPTION_CODE_NONE;
	/* 地址+寄存器数量不能超过0xFFFF */
	if (((uint32_t)_RegNum + (uint32_t)_Addr) > (uint32_t)0xFFFF)	Excode = EXCEPTION_CODE_02H;// 异常码 02H 
	return Excode;
}
/**
* 函数功能: 判断操作的数据量是否符合协议范围
* 输入参数: _RegNum:寄存器数量,_FunCode:功能码,_ByteNum:字节数量
* 返 回 值: 异常码:03或NONE
* 说    明: 对可操作连续内存空间的功能码需要验证操作的地址是否符合范围
*/
uint8_t MB_JudgeNum(uint16_t _RegNum, uint8_t _FunCode, uint16_t _ByteNum)
{
	uint8_t Excode = EXCEPTION_CODE_NONE;
	uint16_t _CoilNum = _RegNum; // 线圈(离散量)的数量
	switch (_FunCode){
	case FUN_CODE_01H:
	case FUN_CODE_02H:
		if ((_CoilNum<0x0001) || (_CoilNum>0x07D0))Excode = EXCEPTION_CODE_03H;// 异常码03H;
		break;
	case FUN_CODE_03H:
	case FUN_CODE_04H:
		if ((_RegNum<0x0001) || (_RegNum>0x007D))  Excode = EXCEPTION_CODE_03H;// 异常码03H;      
		break;
	case FUN_CODE_10H:
		if ((_RegNum<0x0001) || (_RegNum>0x007B))  Excode = EXCEPTION_CODE_03H;// 异常码03H
		if (_ByteNum != (_RegNum << 1))            Excode = EXCEPTION_CODE_03H;// 异常码03H
		break;
	}
	return Excode;
}
/*************************************************************************
* 函数功能: 提取数据帧,进行解析数据帧
**************************************************************************/
bool  MB_Parse_Data(uint8_t *RevBuf,uint8_t len)
{
	bool ret = false;
	uint16_t val=0;
	uint16_t crc=0;
	if(len>2){
		crc=CRC16_MODBUS(RevBuf, len - 2);
		val =(uint16_t)((RevBuf[len - 2] << 8) | RevBuf[len - 1]);
		if(crc==val) ret=true;
	}
#if 0
	tPdu.Code = RevBuf[1];                   // 功能码
	tPdu.Addr = ((RevBuf[2] << 8) | RevBuf[3]);// 寄存器起始地址
	tPdu.Num = ((RevBuf[4] << 8) | RevBuf[5]);// 数量(Coil,Input,Holding Reg,Input Reg)
	tPdu.CRC = CRC16_MODBUS(RevBuf, len - 2);             // CRC校验码
	if (tPdu.CRC == (uint16_t)((RevBuf[len - 2] << 8) | RevBuf[len - 1]))
	ret = true;

	tPdu.byteNums = RevBuf[6];                                     // 获得字节数
	tPdu.ValueReg = (uint8_t*)&RevBuf[7];                          // 寄存器值起始地址
	tPdu.PtrHoldingOffset = tPdu.PtrHoldingbase + tPdu.Addr; // 保持寄存器的起始地址
#endif
	//printf("%s:ret=%d\n", __func__, ret);
	return ret;
}
/**
* 函数功能: 对接收到的数据进行分析并执行
* 返 回 值: 异常码或0x00
* 说    明: 判断功能码,验证地址是否正确.数值内容是否溢出,数据没错误就发送响应信号
*/
uint8_t MB_Exception_Check(PDU_t ModBus)
{
	uint16_t ExCode = EXCEPTION_CODE_NONE;
	/* 校验功能码 */
	if (IS_NOT_FUNCODE(ModBus.Code)){ // 不支持的功能码	
		ExCode = EXCEPTION_CODE_01H;            // /* Modbus异常响应 */ 异常码01H
		return ExCode;
	}
	switch (ModBus.Code){/* 根据功能码分别做判断 */
	case FUN_CODE_01H:/* ---- 01H  02H 读取离散量输入(Coil Input)---------------------- */
	case FUN_CODE_02H:
		ExCode = MB_JudgeNum(ModBus.Num, ModBus.Code, 1);/* 判断线圈数量是否正确 */
		if (ExCode != EXCEPTION_CODE_NONE) return ExCode;
		ExCode = MB_JudgeAddr(ModBus.Addr, ModBus.Num);/* 判断地址是否正确*/
		if (ExCode != EXCEPTION_CODE_NONE) return ExCode;
		break;
		/* ---- 03H  04H 读取保持/输入寄存器---------------------- */
	case FUN_CODE_03H:
	case FUN_CODE_04H:
		/* 判断寄存器数量是否正确 */
		ExCode = MB_JudgeNum(ModBus.Num, ModBus.Code, ModBus.byteNums);
		if (ExCode != EXCEPTION_CODE_NONE)  return ExCode;	
		ExCode = MB_JudgeAddr(ModBus.Addr, ModBus.Num);/* 判断地址是否正确*/
		if (ExCode != EXCEPTION_CODE_NONE) return ExCode;
		break;	
	case FUN_CODE_05H:/* ---- 05H 写入单个离散量---------------------- */
		break;	
	case FUN_CODE_06H:/* ---- 06H 写单个保持寄存器 ---------------------- */
		break;	
	case FUN_CODE_10H:/* ---- 10H 写多个保持寄存器 ---------------------- */
		ExCode = MB_JudgeNum(ModBus.Num, ModBus.Code, ModBus.byteNums);/* 判断寄存器数量是否正确 */
		if (ExCode != EXCEPTION_CODE_NONE) return ExCode;
		ExCode = MB_JudgeAddr(ModBus.Addr, ModBus.Num);/* 判断地址是否正确*/
		if (ExCode != EXCEPTION_CODE_NONE) return ExCode;
		break;
	}
	/* 数据帧没有异常 */
	//printf("MB_Exception_Check:ExCode=%d\n", ExCode);
	return ExCode; //   EXCEPTION_CODE_NONE
}
/*==========================SlaveFunc=====================================*/
/**
* 函数功能: 异常响应
* 输入参数: _FunCode :发送异常的功能码,_ExCode:异常码
* 返 回 值: 无
* 说    明: 当通信数据帧发生异常时,发送异常响应
*/
void MB_ResponseException(uint8_t _FunCode, uint8_t _ExCode)
{
	uint16_t TxCount = 0;
	uint16_t crc = 0;
	Tx_Buf[TxCount++] = MODBUS_SLAVE_ADDRESS;	/* 从站地址 */
	Tx_Buf[TxCount++] = _FunCode | 0x80;	    /* 功能码 + 0x80*/
	Tx_Buf[TxCount++] = _ExCode;	            /* 异常码*/
	crc = CRC16_MODBUS((uint8_t*)&Tx_Buf, TxCount);
	Tx_Buf[TxCount++] = crc;	          /* crc 低字节 */
	Tx_Buf[TxCount++] = crc >> 8;		      /* crc 高字节 */

	if (TxCount)  uartTaskQueueIn_HPri(UART_FORWARD_MAINBOARD, 0, Tx_Buf, TxCount);
	//UART_Tx((uint8_t*)Tx_Buf, TxCount);
}
/*******************************************************************/