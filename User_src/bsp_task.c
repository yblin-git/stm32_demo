/*
 * @Author: your name
 * @Date: 2020-04-29 21:57:58
 * @LastEditTime: 2020-06-07 12:23:38
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \MobileWrappingMachine_V3\Src\bsp_task.c
 */

#include "bsp_task.h"

#include "FreeRTOS.h"
#include "bsp/user_uart.h"
#include "bsp\flash_operate.h"
#include "bsp\led.h"
#include "bsp\ozone_pwm.h"
#include "bsp\user_gpio.h"
#include "hw_helper\z_io.h"
#include "main.h"
#include "msg_process_task.h"
#include "os_helper\swtimer.h"
#include "queue.h"
#include "stdlib.h"
#include "stm32f1xx_hal_tim.h"
#include "string.h"
#include "system.h"
#include "task.h"
#include "timers.h"
#include "tools\char_opertion.h"
#include "usart.h"
#include "zwtimec\utils\cbus_helper.h"
#include "zwtimec\utils\loop_queue.h"
#include "zwtimec\utils\simple_logger_helper.h"
//*******************宏定义***************
#define BspTaskSTK_SIZE (256)
#define BspTaskPRO (3)
#define BspTaskEvent_SIZE (100)
#define BspTaskMSG_SIZE (10)

#define RF_CLEAR_TIME 8

/******************extern*******************/
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim2;
//extern IWDG_HandleTypeDef hiwdg;
/******************************************/

cmdMenuDECLEAR_ENABLE_CMDS_FUNC(CmdMenu);
LOOP_QUEUE_ENABLE_SUPPORT_PROTECT(
    event, Event_t, { taskDISABLE_INTERRUPTS(); },
    { taskENABLE_INTERRUPTS(); });
LOOP_QUEUE_DEFINE(event, Event_t, 50);
LOOP_QUEUE_ENABLE_SUPPORT_PROTECT(
    msg, Msg_t, { taskDISABLE_INTERRUPTS(); }, { taskENABLE_INTERRUPTS(); });
LOOP_QUEUE_DEFINE(msg, Msg_t, 10);
LOOP_QUEUE_ENABLE_SUPPORT_PROTECT(
    log, Log_t, { taskDISABLE_INTERRUPTS(); }, { taskENABLE_INTERRUPTS(); });
LOOP_QUEUE_DEFINE(log, Log_t, 50);
LOOP_QUEUE_ENABLE_SUPPORT_PROTECT(
    orderSend, Msg_t, { taskDISABLE_INTERRUPTS(); },
    { taskENABLE_INTERRUPTS(); });
LOOP_QUEUE_DEFINE(orderSend, Msg_t, 20);



/*****************静态变量*****************/
static TaskHandle_t s_bspTaskHandle = NULL;
static TimerHandle_t s_instructionsSwtimer = NULL;  //定时闪烁指示灯
static TimerHandle_t s_keyScanSwtimer = NULL;       //扫描按键
static TimerHandle_t s_iWDGSwtimer = NULL;          //看门狗
static TimerHandle_t s_ozoneTimerSwtimer = NULL;          //看门狗

static KeyState_t keyState;

static uint32_t begin = 0;
static uint32_t end = 0;
static bool captureTrun = false;
static uint32_t count = 0;

static char logBuf[512];
static uint8_t orderSendBuf[512];

/*****************内部功能函数*****************/

/**
 * @description:
 * @param {type}
 * @return:
 */
static void prvPushEventFromISR(Event_t *evt) {
  // printf("prvPushEventFromISR\n");
  BaseType_t state;
  // if (xQueueSendFromISR(s_eventQueue, evt, &state))
  loop_queue_push_one_event(&s_eventQueue, *evt);
  vTaskNotifyGiveFromISR(s_bspTaskHandle, &state);  //发送任务通知
}
/**
 * @description:
 * @param {type}
 * @return:
 */
static void prvPushEvent(Event_t *evt) {
  // printf("prvPushEvent\n");
  // if (xQueueSend(s_eventQueue, evt, 0))
  loop_queue_push_one_event(&s_eventQueue, *evt);
  xTaskNotifyGive(s_bspTaskHandle);  //发送任务通知
}

/**
 * @description:
 * @param {type}
 * @return:
 */
