/*
 * @Author: your name
 * @Date: 2020-05-01 16:25:25
 * @LastEditTime: 2020-05-10 10:20:42
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \xl_analysis\user_src\bsp\flash_operate.c
 */
#include "flash_operate.h"
#include "stmflash.h"

static FlashData_t s_flashData;

// const static uint8_t s_buf[sizeof(s_flashData) + 2 * FLASH_PAGE_SIZE];
static uint32_t getDataBufAlignAddress() {
  // return (uint32_t)(s_buf) / FLASH_PAGE_SIZE * FLASH_PAGE_SIZE;
  return FLASH_SAVE_ADDR;
}
#define BADCODE ((uint32_t)0xbadc0de)

void Flash_read() {
  STMFLASH_Read(getDataBufAlignAddress(), (uint16_t*)(&s_flashData),
                sizeof(FlashData_t) / sizeof(uint16_t));
  if (BADCODE == s_flashData.markbegin && BADCODE == s_flashData.markend) {
    s_flashData.initialized = true;
  } else {
    s_flashData.initialized = false;
  }
}
FlashData_t* Flash_getPointer() { return &s_flashData; }
void Flash_flush() {
  s_flashData.markbegin = BADCODE;
  s_flashData.markend = BADCODE;
  s_flashData.initialized = true;
  STMFLASH_Write(getDataBufAlignAddress(), (uint16_t*)&s_flashData,
                 sizeof(FlashData_t) / sizeof(uint16_t));
}
