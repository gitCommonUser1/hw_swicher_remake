#ifndef _UVC_PORTS_H
#define _UVC_PORTS_H


#ifdef __cplusplus
extern "C" {
#endif

enum uvc_out_mode_t {
    UVC_OUT_OTG = 0,
    UVC_OUT_HOST,
};


/**
 * @brief uvc_out_is_otg_mode 获取uvc_out是否处于otg模式
 * @return
 */
int uvc_out_is_otg_mode();

/**
 * @brief uvc_out_is_otg_online uvc_out在otg模式下是否在线
 * @return
 */
int uvc_out_is_otg_online();

/**
 * @brief set_uvc_out_mode 设置uvc_out的工作模式（otg/host）
 * @param mode
 * @return
 */
int set_uvc_out_mode(enum uvc_out_mode_t mode);

/**
 * @brief get_uvc_in_netif 获取uvc_in接入的网卡名称
 * @return
 */
char *get_uvc_in_netif();

/**
 * @brief get_uvc_out_netif 获取uvc_out接入的网卡名称
 * @return
 */
char *get_uvc_out_netif();


#ifdef __cplusplus
}
#endif

#endif //_UVC_PORTS_H
