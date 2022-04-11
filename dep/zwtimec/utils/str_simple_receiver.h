#pragma once
#include <string.h>
#include "stdbool.h"
#include "zwtimec/utils/compiler_config.h"

typedef struct {
  char *strBuf;
  int strBufSize;
  int hasRecvd;
  const char *endMark;
  int endMarkSize;
  bool ready;
} StrSimpleRecv_t;

#define strSimpleReceiverDEFINE(name, size) \
  static uint8_t s_##name##buf[size];       \
  static StrSimpleRecv_t s_##name;

#define strSimpleReceiverINIT(name) \
  StrSimpleRecv_init(&s_##name, s_##name##buf, ARRARY_SIZE(s_##name##buf));

/**
 * @description:
 * @param buf 字符串接收缓冲区
 *
 * @param len 缓冲区的大小
 *
 * @param endMark 一包数据的结束标志, 比如"\0", "\n", "\r\n"
 *
 * @return:
 */
static INLINE void StrSimpleRecv_init(StrSimpleRecv_t *receiver, char *buf,
                                      int len, const char *endMark) {
  memset(receiver, 0, sizeof(StrSimpleRecv_t));
  receiver->strBuf = buf;
  receiver->strBufSize = len;
  receiver->endMark = endMark;
  receiver->endMarkSize = strlen(endMark);
}

/**
 * @description: 向缓冲区中推数据
 * @param {type}
 * @return:
 *          true 表示当前缓冲区中有一包完成的数据
 *          false 表示不处理
 */
static INLINE bool StrSimpleRecv_pushData(StrSimpleRecv_t *handle, const char *pData,
                                          int size) {
  // 如果当前的字符串还没有被处理直接丢新到的字符
  if (handle->ready) {
    return false;
  }

  // 如果接收的字符超过限制, 则截断字符
  int cpySize = size;
  if (size + handle->hasRecvd + 1 /*存放\0*/ > handle->strBufSize)
    cpySize = handle->strBufSize - 1 - handle->hasRecvd;

  memcpy(&handle->strBuf[handle->hasRecvd], pData, cpySize);
  handle->hasRecvd += cpySize;

  int endMarkBegin = handle->hasRecvd - handle->endMarkSize;

  for (int i = endMarkBegin; i + handle->endMarkSize <= handle->hasRecvd; i++) {
    if (handle->endMarkSize == 0) {
      if (handle->strBuf[i] == '\0') {
        handle->strBuf[i] = '\0';
        handle->ready = true;
        return true;
      }
    } else {
      if (strncmp((char *)&handle->strBuf[i], handle->endMark,
                  handle->endMarkSize) == 0) {
        handle->strBuf[i] = '\0';
        handle->ready = true;
        return true;
      }
    }
  }

  // 如果当前的buffer已满,则处理buffer
  if (handle->hasRecvd + 1 >= handle->strBufSize) {
    handle->strBuf[handle->strBufSize - 1] = '\0';
    handle->ready = true;
    return true;
  }

  return false;
}

/**
 * @description: 获取当前的字符串
 * @param {type}
 * @return:
 */
static INLINE char *StrSimpleRecv_tryGetRecvStr(StrSimpleRecv_t *handle) {
  if (handle->ready) {
    return handle->strBuf;
  }
  return NULL;
}

/**
 * @description: 释放当前的缓冲区
 * @param {type}
 * @return:
 */
static INLINE void StrSimpleRecv_freeRecvStr(StrSimpleRecv_t *handle) {
  if (handle->ready) {
    handle->hasRecvd = 0;
    handle->ready = false;
  }
}
