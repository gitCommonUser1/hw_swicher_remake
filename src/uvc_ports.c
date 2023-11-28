#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#include "files.h"

#include "osee_errors.h"
#include "osee_syslog.h"

#include "uvc_ports.h"
#include "uvc_out_power.h"


#define UVC_IN_PATH      "/sys/devices/platform/ffe00000.usb/usb1/1-1"
#define UVC_OUT_PATH     "/sys/devices/platform/usb0/ffd00000.dwc3/xhci-hcd.0.auto/usb3/3-1"

#define UVC_OUT_MODE	"/sys/devices/platform/ff4c0000.usb2-phy/otg_mode"
#define UVC_OUT_STATE	"/sys/devices/platform/ff4c0000.usb2-phy/extcon/extcon0/state"


/**
 * @brief uvc_out_is_otg_mode 获取uvc_out是否处于otg模式
 * @return
 */
int uvc_out_is_otg_mode()
{
    int fd = -1;
    char buf[64];
    int r_size = -1;

    if (access(UVC_OUT_MODE, R_OK) != 0)
        return 0;

    fd = open(UVC_OUT_MODE, O_RDONLY);
    if (fd < 0)
    {
        SYS_ERR("%s: open %s error(%d - %s)\n", __func__, UVC_OUT_MODE, errno, strerror(errno));

        return 0;
    }

    r_size = read(fd, buf, 64);
    if (r_size <= 0)
    {
        SYS_ERR("%s: read %s error(%d - %s)\n", __func__, UVC_OUT_MODE, errno, strerror(errno));

        return 0;
    }
    buf[r_size] = '\0';

    close(fd);

    if (strncmp(buf, "otg", 3) == 0)
        return 1;

    return 0;
}

/**
 * @brief uvc_out_is_otg_online uvc_out在otg模式下是否在线
 * @return
 */
int uvc_out_is_otg_online()
{
#define BUF_SIZE		128
    int fd = -1;
    char buf[BUF_SIZE];
    int r_size = -1;
    int offset = 0;

    int usb = -1;
    int host = -1;

    if (access(UVC_OUT_STATE, R_OK) != 0)
    {
        SYS_ERR("%s: access %s error(%d - %s)\n", __func__, UVC_OUT_STATE, errno, strerror(errno));

        return 0;
    }

    fd = open(UVC_OUT_STATE, O_RDONLY);
    if (fd < 0)
    {
        SYS_ERR("%s: open %s error(%d - %s)\n", __func__, UVC_OUT_STATE, errno, strerror(errno));

        return 0;
    }

    r_size = read(fd, buf, BUF_SIZE);
    if (r_size <= 0)
    {
        SYS_ERR("%s: read %s error(%d - %s)\n", __func__, UVC_OUT_STATE, errno, strerror(errno));

        return 0;
    }
    buf[r_size] = '\0';

    close(fd);

    offset = 0;
    while(offset < r_size)
    {
        if (sscanf((char *)&buf[offset], "USB=%d", &usb) == 1)
        {
            offset += 4;
        }
        else if (sscanf((char *)&buf[offset], "USB-HOST=%d", &host) == 1)
        {
            offset += 9;
        }

        while(offset < r_size
                && buf[offset] != '\n')
            offset++;

        if (buf[offset] == '\n')
        {
            offset++;
        }
    }

    if (host == 0
            && usb == 1)
        return 1;

    return 0;
}

/**
 * @brief set_uvc_out_mode 设置uvc_out的工作模式（otg/host）
 * @param mode
 * @return
 */
int set_uvc_out_mode(enum uvc_out_mode_t mode)
{
    int fd = -1;
    int otg = 0;

    if (access(UVC_OUT_MODE, W_OK) != 0)
    {
        return -ERROR_CANNOT_ACCESS;
    }

    uvc_out_power_init();
    if (mode == UVC_OUT_OTG)
    {
        set_uvc_out_power(0);
    }

    otg = uvc_out_is_otg_mode();

    if ((mode == UVC_OUT_OTG && otg)
            || (mode == UVC_OUT_HOST && (! otg))){

        if (mode == UVC_OUT_OTG)
        {
            set_uvc_out_power(0);
        }
        else
        {
            set_uvc_out_power(1);
        }

        return 0;
    }


    fd = open(UVC_OUT_MODE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        SYS_ERR("%s: open %s error(%d - %s)\n", __func__, UVC_OUT_MODE, errno, strerror(errno));

        return 0;
    }

    if (mode == UVC_OUT_OTG)
    {
        write(fd, "otg\n", 4);
    }
    else
    {
        write(fd, "host\n", 5);
    }

    fsync(fd);
    close(fd);

    otg = uvc_out_is_otg_mode();

    if ((mode == UVC_OUT_OTG && (! otg))
            || (mode == UVC_OUT_HOST && otg))
        return -ERROR_EXEC;

    if (mode == UVC_OUT_HOST)
    {
        set_uvc_out_power(1);
    }

    return 0;
}

