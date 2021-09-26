#include <assert.h>
#include "../scene.h"
#include "CRC.h"


/* CRC 高位字节值表 */ 
const unsigned char auchCRCHi[] = { 
0X00, 0xC1, 0x81, 0x40, 0X01, 0xC0, 0x80, 0x41, 0X01, 0xC0, 
0x80, 0x41, 0X00, 0xC1, 0x81, 0x40, 0X01, 0xC0, 0x80, 0x41, 
0X00, 0xC1, 0x81, 0x40, 0X00, 0xC1, 0x81, 0x40, 0X01, 0xC0, 
0x80, 0x41, 0X01, 0xC0, 0x80, 0x41, 0X00, 0xC1, 0x81, 0x40, 
0X00, 0xC1, 0x81, 0x40, 0X01, 0xC0, 0x80, 0x41, 0X00, 0xC1, 
0x81, 0x40, 0X01, 0xC0, 0x80, 0x41, 0X01, 0xC0, 0x80, 0x41, 
0X00, 0xC1, 0x81, 0x40, 0X01, 0xC0, 0x80, 0x41, 0X00, 0xC1, 
0x81, 0x40, 0X00, 0xC1, 0x81, 0x40, 0X01, 0xC0, 0x80, 0x41, 
0X00, 0xC1, 0x81, 0x40, 0X01, 0xC0, 0x80, 0x41, 0X01, 0xC0, 
0x80, 0x41, 0X00, 0xC1, 0x81, 0x40, 0X00, 0xC1, 0x81, 0x40, 
0X01, 0xC0, 0x80, 0x41, 0X01, 0xC0, 0x80, 0x41, 0X00, 0xC1, 
0x81, 0x40, 0X01, 0xC0, 0x80, 0x41, 0X00, 0xC1, 0x81, 0x40, 
0X00, 0xC1, 0x81, 0x40, 0X01, 0xC0, 0x80, 0x41, 0X01, 0xC0, 
0x80, 0x41, 0X00, 0xC1, 0x81, 0x40, 0X00, 0xC1, 0x81, 0x40, 
0X01, 0xC0, 0x80, 0x41, 0X00, 0xC1, 0x81, 0x40, 0X01, 0xC0, 
0x80, 0x41, 0X01, 0xC0, 0x80, 0x41, 0X00, 0xC1, 0x81, 0x40, 
0X00, 0xC1, 0x81, 0x40, 0X01, 0xC0, 0x80, 0x41, 0X01, 0xC0, 
0x80, 0x41, 0X00, 0xC1, 0x81, 0x40, 0X01, 0xC0, 0x80, 0x41, 
0X00, 0xC1, 0x81, 0x40, 0X00, 0xC1, 0x81, 0x40, 0X01, 0xC0, 
0x80, 0x41, 0X00, 0xC1, 0x81, 0x40, 0X01, 0xC0, 0x80, 0x41, 
0X01, 0xC0, 0x80, 0x41, 0X00, 0xC1, 0x81, 0x40, 0X01, 0xC0, 
0x80, 0x41, 0X00, 0xC1, 0x81, 0x40, 0X00, 0xC1, 0x81, 0x40, 
0X01, 0xC0, 0x80, 0x41, 0X01, 0xC0, 0x80, 0x41, 0X00, 0xC1, 
0x81, 0x40, 0X00, 0xC1, 0x81, 0x40, 0X01, 0xC0, 0x80, 0x41, 
0X00, 0xC1, 0x81, 0x40, 0X01, 0xC0, 0x80, 0x41, 0X01, 0xC0, 
0x80, 0x41, 0X00, 0xC1, 0x81, 0x40 
} ; 
/* CRC低位字节值表*/ 
const unsigned char auchCRCLo[] = {         
0X00, 0xC0, 0xC1, 0X01, 0xC3, 0X03, 0X02, 0xC2, 0xC6, 0X06, 
0X07, 0xC7, 0X05, 0xC5, 0xC4, 0X04, 0xCC, 0X0C, 0X0D, 0xCD, 
0X0F, 0xCF, 0xCE, 0X0E, 0X0A, 0xCA, 0xCB, 0X0B, 0xC9, 0X09, 
0X08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 
0x43, 0x83, 0x41, 0x81, 0x80, 0x40            
} ;
const unsigned char  CRC8_TABLE[] =
{
	0x00, 0x31, 0x62, 0x53, 0xC4, 0xF5, 0xA6, 0x97,
	0xB9, 0x88, 0xDB, 0xEA, 0x7D, 0x4C, 0x1F, 0x2E,
	0x43, 0x72, 0x21, 0x10, 0x87, 0xB6, 0xE5, 0xD4,
	0xFA, 0xCB, 0x98, 0xA9, 0x3E, 0x0F, 0x5C, 0x6D,
	0x86, 0xB7, 0xE4, 0xD5, 0x42, 0x73, 0x20, 0x11,
	0x3F, 0x0E, 0x5D, 0x6C, 0xFB, 0xCA, 0x99, 0xA8,
	0xC5, 0xF4, 0xA7, 0x96, 0x01, 0x30, 0x63, 0x52,
	0x7C, 0x4D, 0x1E, 0x2F, 0xB8, 0x89, 0xDA, 0xEB,
	0x3D, 0x0C, 0x5F, 0x6E, 0xF9, 0xC8, 0x9B, 0xAA,
	0x84, 0xB5, 0xE6, 0xD7, 0x40, 0x71, 0x22, 0x13,
	0x7E, 0x4F, 0x1C, 0x2D, 0xBA, 0x8B, 0xD8, 0xE9,
	0xC7, 0xF6, 0xA5, 0x94, 0x03, 0x32, 0x61, 0x50,
	0xBB, 0x8A, 0xD9, 0xE8, 0x7F, 0x4E, 0x1D, 0x2C,
	0x02, 0x33, 0x60, 0x51, 0xC6, 0xF7, 0xA4, 0x95,
	0xF8, 0xC9, 0x9A, 0xAB, 0x3C, 0x0D, 0x5E, 0x6F,
	0x41, 0x70, 0x23, 0x12, 0x85, 0xB4, 0xE7, 0xD6,
	0x7A, 0x4B, 0x18, 0x29, 0xBE, 0x8F, 0xDC, 0xED,
	0xC3, 0xF2, 0xA1, 0x90, 0x07, 0x36, 0x65, 0x54,
	0x39, 0x08, 0x5B, 0x6A, 0xFD, 0xCC, 0x9F, 0xAE,
	0x80, 0xB1, 0xE2, 0xD3, 0x44, 0x75, 0x26, 0x17,
	0xFC, 0xCD, 0x9E, 0xAF, 0x38, 0x09, 0x5A, 0x6B,
	0x45, 0x74, 0x27, 0x16, 0x81, 0xB0, 0xE3, 0xD2,
	0xBF, 0x8E, 0xDD, 0xEC, 0x7B, 0x4A, 0x19, 0x28,
	0x06, 0x37, 0x64, 0x55, 0xC2, 0xF3, 0xA0, 0x91,
	0x47, 0x76, 0x25, 0x14, 0x83, 0xB2, 0xE1, 0xD0,
	0xFE, 0xCF, 0x9C, 0xAD, 0x3A, 0x0B, 0x58, 0x69,
	0x04, 0x35, 0x66, 0x57, 0xC0, 0xF1, 0xA2, 0x93,
	0xBD, 0x8C, 0xDF, 0xEE, 0x79, 0x48, 0x1B, 0x2A,
	0xC1, 0xF0, 0xA3, 0x92, 0x05, 0x34, 0x67, 0x56,
	0x78, 0x49, 0x1A, 0x2B, 0xBC, 0x8D, 0xDE, 0xEF,
	0x82, 0xB3, 0xE0, 0xD1, 0x46, 0x77, 0x24, 0x15,
	0x3B, 0x0A, 0x59, 0x68, 0xFF, 0xCE, 0x9D, 0xAC
};
/******************************************************************************************/
uint8_t GetCrc8(uint8_t init, uint8_t * buf, uint8_t size)
{
	uint8_t crc;
	uint8_t count;
	crc = init;
	for (count = 0; count < size; count++)
	{
		crc = CRC8_TABLE[(crc ^buf[count])];
	}
	return (crc);
}
uint8_t GetCrc8Val(uint8_t*data, uint8_t offset, uint8_t length)
{
	uint8_t crc = 255;

	for (int i = offset; i < length; ++i) {
		crc = CRC8_TABLE[crc ^ data[i]];
	}
	return crc;
}
/******************************************************************************************/
/**
 * @brief      计算CRC16数据(CRC16/IBM)
 * @param     puchMsg:需要计算的数据指针
*            usDataLen：数据长度
 * @return     返回CRC16结果
* @details     
*/
uint16_t Crc16Check(void const*puchMsgp, unsigned short usDataLen) 
{ 
    unsigned char const*puchMsg=puchMsgp;//按字节校验
    unsigned char uchCRCHi = 0xFF ; /* 高CRC字节初始化 */ 
    unsigned char uchCRCLo = 0xFF ; /* 低CRC 字节初始化 */ 
    unsigned long uIndex ; /* CRC循环中的索引 */ 
    while (usDataLen--) /* 传输消息缓冲区 */ 
    { 
        uIndex = uchCRCHi ^ *puchMsg++ ; /* 计算CRC */ 
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ; 
        uchCRCLo = auchCRCLo[uIndex] ; 
    } 
    uIndex=uchCRCHi;
    uIndex<<=8;
    uIndex|=uchCRCLo;
    return uIndex;    
}
/**
* 函数功能: Modbus CRC16 校验计算函数
* 输入参数: pushMsg:待计算的数据首地址,usDataLen:数据长度
* 返 回 值: CRC16 计算结果
* 说    明: 计算结果是高位在前,需要转换才能发送
*/
uint16_t MB_CRC16(uint8_t *pushMsg, uint8_t usDataLen)
{
	uint8_t uchCRCHi = 0xFF;
	uint8_t uchCRCLo = 0xFF;
	uint16_t uIndex;
	while (usDataLen--)
	{
		uIndex = uchCRCLo ^ *pushMsg++;
		uchCRCLo = uchCRCHi^auchCRCHi[uIndex];
		uchCRCHi = auchCRCLo[uIndex];
	}
	return (uchCRCHi << 8 | uchCRCLo);
}



