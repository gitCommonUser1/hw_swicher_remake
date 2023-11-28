#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

#include "osee_errors.h"
#include "osee_syslog.h"

#include "libavcodec/avcodec.h"
#include "libavutil/opt.h"
#include "libavutil/imgutils.h"
#include "libavformat/avformat.h"
#include "libavutil/samplefmt.h"
#include "libavutil/timestamp.h"

#include "mp4.h"

//#define MP4_DEBUG
#undef MP4_DEBUG

int get_mp4_info(const char *filename, struct mp4_info_t *info)
{
    int ret = 0;

    if (filename == NULL
            || strlen(filename) <= 0)
        return -ERROR_PARAM_NULL;

    if (info == NULL)
        return -ERROR_NULL;

#ifdef MP4_DEBUG
    struct timeval _start, _now, _tmp;

    gettimeofday(&_start, NULL);
#endif

    memset(info, 0, sizeof(struct mp4_info_t));

    if (access(filename, R_OK) != 0)
        return -ERROR_CANNOT_ACCESS;

    av_register_all();
    AVFormatContext *pFormatCtx = avformat_alloc_context();

    ret = avformat_open_input(&pFormatCtx, filename, NULL, NULL);
    if (ret)
    {
        return -ERROR_OPEN;
    }

    /*ret = avformat_find_stream_info(pFormatCtx, NULL);
    if (ret < 0)
    {
        SYS_ERR("Cannot find stream information\n");

        avformat_close_input(&pFormatCtx);

        return -ERROR_READ;
    }*/

    info->duration = pFormatCtx->duration / 1000;

    for(int i = 0; i < pFormatCtx->nb_streams;++i)
    {
        if(pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            //获取视频的帧率
            int den = pFormatCtx->streams[i]->avg_frame_rate.den;
            int num = pFormatCtx->streams[i]->avg_frame_rate.num;
            info->frame_rate = (float)num / (float)den;

            info->video_code_id = pFormatCtx->streams[i]->codec->codec_id;

            info->width = pFormatCtx->streams[i]->codec->width;
            info->height = pFormatCtx->streams[i]->codec->height;

            if(AV_CODEC_ID_NONE != info->audio_code_id)
                break;
        }
        else if(pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            info->audio_code_id = pFormatCtx->streams[i]->codec->codec_id;
            info->channels = pFormatCtx->streams[i]->codec->channels;

            if(AV_CODEC_ID_NONE != info->video_code_id)
                break;
        }
    }

    avformat_close_input(&pFormatCtx);

#ifdef MP4_DEBUG
    printf("%s: %dx%d %.2ffps %ldms codec_id:%d\n",
           filename,
           info->width,
           info->height,
           info->frame_rate,
           info->duration,
           info->codec_id);

    gettimeofday(&_now, NULL);
    timersub(&_now, &_start, &_tmp);
    printf("%s:%d: %s: used: %ld.%.6lds\n",
           __func__, __LINE__,
           filename,
           _tmp.tv_sec, _tmp.tv_usec);
#endif

    return 0;
}
