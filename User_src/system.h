/*
 * @Author: your name
 * @Date: 2020-04-30 11:41:16
 * @LastEditTime: 2020-06-06 12:35:40
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \MobileWrappingMachine_V3\Src\system.h
 */
#ifndef __SYSTEM_H
#define __SYSTEM_H
#include "zwtimec/utils/bean_helper.h"
#include "zwtimec/utils/cbus_helper.h"
#include "zwtimec/utils/cmd_menu_helper.h"
#include "zwtimec/utils/simple_logger_helper.h"
#include "zwtimec/utils/marco_utils.h"
#include "stdio.h"
#include "usart.h"

int fputc(int c, FILE *stream);
typedef struct {
  GPIO_PinState state;
  uint32_t times;
} key_t;
typedef struct {
  key_t KEY1_state;
  key_t KEY2_state;
} KeyState_t;
typedef enum {
  kKEY1 = 0,
  kKEY2 = 1,
} keyType_e;
typedef enum {
  kRelease = 0,
  kPress,
} keyEventType_e;
typedef enum {
  kWorkStateIdel = 0,
  kWorkStateHight = 1,
  kWorkStateMiddle = 2,
  kWorkStateLow = 3,
} workStateType_e;
typedef enum {
  kTimerStateIdel = 0,
  kTimerStateTwoHour = 1,
  kTimerStateFourHour = 2,
  kTimerStateSixHour = 3,
  kTimerStateEightHour = 4,
  kTimerStateStop = 5,
} timerStateType_e;

/**
 * @brief 系统配置
 */
#define configLIST(_, marco)                     /**/                       \
  marco(_, 0, int32_t, logLevel, 2, 0, 5)        /*logLevel*/               \
      marco(_, 1, bool, inTestMode, false, 0, 1) /**/                       
cBeanHelperDECLEAR(Config, configLIST);

/**
 * @brief 系统状态
 */
#define stateLIST(_, marco)                                     /**/ \
      marco(_, 1, bool, powerKeyState, false, false, true)      /**/ \
      marco(_, 2, bool, onOffMachineState, false, false, true)  /**/ \
      marco(_, 3, bool, initialized, false, false, true)             \
      marco(_, 4, uint8_t, workState,kWorkStateIdel, kWorkStateIdel, kWorkStateLow)\
      marco(_, 5, uint8_t, timerState,kTimerStateIdel, kTimerStateIdel, kTimerStateStop)
cBeanHelperDECLEAR(State, stateLIST);
/**
 * @brief EventBus定义
 */
/*********************************EventBus定义******************************* */
typedef struct { bool pad; } IWDGEvent_t;
typedef struct { bool pad; } DebugUartSendStateIsIDLE_t;
typedef struct { bool pad; } OrderUartSendStateIsIDLE_t;
typedef struct { bool pad; } DebugUartSendQueueIsEmpty_t;
typedef struct { bool pad; } OrderUartSendQueueIsEmpty_t;
typedef struct { bool pad; } IndicatirledFlaseMsg_t;

typedef struct {
  ConfigIndexEnum_t index;
  Iterm_t oldValue;
  Iterm_t newValue;
} ConfigChangeEventContent_t;

typedef struct {
  StateIndexEnum_t index;
  Iterm_t oldValue;
  Iterm_t newValue;
} StateChangeEventContent_t;

typedef struct {
  bool pad;
} KeyScanEvent_t;
typedef struct {
  keyType_e key;
  keyEventType_e type;
  uint8_t seconds;
} KeyEvent_t;
/**
 * @brief 事件列表
 */
#define EVENT_LIST(_, marco)              /**/                         \
      marco(_, IWDGEvent)                      /*按键消息*/             \
      marco(_, KeyEvent)                      /*按键消息*/              \
      marco(_, ConfigChangeEventContent)  /**/                         \
      marco(_, DebugUartSendStateIsIDLE)  /**/                         \
      marco(_, OrderUartSendStateIsIDLE)  /**/                         \
      marco(_, DebugUartSendQueueIsEmpty) /**/                         \
      marco(_, OrderUartSendQueueIsEmpty) /**/                         \
      marco(_, StateChangeEventContent)                                \
      marco(_, IndicatirledFlaseMsg)                                   \
      marco(_, KeyScanEvent)                                           
    cbusHelperDECLEAR(Event, EVENT_LIST)

