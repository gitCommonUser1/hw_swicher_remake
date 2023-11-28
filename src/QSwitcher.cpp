/*
 * Copyright (c) 2015-2017 Alex Spataru <alex_spataru@outlook.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the folstarting conditions:
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

#include <QDebug>
#include <QSettings>
#include <QSwitcher.h>

#include <sys/ioctl.h>
#include <unistd.h>

#include <math.h>

#include "fpga.h"

#define LEDS_DEVICE     "/dev/leds-switcher"


#define IOCTL_LED_OFF           _IOW('c', 0, int)
#define IOCTL_LED_ON            _IOW('c', 1, int)
#define IOCTL_LED_W             _IOW('c', 2, int)
#define IOCTL_LED_R             _IOW('c', 3, int)
#define IOCTL_LED_G             _IOW('c', 4, int)
#define IOCTL_BRIGHTNESS        _IOW('s', 0, unsigned char)
#define IOCTL_BLINK             _IOW('s', 1, unsigned char)
#define IOCTL_GET_LED           _IOR('c', 0, int)
#define IOCTL_GET_BRIGHTNESS    _IOR('s', 0, unsigned char)
#define IOCTL_GET_BLINK         _IOR('s', 1, unsigned char)
#define IOCTL_TEST              _IOW('t', 0, int)


struct led_t {
    uint8_t led;
    SwitcherLed color;
};

static struct led_t g_led[] = {
    {KEY_LED_AUDIO_MIC1, SWITCHER_LED_OFF},
    {KEY_LED_AUDIO_MIC2, SWITCHER_LED_OFF},
    {KEY_LED_AUDIO_IN1, SWITCHER_LED_OFF},
    {KEY_LED_AUDIO_IN2, SWITCHER_LED_OFF},
    {KEY_LED_AUDIO_IN3, SWITCHER_LED_OFF},
    {KEY_LED_AUDIO_IN4, SWITCHER_LED_OFF},
    {KEY_LED_AUDIO_AUX, SWITCHER_LED_OFF},
    {KEY_LED_AUDIO_PGM, SWITCHER_LED_OFF},
    {KEY_LED_AUDIO_AFV, SWITCHER_LED_OFF},
    {KEY_LED_AUDIO_ON, SWITCHER_LED_OFF},
    {KEY_LED_MEM1, SWITCHER_LED_OFF},
    {KEY_LED_MEM2, SWITCHER_LED_OFF},
    {KEY_LED_MEM3, SWITCHER_LED_OFF},
    {KEY_LED_MEM4, SWITCHER_LED_OFF},
    {KEY_LED_MEM5, SWITCHER_LED_OFF},
    {KEY_LED_MEM6, SWITCHER_LED_OFF},
    {KEY_LED_MEM7, SWITCHER_LED_OFF},
    {KEY_LED_MEM8, SWITCHER_LED_OFF},
    {KEY_LED_MEMU, SWITCHER_LED_OFF},
    {KEY_LED_RECORDER_REC, SWITCHER_LED_OFF},		//R
    {KEY_LED_RECORDER_STOP, SWITCHER_LED_OFF},
    {KEY_LED_PLAYER_PREV, SWITCHER_LED_OFF},       //G
    {KEY_LED_PLAYER_NEXT, SWITCHER_LED_OFF},
    {KEY_LED_PLAYER_PLAY, SWITCHER_LED_OFF},
    {KEY_LED_PLAYER_PAUSE, SWITCHER_LED_OFF},
    {KEY_LED_LIVE, SWITCHER_LED_OFF},		//R
    {KEY_LED_KEY_ON_AIR, SWITCHER_LED_OFF},	//R
    {KEY_LED_DSK_ON_AIR, SWITCHER_LED_OFF},	//R
    {KEY_LED_KEY, SWITCHER_LED_OFF},
    {KEY_LED_DSK, SWITCHER_LED_OFF},
    {KEY_LED_BKGD, SWITCHER_LED_OFF},
    {KEY_LED_TRANS_MIX, SWITCHER_LED_OFF},
    {KEY_LED_TRANS_DIP, SWITCHER_LED_OFF},
    {KEY_LED_TRANS_WIPE, SWITCHER_LED_OFF},
    {KEY_LED_TRANS_PREVIEW, SWITCHER_LED_OFF}, 	//R
    {KEY_LED_TRANS_FTB, SWITCHER_LED_OFF},		//R
    {KEY_LED_TRANS_CUT, SWITCHER_LED_OFF},
    {LED_POWER, SWITCHER_LED_OFF},        //G
};

static struct led_t g_led_wr[] = {
    {KEY_LED_TRANS_AUTO, SWITCHER_LED_OFF},
};

static struct led_t g_led_rg[] = {
    {KEY_LED_PVW_1, SWITCHER_LED_OFF},
    {KEY_LED_PVW_2, SWITCHER_LED_OFF},
    {KEY_LED_PVW_3, SWITCHER_LED_OFF},
    {KEY_LED_PVW_4, SWITCHER_LED_OFF},
    {KEY_LED_PVW_AUX, SWITCHER_LED_OFF},
    {KEY_LED_PVW_BLACK, SWITCHER_LED_OFF},
    {KEY_LED_PVW_STLL1, SWITCHER_LED_OFF},
    {KEY_LED_PVW_STLL2, SWITCHER_LED_OFF},
    {KEY_LED_PGM_1, SWITCHER_LED_OFF},
    {KEY_LED_PGM_2, SWITCHER_LED_OFF},
    {KEY_LED_PGM_3, SWITCHER_LED_OFF},
    {KEY_LED_PGM_4, SWITCHER_LED_OFF},
    {KEY_LED_PGM_AUX, SWITCHER_LED_OFF},
    {KEY_LED_PGM_BLACK, SWITCHER_LED_OFF},
    {KEY_LED_PGM_STLL1, SWITCHER_LED_OFF},
    {KEY_LED_PGM_STLL2, SWITCHER_LED_OFF},
    {LED_MAX, SWITCHER_LED_OFF},
    {LED_MIN, SWITCHER_LED_OFF},
};

static QList<struct led_t> g_leds_blink[LEDS_BLINK_NUM];   //单个LED 闪烁

static const uint8_t g_keys_map[] = {
    KEY_LED_KEY_ON_AIR,
    KEY_LED_DSK_ON_AIR,
    KEY_LED_DSK,
    KEY_LED_KEY,
    KEY_LED_PLAYER_NEXT,
    KEY_LED_PLAYER_PAUSE,
    KEY_LED_PLAYER_PLAY,
    KEY_LED_RECORDER_STOP,
    KEY_LED_MEMU,
    KEY_LED_MEM8,
    KEY_LED_MEM5,
    KEY_LED_MEM6,
    KEY_LED_MEM4,
    KEY_LED_PLAYER_PREV,
    KEY_LED_RECORDER_REC,
    KEY_MENU_ENTER,
    KEY_LED_MEM7,
    KEY_LED_AUDIO_ON,
    KEY_LED_AUDIO_AFV,
    KEY_LED_AUDIO_PGM,
    KEY_AUDIO_ENTER,
    KEY_LED_MEM3,
    KEY_LED_MEM2,
    KEY_LED_MEM1,
    KEY_LED_AUDIO_IN3,
    KEY_LED_AUDIO_IN4,
    KEY_LED_AUDIO_AUX,
    KEY_LED_LIVE,
    KEY_LED_AUDIO_MIC2,
    KEY_LED_AUDIO_MIC1,
    KEY_LED_AUDIO_IN2,
    KEY_LED_AUDIO_IN1,
    KEY_LED_PVW_3,
    KEY_LED_PVW_4,
    KEY_LED_PVW_1,
    KEY_LED_PVW_2,
    KEY_LED_PGM_2,
    KEY_LED_PGM_1,
    KEY_LED_PGM_4,
    KEY_LED_PGM_3,
    KEY_LED_PVW_STLL1,
    KEY_LED_PVW_STLL2,
    KEY_LED_PVW_BLACK,
    KEY_LED_PVW_AUX,
    KEY_LED_PGM_BLACK,
    KEY_LED_PGM_AUX,
    KEY_LED_PGM_STLL1,
    KEY_LED_PGM_STLL2,
    KEY_LED_TRANS_WIPE,
    KEY_LED_TRANS_PREVIEW,
    KEY_LED_TRANS_CUT,
    KEY_LED_TRANS_AUTO,
    KEY_LED_BKGD,
    KEY_LED_TRANS_FTB,
    KEY_LED_TRANS_DIP,
    KEY_LED_TRANS_MIX,
};

static const uint8_t g_keys_status_map[] = {
    KEY_LED_TRANS_FTB,
    KEY_LED_DSK,
    KEY_LED_KEY,
    KEY_LED_BKGD,
    KEY_LED_DSK_ON_AIR,
    KEY_LED_KEY_ON_AIR,
    LED_MAX,
    LED_MIN,
    KEY_LED_PGM_STLL2,
    KEY_LED_PGM_STLL1,
    KEY_LED_PGM_BLACK,
    KEY_LED_PGM_AUX,
    KEY_LED_PGM_4,
    KEY_LED_PGM_3,
    KEY_LED_PGM_2,
    KEY_LED_PGM_1,
    KEY_LED_PVW_STLL2,
    KEY_LED_PVW_STLL1,
    KEY_LED_PVW_BLACK,
    KEY_LED_PVW_AUX,
    KEY_LED_PVW_4,
    KEY_LED_PVW_3,
    KEY_LED_PVW_2,
    KEY_LED_PVW_1,
    KEY_LED_TRANS_AUTO,
    KEY_LED_TRANS_CUT,
    KEY_LED_TRANS_PREVIEW,
    KEY_LED_TRANS_WIPE,
    KEY_LED_TRANS_DIP,
    KEY_LED_TRANS_MIX,
};

static uint64_t s_keys_value = 0;

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:   the type of the container struct this is embedded in.
 * @member: the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({          \
    const typeof(((type *)0)->member)*__mptr = (ptr);    \
             (type *)((char *)__mptr - offsetof(type, member)); })

#define find_led(pos, id, ptr)  do { \
    int start = 0; \
    int end = sizeof(ptr) / sizeof((ptr)[0]) - 1; \
    pos = NULL; \
    while(start <= end) { \
        int mid = start + (end - start) / 2; \
        typeof(*(pos)) *_tmp = container_of(&((ptr)[mid]).led, typeof(*(pos)), led); \
        if (_tmp->led == (id)) { \
            pos = _tmp; \
            break; \
        } \
        else if (_tmp->led > (id)) \
            end  = mid - 1; \
        else if (_tmp->led < (id)) \
            start = mid + 1; \
    } \
} while(0)

QSwitcher::QSwitcher(QObject *parent) : QThread(parent)
{
    for(int i = 0; i < LEDS_BLINK_NUM; i++)
    {
        for(int k = 0; k < g_leds_blink[i].size(); k++)
        {
            g_leds_blink[i].removeAt(k);
        }
    }

    running = true;
}

QSwitcher::~QSwitcher()
{
    running = false;

    for(int i = 0; i < LEDS_BLINK_NUM; i++)
    {
        for(int k = 0; k < g_leds_blink[i].size(); k++)
        {
            g_leds_blink[i].removeAt(k);
        }
    }
}

void led_init()
{
    char cmd[64];

    if (access(LEDS_DEVICE, R_OK) == 0)
        return;

    sprintf(cmd, "mknod %s c 76 0", LEDS_DEVICE);

    system(cmd);
}

/**
 * @brief isValidLed 判断led编码是否有效
 * @param led: led编码（参见宏定义）
 * @return
 */
