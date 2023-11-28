#ifndef _FILES_H
#define _FILES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <malloc.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/reboot.h>
#include <linux/reboot.h>
#include <mtd/mtd-user.h>
#include <dirent.h>
#include <stdint.h>

//#include <QtGlobal>

#include "md5.h"

#include "osee_syslog.h"
#include "osee_errors.h"


#define PATH_LENGTH	512
#define BUF_SIZE	4096


static int _find(char *path, char *from)
{
	DIR *dp;
	struct dirent *drip;
	struct stat statbuf;
	char tmppath[PATH_LENGTH];

    //SYS_DEBUG("%s: look %s in %s\n", __func__, path, from);
	
	if ((dp = opendir(from)) == NULL)
	{
        SYS_ERR( "%s: opendir %s ERROR(%d - %s) !\n", __func__, from, errno, strerror(errno));

		return 0;
	}

	while ((drip = readdir(dp)) != NULL)
	{
		if (strcmp(drip->d_name, ".") == 0 ||
				strcmp(drip->d_name, "..") == 0)
			continue;

		if (path[0] != '*')
		{
			if (strcmp(path, drip->d_name) == 0)
			{
				sprintf(path, "%s/%s", from, drip->d_name);
				closedir(dp);
				return 1;
			}
		}
		else
		{
			if (strncmp(&(drip->d_name)[strlen(drip->d_name) - strlen(path) + 1], &path[1], strlen(path) - 1) == 0)
			{
				sprintf(path, "%s/%s", from, drip->d_name);
				closedir(dp);
				return 1;
			}
		}

		lstat(drip->d_name, &statbuf);
		if (S_ISDIR(statbuf.st_mode))
		{
			sprintf(tmppath, "%s/%s", from, drip->d_name);
			
			if (_find(path, tmppath))
			{
				closedir(dp);
				return 1;
			}
		}
	}

	closedir(dp);
	
	return 0;
}

static int delete_file(const char *path)
{
    int ret = 0;
    int status = 0;
    int i;

    DIR *dir;
    struct dirent *dirinfo;
    struct stat statbuf;

    char pwdpath[PATH_LENGTH] = {0};

    if (NULL == path)
    {
        return -ERROR_PARAM_NULL;
    }

    if (path[strlen(path) - 1] == '*')
    {
        char *delpath = (char *)malloc(strlen(path));
        memmove(delpath, path, strlen(path) - 1);

        for(i = strlen(delpath) - 1; i >= 0; i--)
        {
            if (path[i] != '*' && path[i] != '/')
                break;
        }
        delpath[i+1] = '\0';

        if(access(delpath, F_OK) < 0)
        {
            free(delpath);
            return 0;
        }

        /*lstat(delpath, &statbuf);

          if(! S_ISDIR(statbuf.st_mode))
          {
          free(delpath);
          return 0;
          }*/

        if((dir = opendir(delpath)) == NULL)
        {
            SYS_ERR("%s: open %s error(%d - %s)\n", __func__, delpath, errno, strerror(errno));

            free(delpath);
            return -ERROR_OPEN;
        }

        getcwd(pwdpath, sizeof(pwdpath));

        chdir(delpath);

        while((dirinfo = readdir(dir)) != NULL)
        {
            if(strcmp(dirinfo->d_name, ".") == 0
                    || strcmp(dirinfo->d_name, "..") == 0)
                continue;

            ret = delete_file(dirinfo->d_name);
            if (ret < 0)
            {
                status = ret;
            }
        }

        chdir(pwdpath);

        closedir(dir);
        free(delpath);
    }

    if(access(path, F_OK) < 0)
        return 0;

    lstat(path, &statbuf);

    if(S_ISREG(statbuf.st_mode))
    {
        //SYS_DEBUG("%s: remove file: %s\n", __func__, path);

        if (remove(path) < 0)
        {
            SYS_ERR("%s: remove %s ERROR(%s)\n", __func__, path, strerror(errno));

            status = -ERROR_WRITE;
        }
    }
    else if(S_ISDIR(statbuf.st_mode))
    {
        if((dir = opendir(path)) == NULL)
            return -ERROR_OPEN;

        getcwd(pwdpath, sizeof(pwdpath));

        chdir(path);

        while((dirinfo = readdir(dir)) != NULL)
        {
            if(strcmp(dirinfo->d_name, ".") == 0
                    || strcmp(dirinfo->d_name, "..") == 0)
                continue;

            ret = delete_file(dirinfo->d_name);
            if (ret < 0)
            {
                status = ret;
            }
        }

        chdir(pwdpath);
        closedir(dir);

        SYS_DEBUG("%s: rmdir=%s\n", __func__, path);

        if (rmdir(path) < 0)
        {
            SYS_ERR("%s: rmdir %s ERROR(%s)\n", __func__, path, strerror(errno));

            status = -ERROR_WRITE;
        }
    }

    return status;
}

