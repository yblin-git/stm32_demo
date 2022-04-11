/*
 * @Author: your name
 * @Date: 2020-04-29 21:58:09
 * @LastEditTime: 2020-06-06 12:28:56
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \MobileWrappingMachine_V3\Src\bsp_task.h
 */
#pragma once
#include "main.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"

/**
 * @description: bspTask init
 * @param {type}
 * @return:
 */
void
BspTask_init(void);
void BspTask_startInputCapture();
/**
 * @description: 开机
 * @param {type}
 * @return:
 */
void BspTask_powerOn() ;
/**
 * @description: 关机
 * @param {type}
 * @return:
 */
void BspTask_shutdown() ;
void BspTask_openFan();
void BspTask_closeFan();
void BspTask_openOzoneHightLevel();
void BspTask_openOzoneMiddleLevel();
void BspTask_openOzoneLowLevelt();
void BspTask_closeOzone();
void BspTask_startTimer(uint32_t periodMs);
void BspTask_stopTimer();
void BspTask_showTwoHourLed();
void BspTask_showFourHourLed();
void BspTask_showSixHourLed();
void BspTask_showEightHourLed();