bool QSwitcher::isValidLed(const int led)
{
    struct led_t *p_led;

    find_led(p_led, led, g_led);
    if (p_led)
        return true;

    find_led(p_led, led, g_led_wr);
    if (p_led)
        return true;

    find_led(p_led, led, g_led_rg);
    if (p_led)
        return true;

    return false;
}

/**
 * @brief isValidColor
 * @param led: led编码（参见宏定义）
 * @param color
 * @return
 */
bool QSwitcher::isValidColor(const int led, const int color)
{
    struct led_t *p_led;

    find_led(p_led, led, g_led_wr);
    if (p_led)
    {
        switch (color) {
            case SWITCHER_LED_W:
            case SWITCHER_LED_R:
            case SWITCHER_LED_OFF:
                return true;
                break;
            default:
                return false;
                break;
        }
    }

    find_led(p_led, led, g_led_rg);
    if (p_led)
    {
        switch (color) {
            case SWITCHER_LED_R:
            case SWITCHER_LED_G:
            case SWITCHER_LED_OFF:
                return true;
                break;
            default:
                return false;
                break;
        }
    }

    find_led(p_led, led, g_led);
    if (p_led)
    {
        switch (color) {
            case SWITCHER_LED_ON:
            case SWITCHER_LED_OFF:
                return true;
                break;
            default:
                return false;
                break;
        }
    }

    return false;
}

