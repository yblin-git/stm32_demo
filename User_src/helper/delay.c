/*
 * @Author: your name
 * @Date: 2020-05-12 09:46:59
 * @LastEditTime: 2020-05-13 17:18:59
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \MobileWrappingMachine_drive_V3\Src\helper\delay.c
 */
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////
//********************************************************************************
//修改说明
//无
//////////////////////////////////////////////////////////////////////////////////
// us级延时时,关闭任务调度(防止打断us级延迟)
void delay_osschedlock(void) {
  
}

// us级延时时,恢复任务调度
void delay_osschedunlock(void) {
}

void delay_us(uint32_t us) {
  __IO uint32_t currentTicks = SysTick->VAL;
  /* Number of ticks per millisecond */
  const uint32_t tickPerMs = SysTick->LOAD + 1;
  /* Number of ticks to count */
  const uint32_t nbTicks = ((us - ((us > 0) ? 1 : 0)) * tickPerMs) / 1000;
  /* Number of elapsed ticks */
  uint32_t elapsedTicks = 0;
  __IO uint32_t oldTicks = currentTicks;
  delay_osschedlock();  //阻止OS调度，防止打断us延时

  do {
    currentTicks = SysTick->VAL;
    elapsedTicks += (oldTicks < currentTicks)
                        ? tickPerMs + oldTicks - currentTicks
                        : oldTicks - currentTicks;
    oldTicks = currentTicks;
  } while (nbTicks > elapsedTicks);
  delay_osschedunlock();  //恢复OS调度
}