#if 1 //Q_OS_LINUX
static inline char *get_dirname(const char *filename)
{
    char *dirpath = NULL;

    if (filename == NULL
            || strlen(filename) == 0)
    {
        return NULL;
    }

    int len = strlen(filename);
    int i = len - 1;

    while(i >= 0)
    {
        if (filename[i] == '/')
        {
            break;
        }

        i--;
    }

    if (i > 0)
    {
        dirpath = (char *)malloc(i + 1);
        if (dirpath == NULL)
        {
            SYS_ALERT("%s: malloc error !\n", __func__);

            return NULL;
        }

        memcpy(dirpath, filename, i);
        dirpath[i] = '\0';
    }
    else if (i == 0)
    {
        dirpath = (char *)malloc(2);
        if (dirpath == NULL)
        {
            SYS_ALERT("%s: malloc error !\n", __func__);

            return NULL;
        }

        if (i == 0)
            dirpath[0] = '/';
        else
            dirpath[0] = '.';

        dirpath[1] = '\0';
    }

    return dirpath;
}
#elif Q_OS_WIN
static inline char *get_dirname(const char *filename)
{
    char *dirpath = NULL;

    if (filename == NULL
            || strlen(filename) == 0)
    {
        return NULL;
    }

    int len = strlen(filename);
    int i = len - 1;

    while(i >= 0)
    {
        if (filename[i] == '\\')
        {
            break;
        }
    }

    if (i > 0)
    {
        dirpath = (char *)malloc(i + 1);
        if (dirpath == NULL)
        {
            SYS_ALERT("%s: malloc error !\n", __func__);

            return NULL;
        }

        memcpy(dirpath, filename, i);
        dirpath[i] = '\0';
    }
    else if (i == 0)
    {
        dirpath = (char *)malloc(2);
        if (dirpath == NULL)
        {
            SYS_ALERT("%s: malloc error !\n", __func__);

            return NULL;
        }

        if (i == 0)
            dirpath[0] = '\\';
        else
            dirpath[0] = '.';

        dirpath[1] = '\0';
    }

    return dirpath;
}
#endif

static char *getfilename(char *path)
{
	char *file = NULL;
	int i;
	int len = strlen(path);

	i = len - 1;
	while(i >= 0)
	{
		if (path[i] == '/')
			break;

		i--;
	}

    file = (char *)malloc(len - i);
    if (file == NULL)
    {
        SYS_ALERT("%s: malloc error !\n", __func__);

        return NULL;
    }

	strcpy(file, &path[i + 1]);

	return file;
}

static inline char *get_filename(const char *path)
{
    int i;

    for(i = strlen(path) - 1; i > 0; i--)
    {
        if (path[i] == '/')
        {
            i++;
            break;
        }
    }

    return (char *)&path[i];
}

static inline char* get_basename(const char *filename)
{
    char *name = NULL;

    if (filename == NULL)
          return NULL;

    int i;
    for(i = strlen(filename) - 1; i >= 0; i--)
    {
        if (filename[i] == '.')
            break;
    }

    if (filename[i] == '.')
    {
        name = (char *)malloc(strlen(filename) - i + 1);
        if (name == NULL)
        {
            SYS_ALERT("%s: malloc error !\n", __func__);

            return NULL;
        }

        strncpy(name, filename, i);
        name[i + 1] = '\0';
    }

    return name;
}

static inline char* get_uffix(const char *filename)
{
    int i;

    if (filename == NULL
            || strlen(filename) == 0)
    {
        return NULL;
    }

    for(i = strlen(filename) - 1; i >= 0; i--)
    {
        if (filename[i] == '.')
            break;
    }

    if (filename[i] == '.'
            && i != (strlen(filename) - 1))
    {
        return (char *)&filename[i + 1];
    }

    return NULL;
}

#if 1//Q_OS_LINUX
static inline long get_file_size(const char *filename)
{
    unsigned long filesize = -1;
    struct stat statbuff;

    if(stat(filename, &statbuff) < 0)
    {
        return filesize;
    }
    else
    {
        filesize = statbuff.st_size;
    }

    return filesize;
}
#elif Q_OS_WIN
static inline long get_file_size(const char *filename)
{
    long size = -1;
    FILE *fp = fopen(filename, "rb");
    if(!fp)
        return -1;

    fseek(fp, 0L, SEEK_END);
    size=ftell(fp);
    fclose(fp);

    return size;
}
#else
static inline long get_file_size(const char *filename);
#endif