/**
 * @brief _get_led 获取led当前显示状态
 * @param led: led编码（参见宏定义）
 * @return
 */
int QSwitcher::_get_led(const int led)
{
    struct led_t *p_led;

    find_led(p_led, led, g_led);
    if (p_led)
        return p_led->color;

    find_led(p_led, led, g_led_wr);
    if (p_led)
        return p_led->color;

    find_led(p_led, led, g_led_rg);
    if (p_led)
        return p_led->color;

    return -1;
}

/**
 * @brief get_led 获取led当前状态
 * @param led: led编码（参见宏定义）
 * @return
 */
SwitcherLed QSwitcher::get_led(const int led)
{
    //检查led是否可用
    if (! isValidLed(led))
        return SWITCHER_LED_INVALID;

    //优先从闪烁列表里查找
    for(int i = 0; i < LEDS_BLINK_NUM; i++)
    {
        foreach(auto item, g_leds_blink[i])
        {
            if (item.led == led)
            {
                return (SwitcherLed)(item.color | SWITCHER_LED_BLINK_MASK);
            }
        }
    }

    return (SwitcherLed)_get_led(led);
}

/**
 * @brief _set_led 设置led显示
 * @param led: led编码（参见宏定义）
 * @param color
 * @return
 */
int QSwitcher::_set_led(const int led, const SwitcherLed color)
{
    int ret = 0;
    unsigned long cmd = 0;

    //检查color是否可设置
    if (! isValidColor(led, color))
        return -2;

    led_init();

    //设置
    int fd = open(LEDS_DEVICE, O_RDWR, 0755);
    if (fd < 0)
        return fd;

    switch (color)
    {
    case SWITCHER_LED_OFF:
        cmd = IOCTL_LED_OFF;
        break;
    case SWITCHER_LED_ON:
        cmd = IOCTL_LED_ON;
        break;
    case SWITCHER_LED_W:
        cmd = IOCTL_LED_W;
        break;
    case SWITCHER_LED_R:
        cmd = IOCTL_LED_R;
        break;
    case SWITCHER_LED_G:
        cmd = IOCTL_LED_G;
        break;
    default:
        return -3;
    }

    ret = ioctl(fd, cmd, led);

    close(fd);

    if (ret == 0)
    {
        struct led_t *p_led;

        find_led(p_led, led, g_led);
        if (p_led)
        {
            p_led->color = color;
        }
        else
        {
            find_led(p_led, led, g_led_wr);
            if (p_led)
            {
                p_led->color = color;
            }
            else
            {
                find_led(p_led, led, g_led_rg);
                if (p_led)
                {
                    p_led->color = color;
                }
            }
        }
    }

    return ret;
}

