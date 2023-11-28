/******************************************************************************
 *
 *       Filename:  rv_switch_api.h
 *
 *    Description:  rv1126 切换台api 头文件
 *
 *        Version:  1.0
 *        Created:  2023年03月16日 21时27分27秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  yangkun (yk)
 *          Email:  xyyangkun@163.com
 *        Company:  yangkun.com
 *
 *****************************************************************************/

#ifndef RV11XX_PRJ_RV_SWITCH_API_H
#define RV11XX_PRJ_RV_SWITCH_API_H
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

enum RV_RESOLUTION{
    RV_1080P60=0x04, // default
    RV_1080P59=0x0c, // default
    RV_1080P50=0x03, // default
    RV_1080P30=0x02, // default
    RV_1080P29=0x0a, // default
    RV_1080P25=0x01, // default
    RV_1080P24=0x00, // default
    RV_1080P23=0x08, // default
};
enum RV_COLORTYPE{
    RV_420,
    RV_422,
    RV_444,
    RV_RGB1555,
    RV_RGB565,
    RV_RGB888,
    RV_RGBA8888,
    RV_ARGB8888,
};
// 从分辨率枚举中获取w, h, fps
#define RV_RESOLUTION_INFO(resolution, w, h, fps)                                                           \
    do{                                                                                                     \
        switch(resolution){                                                                                 \
            case RV_1080P60:w=1920;h=1080;fps=60;break;                                                     \
            case RV_1080P59:w=1920;h=1080;fps=59;break;                                                     \
            case RV_1080P50:w=1920;h=1080;fps=50;break;                                                     \
            case RV_1080P30:w=1920;h=1080;fps=30;break;                                                     \
            case RV_1080P29:w=1920;h=1080;fps=29;break;                                                     \
            case RV_1080P25:w=1920;h=1080;fps=25;break;                                                     \
            case RV_1080P24:w=1920;h=1080;fps=24;break;                                                     \
            case RV_1080P23:w=1920;h=1080;fps=23;break;                                                     \
        }                                                                                                   \
    }while(0)

const char *rv_switch_build_time();
unsigned char rv_switch_get_verion();

int rv_switch_init(int resolution);
int rv_switch_deinit();

// 暂时
int rv_switch_init_video_enc();
// 暂时
int rv_switch_deinit_video_enc();

/*******************************************************************************
 * @brief 设置输出分辨率，输入分辨率同输出分辨率，切换分辨率需要停止录像，推流功能
 *
 *******************************************************************************/
int rv_switch_resolutionset(int resolution);
enum E_STATUS{
    E_STATUE_SUCCESS = 0,
    E_STATUS_PROCESS = 1,
    E_STATUS_FAILED = 2,
    E_STATUS_LOSE_PACKET = 3,
    E_STATUS_CLOSE = 4,   // 关闭标记
    E_STATUS_MP4_PLAY = 5,
    E_STATUS_MP4_CLOSE = 6,  // 停止close
    E_STATUS_MP4_PAUSE = 7,
    E_STATUS_MP4_RECORD_LOSS = 8, // 低速SD卡录像丢帧
};

/*******************************************************************************
 * @brief 推流，录像，打开camera等
 * @param handle 传递结构体或类使用
 * @param status 状态  0 成功， 1 正确处理中， 2 失败, 3 网络有丢包
 * @param error 错误码
 *******************************************************************************/
typedef void (*rk_switch_cb)(long handle, int status, int error);
typedef void (*rk_ndi_search_cb)(long handle, const char *buf, int buf_size);

int rv_switch_push_start0(char *url, rk_switch_cb cb, long handle);
int rv_switch_push_stop0(rk_switch_cb cb, long handle);

int rv_switch_push_start1(char *url, rk_switch_cb cb, long handle);
int rv_switch_push_stop1(rk_switch_cb cb, long handle);

int rv_switch_push_start2(char *url, rk_switch_cb cb, long handle);
int rv_switch_push_stop2(rk_switch_cb cb, long handle);

/*******************************************************************************
 * @brief 低速sd卡录像时，会存在丢帧的情况，当发生丢帧时，如果注册此回调，会被调用，
 * error=1， statue=E_STATUS_MP4_RECORD_LOSS
 * @param in cb  sd卡录像丢帧时，回调
 * @return 0 success, other failed
 *******************************************************************************/
int rv_switch_record_start0(char *path, rk_switch_cb cb, long handle);
int rv_switch_record_stop0(rk_switch_cb cb, long handle);

int rv_switch_usb_camera_start0(rk_switch_cb cb, long handle);
int rv_switch_usb_camera_stop0(rk_switch_cb cb, long handle);

int rv_switch_nau8540_config();

