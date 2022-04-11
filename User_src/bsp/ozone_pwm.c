/*
 * @Author: your name
 * @Date: 2020-06-02 16:38:31
 * @LastEditTime: 2020-06-06 12:27:21
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \OzoneGenerator\User_src\bsp\ozone_pwm.c
 */
#include "ozone_pwm.h"

#define TIME_PERIOD 1000
#define TIME_PRESCALER 71

#define OZONE_HIGHT 7500
#define OZONE_MIDDLE 5000
#define OZONE_LOW 2500
/*****************extern********************/
extern TIM_HandleTypeDef htim2;

static uint32_t s_frequency = TIME_PERIOD;

void Pwn_setCompare(uint8_t Duty) {
  uint32_t value = Duty * s_frequency / 100;
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, value);
}
void Pwn_setFrequency(uint32_t frequency) {
  HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
  s_frequency = 72 / (TIME_PRESCALER + 1) * 1000000 / frequency;
  uint32_t autoReload = __HAL_TIM_GET_AUTORELOAD(&htim2);
  uint32_t compare = __HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_2);
  uint32_t newCompare = s_frequency * compare / autoReload;
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, newCompare);
  __HAL_TIM_SET_AUTORELOAD(&htim2, s_frequency);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
}
void Ozone_Start(){ 
    
}
void Ozone_Stop(){ 
    
}
void Ozone_setLevel(uint8_t level){ 
    
}