/**
 * @brief set_led 设置led显示
 * @param led: led编码（参见宏定义）
 * @param color
 * @return
 */
int QSwitcher::set_led(const int led, const SwitcherLed color)
{
    int ret = 0;

    //检查color是否可设置
    if (! isValidColor(led, color))
        return -12;

    //若该led在闪烁列表里存在,则删除之
    for(int i = 0; i < LEDS_BLINK_NUM; i++)
    {
        for(int k = 0; k < g_leds_blink[i].size(); k++)
        {
            if (g_leds_blink[i].at(k).led == led)
            {
                g_leds_blink[i].removeAt(k);

                break;
            }
        }
    }

    ret = _set_led(led, color);

    return ret;
}

/**
 * @brief set_led_blink 设置led闪烁
 * @param led: led编码（参见宏定义）
 * @param color
 * @param blink
 * @return
 */
int QSwitcher::set_led(const int led,
                  const SwitcherLed color,
                  const SwitcherBlink blink)
{
    int ret = 0;

    struct led_t item_led;

    //检查led是否可用
    if (! isValidLed(led))
        return -11;

    if (LEDS_BLINK_OFF != blink)
    {
        //闪烁列表里不允许添加关灯
        if (SWITCHER_LED_OFF == color)
            return -12;

        //检查color是否可设置
        if (! isValidColor(led, color))
            return -13;

        item_led.led = led;
        item_led.color = color;
    }

    //若该led在闪烁列表里存在,则删除之
    for(int i = 0; i < LEDS_BLINK_NUM; i++)
    {
        for(int k = 0; k < g_leds_blink[i].size(); k++)
        {
            if (g_leds_blink[i].at(k).led == led)
            {
                g_leds_blink[i].removeAt(k);

                break;
            }
        }
    }

    switch (blink)
    {
    case LEDS_BLINK_2Hz:
        g_leds_blink[LEDS_BLINK_2Hz - 1].append(item_led);
        break;
    case LEDS_BLINK_1Hz:
        g_leds_blink[LEDS_BLINK_1Hz - 1].append(item_led);
        break;
    case LEDS_BLINK_0_5Hz:
        g_leds_blink[LEDS_BLINK_0_5Hz - 1].append(item_led);
        break;
    case LEDS_BLINK_OFF:
        ret = _set_led(led, color);
        break;
    default:
        break;
    }

    return ret;
}

/**
 * @brief get_leds_deep 获取led当前底层状态(供测试使用)
 * @return
 */
