#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/statfs.h>
#include <sys/types.h>
#include <dirent.h>

#include <QString>
#include <QProcess>

#include "general.h"
#include "files.h"

#include "hw_config.h"

#include "osee_errors.h"

#include "qthread_uevent.h"

#include "media_sd.h"

#include "osee_math.h"

#include <QDateTime>


Media_sd *media_sd = NULL;

Media_sd::Media_sd()
{
    bused = 0;
    bfree = 0;
    btotal = 0;

    online = false;
    label = NULL;
    mount_try = 0;

    for(int i = 0; i < 4; i++)
    {
        partition[i] = NULL;
    }

    connect(&qthread_uevent, SIGNAL(emit_sd_change(bool )),
            this, SLOT(slot_sd_change(bool )));

    _is_online();

    if (online)
    {
        _get_label();
        _get_partition();
    }
}

Media_sd::~Media_sd()
{
    disconnect(&qthread_uevent, SIGNAL(emit_sd_change(bool )),
               this, SLOT(slot_sd_change(bool )));
}

QString Media_sd::secondToHMS(int second)
{
    int hours = second / 3600;
    int minutes = (second - (hours * 3600)) / 60;
    int seconds = second - (hours * 3600) - (minutes * 60);
    QString formattedTime = QString("%1:%2:%3").arg(hours, 2, 10, QChar('0'))
                                               .arg(minutes, 2, 10, QChar('0'))
                                               .arg(seconds, 2, 10, QChar('0'));
    return formattedTime;
}

void Media_sd::slot_sd_change(bool is_online)
{
    online = is_online;

    mount_try = 0;

    if (online)
    {
        _get_label();
        _get_partition();
    }
    else if (label)
    {
        free(label);
        label = NULL;
    }

    SYS_DEBUG("\n%s: online:%d\n", __func__, online);
}

void Media_sd::_is_online()
{
#define SYS_PATH_SD "/sys/bus/mmc/devices"

    DIR *dp;
    struct dirent *drip;

    online = false;

    if (access(SYS_PATH_SD, R_OK) != 0)
    {
        SYS_ERR( "%s: access %s ERROR(%d-%s) !\n", __func__, SYS_PATH_SD, errno, strerror(errno));

        return;
    }

    if ((dp = opendir(SYS_PATH_SD)) == NULL)
    {
        SYS_ERR( "%s: opendir %s ERROR(%d-%s) !\n", __func__, SYS_PATH_SD, errno, strerror(errno));

        return ;
    }

    while ((drip = readdir(dp)) != NULL)
    {
        if (strcmp(drip->d_name, ".") == 0 ||
                strcmp(drip->d_name, "..") == 0)
            continue;

        if (strncmp(drip->d_name, "mmc2:", 5) == 0)
        {
            online = true;

            closedir(dp);

            return;
        }
    }

    closedir(dp);
}

void Media_sd::_get_partition()
{
#define SYS_PATH_SD "/sys/bus/mmc/devices"

    DIR *dp;
    struct dirent *drip;
    char tmppath[PATH_LENGTH];
    char pwdpath[PATH_LENGTH] = {0};
    char mmcp[64];
    char tmp[64];

    for(int i = 0; i < 4; i++)
    {
        if (partition[i])
        {
            free(partition[i]);
            partition[i] = NULL;
        }
    }

    /*
     * 查找mmc2:xxx目录
     */
    if (access(SYS_PATH_SD, R_OK) != 0)
    {
        SYS_ERR( "%s: access %s ERROR(%d-%s) !\n", __func__, SYS_PATH_SD, errno, strerror(errno));

        return;
    }

    if ((dp = opendir(SYS_PATH_SD)) == NULL)
    {
        SYS_ERR( "%s: opendir %s ERROR(%d-%s) !\n", __func__, SYS_PATH_SD, errno, strerror(errno));

        return ;
    }

    while ((drip = readdir(dp)) != NULL)
    {
        if (strcmp(drip->d_name, ".") == 0 ||
                strcmp(drip->d_name, "..") == 0)
            continue;

        if (strncmp(drip->d_name, "mmc2:", 5) == 0)
        {
            sprintf(tmppath, "%s/%s/block", SYS_PATH_SD, drip->d_name);

            break;
        }
    }

    closedir(dp);

    /*
     * 查找mmc2:xxx/block/mmcblkN目录
     */
    if (access(tmppath, R_OK) != 0)
    {
        SYS_ERR( "%s: access %s ERROR(%d-%s) !\n", __func__, tmppath, errno, strerror(errno));

        return;
    }

    if ((dp = opendir(tmppath)) == NULL)
    {
        SYS_ERR( "%s: opendir %s ERROR(%d-%s) !\n", __func__, tmppath, errno, strerror(errno));

        return ;
    }

    while ((drip = readdir(dp)) != NULL)
    {
        if (strcmp(drip->d_name, ".") == 0 ||
                strcmp(drip->d_name, "..") == 0)
            continue;

        sprintf(mmcp, "%sp", drip->d_name);
        sprintf(&tmppath[strlen(tmppath)], "/%s", drip->d_name);

        break;
    }

    closedir(dp);

    /*
     * 遍历mmcblkNpN目录
     */
    if (access(tmppath, R_OK) != 0)
    {
        SYS_ERR( "%s: access %s ERROR !\n", __func__, tmppath);

        return;
    }

    getcwd(pwdpath, sizeof(pwdpath));

    chdir(tmppath);

    for(int i = 0; i < 4; i++)
    {
        sprintf(tmp, "%s%d", mmcp, (i + 1));

        if (access(tmp, R_OK) != 0)
            continue;

        partition[i] = (char *)malloc(strlen(tmp) + 1);
        if (partition[i] == NULL)
            continue;

        sprintf(partition[i], "%s", tmp);
    }

    chdir(pwdpath);

    /*for(int i = 0; i < 4; i++)
    {
        if (partition[i])
            SYS_DEBUG("partion%d: [%s]\n", i, partition[i]);
    }*/
}

