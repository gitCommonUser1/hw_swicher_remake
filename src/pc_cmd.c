#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include <arpa/inet.h>

#include "osee_time.h"
#include "osee_errors.h"
#include "osee_syslog.h"

#include "tcp_socket.h"

#include "pc_cmd.h"

#include "zlog.h"

struct msg_wait_t g_msg_wait;


/**
 * @brief get_live_version
 * @param time_out  =0:忽略(单位：毫秒)
 * @return
 */
int get_live_version(int fd, int time_out)
{
    struct timespec _start, _end, _step;
    uint8_t sendbuf[to_msg_len(0)];
    struct pc_ctrl_head_t *pc_ctrl = (struct pc_ctrl_head_t *)sendbuf;

    if (fd < 0)
        return -ERROR_DISCONNECT;

    pc_ctrl->header = htons(QT_CTRL_HEAD);
    pc_ctrl->gid = QT_CMD_GROUP_0;
    pc_ctrl->cid = QT_CMD_SET_SN;
    pc_ctrl->len = 0;

    if (time_out > 0)
    {
        msg_wait_clean(g_msg_wait);
        g_msg_wait.gid = pc_ctrl->gid;
        g_msg_wait.cid = pc_ctrl->cid;
    }

    tcp_pack_send(fd, sendbuf, get_msg_len(sendbuf));
    if (fd < 0)
    {
        if (time_out > 0)
            msg_wait_clean(g_msg_wait);;

        return -ERROR_DISCONNECT;
    }

    if (time_out > 0)
    {
        clock_gettime(CLOCK_BOOTTIME, &_start);

        ms2timespec(time_out, &_step);
        timespec_add(&_start, &_step, &_end);

        do {
            if (IS_MSG_WAIT_RETURN(g_msg_wait))
                break;

            usleep(1000);
            clock_gettime(CLOCK_BOOTTIME, &_start);
        } while(timespec_cmp(&_start, &_end, <));

        msg_wait_stop(g_msg_wait);

        if (! IS_MSG_WAIT_RETURN(g_msg_wait))
            return -ERROR_TIMEOUT;

        if (g_msg_wait.result != 0)
            return CODE_HEX2DEC(g_msg_wait.result);
    }

    return 0;
}

/**
 * @brief set_factory_reset
 * @param time_out  =0:忽略(单位：毫秒)
 * @return
 */
int set_factory_reset(int fd, int time_out)
{
    struct timespec _start, _end, _step;
    uint8_t sendbuf[to_msg_len(1)];
    struct pc_ctrl_head_t *pc_ctrl = (struct pc_ctrl_head_t *)sendbuf;

    if (fd < 0)
        return -ERROR_DISCONNECT;

    pc_ctrl->header = htons(QT_CTRL_HEAD);
    pc_ctrl->gid = QT_CMD_GROUP_0;
    pc_ctrl->cid = QT_CMD_SET_MAC;
    pc_ctrl->len = 1;

    pc_ctrl->data[0] = 1;

    if (time_out > 0)
    {
        msg_wait_clean(g_msg_wait);
        g_msg_wait.gid = pc_ctrl->gid;
        g_msg_wait.cid = pc_ctrl->cid;
    }

    tcp_pack_send(fd, sendbuf, get_msg_len(sendbuf));
    if (fd < 0)
    {
        if (time_out > 0)
            msg_wait_clean(g_msg_wait);;

        return -ERROR_DISCONNECT;
    }

    if (time_out > 0)
    {
        clock_gettime(CLOCK_BOOTTIME, &_start);

        ms2timespec(time_out, &_step);
        timespec_add(&_start, &_step, &_end);

        do {
            if (IS_MSG_WAIT_RETURN(g_msg_wait))
                break;

            usleep(1000);
            clock_gettime(CLOCK_BOOTTIME, &_start);
        } while(timespec_cmp(&_start, &_end, <));

        msg_wait_stop(g_msg_wait);

        if (! IS_MSG_WAIT_RETURN(g_msg_wait))
            return -ERROR_TIMEOUT;

        if (g_msg_wait.result != 0)
            return CODE_HEX2DEC(g_msg_wait.result);
    }

    return 0;
}
