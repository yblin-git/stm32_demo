
#pragma once
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "zwtimec/utils/compiler_config.h"

/**
 * @brief
 *
 *
 *
 * @Usage:
 *  参考zwtimecpp/core/test/runner/demo/c_bean_helper_demo.cpp
 *
 * @API
 * cBeanHelperDECLEAR(className,list)
 * cBeanHelperIMPL(className,list)
 *
 * @声明定义后会自动生成下面方法和常量
 *
 * ClassName##_t 结构体定义
 * ClassName##IndexEnum_t 型枚举定义
 *    className##Index_##name 枚举成员名字定义
 *
 * 通用方法
 * 通过Index获得属性数值
 * ClassName##_getIterm
 * 通过IndexId获得数据类型ID
 * ClassName##_getItermTypeID
 * 通过Index获得设置属性的方法指针
 * ClassName##_getSetFuncByIndex
 * 通过Index获得获取属性的方法指针
 * ClassName##_getGetFuncByIndex
 * 通过Index设置属性
 * ClassName##_setByIndex
 * 通过Index读取属性
 * ClassName##_getByIndex
 * 通过Index设置属性,传入参数为字符串
 * ClassName##_setByIndexChar
 * 将Iterm转换成字符串
 * ClassName##_itermToStr
 * 将Index转换成字符串
 * ClassName##_indexToStr
 *
 * 属性操作方法
 * 获得属性
 * className##_get_##name
 * 属性转换型字符串
 * className##_##name##_ToStr
 * 设置属性
 * className##_set_##name
 *
 */

/**
 * @brief Utils
 */
static INLINE int cBeanHelperCompare_floatEq(float va1, float va2) {
  if (fabs(va1 - va2) <= 1e-6) {
    return 0;
  }
  if (va1 > va2) {
    return 1;
  } else {
    return -1;
  }
}

/**
 * @brief 基础数据结构定义,当需要添加新的数据结构时,需要一次提供下面几个宏的定义
 *
 * Iterm_t data 中添加新的数据类型
 * #define cBeanHelperTYPE_ID_XXXXX ()
 * #define cBeanHelperFormatID_XXXXX ()
 * #define cBeanHelperStrToValueF_XXXXX ()
 * cBeanHelperSUPPORT_TYPE_LIST 中添加新的数据类型
 *
 */

typedef struct {
  union {
    uint32_t _uint32_t;
    int32_t _int32_t;
    uint8_t _uint8_t;
    int8_t _int8_t;
    uint8_t _bool;
    float _float;
  } data;
} Iterm_t;

typedef bool (*beanUtilsCommonSetFunc_t)(Iterm_t val, bool inTaskContext);
typedef Iterm_t (*beanUtilsCommonGetFunc_t)();
#define cBeanHelperTYPE_ID_uint32_t (0)
#define cBeanHelperTYPE_ID_int32_t (1)
#define cBeanHelperTYPE_ID_uint8_t (2)
#define cBeanHelperTYPE_ID_int8_t (3)
#define cBeanHelperTYPE_ID_bool (4)
#define cBeanHelperTYPE_ID_float (5)
#define cBeanHelperTYPE_ID_errorType (-1)

#define cBeanHelperFormatID_uint32_t "%u"
#define cBeanHelperFormatID_int32_t "%d"
#define cBeanHelperFormatID_uint8_t "%u"
#define cBeanHelperFormatID_int8_t "%d"
#define cBeanHelperFormatID_bool "%d"
#define cBeanHelperFormatID_float "%f"

#define cBeanHelperStrToValueF_uint32_t atoi
#define cBeanHelperStrToValueF_int32_t atoi
#define cBeanHelperStrToValueF_uint8_t atoi
#define cBeanHelperStrToValueF_int8_t atoi
#define cBeanHelperStrToValueF_bool atoi
#define cBeanHelperStrToValueF_float atof

#define cBeanHelperSUPPORT_TYPE_LIST(maro) /**/ \
  maro(uint32_t)                           /**/ \
      maro(int32_t)                        /**/ \
      maro(uint8_t)                        /**/ \
      maro(int8_t)                         /**/ \
      maro(bool)                           /**/ \
      maro(float)

