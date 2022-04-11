/*
 * @Author: your name
 * @Date: 2020-05-10 15:49:26
 * @LastEditTime: 2020-05-25 09:12:09
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \MobileWrappingMachine_V3\Src\bsp\uart.h
 */
#pragma once
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "system.h"
#include "main.h"

void Uart_enable() ;

bool Uart_isSending() ;
void Uart_asynSendMsg(UART_HandleTypeDef *huart, uint8_t *pData,
                      uint16_t Size) ;
void Uart_reEnableTouchScreenUart();
