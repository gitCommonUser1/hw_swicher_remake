/*
 * Copyright (c) 2015-2017 Alex Spataru <alex_spataru@outlook.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef _QSwitcher_MAIN_H
#define _QSwitcher_MAIN_H

#include <QObject>
#include <QThread>

#include <QSwitcher/QSwitcherCommon.h>


//Audio
#define KEY_LED_AUDIO_MIC1  0x00
#define KEY_LED_AUDIO_MIC2  0x01
#define KEY_LED_AUDIO_IN1   0x02
#define KEY_LED_AUDIO_IN2   0x03
#define KEY_LED_AUDIO_IN3   0x04
#define KEY_LED_AUDIO_IN4   0x05
#define KEY_LED_AUDIO_AUX   0x06
#define KEY_LED_AUDIO_PGM   0x07
#define KEY_LED_AUDIO_AFV   0x08
#define KEY_LED_AUDIO_ON    0x09
#define KEY_AUDIO_ENTER     0x0A
#define KNOB_AUDIO          0x0B

//MACRO
#define KEY_LED_MEM1        0x10
#define KEY_LED_MEM2        0x11
#define KEY_LED_MEM3        0x12
#define KEY_LED_MEM4        0x13
#define KEY_LED_MEM5        0x14
#define KEY_LED_MEM6        0x15
#define KEY_LED_MEM7        0x16
#define KEY_LED_MEM8        0x17

//Menu
#define KEY_LED_MEMU        0x20
#define KEY_MENU_ENTER      0x21
#define KNOB_MENU           0x22

//Media Recorder
#define KEY_LED_RECORDER_REC    0x30
#define KEY_LED_RECORDER_STOP   0x31
#define KEY_LED_PLAYER_PREV     0x32
#define KEY_LED_PLAYER_NEXT     0x33
#define KEY_LED_PLAYER_PLAY     0x34
#define KEY_LED_PLAYER_PAUSE    0x35


//Streaming
#define KEY_LED_LIVE        0x40

//Next Transition
#define KEY_LED_KEY_ON_AIR    0x50
#define KEY_LED_DSK_ON_AIR    0x51
#define KEY_LED_KEY         0x52
#define KEY_LED_DSK         0x53
#define KEY_LED_BKGD        0x54

//PST
#define KEY_LED_PVW_1       0x60
#define KEY_LED_PVW_2       0x61
#define KEY_LED_PVW_3       0x62
#define KEY_LED_PVW_4       0x63
#define KEY_LED_PVW_AUX     0x64
#define KEY_LED_PVW_BLACK   0x65
#define KEY_LED_PVW_STLL1   0x66
#define KEY_LED_PVW_STLL2   0x67

//PGM
#define KEY_LED_PGM_1       0x70
#define KEY_LED_PGM_2       0x71
#define KEY_LED_PGM_3       0x72
#define KEY_LED_PGM_4       0x73
#define KEY_LED_PGM_AUX     0x74
#define KEY_LED_PGM_BLACK   0x75
#define KEY_LED_PGM_STLL1   0x76
#define KEY_LED_PGM_STLL2   0x77

//Transition Style
#define KEY_LED_TRANS_MIX         0x80
#define KEY_LED_TRANS_DIP         0x81
#define KEY_LED_TRANS_WIPE        0x82
#define KEY_LED_TRANS_PREVIEW     0x83
#define KEY_LED_TRANS_FTB         0x84
#define KEY_LED_TRANS_AUTO        0x85
#define KEY_LED_TRANS_CUT         0x86

//Push Rod
#define PUSH_ROD            0x90	//推杆
#define LED_MAX        		0x91
#define LED_MIN       	 	0x92

//Power
#define LED_POWER			0xFF


enum SwitcherLed {
#define SWITCHER_LED_BLINK_MASK     0x80
    SWITCHER_LED_OFF = 0,
    SWITCHER_LED_ON,
    SWITCHER_LED_W,
    SWITCHER_LED_R,
    SWITCHER_LED_G,

    SWITCHER_LED_ON_BLINK = (SWITCHER_LED_BLINK_MASK | SWITCHER_LED_ON),
    SWITCHER_LED_W_BLINK,
    SWITCHER_LED_R_BLINK,
    SWITCHER_LED_G_BLINK,

    SWITCHER_LED_INVALID = -1,
};

#define LEDS_BLINK_NUM  3

enum SwitcherBlink {
    LEDS_BLINK_OFF = 0, //关闭闪烁功能
    LEDS_BLINK_2Hz,     //LED 按 2Hz 的频率闪烁
    LEDS_BLINK_1Hz,     //LED 按 1Hz 的频率闪烁
    LEDS_BLINK_0_5Hz,   //LED 按 0.5Hz 的频率闪烁
};

enum SwitcherLedStatus {
    LED_STATUS_OFF = 0,
    LED_STATUS_NORMAL,
    LED_STATUS_RED,
    LED_STATUS_FLASH,

    LED_STATUS_INVALID = -1,
};

class QSwitcher : public QThread
{
   Q_OBJECT

   //friend class Test_QSwitcher;

signals:
   void knobEvent(const QSwitcherKnobEvent &event);
   void pushEvent(const QSwitcherPushEvent &event);
   void keyEvent(const QSwitcherKeyEvent &event);

   /**
    * @brief knobChanged 旋钮发生变化信号
    * @param knob: 旋钮编码（参见宏定义）
    * @param value: ..., -2, -1, +1, +2,...
    */
   void knobChanged(const int knob, const int value); //旋转编码器

   /**
    * @brief pushChanged 推杆发生变化信号
    * @param push: 推杆编码（参见宏定义）
    * @param value: 0~256
    */
   void pushChanged(const int push, const int value); //推杆

   /**
    * @brief keyChanged 按键发生变化信号
    * @param key: 按键编码（参见宏定义）
    * @param value: 0: 释放; 1: 按下
    */
   void keyChanged(const int key, const int value); //按键

   /**
    * @brief keyStatusChanged
    * @param key
    * @param status: 参见SwitcherLedStatus
    */
   void keyStatusChanged(const int key, const int status); //按键灯状态

