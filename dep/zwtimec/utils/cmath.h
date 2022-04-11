#include <stdint.h>
#include "zwtimec/utils/compiler_config.h"

STATIC_INLINE int32_t zabs_int32_t(int32_t v) {
  if (v < 0) return -v;
  return v;
}