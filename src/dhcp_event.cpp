#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>

#include "general.h"

#include "osee_errors.h"
#include "osee_syslog.h"

#include "net_opr.h"
#include "dhcp_event.h"


extern int g_connect_epollfd;

DhcpEvent dhcp_event;

int g_dhcp_fd = -1;


DhcpEvent::DhcpEvent()
{
}

int DhcpEvent::dhcp_event_init()
{
    int ret = 0;

    g_dhcp_fd = inotify_init1(IN_NONBLOCK);
    if (g_dhcp_fd <= 0)
    {
        SYS_ERR("%s: inotify_init error: %s\n", __func__, strerror(errno));

        return g_dhcp_fd;
    }

    if (access(DNS_PATH, F_OK) != 0)
    {
        char cmd[128];
        sprintf(cmd, "mkdir -p %s",  DNS_PATH);
        system(cmd);
    }

    ret = inotify_add_watch(g_dhcp_fd, DNS_PATH, IN_CREATE | IN_DELETE | IN_MODIFY);
    if (ret < 0)
    {
        SYS_ERR("%s: inotify_add_watch(%d) error: %s\n",
                __func__,
                g_dhcp_fd,
                strerror(errno));

        return ret;
    }

    //set_nonblocking(g_dhcp_fd);

    if (g_connect_epollfd > 0)
    {
        addfd(g_connect_epollfd, g_dhcp_fd);
    }

    SYS_INFO("%s: add inotify %s (fd:%d)\n", __func__, DNS_PATH, g_dhcp_fd);

    return 0;
}

void DhcpEvent::dhcp_event_exit()
{
    if (g_dhcp_fd > 0)
    {
        if (g_connect_epollfd > 0)
            delfd(g_connect_epollfd, g_dhcp_fd);

        SYS_INFO("DhcpEvent disconnect from %s(%d)\n",
                DNS_PATH,
                g_dhcp_fd);

        close(g_dhcp_fd);
        g_dhcp_fd = -1;
    }
}

int DhcpEvent::dhcp_event(char *buf, int len)
{
    char *p, *p_end;
    struct inotify_event *event;
    char netif[16];
    char _ext[16];

    if (buf == NULL
            || len <= 0)
    {
        return -ERROR_NULL;
    }

    p = buf;
    p_end = &buf[len - 1];
    while((p < p_end)
          && ((p_end - p) >= sizeof(struct inotify_event)))
    {
        event = (struct inotify_event *)p;

        netif[0] = '\0';
        _ext[0] = '\0';
        if (sscanf(event->name, "%[^.]%s", netif, _ext) < 2)
        {
            p += sizeof(struct inotify_event) + event->len;
            continue;
        }

        if (strcmp(_ext, ".dhcp") != 0)
        {
            p += sizeof(struct inotify_event) + event->len;
            continue;
        }

        if (event->mask & IN_DELETE)
        {
            send_dhcp_change(false, netif);
        }
        else if ((event->mask & IN_CREATE)
                 || (event->mask & IN_MODIFY))
        {
            send_dhcp_change(true, netif);
        }

        p += sizeof(struct inotify_event) + event->len;
    }

    return 0;
}

