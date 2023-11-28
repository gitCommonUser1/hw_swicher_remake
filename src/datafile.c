#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/stat.h>

#include "files.h"

#include "crc32.h"
#include "general.h"

#include "osee_syslog.h"
#include "osee_errors.h"

#include "datafile.h"



/******************************* FILE OPR *************************/
void get_file_path(const char *path, const char *filename, char *filepath)
{
    strcpy(filepath, path);

    if(filepath[strlen(path) - 1] != '/')
        strcat(filepath, "/");

    strcat(filepath, filename);
}

/**
 * @brief read_file_data
 * @param filename
 * @param p_file_data
 * @return
 *
 * Must use FILL_FILEDATA_BUFSIZE() first
 */
int read_file_data(const char *filename, struct file_data_t *p_file_data)
{
    size_t data_size = 0;
    size_t read_size = 0;
    size_t rsize = 0;

    if (filename == NULL
            || strlen(filename) == 0
            || NULL == p_file_data)
    {
        return -ERROR_PARAM_NULL;
    }

    if (0xFFFFFFFF == p_file_data->len
            || 0 == p_file_data->len)
    {
        return -ERROR_PARAM_NULL;
    }

    data_size = p_file_data->len;

    if(access(filename, F_OK) < 0)
    {
        return -ERROR_NO_EXIST;
    }

    FILE *fp = fopen(filename, "rb");
    if(fp == NULL)
    {
        SYS_ERR("%s: fopen %s error(%d - %s)\n", __func__, filename, errno, strerror(errno));

        return -ERROR_OPEN;
    }

    read_size = sizeof(struct file_data_t);
    rsize = fread((uint8_t *)p_file_data, 1, read_size, fp);
    if(rsize < read_size)
    {
        SYS_ERR("%s: fread1 %s fail(%d/%d) (%d-%s)\n", __func__, filename, rsize, read_size, errno, strerror(errno));

        fclose(fp);

        return -ERROR_READ;
    }

    if (p_file_data->len == 0xFFFFFFFF)
    {
        fclose(fp);

        return -ERROR_DATA;
    }

    if (p_file_data->len > data_size)
    {
        SYS_ERR("%s: %s size:%d/%d\n",
                __func__, filename,
                p_file_data->len, data_size);

        p_file_data->len = data_size;
    }

    rsize = fread((uint8_t *)p_file_data->data, 1, p_file_data->len, fp);
    if(rsize < p_file_data->len)
    {
        SYS_ERR("%s: fread2 %s fail(%d/%d) (%d-%s)\n", __func__, filename, rsize, p_file_data->len, errno, strerror(errno));

        fclose(fp);
        return -ERROR_READ;
    }

    fclose(fp);

    return check_crc(p_file_data, get_file_size(filename));
}

/**
 * @brief write_file_data
 * @param filename
 * @param p_file_data
 * @return
 */
int write_file_data(const char *filename, struct file_data_t *p_file_data)
{
    size_t write_size = 0;
    size_t wsize = 0;
    int fd = 0;

    if (filename == NULL
            || strlen(filename) == 0
            || NULL == p_file_data)
    {
        return -ERROR_PARAM_NULL;
    }

    if(access(filename, F_OK) < 0)
    {
        /* 若文件所在目录不存在，则创建目录 */
        char *dirpath = get_dirname(filename);
        if (dirpath == NULL)
        {
            return -ERROR_MEM;
        }

        if(access(dirpath, F_OK) < 0)
        {
            char cmd[128];

            sprintf(cmd, "mkdir -p %s", dirpath);
            if (system(cmd) < 0)
            {
                SYS_ERR("%s: %s ERROR(%d-%s)\n", __func__, cmd, errno, strerror(errno));

                return -ERROR_WRITE;
            }
        }

        free(dirpath);
        dirpath = NULL;
    }

    FILE *fp = fopen(filename, "wb");
    if(fp == NULL)
    {
        SYS_ERR("%s: fopen %s fail\n", __func__, filename);

        return -ERROR_OPEN;
    }

    fill_crc(p_file_data);

    write_size = sizeof(struct file_data_t) + p_file_data->len;

    wsize = fwrite((uint8_t *)p_file_data, 1, write_size, fp);

    fflush(fp);

    fd = fileno(fp);
    if(fd != -1)
    {
        fsync(fd);
    }

    fclose(fp);

    if(wsize < write_size)
    {
        SYS_ERR("%s: fwrite %s fail(%d/%d)\n", __func__, filename, wsize, write_size);

        return -ERROR_WRITE;
    }

    return 0;
}