int Media_sd::is_online()
{
    return online;
}

char *Media_sd::_get_label()
{
    char result[256];

    memset(result, 0, 256);
    int ret = executeCMD("blkid | grep mmcblk*[0-9]p1 | awk -F 'LABEL=\"' '{print $2}'|awk -F '\"' '{print$1}'", result, 256);
    if (ret != 0)
    {
        return NULL;
    }

    uln(result);
    if (strlen(result) <= 0)
    {
        sprintf(result, "Untitled");
        result[8] = '\0';
    }

    if (label)
        free(label);

    label = (char *)malloc(strlen(result) + 1);
    if (label == NULL)
    {
        return NULL;
    }
    strcpy(label, result);
    label[strlen(result)] = '\0';

    return label;
}

char *Media_sd::name()
{
    if (label == NULL
            && online == true)
    {
        _get_label();
    }

    return label;
}

uint64_t Media_sd::capacity()
{
    int ret = 0;

    bused = 0;
    bfree = 0;
    btotal = 0;

    if (online == false)
        return 0;

    if (mount_try == 0)
    {
        if (! is_path_mount(SD_MOUNTED_PATH))
        {
            system("./format_sd.sh exfat remount");
        }

        mount_try++;
    }

    ret = df(SD_MOUNTED_PATH, &btotal, &bused, &bfree);

    printf("__________________btotal:%lld\n",btotal);
    printf("__________________bused:%lld\n",bused);
    printf("__________________bfree:%lld\n",bfree);

    return btotal;
}

char *Media_sd::capacity_huma()
{
    size_t size = capacity();
    float f_size = 0;
    QString info;

    if (size >= 0x40000000)
    {
        f_size = size / (float)0x40000000;

        info = QString("%1").arg(f_size) + "GB";
    }
    else if (size >= 0x100000)
    {
        f_size = size / (float)0x100000;

        info = QString("%1").arg(f_size) + "MB";
    }
    else if (size >= 0x400)
    {
        f_size = size / (float)0x400;

        info = QString("%1").arg(f_size) + "KB";
    }
    else
    {
        info = QString("%1").arg(size) + "B";
    }

    return info.toLatin1().data();
}

int Media_sd::current_usage()
{
#define SIZE_RESERVE    (1024 * 1024 * 200)
    bused = 0;
    bfree = 0;
    btotal = 0;

    if (online == false)
        return -ERROR_NULL;

    if (mount_try == 0)
    {
        if (! is_path_mount(SD_MOUNTED_PATH))
        {
            system("./format_sd.sh exfat remount");
        }

        mount_try++;
    }

    int ret = df(SD_MOUNTED_PATH, &btotal, &bused, &bfree);

    //SD预留200MB空间（保留给录像信息等其它内容））
    if (bfree > SIZE_RESERVE)
        bfree -= SIZE_RESERVE;
    else
        bfree = 0;

    return ret;
}

int Media_sd::format_exfat(int index)
{
    int ret = 0;

    if (label)
    {
        free(label);
        label = NULL;
    }

    QString cmd = "./format_sd.sh exfat";

    switch (index) {
    case 0:
        cmd += " umount";
        break;
    case 1:
        cmd += " rm";
        break;
    case 2:
        cmd += " format";
        break;
    case 3:
        cmd += " remount";
        break;
    default:
        return -1;
    }

    SYS_DEBUG("format execute [%s]\n",cmd.toStdString().data());

    ret = system(cmd.toStdString().data());
    if (ret != 0)
    {
        SYS_ERR("%s ret:%d (%d - %s)\n", cmd.toLatin1().data(), ret, errno, strerror(errno));
        return -ERROR_EXEC;
    }

//    if (index == 0)
//    {
//        emit qthread_uevent.emit_sd_change(false);
//    }
//    else if (index == 3)
//    {
//        emit qthread_uevent.emit_sd_change(true);
//    }

    return 0;
}

int Media_sd::format_fat32(int index)
{
    int ret = 0;

    char cmd[32];

    if (label)
    {
        free(label);
        label = NULL;
    }

    sprintf(cmd, "./format_sd.sh fat32 ");

    switch (index) {
    case 0:
        sprintf(&cmd[strlen(cmd)], "umount");
        break;
    case 1:
        sprintf(&cmd[strlen(cmd)], "rm");
        break;
    case 2:
        sprintf(&cmd[strlen(cmd)], "format");
        break;
    case 3:
        sprintf(&cmd[strlen(cmd)], "remount");
        break;
    default:
        return -1;
    }

    ret = system(cmd);
    if (ret != 0)
    {
        return -ret;
    }

    if (index == 0)
    {
        emit qthread_uevent.emit_sd_change(false);
    }
    else if (index == 3)
    {
        emit qthread_uevent.emit_sd_change(true);
    }

    return 0;
}

void Media_sd::checkGbFree()
{
    setGbFree(QString::number(dround(bfree / 1073741824.0,2),'f',1));
}

void Media_sd::checkGbFree(int second)
{
    QString text = QString::number(dround(bfree / 1073741824.0,2),'f',1);
    text += "GB";
    text += " (";
    text += secondToHMS(second);
    text += ")";
    setGbFree(text);
}

