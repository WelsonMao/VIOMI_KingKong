#ifndef __MODBUS_H__
#define __MODBUS_H__
/* 包含头文件 ----------------------------------------------------------------*/
/* 类型定义 ------------------------------------------------------------------*/
typedef struct {
	 uint8_t  Code;  	        // 功能码
	 uint8_t byteNums; 	        // 字节数
	 uint16_t Addr;            // 操作内存的起始地址
	 uint16_t Num; 	            // 寄存器或者线圈的数量
	 uint16_t CRC;       	      // CRC校验码
	 uint8_t *ValueReg; 	      // 10H功能码的数据
	 uint16_t *PtrHoldingbase;  // HoldingReg内存首地址
	 uint16_t *PtrHoldingOffset;// HoldingReg内存首地址
}PDU_t;
typedef enum
{
	ModBusFunc_Read_Coil_01H = 0x01,
	ModBusFunc_Read_Status_02H = 0x02,
	ModBusFunc_Read_HoldReg_03H = 0x03,
	ModBusFunc_Read_InputReg_04H = 0x04,
	ModBusFunc_Write_Coil_05H = 0x05,
	ModBusFunc_Write_HoldReg_06H = 0x06,
	ModBusFunc_Write_NumHoldReg_10H = 0x10,
};
/* 宏定义 --------------------------------------------------------------------*/
#define BAUDRATE                           115200
#define DATA_FORMAT                        10  //bits
#define TIMEOUT_CHAR35                     (BAUDRATE>=115200)?(4000):(40*1000000/BAUDRATE) //us 40-35
#define MODBUS_SLAVE_ADDRESS               0x02
#define MODBUS_FUNC_READ_COIL_01H          0x01
#define MODBUS_FUNC_READ_STATUS_02H        0x02
#define MODBUS_FUNC_READ_HOLD_REG_03H      0x03
#define MODBUS_FUNC_READ_INPUT_REG_04H     0x04
#define MODBUS_FUNC_WRITE_COIL_05H         0x05
#define MODBUS_FUNC_WRITE_HOLD_REG_06H     0x06
#define MODBUS_FUNC_WRITE_xHOLD_REG_10H    0x10

#define FUN_CODE_01H						0x01  // 功能码01H 
#define FUN_CODE_02H						0x02  // 功能码02H
#define FUN_CODE_03H						0x03  // 功能码03H
#define FUN_CODE_04H						0x04  // 功能码04H
#define FUN_CODE_05H						0x05  // 功能码05H
#define FUN_CODE_06H						0x06  // 功能码06H
#define FUN_CODE_10H						0x10  // 功能码10H


#define SET_DATA                            0
#define GET_DATA                            1                                 
/*
代码 名称 含义
1  功能代码           从机接收的是一种不能执行功能代码。发出查询命令后，该代码指示无程序功能。
2  地址               接收的数据地址，是从机不允许的地址。
3  不合法数据         查询数据区的值是从机不允许的值。
4  从机设备故障       执行主机请求的动作时出现不可恢复的错误。
5  确认               从机已接收请求处理数据，但需要较长 的处理时间，为避免主机出现超时错误而发送该确认响应。主机以此再发送一个“查询程序完成”未决定
6  从机设备忙碌       从机正忙于处理一个长时程序命令，请求主机在从机空闲时发送信息。
7  否定               从机不能执行查询要求的程序功能时，该代码使用
8  内存奇偶校验错误   从机读扩展内存中的数据时，发现有奇偶校验错误，主机按从机的要求重新发送数据
*/
#define EXCEPTION_CODE_NONE						0x00  // 异常码 无异常
#define EXCEPTION_CODE_01H						0x01  // 异常码
#define EXCEPTION_CODE_02H						0x02  // 异常码
#define EXCEPTION_CODE_03H						0x03  // 异常码
#define EXCEPTION_CODE_04H						0x04  // 异常码
#define EXCEPTION_CODE_08H						0x08  // 异常码

#define IS_NOT_FUNCODE(code)  (!((code == FUN_CODE_01H)||\
								(code == FUN_CODE_02H) || \
								(code == FUN_CODE_03H) || \
								(code == FUN_CODE_04H) || \
								(code == FUN_CODE_05H) || \
								(code == FUN_CODE_06H) || \
								(code == FUN_CODE_10H)))
/* 扩展变量 ------------------------------------------------------------------*/
extern PDU_t tPdu;
/* 函数声明 ------------------------------------------------------------------*/
void MB_ReadCoil_01H(uint8_t _addr, uint16_t _reg, uint16_t _num);
void MB_WriteCoil_05H(uint8_t _addr, uint16_t _reg, uint16_t _num);
void MB_ReadInput_02H(uint8_t _addr, uint16_t _reg, uint16_t _num);
void MB_ReadHoldingReg_03H(uint8_t _addr, uint16_t _reg, uint16_t _num);
void MB_ReadInputReg_04H(uint8_t _addr, uint16_t _reg, uint16_t _num);
void MB_WriteHoldingReg_06H(uint8_t _addr, uint16_t _reg, uint16_t _data);
void MB_WriteNumHoldingReg_10H(uint8_t _addr, uint16_t _reg, uint16_t _num, uint8_t *_databuf);

bool  MB_Parse_Data(uint8_t *RevBuf, uint8_t len);
uint8_t MB_Exception_Check(PDU_t ModBus);
void MB_ResponseException(uint8_t _FunCode, uint8_t _ExCode);

void ProtocolModBus_Send(uint8_t type, uint16_t _reg, uint16_t _num, uint8_t *_databuf);

#endif /* __MODBUS_H__ */
/******************* (C) Green-Motive *****END OF FILE****/