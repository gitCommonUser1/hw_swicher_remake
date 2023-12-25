#ifndef HW_CONFIG_H
#define HW_CONFIG_H

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#define OSEE_HW_NAME              "hw_rockchip_qt"
#define OSEE_HW_VERSION_MAJOR     0
#define OSEE_HW_VERSION_MINOR     0
#define OSEE_HW_VERSION_PATCH     52

#define SD_MOUNTED_PATH     "/mnt/sdcard"
#define SD_IMAGE_PATH       "/mnt/sdcard/images/"
#define SD_VIDEO_PATH       "/mnt/sdcard/videos/"
#define SD_STREAM_KEY_PATH       "/mnt/sdcard/stream_key/"
#define SD_MACRO_PATH       "/mnt/sdcard/macro/"
#define OEM_MOUNTED_PATH	"/oem"
#define OEM_HW_IMAGES "/oem/hw_rockchip_qt/images/"

#define FONT_PATH "./inter.ttf"

#define STILLMAX 32
#define STILLPATH "/userdata/stillImages/"

#define DATA_PATH "/data/hw_rockchip_qt"
#define DATA_STILL_PATH DATA_PATH "/still.ini"

#define NDI_PATH "/userdata/hw_rockchip_qt/ndi.ini"

#define MACRO_PATH "/userdata/macro/"   //userdata/macro/      //mnt/sdcard/macro/
#define MACRO_NUMBER_MAX 8

#define CHROMAKEY_SMP_COLOR_PATH "/userdata/hw_rockchip_qt/chromakey_color.ini"

#define OEM_VERSION         OEM_MOUNTED_PATH "/etc/version"
#define OEM_TIME            OEM_MOUNTED_PATH "/etc/time"

//恢复出厂设置标记文件
#define FACTORY_RESET_FILE_PATH "/userdata/hw_rockchip_qt/factory_reset"
//kms文件路径
#define QTKMS_FILE_PATH "/userdata/hw_rockchip_qt/qtkms.json"
//默认macro文件路径
#define MACRO_DEFAULT_FILE_PATH "/oem/hw_rockchip_qt/macro.xml"

#define MASK_H_MIN -16.0
#define MASK_H_MAX 16.0
#define MASK_V_MIN -9.0
#define MASK_V_MAX 9.0

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif // HW_CONFIG_H
