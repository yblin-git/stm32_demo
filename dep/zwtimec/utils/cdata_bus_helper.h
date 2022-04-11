#pragma once
#include <stdbool.h>
#include "zwtimec/utils/compiler_config.h"

/****************************工具宏***************************************/
/**
 * 声明一个Event
 * @Usage:
 *
 */

#define __cdataBusHelperENUM_ITERM_MARCO(className, itermName) \
  className##ID_##itermName,
#define __cdataBusHelperENUM_ITERM_TO_STR_MARCO(className, itermName) \
  case className##ID_##itermName:                                     \
    return #itermName;
#define __cdataBusHelperSTRUCT_ITERM_MARCO(className, itermName) \
  itermName##_t itermName;

/**
 * @brief 定义Event结构体,枚举,通用方法
 */
#define cdataBusHelperDECLEAR(className, list)                                                        \
  /*******************************自动生成EventId_t,eventIdToStr,Event_t************************/ \
  typedef enum {                                                                                      \
    list(className, __cdataBusHelperENUM_ITERM_MARCO)                                                 \
  } className##TypeId_t;                                                                              \
  /**                                                                                                 \
   * @brief eventIdToStr                                                                              \
   *                                                                                                  \
   * @param id                                                                                        \
   * @return const char*                                                                              \
   */                                                                                                 \
  static inline const char *className##_typeIdToStr(className##TypeId_t id) {                         \
    switch (id) { list(className, __cdataBusHelperENUM_ITERM_TO_STR_MARCO); }                         \
    return "unkownEvent";                                                                             \
  };                                                                                                  \
  /**                                                                                                 \
   * @description: 监听者typedef                                                                   \
   * @param inTaskContext                                                                             \
   *              true 表示task上下文                                                            \
   *              false 中断上下文                                                               \
   * @return:                                                                                         \
   */                                                                                                 \
  typedef void (*className##Listerner_t)(className##TypeId_t index,                                   \
                                         uint8_t * data, size_t size,                                 \
                                         bool inTaskContext);                                         \
                                                                                                      \
  /**                                                                                                 \
   * @description: 事件总线初始化                                                              \
   * @param {type}                                                                                    \
   * @return:                                                                                         \
   */                                                                                                 \
  void className##DataBus_init(void);                                                                 \
                                                                                                      \
  /**                                                                                                 \
   * @description: 监听者注册                                                                    \
   * @param {type}                                                                                    \
   * @return:                                                                                         \
   *          true sucess                                                                             \
   *          false fail                                                                              \
   *              监听表已满                                                                     \
   */                                                                                                 \
  bool className##DataBus_regListerner(className##Listerner_t listerner);                             \
                                                                                                      \
  /**                                                                                                 \
   * @description: 发送事件给监听者                                                           \
   * @param inTaskContext                                                                             \
   *              true 表示task上下文                                                            \
   *              false 中断上下文                                                               \
   * @return:                                                                                         \
   */                                                                                                 \
  void className##DataBus_fire(className##TypeId_t index, uint8_t *data,                              \
                               size_t size);                                                          \
  void className##DataBus_fireISR(className##TypeId_t index, uint8_t *data,                           \
                                  size_t size);

#define cdataBusHelperIMPLE(className, maxListenerSize)                     \
  typedef struct {                                                          \
    className##Listerner_t listerner[maxListenerSize];                      \
    uint8_t listernerIndex;                                                 \
  } className##DataBus_t;                                                   \
                                                                            \
  static className##DataBus_t s_##className##Bus;                           \
                                                                            \
  /**                                                                       \
   * @description: 事件总线初始化                                    \
   * @param {type}                                                          \
   * @return:                                                               \
   */                                                                       \
  void className##DataBus_init(void) {                                      \
    memset(&s_##className##Bus, 0, sizeof(className##DataBus_t));           \
  }                                                                         \
                                                                            \
  /**                                                                       \
   * @description: 监听者注册                                          \
   * @param {type}                                                          \
   * @return:                                                               \
   *          true sucess                                                   \
   *          false fail                                                    \
   *              监听表已满                                           \
   */                                                                       \
  bool className##DataBus_regListerner(className##Listerner_t listerner) {  \
    if (s_##className##Bus.listernerIndex >= maxListenerSize) return false; \
                                                                            \
    s_##className##Bus.listerner[s_##className##Bus.listernerIndex++] =     \
        listerner;                                                          \
                                                                            \
    return true;                                                            \
  }                                                                         \
                                                                            \
  /**                                                                       \
   * @description: 发送事件给监听者                                 \
   * @param context                                                         \
   *              true 表示task上下文                                  \
   *              false 中断上下文                                     \
   * @return:                                                               \
   */                                                                       \
  void className##DataBus_fire(className##TypeId_t index, uint8_t *data,    \
                               size_t size) {                               \
    for (int i = 0; i < s_##className##Bus.listernerIndex; i++) {           \
      s_##className##Bus.listerner[i](index, data, size, true);             \
    }                                                                       \
  }                                                                         \
  void className##DataBus_fireISR(className##TypeId_t index, uint8_t *data, \
                                  size_t size) {                            \
    for (int i = 0; i < s_##className##Bus.listernerIndex; i++) {           \
      s_##className##Bus.listerner[i](index, data, size, false);            \
    }                                                                       \
  }