cbusDEFINE_ITERM_CREATE_FUNC_VERSION2(Event, IWDGEvent);
cbusDEFINE_ITERM_CREATE_FUNC_VERSION2(Event, KeyEvent);
cbusDEFINE_ITERM_CREATE_FUNC_VERSION2(Event, DebugUartSendStateIsIDLE);
cbusDEFINE_ITERM_CREATE_FUNC_VERSION2(Event, OrderUartSendStateIsIDLE);
cbusDEFINE_ITERM_CREATE_FUNC_VERSION2(Event, OrderUartSendQueueIsEmpty);
cbusDEFINE_ITERM_CREATE_FUNC_VERSION2(Event, DebugUartSendQueueIsEmpty);
cbusDEFINE_ITERM_CREATE_FUNC_VERSION2(Event, StateChangeEventContent);
cbusDEFINE_ITERM_CREATE_FUNC_VERSION2(Event, ConfigChangeEventContent);
cbusDEFINE_ITERM_CREATE_FUNC_VERSION2(Event, IndicatirledFlaseMsg);

/*********************************MsgBus定义******************************* */
typedef struct
{
  char data[64];
  uint16_t size;
} OrderUartReceiveMsg_t;
typedef struct
{
  char data[64];
  uint16_t size;
} OrderUartSendMsg_t;
typedef struct
{
  uint8_t data[64];
  size_t size;
} Uart2ReceiveData_t;
typedef struct
{
  uint8_t data[64];
  size_t size;
} Uart1ReceiveData_t;
/**
 * @brief 事件列表
 */
#define MSG_LIST(_, marco) /**/ \
  marco(_, Uart2ReceiveData)\
  marco(_, Uart1ReceiveData)\
  marco(_, OrderUartSendMsg)
/**
 * @brief 定义Event
 */
cbusHelperDECLEAR(Msg, MSG_LIST)

// cbusDEFINE_ITERM_CREATE_FUNC_VERSION2(Msg, Uart2ReceiveData);
static INLINE void Msg_createUart2ReceiveData(Msg_t *msg, uint8_t *data,
                                              size_t size)
{
  msg->id = MsgID_Uart2ReceiveData;
  uint32_t maxCpy = ARRARY_SIZE(msg->d.Uart2ReceiveData.data);
  if (maxCpy > size)
  {
    memcpy(msg->d.Uart2ReceiveData.data, data, size);
    msg->d.Uart2ReceiveData.size = size;
  }
  else
  {
    memcpy(msg->d.Uart2ReceiveData.data, data, maxCpy);
    msg->d.Uart2ReceiveData.size = maxCpy;
  }
}
static INLINE void Msg_createOrderUartSendMsg(Msg_t *msg, uint8_t *data,
                                              size_t size) {
  msg->id = MsgID_OrderUartSendMsg;
  uint32_t maxCpy = ARRARY_SIZE(msg->d.OrderUartSendMsg.data);
  if (maxCpy > size) {
    memcpy(msg->d.OrderUartSendMsg.data, data, size);
    msg->d.OrderUartSendMsg.size = size;
  } else {
    memcpy(msg->d.OrderUartSendMsg.data, data, maxCpy);
    msg->d.OrderUartSendMsg.size = maxCpy;
  }
}
static INLINE void Msg_createUart1ReceiveData(Msg_t *msg, uint8_t *data,
                                              size_t size)
{
  msg->id = MsgID_Uart1ReceiveData;
  uint32_t maxCpy = ARRARY_SIZE(msg->d.Uart1ReceiveData.data);
  if (maxCpy > size)
  {
    memcpy(msg->d.Uart1ReceiveData.data, data, size);
    msg->d.Uart1ReceiveData.size = size;
  }
  else
  {
    memcpy(msg->d.Uart1ReceiveData.data, data, maxCpy);
    msg->d.Uart1ReceiveData.size = maxCpy;
  }
}

/*********************************LogBus定义******************************* */
typedef struct
{
  char data[64];
  uint16_t size;
} LogMsg_t;
/**
 * @brief 事件列表
 */
#define LOG_LIST(_, marco) /**/ \
  marco(_, LogMsg)         /**/ 

/**
 * @brief 定义Event
 */
cbusHelperDECLEAR(Log, LOG_LIST)

cbusDEFINE_ITERM_CREATE_FUNC_VERSION2(Log, LogMsg);

/*********************************指令Table支持********************************/
cmdMenuDECLEAR(CmdMenu, 100 /*cmdMaxNum*/, "\r\n")
/*-----------------------------flash 存储*************************************/

#endif /* __SYSTEM_H */
