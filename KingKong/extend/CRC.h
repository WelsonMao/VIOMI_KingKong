#ifndef __CRC_H__
#define __CRC_H__
uint8_t GetCrc8(uint8_t init, uint8_t * buf, uint8_t size);
uint8_t GetCrc8Val(uint8_t*data, uint8_t offset, uint8_t length);
uint16_t Crc16Check(void const *puchMsg, unsigned short usDataLen);
uint16_t Crc16CheckRealTime(unsigned short vByte);
uint16_t GetLCRC16(uint8_t *buff,uint8_t cntnum);
uint16_t Aux_ChecksumGet(uint8_t *data, uint16_t len);
uint8_t CRCSumGet(uint8_t *data, uint16_t len);
uint16_t MB_CRC16(uint8_t *pushMsg, uint8_t usDataLen);
unsigned short CRC16_CCITT(unsigned char *data, unsigned int datalen);
unsigned short CRC16_CCITT_FALSE(unsigned char *data, unsigned int datalen);
unsigned short CRC16_XMODEM(unsigned char *data, unsigned int datalen);
unsigned short CRC16_MODBUS(unsigned char *data, unsigned int datalen);
unsigned short CRC16_IBM(unsigned char *data, unsigned int datalen);
unsigned short CRC16_MAXIM(unsigned char *data, unsigned int datalen);
unsigned short CRC16_USB(unsigned char *data, unsigned int datalen);
unsigned short CRC16_DNP(unsigned char *data, unsigned int datalen);
void InvertUint8(unsigned char *dBuf, unsigned char *srcBuf);
void InvertUint16(unsigned short *dBuf, unsigned short *srcBuf);
void InvertUint32(unsigned int *dBuf, unsigned int *srcBuf);
unsigned int CRC32(unsigned char *data, unsigned int datalen);
unsigned int CRC32_MPEG(unsigned char *data, unsigned int datalen);
unsigned char CRC4_ITU(unsigned char *data, unsigned int datalen);
unsigned char CRC5_EPC(unsigned char *data, unsigned int datalen);
unsigned char CRC5_USB(unsigned char *data, unsigned int datalen);

float Hex_To_Decimal(unsigned char *Byte, int num);
long Float_To_hex(float HEX);
void Float_To_NByte(float floatNum, unsigned char* byteArry);
#endif