#define cBeanHelperITERM_CREATE_FUNC(type)                \
  static INLINE Iterm_t Iterm_create_##type(type value) { \
    Iterm_t iterm;                                        \
    iterm.data._##type = value;                           \
    return iterm;                                         \
  }
cBeanHelperSUPPORT_TYPE_LIST(cBeanHelperITERM_CREATE_FUNC);

static INLINE bool cBeanHelperCompare_uint32_t(uint32_t value, uint32_t min,
                                               uint32_t max) {
  return value >= min && value <= max;
}
static INLINE bool cBeanHelperCompare_int32_t(int32_t value, int32_t min,
                                              int32_t max) {
  return value >= min && value <= max;
}
static INLINE bool cBeanHelperCompare_uint8_t(uint8_t value, uint8_t min,
                                              uint8_t max) {
  return value >= min && value <= max;
}
static INLINE bool cBeanHelperCompare_int8_t(int8_t value, int8_t min,
                                             int8_t max) {
  return value >= min && value <= max;
}
static INLINE bool cBeanHelperCompare_bool(bool value, bool min, bool max) {
  return value >= min && value <= max;
}

static INLINE bool cBeanHelperCompare_float(float value, float min, float max) {
  if (cBeanHelperCompare_floatEq(value, min) >= 0 &&
      cBeanHelperCompare_floatEq(value, max) <= 0) {
    return true;
  }
  return false;
}

/**
 * @brief
 *
 */

#define cBeanHelperItermToStrCase(type)                                     \
  case cBeanHelperTYPE_ID_##type:                                           \
    snprintf(buf, bufsize, cBeanHelperFormatID_##type, value.data._##type); \
    return buf;

#define cBeanHelperCONFIG_STRUCT_MARCO(className, index, type, name, \
                                       defaultValue, ...)            \
  Iterm_t name;

#define cBeanHelperCONFIG_FUNCTION_DEFINE_MARCO(className, index, type, name, \
                                                defaultValue, min, max)       \
  static INLINE type className##_get_##name(void) {                           \
    return className##_get()->name.data._##type;                              \
  }                                                                           \
  static INLINE const char *className##_##name##_ToStr(                       \
      char *buf, size_t bufsize, Iterm_t iterm) {                             \
    snprintf(buf, bufsize, cBeanHelperFormatID_##type, iterm.data._##type);   \
    return buf;                                                               \
  }                                                                           \
  static INLINE bool className##_set_##name##2(type val, bool inTaskContext) {  \
    Iterm_t oldValue = className##_get()->name;                               \
    if (!cBeanHelperCompare_##type(val, min, max)) return false;              \
    className##_get()->name.data._##type = val;                               \
    className##ChangeListener_t listener = className##_getChangeListener();   \
    if (listener)                                                             \
      listener(className##Index_##name, oldValue, className##_get()->name,    \
               inTaskContext);                                                \
    return true;                                                              \
  }                                                                           \
  static INLINE bool className##_set_##name##ByIterm2(Iterm_t val,            \
                                                      bool inTaskContext) {   \
    return className##_set_##name##2(val.data._##type, inTaskContext);          \
  }                                                                           \
  static INLINE bool className##_set_##name(type val) {                       \
    return className##_set_##name##2(val, true);                                \
  }                                                                           \
  static INLINE bool className##_set_##name##ISR(type val) {                    \
    return className##_set_##name##2(val, false);                               \
  }                                                                           \
  static INLINE bool className##_set_##name##ByIterm(Iterm_t val) {           \
    return className##_set_##name##2(val.data._##type, true);                   \
  }                                                                           \
  static INLINE bool className##_set_##name##ByItermISR(Iterm_t val) {        \
    return className##_set_##name##2(val.data._##type, false);                  \
  }                                                                           \
  static INLINE Iterm_t className##_get_##name##Withterm(void) {              \
    Iterm_t iterm;                                                            \
    iterm.data._##type = className##_get_##name();                            \
    return iterm;                                                             \
  }

#define cBeanHelperCONFIG_INDEX_ENUM_MARCO(className, index, type, name, \
                                           defaultValue, ...)            \
  className##Index_##name = index,