/**
 * @brief      计算CRC16数据
 * @param     puchMsg:需要计算的数据指针
*            usDataLen：数据长度
 * @return     返回CRC16结果
* @details     
*/
uint16_t Crc16CheckRealTime(uint16_t vByte)
{ 
	static uint8_t uchCRCHi = 0xFF; /* 高CRC字节初始化 */
	static uint8_t uchCRCLo = 0xFF; /* 低CRC 字节初始化 */
	static uint32_t uIndex; /* CRC循环中的索引 */
    if(vByte==0xffff)//Crc初始化
    {
        uchCRCHi = 0xFF ; /* 高CRC字节初始化 */ 
        uchCRCLo = 0xFF ; /* 低CRC 字节初始化 */;    
        return 0;
    }else
    {
		uint8_t uchMsg = (uint8_t)vByte;//按字节校;
		uint32_t t;
        uIndex = uchCRCHi ^ uchMsg; /* 计算CRC */ 
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ; 
        uchCRCLo = auchCRCLo[uIndex] ; 
     
        t=uchCRCHi;
        t<<=8;
        t|=uchCRCLo;
        return (uint16_t)t;
    }
}

uint16_t GetLCRC16(uint8_t *buff,uint8_t cntnum)
{
    uint8_t i;
    uint16_t CHKDATA=0;
    for(i=0;i<cntnum;i++)
    CHKDATA^=buff[i]; 
    CHKDATA&=0xFF;  
    return  CHKDATA;   
}

