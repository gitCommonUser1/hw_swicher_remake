#ifndef TIMEZONE_H
#define TIMEZONE_H

#include "include/general.h"

static void setTimeZone()
{
    int ret = 0;
    char cmd[64];
    char result[64] = {0};

    sprintf(cmd,"curl -k https://ipapi.co/timezone");
    ret = executeCMD(cmd,result,64);
    if (ret != 0)
       return ;

    if(strlen(result) == 0)
        return ;

    sprintf(cmd,"rm -f /etc/localtime");
    system(cmd);

    printf("_____result:%s\n",result);

    sprintf(cmd,"ln -s /usr/share/zoneinfo/%s  /etc/localtime  ",result);
    system(cmd);

    printf("_____execute:%s\n",cmd);

    sprintf(cmd,"hwclock -w -u&");
    system(cmd);
}


const char *ntpServers[] = {
    "0.pool.ntp.org",
    "1.pool.ntp.org",
    "0.freebsd.pool.ntp.org",
    "1.freebsd.pool.ntp.org",
    "0.fedora.pool.ntp.org",
    "1.fedora.pool.ntp.org",
    "0.centos.pool.ntp.org",
    "1.centos.pool.ntp.org",
    "0.debian.pool.ntp.org",
    "1.debian.pool.ntp.org",
    "0.netbsd.pool.ntp.org",
    "1.netbsd.pool.ntp.org",
    "0.android.pool.ntp.org",
    "1.android.pool.ntp.org",
};
int ntpServerLength = sizeof(ntpServers) / sizeof(const char *);

static void ntp_date_reset_thread()
{
    char cmd[256] = "";
    for(int i = 0;i < ntpServerLength;++i)
    {
        auto date = ntpServers[i];
        sprintf(cmd,"ntpdate %s ",date);
        int ret = system(cmd);
        if(ret == 0)
            break;
    }
    sprintf(cmd,"hwclock -w -u&");
    system(cmd);
}


#endif // TIMEZONE_H
