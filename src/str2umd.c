#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <malloc.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "osee_syslog.h"
#include "osee_time.h"

#include "str2umd.h"


#define fillumd(buffer, data, index)    ((buffer)[(index) >> 2] |= ((data) & 0xC0) >> (((index) & 0x03) * 2))

inline static void draw_bitmap(struct umd_config_t *cfg,
             FT_Bitmap*  bitmap,
             FT_Int      x,
             FT_Int      y)
{
    FT_Int  i, j, p, q;
    FT_Int  x_max = x + bitmap->width;
    FT_Int  y_max = y + bitmap->rows;

    for ( i = x, p = 0; i < x_max; i++, p++ )
    {
        for ( j = y, q = 0; j < y_max; j++, q++ )
        {
            if ( i < 0      || j < 0       ||
                i >= cfg->width || j >= cfg->height )
            continue;

            fillumd((uint8_t *)cfg->buffer, bitmap->buffer[q * bitmap->width + p], j * cfg->width + i);
        }
    }
}

inline static void align_umd(struct umd_config_t *cfg)
{
    if (ALIGN_LEFT == cfg->align)
        return ;

    int bytesPerLine = cfg->width / 4;
    int bits = cfg->str_width * 2;

    int bytes = (bits + 7) / 8;
    int bytes_pos = 0;

    if (ALIGN_RIGHT == cfg->align)
    {
        bytes_pos = bytesPerLine - bytes;

        if (bytes_pos > 0)
        {
            for(int y = 0; y < cfg->height; y++)
            {
                memmove(&((uint8_t *)cfg->buffer)[y * bytesPerLine + bytes_pos],
                        &((uint8_t *)cfg->buffer)[y * bytesPerLine],
                        bytes);
                memset(&((uint8_t *)cfg->buffer)[y * bytesPerLine], 0, bytes_pos);
            }
        }
    }

    if (ALIGN_CENTER == cfg->align)
    {
        bytes_pos = (bytesPerLine - bytes) / 2;

        if (bytes_pos > 0)
        {
            for(int y = 0; y < cfg->height; y++)
            {
                memmove(&((uint8_t *)cfg->buffer)[y * bytesPerLine + bytes_pos],
                        &((uint8_t *)cfg->buffer)[y * bytesPerLine],
                        bytes);
                memset(&((uint8_t *)cfg->buffer)[y * bytesPerLine], 0, bytes_pos);
            }
        }
    }
}

/**
 * @brief show_umd: 以可视化方式将点阵数据输出到文件
 * @param filename: 待输出文件
 * @param cfg
 */
void show_umd(const char *filename, struct umd_config_t *cfg)
{
    FILE *fp;

    if (filename == NULL
            || strlen(filename) <= 0)
        return;

    fp = fopen(filename, "w+b");

    int x = 0;
    for(int i = 0; i < cfg->bufsize; i++)
    {
        uint8_t data = ((uint8_t *)cfg->buffer)[i];
        for(int j = 6; j >= 0; j -= 2)
        {
            if (((data & (3 << j)) >> j) >= 2)
                fprintf(fp, "*");
            else
                fprintf(fp, " ");
        }

        x += 4;

        if (x >= cfg->width)
        {
            fprintf(fp, "\n");
            x = 0;
        }
    }

    fflush(fp);
    fsync(fileno(fp));
    fclose(fp);
}

/**
 * @brief str2umd: 字符串转成umd所使用的点阵数据cfg->buffer
 *                  (大小cfg->bufsize由cfg中的width和height共同决定)
 * @param text: utf8字符串
 * @param tsize: text大小
 * @param cfg
 * @return
 */