int QSwitcher::get_leds_deep()
{
    int ret = 0;
    uint8_t ram[48];

    memset(ram, 0, sizeof(ram));

    led_init();

    int fd = open(LEDS_DEVICE, O_RDWR, 0755);
    if (fd < 0)
        return fd;

    ret = ioctl(fd, IOCTL_GET_LED, &ram);

    close(fd);

    printf("%s:\n", __func__);
    for(int i = 0; i < 48; i++)
    {
        printf("%.2X ", ram[i]);

        if ((i & 0x0F) == 15)
            printf("\n");
    }

    return ret;
}

/**
 * @brief get_leds_brightness 获取led当前亮度
 * @return
 */
int QSwitcher::get_leds_brightness()
{
    int ret = 0;
    int brightness = 0;

    led_init();

    int fd = open(LEDS_DEVICE, O_RDWR, 0755);
    if (fd < 0)
        return fd;

    ret = ioctl(fd, IOCTL_GET_BRIGHTNESS, &brightness);

    close(fd);

    if (ret < 0)
        return ret;

    return brightness;
}

/**
 * @brief set_leds_brightness 设置led亮度
 * @param brightness 0~15
 * @return
 */
int QSwitcher::set_leds_brightness(const int brightness)
{
    int ret = 0;

    led_init();

    int fd = open(LEDS_DEVICE, O_RDWR, 0755);
    if (fd < 0)
        return fd;

    ret = ioctl(fd, IOCTL_BRIGHTNESS, brightness);

    close(fd);

    return ret;
}

/**
 * @brief get_leds_blink 获取led当前闪烁状态
 * @return
 */
int QSwitcher::get_leds_blink()
{
    int ret = 0;
    int blink = 0;

    led_init();

    int fd = open(LEDS_DEVICE, O_RDWR, 0755);
    if (fd < 0)
        return fd;

    ret = ioctl(fd, IOCTL_GET_BLINK, &blink);

    close(fd);

    if (ret < 0)
        return ret;

    return blink;
}

/**
 * @brief set_leds_blink 设置led闪烁
 * @param blink
 * @return
 */
int QSwitcher::set_leds_blink(const SwitcherBlink blink)
{
    int ret = 0;

    led_init();

    int fd = open(LEDS_DEVICE, O_RDWR, 0755);
    if (fd < 0)
        return fd;

    ret = ioctl(fd, IOCTL_BLINK, blink);

    close(fd);

    return ret;
}

/**
 * @brief set_leds_test: 设置led测试模式
 * @param test: 1: 点亮所有led; 0: 熄灭所有led
 * @return
 */
int QSwitcher::set_leds_test(const int test)
{
    int ret = 0;

    led_init();

    int fd = open(LEDS_DEVICE, O_RDWR, 0755);
    if (fd < 0)
        return fd;

    ret = ioctl(fd, IOCTL_TEST, test);

    close(fd);

    return ret;
}

void QSwitcher::knob_process()
{
    const int knobs[] = {KNOB_MENU, KNOB_AUDIO};
    const int knobs_num = sizeof(knobs) / 4;

    uint16_t knob_value = 0;
    struct fpga_knob_t *p_knob = (struct fpga_knob_t *)&knob_value;
    uint8_t *p_data[] = {&p_knob->menu, &p_knob->audio};

    static int s_value[] = {-1, -1};

    int ret = 0;
    int diff = 0;

    static struct timespec ts_last = {-1, -1};
    struct timespec ts_cur;

    clock_gettime(CLOCK_BOOTTIME, &ts_cur);

    __time_t time_diff = (ts_cur.tv_sec - ts_last.tv_sec) * 1000 + (ts_cur.tv_nsec - ts_last.tv_nsec) / 1000000;

    if (time_diff < 50)
        return;

    //获取
    ret = fpga_read(&g_fpga, FPGA_KNOB_VAL, &knob_value);
    if (ret < 0)
        return;

    for(int i = 0; i < knobs_num; i++)
    {
        if ((s_value[i] >= 0)
                && (*p_data[i] != s_value[i]))
        {
            diff = *p_data[i] - s_value[i];

            if (diff != 0)
            {
                if (diff > 128)
                    diff -= 256;
                else if (diff < -128)
                    diff += 256;

                if (diff >= 11 || diff <= -11)
                    diff *= 32;
                else if (diff >= 9 || diff <= -9)
                    diff *= 16;
                else if (diff >= 7 || diff <= -7)
                    diff *= 8;
                else if (diff >= 5 || diff <= -5)
                    diff *= 4;
                else if (diff >= 3 || diff <= -3)
                    diff *= 2;

                emit knobChanged(knobs[i], diff);

                struct timespec ts;
                clock_gettime(CLOCK_BOOTTIME, &ts);
                printf("[%d.%.9d] Knob: 0x%.2X  %d [%d]\n", ts.tv_sec, ts.tv_nsec, knobs[i], diff, *p_data[i]);
            }
        }

        s_value[i] = *p_data[i];
    }

    ts_last.tv_sec = ts_cur.tv_sec;
    ts_last.tv_nsec = ts_cur.tv_nsec;
}

