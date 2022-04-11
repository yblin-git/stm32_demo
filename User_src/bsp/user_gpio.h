/*
 * @Author: your name
 * @Date: 2020-04-29 21:58:09
 * @LastEditTime: 2020-06-02 11:26:45
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \MobileWrappingMachine_V3\Src\bsp_task.h
 */
#pragma once
#include "main.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#include "system.h"

/* 200MS循环一次*/
#define KEY_SCAN(name, st, type)                                            \
  if (GPIO_READ(name) != KEY_STATE(name, st)) {                             \
    if ((KEY_TIME(name, st) != 0) &&                                        \
        (KEY_TIME(name, st) % 50 == 0 || KEY_TIME(name, st) == 1)) {        \
      keyScanEventSendToBus(type, kPress, KEY_TIME(name, st) / 50);         \
    }                                                                       \
    if (KEY_TIME(name, st) * boardKEYSCAN_PERIODUS <= boardKEYSCAN_MAXTIME) \
      KEY_TIME(name, st)++;                                                 \
  } else {                                                                  \
    if (KEY_TIME(name, st) != 0) {                                          \
      keyScanEventSendToBus(type, kRelease, KEY_TIME(name, st) / 50);       \
      KEY_STATE_CLEAR(name, st, KEY_STATE(name, st));                       \
    }                                                                       \
  }

static inline void keyScanEventSendToBus(keyType_e keyType, keyEventType_e type,
                                         uint8_t seconds) {
  Event_t evt;
  evt.id = EventID_KeyScanEvent;
  KeyEvent_t iterm;
  iterm.key = keyType;
  iterm.type = type;
  iterm.seconds = seconds;
  Event_createKeyEvent(&evt, iterm);
  EventBus_fire(&evt);
  LOG_INFO("key1:%d type : %d seconds %d\n", iterm.key, iterm.type,
           iterm.seconds);
}
void UserGpio_airCompressorMachineControl(bool open);
void UserGpio_airCompressorMachine2Control(bool open);
void UserGpio_fanControl(bool open);
void UserGpio_runIndicatorControl(bool open);
void UserGpio_rfPowerControl(bool open);
void UserGpio_lcdPowerContorl(bool open);
void UserGpio_solenoidValveContorl(bool open);
void UserGpio_powerIndicatorControl(bool open);
