#pragma once
// #include "zwtimecpp/core/utils/better-enums/enum.h"
// #include "/home/zhaohe/workplace/project/MobileWrappingMachines/tmp/enum.h"
#define ZWSD_MACROS_ID(x) x

#define ZWSD_APPLY(macro, ...) ZWSD_MACROS_ID(macro(__VA_ARGS__))
#define MARCO_APPLY ZWSD_APPLY
/**
 * @brief 参数的数量最大是64个,当大于64的时候会出错
 */
#define ZWSD_COUNT_BASIC(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, \
                         _13, _14, _15, _16, _17, _18, _19, _20, _21, _22,  \
                         _23, _24, _25, _26, _27, _28, _29, _30, _31, _32,  \
                         _33, _34, _35, _36, _37, _38, _39, _40, _41, _42,  \
                         _43, _44, _45, _46, _47, _48, _49, _50, _51, _52,  \
                         _53, _54, _55, _56, _57, _58, _59, _60, _61, _62,  \
                         _63, _64, count, ...)                              \
  count

#define ZWSD_PARAMETER_COUNT(...)                                              \
  ZWSD_MACROS_ID(ZWSD_COUNT_BASIC(                                             \
      __VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
      49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32,  \
      31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14,  \
      13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1))

/**
 * @brief MARCO_LIST
 * @Usage:
 *
 * 这个工具用来使宏支持多参数功能
 * 例如定义一个宏LIST实现下面功能
 *
 * LIST(int,3,a,b,c,d,e)
 *  展开后如下
 * int a = 3;
 * int b = 3;
 * int c = 3;
 * int d = 3;
 * int e = 3;
 *
 * LIST(int,3,a)
 *  展开后如下
 * int a = 3;
 *
 * 实现方法
 *
 * #define EACH_EXPTR(type,initvalue,v3,v4,v5,index,varName) \
 *  type varName = initvalue;
 *
 * #define LIST(type,initvalue,...) \
 *  MARCO_LIST(EACH_EXPTR,type,initvalue,_3,_4,_5,__VA_ARGS__)
 */
/**
 * @brief
 *
 */
#define MARCO_LIST(marco, v1, v2, v3, v4, v5, ...) \
  ZWSD_MACROS_ID(                                  \
      V5M_PP_MAP(ZWSD_MACROS_ID(marco), v1, v2, v3, v4, v5, __VA_ARGS__))

#define V5M_PP_MAP(macro, v1, v2, v3, v4, v5, ...)                        \
  ZWSD_MACROS_ID(                                                         \
      ZWSD_APPLY(V5_PP_MAP_VAR_COUNT, ZWSD_PARAMETER_COUNT(__VA_ARGS__))( \
          macro, v1, v2, v3, v4, v5, __VA_ARGS__))

