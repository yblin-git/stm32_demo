#pragma once

/**
 * @brief
 * API-List
 *
 * ARRARY_SIZE
 *
 * BUILD_ASSERT
 * BUILD_ASSERT_OR_ZERO
 *
 * min
 * max
 * zclamp
 */

#ifndef ARRARY_SIZE
#define ARRARY_SIZE(arrary) sizeof(arrary) / sizeof(arrary[0])
#endif

/**
 * BUILD_ASSERT - assert a build-time dependency.
 * @cond: the compile-time condition which must be true.
 *
 * Your compile will fail if the condition isn't true, or can't be evaluated
 * by the compiler.  This can only be used within a function.
 *
 * Example:
 *	#include <stddef.h>
 *	...
 *	static char *foo_to_char(struct foo *foo)
 *	{
 *		// This code needs string to be at start of foo.
 *		BUILD_ASSERT(offsetof(struct foo, string) == 0);
 *		return (char *)foo;
 *	}
 */
#define BUILD_ASSERT(cond)               \
  do {                                   \
    (void)sizeof(char[1 - 2 * !(cond)]); \
  } while (0)

/**
 * BUILD_ASSERT_OR_ZERO - assert a build-time dependency, as an expression.
 * @cond: the compile-time condition which must be true.
 *
 * Your compile will fail if the condition isn't true, or can't be evaluated
 * by the compiler.  This can be used in an expression: its value is "0".
 *
 * Example:
 *	#define foo_to_char(foo)					\
 *		 ((char *)(foo)						\
 *		  + BUILD_ASSERT_OR_ZERO(offsetof(struct foo, string) == 0))
 */
#define BUILD_ASSERT_OR_ZERO(cond) (sizeof(char[1 - 2 * !(cond)]) - 1)

#define EXTERN extern

/**
 * @brief min and max
 *
 */
#define Z_MINMAX_ASSERT_COMPATIBLE(a, b) \
  BUILD_ASSERT(__builtin_types_compatible_p(a, b))

/**
 * @brief 最小值
 *
 */
#define zmin(a, b)                                      \
  ({                                                    \
    typeof(a) _a = (a);                                 \
    typeof(b) _b = (b);                                 \
    Z_MINMAX_ASSERT_COMPATIBLE(typeof(_a), typeof(_b)); \
    _a < _b ? _a : _b;                                  \
  })

/**
 * @brief 最大值
 *
 */
#define zmax(a, b)                                      \
  ({                                                    \
    typeof(a) _a = (a);                                 \
    typeof(b) _b = (b);                                 \
    Z_MINMAX_ASSERT_COMPATIBLE(typeof(_a), typeof(_b)); \
    _a > _b ? _a : _b;                                  \
  })

/**
 * @brief 中间值
 *
 */
#define zclamp(v, f, c) (zmax(zmin((v), (c)), (f)))

#define zmin_t(t, a, b) \
  ({                    \
    t _ta = (a);        \
    t _tb = (b);        \
    zmin(_ta, _tb);     \
  })
#define zmax_t(t, a, b) \
  ({                    \
    t _ta = (a);        \
    t _tb = (b);        \
    zmax(_ta, _tb);     \
  })

#define zclamp_t(t, v, f, c) \
  ({                         \
    t _tv = (v);             \
    t _tf = (f);             \
    t _tc = (c);             \
    zclamp(_tv, _tf, _tc);   \
  })

#define __MARCO_TO_STR(name) #name
#define MARCO_TO_STR(name) __MARCO_TO_STR(name)

#define SPLIT_DATA(src, srcSize, _splitSize, processSplitData) \
  {                                                            \
    unsigned npacket = srcSize / _splitSize;                   \
    unsigned finalPacketSize = srcSize % _splitSize;           \
    if (finalPacketSize > 0) npacket += 1;                     \
    if (finalPacketSize == 0) {                                \
      for (unsigned i = 0; i < npacket; ++i) {                 \
        {                                                      \
          void* splitData = src + i * _splitSize;              \
          size_t splitDataSize = _splitSize;                   \
          processSplitData                                     \
        }                                                      \
      }                                                        \
    } else {                                                   \
      for (unsigned i = 0; i < npacket; ++i) {                 \
        if (i + 1 != npacket) {                                \
          void* splitData = src + i * _splitSize;              \
          size_t splitDataSize = _splitSize;                   \
          processSplitData                                     \
        } else {                                               \
          void* splitData = src + i * _splitSize;              \
          size_t splitDataSize = finalPacketSize;              \
          processSplitData                                     \
        }                                                      \
      }                                                        \
    }                                                          \
  }

/** */
