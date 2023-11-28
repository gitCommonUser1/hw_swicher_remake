#ifndef _GENERAL_H
#define _GENERAL_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <errno.h>
#include <assert.h>

#include <sys/statfs.h>
#include <sys/types.h>
#include <dirent.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "osee_errors.h"
#include "osee_syslog.h"


#ifndef offsetof
#define offsetof(TYPE, MEMBER)  ((int)&((TYPE *)0)->MEMBER)
#endif


static int set_nonblocking(int fd)
{
    if (fd < 0)
        return -ERROR_NULL;

    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);

    return old_option;
}

static int addfd(int epollfd, int fd)
{
    int ret = 0;
    struct epoll_event event;

    if (epollfd < 0
            || fd < 0)
        return -ERROR_NULL;

    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    ret = epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    if (ret != 0)
        return ret;

    return set_nonblocking(fd);
}

static int delfd(int epollfd, int fd)
{
    struct epoll_event event;

    if (epollfd < 0
            || fd < 0)
        return -ERROR_NULL;

    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;

    return epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &event);
}

#define GET_MAX(a, b)	((a) >= (b) ? (a) : (b))

#define GET_DIFF(a, b)	((a) >= (b) ? ((a) - (b)) : (((b) - (a)) > 2))


#define STATUS_REAL_SIZE(size)	(((size) + 7) / 8)

static inline int is_status_clean(uint8_t *status, int size)
{
    int i;
    int len = (size + 7) / 8;

    for(i = 0; i < len; i++)
        if (0 != status[i])
            return 0;

    return 1;
}

static inline int is_status_all(uint8_t *status, int size)
{
    int i;
    int m = size / 8;
    int n = size % 8;

    for(i = 0; i < m; i++)
        if (0xFF != status[i])
            return 0;

    for(i = 0; i < n; i++)
        if (! (status[m] & (1 << i)))
            return 0;

    return 1;
}

static inline void clean_status(uint8_t *status, int size)
{
    memset((void *)status, 0, STATUS_REAL_SIZE(size));
}

static inline void mask_status(uint8_t *status, int pos)
{
    status[pos / 8] |= 1 << (pos % 8);
}

static inline void unmask_status(uint8_t *status, int pos)
{
    status[pos / 8] &= ~(1 << (pos % 8));
}

static inline int is_mask(uint8_t *status, int pos)
{
    if ((status[pos / 8]) & (1 << (pos % 8)))
        return 1;
    else
        return 0;
}

/* ERROR */
#define E_EXEC				1
#define E_CMD				2

static inline int _strcat(char *dst, int dst_size, const char *src)
{
    if (dst == NULL
        || src == NULL)
    {
        return -1;
    }

    int i, s = 0;
    int len = strlen(src);

    for(i = strlen(dst); i < dst_size; i++)
    {
        dst[i] = src[s++];
        if (s >= len)
            break;
    }

    return s;
}

static inline int executeCMD(const char *cmd, char *result, int max_size)
{
    char buf_ps[128] = {0};
    FILE *ptr = NULL;

    if (cmd == NULL
            || strlen(cmd) <= 0)
    {
        SYS_ERR("%s: cmd == NULL error !\n", __func__);

        return -ERROR_PARAM_NULL;
    }

    if (result == NULL)
    {
        SYS_ERR("%s: result == NULL error !\n", __func__);

        return -ERROR_PARAM_NULL;
    }

    buf_ps[0] = '\0';
    *result = '\0';

    if ((ptr = popen(cmd, "r")) != NULL)
    {
        while(fgets(buf_ps, 128, ptr) != NULL)
        {
            _strcat(result, max_size, buf_ps);
            if ((int)strlen(result) >= max_size)
                break;
        }

        while(fgets(buf_ps, 128, ptr) != NULL)
        {
        }

        pclose(ptr);
        ptr = NULL;

        return 0;
    }
    else
    {
        SYS_ERR("%s: popen \"%s\" error(%d - %s) !\n", __func__, cmd, errno, strerror(errno));

        return -ERROR_EXEC;
    }
}