static void prvPushMsgFromISR(Msg_t *msg) {
  // printf("%s\n", __FUNCTION__);
  BaseType_t state;
  // if (xQueueSendFromISR(s_msgQueue, msg, &state))
  loop_queue_push_one_msg(&s_msgQueue, *msg);
  vTaskNotifyGiveFromISR(s_bspTaskHandle, &state);  //发送任务通知
}
/**
 * @description:
 * @param {type}
 * @return:
 */
static void prvPushMsg(Msg_t *msg) {
  // printf("%s\n", __FUNCTION__);
  loop_queue_push_one_msg(&s_msgQueue, *msg);
  xTaskNotifyGive(s_bspTaskHandle);  //发送任务通知
}
/**
 * @description:
 * @param {type}
 * @return:
 */
static void prvPushOrderMsgFromISR(Msg_t *msg) {
  // printf("%s\n", __FUNCTION__);
  BaseType_t state;
  // if (xQueueSendFromISR(s_msgQueue, msg, &state))
  loop_queue_push_one_orderSend(&s_orderSendQueue, *msg);
  vTaskNotifyGiveFromISR(s_bspTaskHandle, &state);  //发送任务通知
}
/**
 * @description:
 * @param {type}
 * @return:
 */
static void prvPushOrderMsg(Msg_t *msg) {
  // printf("%s\n", __FUNCTION__);
  loop_queue_push_one_orderSend(&s_orderSendQueue, *msg);
  xTaskNotifyGive(s_bspTaskHandle);  //发送任务通知
}

/**
 * @description:
 * @param {type}
 * @return:
 */
static void prvPushLogFromISR(Log_t *log) {
  // printf("%s\n", __FUNCTION__);
  BaseType_t state;
  // if (xQueueSendFromISR(s_msgQueue, msg, &state))
  loop_queue_push_one_log(&s_logQueue, *log);
  vTaskNotifyGiveFromISR(s_bspTaskHandle, &state);  //发送任务通知
}
/**
 * @description:
 * @param {type}
 * @return:
 */
static void prvPushLog(Log_t *log) {
  // printf("%s\n", __FUNCTION__);
  // printf("s_logQueue size %d\n", loop_queue_get_size_log(&s_logQueue));
  bool ret = loop_queue_push_one_log(&s_logQueue, *log);
  // printf("s_logQueue size %d ret %d\n",
  //  loop_queue_get_size_log(&s_logQueue), ret);

  xTaskNotifyGive(s_bspTaskHandle);  //发送任务通知
}

/**
 * @description:
 * @param {type}
 * @return:
 */
static void prvToggleIndicatorLed(Event_t *pMsg) {
  LOG_INFO("prvToggleIndicatorLed\n");
  // HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
}

/**
 * @description:按键扫描处理函数
 * @param {type}
 * @return:
 */
static void prvKeyScanProcess(Event_t *pMsg) {
  KEY_SCAN(KEY1, keyState, kKEY1);
  KEY_SCAN(KEY2, keyState, kKEY2);
}

/**
 * @description:消息处理
 * @param {type}
 * @return:
 */
static bool prvFuncProcess(Event_t *event, Msg_t *msg, Log_t *log) {
  // printf("%s\n", __FUNCTION__);

  if (loop_queue_get_size_event(&s_eventQueue) != 0) {
    loop_queue_pop_one_event(&s_eventQueue, event);
    if (event->id == EventID_IndicatirledFlaseMsg) {
      prvToggleIndicatorLed(event);
    } else if (event->id == EventID_KeyScanEvent) {
      prvKeyScanProcess(event);
    } else if (event->id == EventID_IWDGEvent) {
      //HAL_IWDG_Refresh(&hiwdg);
    } 
  } else if (loop_queue_get_size_msg(&s_msgQueue) != 0) {
    //调试串口接收到的消息
    loop_queue_pop_one_msg(&s_msgQueue, msg);
    if (msg->id == MsgID_Uart1ReceiveData) {
      memcpy(logBuf, msg->d.Uart1ReceiveData.data,
             msg->d.Uart1ReceiveData.size);
      // HAL_UART_Transmit_IT(&huart1, (uint8_t *)logBuf, msg->d.LogMsg.size);
      // //????
      CmdMenu_processReceive((const char *)msg->d.Uart1ReceiveData.data,
                             msg->d.Uart1ReceiveData.size);
    }
  }else if (loop_queue_get_size_log(&s_logQueue) != 0) {
    if (Uart_isSending() == false) {
      loop_queue_pop_one_log(&s_logQueue, log);
      if (log->id == LogID_LogMsg) {
        memset(logBuf, 0, 512);
        memcpy(logBuf, log->d.LogMsg.data, log->d.LogMsg.size);
        Uart_asynSendMsg(&huart1, (uint8_t *)logBuf, log->d.LogMsg.size);
        // printf("%s", logBuf);
        return true;
      }
    } else {
      return false;  //不发送
    }
  } else {
    return true;
  }
  return false;
}
/*************************************回调函数**********************************/
/**
 * @description:监听Event事件
 * @param {type}
 * @return:
 */
