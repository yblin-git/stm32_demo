#include "loop_queue.h"
// #include <assert.h>
// #include <stdio.h>
#include <string.h>
/**
 *
 * @满空状态定义
 * 
 * 满 write_off + 1 *each_element_size == read_off
 * _________|________|______________
 *       write_off read_off
 *
 * 空 write_off == read_off
 * _________|_______________________
 *       write_off(read_off)
 *
 * 
 **************************************************************************/

static size_t get_next_offset2(__loop_queue_t *queue, uint32_t cur_off) {
  /**
   * @brief 
   * 
   * 0______1______2
   * start_off = 0
   * size = 2
   * 有的的off 0,1
   * 
   */
  if (cur_off + queue->each_element_size * 2 <= queue->capacity)
    return cur_off + queue->each_element_size;
  return 0;
}

size_t __loop_queue_get_element_capacity(__loop_queue_t *queue) {
  //循环buffer的大小是buffer真正大小的size -1
  return queue->capacity / queue->each_element_size - 1;
}

void __loop_queue_clear_xxxx(__loop_queue_t *queue) {
  queue->read_offset = 0;
  queue->write_offset = 0;
  return;
}
void __loop_queue_init_xxxx(__loop_queue_t *queue, size_t each_element_size,
                            uint8_t *data, size_t size) {
  queue->buf = data;
  queue->capacity = size;
  queue->read_offset = 0;
  queue->write_offset = 0;
  queue->each_element_size = each_element_size;
  return;
}

size_t __loop_queue_get_remain_size(__loop_queue_t *queue) {
  return __loop_queue_get_element_capacity(queue) -
         __loop_queue_get_size(queue);
}

bool __loop_queue_pop_one_xxxx(__loop_queue_t *queue, void *data) {
  //当前队列为空
  if (__loop_queue_is_empty(queue)) return false;
  memcpy(data, queue->buf + queue->read_offset, queue->each_element_size);
  queue->read_offset = get_next_offset2(queue, queue->read_offset);
  return true;
}

int __loop_queue_pop_some_xxxx(__loop_queue_t *queue, void *data, int num) {
  if (__loop_queue_is_empty(queue)) return 0;

  size_t q_size = __loop_queue_get_size(queue);
  size_t cpy_size = q_size < num ? q_size : num;

  size_t readoff = queue->read_offset;
  size_t writeoff = queue->write_offset;
  /**
   * @brief
   * @情况1,read_off > write_off,
   * ____1_____|______2_______|________3__
   *          write_off     read_off
   * 拷贝3&&1
   *
   * @情况2,read_off < write_off,
   * ____1_____|______2_______|________3__
   *          read_off     write_off
   *拷贝2
   *
   */
  if (readoff > writeoff) {
    size_t d3cpy_size = (queue->capacity - readoff) / queue->each_element_size;
    if (d3cpy_size > cpy_size) {
      memcpy(data, queue->buf + readoff, cpy_size * queue->each_element_size);
      queue->read_offset = readoff + cpy_size * queue->each_element_size;
    } else {
      memcpy(data, queue->buf + readoff, d3cpy_size * queue->each_element_size);
      if (cpy_size - d3cpy_size != 0) {
        memcpy((uint8_t *)data + d3cpy_size * queue->each_element_size,
               queue->buf, (cpy_size - d3cpy_size) * queue->each_element_size);
      }
      queue->read_offset = (cpy_size - d3cpy_size) * queue->each_element_size;
    }
  } else if (readoff < writeoff) {
    memcpy(data, queue->buf + readoff, cpy_size * queue->each_element_size);
    queue->read_offset = readoff + cpy_size * queue->each_element_size;
  }
  return cpy_size;
}

bool __loop_queue_push_some_xxxx(__loop_queue_t *queue, const void *data,
                                 int num) {
  size_t q_size = __loop_queue_get_remain_size(queue);
  if (num > q_size) {
    return false;
  }
  size_t readoff = queue->read_offset;
  size_t writeoff = queue->write_offset;
  /**
   * @brief
   * @情况1,read_off > write_off,
   * ____1_____|______2_______|________3__
   *          write_off     read_off
   * 拷贝数据到区间2
   *
   * @情况2,read_off < write_off,
   * ____1_____|______2_______|________3__
   *          read_off     write_off
   *拷贝数据到3&1
   *
   */
  if (readoff > writeoff) {
    memcpy(queue->buf + writeoff, data, num * queue->each_element_size);
    queue->write_offset = writeoff + num * queue->each_element_size;
  } else if (readoff <= writeoff) {
    size_t d3cpy_size = (queue->capacity - writeoff) / queue->each_element_size;
    if (num <= d3cpy_size) {
      memcpy(queue->buf + writeoff, data, num * queue->each_element_size);
      queue->write_offset = writeoff + num * queue->each_element_size;
    } else {
      memcpy(queue->buf + writeoff, data,
             d3cpy_size * queue->each_element_size);
      if (num - d3cpy_size > 0) {
        memcpy(queue->buf,
               (uint8_t *)data + d3cpy_size * queue->each_element_size,
               (num - d3cpy_size) * queue->each_element_size);
      }
      queue->write_offset = 0 + (num - d3cpy_size) * queue->each_element_size;
    }
  }
  return true;
}

bool __loop_queue_push_one_xxxx(__loop_queue_t *queue, const void *data) {
  //当前queue中数据已经满了
  if (__loop_queue_is_full(queue)) {
    return false;
  }

  uint8_t *write_add = queue->buf + queue->write_offset;
  memcpy(write_add, data, queue->each_element_size);
  queue->write_offset = get_next_offset2(queue, queue->write_offset);
  return true;
}

bool __loop_queue_is_full(__loop_queue_t *queue) {
  if (get_next_offset2(queue, queue->write_offset) == queue->read_offset) {
    return true;
  }
  return false;
}
bool __loop_queue_is_empty(__loop_queue_t *queue) {
  if (queue->read_offset == queue->write_offset) {
    return true;
  }
  return false;
}

size_t __loop_queue_get_size(__loop_queue_t *queue) {
  size_t readoff = queue->read_offset;
  size_t writeoff = queue->write_offset;
  if (readoff > writeoff) {
    return (writeoff + queue->capacity - readoff) / queue->each_element_size;
  } else if (readoff < writeoff) {
    return (writeoff - readoff) / queue->each_element_size;
  } else {
    return 0;
  }
}
// size_t __loop_queue_get_size(__loop_queue_t *queue) {
//   if (queue->read_offset > queue->write_offset) {
//     return (queue->write_offset + queue->capacity - queue->read_offset) /
//     queue->each_element_size;
//   } else if (queue->read_offset < queue->write_offset) {
//     return (queue->write_offset - queue->read_offset) /
//     queue->each_element_size;
//   } else {
//     return 0;
//   }
// }