static int filecmp(const char *file1, const char *file2)
{
    int fd;
    size_t filesize;
    uint8_t *buf = NULL;

    uint8_t _md5[2][16];

    if (file1 == NULL
            || strlen(file1) <= 0
            || file2 == NULL
            || strlen(file2) <= 0)
        return -ERROR_PARAM_NULL;

    if ((access(file1, F_OK) != 0)
            || (access(file2, F_OK) != 0))
    {
        return -ERROR_NO_EXIST;
    }

    //File1
    filesize = get_file_size(file1);
    if (filesize <= 0)
    {
//        return -ERROR_DATA;
        return -10086;
    }

    fd = open(file1, O_RDONLY);
    if (fd < 0)
    {
        SYS_ERR("%s: open %s error(%d - %s)\n", __func__, file1, errno, strerror(errno));

        return -ERROR_OPEN;
    }

    buf = (uint8_t *)mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (buf == NULL)
    {
        SYS_ERR("%s: mmap %s error(%d - %s)\n", __func__, file1, errno, strerror(errno));

        close(fd);

        return -ERROR_READ;
    }

    md5(buf, filesize, _md5[0]);

    munmap(buf, filesize);
    close(fd);

    //File2
    filesize = get_file_size(file2);
    if (filesize <= 0)
    {
//        return -ERROR_DATA;
        return -10086;
    }

    fd = open(file2, O_RDONLY);
    if (fd < 0)
    {
        SYS_ERR("%s: open %s error(%d - %s)\n", __func__, file2, errno, strerror(errno));

        return -ERROR_OPEN;
    }

    buf = (uint8_t *)mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (buf == NULL)
    {
        SYS_ERR("%s: mmap %s error(%d - %s)\n", __func__, file2, errno, strerror(errno));

        close(fd);

        return -ERROR_READ;
    }

    md5(buf, filesize, _md5[1]);

    munmap(buf, filesize);
    close(fd);

#if 0
    int i;
    SYS_DEBUG("%s: ", file1);
    for(i = 0; i < 16; i ++)
        printf("%.2x", _md5[0][i]);
    printf("\n");

    SYS_DEBUG("%s: ", file2);
    for(i = 0; i < 16; i ++)
        printf("%.2x", _md5[1][i]);
    printf("\n");
#endif

    //
    return memcmp(_md5[0], _md5[1], 16);
}

static int filecp(const char *path_from, const char *path_to)
{
    int ret = 0;

    int fd;
    size_t filesize;
    uint8_t *buf = NULL;

    char *dirpath = NULL;

    if (path_from == NULL
            || strlen(path_from) <= 0
            || path_to == NULL
            || strlen(path_to) <= 0)
        return -ERROR_PARAM_NULL;

    if ((access(path_from, F_OK) != 0))
    {
        return -ERROR_NO_EXIST;
    }

    //File1
    filesize = get_file_size(path_from);
    if (filesize <= 0)
    {
//        return -ERROR_DATA;
        return -10086;
    }

    fd = open(path_from, O_RDONLY);
    if (fd < 0)
    {
        SYS_ERR("%s: open %s error(%d - %s)\n", __func__, path_from, errno, strerror(errno));

        return -ERROR_OPEN;
    }

    buf = (uint8_t *)mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (buf == NULL)
    {
        SYS_ERR("%s: mmap %s error(%d - %s)\n", __func__, path_from, errno, strerror(errno));

        close(fd);

        return -ERROR_READ;
    }

    dirpath = get_dirname(path_to);
    if (access(dirpath, F_OK) != 0)
    {
        char *cmd = (char *)malloc(strlen(dirpath) + 10);
        if (cmd == NULL)
        {
            munmap(buf, filesize);
            close(fd);

            return -ERROR_MEM;
        }

        snprintf(cmd, malloc_usable_size(cmd), "mkdir -p %s", dirpath);
        ret = system(cmd);
        free(cmd);
        if (0 != ret)
        {
            munmap(buf, filesize);
            close(fd);

            return -ERROR_CANNOT_ACCESS;
        }
    }

    int fd2 = open(path_to, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd2 < 0)
    {
        SYS_ERR("%s: open %s error(%d - %s)\n", __func__, path_to, errno, strerror(errno));

        munmap(buf, filesize);
        close(fd);

        return -ERROR_OPEN;
    }

    ret = write(fd2, buf, filesize);

    fsync(fd2);
    close(fd2);

    munmap(buf, filesize);
    close(fd);

    return ret;
}

static int64_t _gettime_relative(void)
{
    struct timespec ts;
    // 获取开机后，相对于开机时的时间戳
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

//获取录制实际剩余空间（sd卡剩余容量-录制文件时长系数）
static int64_t _getfree_recordreally(int64_t bfree,int64_t time_t)
{
    //1min - 60kb   1s - 1kb  1ms - 1b
    return bfree - time_t;
}

#ifdef __cplusplus
}
#endif

#endif //_FILES_H
