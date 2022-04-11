/*
 * @Author: your name
 * @Date: 2020-04-29 21:57:58
 * @LastEditTime: 2020-06-07 13:18:55
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \MobileWrappingMachine_V3\Src\bsp_task.c
 */
#include "FreeRTOS.h"
#include "board.h"
#include "bsp\flash_operate.h"
#include "bsp\led.h"
#include "bsp\user_gpio.h"
#include "bsp_task.h"
#include "helper/dwin_packet.h"
#include "os_helper\swtimer.h"
#include "queue.h"
#include "stdlib.h"
#include "string.h"
#include "system.h"
#include "task.h"
#include "timers.h"
#include "zwtimec/utils/cbus_helper.h"
#include "zwtimec\utils\loop_queue.h"
#include "zwtimec\utils\simple_logger_helper.h"

//*******************宏定义***************
#define MsgProcessTaskSTK_SIZE (256)
#define MsgProcessTaskPRO (2)
#define MsgProcessTaskEvent_SIZE (100)
#define MsgProcessTaskMSG_SIZE (10)

#define CONFIRM_TIME 3
#define ENTER_TEST_PAGE_TIME 5

/******************************************/

LOOP_QUEUE_ENABLE_SUPPORT_PROTECT(mPEvent, Event_t,
                                  {taskDISABLE_INTERRUPTS(); },
                                  {taskENABLE_INTERRUPTS();});
LOOP_QUEUE_DEFINE(mPEvent, Event_t, 50);
LOOP_QUEUE_ENABLE_SUPPORT_PROTECT(
    order, Msg_t, { taskDISABLE_INTERRUPTS(); }, { taskENABLE_INTERRUPTS(); });
LOOP_QUEUE_DEFINE(order, Msg_t, 20);
void MsgProcessTask_enterProductTestPage();

/*****************静态变量*****************/
static TimerHandle_t s_countDownSwtimer = NULL;
static TimerHandle_t s_getPageNumSwtimer = NULL;
static TimerHandle_t s_TemperatureDetectionSwtimer = NULL;
static TimerHandle_t s_voltageTypeCheckSwtimer = NULL;
static TimerHandle_t s_pairProcessSwtimer = NULL;
static TimerHandle_t s_idleDetectionSwtimer = NULL;

static TaskHandle_t s_msgProcessTaskHandle = NULL;
static char s_msgProcessLogBuf[512];
static uint32_t countDownTimes = 0;
static uint32_t useDetergentVolume = 0;
static uint32_t useTime = 0;
static int32_t remainingTime = 0;
static int32_t disinfectTime = 0;
static uint32_t tempAlemTimes = 0;
static uint32_t tempAlemTimes2 = 0;

#define MAX_PACKAGE_SIZE 64
static uint8_t packetReceiveBuffer[MAX_PACKAGE_SIZE * 2] = {0};
static uint8_t packetProcessBuffer[MAX_PACKAGE_SIZE * 2 + 2] = {0};
static uint8_t packetSendBuffer[MAX_PACKAGE_SIZE * 2] = {0};
static DwinPacketProcesser_t processser;

static uint8_t voltage220TypeCount = 0;
static uint8_t voltage110TypeCount = 0;

/**
 * @description:
 * @param {type}
 * @return:
 */
static void prvPushEventFromISR(Event_t *evt) {
  BaseType_t state;
  loop_queue_push_one_mPEvent(&s_mPEventQueue, *evt);
  vTaskNotifyGiveFromISR(s_msgProcessTaskHandle, &state);  //发送任务通知
}
/**
 * @description:
 * @param {type}
 * @return:
 */
static void prvPushEvent(Event_t *evt) {
  loop_queue_push_one_mPEvent(&s_mPEventQueue, *evt);
  xTaskNotifyGive(s_msgProcessTaskHandle);  //发送任务通知
}

/**
 * @description:
 * @param {type}
 * @return:
 */
static void prvPushOrderFromISR(Msg_t *msg) {
  BaseType_t state;
  loop_queue_push_one_order(&s_orderQueue, *msg);
  vTaskNotifyGiveFromISR(s_msgProcessTaskHandle, &state);  //发送任务通知
}
/**
 * @description:
 * @param {type}
 * @return:
 */
static void prvPushOrder(Msg_t *msg) {
  loop_queue_push_one_order(&s_orderQueue, *msg);
  xTaskNotifyGive(s_msgProcessTaskHandle);  //发送任务通知
}

static void prvTimingProcessing() {
  LOG_INFO("timerState %d", State_get_timerState());

  switch (State_get_timerState()) {
    case kTimerStateIdel:
      BspTask_stopTimer();
      break;
    case kTimerStateTwoHour:
      BspTask_showTwoHourLed();
      BspTask_startTimer(OZONE_TWO_HOUR);
      break;
    case kTimerStateFourHour:
      BspTask_showFourHourLed();
      BspTask_startTimer(OZONE_FOUR_HOUR);
      break;
    case kTimerStateSixHour:
      BspTask_showSixHourLed();
      BspTask_startTimer(OZONE_SIX_HOUR);
      break;
    case kTimerStateEightHour:
      BspTask_showEightHourLed();
      BspTask_startTimer(OZONE_EIGHT_HOUR);
      break;
    default://停止定时器
      BspTask_stopTimer();
      break;
  }
}
/**
 * @description:按键处理
 * @param {type}
 * @return:
 */