int rv_switch_ndi_recv_start0(rk_switch_cb cb, long handle);
int rv_switch_ndi_recv_stop0(rk_switch_cb cb, long handle);

/*******************************************************************************
 * @brief 开始播放
 * @param in type
 *******************************************************************************/
int rv_switch_mp4_open(const char *path, rk_switch_cb cb, long handle);
int rv_switch_mp4_close(rk_switch_cb cb, long handle);
int rv_switch_mp4_pause(int pause, rk_switch_cb cb, long handle);
int rv_switch_mp4_replay(int replay);
/*******************************************************************************
 * @brief 设置文件播放结尾状态
 * @param in type
 *******************************************************************************/
int rv_swithc_mp4_set_end_state(int state);

/*******************************************************************************
 * @brief 获取文件播放结尾状态
 * @param in type
 *******************************************************************************/
int rv_swithc_mp4_get_end_state();

/*******************************************************************************
 * @brief 设置mic1 mic带电平 line模式
 * @param in type 1 mic不带输出电平  0 mic带电平 2 line模式
 * @return 0 success, other failed
 *******************************************************************************/
int rv_switch_set_mic0(int type);

/*******************************************************************************
 * @brief 设置mic2 mic带电平 line模式
 * @param in type 1 mic不带输出电平  0 mic带电平 2 line模式
 * @return 0 success, other failed
 *******************************************************************************/
int rv_switch_set_mic1(int type);

/*******************************************************************************
 * @brief 设置mic1 问音量
 * @param in type 0 left 1 right
 * @return 0 success, other failed
 *******************************************************************************/
int rv_switch_set_mic0_volume(float left, float right);

/*******************************************************************************
 * @brief 设置mic2 问音量
 * @param in type 0 left 1 right
 * @return 0 success, other failed
 *******************************************************************************/
int rv_switch_set_mic1_volume(float left, float right);

/*******************************************************************************
 * @brief 设置录像编译码率
 * @param in bitrate 要设置的码流
 * @return 0 success, other failed
 *******************************************************************************/
int rv_switch_set_record_bitrate(int bitrate);

/*******************************************************************************
 * @brief 设置推流编译码率
 * @param in bitrate 要设置的码流
 * @return 0 success, other failed
 *******************************************************************************/
int rv_switch_set_push_bitrate(int bitrate);

/*******************************************************************************
 * @brief 设置aac音频编译码率
 * @param in bitrate 要设置的码流
 * @return 0 success, other failed
 *******************************************************************************/
int rv_switch_set_aac_bitrate(int bitrate);

/*******************************************************************************
 * @brief 推流失败时回调
 *
 *******************************************************************************/
typedef void (*error_push)(int errno, char *error_str);

/*******************************************************************************
 * @brief 开始推流
 * @param seq 推流序号，默认为0
 *******************************************************************************/
int rv_switch_push_start(int seq, char *url, error_push cb);

/*******************************************************************************
 * @brief 断开推流
 *
 *******************************************************************************/
int rv_switch_push_stop(int seq);


/*******************************************************************************
 * @brief 开始录像
 *
 *******************************************************************************/
int rv_switch_record_start(char *record_path);

/*******************************************************************************
 * @brief 停止录像
 *
 *******************************************************************************/
int rv_switch_record_stop();

/*******************************************************************************
 * @brief 设置视频编码属性, 可以设置码率，编码类型h264, h265
 * @param in type 0=推流，1=录像
 *******************************************************************************/
int rv_switch_video_set(int type, void *param);


/*******************************************************************************
 * @brief 设置播放类型 usb camera直接播放， mp4需要后续指定路径，还有控制等
 * @param in type 0=mp4, 1 usb camera, 2 ndi,3 rtsp
 *******************************************************************************/
int rv_switch_play_set(int type);




/*******************************************************************************
 * @brief 上传静图像
 * @param in png_path png路径数组
 * @param in no  png路径数量
 * @return 0 success, other failed
 *******************************************************************************/
int rv_switch_still_upload(const char *png_path[], int no);


/******************************************* ndi ****************************************************************/

/**
 * @brief ndi 反初始化
 */
int osee_ndi_deinit();
/**
 * @brief 搜索ndi设备
 * @param[in] timeout 搜索超时时间，单位ms
 * @return 0 success, other failed
 */
int rkav_ndi_search(int timeout, rk_ndi_search_cb cb, long handle);

/**
 * @brief 需要连接的ndi设备
 * @param[in] str ndi设备字符串
 * @param[in] len ndi字符串长度
 * @return 0 success, other failed
 */
int rkav_ndi_connect(char *str, int len, rk_switch_cb cb, long handle);

/**
 * @brief 断开ndi设备
 */
void rkav_ndi_disconnect();
/******************************************* ndi ****************************************************************/
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif //RV11XX_PRJ_RV_SWITCH_API_H
