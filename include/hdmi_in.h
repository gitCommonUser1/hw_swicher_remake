#ifndef _HDMI_IN_H
#define _HDMI_IN_H

#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

#define HDMI_IN_NUM    4

enum hdmi_in_colorspace_t {
    INPUT_COLORSPACE_AUTO = 0,
    INPUT_COLORSPACE_RGB_FULL,
    INPUT_COLORSPACE_RGB_LIMITED,
    INPUT_COLORSPACE_YCC422_FULL,
    INPUT_COLORSPACE_YCC422_LIMITED,
    INPUT_COLORSPACE_YCC444_FULL,
    INPUT_COLORSPACE_YCC444_LIMITED,
};


/**
 * @brief set_hdmi_in_colorspace
 * @param id
 * @param format
 * @return
 */
int set_hdmi_in_colorspace(uint8_t id, enum hdmi_in_colorspace_t format);

#ifdef __cplusplus
 }
#endif

#endif //_HDMI_IN_H