/**
 * @brief get_push_value 获取推杆当前值
 * @return 0~256
 */
int QSwitcher::get_push_value()
{
#define PUSH_ADC_RAW    "/sys/devices/platform/ff5e0000.saradc/iio:device0/in_voltage3_raw"

    int fd = -1;
    char buf[8];
    int r_size = 0;

    int value_raw = 0;
    int value = 0;

    if (access(PUSH_ADC_RAW, R_OK) != 0)
        return -1;

    fd = open(PUSH_ADC_RAW, O_RDONLY);
    if (fd < 0)
    {
        printf("%s: open %s error(%d - %s)\n", __func__, PUSH_ADC_RAW, errno, strerror(errno));

        return -2;
    }

    r_size = read(fd, buf, 8);

    if (r_size <= 0)
    {
        printf("%s: read %s error(%d - %s)\n", __func__, PUSH_ADC_RAW, errno, strerror(errno));

        close(fd);
        return -3;
    }
    buf[r_size] = '\0';

    close(fd);

    if (sscanf(buf, "%d", &value_raw) != 1)
    {
        return -4;
    }

    //转换到0~256
    static int s_value = 0;
    static int8_t direction = 0;

    static struct timespec ts_last = {-1, -1};
    struct timespec ts_cur;

    int diff = value_raw - s_value;

    if (diff <= 8 && diff >= -8)
    {
        if ((direction > 0 && diff > 0)
                || (direction < 0 && diff < 0))
        {
            s_value = value_raw;

            clock_gettime(CLOCK_BOOTTIME, &ts_last);
        }
        else
        {
            value_raw = s_value;

            clock_gettime(CLOCK_BOOTTIME, &ts_cur);

            __time_t time_diff = (ts_cur.tv_sec - ts_last.tv_sec) * 1000 + (ts_cur.tv_nsec - ts_last.tv_nsec) / 1000000;

            if (time_diff > 50)
            {
                direction = 0;
            }
        }
    }
    else
    {
        s_value = value_raw;

        if (diff > 0)
            direction = 1;
        else
            direction = -1;
    }

    if (value_raw <= 16)
    {
        value = 0;
    }
    else if (value_raw >= 1008)
    {
        value = 256;
    }
    else
    {
        value = (int)((float)(value_raw - 16) * 255.0 / 992.0 + 0.5 + 1);
    }

    static int s_v = -1;
    if (s_v != value)
    {
        s_v = value;

        //printf("%s: value_raw:%d value:%d\n", __func__, value_raw, value);
    }

    return value;
}

void QSwitcher::push_process()
{
    int value = 0;

    static int s_value = -1;

    static struct timespec ts_last = {-1, -1};
    struct timespec ts_cur;

    clock_gettime(CLOCK_BOOTTIME, &ts_cur);

    __time_t time_diff = (ts_cur.tv_sec - ts_last.tv_sec) * 1000 + (ts_cur.tv_nsec - ts_last.tv_nsec) / 1000000;

    if (time_diff < 10)
        return;

    //获取
    value = get_push_value();

    if (s_value != value)
    {
        emit pushChanged(PUSH_ROD, value);

        fpga_write(&g_fpga, FPGA_T_BAR, value);

        struct timespec ts;
        clock_gettime(CLOCK_BOOTTIME, &ts);
        printf("[%d.%.9d] PUSH_ROD: %d\n", ts.tv_sec, ts.tv_nsec, value);

        s_value = value;
    }

    ts_last.tv_sec = ts_cur.tv_sec;
    ts_last.tv_nsec = ts_cur.tv_nsec;
}