//int delete_file(const char *path)
//{
//    int ret = 0;
//    int status = 0;
//    int i;

//    DIR *dir;
//    struct dirent *dirinfo;
//    struct stat statbuf;

//    char pwdpath[PATH_LENGTH] = {0};

//    if (NULL == path)
//    {
//        return -ERROR_PARAM_NULL;
//    }

//    if (path[strlen(path) - 1] == '*')
//    {
//        char *delpath = (char *)malloc(strlen(path));
//        if (delpath == NULL)
//        {
//            SYS_ALERT("%s: malloc error(%d-%s) !\n", __func__, errno, strerror(errno));

//            return -ERROR_MEM;
//        }

//        memmove(delpath, path, strlen(path) - 1);
//        delpath[strlen(path) - 1] = '\0';

//        for(i = strlen(delpath) - 1; i >= 0; i--)
//        {
//            if (path[i] != '*' && path[i] != '/')
//                break;
//        }
//        delpath[i+1] = '\0';

//        if(access(delpath, F_OK) < 0)
//        {
//            if(delpath)
//                free(delpath);
//            return 0;
//        }

//        /*lstat(delpath, &statbuf);

//        if(! S_ISDIR(statbuf.st_mode))
//        {
//            free(delpath);
//            return 0;
//        }*/

//        if((dir = opendir(delpath)) == NULL)
//        {
//            if(delpath)
//                free(delpath);
//            return -ERROR_OPEN;
//        }

//        getcwd(pwdpath, sizeof(pwdpath));

//        chdir(delpath);

//        while((dirinfo = readdir(dir)) != NULL)
//        {
//            if(strcmp(dirinfo->d_name, ".") == 0
//                    || strcmp(dirinfo->d_name, "..") == 0)
//                continue;

//            ret = delete_file(dirinfo->d_name);
//            if (ret < 0)
//            {
//                status = ret;
//            }
//        }

//        chdir(pwdpath);

//        closedir(dir);

//        sync();

//        if(delpath)
//            free(delpath);
//    }

//    if(access(path, F_OK) < 0)
//        return 0;

//    lstat(path, &statbuf);

//    if(S_ISREG(statbuf.st_mode))
//    {
//        //SYS_DEBUG("%s: remove file: %s\n", __func__, path);

//        if (remove(path) < 0)
//        {
//            SYS_ERR("%s: remove %s ERROR(%d-%s)\n", __func__, path, errno, strerror(errno));

//            status = -ERROR_WRITE;
//        }
//    }
//    else if(S_ISDIR(statbuf.st_mode))
//    {
//        if((dir = opendir(path)) == NULL)
//            return -ERROR_OPEN;

//        getcwd(pwdpath, sizeof(pwdpath));

//        chdir(path);

//        while((dirinfo = readdir(dir)) != NULL)
//        {
//            if(strcmp(dirinfo->d_name, ".") == 0
//                    || strcmp(dirinfo->d_name, "..") == 0)
//                continue;

//            ret = delete_file(dirinfo->d_name);
//            if (ret < 0)
//            {
//                status = ret;
//            }
//        }

//        chdir(pwdpath);
//        closedir(dir);

//        sync();

//        //SYS_DEBUG("%s: rmdir=%s\n", __func__, path);

//        if (rmdir(path) < 0)
//        {
//            SYS_ERR("%s: rmdir %s ERROR(%d-%s)\n", __func__, path, errno, strerror(errno));

//            status = -ERROR_WRITE;
//        }
//    }

//    return status;
//}

