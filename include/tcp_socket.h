#ifndef _TCP_SOCKET_H
#define _TCP_SOCKET_H

#include <stdint.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "list.h"
#include "osee_time.h"

#include "pc_cmd.h"


#define TCP_IPPORT          2000

#define MAX_TCP_CLIENT_NUM  100
#define MAX_TCP_BUF_SIZE    2000


struct tcp_client_t {
        int fd;

        uint8_t buffer[MAX_TCP_BUF_SIZE];
        int buffer_len;

        struct timespec last_time;

        struct list_head entry;
};

extern int g_tcp_server_fd;
extern struct list_head g_tcp_client_list;

/**********************/
int tcp_pack_send(const int fd, uint8_t *data, int len);
int tcp_reply_pack_send(const int fd, uint8_t *data, int len);

void tcp_close(int fd);
int tcp_accept(const int fd);

int tcp_init();
void tcp_exit();

#ifdef __cplusplus
}
#endif

#endif //_TCP_SOCKET_H