static void prvPowerKeyProcess(Event_t *evt) {
  if (evt->d.KeyEvent.type == kPress) {
    State_set_powerKeyState(true);
    if (evt->d.KeyEvent.seconds == SHORT_PRESS_TIMES) {  //短按 开始
      LOG_INFO("workState %d", State_get_workState());
      if (State_get_workState() == kWorkStateIdel) {
        State_set_workState(kWorkStateHight);
        BspTask_openOzoneHightLevel();  //最高等级产生臭氧 打开风扇
      } else if (State_get_workState() == kWorkStateHight) {
        State_set_workState(kWorkStateMiddle);
        BspTask_openOzoneMiddleLevel();  //中等级产生臭氧 打开风扇
      } else if (State_get_workState() == kWorkStateMiddle) {
        State_set_workState(kWorkStateLow);
        BspTask_openOzoneLowLevelt();  //低高等级产生臭氧 打开风扇
      } else if (State_get_workState() == kWorkStateLow) {
        State_set_workState(kWorkStateIdel);
        BspTask_shutdown();  //关闭臭氧发生 关闭风扇
      }
    } else if (evt->d.KeyEvent.seconds ==
               LOING_PRESS_TIMES) {  //长按 请求关机 屏幕关机
        BspTask_shutdown();
    } else if (evt->d.KeyEvent.seconds ==
               LOING_LOING_PRESS_TIMES) {  //长长按 延时 关机
    }
  } else {
    State_set_powerKeyState(false);
  }
}

/**
 * @description:
 * @param {type}
 * @return:
 */
static void prvTimerKeyProcess(Event_t *evt) {
  if (evt->d.KeyEvent.type == kPress) {
    State_set_powerKeyState(true);
    if (evt->d.KeyEvent.seconds == SHORT_PRESS_TIMES) {  //短按 开始
      if (State_get_workState() != kWorkStateIdel) {
        uint8_t times = State_get_timerState();
        times++;
        State_set_timerState(times);
        prvTimingProcessing();
      }
    } else if (evt->d.KeyEvent.seconds ==
               LOING_PRESS_TIMES) {  //长按 请求关机 屏幕关机
                                     // BspTask_shutdown();
    } else if (evt->d.KeyEvent.seconds ==
               LOING_LOING_PRESS_TIMES) {  //长长按 延时 关机
    }
  } else {
    State_set_powerKeyState(false);
  }
}
/********************************内部任务********************************/
/**
 * @description:
 * @param {type}
 * @return:
 */
static bool prvTaskFuncProcess(Event_t *event, Msg_t *msg) {
  if (loop_queue_get_size_mPEvent(&s_mPEventQueue) != 0) {
    loop_queue_pop_one_mPEvent(&s_mPEventQueue, event);
    if (event->id == EventID_KeyEvent) {
      switch (event->d.KeyEvent.key) {
        case kKEY2:
          prvPowerKeyProcess(event);
          break;
        case kKEY1:
          prvTimerKeyProcess(event);
          break;
        defined:
          break;
      }
    }
  } else if (loop_queue_get_size_order(&s_orderQueue) != 0) {
    loop_queue_pop_one_order(&s_orderQueue, msg);

    if (msg->id == MsgID_Uart2ReceiveData) {
    }
  } else {
    return true;
  }
  return false;
}
/*************************************回调函数**********************************/
/**
 * @description:
 * @param {type}
 * @return:
 */
static void prvEventListenerCB(Event_t *evt, bool context) {
  if (evt->id == EventID_KeyEvent) {
    if (context) {
      prvPushEvent(evt);
    } else {
      prvPushEventFromISR(evt);
    }
  }
}
/**
 * @description:
 * @param {type}
 * @return:
 */
static void prvMsgListenerCB(Msg_t *msg, bool context) {
  // LOG_INFO("%s\n", __FUNCTION__);

  if (msg->id == MsgID_Uart2ReceiveData) {
    if (context) {
      prvPushOrder(msg);
    } else {
      prvPushOrderFromISR(msg);
    }
  }
}

/**
 * @description: task 的回调函数
 * @param {type}
 * @return:
 */
static void prvTaskFuncCB(void *pvParamter) {
  uint32_t notifyValue;
  Event_t event;
  Msg_t msg;
  Log_t log;
  bool isNotify = true;
  while (1) {
    if (isNotify) {
      notifyValue = ulTaskNotifyTake(pdFALSE, portMAX_DELAY);  //获取任务通知
      // printf("notifyValue %d\n", notifyValue);
      if (notifyValue > 0) {
          isNotify = prvTaskFuncProcess(&event, &msg);
      }
    } else {
        isNotify = prvTaskFuncProcess(&event, &msg);
    }
  }
}
/*************************************外部函数**********************************/
/**
 * @description: MsgProcessTask init
 * @param {type} 
 * @return: 
 */
void MsgProcessTask_init(void)
{
  //创建队列
  LOOP_QUEUE_INIT(mPEvent);
  LOOP_QUEUE_INIT(order);

  // protocol preocess init

  EventBus_regListerner(prvEventListenerCB);
  MsgBus_regListerner(prvMsgListenerCB);

  // create emegency task
  xTaskCreate((TaskFunction_t)prvTaskFuncCB,  //任务函数
              (const char *)"MsgProcess Task",        //任务名称
              (uint16_t)MsgProcessTaskSTK_SIZE,       //任务堆栈大小
              (void *)NULL,                    //传递给任务函数的参数
              (UBaseType_t)MsgProcessTaskPRO,  //任务优先级
              (TaskHandle_t *)&s_msgProcessTaskHandle);  //任务句柄
  if (s_msgProcessTaskHandle == NULL) printf("MsgProcess Task cerate fail\n");

  // s_countDownSwtimer =
  //     zOSTimerCreate("swtimer", boardCountDown_PERIOUS, (UBaseType_t)pdTRUE,
  //                    (void *)1, prvCountDownSwtimerCB);
  // if (s_countDownSwtimer) xTimerStart(s_countDownSwtimer, 0);
}

