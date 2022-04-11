/*
 * @Author: your name
 * @Date: 2020-04-30 13:10:32
 * @LastEditTime: 2020-05-13 16:00:37
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \MobileWrappingMachine_V3\Src\system.c
 */
#include "system.h"

cbusHelperIMPLE(Event, 20);
cbusHelperIMPLE(Msg, 20);
cbusHelperIMPLE(Log, 20);
cBeanHelperIMPL(Config, configLIST);
cBeanHelperIMPL(State, stateLIST);
cmdMenuIMPL(CmdMenu);

/**
 * @brief
 *  当配置或者状态发生改变时候，会触发下面的回调方法
 *
 * @warning
 *  无需注册,状态改变时,会自动进入下面的方法中
 */
cBeanHelperLISTENER_IMPL(Config, onDeviceConfigChange)
{
  Event_t event;
  Event_createConfigChangeEventContent(&event, (ConfigChangeEventContent_t){
                                                   .index = index,
                                                   .oldValue = oldValue,
                                                   .newValue = newValue,
                                               });
  EventBus_fire(&event);
};
cBeanHelperLISTENER_IMPL(State, onDeviceStateChange)
{
  
  Event_t event;
  Event_createStateChangeEventContent(&event, (StateChangeEventContent_t){
                                                  .index = index,
                                                  .oldValue = oldValue,
                                                  .newValue = newValue,
                                              });
  EventBus_fire(&event);
};

int fputc(int c, FILE *stream) //??fputc??
{
  /*
    huart2??????????UART1???,
    ?????????????,?????????????UART????
*/
  HAL_UART_Transmit(&huart1, (unsigned char *)&c, 1, 1000);
  return 1;
}

/**
 * @brief 日志回调,使用SIMPLE_LOGGER_API产生的Log会自动回调到此处
 */
simpleLogger_FORMATER_IMPL()
{
  if (level >= Config_get_logLevel())
  {
    static char logbuff[256];
    int logsize = vsnprintf(logbuff, sizeof(logbuff) - 1, format, ap);
    int logMsgSize = ARRARY_SIZE(((LogMsg_t *)(0))->data);
    //构造和发送Msg_t
    SPLIT_DATA(logbuff, logsize, logMsgSize, {

      if(State_get_initialized())
      {
        LogMsg_t logmsg;
        Log_t msg;
        memcpy(logmsg.data, splitData, splitDataSize);
        logmsg.size = splitDataSize;
        Log_createLogMsg(&msg, logmsg);
        if (isISR)
          LogBus_fireISR(&msg);
        else
          LogBus_fire(&msg);
      }else
      {
        printf("%s", logbuff);
      }

    });
  }
};


/**
 * @brief 指令支持打印输出，将其重定向到我我们的log系统
 */
cmdMenuDEFINE_PRINTF(CmdMenu, {
  static char buf[1024];
  va_list ap;
  va_start(ap, fmt);
  SimpleLogger_vlog(LOGLEVEL_INFO, false, fmt, ap);
  va_end(ap);
});
