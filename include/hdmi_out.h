#ifndef _HDMI_OUT_H
#define _HDMI_OUT_H

#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

#define HDMI_OUT_NUM    2

enum hdmi_out_colorspace_t {
    OUTPUT_COLORSPACE_AUTO = 0,
    OUTPUT_COLORSPACE_RGB_FULL,
    OUTPUT_COLORSPACE_RGB_LIMITED,
    OUTPUT_COLORSPACE_YCC422_FULL,
    OUTPUT_COLORSPACE_YCC422_LIMITED,
    OUTPUT_COLORSPACE_YCC444_FULL,
    OUTPUT_COLORSPACE_YCC444_LIMITED,
};


/**
 * @brief set_hdmi_out_format
 * @param id
 * @param format
 * @return
 */
int set_hdmi_out_format(uint8_t id, int format);

/**
 * @brief set_hdmi_out_colorspace
 * @param id
 * @param format
 * @return
 */
int set_hdmi_out_colorspace(uint8_t id, enum hdmi_out_colorspace_t format);

#ifdef __cplusplus
 }
#endif

#endif //_HDMI_OUT_H