uint16_t Aux_ChecksumGet(uint8_t *data, uint16_t len)
{
    uint64_t cksum = 0;
    int i = 0;
    while (i < len)
   {
     if (i + 2 > len)
       {
            cksum += data[i];
       } 
       else
       {
            cksum += data[i + 1] << 8 | data[i];
       }
       i += 2;
    }
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >>16);
    cksum = (uint16_t)~cksum;
    return ((cksum >> 8) | (cksum & 0xff) << 8);
}
uint8_t CRCSumGet(uint8_t *data, uint16_t len)
{
	uint16_t sum = 0;
	int i = 0;
	for (i = 0; i < len; i++){
		sum += data[i];
	}
	sum = ~sum;
	return ((uint8_t)sum);
}
/****************************Info**********************************************
* Name:    CRC-16/CCITT        x16+x12+x5+1
* Width:	16
* Poly:    0x1021
* Init:    0x0000
* Refin:   True
* Refout:  True
* Xorout:  0x0000
* Alias:   CRC-CCITT,CRC-16/CCITT-TRUE,CRC-16/KERMIT
*****************************************************************************/
unsigned short CRC16_CCITT(unsigned char *data, unsigned int datalen)
{
	unsigned short wCRCin = 0x0000;
	unsigned short wCPoly = 0x1021;
	unsigned char wChar = 0;

	InvertUint16(&wCPoly, &wCPoly);
	while (datalen--)
	{
		wCRCin ^= *(data++);
		for (int i = 0; i < 8; i++)
		{
			if (wCRCin & 0x01)
				wCRCin = (wCRCin >> 1) ^ wCPoly;
			else
				wCRCin = wCRCin >> 1;
		}
	}
	return (wCRCin);
}

