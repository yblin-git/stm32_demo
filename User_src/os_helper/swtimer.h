#pragma once
#include "FreeRTOS.h"
#include "timers.h"

/**
 * @description: freeRTOS软件定时器重写
 *          freeRTOS软件定时器的定时周期是以tick为单位的
 *          现在改成以时间为单位
 *
 * @param {type}
 * @return:
 */
TimerHandle_t zOSTimerCreate(const char* const pcTimerName,
                             const uint32_t periodUs,  //单位us
                             const UBaseType_t uxAutoReload,
                             void* const pvTimerID,
                             TimerCallbackFunction_t pxCallbackFunction);

/**
 * @description: freeRTOS更改软件定时器周期
 * @param period 单位us
 * @return:
 */
void zOSTimerChangePeriod(TimerHandle_t handle, int32_t period);
