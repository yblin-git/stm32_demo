/*
 * @Author: your name
 * @Date: 2020-05-15 15:17:10
 * @LastEditTime: 2020-05-27 21:04:24
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \DisinfectionTrolleyBox\user_src\helper\dwin_packet.c
 */

#include "zwtimec/utils/loop_queue.h"
#define DWIN_WRITE 0x82
#define DWIN_READ 0x83

typedef enum {
  kReadPageNum = 0x0014,
  kSwitchPage = 0x0084,
  kSlectLanguage = 0x2001,
  kDisinfectSpace = 0x2003,
  kDisplayDisinfectSpace = 0x2007,
  kUseTime = 0x200b,
  kUseDetergent = 0x2017,
  kStartDisinfect = 0x201b,
  kDisinfectDelayStart = 0x201d,
  kStopDisinfect = 0x201f,
  kDisinfectRemainingTime = 0x2021,
  kCompleteDisinfectSpace = 0x202d,
  kCompleteUseTime = 0x2031,
  kCompleteUseDetergent = 0x203d,
  kDisinfectSpaceMinuteSet = 0x2041,
  kDelayStartTimeSet = 0x2045,
  kEmptyWaterTime = 0x2049,
  kPairDisplay = 0x205B,
  kStartPair = 0x205D,
  kClearPair = 0x205F,
  kTestAirCompressorMachineOpen = 0x3001,
  kTestAirCompressorMachineClose = 0x3003,
  kTestFanOpen = 0x3005,
  kTestFanClose = 0x3007,
  kTestRfPowerOpen = 0x3009,
  kTestRfPowerClose = 0x300B,
  kTestRfDisplay = 0x300D,
  kTestScreenInput = 0x300F,
  kTestScreenDisplay = 0x3011,
  kTestPowerKey = 0x3013,
  kTestTempCollectionOpen = 0x3015,
  kTestTempCollectionClose = 0x3017,
  kTestTempCollectionDisplay = 0x3019,
  kTestBuzzerOpen = 0x301B,
  kTestBuzzerClose = 0x301D,
  kTestExit = 0x301F,
  kAck = 0x4f4b,
} AddType_e;

typedef enum {
  kLanagePage = 7,
  kWelcomePage = 6,
  kDisplayDisinfectSpacePage = 2,
  kDisplayDisinfectParameterPage = 42,
  kDisinfectDelayStartPage = 44,
  kDisinfectRemainingTimePage = 46,
  kCompleteDisinfect = 47,
  kAlarmPage = 49,
  kEnDisplayDisinfectSpacePage = 53,
  kEnDisplayDisinfectParameterPage = 55,
  kEnDisinfectDelayStartPage = 57,
  kEnDisinfectRemainingTimePage = 59,
  kEnCompleteDisinfect = 60,
  kEnAlarmPage = 62,
  kProductTestPage = 66,
  kEmptyWaterPage = 68,
  kEnEmptyWaterPage = 69,
} DwinPage_e;

typedef enum {
  kPairSuccess = 1,
  kPairFail = 2,
  kClearSuccess = 3,
  kClearFail = 4,
  kPressRemoteContro = 5,
} PairEvent_e;

typedef struct {
  loop_queue_u8_t receiveq;
  uint8_t* receiveqbuf;
  size_t receivebufqSize;
  uint8_t* processbuf;
  size_t processbufSize;
  size_t usefullSize;

  void* userData;
} DwinPacketProcesser_t;

#pragma pack(1)
typedef struct {
  uint8_t h_header;
  uint8_t l_header;
  uint8_t len;
  uint8_t cmd;
  uint8_t h_addr;
  uint8_t l_addr;
  uint8_t data[];
} DwinPacket_t;
#pragma pack()

typedef enum {
  DPPE_doNothing = 0,
  //调用PacketV2Processer_process
  DPPE_processPacketIsReady = 1,
  //调用PacketV2Processer_process,并上报消息处理回执
  DPPE_receiveBufOverflow = 2,
} DwinPacketProcessEvent_t;  // DPPE

typedef enum {
  DPPS_success = 0,
  DPPS_PacketIsTooLong = 1,
  DPPS_LosePacketTail = 2,
} DwinPacketProcessStatus_t;  // DPPS

typedef void (*onPacket_t)(DwinPacketProcesser_t* processer,
                           DwinPacketProcessStatus_t status,
                           DwinPacket_t* packet, size_t size);
uint8_t DwinPacketProcesser_getReportDataSize(DwinPacket_t* header) ;
uint8_t* DwinPacketProcesser_getReportData(DwinPacket_t* header) ;
uint16_t DwinPacketProcesser_getAddr(DwinPacket_t* header);
void DwinPacket_init(DwinPacketProcesser_t* handler, uint8_t* receiveqbuf,
                     size_t receivebufqSize, uint8_t* processbuf,
                     size_t processbufSize);
DwinPacketProcessEvent_t DwinPacket_pushData(DwinPacketProcesser_t* handler,
                                             uint8_t* data, size_t size);
static void DwinPacket_parseInternal(DwinPacketProcesser_t* handler,
                                     onPacket_t onPacket);
void DwinPacket_process(DwinPacketProcesser_t* handler,
                        DwinPacketProcessEvent_t event, onPacket_t onPacket);
size_t DwinPacket_createPacket(uint8_t* packetBuffer, size_t packetBufferSize,
                               uint8_t cmd, uint16_t addr, void* data,
                               size_t size) ;