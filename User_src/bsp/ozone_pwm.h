/*
 * @Author: your name
 * @Date: 2020-06-02 16:38:31
 * @LastEditTime: 2020-06-06 12:27:28
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \OzoneGenerator\User_src\bsp\ozone_pwm.c
 */
#pragma once
#include "main.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#include "system.h"
void Pwn_setCompare(uint8_t Duty);
void Pwn_setFrequency(uint32_t frequency);
void Ozone_Start() ;
void Ozone_Stop() ;
void Ozone_setLevel(uint8_t level);
