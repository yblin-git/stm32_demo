#pragma once
/*
   _                   ___
  | |   ___  ___ _ __ / _ \ _  _ ___ _  _ ___
  | |__/ _ \/ _ \ '_ \ (_) | || / -_) || / -_)
  |____\___/\___/ .__/\__\_\\_,_\___|\_,_\___|
                |_|
*/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "zwtimec/utils/compiler_config.h"

typedef struct __loop_queue_s __loop_queue_t;
struct __loop_queue_s {
  uint8_t *buf;
  size_t capacity;

  volatile size_t read_offset;  // real_offset = read_offset*each_element_size
  volatile size_t write_offset;
  volatile size_t each_element_size;
};

/*
   _   _
  | | | |___ __ _ __ _ ___
  | |_| (_-</ _` / _` / -_)
   \___//__/\__,_\__, \___|
                 |___/
*/
// /*使能对uint8_t类型的支持*/
// LOOP_QUEUE_ENABLE_SUPPORT(uint8_t);
// void usage() {
//   uint8_t buf[16] = {0};
//   __loop_queue_t loop_queue;
//   loop_queue_push_one_uint8_t(&loop_queue, 1);
// }
//
// /*使能对uint16_t类型的支持*/
// LOOP_QUEUE_ENABLE_SUPPORT(uint16_t);
// void usage() {
//   uint16_t buf[16] = {0};
//   __loop_queue_t loop_queue;
//   loop_queue_push_one_uint16_t(&loop_queue, 1);
// }

/**
 * @brief 初始化队列
 *
 * @param queue
 * @param each_element_size 每个元素所使用的空间大小
 * @param data
 * @param size
 */
void __loop_queue_init_xxxx(__loop_queue_t *queue, size_t each_element_size,
                            uint8_t *data, size_t size);
/**
 * @brief pop一个元素
 *
 * @param queue
 * @param data
 * @return true
 * @return false
 */
bool __loop_queue_pop_one_xxxx(__loop_queue_t *queue, void *data);

/**
 * @brief
 *
 * @param queue
 * @param data
 * @param num 数据的数量，非uint8_t的数量,而是element的数量
 * @return int
 */
int __loop_queue_pop_some_xxxx(__loop_queue_t *queue, void *data, int num);
/**
 * @brief __loop_queue_push_some_xxxx
 *
 * @param queue
 * @param data
 * @param num　推入数据的数量，非uint8_t的数量,而是element的数量
 * @return true
 * @return false
 */
bool __loop_queue_push_some_xxxx(__loop_queue_t *queue, const void *data,
                                 int num);
/**
 * @brief push一个元素
 *
 * @param queue
 * @param data
 * @return true
 * @return false
 */
bool __loop_queue_push_one_xxxx(__loop_queue_t *queue, const void *data);
/**
 * @brief 判断队列是否为空
 *
 * @param queue
 * @return true
 * @return false
 */
bool __loop_queue_is_empty(__loop_queue_t *queue);
/**
 * @brief 判断队列是否满
 *
 * @param queue
 * @return true
 * @return false
 */
bool __loop_queue_is_full(__loop_queue_t *queue);
/**
 * @brief 获得队列当前size
 *
 * @param queue
 * @return size_t
 */
size_t __loop_queue_get_size(__loop_queue_t *queue);
/**
 * @brief 获得队列当前size
 *
 * @param queue
 * @return size_t
 */
void __loop_queue_clear_xxxx(__loop_queue_t *queue);

/**
 * @brief 获得队列的容量
 *
 * @param queue
 * @return size_t
 */
size_t __loop_queue_get_element_capacity(__loop_queue_t *queue);
#define LOOP_QUEUE_ENABLE_SUPPORT(name, type) \
  LOOP_QUEUE_ENABLE_SUPPORT_PROTECT(name, type, {}, {})

