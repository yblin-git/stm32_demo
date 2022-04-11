#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "stdarg.h"
#include "stdlib.h"
#include "zwtimec/utils/compiler_config.h"
#include "zwtimec/utils/cstring_helper.h"
#include "zwtimec/utils/marco_utils.h"
#include "zwtimec/utils/marcos.h"
#include "zwtimec/utils/str_simple_receiver.h"

#define __cmdMenuEACH_CMD_REG(structName, v2, v3, v4, v5, index, varName) \
  __##structName##_regCmd(&__s_##varName##descriptor);

#define cmdMenuDECLEAR(structName, maxCmdNum, endMark)                        \
  typedef bool (*CmdFunction_t)(const char *argv[], int argc);                \
  typedef struct {                                                            \
    const char *name;                                                         \
    const char *usage;                                                        \
    CmdFunction_t cmdFunction;                                                \
  } CmdDescriptor_t;                                                          \
  typedef struct {                                                            \
    const CmdDescriptor_t *descriptors[maxCmdNum];                            \
    int usefullOff;                                                           \
    char strParserBuf[256];                                                   \
    StrSimpleRecv_t simpleRecv;                                               \
  } CmdDescriptorTable_t;                                                     \
  void structName##_init();                                                   \
  void __##structName##_regCmd(const CmdDescriptor_t *cmd);                   \
  void __##structName##_printf(const char *fmt, ...);                         \
  static INLINE const char *__##structName##_getEndMark() { return endMark; } \
  void structName##_processReceive(const char *data, size_t size);

#define cmdMenuIMPL(structName)                                               \
  static CmdDescriptorTable_t __s_cmddescriptortable;                         \
  static bool __helpCmd(const char *argv[], int argc) {                       \
    for (int i = 0; i < __s_cmddescriptortable.usefullOff; ++i) {             \
      __##structName##_printf("-> %s\n",                                    \
                              __s_cmddescriptortable.descriptors[i]->name);   \
      __##structName##_printf("-> \t%s\n",                                  \
                              __s_cmddescriptortable.descriptors[i]->usage);  \
    }                                                                         \
    return true;                                                              \
  }                                                                           \
  static CmdDescriptor_t __s_helpCmdDescriptor = {                            \
      .name = "help",                                                         \
      .usage = "help",                                                        \
      .cmdFunction = __helpCmd,                                               \
  };                                                                          \
  void structName##_init() {                                                  \
    memset(&__s_cmddescriptortable, 0, sizeof(CmdDescriptorTable_t));         \
    StrSimpleRecv_init(&__s_cmddescriptortable.simpleRecv,                    \
                       __s_cmddescriptortable.strParserBuf,                   \
                       ARRARY_SIZE(__s_cmddescriptortable.strParserBuf),      \
                       __##structName##_getEndMark());                        \
                                                                              \
    __##structName##_regCmd(&__s_helpCmdDescriptor);                          \
  };                                                                          \
  void __##structName##_regCmd(const CmdDescriptor_t *cmd) {                  \
    __s_cmddescriptortable.descriptors[__s_cmddescriptortable.usefullOff++] = \
        cmd;                                                                  \
  }                                                                           \
  void structName##_processReceive(const char *data, size_t size) {           \
    char *argv[8];                                                            \
    int argc = 0;                                                             \
    bool ready = StrSimpleRecv_pushData(&__s_cmddescriptortable.simpleRecv,   \
                                        data, size);                          \
    if (!ready) return;                                                       \
    do {                                                                      \
      char *oneLine =                                                         \
          StrSimpleRecv_tryGetRecvStr(&__s_cmddescriptortable.simpleRecv);    \
      if (oneLine == NULL) {                                                  \
        break;                                                                \
      }                                                                       \
      __##structName##_printf("<-%s\n", oneLine);                             \
      int argc = StrHelper_strSplit(oneLine, " ", argv);                      \
      if (argc <= 0) {                                                        \
        __##structName##_printf("->input str is wrong\n");                    \
        break;                                                                \
      }                                                                       \
      bool exeFlag = false;                                                   \
      for (int i = 0; i < __s_cmddescriptortable.usefullOff; i++) {           \
        if (strcmp(argv[0], __s_cmddescriptortable.descriptors[i]->name) ==   \
            0) {                                                              \
          if (__s_cmddescriptortable.descriptors[i]->cmdFunction) {           \
            bool exce_rec =                                                   \
                __s_cmddescriptortable.descriptors[i]->cmdFunction(           \
                    (const char **)argv, argc);                               \
            if (!exce_rec) {                                                  \
              __##structName##_printf("->exce fail\n");                       \
            }                                                                 \
          } else {                                                            \
            __##structName##_printf("->cmdFunction is null!\n");              \
          }                                                                   \
          exeFlag = true;                                                     \
          break;                                                              \
        }                                                                     \
      }                                                                       \
      if (!exeFlag) {                                                         \
        __##structName##_printf("->unknow cmd!\n");                           \
      }                                                                       \
    } while (false);                                                          \
    StrSimpleRecv_freeRecvStr(&__s_cmddescriptortable.simpleRecv);            \
  }

#define cmdMenuDEFINE_PRINTF(structName, impl) \
  void __##structName##_printf(const char *fmt, ...) { impl }

#define cmdMenuDEFINE_CMD(_name, _usage, expectArgc, impl)   \
  static bool __##_name##CMD(const char *argv[], int argc) { \
    if (expectArgc == argc) {                                \
      impl;                                                  \
    }                                                        \
    return false;                                            \
  }                                                          \
  static CmdDescriptor_t __s_##_name##descriptor = {         \
      .name = #_name,                                        \
      .usage = _usage,                                       \
      .cmdFunction = __##_name##CMD,                         \
  };

#define cmdMenuDEFINE_ENABLE_CMDS_FUNC(structName, ...)           \
  static void structName##_enableCmdSInThisModle() {              \
    MARCO_LIST(__cmdMenuEACH_CMD_REG, structName, v2, v3, v4, v5, \
               __VA_ARGS__);                                      \
  }

#define cmdMenuDECLEAR_ENABLE_CMDS_FUNC(structName) \
  static INLINE void structName##_enableCmdSInThisModle();

// #define cmdMenuDEFINE_CMD(name,usage,impl)
