#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>           /* Definition of AT_* constants */
#include <unistd.h>
#include <stdlib.h>
#include <sys/file.h>

#include "osee_errors.h"
#include "osee_syslog.h"

#include "hdmi_out.h"


#define HDMI_OUT_PATH           "/tmp/video"
#define HDMI_OUT_NAME_PREFIX    "sii9022-"

#define OUTPUT_FORMAT           "output_format"
#define OUTPUT_COLORSPACE       "output_colorspace"
#define OUTPUT_RANGE            "output_range"

enum OUT_FORMAT{
    OUT_FORMAT_1080P24 = 0,
    OUT_FORMAT_1080P25,
    OUT_FORMAT_1080P30,
    OUT_FORMAT_1080P50,
    OUT_FORMAT_1080P60,
    OUT_FORMAT_MAX,
};


int set_hdmi_out_format(uint8_t id, int format)
{
#define PATH_SIZE       64
#define BUF_SIZE		64

    int fd = -1;

    char path[PATH_SIZE];
    char buf[BUF_SIZE];

    if (id >= HDMI_OUT_NUM)
        return -ERROR_CMD_UNSUPPORT;

    snprintf(path, PATH_SIZE, "%s/%s%d", HDMI_OUT_PATH, HDMI_OUT_NAME_PREFIX, id);

    if (access(path, F_OK) != 0)
    {
        snprintf(buf, PATH_SIZE, "mkdir -p %s", path);
        if (0 != system(buf))
        {
            SYS_ERR("%s: mkdir %s error(%d - %s)\n", __func__, path, errno, strerror(errno));

            return -ERROR_WRITE;
        }
    }

    switch (format)
    {
    case OUT_FORMAT_1080P24:
        snprintf(buf, BUF_SIZE, "HDMI_1080P24");
        break;
    case OUT_FORMAT_1080P25:
        snprintf(buf, BUF_SIZE, "HDMI_1080P25");
        break;
    case OUT_FORMAT_1080P30:
        snprintf(buf, BUF_SIZE, "HDMI_1080P30");
        break;
    case OUT_FORMAT_1080P50:
        snprintf(buf, BUF_SIZE, "HDMI_1080P50");
        break;
    case OUT_FORMAT_1080P60:
        snprintf(buf, BUF_SIZE, "HDMI_1080P60");
        break;
    default:
        return -ERROR_OUTRANGE;
        break;
    }

    snprintf(path, PATH_SIZE, "%s/%s%d/%s", HDMI_OUT_PATH, HDMI_OUT_NAME_PREFIX, id, OUTPUT_FORMAT);

    fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        SYS_ERR("%s: open %s error(%d - %s)\n", __func__, path, errno, strerror(errno));

        return -ERROR_OPEN;
    }

    if (flock(fd, LOCK_EX) != 0)
    {
        SYS_ERR("%s: flock %s error(%d - %s)\n", __func__, path, errno, strerror(errno));

        close(fd);
        return -ERROR_CANNOT_ACCESS;
    }

    if (write(fd, buf, strlen(buf)) < 0)
    {
        SYS_ERR("%s: write %s error(%d - %s)\n", __func__, path, errno, strerror(errno));

        flock(fd, LOCK_UN);
        close(fd);
        return -ERROR_WRITE;
    }

    fsync(fd);
    flock(fd, LOCK_UN);
    close(fd);

    system("killall sii9022");

    return 0;
}