#define cBeanHelperASSIGN_MARCO(className, index, type, name, defaultValue, \
                                ...)                                        \
  className##_get()->name.data._##type = defaultValue;

#define cBeanHelperCONFIG_GET_ITERM_SWITCH_CASE_MARCO(className, index, type,  \
                                                      name, defaultValue, ...) \
  case className##Index_##name:                                                \
    return &className##_get()->name;

#define cBeanHelperCONFIG_GET_ITERM_TYPE_ID_SWITCH_CASE_MARCO( \
    className, index, type, name, defaultValue, ...)           \
  case className##Index_##name:                                \
    return cBeanHelperTYPE_ID_##type;

#define cBeanHelperCONFIG_GET_ITERM_SET_FUNC_SWITCH_CASE_MARCO( \
    className, index, type, name, defaultValue, ...)            \
  case className##Index_##name:                                 \
    return className##_set_##name##ByIterm2;

#define cBeanHelperCONFIG_GET_ITERM_GET_FUNC_SWITCH_CASE_MARCO( \
    className, index, type, name, defaultValue, ...)            \
  case className##Index_##name:                                 \
    return className##_get_##name##Withterm;

#define cBeanHelperCONFIG_GET_NAME_BY_INDEX_SWITCH_CASE_MARCO( \
    className, index, type, name, defaultValue, ...)           \
  case className##Index_##name:                                \
    return #name;

#define cBeanHelperCONVERT_VALUE_BY_INDEX_SWITCH_CASE_MARCO(             \
    className, index, type, name, defaultValue, ...)                     \
  case className##Index_##name:                                          \
    newValue.data._##type = cBeanHelperStrToValueF_##type(charNewValue); \
    break;

