/*
 * @Author: your name
 * @Date: 2020-05-29 15:44:01
 * @LastEditTime: 2022-04-11 11:00:36
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \MobileWrappingMachine_drive_V3\dep\ztimecpp\zwtimec\utils\compiler_config.h
 */ 
#pragma once

#if defined(__CC_ARM)
#define ASM __asm       /*!< asm keyword for ARM Compiler          */
#define INLINE __inline /*!< inline keyword for ARM Compiler       */
#define STATIC_INLINE static __inline
	#ifndef WEAK
#define WEAK __weak
#endif

#elif defined(__ICCARM__)
#define ASM __asm /*!< asm keyword for IAR Compiler          */
#define INLINE                                                        \
  inline /*!< inline keyword for IAR Compiler. Only available in High \
            optimization mode! */
#define STATIC_INLINE static inline
#define WEAK                                 __attribute__((weak))

#elif defined(__TMS470__)
#define ASM __asm /*!< asm keyword for TI CCS Compiler       */
#define STATIC_INLINE static inline
#define WEAK                                 __attribute__((weak))

#elif defined(__GNUC__)
#define ASM __asm     /*!< asm keyword for GNU Compiler          */
#define INLINE inline /*!< inline keyword for GNU Compiler       */
#define STATIC_INLINE static inline
#define WEAK                                 __attribute__((weak))

#elif defined(__TASKING__)
#define ASM __asm     /*!< asm keyword for TASKING Compiler      */
#define INLINE inline /*!< inline keyword for TASKING Compiler   */
#define STATIC_INLINE static inline
#define WEAK                                 __attribute__((weak))

#endif

#ifndef INLINE
#define INLINE inline
#endif

#ifndef ASM
#define ASM __asm
#endif

#ifndef STATIC_INLINE
#define STATIC_INLINE static inline
#endif

#ifndef WEAK
#define WEAK __attribute__((weak))
#endif

/**/
