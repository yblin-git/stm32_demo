#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "zwtimec/utils/compiler_config.h"
/**
 * @brief
 *
 * @return int
 * 1 大端
 * 0 小端
 */
static INLINE bool checkCPUEndian() {
  union {
    unsigned long int i;
    unsigned char s[4];
  } c;

  c.i = 0x12345678;
  return (0x12 == c.s[0]);
}

static INLINE void transNet(uint8_t* data, size_t len) {
  // if (len < 0) return;
  if (checkCPUEndian()) {
    return;
  }

  size_t off = len / 2;
  for (size_t i = 0; i < off; ++i) {
    uint8_t tmp = data[i];
    data[i] = data[len -1- i];
    data[len - 1 - i] = tmp;
  }
  return;
}

static INLINE void transFromNet(uint8_t* data, size_t len) {
  // if (len < 0) return;
  if (checkCPUEndian()) {
    return;
  }
  size_t off = len / 2;
  for (size_t i = 0; i < off; ++i) {
    uint8_t tmp = data[i];
    data[i] = data[len - 1-i];
    data[len - 1 - i] = tmp;
  }
  return;
}