/****************************Info**********************************************
* Name:    CRC-16/CCITT-FALSE   x16+x12+x5+1
* Width:	16
* Poly:    0x1021
* Init:    0xFFFF
* Refin:   False
* Refout:  False
* Xorout:  0x0000
* Note:
*****************************************************************************/
unsigned short CRC16_CCITT_FALSE(unsigned char *data, unsigned int datalen)
{
	unsigned short wCRCin = 0xFFFF;
	unsigned short wCPoly = 0x1021;

	while (datalen--)
	{
		wCRCin ^= *(data++) << 8;
		for (int i = 0; i < 8; i++)
		{
			if (wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	return (wCRCin);
}
/****************************Info**********************************************
* Name:    CRC-16/XMODEM       x16+x12+x5+1
* Width:	16
* Poly:    0x1021
* Init:    0x0000
* Refin:   False
* Refout:  False
* Xorout:  0x0000
* Alias:   CRC-16/ZMODEM,CRC-16/ACORN
*****************************************************************************/
unsigned short CRC16_XMODEM(unsigned char *data, unsigned int datalen)
{
	unsigned short wCRCin = 0x0000;
	unsigned short wCPoly = 0x1021;

	while (datalen--)
	{
		wCRCin ^= (*(data++) << 8);
		for (int i = 0; i < 8; i++)
		{
			if (wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	return (wCRCin);
}
/****************************Info**********************************************
* Name:    CRC-16/X25          x16+x12+x5+1
* Width:	16
* Poly:    0x1021
* Init:    0xFFFF
* Refin:   True
* Refout:  True
* Xorout:  0XFFFF
* Note:
*****************************************************************************/
unsigned short CRC16_X25(unsigned char *data, unsigned int datalen)
{
	unsigned short wCRCin = 0xFFFF;
	unsigned short wCPoly = 0x1021;

	InvertUint16(&wCPoly, &wCPoly);
	while (datalen--)
	{
		wCRCin ^= *(data++);
		for (int i = 0; i < 8; i++)
		{
			if (wCRCin & 0x01)
				wCRCin = (wCRCin >> 1) ^ wCPoly;
			else
				wCRCin = wCRCin >> 1;
		}
	}
	return (wCRCin ^ 0xFFFF);
}
/****************************Info**********************************************
* Name:    CRC-16/MODBUS       x16+x15+x2+1
* Width:	16
* Poly:    0x8005
* Init:    0xFFFF
* Refin:   True
* Refout:  True
* Xorout:  0x0000
* Note:
*****************************************************************************/
unsigned short CRC16_MODBUS(unsigned char *data, unsigned int datalen)
{
	unsigned short wCRCin = 0xFFFF;
	unsigned short wCPoly = 0x8005;

	InvertUint16(&wCPoly, &wCPoly);
	if (datalen >= 1){
		while (datalen--)
		{
			wCRCin ^= *(data++);
			for (int i = 0; i < 8; i++)
			{
				if (wCRCin & 0x01)
					wCRCin = (wCRCin >> 1) ^ wCPoly;
				else
					wCRCin = wCRCin >> 1;
			}
		}
	}
	//printf("datalen=%d,%04X\n", datalen, wCRCin);
	return (wCRCin);
}
/****************************Info**********************************************
* Name:    CRC-16/IBM          x16+x15+x2+1
* Width:	16
* Poly:    0x8005
* Init:    0x0000
* Refin:   True
* Refout:  True
* Xorout:  0x0000
* Alias:   CRC-16,CRC-16/ARC,CRC-16/LHA
*****************************************************************************/
unsigned short CRC16_IBM(unsigned char *data, unsigned int datalen)
{
	unsigned short wCRCin = 0x0000;
	unsigned short wCPoly = 0x8005;

	InvertUint16(&wCPoly, &wCPoly);
	while (datalen--)
	{
		wCRCin ^= *(data++);
		for (int i = 0; i < 8; i++)
		{
			if (wCRCin & 0x01)
				wCRCin = (wCRCin >> 1) ^ wCPoly;
			else
				wCRCin = wCRCin >> 1;
		}
	}
	return (wCRCin);
}
/****************************Info**********************************************
* Name:    CRC-16/MAXIM        x16+x15+x2+1
* Width:	16
* Poly:    0x8005
* Init:    0x0000
* Refin:   True
* Refout:  True
* Xorout:  0xFFFF
* Note:
*****************************************************************************/
unsigned short CRC16_MAXIM(unsigned char *data, unsigned int datalen)
{
	unsigned short wCRCin = 0x0000;
	unsigned short wCPoly = 0x8005;

	InvertUint16(&wCPoly, &wCPoly);
	while (datalen--)
	{
		wCRCin ^= *(data++);
		for (int i = 0; i < 8; i++)
		{
			if (wCRCin & 0x01)
				wCRCin = (wCRCin >> 1) ^ wCPoly;
			else
				wCRCin = wCRCin >> 1;
		}
	}
	return (wCRCin ^ 0xFFFF);
}
/****************************Info**********************************************
* Name:    CRC-16/USB          x16+x15+x2+1
* Width:	16
* Poly:    0x8005
* Init:    0xFFFF
* Refin:   True
* Refout:  True
* Xorout:  0xFFFF
* Note:
*****************************************************************************/
unsigned short CRC16_USB(unsigned char *data, unsigned int datalen)
{
	unsigned short wCRCin = 0xFFFF;
	unsigned short wCPoly = 0x8005;

	InvertUint16(&wCPoly, &wCPoly);
	while (datalen--)
	{
		wCRCin ^= *(data++);
		for (int i = 0; i < 8; i++)
		{
			if (wCRCin & 0x01)
				wCRCin = (wCRCin >> 1) ^ wCPoly;
			else
				wCRCin = wCRCin >> 1;
		}
	}
	return (wCRCin ^ 0xFFFF);
}
/****************************Info**********************************************
* Name:    CRC-16/DNP          x16+x13+x12+x11+x10+x8+x6+x5+x2+1
* Width:	16
* Poly:    0x3D65
* Init:    0x0000
* Refin:   True
* Refout:  True
* Xorout:  0xFFFF
* Use:     M-Bus,ect.
*****************************************************************************/
unsigned short CRC16_DNP(unsigned char *data, unsigned int datalen)
{
	unsigned short wCRCin = 0x0000;
	unsigned short wCPoly = 0x3D65;

	InvertUint16(&wCPoly, &wCPoly);
	while (datalen--)
	{
		wCRCin ^= *(data++);
		for (int i = 0; i < 8; i++)
		{
			if (wCRCin & 0x01)
				wCRCin = (wCRCin >> 1) ^ wCPoly;
			else
				wCRCin = (wCRCin >> 1);
		}
	}
	return (wCRCin ^ 0xFFFF);
}
/****************************Info**********************************************
* Name:    InvertUint8
* Note: 	把字节颠倒过来，如0x12变成0x48
0x12: 0001 0010
0x48: 0100 1000
*****************************************************************************/
void InvertUint8(unsigned char *dBuf, unsigned char *srcBuf)
{
	int i;
	unsigned char tmp[4] = { 0 };

	for (i = 0; i< 8; i++)
	{
		if (srcBuf[0] & (1 << i))
			tmp[0] |= 1 << (7 - i);
	}
	dBuf[0] = tmp[0];

}
void InvertUint16(unsigned short *dBuf, unsigned short *srcBuf)
{
	int i;
	unsigned short tmp[4] = { 0 };

	for (i = 0; i< 16; i++)
	{
		if (srcBuf[0] & (1 << i))
			tmp[0] |= 1 << (15 - i);
	}
	dBuf[0] = tmp[0];
}
void InvertUint32(unsigned int *dBuf, unsigned int *srcBuf)
{
	int i;
	unsigned int tmp[4] = { 0 };

	for (i = 0; i< 32; i++)
	{
		if (srcBuf[0] & (1 << i))
			tmp[0] |= 1 << (31 - i);
	}
	dBuf[0] = tmp[0];
}
/****************************Info**********************************************
* Name:    CRC-32  x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
* Width:	32
* Poly:    0x4C11DB7
* Init:    0xFFFFFFF
* Refin:   True
* Refout:  True
* Xorout:  0xFFFFFFF
* Alias:   CRC_32/ADCCP
* Use:     WinRAR,ect.
*****************************************************************************/
unsigned int CRC32(unsigned char *data, unsigned int datalen)
{

	unsigned int wCRCin = 0xFFFFFFFF;
	unsigned int wCPoly = 0x04C11DB7;

	InvertUint32(&wCPoly, &wCPoly);
	while (datalen--)
	{
		wCRCin ^= *(data++);
		for (int i = 0; i < 8; i++)
		{
			if (wCRCin & 0x01)
				wCRCin = (wCRCin >> 1) ^ wCPoly;
			else
				wCRCin = wCRCin >> 1;
		}
	}
	return (wCRCin ^ 0xFFFFFFFF);
}
/****************************Info**********************************************
* Name:    CRC-32/MPEG-2  x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
* Width:	32
* Poly:    0x4C11DB7
* Init:    0xFFFFFFF
* Refin:   False
* Refout:  False
* Xorout:  0x0000000
* Note:
*****************************************************************************/
unsigned int CRC32_MPEG(unsigned char *data, unsigned int datalen)
{

	unsigned int wCRCin = 0xFFFFFFFF;
	unsigned int wCPoly = 0x04C11DB7;
	unsigned int wChar = 0;
	while (datalen--)
	{
		wChar = *(data++);
		wCRCin ^= (wChar << 24);
		for (int i = 0; i < 8; i++)
		{
			if (wCRCin & 0x80000000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	return (wCRCin);
}
/****************************Info**********************************************
* Name:    CRC-4/ITU	x4+x+1
* Width:	4
* Poly:    0x03
* Init:    0x00
* Refin:   True
* Refout:  True
* Xorout:  0x00
* Note:
*****************************************************************************/
unsigned char CRC4_ITU(unsigned char *data, unsigned int datalen)
{
	unsigned char wCRCin = 0x00;
	unsigned char wCPoly = 0x03;
	unsigned char wChar = 0;

	while (datalen--)
	{
		wChar = *(data++);
		InvertUint8(&wChar, &wChar);
		wCRCin ^= (wChar);
		for (int i = 0; i < 8; i++)
		{
			if (wCRCin & 0x80)
				wCRCin = (wCRCin << 1) ^ (wCPoly << 4);
			else
				wCRCin = wCRCin << 1;
		}
	}
	InvertUint8(&wCRCin, &wCRCin);
	return (wCRCin);
}
/****************************Info**********************************************
* Name:    CRC-5/EPC	x5+x3+1
* Width:	5
* Poly:    0x09
* Init:    0x09
* Refin:   False
* Refout:  False
* Xorout:  0x00
* Note:
*****************************************************************************/
unsigned char CRC5_EPC(unsigned char *data, unsigned int datalen)
{
	unsigned char wCRCin = 0x09 << 3;
	unsigned char wCPoly = 0x09 << 3;

	while (datalen--)
	{
		wCRCin ^= *(data++);
		for (int i = 0; i < 8; i++)
		{
			if (wCRCin & 0x80)
				wCRCin = (wCRCin << 1) ^ (wCPoly);
			else
				wCRCin = wCRCin << 1;
		}
	}
	return (wCRCin >> 3);
}
/****************************Info**********************************************
* Name:    CRC-5/USB	x5+x2+1
* Width:	5
* Poly:    0x05
* Init:    0x1F
* Refin:   True
* Refout:  True
* Xorout:  0x1F
* Note:
*****************************************************************************/
unsigned char CRC5_USB(unsigned char *data, unsigned int datalen)
{
	unsigned char wCRCin = 0x1F;
	unsigned char wCPoly = 0x05;

	InvertUint8(&wCPoly, &wCPoly);
	while (datalen--)
	{
		wCRCin ^= *(data++);
		for (int i = 0; i < 8; i++)
		{
			if (wCRCin & 0x01)
				wCRCin = (wCRCin >> 1) ^ (wCPoly >> 3);
			else
				wCRCin = wCRCin >> 1;
		}
	}
	return (wCRCin ^ 0x1F);
}
/****************************Info**********************************************
* Name:    CRC-5/ITU	x5+x4+x2+1
* Width:	5
* Poly:    0x15
* Init:    0x00
* Refin:   True
* Refout:  True
* Xorout:  0x00
* Note:
*****************************************************************************/
unsigned char CRC5_ITU(unsigned char *data, unsigned int datalen)
{
	unsigned char wCRCin = 0x00;
	unsigned char wCPoly = 0x15;

	InvertUint8(&wCPoly, &wCPoly);
	while (datalen--)
	{
		wCRCin ^= *(data++);
		for (int i = 0; i < 8; i++)
		{
			if (wCRCin & 0x01)
				wCRCin = (wCRCin >> 1) ^ (wCPoly >> 3);
			else
				wCRCin = wCRCin >> 1;
		}
	}
	return (wCRCin);
}

/****************************Info**********************************************
* Name:    CRC-6/ITU	x6+x+1
* Width:	6
* Poly:    0x03
* Init:    0x00
* Refin:   True
* Refout:  True
* Xorout:  0x00
* Note:
*****************************************************************************/
unsigned char CRC6_ITU(unsigned char *data, unsigned int datalen)
{
	unsigned char wCRCin = 0x00;
	unsigned char wCPoly = 0x03;
	unsigned char wChar = 0;

	while (datalen--)
	{
		wChar = *(data++);
		InvertUint8(&wChar, &wChar);
		wCRCin ^= (wChar);
		for (int i = 0; i < 8; i++)
		{
			if (wCRCin & 0x80)
				wCRCin = (wCRCin << 1) ^ (wCPoly << 2);
			else
				wCRCin = wCRCin << 1;
		}
	}
	InvertUint8(&wCRCin, &wCRCin);
	return (wCRCin);
}
/****************************Info**********************************************
* Name:    CRC-7/MMC           x7+x3+1
* Width:	7
* Poly:    0x09
* Init:    0x00
* Refin:   False
* Refout:  False
* Xorout:  0x00
* Use:     MultiMediaCard,SD,ect.
*****************************************************************************/
unsigned char CRC7_MMC(unsigned char *data, unsigned int datalen)
{
	unsigned char wCRCin = 0x00;
	unsigned char wCPoly = 0x09;

	while (datalen--)
	{
		wCRCin ^= *(data++);
		for (int i = 0; i < 8; i++)
		{
			if (wCRCin & 0x80)
				wCRCin = (wCRCin << 1) ^ (wCPoly << 1);
			else
				wCRCin = wCRCin << 1;
		}
	}
	return (wCRCin >> 1);
}
/****************************Info**********************************************
* Name:    CRC-8               x8+x2+x+1
* Width:	8
* Poly:    0x07
* Init:    0x00
* Refin:   False
* Refout:  False
* Xorout:  0x00
* Note:
*****************************************************************************/
unsigned char CRC8(unsigned char *data, unsigned int datalen)
{
	unsigned char wCRCin = 0x00;
	unsigned char wCPoly = 0x07;

	while (datalen--)
	{
		wCRCin ^= *(data++);
		for (int i = 0; i < 8; i++)
		{
			if (wCRCin & 0x80)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	return (wCRCin);
}
/****************************Info**********************************************
* Name:    CRC-8/ITU           x8+x2+x+1
* Width:	8
* Poly:    0x07
* Init:    0x00
* Refin:   False
* Refout:  False
* Xorout:  0x55
* Alias:   CRC-8/ATM
*****************************************************************************/
unsigned char CRC8_ITU(unsigned char *data, unsigned int datalen)
{
	unsigned char wCRCin = 0x00;
	unsigned char wCPoly = 0x07;

	while (datalen--)
	{
		wCRCin ^= *(data++);
		for (int i = 0; i < 8; i++)
		{
			if (wCRCin & 0x80)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	return (wCRCin ^ 0x55);
}
/****************************Info**********************************************
* Name:    CRC-8/ROHC          x8+x2+x+1
* Width:	8
* Poly:    0x07
* Init:    0xFF
* Refin:   True
* Refout:  True
* Xorout:  0x00
* Note:
*****************************************************************************/
unsigned char CRC8_ROHC(unsigned char *data, unsigned int datalen)
{
	unsigned char wCRCin = 0xFF;
	unsigned char wCPoly = 0x07;

	InvertUint8(&wCPoly, &wCPoly);
	while (datalen--)
	{
		wCRCin ^= *(data++);
		for (int i = 0; i < 8; i++)
		{
			if (wCRCin & 0x01)
				wCRCin = (wCRCin >> 1) ^ wCPoly;
			else
				wCRCin = wCRCin >> 1;
		}
	}
	return (wCRCin);
}

/****************************Info**********************************************
* Name:    CRC-8/MAXIM         x8+x5+x4+1
* Width:	8
* Poly:    0x31
* Init:    0x00
* Refin:   True
* Refout:  True
* Xorout:  0x00
* Alias:   DOW-CRC,CRC-8/IBUTTON
* Use:     Maxim(Dallas)'s some devices,e.g. DS18B20
*****************************************************************************/
unsigned char CRC8_MAXIM(unsigned char *data, unsigned int datalen)
{
	unsigned char wCRCin = 0x00;
	unsigned char wCPoly = 0x31;

	InvertUint8(&wCPoly, &wCPoly);
	while (datalen--)
	{
		wCRCin ^= *(data++);
		for (int i = 0; i < 8; i++)
		{
			if (wCRCin & 0x01)
				wCRCin = (wCRCin >> 1) ^ wCPoly;
			else
				wCRCin = wCRCin >> 1;
		}
	}
	return (wCRCin);
}

int GetWidgetIndex(char*buf, char*name)
{
	int val;
	char *token;
	token = strtok(buf, name);
	while (token != NULL) {
		val = atoi(token);
		token = strtok(NULL, name);
	}
	return val;
}
/************************************************************/
float Hex_To_Decimal(unsigned char *Byte, int num)//十六进制到浮点数

{
#if 0
	char cByte[4];//方法一
	for (int i=0;i<num;i++) cByte[i] = Byte[i];
	float pfValue=*(float*)&cByte;
	return pfValue;
#else
	return *((float*)Byte);//方法二
#endif
}
long Float_To_hex(float HEX)//浮点数到十六进制转换1
{
	return *(long *)&HEX;
}

void Float_To_NByte(float floatNum, unsigned char* byteArry)////浮点数到十六进制转换2
{
	char* pchar = (char*)&floatNum;
	for (int i = 0; i<sizeof(float); i++){
		*byteArry = *pchar;
		pchar++;
		byteArry++;
	}
}