#define V5_PP_MAP_VAR_COUNT(count) V5M##count
#define V5M1(m, v1, v2, v3, v4, v5, x) m(v1, v2, v3, v4, v5, 0, x)
#define V5M2(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 1, x)               \
      ZWSD_MACROS_ID(V5M1(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M3(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 2, x)               \
      ZWSD_MACROS_ID(V5M2(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M4(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 3, x)               \
      ZWSD_MACROS_ID(V5M3(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M5(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 4, x)               \
      ZWSD_MACROS_ID(V5M4(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M6(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 5, x)               \
      ZWSD_MACROS_ID(V5M5(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M7(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 6, x)               \
      ZWSD_MACROS_ID(V5M6(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M8(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 7, x)               \
      ZWSD_MACROS_ID(V5M7(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M9(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 8, x)               \
      ZWSD_MACROS_ID(V5M8(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M10(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 9, x)                \
      ZWSD_MACROS_ID(V5M9(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M11(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 10, x)               \
      ZWSD_MACROS_ID(V5M10(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M12(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 11, x)               \
      ZWSD_MACROS_ID(V5M11(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M13(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 12, x)               \
      ZWSD_MACROS_ID(V5M12(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M14(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 13, x)               \
      ZWSD_MACROS_ID(V5M13(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M15(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 14, x)               \
      ZWSD_MACROS_ID(V5M14(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M16(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 15, x)               \
      ZWSD_MACROS_ID(V5M15(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M17(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 16, x)               \
      ZWSD_MACROS_ID(V5M16(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M18(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 17, x)               \
      ZWSD_MACROS_ID(V5M17(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M19(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 18, x)               \
      ZWSD_MACROS_ID(V5M18(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M20(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 19, x)               \
      ZWSD_MACROS_ID(V5M19(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M21(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 20, x)               \
      ZWSD_MACROS_ID(V5M20(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M22(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 21, x)               \
      ZWSD_MACROS_ID(V5M21(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M23(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 22, x)               \
      ZWSD_MACROS_ID(V5M22(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M24(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 23, x)               \
      ZWSD_MACROS_ID(V5M23(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M25(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 24, x)               \
      ZWSD_MACROS_ID(V5M24(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M26(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 25, x)               \
      ZWSD_MACROS_ID(V5M25(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M27(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 26, x)               \
      ZWSD_MACROS_ID(V5M26(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M28(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 27, x)               \
      ZWSD_MACROS_ID(V5M27(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M29(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 28, x)               \
      ZWSD_MACROS_ID(V5M28(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M30(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 29, x)               \
      ZWSD_MACROS_ID(V5M29(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M31(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 30, x)               \
      ZWSD_MACROS_ID(V5M30(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M32(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 31, x)               \
      ZWSD_MACROS_ID(V5M31(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M33(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 32, x)               \
      ZWSD_MACROS_ID(V5M32(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M34(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 33, x)               \
      ZWSD_MACROS_ID(V5M33(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M35(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 34, x)               \
      ZWSD_MACROS_ID(V5M34(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M36(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 35, x)               \
      ZWSD_MACROS_ID(V5M35(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M37(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 36, x)               \
      ZWSD_MACROS_ID(V5M36(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M38(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 37, x)               \
      ZWSD_MACROS_ID(V5M37(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M39(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 38, x)               \
      ZWSD_MACROS_ID(V5M38(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M40(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 39, x)               \
      ZWSD_MACROS_ID(V5M39(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M41(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 40, x)               \
      ZWSD_MACROS_ID(V5M40(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M42(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 41, x)               \
      ZWSD_MACROS_ID(V5M41(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M43(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 42, x)               \
      ZWSD_MACROS_ID(V5M42(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M44(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 43, x)               \
      ZWSD_MACROS_ID(V5M43(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M45(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 44, x)               \
      ZWSD_MACROS_ID(V5M44(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M46(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 45, x)               \
      ZWSD_MACROS_ID(V5M45(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M47(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 46, x)               \
      ZWSD_MACROS_ID(V5M46(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M48(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 47, x)               \
      ZWSD_MACROS_ID(V5M47(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M49(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 48, x)               \
      ZWSD_MACROS_ID(V5M48(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M50(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 49, x)               \
      ZWSD_MACROS_ID(V5M49(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M51(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 50, x)               \
      ZWSD_MACROS_ID(V5M50(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M52(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 51, x)               \
      ZWSD_MACROS_ID(V5M51(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M53(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 52, x)               \
      ZWSD_MACROS_ID(V5M52(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M54(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 53, x)               \
      ZWSD_MACROS_ID(V5M53(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M55(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 54, x)               \
      ZWSD_MACROS_ID(V5M54(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M56(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 55, x)               \
      ZWSD_MACROS_ID(V5M55(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M57(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 56, x)               \
      ZWSD_MACROS_ID(V5M56(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M58(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 57, x)               \
      ZWSD_MACROS_ID(V5M57(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M59(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 58, x)               \
      ZWSD_MACROS_ID(V5M58(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M60(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 59, x)               \
      ZWSD_MACROS_ID(V5M59(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M61(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 60, x)               \
      ZWSD_MACROS_ID(V5M60(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M62(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 61, x)               \
      ZWSD_MACROS_ID(V5M61(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M63(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 62, x)               \
      ZWSD_MACROS_ID(V5M62(m, v1, v2, v3, v4, v5, __VA_ARGS__))
#define V5M64(m, v1, v2, v3, v4, v5, x, ...) \
  m(v1, v2, v3, v4, v5, 63, x)               \
      ZWSD_MACROS_ID(V5M63(m, v1, v2, v3, v4, v5, __VA_ARGS__))

/*************************************************************** */