static inline int uln(char *str)
{
    if (str == NULL)
        return -ERROR_PARAM_NULL;

    int k = strlen(str);

    if (k <= 0)
        return 0;

    k -= 1;

    while((str[k] == '\r') || (str[k] == '\n'))
    {
        k--;

        if (k <= 0)
            break;
    }

    k += 1;
    str[k] = '\0';

    return k;
}

static inline uint8_t Xor(uint8_t *start, int length)
{
    uint8_t _xor = 0;
    uint8_t *point = start;

    if (length <= 0)
        return _xor;

    while(length--)
    {
        _xor ^= *point;
        point++;
    }

    return _xor;
}


static int is_path_mount(const char *s)
{
#define MOUNT_PATH "/proc/mounts"
#define HSTRS_LEN       512

    char hstrs[HSTRS_LEN] = {0};
    char mpath[256];
    FILE *fp = NULL;
    long off = 0;

    if (s == NULL
        || strlen(s) <= 0)
        return 0;

    fp = fopen(MOUNT_PATH, "rb");
    if (fp == NULL)
    {
        SYS_ERR("%s: open %s error(%d - %s)\n", __func__, MOUNT_PATH, errno, strerror(errno));

        return 0;
    }

    while(! feof(fp))
    {
        off = ftell(fp);

        if (NULL == fgets(hstrs, HSTRS_LEN, fp))
            break;

        if (strlen(hstrs) == 0)
            continue;

        if (sscanf(hstrs, "%*s %s", mpath) == 1)
        {
            if (strcmp(mpath, s) == 0)
            {
                fclose(fp);

                return 1;
            }
        }
    }

    fclose(fp);

    return 0;
}

static int _df(const char *s, uint64_t *btotal, uint64_t *bused, uint64_t *bfree)
{
    struct statfs st;

    if (btotal == NULL
        || bused == NULL
        || bfree == NULL)
    {
        return -ERROR_NULL;
    }

    *btotal = 0;
    *bused = 0;
    *bfree = 0;

    if (s == NULL
        || strlen(s) <= 0)
        return -ERROR_PARAM_NULL;

    if (statfs(s, &st) < 0)
    {
        SYS_ERR("%s: error(%d - %s)\n", s, errno, strerror(errno));
        return -ERROR_READ;
    }

    if (st.f_blocks == 0)
    {
        SYS_ERR("st.f_blockS == 0\n");
        return -ERROR_MEM;
    }

    *btotal = st.f_blocks * st.f_bsize;
    *bfree = st.f_bfree * st.f_bsize;
    *bused = *btotal - *bfree;

    //SYS_DEBUG("%s: total:%ld, used:%ld free:%ld\n", __func__,
    //       *btotal, *bused, *bfree);

    return 0;
}

static int df(const char *s, uint64_t *btotal, uint64_t *bused, uint64_t *bfree)
{
    if (btotal == NULL
        || bused == NULL
        || bfree == NULL)
    {
        return -ERROR_NULL;
    }

    *btotal = 0;
    *bused = 0;
    *bfree = 0;

    if (s == NULL
        || strlen(s) <= 0)
        return -ERROR_PARAM_NULL;

    if (! is_path_mount(s))
        return -ERROR_NO_EXIST;

    return  _df(s, btotal, bused, bfree);
}

static int is_mounted(const char *path) {
#define MAX_LEN 4096
    char *rst = (char*)(malloc(MAX_LEN));
    memset(rst,MAX_LEN,0);
    executeCMD("mount",rst,MAX_LEN);
    if(strstr(rst,path) == NULL){
        free(rst);
        return -1;
    }else{
        free(rst);
        return 0;
    }
}

#ifdef __cplusplus
}
#endif

#endif //_GENERAL_H