public:
   QSwitcher(QObject *parent = 0);
   ~QSwitcher();

   void run();

   /**
    * @brief setStop 退出键盘扫描线程
    */
   void setStop();

   /**
    * @brief isValidLed 判断led编码是否有效
    * @param led: led编码（参见宏定义）
    * @return
    */
   static bool isValidLed(const int led);

   /**
    * @brief isValidColor
    * @param led: led编码（参见宏定义）
    * @param color
    * @return
    */
   static bool isValidColor(const int led, const int color);

   /**
    * @brief get_led 获取led当前状态
    * @param led: led编码（参见宏定义）
    * @return: 参见SwitcherLed
    */
   Q_INVOKABLE static SwitcherLed get_led(const int led);

   /**
    * @brief set_led 设置led显示
    * @param led: led编码（参见宏定义）
    * @param color
    * @return
    */
   Q_INVOKABLE static int set_led(const int led,
                                  const SwitcherLed color);

   /**
    * @brief set_led_blink 设置led闪烁
    * @param led: led编码（参见宏定义）
    * @param color
    * @param blink
    * @return
    */
   Q_INVOKABLE static int set_led(const int led,
                                        const SwitcherLed color,
                                        const SwitcherBlink blink);

   /**
    * @brief get_leds_deep 获取led当前底层状态(供测试使用)
    * @return
    */
   Q_INVOKABLE static int get_leds_deep();

   /**
    * @brief get_leds_brightness 获取led当前亮度（全局）
    * @return
    */
   Q_INVOKABLE static int get_leds_brightness();

   /**
    * @brief set_leds_brightness 设置led亮度（全局）
    * @param brightness 0~15
    * @return
    */
   Q_INVOKABLE static int set_leds_brightness(const int brightness);

   /**
    * @brief get_leds_blink 获取led当前闪烁状态（全局）
    * @return
    */
   Q_INVOKABLE static int get_leds_blink();

   /**
    * @brief set_led_blink 设置led闪烁（全局）
    * @param blink
    * @return
    */
   Q_INVOKABLE static int set_leds_blink(const SwitcherBlink blink);

   /**
    * @brief set_leds_test: 设置led测试模式
    * @param test: 1: 点亮所有led; 0: 熄灭所有led
    * @return
    */
   Q_INVOKABLE static int set_leds_test(const int test);

   /**
    * @brief get_push_value 获取推杆当前值
    * @return 0~256
    */
   Q_INVOKABLE static int get_push_value();

   /**
    * @brief get_led_status
    * @param led
    * @return: 参见SwitcherLedStatus
    */
   Q_INVOKABLE static SwitcherLedStatus get_led_status(const int led);

   /**
    * @brief set_softkey
    * @param key: 按键编码（参见宏定义）
    * @param value: 0: 释放; 1: 按下
    * @return
    */
   Q_INVOKABLE static int set_softkey(const int key, const int value);

private:
   bool running;

   /**
    * @brief _get_led 获取led当前显示状态
    * @param led: led编码（参见宏定义）
    * @return
    */
   static int _get_led(const int led);

   /**
    * @brief _set_led 设置led显示
    * @param led: led编码（参见宏定义）
    * @param color
    * @return
    */
   static int _set_led(const int led, const SwitcherLed color);

   void knob_process();
   void push_process();
   void key_process();
   void led_status_process();
   void led_blink_process();
};

#endif
