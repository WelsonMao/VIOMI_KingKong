#ifndef __COPY_DIR_H__
#define __COPY_DIR_H__

#define STRLOG(s)     #s
#define TP_I2C_KEY_SLAVER_ADDR     (0x48>>1)    //7bit：0x24
#define TP_CMD_LEN                  7
#define TP_RETRY_CNT                5

typedef enum
{
	TP_STEP_DEFAULT = 0,
	TP_STEP_START = 1,
	TP_STEP_START_ACK = 2,
	TP_STEP_READ_VER = 3,
	TP_STEP_READ_VER_ACK=4,
	TP_STEP_RESUME_SCAN = 5,
	TP_STEP_RESUME_SCAN_ACK=6,
	TP_STEP_RESUME_DLY=7
}TP_STEP_e;
typedef struct
{
	TP_STEP_e step;
	uint16_t TPVer;
}TP_VER_t;
extern TP_VER_t tTPVersion;
void TPVersionTaskInit(void);


#endif