#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include <QMetaType>
#include <QThread>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <sys/select.h>
#include <sys/un.h>
#include <errno.h>
#include <assert.h>

#include "osee_syslog.h"
#include "osee_errors.h"

#include "qthread_connect.h"

#include "tcp_socket.h"

#include "pccmd.h"

#define USE_DHCP

#ifdef USE_WPA
#include "wpa.h"
#endif
#ifdef USE_DHCP
#include "dhcp_event.h"
#endif

#include "zlog.h"


#define MAX_EVENT_NUM   (5 + MAX_TCP_CLIENT_NUM)

#define CALIBRATION_MSGSIZE     300


int g_connect_epollfd = -1;

extern bool g_running;


QThreadConnect::QThreadConnect(QObject *parent) : QThread(parent)
{
    g_connect_epollfd = epoll_create(MAX_EVENT_NUM);

    qRegisterMetaType<uint8_t>("uint8_t");
}

QThreadConnect::~QThreadConnect()
{
}

void QThreadConnect::run()
{
    int i, len;
    int ret = 0;

#ifdef USE_DHCP || USE_WPA
    uint8_t *buf = (uint8_t *)malloc(CALIBRATION_MSGSIZE);
#endif

    //epoll
    int con = 0;
    struct epoll_event events[MAX_EVENT_NUM];

    assert(g_connect_epollfd != -1);

    tcp_init();

#ifdef USE_WPA
    wpa.wpa_init();
    wpa.check_status();
#endif

#ifdef USE_DHCP
    dhcp_event.dhcp_event_init();
#endif

    SYS_NOTICE("QThreadConnect start !\n");

    while (g_running)
    {
        con = epoll_wait(g_connect_epollfd, events, MAX_EVENT_NUM, -1);
        if (con < 0)
        {
            SYS_ERR("%s:%d: epoll failure errno:%d\n", __FILE__, __LINE__, errno);
            continue;
        }

        for(i = 0; i < con; i++)
        {
            if (events[i].events & EPOLLERR)
            {
                continue;
            }
            else if (events[i].events & EPOLLIN
                     || events[i].events & EPOLLHUP
                     || events[i].events & EPOLLRDHUP)
            {
                if (g_tcp_server_fd > 0
                         && (g_tcp_server_fd == events[i].data.fd))
                {
                    tcp_accept(g_tcp_server_fd);
                }
#ifdef USE_WPA
                else if (g_wpa_fd > 0
                         && (g_wpa_fd == events[i].data.fd))
                {
                    len = read(g_wpa_fd, buf, CALIBRATION_MSGSIZE);
                    if (len < 0)
                    {
                        SYS_ERR("Read(%d) error: %s\n", g_wpa_fd, strerror(errno));
                    }
                    else if (len > 0)
                    {
                        wpa.wpa_inotify((char *)buf, len);
                    }
                }
#endif

#ifdef USE_DHCP
                else if (g_dhcp_fd > 0
                         && (g_dhcp_fd == events[i].data.fd))
                {
                    len = read(g_dhcp_fd, buf, CALIBRATION_MSGSIZE);
                    if (len < 0)
                    {
                        SYS_ERR("Read(%d) error: %s\n", g_dhcp_fd, strerror(errno));
                    }
                    else if (len > 0)
                    {
                        dhcp_event.dhcp_event((char *)buf, len);
                    }
                }
#endif
                else
                {
                    struct tcp_client_t *client;
                    list_for_each_entry(client, &g_tcp_client_list, entry)
                    {
                        if (client->fd == events[i].data.fd)
                        {
                            ret = do_tcp_process(client);
                            if (ret == -ERROR_DISCONNECT)
                            {
                                tcp_close(client->fd);
                            }

                            break;
                        }
                    }
                }
            }
        }
    }

    tcp_exit();

#ifdef USE_DHCP
    dhcp_event.dhcp_event_exit();
#endif

#ifdef USE_WPA
    wpa.wpa_exit();
#endif

    sync();

    SYS_NOTICE("QThreadConnect exit !\n");
}

