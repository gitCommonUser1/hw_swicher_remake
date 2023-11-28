#ifndef DATAFILE_H
#define DATAFILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "crc32.h"
#include "osee_errors.h"


#pragma pack(1)
struct file_data_t {
    uint32_t crc;
    uint32_t len;
    uint8_t data[0];
};
#pragma pack()


#define FILL_FILEDATA_BUFSIZE(data)  ((data)->len = sizeof((data)) - sizeof(struct file_data_t))


static inline int check_crc(struct file_data_t *p_crc, int data_size)
{
#define CRC32_HEADER_SIZE  sizeof(struct file_data_t)
    if (p_crc == NULL)
    {
        return -ERROR_PARAM_NULL;
    }

    if (p_crc->len == 0xFFFFFFFF)
        return -ERROR_NO_EXIST;

    if ((p_crc->len + CRC32_HEADER_SIZE) > data_size)
        return -ERROR_NO_EXIST;

    if (p_crc->crc != crc32((uint8_t *)&p_crc->len, p_crc->len + 4))
        return -ERROR_DATA;

    return 0;
}

static inline void fill_crc(struct file_data_t *p_crc)
{
    p_crc->crc = crc32((uint8_t *)&p_crc->len, p_crc->len + 4);
}

/**
 * @brief read_file_data
 * @param filename
 * @param p_file_data
 * @return
 *
 * Must use FILL_FILEDATA_BUFSIZE() first
 */
int read_file_data(const char *filename, struct file_data_t *p_file_data);

/**
 * @brief write_file_data
 * @param filename
 * @param p_file_data
 * @return
 */
int write_file_data(const char *filename, struct file_data_t *p_file_data);


#ifdef __cplusplus
}
#endif

#endif // DATAFILE_H
