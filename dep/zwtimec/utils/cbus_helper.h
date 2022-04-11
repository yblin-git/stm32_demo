#pragma once
#include <stdbool.h>
#include "zwtimec/utils/compiler_config.h"

/****************************工具宏***************************************/
/**
 * 声明一个Event
 * @Usage:
 *
 */

#define __cbusHelperENUM_ITERM_MARCO(className, itermName) \
  className##ID_##itermName,
#define __cbusHelperENUM_ITERM_TO_STR_MARCO(className, itermName) \
  case className##ID_##itermName:                             \
    return #itermName;
#define __cbusHelperSTRUCT_ITERM_MARCO(className, itermName) \
  itermName##_t itermName;

#define __cbusHelperCreateEvent(className, itermName)

#define cbusDEFINE_ITERM_CREATE_FUNC(className, itermName)   \
  static INLINE className##_t className##_create##itermName( \
      itermName##_t iterm) {                                 \
    className##_t var;                                       \
    var.id = className##ID_##itermName;                      \
    var.d.itermName = iterm;                                 \
    return var;                                              \
  }

#define cbusDEFINE_ITERM_CREATE_FUNC_VERSION2(className, itermName)  \
  static INLINE void className##_create##itermName(className##_t *var,    \
                                              itermName##_t iterm) { \
    var->id = className##ID_##itermName;                             \
    var->d.itermName = iterm;                                        \
  }

/**
 * @brief 定义Event结构体,枚举,通用方法
 */
#define cbusHelperDECLEAR(className, list)                                                            \
  /*******************************自动生成EventId_t,eventIdToStr,Event_t************************/ \
  typedef enum {                                                                                      \
    list(className, __cbusHelperENUM_ITERM_MARCO)                                                     \
  } className##TypeId_t;                                                                              \
  /**                                                                                                 \
   * @brief eventIdToStr                                                                              \
   *                                                                                                  \
   * @param id                                                                                        \
   * @return const char*                                                                              \
   */                                                                                                 \
  static inline const char *className##_typeIdToStr(className##TypeId_t id) {                         \
    switch (id) { list(className, __cbusHelperENUM_ITERM_TO_STR_MARCO); }                             \
    return "unkownEvent";                                                                             \
  };                                                                                                  \
  /**                                                                                                 \
   * @brief EventStruct                                                                               \
   */                                                                                                 \
  typedef union {                                                                                     \
    uint8_t pad;                                                                                      \
    list(className, __cbusHelperSTRUCT_ITERM_MARCO);                                                  \
  } className##Union_t;                                                                               \
  typedef struct {                                                                                    \
    className##TypeId_t id;                                                                           \
    className##Union_t d;                                                                             \
  } className##_t;                                                                                    \
  /**                                                                                                 \
   * @description: 监听者typedef                                                                   \
   * @param inTaskContext                                                                             \
   *              true 表示task上下文                                                            \
   *              false 中断上下文                                                               \
   * @return:                                                                                         \
   */                                                                                                 \
  typedef void (*className##Listerner_t)(className##_t * evt,                                         \
                                         bool inTaskContext);                                         \
                                                                                                      \
  /**                                                                                                 \
   * @description: 事件总线初始化                                                              \
   * @param {type}                                                                                    \
   * @return:                                                                                         \
   */                                                                                                 \
  void className##Bus_init(void);                                                                     \
                                                                                                      \
  /**                                                                                                 \
   * @description: 监听者注册                                                                    \
   * @param {type}                                                                                    \
   * @return:                                                                                         \
   *          true sucess                                                                             \
   *          false fail                                                                              \
   *              监听表已满                                                                     \
   */                                                                                                 \
  bool className##Bus_regListerner(className##Listerner_t listerner);                                 \
                                                                                                      \
  /**                                                                                                 \
   * @description: 发送事件给监听者                                                           \
   * @param inTaskContext                                                                             \
   *              true 表示task上下文                                                            \
   *              false 中断上下文                                                               \
   * @return:                                                                                         \
   */                                                                                                 \
  void className##Bus_fire(className##_t *evt);                                                       \
  void className##Bus_fireISR(className##_t *evt);                                                    \
  list(className, __cbusHelperCreateEvent);

#define cbusHelperIMPLE(className, maxListenerSize)                         \
  typedef struct {                                                          \
    className##Listerner_t listerner[maxListenerSize];                      \
    uint8_t listernerIndex;                                                 \
  } className##Bus_t;                                                       \
                                                                            \
  static className##Bus_t s_##className##Bus;                               \
                                                                            \
  /**                                                                       \
   * @description: 事件总线初始化                                    \
   * @param {type}                                                          \
   * @return:                                                               \
   */                                                                       \
  void className##Bus_init(void) {                                          \
    memset(&s_##className##Bus, 0, sizeof(className##Bus_t));               \
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
  bool className##Bus_regListerner(className##Listerner_t listerner) {      \
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
  void className##Bus_fire(className##_t *evt) {                            \
    for (int i = 0; i < s_##className##Bus.listernerIndex; i++) {           \
      s_##className##Bus.listerner[i](evt, true);                           \
    }                                                                       \
  }                                                                         \
  void className##Bus_fireISR(className##_t *evt) {                         \
    for (int i = 0; i < s_##className##Bus.listernerIndex; i++) {           \
      s_##className##Bus.listerner[i](evt, false);                          \
    }                                                                       \
  }