static void prvEventListenerCB(Event_t *evt, bool context) {
  if (evt->id == EventID_KeyScanEvent ||
      evt->id == EventID_IndicatirledFlaseMsg || evt->id == EventID_IWDGEvent) {
    if (context) {
      prvPushEvent(evt);
    } else {
      prvPushEventFromISR(evt);
    }
  }
}
/**
 * @description:监听msg事件
 * @param {type}
 * @return:
 */
static void prvMsgListenerCB(Msg_t *msg, bool context) {
  // printf("%s\n", __FUNCTION__);
  if (msg->id == MsgID_Uart1ReceiveData) {
    if (context) {
      prvPushMsg(msg);
    } else {
      prvPushMsgFromISR(msg);
    }
  } else if (msg->id == MsgID_OrderUartSendMsg) {
    if (context) {
      prvPushOrderMsg(msg);
    } else {
      prvPushOrderMsgFromISR(msg);
    }
  }
}
/**
 * @description:监听log事件
 * @param {type}
 * @return:
 */
static void prvLogListenerCB(Log_t *log, bool context) {
  // printf("%s\n", __FUNCTION__);

  if (context) {
    prvPushLog(log);
  } else {
    prvPushLogFromISR(log);
  }
}
/**
 * @description:定时电量采集回调幻术
 * @param {type}
 * @return:
 */
static void prvInstructionsSwtimerCB(xTimerHandle xTimer) {
  Event_t evt;
  evt.id = EventID_IndicatirledFlaseMsg;
  EventBus_fire(&evt);
}

/**
 * @description:按键扫描回调
 * @param {type}
 * @return:
 */
static void prvKeyScanSwtimerCB(xTimerHandle xTimer) {
  Event_t evt;
  evt.id = EventID_KeyScanEvent;
  EventBus_fire(&evt);
}

static void prvIWDGSwtimerCB(xTimerHandle xTimer) {
  Event_t evt;
  IWDGEvent_t IWDGEvent;
  Event_createIWDGEvent(&evt, IWDGEvent);
  EventBus_fire(&evt);
}
/**
 * @description: 
 * @param {type} 
 * @return: 
 */
static void prvOzoneTimerSwtimerCB(xTimerHandle xTimer) { BspTask_shutdown(); }
/**
 * @description:
 * @param {type}
 * @return:
 */
static void prvFuncCB(void *pvParamter) {
  uint32_t notifyValue;
  Event_t event;
  Msg_t msg;
  Log_t log;
  bool isNotify = true;
  State_set_initialized(true);
  while (1) {
    if (isNotify) {
      // printf("ulTaskNotifyTake isNotify %d \n", isNotify);
      notifyValue = ulTaskNotifyTake(pdFALSE, portMAX_DELAY);  //获取任务通知
      // printf("notifyValue %d\n", notifyValue);
      if (notifyValue > 0) {
        isNotify = prvFuncProcess(&event, &msg, &log);
      }
    } else {
      isNotify = prvFuncProcess(&event, &msg, &log);
    }
  }
}
/*************************************外部函数**********************************/
/**
 * @description: bspTask init
 * @param {type}
 * @return:
 */