#define cBeanHelperDECLEAR(ClassName, list)                                    \
  /* static ClassName##_t s_config;*/                                          \
                                                                               \
  typedef struct {                                                             \
    list(ClassName, cBeanHelperCONFIG_STRUCT_MARCO)                            \
  } ClassName##_t;                                                             \
  typedef enum {                                                               \
    list(ClassName, cBeanHelperCONFIG_INDEX_ENUM_MARCO)                        \
  } ClassName##IndexEnum_t;                                                    \
  typedef void (*ClassName##ChangeListener_t)(                                 \
      ClassName##IndexEnum_t index, Iterm_t oldValue, Iterm_t newValue,        \
      bool inTaskContext);                                                     \
  /*Config_get*/                                                               \
  ClassName##_t *ClassName##_get(void);                                        \
  ClassName##ChangeListener_t ClassName##_getChangeListener(void);             \
  void ClassName##_init(void);                                                 \
  /*get and set function*/                                                     \
  list(ClassName, cBeanHelperCONFIG_FUNCTION_DEFINE_MARCO);                    \
  static INLINE Iterm_t *ClassName##_getIterm(ClassName##IndexEnum_t index) {  \
    switch (index) {                                                           \
      list(ClassName, cBeanHelperCONFIG_GET_ITERM_SWITCH_CASE_MARCO);          \
      default:                                                                 \
        return NULL;                                                           \
    }                                                                          \
    return NULL;                                                               \
  }                                                                            \
  static INLINE int32_t ClassName##_getItermTypeID(                            \
      ClassName##IndexEnum_t index) {                                          \
    switch (index) {                                                           \
      list(ClassName, cBeanHelperCONFIG_GET_ITERM_TYPE_ID_SWITCH_CASE_MARCO);  \
      default:                                                                 \
        return -1;                                                             \
    }                                                                          \
    return -1;                                                                 \
  }                                                                            \
  static INLINE beanUtilsCommonSetFunc_t ClassName##_getSetFuncByIndex(        \
      ClassName##IndexEnum_t index) {                                          \
    switch (index) {                                                           \
      list(ClassName, cBeanHelperCONFIG_GET_ITERM_SET_FUNC_SWITCH_CASE_MARCO); \
      default:                                                                 \
        return NULL;                                                           \
    }                                                                          \
    return NULL;                                                               \
  }                                                                            \
  static INLINE beanUtilsCommonGetFunc_t ClassName##_getGetFuncByIndex(        \
      ClassName##IndexEnum_t index) {                                          \
    switch (index) {                                                           \
      list(ClassName, cBeanHelperCONFIG_GET_ITERM_GET_FUNC_SWITCH_CASE_MARCO); \
      default:                                                                 \
        return NULL;                                                           \
    }                                                                          \
    return NULL;                                                               \
  }                                                                            \
  static INLINE bool ClassName##_setByIndex(ClassName##IndexEnum_t index,      \
                                            Iterm_t val) {                     \
    beanUtilsCommonSetFunc_t setFunc = ClassName##_getSetFuncByIndex(index);   \
    if (setFunc) return setFunc(val, true);                                    \
    return false;                                                              \
  }                                                                            \
  static INLINE bool ClassName##_setByIndexISR(ClassName##IndexEnum_t index,   \
                                               Iterm_t val) {                  \
    beanUtilsCommonSetFunc_t setFunc = ClassName##_getSetFuncByIndex(index);   \
    if (setFunc) return setFunc(val, false);                                   \
    return false;                                                              \
  }                                                                            \
  static INLINE bool ClassName##_setByIndexChar(ClassName##IndexEnum_t index,  \
                                                const char *charNewValue) {    \
    Iterm_t *iterm = ClassName##_getIterm(index);                              \
    if (iterm != NULL) {                                                       \
      Iterm_t newValue;                                                        \
      switch (index) {                                                         \
        list(ClassName, cBeanHelperCONVERT_VALUE_BY_INDEX_SWITCH_CASE_MARCO);  \
        default:                                                               \
          return false;                                                        \
      }                                                                        \
      return ClassName##_setByIndex(index, newValue);                          \
    }                                                                          \
    return false;                                                              \
  }                                                                            \
  static INLINE bool ClassName##_strToIterm(ClassName##IndexEnum_t index,      \
                                            const char *charNewValue,          \
                                            Iterm_t *iterm) {                  \
    if (iterm != NULL) {                                                       \
      Iterm_t newValue;                                                        \
      switch (index) {                                                         \
        list(ClassName, cBeanHelperCONVERT_VALUE_BY_INDEX_SWITCH_CASE_MARCO);  \
        default:                                                               \
          return false;                                                        \
      }                                                                        \
      *iterm = newValue;                                                       \
      return true;                                                             \
    }                                                                          \
    return false;                                                              \
  }                                                                            \
  static INLINE char *ClassName##_itermToStr(char *buf, size_t bufsize,        \
                                             ClassName##IndexEnum_t index,     \
                                             Iterm_t value) {                  \
    int typeID = ClassName##_getItermTypeID(index);                            \
    char *formatID = NULL;                                                     \
    switch (typeID) {                                                          \
      cBeanHelperSUPPORT_TYPE_LIST(cBeanHelperItermToStrCase) default          \
          : snprintf(buf, bufsize, "unkownType");                              \
      return buf;                                                              \
    }                                                                          \
  }                                                                            \
  static INLINE const char *ClassName##_indexToStr(                            \
      ClassName##IndexEnum_t index) {                                          \
    switch (index) {                                                           \
      list(ClassName, cBeanHelperCONFIG_GET_NAME_BY_INDEX_SWITCH_CASE_MARCO);  \
      default:                                                                 \
        return "unkownIndex";                                                  \
    }                                                                          \
  }

#define cBeanHelperIMPL(ClassName, list)                            \
  static ClassName##ChangeListener_t s_##ClassName##ChangeListener; \
  static ClassName##_t s_##ClassName;                               \
  ClassName##_t *ClassName##_get(void) { return &s_##ClassName; };  \
  void ClassName##_init(void) { list(ClassName, cBeanHelperASSIGN_MARCO); };

#define cBeanHelperLISTENER_IMPL(ClassName, listenerName)                  \
  static void listenerName(ClassName##IndexEnum_t index, Iterm_t oldValue, \
                           Iterm_t newValue, bool inTaskContext);          \
  ClassName##ChangeListener_t ClassName##_getChangeListener(void) {        \
    return listenerName;                                                   \
  } /******************************* */                                    \
  static void listenerName(ClassName##IndexEnum_t index, Iterm_t oldValue, \
                           Iterm_t newValue, bool inTaskContext)
