#ifndef _UVC_OUT_POWER_H
#define _UVC_OUT_POWER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "gpio.h"


#define GPIOS_UVC_OUT_POWER   5


/**
 * @brief set_uvc_out_power:
 * @param led: 0： 对外不供电; 1： 对外供电
 * @return 0: 成功
 *          <0 错误
 */
int set_uvc_out_power(const uint8_t power);

/**
 * @brief led_init
 * @return 0: 成功
 *          <0 错误
 */
int uvc_out_power_init();

/**
 * @brief uvc_out_power_exit
 * @return 0: 成功
 *          <0 错误
 */
int uvc_out_power_exit();


#ifdef __cplusplus
}
#endif

#endif //_UVC_OUT_POWER_H
