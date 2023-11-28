#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <net/if.h>
#include <net/route.h>
#include <assert.h>

#include "general.h"

#include "osee_errors.h"
#include "osee_syslog.h"

#include "pc_cmd.h"

#include "tcp_socket.h"

extern int g_connect_epollfd;

int g_tcp_server_fd = -1;
pthread_mutex_t tcp_mutex;

struct list_head g_tcp_client_list;


int tcp_pack_send(const int fd, uint8_t *data, int len)
{
    int ret = 0;

    if (fd <= 0)
        return -ERROR_SOCKET;

    if (data == NULL
            || len <= 0)
        return -ERROR_PARAM_NULL;

    pack_act(data);
    ret = send((fd), (data), get_msg_len(data), MSG_NOSIGNAL);
    if (ret <= 0)
    {
        tcp_close(fd);
    }

    return ret;
}

int tcp_reply_pack_send(const int fd, uint8_t *data, int len)
{
    int ret = 0;

    if (fd <= 0)
        return -ERROR_SOCKET;

    if (data == NULL
            || len <= 0)
        return -ERROR_PARAM_NULL;

    pack_reply_act(data);
    ret = send((fd), (data), get_re_msg_len(data), MSG_NOSIGNAL);
    if (ret <= 0)
    {
        tcp_close(fd);
    }

    return ret;
}

void tcp_close(int fd)
{
    struct tcp_client_t *client;

    if (fd <= 0)
        return;

    if (g_connect_epollfd > 0)
        delfd(g_connect_epollfd, fd);

    close(fd);

    SYS_DEBUG("tcp disconnect:socket:%d\n", fd);

    list_for_each_entry(client, &g_tcp_client_list, entry)
    {
        if (client->fd == fd)
        {
            list_del(&client->entry);

            free(client);

            break;
        }
    }
}

int tcp_accept(const int fd)
{
    int socket_fd = 0;
    struct sockaddr_in client_addr;
    socklen_t client_addrlen = sizeof(client_addr);
    bzero(&client_addr, sizeof(client_addr));

    struct tcp_client_t *client = NULL;

    int ret = 0;

    socket_fd = accept(fd, (struct sockaddr *)&client_addr, &client_addrlen);
    if(socket_fd < 0)
    {
        perror("accept");
    }
    else
    {
        int num = 0;

        list_for_each_entry(client, &g_tcp_client_list, entry)
        {
            num++;
        }

        if(num >= MAX_TCP_CLIENT_NUM)
        {
            close(socket_fd);
            SYS_ERR("ERROR TCP too many client (%d/%d)\n", num, MAX_TCP_CLIENT_NUM);

            return -ERROR_OUTRANGE;
        }

        client = malloc(sizeof(struct tcp_client_t));
        if (client == NULL)
        {
            close(socket_fd);

            return -ERROR_MEM;
        }

        ret = addfd(g_connect_epollfd, socket_fd);
        if (ret < 0)
        {
            SYS_ERR("%s: addfd:%d error(%d-%d)\n", __func__, socket_fd, ret, errno);

            close(socket_fd);

            free(client);

            return ret;
        }

        client->fd = socket_fd;
        client->buffer_len = 0;
        timespec_clear(&client->last_time);

        list_add(&client->entry, &g_tcp_client_list);

        SYS_DEBUG("tcp connect success:socket:%d (%s:%d)\n", socket_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    }

    return socket_fd;
}


int tcp_init(void)
{
    int ret = 0;
    struct sockaddr_in server_addr;

    g_tcp_server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(g_tcp_server_fd < 0)
    {
        perror("tcp socket");
        return -ERROR_SOCKET;
    }
    SYS_DEBUG("g_tcp_server_fd = %d\n", g_tcp_server_fd);

    ret = 1;
    setsockopt(g_tcp_server_fd, SOL_SOCKET, SO_REUSEADDR, &ret, sizeof(ret));

    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(TCP_IPPORT);

    ret = bind(g_tcp_server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ;
    if(ret < 0)
    {
        perror("tcp bind");
        return -ERROR_BIND;
    }
    SYS_DEBUG("TCP bind@ip=%s:%d\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));

    ret = listen(g_tcp_server_fd, 20);
    if(ret < 0)
    {
        perror("tcp listen");
        return -ERROR_LISTEN;
    }

    INIT_LIST_HEAD(&g_tcp_client_list);

    if (g_connect_epollfd > 0)
        addfd(g_connect_epollfd, g_tcp_server_fd);

    return g_tcp_server_fd;
}

/**
 * @brief tcp_exit
 */
void tcp_exit()
{
    struct tcp_client_t *client;

	while (! list_empty(&g_tcp_client_list))
    {
		client = list_first_entry(&g_tcp_client_list, struct tcp_client_t, entry);

        delfd(g_connect_epollfd, client->fd);

        list_del(&client->entry);

        free(client);
    }

    if (g_tcp_server_fd > 0)
    {
        if (g_connect_epollfd > 0)
            delfd(g_connect_epollfd, g_tcp_server_fd);

        close(g_tcp_server_fd);
        g_tcp_server_fd = -1;
    }

    pthread_mutex_destroy(&tcp_mutex);
}
