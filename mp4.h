#ifndef _MP4_H
#define _MP4_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "libavcodec/avcodec.h"


struct mp4_info_t {
    int64_t duration;   //单位：ms

    float frame_rate;

    int width;
    int height;

    enum AVCodecID video_code_id;
    enum AVCodecID audio_code_id;

    int channels;

};


int get_mp4_info(const char *filename, struct mp4_info_t *info);

#ifdef __cplusplus
}
#endif

#endif //_MP4_H
