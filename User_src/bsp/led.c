/*
 * @Author: your name
 * @Date: 2020-05-09 17:49:21
 * @LastEditTime: 2020-06-06 12:20:39
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \MobileWrappingMachine_V3\Src\bsp\led.c
 */
#include "FreeRTOS.h"
#include "board.h"
#include "bsp\user_gpio.h"
#include "hw_helper\z_io.h"
#include "os_helper\swtimer.h"
#include "queue.h"
#include "stdlib.h"
#include "string.h"
#include "system.h"
#include "task.h"
#include "timers.h"
static TimerHandle_t s_ledTwinkleSwtimer = NULL;  //扫描按键
static void Led_TwinkleCallBack(xTimerHandle xTimer){
  
}
void Led_init() {
  s_ledTwinkleSwtimer =
      zOSTimerCreate("key sacn swtimer", boardKEYSCAN_PERIODUS,
                     (UBaseType_t)pdTRUE, (void *)1, Led_TwinkleCallBack);
}

void Led_showLed0(bool open){
  if (!open) {
    GPIO_WIRTE(LED0, GPIO_PIN_SET);
  } else
    GPIO_WIRTE(LED0, GPIO_PIN_RESET);
}
void Led_showLed1(bool open) {
  if (!open) {
    GPIO_WIRTE(LED1, GPIO_PIN_SET);
  } else
    GPIO_WIRTE(LED1, GPIO_PIN_RESET);
}
void Led_showLed2(bool open) {
  if (!open) {
    GPIO_WIRTE(LED2, GPIO_PIN_SET);
  } else
    GPIO_WIRTE(LED2, GPIO_PIN_RESET);
}
void Led_showLed3(bool open) {
  if (!open) {
    GPIO_WIRTE(LED3, GPIO_PIN_SET);
  } else
    GPIO_WIRTE(LED3, GPIO_PIN_RESET);
}
void Led_closeLed(){ 
    GPIO_WIRTE(LED0, GPIO_PIN_SET);
    GPIO_WIRTE(LED1, GPIO_PIN_SET);
    GPIO_WIRTE(LED2, GPIO_PIN_SET);
    GPIO_WIRTE(LED3, GPIO_PIN_SET);
}
void Led_showRGB_R(bool open) {
  if (!open) {
    GPIO_WIRTE(RGB_R, GPIO_PIN_SET);
  } else
    GPIO_WIRTE(RGB_R, GPIO_PIN_RESET);
}
void Led_showRGB_G(bool open) {
  if (!open) {
    GPIO_WIRTE(RGB_G, GPIO_PIN_SET);
  } else
    GPIO_WIRTE(RGB_G, GPIO_PIN_RESET);
}
void Led_showRGB_B(bool open) {
  if (!open) {
    GPIO_WIRTE(RGB_B, GPIO_PIN_SET);
  } else
    GPIO_WIRTE(RGB_B, GPIO_PIN_RESET);
}
void Led_closeRGB(){ 
    GPIO_WIRTE(RGB_R, GPIO_PIN_SET);
    GPIO_WIRTE(RGB_G, GPIO_PIN_SET);
    GPIO_WIRTE(RGB_B, GPIO_PIN_SET);
}