int str2umd(const void *text, const int tsize, struct umd_config_t *cfg)
{
    FT_Library library;                         //FreeType库的句柄
    FT_Face face;

    FT_BBox       bbox;
    FT_GlyphSlot  slot;
    FT_Matrix     matrix;                 /* transformation matrix */
    FT_Vector     pen;                    /* untransformed origin  */
    FT_Error      error;
    FT_ULong      char_code;

    int tmp;
    int x_offset = 0, y_offset = 0;

    uint8_t *p, *p_end;

    double angle;

    if (cfg == NULL
            || cfg->font == NULL)
    {
        SYS_ERR("cfg or cfg->font is null \n");
        return -2;
    }

    cfg->bufsize = cfg->width * cfg->height / 4;

    if (malloc_usable_size(cfg->buffer) < cfg->bufsize)
    {
        cfg->buffer = (void *)realloc(cfg->buffer, cfg->bufsize);
        if (cfg->buffer == NULL)
        {
            SYS_ERR("malloc cfg->buffer error(%d - %s)\n", errno, strerror(errno));

            return -4;
        }
    }
    memset(cfg->buffer, 0, cfg->bufsize);

    if (text == NULL
            || tsize <= 0)
    {
        //SYS_ERR("text is null \n");
        return 0;
    }

    if (access(cfg->font, R_OK) != 0)
    {
        SYS_ERR("acess cfg->font error(%d - %s)\n", errno, strerror(errno));
        return -3;
    }

    if (cfg->italic)
        angle = ( -25.0 / 360 ) * 3.14159 * 2;
    else
        angle = 0;

    error = FT_Init_FreeType( &library );
    if ( error )
    {
        SYS_ERR("FT_Init_FreeType: error(%d - %s)\n", error, FT_Error_String(error));

        return error;
    }

    error = FT_New_Face( library, cfg->font, 0, &face);
    if (error == FT_Err_Unknown_File_Format )
    {
        SYS_ERR("FT_New_Face: error(%d - %s)\n", error, FT_Error_String(error));

        return error;
    }
    else if (error < 0)
    {
        SYS_ERR("FT_New_Face: error(%d - %s)\n", error, FT_Error_String(error));

        return error;
    }

    FT_Select_Charmap(face, FT_ENCODING_UNICODE);

    FT_Set_Pixel_Sizes(face, cfg->font_size, cfg->font_size);      //把字符像素设置为16*16像素, 0表示与另一个尺寸值相等。

    slot = face->glyph;

    /* set up matrix */
    matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
    matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
    matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
    matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );

    bbox.xMin = cfg->width;
    bbox.xMax = 0;
    bbox.yMin = cfg->height;
    bbox.yMax = 0;

    /* the pen position in 26.6 cartesian space coordinates; */
    /* start at (0,40) relative to the upper left corner  */
    pen.x = 0 * 64;
    pen.y = 0; //( cfg->height - 40 ) * 64;

    p = (uint8_t *)text;
    p_end = &((uint8_t *)text)[tsize - 1];

    while (p <= p_end)
    {
        if ((p[0] & 0x80) == 0x00)
        {
            char_code = p[0];
            p++;
        }
        else if (((p[0] & 0xE0) == 0xC0)
                 && ((p_end - p) >= 1)
                 && ((p[1] & 0xC0) == 0x80))
        {
            char_code = ((uint32_t)(p[0] & 0x1F) << 6) | ((uint32_t)(p[1] & 0x3F));

            p += 2;
        }
        else if (((p[0] & 0xF0) == 0xE0)
                 && ((p_end - p) >= 2)
                 && ((p[1] & 0xC0) == 0x80)
                 && ((p[2] & 0xC0) == 0x80))
        {
            char_code = ((uint32_t)(p[0] & 0x0F) << 12) | ((uint32_t)(p[1] & 0x3F) << 6) | ((uint32_t)(p[2] & 0x3F));

            p += 3;
        }
        else if (((p[0] & 0xF8) == 0xF0)
                 && ((p_end - p) >= 3)
                 && ((p[1] & 0xC0) == 0x80)
                 && ((p[2] & 0xC0) == 0x80)
                 && ((p[3] & 0xC0) == 0x80))
        {
            char_code = ((uint32_t)(p[0] & 0x07) << 18) | ((uint32_t)(p[1] & 0x3F) << 12) | ((uint32_t)(p[2] & 0x3F) << 6) | ((uint32_t)(p[3] & 0x3F));

            p += 4;
        }
        else
        {
            p++;
            continue;
        }

        /* set transformation */
        FT_Set_Transform( face, &matrix, &pen );

        /* load glyph image into the slot (erase previous one) */
        error = FT_Load_Char( face, char_code, FT_LOAD_RENDER  );
        if ( error )
        {
            SYS_WARNING("FT_Load_Char [0x%X] error(%d - %s)\n", char_code, error, FT_Error_String(error));

            continue;                 /* ignore errors */
        }

        if (cfg->bold)
            FT_GlyphSlot_Embolden(slot);

        /* increment pen position */
        pen.x += slot->advance.x;
        //pen.y += slot->advance.y;

        cfg->str_width = (pen.x >> 6) + 1;

        if (cfg->str_width >= cfg->width)
            break;

        if (bbox.xMin > slot->bitmap_left)
            bbox.xMin = slot->bitmap_left;

        tmp = slot->bitmap_left + slot->bitmap.width;
        if (bbox.xMax < tmp)
            bbox.xMax = tmp;

        if (bbox.yMax < slot->bitmap_top)
            bbox.yMax = slot->bitmap_top;

        tmp = (int)slot->bitmap_top - (int)slot->bitmap.rows;
        if (bbox.yMin > tmp)
            bbox.yMin = tmp;
    }

    /* the pen position in 26.6 cartesian space coordinates; */
    /* start at (0,40) relative to the upper left corner  */
    pen.x = 0 * 64;
    pen.y = 0; //( cfg->height - 40 ) * 64;

    switch (cfg->align)
    {
    case ALIGN_CENTER:
        x_offset = (cfg->width - (bbox.xMax - bbox.xMin)) / 2 - bbox.xMin;
        break;
    case ALIGN_RIGHT:
        x_offset = cfg->width - (bbox.xMax - bbox.xMin) - bbox.xMin;
        break;
    case ALIGN_LEFT:
    default:
        x_offset = -bbox.xMin;
        break;
    }

    y_offset = bbox.yMax + (cfg->height - (bbox.yMax - bbox.yMin)) / 2;

    p = (uint8_t *)text;
    p_end = &((uint8_t *)text)[tsize - 1];

    while (p <= p_end)
    {
        if ((p[0] & 0x80) == 0x00)
        {
            char_code = p[0];
            p++;
        }
        else if (((p[0] & 0xE0) == 0xC0)
                 && ((p_end - p) >= 1)
                 && ((p[1] & 0xC0) == 0x80))
        {
            char_code = ((uint32_t)(p[0] & 0x1F) << 6) | ((uint32_t)(p[1] & 0x3F));

            p += 2;
        }
        else if (((p[0] & 0xF0) == 0xE0)
                 && ((p_end - p) >= 2)
                 && ((p[1] & 0xC0) == 0x80)
                 && ((p[2] & 0xC0) == 0x80))
        {
            char_code = ((uint32_t)(p[0] & 0x0F) << 12) | ((uint32_t)(p[1] & 0x3F) << 6) | ((uint32_t)(p[2] & 0x3F));

            p += 3;
        }
        else if (((p[0] & 0xF8) == 0xF0)
                 && ((p_end - p) >= 3)
                 && ((p[1] & 0xC0) == 0x80)
                 && ((p[2] & 0xC0) == 0x80)
                 && ((p[3] & 0xC0) == 0x80))
        {
            char_code = ((uint32_t)(p[0] & 0x07) << 18) | ((uint32_t)(p[1] & 0x3F) << 12) | ((uint32_t)(p[2] & 0x3F) << 6) | ((uint32_t)(p[3] & 0x3F));

            p += 4;
        }
        else
        {
            p++;
            continue;
        }

        /* set transformation */
        FT_Set_Transform( face, &matrix, &pen );

        /* load glyph image into the slot (erase previous one) */
        error = FT_Load_Char( face, char_code, FT_LOAD_RENDER  );
        if ( error )
        {
            SYS_WARNING("FT_Load_Char [0x%X] error(%d - %s)\n", char_code, error, FT_Error_String(error));

            continue;                 /* ignore errors */
        }

        if (cfg->bold)
            FT_GlyphSlot_Embolden(slot);

        if ((((pen.x + slot->advance.x) >> 6) + 1) >= cfg->width)
            break;

        /* now, draw to our target surface (convert position) */
        draw_bitmap( cfg,
                     &slot->bitmap,
                     x_offset + slot->bitmap_left,
                     y_offset - slot->bitmap_top);

        /* increment pen position */
        pen.x += slot->advance.x;
        //pen.y += slot->advance.y;

        cfg->str_width = (pen.x >> 6) + 1;

        if (cfg->str_width >= cfg->width)
            break;
    }

    SYS_DEBUG("str_width: %d\n", cfg->str_width);
    //align_umd(cfg);

    FT_Done_Face    ( face );
    FT_Done_FreeType(library);

    return 0;
}