char *_get_netif(char *from)
{
    char *netif = NULL;

    DIR *dp;
    struct dirent *drip;
    char tmppath[PATH_LENGTH];

    DIR *dp1;
    struct dirent *drip1;

    char *filename = basename(from);

    if (access(from, R_OK) != 0)
        return NULL;

    if ((dp = opendir(from)) == NULL)
    {
        SYS_ERR( "%s: opendir %s ERROR !\n", __func__, from);

        return NULL;
    }

    while ((drip = readdir(dp)) != NULL)
    {
        if (strcmp(drip->d_name, ".") == 0 ||
                strcmp(drip->d_name, "..") == 0)
            continue;

        if (strcmp(drip->d_name, "net") == 0)
        {
            sprintf(tmppath, "%s/%s", from, drip->d_name);

            if ((dp1 = opendir(tmppath)) == NULL)
            {
                SYS_ERR( "%s: opendir %s ERROR(%d - %s) !\n", __func__, from, errno, strerror(errno));

                return NULL;
            }

            while ((drip1 = readdir(dp1)) != NULL)
            {
                if (strcmp(drip1->d_name, ".") == 0 ||
                        strcmp(drip1->d_name, "..") == 0)
                    continue;

                netif = (char *)malloc(strlen(drip1->d_name) + 1);
                if (netif == NULL)
                    return NULL;

                strcpy(netif, drip1->d_name);
                netif[strlen(drip1->d_name)] = '\0';

                //SYS_DEBUG("%s: [%s]<%s><%s>\n", __func__, tmppath, drip1->d_name, netif);

                closedir(dp1);
                closedir(dp);

                return netif;
            }

            closedir(dp1);
        }

        if (strncmp(drip->d_name, filename, strlen(filename)) == 0)
        {
            sprintf(tmppath, "%s/%s", from, drip->d_name);
            netif = _get_netif(tmppath);
            if (netif)
            {
                //SYS_DEBUG("%s: <%s>\n", __func__, netif);

                closedir(dp);

                return netif;
            }
        }
    }

    closedir(dp);

    return NULL;
}

/**
 * @brief get_uvc_in_netif 获取uvc_in接入的网卡名称
 * @return
 */
char *get_uvc_in_netif()
{
    char *netif = NULL;

    DIR *dp;
    struct dirent *drip;

    char tmppath[PATH_LENGTH];

    if (access(UVC_IN_PATH, R_OK) != 0)
        return NULL;

    char dirname[] = UVC_IN_PATH;
    char *filename = basename(dirname);

    if ((dp = opendir(UVC_IN_PATH)) == NULL)
    {
        SYS_ERR( "%s: opendir %s ERROR(%d - %s) !\n", __func__, UVC_IN_PATH, errno, strerror(errno));

        return NULL;
    }

    while ((drip = readdir(dp)) != NULL)
    {
        if (strcmp(drip->d_name, ".") == 0 ||
                strcmp(drip->d_name, "..") == 0)
            continue;

        if (strncmp(drip->d_name, filename, strlen(filename)) == 0)
        {
            sprintf(tmppath, "%s/%s", UVC_IN_PATH, drip->d_name);
            netif = _get_netif(tmppath);
            if (netif)
            {
                //SYS_DEBUG("%s: <%s>\n", __func__, netif);
                closedir(dp);

                return netif;
            }
        }
    }

    closedir(dp);

    return NULL;
}

/**
 * @brief get_uvc_out_netif 获取uvc_out接入的网卡名称
 * @return
 */
char *get_uvc_out_netif()
{
    char *netif = NULL;

    DIR *dp;
    struct dirent *drip;

    char tmppath[PATH_LENGTH];

    if (uvc_out_is_otg_mode())
        return NULL;

    if (access(UVC_OUT_PATH, R_OK) != 0)
        return NULL;

    char dirname[] = UVC_OUT_PATH;
    char *filename = basename(dirname);

    if ((dp = opendir(UVC_OUT_PATH)) == NULL)
    {
        SYS_ERR( "%s: opendir %s ERROR(%d - %s) !\n", __func__, UVC_OUT_PATH, errno, strerror(errno));

        return NULL;
    }

    while ((drip = readdir(dp)) != NULL)
    {
        if (strcmp(drip->d_name, ".") == 0 ||
                strcmp(drip->d_name, "..") == 0)
            continue;

        if (strncmp(drip->d_name, filename, strlen(filename)) == 0)
        {
            sprintf(tmppath, "%s/%s", UVC_OUT_PATH, drip->d_name);
            netif = _get_netif(tmppath);
            if (netif)
            {
                //SYS_DEBUG("%s: <%s>\n", __func__, netif);

                closedir(dp);

                return netif;
            }
        }
    }

    closedir(dp);

    return NULL;
}
