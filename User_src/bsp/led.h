/*
 * @Author: your name
 * @Date: 2020-05-09 17:49:30
 * @LastEditTime: 2020-06-06 12:26:58
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \MobileWrappingMachine_V3\Src\bsp\led.h
 */
void Led_init();

void Led_showLed0(bool open) ;
void Led_showLed1(bool open) ;
void Led_showLed2(bool open) ;
void Led_showLed3(bool open) ;
void Led_showRGB_R(bool open);
void Led_showRGB_G(bool open);
void Led_showRGB_B(bool open);
void Led_closeLed();
void Led_closeRGB();