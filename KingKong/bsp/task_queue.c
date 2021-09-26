#include "task_queue.h"

typedef struct Queue_tag{
	uart_event_msg_t TxEvent[MSG_SIZE];
	int front,rear;
}QUEUE;

static QUEUE ControlBoardUartQueue;

static QUEUE QueueCmdFromOut;


void InitQueue(QUEUE* Queue)
{
	int i;
	for (i=0;i<MSG_SIZE;i++)
	{
		Queue->TxEvent[i].uartForward       = UART_FORWARD_NONE;
		Queue->TxEvent[i].payloadInteger    = 0;
		memset(Queue->TxEvent[i].payloadBuf,0,sizeof(MSG_LEN));
 	}
 	Queue->front=Queue->rear=0;
}

int EmptyQueue(QUEUE* Queue)
{
 	return (Queue->front==Queue->rear);
}

int FullQueue(QUEUE* Queue)
{
 	return (((Queue->front+1)%MSG_SIZE)==Queue->rear);
}

void InQueue(QUEUE* Queue, uint8_t event_id, uint32_t integer, uint8_t *payloadBuf, uint32_t payloadLen)
{
 	if (!FullQueue(Queue))
 	{
 		Queue->TxEvent[Queue->front].uartForward        = event_id;
		Queue->TxEvent[Queue->front].payloadInteger     = integer;
		Queue->TxEvent[Queue->front].payloadLen         = payloadLen;
		memcpy(Queue->TxEvent[Queue->front].payloadBuf, payloadBuf, payloadLen);
  		Queue->front=(Queue->front+1)%MSG_SIZE;
 	}else{
  		//log_out("Queue is FULL ,can not input!\n");
 }
}

void InQueue_HPri(QUEUE* Queue, uint8_t forward, uint32_t integer, uint8_t *payloadBuf, uint32_t payloadLen)
{
	if (FullQueue(Queue))
	{
		//log_out("Queue is FULL ,delete front!\n");
		Queue->front = (Queue->front+(MSG_SIZE-1))%MSG_SIZE;
	}
	Queue->rear  = (Queue->rear+(MSG_SIZE-1))%MSG_SIZE;
 	Queue->TxEvent[Queue->rear].uartForward         = forward;
	Queue->TxEvent[Queue->rear].payloadInteger      = integer;
	Queue->TxEvent[Queue->rear].payloadLen          = payloadLen;
	memcpy(Queue->TxEvent[Queue->rear].payloadBuf, payloadBuf, payloadLen);
}

void InQueue_Repeat(QUEUE* Queue)
{
	if (FullQueue(Queue))
	{
		//log_out("Queue is FULL ,delete front!\n");
		Queue->front = (Queue->front+(MSG_SIZE-1))%MSG_SIZE;
	}
	Queue->rear = (Queue->rear+(MSG_SIZE-1))%MSG_SIZE;
}


int OutQueue(QUEUE* Queue, uart_event_msg_t * uart_tx)
{
 	int temp;
	//printf("%d-%d\n", Queue->rear, Queue->front);
 	if (!EmptyQueue(Queue))
 	{
		uart_tx->uartForward        = Queue->TxEvent[Queue->rear].uartForward;
		uart_tx->payloadInteger     = Queue->TxEvent[Queue->rear].payloadInteger;
		uart_tx->payloadLen         = Queue->TxEvent[Queue->rear].payloadLen;
		memcpy(uart_tx->payloadBuf, Queue->TxEvent[Queue->rear].payloadBuf, uart_tx->payloadLen);
  		Queue->rear                 =(Queue->rear+1)%MSG_SIZE;
 	}else{
  		//printf("Queue is empty\r\n");
		return -1;
 	}
 	return 0;
}
int OutPrevQueue(QUEUE* Queue)
{
	if (!EmptyQueue(Queue))
	Queue->rear = (Queue->rear + MSG_SIZE - 1) % MSG_SIZE;
	return 0;
}

void PrintQueue(QUEUE* Queue)
{
 	int front=Queue->front;
 	int rear =Queue->rear;
 	if (front == rear)
 	{
  		printf("Queue is empty!\r\n");
		return ;
 	}
	printf("Queue member:\r\n");
 	while(front != rear)
 	{
		printf("uartForward:%d, value:%d,cmd=%#x\r\n", Queue->TxEvent[rear].uartForward, Queue->TxEvent[rear].payloadInteger, Queue->TxEvent[rear].payloadBuf[9]);
  		rear=(rear+1)%MSG_SIZE;
 	}
 	printf("End\r\n");
}

int uartTaskPrintTest(void)
{
    PrintQueue(&ControlBoardUartQueue);
}

int uartTaskQueueInit(void)
{
    InitQueue(&ControlBoardUartQueue);
}

int uartTaskQueueIn(uint8_t uartForward, uint32_t interger, uint8_t *payloadBuf, int data_len)
{
    InQueue(&ControlBoardUartQueue,uartForward, interger, payloadBuf, data_len);
	return 1;
}
int uartTaskQueueIn_HPri(uint8_t uartForward, uint32_t interger, uint8_t *payloadBuf, int data_len)
{
	InQueue_HPri(&ControlBoardUartQueue, uartForward, interger, payloadBuf, data_len);
	return 1;
}
int uartTaskQueueOut(uart_event_msg_t * forwardEvent)
{
    int bRet = 0;
	bRet = OutQueue(&ControlBoardUartQueue, forwardEvent);
    //bRet = forwardEvent->uartForward;
	//PrintQueue(&ControlBoardUartQueue);
	//printf("uartTaskQueueOut:%d,%d\n", bRet, forwardEvent->payloadBuf[0]);
    return bRet;
}
int uartTaskPrevQueueOut(void)
{
	int bRet = 0;
	bRet = OutPrevQueue(&ControlBoardUartQueue);
	//bRet = forwardEvent->uartForward;
	return bRet;
}

int cmdTaskQueueInit(void)
{
    InitQueue(&QueueCmdFromOut);
}

int cmdTaskQueueIn(uint8_t eventMsg, uint32_t interger, uint8_t *payloadBuf, int data_len)
{
   InQueue(&QueueCmdFromOut,eventMsg, interger, payloadBuf, data_len);
   return 1;
}

int cmdTaskQueueOut(uart_event_msg_t * eventMsg)
{
    int bRet = 0;
    eventMsg->uartForward = 0;
	OutQueue(&QueueCmdFromOut, eventMsg);
    bRet = eventMsg->uartForward;
    return bRet;
}


