#ifndef _GPIO_H
#define _GPIO_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <pthread.h>

#include "osee_errors.h"
#include "osee_syslog.h"

#include "general.h"

#ifdef __cplusplus
extern "C" {
#endif


#define GPIO_EXPORT    "/sys/class/gpio/export"
#define GPIO_UNEXPORT  "/sys/class/gpio/unexport"

#define GPIO_BASENAME  "/sys/class/gpio/gpio"


enum gpio_direction_t {
    GPIO_IN,
    GPIO_OUT,
};


/**
 * @brief gpio_int: gpio初始化
 * @param gpios: gpio号: 0 ~ 159
 * @param dir: gpio方向
 * @return 0: 成功
 *          <0 错误
 */
static int gpio_int(const int gpios, const enum gpio_direction_t dir)
{
    int ret = 0;

    char cmd[256];
    char name[32];

    if (gpios >= 160)
        return -ERROR_OUTRANGE;

    sprintf(name, "%s%d", GPIO_BASENAME, gpios);

    if (access(name, R_OK) != 0)
    {
        sprintf(cmd, "echo %d > %s", gpios, GPIO_EXPORT);
        ret = system(cmd);
        if (ret != 0)
        {
            return -ERROR_EXEC;
        }
    }

    switch (dir)
    {
    case GPIO_IN:
        sprintf(cmd, "test `cat %s/direction` != \"in\n\" && echo in > %s/direction", name, name);
        break;
    case GPIO_OUT:
        sprintf(cmd, "test `cat %s/direction` != \"out\n\" && echo out > %s/direction", name, name);
        break;
    default:
        return -ERROR_OUTRANGE;
        break;
    }

    ret = system(cmd);
    if (ret != 0)
    {
        return -ERROR_EXEC;
    }

    return  0;
}

/**
 * @brief gpio_exit: gpio释放
 * @param gpios: gpio号: 0 ~ 159
 * @return 0: 成功
 *          <0 错误
 */
static int gpio_exit(const int gpios)
{
    int ret = 0;

    char cmd[256];

    if (gpios >= 160)
        return -ERROR_OUTRANGE;

    sprintf(cmd, "test -d %s%d && echo %d > %s", GPIO_BASENAME, gpios, gpios, GPIO_UNEXPORT);
    ret = system(cmd);
    if (ret != 0)
    {
        return -ERROR_EXEC;
    }

    return 0;
}

/**
 * @brief get_gpio_value: 获取输入GPIO的高低电平状态
 * @param gpios: gpio号
 * @return 0, 1 具体对应高低电平需要参考电路设计
 *          <0 错误
 */
static int get_gpio_value(const int gpios)
{
    int ret = 0;
    char cmd[64];
    char result[64];

    /*ret = gpio_int(gpios, GPIO_IN);
    if (ret != 0)
    {
        return ret;
    }*/

    sprintf(cmd, "cat %s%d/value", GPIO_BASENAME, gpios);
    ret = executeCMD(cmd, result, 64);
    if (ret != 0)
        return ret;

    uln(result);

    int data = atoi(result);

    return data;
}

/**
 * @brief set_gpio_value: 设置输出GPIO的高低电平状态
 * @param gpios: gpio号
 * @param value: 0, 1 具体对应高低电平需要参考电路设计
 * @return 0: 成功
 *          <0 错误
 */
static int set_gpio_value(const int gpios, const int value)
{
    int ret = 0;
    char cmd[64];

    /*ret = gpio_int(gpios, GPIO_OUT);
    if (ret != 0)
    {
        return ret;
    }*/

    sprintf(cmd, "echo %d > %s%d/value", value, GPIO_BASENAME, gpios);
    ret = system(cmd);
    if (ret != 0)
        return -ERROR_EXEC;

    return 0;
}

#ifdef __cplusplus
}
#endif

#endif //_GPIO_H