void BspTask_init(void) {
  //创建队列

  LOOP_QUEUE_INIT(event);
  LOOP_QUEUE_INIT(msg);
  LOOP_QUEUE_INIT(log);
  LOOP_QUEUE_INIT(orderSend);

  KEY_STATE_CLEAR(KEY1, keyState, GPIO_PIN_SET);
  KEY_STATE_CLEAR(KEY2, keyState, GPIO_PIN_SET);

  EventBus_regListerner(prvEventListenerCB);
  MsgBus_regListerner(prvMsgListenerCB);
  LogBus_regListerner(prvLogListenerCB);

  CmdMenu_enableCmdSInThisModle();

  Uart_enable();
  // create emegency task
  xTaskCreate((TaskFunction_t)prvFuncCB,  //任务函数
              (const char *)"Bsp Task",   //任务名称
              (uint16_t)BspTaskSTK_SIZE,  //任务堆栈大小
              (void *)NULL,               //传递给任务函数的参数
              (UBaseType_t)BspTaskPRO,    //任务优先级
              (TaskHandle_t *)&s_bspTaskHandle);  //任务句柄
  if (s_bspTaskHandle == NULL) printf("Bsp Task cerate fail\n");

  // 指示灯周期性翻转定时器h获取电池电量数据

  s_instructionsSwtimer =
      zOSTimerCreate("swtimer", boardBATTERYLEVEL_PERIODUS, (UBaseType_t)pdTRUE,
                     (void *)1, prvInstructionsSwtimerCB);
  if (s_instructionsSwtimer) xTimerStart(s_instructionsSwtimer, 0);

  s_keyScanSwtimer =
      zOSTimerCreate("key sacn swtimer", boardKEYSCAN_PERIODUS,
                     (UBaseType_t)pdTRUE, (void *)1, prvKeyScanSwtimerCB);
  if (s_keyScanSwtimer) xTimerStart(s_keyScanSwtimer, 0);

  s_iWDGSwtimer = zOSTimerCreate("swtimer", IWDG_TIME_OUT, (UBaseType_t)pdTRUE,
                                 (void *)1, prvIWDGSwtimerCB);
  if (s_iWDGSwtimer) xTimerStart(s_iWDGSwtimer, 0);

  s_ozoneTimerSwtimer =
      xTimerCreate("swtimer", (TickType_t)OZONE_TWO_HOUR, (UBaseType_t)pdFALSE,
                   (void *)1, prvOzoneTimerSwtimerCB);

  printf("enter normal mode\n");
}

void BspTask_showHightLevelLed(){
  Led_closeRGB();
  Led_showRGB_R(true);
}
void BspTask_showMiddleLevelLed(){
  Led_closeRGB();
  Led_showRGB_G(true);
}
void BspTask_showLowLevelLed(){
  Led_closeRGB();
  Led_showRGB_B(true);
}
void BspTask_showTwoHourLed(){
  Led_closeLed();
  Led_showLed3(true);
}
void BspTask_showFourHourLed(){
  Led_closeLed();
  Led_showLed2(true);
}
void BspTask_showSixHourLed(){
  Led_closeLed();
  Led_showLed1(true);
}
void BspTask_showEightHourLed(){
  Led_closeLed();
  Led_showLed0(true);
}
void BspTask_openFan() {
  GPIO_WIRTE(FAN_ON, GPIO_PIN_SET);
}
void BspTask_closeFan(){ 
  GPIO_WIRTE(FAN_ON, GPIO_PIN_RESET);
}
void BspTask_stopTimer() {
  if (s_ozoneTimerSwtimer) {
    xTimerStop(s_ozoneTimerSwtimer, 0);
  }
  Led_closeLed();
  State_set_timerState(kTimerStateIdel);
}
void BspTask_startTimer(uint32_t periodMs) {
  if (s_ozoneTimerSwtimer) {
    xTimerChangePeriod(s_ozoneTimerSwtimer, periodMs, 0);
    xTimerStart(s_ozoneTimerSwtimer, 0);
  }
}
void BspTask_openOzoneHightLevel() {
  BspTask_showHightLevelLed();
  BspTask_openFan();
  Ozone_setLevel(kWorkStateHight);
  Ozone_Start();
}
void BspTask_openOzoneMiddleLevel() {
  BspTask_showMiddleLevelLed();
  BspTask_openFan();
  Ozone_setLevel(kWorkStateMiddle);
  Ozone_Start();
}
void BspTask_openOzoneLowLevelt() {
  BspTask_showLowLevelLed();
  BspTask_openFan();
  Ozone_setLevel(kWorkStateLow);
  Ozone_Start();
}
void BspTask_closeOzone() {
  BspTask_closeFan();
  Led_closeLed();
  BspTask_stopTimer();
  Ozone_Stop();
}

/**
 * @description: 开机
 * @param {type}
 * @return:
 */
void BspTask_powerOn() {
  
}
/**
 * @description: 关机
 * @param {type}
 * @return:
 */
