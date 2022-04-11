/*
 * @Author: your name
 * @Date: 2020-05-01 16:25:15
 * @LastEditTime: 2020-06-02 10:21:01
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \xl_analysis\user_src\bsp\flash_oprate.h
 */
#pragma once
#include "stdint.h"
#include "main.h"
#include "system.h"
#define FLASH_SAVE_ADDR 0X0800FC00

// 32机中flash一次读写是32bit,这里按照4位对齐，flash读写更自然
#pragma pack(4)
typedef struct {
  uint32_t markbegin;
  Config_t config;
  uint32_t markend;
  // false，表示flash中的数据没有被初始化
  //每次重新烧录后，只保证如果
  bool initialized;  
} FlashData_t;
#pragma pack()

void Flash_read();
FlashData_t* Flash_getPointer();
void Flash_flush();