void QSwitcher::key_process()
{
    const int keys_num = sizeof(g_keys_map) / sizeof(g_keys_map[0]);

    uint64_t value = 0;
    uint64_t diff = 0;

    int ret = 0;
    uint16_t tmp;

    static struct timespec ts_last = {-1, -1};
    struct timespec ts_cur;

    clock_gettime(CLOCK_BOOTTIME, &ts_cur);

    __time_t time_diff = (ts_cur.tv_sec - ts_last.tv_sec) * 1000 + (ts_cur.tv_nsec - ts_last.tv_nsec) / 1000000;

    if (time_diff < 100)
        return;

    //获取
    value = 0;

    ret = fpga_read(&g_fpga, FPGA_KEY_STATUS0, &tmp);
    struct fpga_key0_t *p_key0 = (struct fpga_key0_t *)&tmp;
    value |= ((uint64_t)p_key0->key) << 48;

    ret = fpga_read(&g_fpga, FPGA_KEY_STATUS1, &tmp);
    value |= ((uint64_t)tmp) << 32;

    ret = fpga_read(&g_fpga, FPGA_KEY_STATUS2, &tmp);
    value |= ((uint64_t)tmp) << 16;

    ret = fpga_read(&g_fpga, FPGA_KEY_STATUS3, &tmp);
    value |= (uint64_t)tmp;

    ts_last.tv_sec = ts_cur.tv_sec;
    ts_last.tv_nsec = ts_cur.tv_nsec;

    diff = value ^ s_keys_value;
    if (diff == 0)
        return;

    for(int i = 0; i < keys_num; i++)
    {
        if (diff & ((uint64_t)1 << i))
        {
            emit keyChanged(g_keys_map[i], (value & ((uint64_t)1 << i)) ? 1 : 0);

            struct timespec ts;
            clock_gettime(CLOCK_BOOTTIME, &ts);
            printf("[%d.%.9d] Key: 0x%.2X - %d\n", ts.tv_sec, ts.tv_nsec, g_keys_map[i], (value & ((uint64_t)1 << i)) ? 1 : 0);
        }
    }

    s_keys_value = value;
}

SwitcherLedStatus QSwitcher::get_led_status(const int led)
{
    const int keys_num = sizeof(g_keys_status_map) / sizeof (g_keys_status_map[0]);
    int i = 0;

    for(i = 0; i < keys_num; i++)
    {
        if (led == g_keys_status_map[i])
            break;
    }

    if (i >= keys_num)
        return LED_STATUS_INVALID;

    SwitcherLed color = get_led(led);

    switch ((int)color)
    {
    case SWITCHER_LED_R:
    case SWITCHER_LED_R_BLINK:
        return LED_STATUS_RED;
        break;
    case SWITCHER_LED_ON:
    case SWITCHER_LED_W:
    case SWITCHER_LED_G:
        return LED_STATUS_NORMAL;
        break;
    case SWITCHER_LED_ON_BLINK:
    case SWITCHER_LED_W_BLINK:
    case SWITCHER_LED_G_BLINK:
        return LED_STATUS_FLASH;
        break;
    case SWITCHER_LED_OFF:
        return LED_STATUS_OFF;
        break;
    }

    return LED_STATUS_INVALID;
}

void QSwitcher::led_status_process()
{
    const int keys_num = sizeof(g_keys_status_map) / sizeof(g_keys_status_map[0]);

    static uint64_t s_value = 0;

    uint64_t value = 0;
    uint64_t diff = 0;
    int status = 0;
    struct led_t *p_led;

    int ret = 0;
    uint16_t tmp;

    static struct timespec ts_last = {-1, -1};
    struct timespec ts_cur;

    clock_gettime(CLOCK_BOOTTIME, &ts_cur);

    __time_t time_diff = (ts_cur.tv_sec - ts_last.tv_sec) * 1000 + (ts_cur.tv_nsec - ts_last.tv_nsec) / 1000000;

    if (time_diff < 100)
        return;

    //获取
    value = 0;

    ret = fpga_read(&g_fpga, FPGA_LED_STATUS0, &tmp);
    value |= ((uint64_t)tmp) << 48;

    ret = fpga_read(&g_fpga, FPGA_LED_STATUS1, &tmp);
    value |= ((uint64_t)tmp) << 32;

    ret = fpga_read(&g_fpga, FPGA_LED_STATUS2, &tmp);
    value |= ((uint64_t)tmp) << 16;

    ret = fpga_read(&g_fpga, FPGA_LED_STATUS3, &tmp);
    value |= (uint64_t)tmp;

    ts_last.tv_sec = ts_cur.tv_sec;
    ts_last.tv_nsec = ts_cur.tv_nsec;

    diff = value ^ s_value;
    if (diff == 0)
        return;

    for(int i = 0; i < keys_num; i++)
    {
        if (diff & ((uint64_t)3 << (i * 2)))
        {
            status = (value >> (i * 2)) & 3;
            switch (status)
            {
            case LED_STATUS_OFF:
                set_led(g_keys_status_map[i], SWITCHER_LED_OFF);
                break;
            case LED_STATUS_NORMAL:
                //SWITCHER_LED_W
                find_led(p_led, g_keys_status_map[i], g_led_wr);
                if (p_led)
                {
                    set_led(g_keys_status_map[i], SWITCHER_LED_W);
                    break;
                }

                //SWITCHER_LED_G
                find_led(p_led, g_keys_status_map[i], g_led_rg);
                if (p_led)
                {
                    set_led(g_keys_status_map[i], SWITCHER_LED_G);
                    break;
                }

                //SWITCHER_LED_ON
                find_led(p_led, g_keys_status_map[i], g_led);
                if (p_led)
                {
                    set_led(g_keys_status_map[i], SWITCHER_LED_ON);
                    break;
                }
                break;
            case LED_STATUS_RED: //SWITCHER_LED_R
                set_led(g_keys_status_map[i], SWITCHER_LED_R);
                break;
            case LED_STATUS_FLASH: //SWITCHER_LED_ON
                set_led(g_keys_status_map[i], SWITCHER_LED_ON, LEDS_BLINK_2Hz);
                break;
            }

            emit keyStatusChanged(g_keys_status_map[i], status);

            struct timespec ts;
            clock_gettime(CLOCK_BOOTTIME, &ts);
            printf("[%d.%.9d] Led: 0x%.2X - %d\n", ts.tv_sec, ts.tv_nsec, g_keys_status_map[i], status);
        }
    }

    s_value = value;
}

