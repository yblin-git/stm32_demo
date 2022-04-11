/*
 * @Author: your name
 * @Date: 2020-05-10 15:49:18
 * @LastEditTime: 2020-06-02 09:48:49
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \MobileWrappingMachine_V3\Src\bsp\uart.c
 */
#include "user_uart.h"
#include "bsp_task.h"


/******************************************/
extern DMA_HandleTypeDef hdma_usart1_rx;
extern UART_HandleTypeDef huart1;

#define UART_CLEAR_SIZE 0
#define UART_FULL_SIZE 128

uint8_t s_uart1ReceiveData[128];
volatile const uint32_t s_uart1ReceiveDataBufSize = UART_FULL_SIZE;
volatile uint32_t s_uart1LastDmaTransRemainSize = UART_CLEAR_SIZE;

static volatile bool orderSending = false;
static void prvUart1_sendReceiveEvent(uint8_t *data, size_t size) {
  // printf("%s\n", __FUNCTION__);

	SPLIT_DATA(data, size,
						 (ARRARY_SIZE(((Msg_t *)(0))->d.Uart1ReceiveData.data) - 1), {
							 static Msg_t msg;
							 memset(&msg, 0, sizeof(Msg_t));
							 Msg_createUart1ReceiveData(&msg, splitData, splitDataSize);
							 MsgBus_fireISR(&msg);
						 });
}
static void prvUart1_processReceive(uint32_t dmaTransRemainSize) {
  // printf("%s\n", __FUNCTION__);

  if (s_uart1LastDmaTransRemainSize == dmaTransRemainSize) {
    // 没有变化
  } else if (s_uart1LastDmaTransRemainSize > dmaTransRemainSize) {
    //接收到新的数据
    uint8_t *receiveBegine = &s_uart1ReceiveData[s_uart1ReceiveDataBufSize -
                                                 s_uart1LastDmaTransRemainSize];
    size_t receiveSize = s_uart1LastDmaTransRemainSize - dmaTransRemainSize;
    prvUart1_sendReceiveEvent(receiveBegine, receiveSize);
  } else if (s_uart1LastDmaTransRemainSize < dmaTransRemainSize) {
    {
      uint8_t *receiveBegine =
          &s_uart1ReceiveData[s_uart1ReceiveDataBufSize -
                              s_uart1LastDmaTransRemainSize];
      size_t receiveSize = s_uart1LastDmaTransRemainSize;
      prvUart1_sendReceiveEvent(receiveBegine, receiveSize);
    }

    {
      uint8_t *receiveBegine = &s_uart1ReceiveData[0];
      size_t receiveSize = s_uart1ReceiveDataBufSize - dmaTransRemainSize;
      prvUart1_sendReceiveEvent(receiveBegine, receiveSize);
    }
  }
  s_uart1LastDmaTransRemainSize = dmaTransRemainSize;
}
static void prvUart1Enbale(){
  s_uart1LastDmaTransRemainSize = UART_CLEAR_SIZE;
  HAL_UART_Receive_DMA(&huart1, s_uart1ReceiveData, s_uart1ReceiveDataBufSize);
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);  //使能idle中断
}

/**
 * 串口接受完数据回调
 */
void Uart_enable() {
  prvUart1Enbale();
}

// void Uart_reEnableTouchScreenUart() {
//   HAL_UART_AbortReceive_IT(&huart2);
//   HAL_UART_Receive_DMA(&huart2, s_uart2ReceiveData, s_uart2ReceiveDataBufSize);
//   __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);  //使能idle中断
// }

bool Uart_isSending() { return orderSending; }
void Uart_asynSendMsg(UART_HandleTypeDef *huart, uint8_t *pData,
                          uint16_t Size) {
  orderSending = true;
  HAL_UART_Transmit_DMA(huart, pData, Size);
}

/***************************系统回调*************************/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  // printf("%s\n", __FUNCTION__);
  if (huart == &huart1) {
    prvUart1_processReceive(hdma_usart1_rx.Instance->CNDTR);
  }
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {
  // printf("%s\n", __FUNCTION__);
  if (huart == &huart1) {
    prvUart1_processReceive(hdma_usart1_rx.Instance->CNDTR);
  }
}

/**
 * 串口中断回调
 */
void HookBegin_USART1_IRQHandler(void) {
  if ((__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) !=
       RESET))  // idle标志被置位
  {
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    // printf("CNDTR %d\n", hdma_usart1_rx.Instance->CNDTR);
    /*当计数为0时，意味HAL_UART_RxCpltCallback，即将被回调，如果在此将其处理，则会导致多处理一次*/
    // printf("%s\n", __FUNCTION__);
    if (hdma_usart1_rx.Instance->CNDTR != 0 &&
        hdma_usart1_rx.Instance->CNDTR != s_uart1ReceiveDataBufSize / 2)
      prvUart1_processReceive(hdma_usart1_rx.Instance->CNDTR);
  }
}
void HookEnd_USART1_IRQHandler() {
  if (huart1.RxState == HAL_UART_STATE_READY) {
    prvUart1Enbale();
  }
}
/**
 * @description:
 * @param {type}
 * @return:
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
  // printf("%s\n", __FUNCTION__);
  if (huart == &huart1) {
    orderSending = false;
  }
}