#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>           /* Definition of AT_* constants */
#include <unistd.h>
#include <stdlib.h>
#include <sys/file.h>

#include "osee_errors.h"
#include "osee_syslog.h"

#include "hdmi_in.h"


#define HDMI_IN_PATH           "/sys/class/video"
#define HDMI_IN_NAME_PREFIX    "sii9293-"

#define INPUT_COLORSPACE       "input_colorspace"
#define INPUT_RANGE            "input_range"


int set_hdmi_in_colorspace(uint8_t id, enum hdmi_in_colorspace_t format)
{
#define PATH_SIZE       64
#define BUF_SIZE		32

    int fd = -1;

    char path[PATH_SIZE];
    char buf[BUF_SIZE];

    if (id >= HDMI_IN_NUM)
        return -ERROR_CMD_UNSUPPORT;

    snprintf(path, PATH_SIZE, "%s/%s%d", HDMI_IN_PATH, HDMI_IN_NAME_PREFIX, id);

    if (access(path, F_OK) != 0)
    {
        return -ERROR_CANNOT_ACCESS;
    }

    switch (format)
    {
    case INPUT_COLORSPACE_AUTO:
        snprintf(buf, BUF_SIZE, "auto");
        break;
    case INPUT_COLORSPACE_RGB_FULL:
    case INPUT_COLORSPACE_RGB_LIMITED:
        snprintf(buf, BUF_SIZE, "rgb");
        break;
    case INPUT_COLORSPACE_YCC422_FULL:
    case INPUT_COLORSPACE_YCC422_LIMITED:
        snprintf(buf, BUF_SIZE, "ycc422");
        break;
    case INPUT_COLORSPACE_YCC444_FULL:
    case INPUT_COLORSPACE_YCC444_LIMITED:
        snprintf(buf, BUF_SIZE, "ycc444");
        break;
    default:
        return -ERROR_OUTRANGE;
        break;
    }

    snprintf(path, PATH_SIZE, "%s/%s%d/%s", HDMI_IN_PATH, HDMI_IN_NAME_PREFIX, id, INPUT_COLORSPACE);

    fd = open(path, O_WRONLY | O_TRUNC);
    if (fd < 0)
    {
        SYS_ERR("%s: open %s error(%d - %s)\n", __func__, path, errno, strerror(errno));

        return -ERROR_OPEN;
    }

    if (write(fd, buf, strlen(buf)) < 0)
    {
        SYS_ERR("%s: write %s error(%d - %s)\n", __func__, path, errno, strerror(errno));

        close(fd);
        return -ERROR_WRITE;
    }

    fsync(fd);
    close(fd);

    switch (format)
    {
    case INPUT_COLORSPACE_AUTO:
        snprintf(buf, BUF_SIZE, "default");
        break;
    case INPUT_COLORSPACE_RGB_FULL:
    case INPUT_COLORSPACE_YCC422_FULL:
    case INPUT_COLORSPACE_YCC444_FULL:
        snprintf(buf, BUF_SIZE, "full");
        break;
    case INPUT_COLORSPACE_RGB_LIMITED:
    case INPUT_COLORSPACE_YCC422_LIMITED:
    case INPUT_COLORSPACE_YCC444_LIMITED:
        snprintf(buf, BUF_SIZE, "limited");
        break;
    default:
        return -ERROR_OUTRANGE;
        break;
    }

    snprintf(path, PATH_SIZE, "%s/%s%d/%s", HDMI_IN_PATH, HDMI_IN_NAME_PREFIX, id, INPUT_RANGE);

    fd = open(path, O_WRONLY | O_TRUNC);
    if (fd < 0)
    {
        SYS_ERR("%s: open %s error(%d - %s)\n", __func__, path, errno, strerror(errno));

        return -ERROR_OPEN;
    }

    if (write(fd, buf, strlen(buf)) < 0)
    {
        SYS_ERR("%s: write %s error(%d - %s)\n", __func__, path, errno, strerror(errno));

        close(fd);
        return -ERROR_WRITE;
    }

    fsync(fd);
    close(fd);

    return 0;
}
