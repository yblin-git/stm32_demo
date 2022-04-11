#include "os_helper/swtimer.h"

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
                             TimerCallbackFunction_t pxCallbackFunction) {
  TickType_t tickPeriod;
  tickPeriod = periodUs * 1.0 / (1.0 / configTICK_RATE_HZ * 1000000);
  return xTimerCreate(pcTimerName, (TickType_t)tickPeriod, uxAutoReload,
                      pvTimerID, pxCallbackFunction);
}

void zOSTimerChangePeriod(TimerHandle_t handle, int32_t period) {
  TickType_t tickPeriod;
  tickPeriod = period * 1.0 / (1.0 / configTICK_RATE_HZ * 1000000);
  xTimerChangePeriod(handle, tickPeriod, 0);
}