void QSwitcher::led_blink_process()
{
    static struct timespec ts_last[LEDS_BLINK_NUM] = {{0, 0}, {0, 0}, {0, 0}};
    struct timespec ts_cur;

    static bool blink_led_off[LEDS_BLINK_NUM] = {false, false, false};

    const __time_t time_diff[LEDS_BLINK_NUM] = { //ms
        250,
        500,
        1000,
    };

    clock_gettime(CLOCK_BOOTTIME, &ts_cur);
    for(int i = 0; i < LEDS_BLINK_NUM; i++)
    {
        __time_t _diff = (ts_cur.tv_sec - ts_last[i].tv_sec) * 1000 + (ts_cur.tv_nsec - ts_last[i].tv_nsec) / 1000000;

        if (_diff < time_diff[i])
            continue;

        foreach(auto item, g_leds_blink[i])
        {
            if (blink_led_off[i] == true)
            {
                _set_led(item.led, SWITCHER_LED_OFF);
            }
            else
            {
                _set_led(item.led, item.color);
            }
        }

        blink_led_off[i] = ! blink_led_off[i];

        ts_last[i].tv_sec = ts_cur.tv_sec;
        ts_last[i].tv_nsec = ts_cur.tv_nsec;
    }
}

int QSwitcher::set_softkey(const int key, const int value)
{
    const int keys_num = sizeof(g_keys_map) / sizeof(g_keys_map[0]);

    int i = 0;

    int ret = 0;
    uint16_t tmp;
    uint64_t tmp_value = 0;

    for(i = 0; i < keys_num; i++)
    {
        if (key == g_keys_map[i])
        {
            break;
        }
    }

    if (i >= keys_num)
        return -1;

    tmp_value = s_keys_value;

    if (value)
    {
        if (tmp_value & ((uint64_t)1 << i))
            return 0;

        tmp_value |= (uint64_t)1 << i;
    }
    else
    {
        if (! (tmp_value & ((uint64_t)1 << i)))
            return 0;

        tmp_value &= ~((uint64_t)1 << i);
    }

    if (i >= 48)
    {
        tmp = tmp_value >> 48;
        ret = fpga_write(&g_fpga, FPGA_KEY_STATUS0, tmp);
    }
    else if (i >= 32)
    {
        tmp = tmp_value >> 32;
        ret = fpga_write(&g_fpga, FPGA_KEY_STATUS1, tmp);
    }
    else if (i >= 16)
    {
        tmp = tmp_value >> 16;
        ret = fpga_write(&g_fpga, FPGA_KEY_STATUS2, tmp);
    }
    else
    {
        tmp = tmp_value & 0xFFFF;
        ret = fpga_write(&g_fpga, FPGA_KEY_STATUS3, tmp);
    }

    if (ret > 0)
    {
        ret = 0;
        s_keys_value = tmp_value;
    }

    return ret;
}

void QSwitcher::setStop()
{
    running = false;
}

void QSwitcher::run()
{
    printf("pthread:%ld start...\n", pthread_self());

    led_init();
    set_leds_blink(LEDS_BLINK_OFF);

    fpga_init(&g_fpga);

    while(running)
    {
        knob_process();
        push_process();
        key_process();
        led_status_process();
        led_blink_process();

        msleep(1);
    }

QSwitcher_error:
    fpga_close(&g_fpga);

    pthread_detach(pthread_self());
    pthread_exit(NULL);
}
