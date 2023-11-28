#include <stdint.h>
#include <stdio.h>

#include "gpio.h"
#include "uvc_out_power.h"


int set_uvc_out_power(const uint8_t power)
{
    return set_gpio_value(GPIOS_UVC_OUT_POWER, power ? 1 : 0);
}

int uvc_out_power_init()
{
    int ret = 0;

    ret = gpio_int(GPIOS_UVC_OUT_POWER, GPIO_OUT);
    //ret = set_uvc_out_power(0);

    return ret;
}

int uvc_out_power_exit()
{
    return gpio_exit(GPIOS_UVC_OUT_POWER);
}