void BspTask_shutdown() {
  State_set_workState(kWorkStateIdel);
  State_set_timerState(kTimerStateIdel);
  BspTask_closeOzone();
  BspTask_stopTimer();
  Led_closeRGB();
  Led_closeLed();
}
/********************************支持的方法列表**********************************************************/
#define SHOW_CONFIG_MARCO(_, indexValue, type, name, ...)           \
  LOG_INFO("Index:%d " #name " = " cBeanHelperFormatID_##type "\n", \
           indexValue, Config_get_##name());

#define SHOW_STATE_MARCO(_, indexValue, type, name, ...)            \
  LOG_INFO("Index:%d " #name " = " cBeanHelperFormatID_##type "\n", \
           indexValue, State_get_##name());

    cmdMenuDEFINE_CMD(showALLConfig, "showALLConfig", 1, {
      configLIST(x, SHOW_CONFIG_MARCO);
      return true;
    });
cmdMenuDEFINE_CMD(showALLState, "showALLState", 1, {
  stateLIST(x, SHOW_STATE_MARCO);
  return true;
});

cmdMenuDEFINE_CMD(flash_write, "flash_write", 1, {
  Flash_read();
  FlashData_t *flashData = Flash_getPointer();
  LOG_INFO("initialized %d\n", flashData->initialized);
  LOG_INFO("markbegin %d\n", flashData->markbegin);
  LOG_INFO("markend %d\n", flashData->markend);
  LOG_INFO("logLevel %d\n", flashData->config.logLevel);
  LOG_INFO("inTestMode %d\n", flashData->config.inTestMode);
  //HAL_IWDG_Refresh(&hiwdg);
  Flash_flush();
  return true;
});
cmdMenuDEFINE_CMD(flash_read, "flash_read", 1, {
  Flash_read();
  FlashData_t *flashData = Flash_getPointer();
  LOG_INFO("initialized %d", flashData->initialized);
  LOG_INFO("markbegin %d", flashData->markbegin);
  LOG_INFO("markend %d", flashData->markend);
  LOG_INFO("logLevel %d", flashData->config.logLevel);
  return true;
});
cmdMenuDEFINE_CMD(Config_save, "Config_save", 1, {
  Flash_getPointer()->config = *Config_get();
  //HAL_IWDG_Refresh(&hiwdg);
  Flash_flush();
  return true;
});
cmdMenuDEFINE_CMD(select_mode, "select_mode test:0 normal:1", 2, {
  LOG_INFO("select_mode : %d\n", atoi(argv[1]));
  if (atoi(argv[1]) == 0) {
    Config_set_inTestMode(true);
  } else {
    Config_set_inTestMode(false);
  }
  Flash_getPointer()->config = *Config_get();
  //HAL_IWDG_Refresh(&hiwdg);
  Flash_flush();
  NVIC_SystemReset();
  return true;
});
cmdMenuDEFINE_CMD(set_log_level, "set_log_level ", 2, {
  LOG_INFO("set_log_level : %d\n", atoi(argv[1]));
  if (atoi(argv[1]) == 0) {
    Config_set_inTestMode(true);
  } else {
    Config_set_inTestMode(false);
  }
  Config_set_logLevel(atoi(argv[1]));
  return true;
});
cmdMenuDEFINE_CMD(test_bin, "test_bin", 2, {
  LOG_INFO("test_bin:%s %d\n", argv[1], strlen(argv[1]));
  stringToBin((char *)argv[1]);
  return true;
});
cmdMenuDEFINE_CMD(tast_startPwm, "tast_startPwm", 2, {
  LOG_INFO("tast_startPwm : %d\n", atoi(argv[1]));
  Pwn_setCompare(atoi(argv[1]));
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  LOG_INFO("AUTORELOAD %d", __HAL_TIM_GET_AUTORELOAD(&htim2));
  return true;
});
cmdMenuDEFINE_CMD(tast_stopPwm, "tast_stopPwm", 1, {
  LOG_INFO("tast_stopPwm \n");
  HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
  return true;
});
cmdMenuDEFINE_CMD(tast_pwmChangeFre, "tast_pwmChangeFre", 2, {
  LOG_INFO("tast_pwmChangeFre : %d\n", atoi(argv[1]));
  LOG_INFO("AUTORELOAD %d", __HAL_TIM_GET_AUTORELOAD(&htim2));
  Pwn_setFrequency(atoi(argv[1]));
  LOG_INFO("AUTORELOAD %d", __HAL_TIM_GET_AUTORELOAD(&htim2));
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  return true;
});
cmdMenuDEFINE_ENABLE_CMDS_FUNC(CmdMenu,  //
                               showALLConfig, showALLState, flash_write,
                               flash_read, Config_save, select_mode,
                               set_log_level, test_bin, tast_startPwm,
                               tast_stopPwm, tast_pwmChangeFre)
