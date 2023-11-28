#ifndef _STR2UMD_H
#define _STR2UMD_H

#include <stdint.h>


#ifdef __cplusplus
 extern "C" {
#endif

enum align_t {
    ALIGN_LEFT,
    ALIGN_CENTER,
    ALIGN_RIGHT,
};

struct umd_config_t {
    char *font;         //字库
    int font_size;      //字体大小(单位:像素)

    uint8_t italic;     //是否斜体
    uint8_t bold;       //是否加粗
    enum align_t align; //对齐方式

    /* UMD显示区域大小(单位:像素) */
    int width;
    int height;

    /* 以下参数由str2umd计算产生 */
    void *buffer;       //点阵空间(每像素2bits)
                        //(由内部malloc，外部使用完毕后必须手动free, 以免造成内存泄漏以免造成内存泄漏)
    int bufsize;        //点阵空间大小(单位:字节)
    int str_width;      //字符串实际使用宽度(单位:像素)
};


/**
 * @brief show_umd: 以可视化方式将点阵数据输出到文件
 * @param filename: 待输出文件
 * @param cfg
 */
void show_umd(const char *filename, struct umd_config_t *cfg);

/**
 * @brief str2umd: 字符串转成umd所使用的点阵数据cfg->buffer
 *                  (大小cfg->bufsize由cfg中的width和height共同决定)
 * @param text: utf8字符串
 * @param tsize: text大小
 * @param cfg
 * @return
 */
int str2umd(const void *text, const int tsize, struct umd_config_t *cfg);

#ifdef __cplusplus
 }
#endif

#endif //_STR2UMD_H