#define LOOP_QUEUE_ENABLE_SUPPORT_PROTECT(name, type, enter, exit)             \
  typedef struct { __loop_queue_t handler; } loop_queue_##name##_t;            \
  static INLINE void loop_queue_init_##name(loop_queue_##name##_t *queue,      \
                                            type *data, size_t size) {         \
    __loop_queue_init_xxxx(&queue->handler, sizeof(type), (uint8_t *)data,     \
                           size * sizeof(type));                               \
    return;                                                                    \
  }                                                                            \
  static INLINE bool loop_queue_pop_one_##name(loop_queue_##name##_t *queue,   \
                                               type *data) {                   \
    return __loop_queue_pop_one_xxxx(&queue->handler, data);                   \
  }                                                                            \
  static INLINE bool loop_queue_push_one_##name(loop_queue_##name##_t *queue,  \
                                                type data) {                   \
    enter;                                                                     \
    bool ret = __loop_queue_push_one_xxxx(&queue->handler, &data);             \
    exit;                                                                      \
    return ret;                                                                \
  }                                                                            \
  static INLINE bool loop_queue_push_one_##name##_p(                           \
      loop_queue_##name##_t *queue, const type *data) {                        \
    enter;                                                                     \
    bool ret = __loop_queue_push_one_xxxx(&queue->handler, data);              \
    exit;                                                                      \
    return ret;                                                                \
  }                                                                            \
  static INLINE bool loop_queue_is_empty_##name(                               \
      loop_queue_##name##_t *queue) {                                          \
    return __loop_queue_is_empty(&queue->handler);                             \
  }                                                                            \
  static INLINE bool loop_queue_is_full_##name(loop_queue_##name##_t *queue) { \
    return __loop_queue_is_full(&queue->handler);                              \
  }                                                                            \
  static INLINE size_t loop_queue_get_size_##name(                             \
      loop_queue_##name##_t *queue) {                                          \
    return __loop_queue_get_size(&queue->handler);                             \
  }                                                                            \
  static INLINE int loop_queue_pop_some_##name(loop_queue_##name##_t *queue,   \
                                               type *data, int num) {          \
    return __loop_queue_pop_some_xxxx(&queue->handler, (void *)data, num);     \
  }                                                                            \
  static INLINE bool loop_queue_push_some_##name(loop_queue_##name##_t *queue, \
                                                 const type *data, int num) {  \
    enter;                                                                     \
    bool ret =                                                                 \
        __loop_queue_push_some_xxxx(&queue->handler, (void *)data, num);       \
    exit;                                                                      \
    return ret;                                                                \
  }                                                                            \
  static INLINE int loop_queue_get_capatity_##name(                            \
      loop_queue_##name##_t *queue) {                                          \
    return __loop_queue_get_element_capacity(&queue->handler);                 \
  }                                                                            \
  static INLINE void loop_queue_clear_##name(loop_queue_##name##_t *queue) {   \
    __loop_queue_clear_xxxx(&queue->handler);                                  \
    return;                                                                    \
  }

#define LOOP_QUEUE_DEFINE(name, type, size)     \
  static loop_queue_##name##_t s_##name##Queue; \
  static type s_##name##QueueData[size];

#define LOOP_QUEUE_INIT(name)                                     \
  loop_queue_init_##name##(&s_##name##Queue, s_##name##QueueData, \
                           ARRARY_SIZE(s_##name##QueueData));
#define LOOP_QUEUE_PUSH_ONE(name, queue, data) \
  loop_queue_push_one_##name##(queue, data);
/**
 * @brief 哪里用到这个定义在哪个c文件中使用下面的宏使能对相应结构体的支持
 *
 */
LOOP_QUEUE_ENABLE_SUPPORT(u8, uint8_t);
LOOP_QUEUE_ENABLE_SUPPORT(u16, uint16_t);
LOOP_QUEUE_ENABLE_SUPPORT(u32, uint32_t);
LOOP_QUEUE_ENABLE_SUPPORT(i8, int8_t);
LOOP_QUEUE_ENABLE_SUPPORT(i16, int16_t);
LOOP_QUEUE_ENABLE_SUPPORT(i32, int32_t);
