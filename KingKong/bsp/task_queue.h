#ifndef __TASK_QUEUE_H__
#define __TASK_QUEUE_H__
//#include "hport.h"
//#include "serial_com.h"
//#include "ddl.h"

#include <assert.h>
#include "../scene.h"
#include "ite/itp.h"

#define UART_SEND_USE_QUEUE

#define MSG_SIZE 				(20)
#define BOARD_WIFI_BUF_LEN 		(200)
#define MSG_LEN  				(BOARD_WIFI_BUF_LEN+1)

typedef struct uart_event_msg
{
    uint8_t   uartForward;
    uint32_t  payloadInteger;
	uint32_t  payloadLen;
	uint8_t   payloadBuf[MSG_LEN];
}uart_event_msg_t;

typedef enum{
    UART_FORWARD_NONE,
    UART_FORWARD_MAINBOARD,
    UART_FORWARD_WIFI,
}UART_FOWEARD;

int uartTaskPrintTest(void);
int uartTaskQueueInit(void);
int uartTaskQueueIn(uint8_t uartForward, uint32_t interger, uint8_t *payloadBuf, int data_len);
int uartTaskQueueOut(uart_event_msg_t * forwardEvent);
int uartTaskPrevQueueOut(void);
int uartTaskQueueIn_HPri(uint8_t uartForward, uint32_t interger, uint8_t *payloadBuf, int data_len);

int cmdTaskQueueInit(void);
int cmdTaskQueueIn(uint8_t eventMsg, uint32_t interger, uint8_t *payloadBuf, int data_len);
int cmdTaskQueueOut(uart_event_msg_t * eventMsg);

#endif
