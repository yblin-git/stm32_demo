#pragma once
/*
 * @Author: your name
 * @Date: 2020-05-29 15:44:01
 * @LastEditTime: 2020-05-29 21:56:41
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath:
 * \MobileWrappingMachine_drive_V3\dep\ztimecpp\zwtimec\utils\cstring_helper.h
 */
#include <stdint.h>
#include "stdio.h"
#include "string.h"
#include "zwtimec/utils/compiler_config.h"
/**
 * @description: 实现以separator字符串为分割单元分割src字符串
 * @param src 原字符串
 *
 * @param separator 分离器
 * 比如，以一个逗号分割：","
 *
 * @param dest 存放结果
 *
 * @return: 分割出串的个数
 * 小于0 表示失败
 */
static INLINE int StrHelper_strSplit(char *src, const char *separator,
                                     char **dest) {
  int cnt = 0;
  char *pNext;

  if (src == NULL || strlen(src) == 0) return -1;
  if (separator == NULL || strlen(separator) == 0) return -1;

  pNext = (char *)strtok(src, separator);
  while (pNext != NULL) {
    *dest++ = pNext;
    cnt++;
    pNext = (char *)strtok(NULL, separator);
  }

  return cnt;
}

static INLINE int StrHelper_strSplitSafe(char *src, const char *separator,
                                         char **dest, int destMaxNum) {
  int cnt = 0;
  char *pNext;

  if (src == NULL || strlen(src) == 0) return -1;
  if (separator == NULL || strlen(separator) == 0) return -1;

  pNext = (char *)strtok(src, separator);
  while (pNext != NULL) {
    if (cnt + 1 > destMaxNum) {
      break;
    }
    *dest++ = pNext;
    cnt++;
    pNext = (char *)strtok(NULL, separator);
  }
  return cnt;
}

static inline int StrHelper_tolower(int c) {
  if (c >= 'A' && c <= 'Z') {
    return c + 'a' - 'A';
  } else {
    return c;
  }
}

static INLINE uint32_t StrHelper_htoi(const char *s) {
  int i;
  uint32_t n = 0;
  if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
    i = 2;
  } else {
    i = 0;
  }
  for (; (s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') ||
         (s[i] >= 'A' && s[i] <= 'Z');
       ++i) {
    if (StrHelper_tolower(s[i]) > '9') {
      n = 16 * n + (10 + StrHelper_tolower(s[i]) - 'a');
    } else {
      n = 16 * n + (StrHelper_tolower(s[i]) - '0');
    }
  }
  return n;
}