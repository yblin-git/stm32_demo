/*
 * @Author: your name
 * @Date: 2020-05-15 15:17:10
 * @LastEditTime: 2020-05-25 11:22:49
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \DisinfectionTrolleyBox\user_src\helper\dwin_packet.c
 */
#include "dwin_packet.h"

#include <string.h>

#include "zwtimec\utils\simple_logger_helper.h"

#define PACKETV2_H_HEADER (0x5a)
#define PACKETV2_L_HEADER (0xa5)
#define PACKETV2_TAIL (0x5e)
#define HEAD_SIZE (3)

static uint8_t DwinPacket_getLen(DwinPacket_t* header) {
  return header->len + HEAD_SIZE;
}
uint16_t DwinPacketProcesser_getAddr(DwinPacket_t* header){
  return (header->h_addr << 8) + header->l_addr;
}
uint8_t DwinPacketProcesser_getReportDataSize(DwinPacket_t* header) {
  return header->data[0];
}
uint8_t *DwinPacketProcesser_getReportData(DwinPacket_t* header) {
  return &header->data[1];
}
void DwinPacket_init(DwinPacketProcesser_t* handler, uint8_t* receiveqbuf,
                     size_t receivebufqSize, uint8_t* processbuf,
                     size_t processbufSize) {
  memset(handler, 0, sizeof(*handler));
  handler->receiveqbuf = receiveqbuf;
  handler->receivebufqSize = receivebufqSize;
  handler->processbuf = processbuf;
  handler->processbufSize = processbufSize;
  handler->usefullSize = 0;
  loop_queue_init_u8(&handler->receiveq, receiveqbuf, receivebufqSize);
}
DwinPacketProcessEvent_t DwinPacket_pushData(
    DwinPacketProcesser_t* handler, uint8_t* data, size_t size) {
  bool success = loop_queue_push_some_u8(&handler->receiveq, data, size);
  if (success == false) {
    return DPPE_receiveBufOverflow;
  }
  size_t length = loop_queue_get_size_u8(&handler->receiveq);
  // LOG_INFO("length:%d size %d", length, sizeof(DwinPacket_t));
  if (length >= sizeof(DwinPacket_t)) {
    // for (size_t i = 0; i < length; i++) {
    //   if ((data[i] == PACKETV2_H_HEADER && data[i + 1] == PACKETV2_L_HEADER) &&
    //       data[i + 2] <= length - i - HEAD_SIZE) {
    return DPPE_processPacketIsReady;
    //   }
    // }
  }
  return DPPE_doNothing;
}
static void DwinPacket_parseInternal(DwinPacketProcesser_t* handler,
                                     onPacket_t onPacket) {
  int beginProcessOff = 0;
  while (true) {
    //找到包头
    DwinPacket_t* packet_header = NULL;
    bool findHeader = false;
    if (handler->usefullSize - beginProcessOff < sizeof(DwinPacket_t)) {
      break;
    }

    for (size_t i = beginProcessOff;
         i + sizeof(DwinPacket_t) < handler->usefullSize; i++) {
      packet_header = (DwinPacket_t*)&handler->processbuf[i];
      if (packet_header->h_header == PACKETV2_H_HEADER &&
          packet_header->l_header == PACKETV2_L_HEADER) {
        beginProcessOff = i;
        findHeader = true;
        break;
      }
      beginProcessOff = i;
    }
    if (!findHeader) {
      break;
    }

    int remainUsefulDataSize = handler->usefullSize - beginProcessOff;
    if (remainUsefulDataSize >= sizeof(DwinPacket_t)) {
      size_t totalLength = DwinPacket_getLen(packet_header);
      size_t tailOff = beginProcessOff + totalLength - 1;
      //情况一，完整的包
      if (remainUsefulDataSize >= totalLength) {
        onPacket(handler, DPPS_success, packet_header, totalLength);
        beginProcessOff += totalLength;
      }
      //情况二，包的长度过长
      else if (totalLength > handler->processbufSize) {
        onPacket(handler, DPPS_PacketIsTooLong, NULL, 0);
        beginProcessOff += 1;
      } else {
        //情况四，缓冲区数据不够
        break;
      }
    }
  }
  //移除处理过的数据
  memmove(handler->processbuf, handler->processbuf + beginProcessOff,
          handler->usefullSize - beginProcessOff);
  handler->usefullSize = handler->usefullSize - beginProcessOff;

  //逻辑不应该运行到此处,此处仅是为了保证逻辑的正确性
  if (handler->usefullSize >= handler->processbufSize) {
    handler->usefullSize = 0;
  }
}
void DwinPacket_process(DwinPacketProcesser_t* handler,
                        DwinPacketProcessEvent_t event, onPacket_t onPacket) {
  if (onPacket == NULL) {
    loop_queue_clear_u8(&handler->receiveq);
    handler->usefullSize = 0;
    return;
  }

  while (true) {
    //从缓冲buffer中取出适量的数据
    int pop_size = handler->processbufSize - handler->usefullSize;
    int real_pop_size = 0;
    if (pop_size != 0) {
      real_pop_size = loop_queue_pop_some_u8(
          &handler->receiveq, handler->processbuf + handler->usefullSize,
          pop_size);
    }

    if (real_pop_size == 0) {
      break;
    }
    handler->usefullSize += real_pop_size;
    DwinPacket_parseInternal(handler, onPacket);
  }
  return;
}

size_t DwinPacket_createPacket(uint8_t* packetBuffer, size_t packetBufferSize,
                               uint8_t cmd, uint16_t addr, void* data,
                               size_t size) {
  DwinPacket_t* packet = (DwinPacket_t*)packetBuffer;
  uint16_t totalSize = size + 3;
  if (packetBufferSize < totalSize) {
    return 0;
  }
  packet->h_header = PACKETV2_H_HEADER;
  packet->l_header = PACKETV2_L_HEADER;
  packet->len = totalSize;
  packet->cmd = cmd;
  packet->h_addr = addr >> 8 & 0xff;
  packet->l_addr = addr & 0xff;
  memcpy(packet->data, data, size);
  return totalSize + HEAD_SIZE;
}