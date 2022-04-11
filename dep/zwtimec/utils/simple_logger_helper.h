/*
 * @Author: your name
 * @Date: 2020-05-06 17:58:08
 * @LastEditTime: 2020-05-09 21:30:59
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \MobileWrappingMachine_V3\dep\ztimecpp\zwtimec\utils\simple_logger_helper.h
 */
#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "stdarg.h"
#include "stdlib.h"

#include "zwtimec/utils/compiler_config.h"

typedef enum {
  LOGLEVEL_ERROR = 4,
  LOGLEVEL_WARNING = 3,
  LOGLEVEL_INFO = 2,
  LOGLEVEL_DEBUG = 1,
  LOGLEVEL_TRACE = 0,
} SimpleLogger_logLevel_t;

void SimpleLogger_vlog(int32_t level, bool isISR, const char* format,
                       va_list ap);
static void INLINE SimpleLogger_log(int32_t level, bool isISR, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  SimpleLogger_vlog(level, isISR ,fmt, ap);
  va_end(ap);
};

#define simpleLogger_FORMATER_IMPL()                                    \
  void SimpleLogger_vlog(int32_t level, bool isISR, const char* format, \
                         va_list ap)

#define LOG_DEBUG(fmt, ...) \
  SimpleLogger_log(LOGLEVEL_DEBUG, true, fmt, ##__VA_ARGS__);
#define LOG_INFO(fmt, ...) \
  SimpleLogger_log(LOGLEVEL_INFO, false, fmt, ##__VA_ARGS__);
#define LOG_WARN(fmt, ...) \
  SimpleLogger_log(LOGLEVEL_WARNING, false, fmt, ##__VA_ARGS__);
#define LOG_ERROR(fmt, ...) \
  SimpleLogger_log(LOGLEVEL_ERROR, false, fmt, ##__VA_ARGS__);

#define LOG_DEBUG_ISR(fmt, ...) \
  SimpleLogger_log(LOGLEVEL_DEBUG, true, fmt, ##__VA_ARGS__);
#define LOG_INFO_ISR(fmt, ...) \
  SimpleLogger_log(LOGLEVEL_INFO, true, fmt, ##__VA_ARGS__);
#define LOG_WARN_ISR(fmt, ...) \
  SimpleLogger_log(LOGLEVEL_WARNING, true, fmt, ##__VA_ARGS__);
#define LOG_ERROR_ISR(fmt, ...) \
  SimpleLogger_log(LOGLEVEL_ERROR, true, fmt, ##__VA_ARGS__);

/*****/
