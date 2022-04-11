/*
 * @Author: your name
 * @Date: 2020-04-29 21:57:58
 * @LastEditTime: 2020-05-27 20:13:33
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \MobileWrappingMachine_V3\Src\bsp_task.c
 */
#include "user_gpio.h"
#include "hw_helper/z_io.h"

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
        // EXTI->IMR &= ~(GPIO_Pin); //屏蔽外部中断4
        // EXTI->EMR &= ~(GPIO_Pin); //屏蔽外部中断4
        // EXTI->IMR != GPIO_Pin;    //使能外部中断4
        // EXTI->EMR != GPIO_Pin;    //使能外部中断4
}
void UserGpio_airCompressorMachineControl(bool open) {
  if (open) {
    GPIO_WIRTE(RELAY_CTRL, GPIO_PIN_SET);
  } else
    GPIO_WIRTE(RELAY_CTRL, GPIO_PIN_RESET);
}
void UserGpio_airCompressorMachine2Control(bool open) {
  if (open) {
    GPIO_WIRTE(RELAY2_CTRL, GPIO_PIN_SET);
  } else
    GPIO_WIRTE(RELAY2_CTRL, GPIO_PIN_RESET);
}

void UserGpio_fanControl(bool open){
  if (open) {
    GPIO_WIRTE(MOTOR_PWR, GPIO_PIN_SET);
  } else
    GPIO_WIRTE(MOTOR_PWR, GPIO_PIN_RESET);
}
void UserGpio_runIndicatorControl(bool open) {
  if (open) {
    GPIO_WIRTE(LED1, GPIO_PIN_SET);
  } else
    GPIO_WIRTE(LED1, GPIO_PIN_RESET);
}
void UserGpio_rfPowerControl(bool open){
  if (open) {
    GPIO_WIRTE(RF_PWR, GPIO_PIN_RESET);
  } else
    GPIO_WIRTE(RF_PWR, GPIO_PIN_SET);
}
void UserGpio_lcdPowerContorl(bool open) {
  if (open) {
    GPIO_WIRTE(LCD_PEW, GPIO_PIN_SET);
  } else
    GPIO_WIRTE(LCD_PEW, GPIO_PIN_RESET);
}
void UserGpio_solenoidValveContorl(bool open) {
  if (open) {
    GPIO_WIRTE(DCF_PWR, GPIO_PIN_SET);
  } else
    GPIO_WIRTE(DCF_PWR, GPIO_PIN_RESET);
}
void UserGpio_powerIndicatorControl(bool open) {
  if (open) {
    GPIO_WIRTE(LED2, GPIO_PIN_SET);
  } else
    GPIO_WIRTE(LED2, GPIO_PIN_RESET);
}
