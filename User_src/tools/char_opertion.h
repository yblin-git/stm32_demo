/*
 * @Author: your name
 * @Date: 2020-05-10 12:04:37
 * @LastEditTime: 2020-05-15 16:11:39
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \MobileWrappingMachine_V3\Src\tools\char_opertion.h
 */
#pragma once
#include "system.h"

static inline int tolower(int c) {
  if (c >= 'A' && c <= 'Z') {
    return c + 'a' - 'A';
  } else {
    return c;
  }
}
//将十六进制的字符串转换成整数
static inline int htoi(char s[]) {
  int i;
  int n = 0;
  if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
    i = 2;
  } else {
    i = 0;
  }
  for (; (s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') ||
         (s[i] >= 'A' && s[i] <= 'Z');
       ++i) {
    if (tolower(s[i]) > '9') {
      n = 16 * n + (10 + tolower(s[i]) - 'a');
    } else {
      n = 16 * n + (tolower(s[i]) - '0');
    }
  }
  return n;
}
static inline void stringToBin(char s[]) {
  uint8_t receiveData[64];
  char arg1[1] = {0};
  char arg2[1] = {0};
  for (size_t i = 0; i < strlen(s) / 2; i++) {
    arg1[0] = s[i * 2];
    arg2[0] = s[i * 2 + 1];
    LOG_DEBUG("arg:%d %d\n", htoi(arg1), htoi(arg2));
    uint8_t data = (htoi(arg1) << 4) + htoi(arg2);
    LOG_DEBUG("data:%x\n", data);
    receiveData[i] = data;
  }
  Msg_t msg;
  memset(&msg, 0, sizeof(Msg_t));
  Msg_createUart2ReceiveData(&msg, receiveData, strlen(s) / 2);
  MsgBus_fire(&msg);
}