int set_hdmi_out_colorspace(uint8_t id, enum hdmi_out_colorspace_t format)
{
#define PATH_SIZE       64
#define BUF_SIZE		64

    int fd = -1;

    char path[PATH_SIZE];
    char buf[BUF_SIZE];

    if (id >= HDMI_OUT_NUM)
        return -ERROR_CMD_UNSUPPORT;

    snprintf(path, PATH_SIZE, "%s/%s%d", HDMI_OUT_PATH, HDMI_OUT_NAME_PREFIX, id);

    if (access(path, F_OK) != 0)
    {
        snprintf(buf, PATH_SIZE, "mkdir -p %s", path);
        if (0 != system(buf))
        {
            SYS_ERR("%s: mkdir %s error(%d - %s)\n", __func__, path, errno, strerror(errno));

            return -ERROR_WRITE;
        }
    }

    switch (format)
    {
    case OUTPUT_COLORSPACE_AUTO:
        snprintf(buf, BUF_SIZE, "auto");
        break;
    case OUTPUT_COLORSPACE_RGB_FULL:
    case OUTPUT_COLORSPACE_RGB_LIMITED:
        snprintf(buf, BUF_SIZE, "rgb");
        break;
    case OUTPUT_COLORSPACE_YCC422_FULL:
    case OUTPUT_COLORSPACE_YCC422_LIMITED:
        snprintf(buf, BUF_SIZE, "ycc422");
        break;
    case OUTPUT_COLORSPACE_YCC444_FULL:
    case OUTPUT_COLORSPACE_YCC444_LIMITED:
        snprintf(buf, BUF_SIZE, "ycc444");
        break;
    default:
        return -ERROR_OUTRANGE;
        break;
    }

    snprintf(path, PATH_SIZE, "%s/%s%d/%s", HDMI_OUT_PATH, HDMI_OUT_NAME_PREFIX, id, OUTPUT_COLORSPACE);

    fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        SYS_ERR("%s: open %s error(%d - %s)\n", __func__, path, errno, strerror(errno));

        return -ERROR_OPEN;
    }

    if (flock(fd, LOCK_EX) != 0)
    {
        SYS_ERR("%s: flock %s error(%d - %s)\n", __func__, path, errno, strerror(errno));

        close(fd);
        return -ERROR_CANNOT_ACCESS;
    }

    if (write(fd, buf, strlen(buf)) < 0)
    {
        SYS_ERR("%s: write %s error(%d - %s)\n", __func__, path, errno, strerror(errno));

        flock(fd, LOCK_UN);
        close(fd);
        return -ERROR_WRITE;
    }

    fsync(fd);
    flock(fd, LOCK_UN);
    close(fd);

    switch (format)
    {
    case OUTPUT_COLORSPACE_AUTO:
        snprintf(buf, BUF_SIZE, "default");
        break;
    case OUTPUT_COLORSPACE_RGB_FULL:
    case OUTPUT_COLORSPACE_YCC422_FULL:
    case OUTPUT_COLORSPACE_YCC444_FULL:
        snprintf(buf, BUF_SIZE, "full");
        break;
    case OUTPUT_COLORSPACE_RGB_LIMITED:
    case OUTPUT_COLORSPACE_YCC422_LIMITED:
    case OUTPUT_COLORSPACE_YCC444_LIMITED:
        snprintf(buf, BUF_SIZE, "limited");
        break;
    default:
        return -ERROR_OUTRANGE;
        break;
    }

    snprintf(path, PATH_SIZE, "%s/%s%d/%s", HDMI_OUT_PATH, HDMI_OUT_NAME_PREFIX, id, OUTPUT_RANGE);

    fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        SYS_ERR("%s: open %s error(%d - %s)\n", __func__, path, errno, strerror(errno));

        return -ERROR_OPEN;
    }

    if (flock(fd, LOCK_EX) != 0)
    {
        SYS_ERR("%s: flock %s error(%d - %s)\n", __func__, path, errno, strerror(errno));

        close(fd);
        return -ERROR_CANNOT_ACCESS;
    }

    if (write(fd, buf, strlen(buf)) < 0)
    {
        SYS_ERR("%s: write %s error(%d - %s)\n", __func__, path, errno, strerror(errno));

        flock(fd, LOCK_UN);
        close(fd);
        return -ERROR_WRITE;
    }

    fsync(fd);
    flock(fd, LOCK_UN);
    close(fd);

    return 0;
}
