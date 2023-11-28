#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include <arpa/inet.h>

#include "osee_errors.h"
#include "osee_syslog.h"

#include "tcp_socket.h"

#include "pc_cmd.h"
#include "pccmd.h"

#include "zlog.h"

#include "device_info.h"


PcCmd pccmd;


PcCmd::PcCmd()
{

}

int PcCmd::_set_sn(const int fd, uint8_t *buf, int len)
{
    uint8_t *sendbuf = NULL;
    int sendbuf_size = 0;

    struct pc_ctrl_head_t *pc_ctrl = NULL;
    struct re_pc_ctrl_head_t *pc_re = NULL;

    struct pc_str8_t *p_sn = NULL;
    int ret = 0;

	if (buf == NULL
		|| len < sizeof(struct pc_ctrl_head_t))
		return -ERROR_CMD_SHORT;

    pc_ctrl = (struct pc_ctrl_head_t *)buf;

    sendbuf_size = to_remsg_len(0);
    sendbuf = (uint8_t *)malloc(sendbuf_size);
    if (sendbuf == NULL)
    {
        return -ERROR_MEM;
    }

    pc_re = (struct re_pc_ctrl_head_t *)sendbuf;

    pc_re->header = htons(QT_CTRL_HEAD);
    pc_re->gid = QT_CMD_GROUP_0;
    pc_re->cid = QT_CMD_SET_SN;
    pc_re->re_type = CMD_SET_SN;
    pc_re->result = 0;
    pc_re->len = htonl(0);

    if (QT_CMD_GROUP_0 != pc_ctrl->gid
        || QT_CMD_SET_SN != pc_ctrl->cid)
    {
        pc_re->result = -ERROR_CMD_UNSUPPORT;

        ret = tcp_reply_pack_send(fd, sendbuf, sendbuf_size);
        free(sendbuf);
        if (ret < 0)
            return ret;

        return CODE_HEX2DEC(pc_re->result);
	}

    int data_len = get_msg_data_len(buf);

    if (data_len < sizeof(struct pc_str8_t))
    {
        pc_re->result = -ERROR_CMD_SHORT;

        goto _SET_SN_ERROR;
    }

    p_sn = (struct pc_str8_t *)pc_ctrl->data;

    if (data_len < (sizeof (struct pc_str8_t) + p_sn->len))
    {
        pc_re->result = -ERROR_CMD_SHORT;

        goto _SET_SN_ERROR;
    }

    p_sn->data[p_sn->len] = '\0';
    ret = set_device_id((char *)p_sn->data);

    if (ret != 0)
    {
        pc_re->result = ret;

        goto _SET_SN_ERROR;
    }
    else
    {
        emit emit_sn_change((char *)p_sn->data);
    }

_SET_SN_ERROR:
    ret = tcp_reply_pack_send(fd, sendbuf, sendbuf_size);
    free(sendbuf);
    if (ret < 0)
        return ret;

    return CODE_HEX2DEC(pc_re->result);
}

int PcCmd::_set_mac(const int fd, uint8_t *buf, int len)
{
    uint8_t *sendbuf = NULL;
    int sendbuf_size = 0;

    struct pc_ctrl_head_t *pc_ctrl = NULL;
    struct re_pc_ctrl_head_t *pc_re = NULL;

    int ret = 0;

    if (buf == NULL
        || len < sizeof(struct pc_ctrl_head_t))
        return -ERROR_CMD_SHORT;

    pc_ctrl = (struct pc_ctrl_head_t *)buf;

    sendbuf_size = to_remsg_len(0);
    sendbuf = (uint8_t *)malloc(sendbuf_size);
    if (sendbuf == NULL)
    {
        return -ERROR_MEM;
    }

    pc_re = (struct re_pc_ctrl_head_t *)sendbuf;

    pc_re->header = htons(QT_CTRL_HEAD);
    pc_re->gid = QT_CMD_GROUP_0;
    pc_re->cid = QT_CMD_SET_MAC;
    pc_re->re_type = CMD_SET_MAC;
    pc_re->result = 0;
    pc_re->len = htonl(0);

    if (QT_CMD_GROUP_0 != pc_ctrl->gid
        || QT_CMD_SET_MAC != pc_ctrl->cid)
    {
        pc_re->result = -ERROR_CMD_UNSUPPORT;

        ret = tcp_reply_pack_send(fd, sendbuf, sendbuf_size);
        free(sendbuf);
        if (ret < 0)
            return ret;

        return CODE_HEX2DEC(pc_re->result);
    }

    int data_len = get_msg_data_len(buf);

    if (data_len < 6)
    {
        pc_re->result = -ERROR_CMD_SHORT;

        goto _SET_MAC_ERROR;
    }

    ret = set_device_mac(pc_ctrl->data);

    if (ret != 0)
    {
        pc_re->result = ret;

        goto _SET_MAC_ERROR;
    }
    /*else
    {
        system("/etc/init.d/S40netmac");

        emit emit_mac_change(pc_ctrl->data);
    }*/

_SET_MAC_ERROR:
    ret = tcp_reply_pack_send(fd, sendbuf, sendbuf_size);
    free(sendbuf);
    if (ret < 0)
        return ret;

    return CODE_HEX2DEC(pc_re->result);
}

int PcCmd::_get_sn(const int fd, uint8_t *buf, int len)
{
    uint8_t *sendbuf = NULL;
    int sendbuf_size = 0;

    struct pc_ctrl_head_t *pc_ctrl = NULL;
    struct re_pc_ctrl_head_t *pc_re = NULL;

    struct pc_str8_t *p_sn = NULL;
    int ret = 0;

    if (buf == NULL
        || len < sizeof(struct pc_ctrl_head_t))
        return -ERROR_CMD_SHORT;

    pc_ctrl = (struct pc_ctrl_head_t *)buf;

    sendbuf_size = to_remsg_len(64);
    sendbuf = (uint8_t *)malloc(sendbuf_size);
    if (sendbuf == NULL)
    {
        return -ERROR_MEM;
    }

    pc_re = (struct re_pc_ctrl_head_t *)sendbuf;

    pc_re->header = htons(QT_CTRL_HEAD);
    pc_re->gid = QT_CMD_GROUP_0;
    pc_re->cid = QT_CMD_GET_SN;
    pc_re->re_type = CMD_GET_SN;
    pc_re->result = 0;
    pc_re->len = htonl(0);

    if (QT_CMD_GROUP_0 != pc_ctrl->gid
        || QT_CMD_GET_SN != pc_ctrl->cid)
    {
        pc_re->result = -ERROR_CMD_UNSUPPORT;

        ret = tcp_reply_pack_send(fd, sendbuf, sendbuf_size);
        free(sendbuf);
        if (ret < 0)
            return ret;

        return CODE_HEX2DEC(pc_re->result);
    }

    /*int data_len = get_msg_data_len(buf);

    if (data_len < sizeof(struct pc_str8_t))
    {
        pc_re->result = -ERROR_CMD_SHORT;

        goto _SET_SN_ERROR;
    }*/

    char *sn = get_device_id();

    p_sn = (struct pc_str8_t *)pc_re->data;
    p_sn->len = 0;

    if (sn)
    {
        p_sn->len = strlen(sn);
        memmove(p_sn->data, sn, p_sn->len);

        free(sn);
    }

    pc_re->len = htonl(sizeof(struct pc_str8_t) + p_sn->len);

    ret = tcp_reply_pack_send(fd, sendbuf, sendbuf_size);
    free(sendbuf);
    if (ret < 0)
        return ret;

    return CODE_HEX2DEC(pc_re->result);
}

int PcCmd::_get_mac(const int fd, uint8_t *buf, int len)
{
    uint8_t *sendbuf = NULL;
    int sendbuf_size = 0;

    struct pc_ctrl_head_t *pc_ctrl = NULL;
    struct re_pc_ctrl_head_t *pc_re = NULL;

    int ret = 0;

    if (buf == NULL
        || len < sizeof(struct pc_ctrl_head_t))
        return -ERROR_CMD_SHORT;

    pc_ctrl = (struct pc_ctrl_head_t *)buf;

    sendbuf_size = to_remsg_len(6);
    sendbuf = (uint8_t *)malloc(sendbuf_size);
    if (sendbuf == NULL)
    {
        return -ERROR_MEM;
    }

    pc_re = (struct re_pc_ctrl_head_t *)sendbuf;

    pc_re->header = htons(QT_CTRL_HEAD);
    pc_re->gid = QT_CMD_GROUP_0;
    pc_re->cid = QT_CMD_GET_MAC;
    pc_re->re_type = CMD_GET_MAC;
    pc_re->result = 0;
    pc_re->len = htonl(0);

    if (QT_CMD_GROUP_0 != pc_ctrl->gid
        || QT_CMD_GET_MAC != pc_ctrl->cid)
    {
        pc_re->result = -ERROR_CMD_UNSUPPORT;

        ret = tcp_reply_pack_send(fd, sendbuf, sendbuf_size);
        free(sendbuf);
        if (ret < 0)
            return ret;

        return CODE_HEX2DEC(pc_re->result);
    }

    /*int data_len = get_msg_data_len(buf);

    if (data_len < 6)
    {
        pc_re->result = -ERROR_CMD_SHORT;

        goto _SET_MAC_ERROR;
    }*/

    ret = get_device_mac(pc_re->data);
    if (ret < 0)
        pc_re->result = ret;

    pc_re->len = htonl(6);

    ret = tcp_reply_pack_send(fd, sendbuf, sendbuf_size);
    free(sendbuf);
    if (ret < 0)
        return ret;

    return CODE_HEX2DEC(pc_re->result);
}

int PcCmd::_get_id(const int fd, uint8_t *buf, int len)
{
    uint8_t *sendbuf = NULL;
    int sendbuf_size = 0;

    struct pc_ctrl_head_t *pc_ctrl = NULL;
    struct re_pc_ctrl_head_t *pc_re = NULL;

    struct pc_str8_t *p_id = NULL;
    struct pc_str8_t *p_id2 = NULL;
    int ret = 0;

    if (buf == NULL
        || len < sizeof(struct pc_ctrl_head_t))
        return -ERROR_CMD_SHORT;

    pc_ctrl = (struct pc_ctrl_head_t *)buf;

    sendbuf_size = to_remsg_len((sizeof(struct pc_str8_t) + 16) * 2);
    sendbuf = (uint8_t *)malloc(sendbuf_size);
    if (sendbuf == NULL)
    {
        return -ERROR_MEM;
    }

    pc_re = (struct re_pc_ctrl_head_t *)sendbuf;

    pc_re->header = htons(QT_CTRL_HEAD);
    pc_re->gid = QT_CMD_GROUP_0;
    pc_re->cid = QT_CMD_GET_ID;
    pc_re->re_type = CMD_GET_ID;
    pc_re->result = 0;
    pc_re->len = htonl(0);

    if (QT_CMD_GROUP_0 != pc_ctrl->gid
        || QT_CMD_GET_ID != pc_ctrl->cid)
    {
        pc_re->result = -ERROR_CMD_UNSUPPORT;

        ret = tcp_reply_pack_send(fd, sendbuf, sendbuf_size);
        free(sendbuf);
        if (ret < 0)
            return ret;

        return CODE_HEX2DEC(pc_re->result);
    }

    /*int data_len = get_msg_data_len(buf);

    if (data_len < sizeof(struct pc_str8_t))
    {
        pc_re->result = -ERROR_CMD_SHORT;

        goto _SET_SN_ERROR;
    }*/

    uint64_t id = get_cpu_serial();
    uint32_t id2 = get_fpga_id();

    p_id = (struct pc_str8_t *)pc_re->data;
    sprintf((char *)p_id->data, "%.16llX", id);
    p_id->len = strlen((char *)p_id->data);

    p_id2 = (struct pc_str8_t *)&p_id->data[p_id->len];
    sprintf((char *)p_id2->data, "%.8X", id2);
    p_id2->len = strlen((char *)p_id2->data);

    pc_re->len = htonl(sizeof(struct pc_str8_t) * 2 + p_id->len + p_id2->len);

    ret = tcp_reply_pack_send(fd, sendbuf, sendbuf_size);
    free(sendbuf);
    if (ret < 0)
        return ret;

    return CODE_HEX2DEC(pc_re->result);
}

int PcCmd::_set_license(const int fd, uint8_t *buf, int len)
{
    uint8_t *sendbuf = NULL;
    int sendbuf_size = 0;

    struct pc_ctrl_head_t *pc_ctrl = NULL;
    struct re_pc_ctrl_head_t *pc_re = NULL;

    int ret = 0;

    if (buf == NULL
        || len < sizeof(struct pc_ctrl_head_t))
        return -ERROR_CMD_SHORT;

    pc_ctrl = (struct pc_ctrl_head_t *)buf;

    sendbuf_size = to_remsg_len(0);
    sendbuf = (uint8_t *)malloc(sendbuf_size);
    if (sendbuf == NULL)
    {
        return -ERROR_MEM;
    }

    pc_re = (struct re_pc_ctrl_head_t *)sendbuf;

    pc_re->header = htons(QT_CTRL_HEAD);
    pc_re->gid = QT_CMD_GROUP_0;
    pc_re->cid = QT_CMD_SET_LICENSE;
    pc_re->re_type = CMD_SET_LICENSE;
    pc_re->result = 0;
    pc_re->len = htonl(0);

    if (QT_CMD_GROUP_0 != pc_ctrl->gid
        || QT_CMD_SET_LICENSE != pc_ctrl->cid)
    {
        pc_re->result = -ERROR_CMD_UNSUPPORT;

        ret = tcp_reply_pack_send(fd, sendbuf, sendbuf_size);
        free(sendbuf);
        if (ret < 0)
            return ret;

        return CODE_HEX2DEC(pc_re->result);
    }

    int data_len = get_msg_data_len(buf);

    ret = set_license(pc_ctrl->data, data_len);
    if (ret < 0)
    {
        pc_re->result = ret;
    }

    ret = tcp_reply_pack_send(fd, sendbuf, sendbuf_size);
    free(sendbuf);
    if (ret < 0)
        return ret;

    return CODE_HEX2DEC(pc_re->result);
}

int PcCmd::_get_license(const int fd, uint8_t *buf, int len)
{
    uint8_t *sendbuf = NULL;
    int sendbuf_size = 0;

    struct pc_ctrl_head_t *pc_ctrl = NULL;
    struct re_pc_ctrl_head_t *pc_re = NULL;

    int ret = 0;

    if (buf == NULL
        || len < sizeof(struct pc_ctrl_head_t))
        return -ERROR_CMD_SHORT;

    pc_ctrl = (struct pc_ctrl_head_t *)buf;

    sendbuf_size = to_remsg_len(256);
    sendbuf = (uint8_t *)malloc(sendbuf_size);
    if (sendbuf == NULL)
    {
        return -ERROR_MEM;
    }

    pc_re = (struct re_pc_ctrl_head_t *)sendbuf;

    pc_re->header = htons(QT_CTRL_HEAD);
    pc_re->gid = QT_CMD_GROUP_0;
    pc_re->cid = QT_CMD_GET_LICENSE;
    pc_re->re_type = CMD_GET_LICENSE;
    pc_re->result = 0;
    pc_re->len = htonl(0);

    if (QT_CMD_GROUP_0 != pc_ctrl->gid
        || QT_CMD_GET_LICENSE != pc_ctrl->cid)
    {
        pc_re->result = -ERROR_CMD_UNSUPPORT;

        ret = tcp_reply_pack_send(fd, sendbuf, sendbuf_size);
        free(sendbuf);
        if (ret < 0)
            return ret;

        return CODE_HEX2DEC(pc_re->result);
    }

    int data_len = get_msg_data_len(buf);

    uint8_t *lic = NULL;
    int lic_size = 0;
    ret = get_license(&lic, &lic_size);
    if (ret < 0)
    {
        pc_re->result = ret;
    }

    pc_re->len = htonl(lic_size);
    memmove(pc_re->data, lic, lic_size);

    ret = tcp_reply_pack_send(fd, sendbuf, sendbuf_size);
    free(sendbuf);
    if (ret < 0)
        return ret;

    return CODE_HEX2DEC(pc_re->result);
}

int PcCmd::_get_license_status(const int fd, uint8_t *buf, int len)
{
    uint8_t *sendbuf = NULL;
    int sendbuf_size = 0;

    struct pc_ctrl_head_t *pc_ctrl = NULL;
    struct re_pc_ctrl_head_t *pc_re = NULL;

    int ret = 0;

    if (buf == NULL
        || len < sizeof(struct pc_ctrl_head_t))
        return -ERROR_CMD_SHORT;

    pc_ctrl = (struct pc_ctrl_head_t *)buf;

    sendbuf_size = to_remsg_len(1);
    sendbuf = (uint8_t *)malloc(sendbuf_size);
    if (sendbuf == NULL)
    {
        return -ERROR_MEM;
    }

    pc_re = (struct re_pc_ctrl_head_t *)sendbuf;

    pc_re->header = htons(QT_CTRL_HEAD);
    pc_re->gid = QT_CMD_GROUP_0;
    pc_re->cid = QT_CMD_GET_LICENSE_STATUS;
    pc_re->re_type = CMD_GET_LICENSE_STATUS;
    pc_re->result = 0;
    pc_re->len = htonl(0);

    if (QT_CMD_GROUP_0 != pc_ctrl->gid
        || QT_CMD_GET_LICENSE_STATUS != pc_ctrl->cid)
    {
        pc_re->result = -ERROR_CMD_UNSUPPORT;

        ret = tcp_reply_pack_send(fd, sendbuf, sendbuf_size);
        free(sendbuf);
        if (ret < 0)
            return ret;

        return CODE_HEX2DEC(pc_re->result);
    }

    int data_len = get_msg_data_len(buf);

    pc_re->len = htonl(1);
    pc_re->data[0] = get_license_status();

    ret = tcp_reply_pack_send(fd, sendbuf, sendbuf_size);
    free(sendbuf);
    if (ret < 0)
        return ret;

    return CODE_HEX2DEC(pc_re->result);
}

int PcCmd::_do_control(const int fd, uint8_t *buf, int len)
{
	int ret = 0;
    struct re_pc_ctrl_head_t *pc_ctrl = (struct re_pc_ctrl_head_t *)buf;
	
	if (buf == NULL
        || len < sizeof(struct re_pc_ctrl_head_t))
		return -ERROR_CMD_SHORT;

	if (QT_CTRL_HEAD != ntohs(pc_ctrl->header))
		return -ERROR_CMD_UNKNOWN;

    /*if (pack_crc16(data) != (data[len -2] | (data[len - 1] << 8)))
	{
		return -ERROR_CMD_XOR;
	}*/
    //SYS_DEBUG("%s: %d.%d...\n", __func__, pc_ctrl->gid, pc_ctrl->cid);

	switch(pc_ctrl->gid)
	{
        case QT_CMD_GROUP_0:
			switch(pc_ctrl->cid)
			{
                case QT_CMD_SET_SN:
                    ret = _set_sn(fd, buf, len);
                    break;
                case QT_CMD_SET_MAC:
                    ret = _set_mac(fd, buf, len);
                    break;
                case QT_CMD_GET_SN:
                    ret = _get_sn(fd, buf, len);
                    break;
                case QT_CMD_GET_MAC:
                    ret = _get_mac(fd, buf, len);
                    break;
                case QT_CMD_GET_ID:
                    ret = _get_id(fd, buf, len);
                    break;
                case QT_CMD_SET_LICENSE:
                    ret = _set_license(fd, buf, len);
                    break;
                case QT_CMD_GET_LICENSE:
                    ret = _get_license(fd, buf, len);
                    break;
                case QT_CMD_GET_LICENSE_STATUS:
                    ret = _get_license_status(fd, buf, len);
                    break;
                default:
					ret = -ERROR_CMD_UNSUPPORT;
					break;
			}
            break;
        default:
			ret = -ERROR_CMD_UNSUPPORT;
			break;
	}

	return ret;
}

uint8_t *_get_pc_msg(uint8_t *buf, int *len)
{
    struct re_pc_ctrl_head_t *pc_ctrl = NULL;

	int last = 0;
	int msg_len = 0;
    uint8_t *sendbuf = NULL;
    uint16_t *p_crc = NULL;

    const int MSG_HEAD_SIZE = to_msg_len(0);

	if (buf == NULL)
		return NULL;

    sendbuf = buf;
	last = *len;

GET_MSG_RETRY:
	if (last < MSG_HEAD_SIZE)
    {
        if (sendbuf != buf)
        {
			if (last > 0)
                memmove((void *)buf, (void *)sendbuf, last);
			*len = last;
        }

		return NULL;
    }

    pc_ctrl = (struct re_pc_ctrl_head_t *)sendbuf;

	if (QT_CTRL_HEAD != ntohs(pc_ctrl->header))
    {
        sendbuf++;
		last -= 1;

		goto GET_MSG_RETRY;
    }

    msg_len = get_msg_len(sendbuf);
	
	if (last < msg_len)
    {
        if (sendbuf != buf)
		{
			if (last > 0)
                memmove((void *)buf, (void *)sendbuf, last);
			*len = last;
        }

		return NULL;
    }

    p_crc = (uint16_t *)&sendbuf[msg_len - 2];
    if (pack_crc16(sendbuf) != ntohs(*p_crc))
    {
        sendbuf += 2;
		last -= 2;

		goto GET_MSG_RETRY;
    }

    return sendbuf;
}
 
int _do_pc_cmd(const int fd, uint8_t *buf, int *len)
{
    uint8_t *sendbuf = NULL;
	int msg_len = 0;

	int ret = 0;
	
	while (*len > 0)
	{
        sendbuf = _get_pc_msg(buf, len);
        if (sendbuf == NULL)
			break;
		
        msg_len = get_msg_len(sendbuf);

#if 1
        for(int i = 0; i < msg_len; i++)
        {
            printf("%.2X ", buf[i]);
            if ((i & 0xF) == 0xF)
                printf("\n");
        }
        printf("\n");
#endif
		
        ret = pccmd._do_control(fd, buf, msg_len);

		*len -= msg_len;
		if (*len > 0)
        {
            sendbuf += msg_len;
            memmove((void *)buf, (void *)sendbuf, *len);
        }
	}

	return ret;
}

int do_tcp_process(struct tcp_client_t *client)
{
    struct timespec _end, _tmp;

    int len = 0;
    int ret = 0;

    /* 存在超过200ms以前的数据仍未使用，则会被直接清除 */
    ms2timespec(200, &_tmp);
    timespec_add(&client->last_time, &_tmp, &_end);

    clock_gettime(CLOCK_BOOTTIME, &_tmp);

    if (timespec_cmp(&_tmp, &_end, >)
            && (client->buffer_len > 0))
    {
        printf("%s:%d.%.9d: TIMEOUT !!!! (last:%d.%.9d) client->buffer_len:%d\n", __func__,
               _tmp.tv_sec, _tmp.tv_nsec,
               client->last_time.tv_sec, client->last_time.tv_nsec,
               client->buffer_len);
        for(int i = 0; i < client->buffer_len; i++)
        {
            printf("%.2X ", client->buffer[i]);

            if ((i & 0x0F) == 15)
                printf("\n");
        }
        printf("\n");

        client->buffer_len = 0;
    }

    len = MAX_TCP_BUF_SIZE - client->buffer_len;
    ret = read(client->fd, &client->buffer[client->buffer_len], len);

    // recv 返回0代表连接已经断开
    if (ret == 0)
    {
        return -ERROR_DISCONNECT;
    }
    else if (ret < 0)
    {
        //资源暂不可用， 在尝试一次
        if(errno == EAGAIN || errno == EWOULDBLOCK)
            return 0;
        else
            return -ERROR_DISCONNECT;
    }

    clock_gettime(CLOCK_BOOTTIME, &client->last_time);

    client->buffer_len += ret;

//#ifdef DATA_RAW
#if 0
    SYS_DEBUG("tcp(fd:%d) recv: %d\n", client->fd, ret);
    int i, psize;
    psize = client->buffer_len; // > 16 ? 16 : rr;
    for(i = 0; i < psize; i++)
    {
        printf("%.2X ", client->buffer[i]);

        if ((i & 0x0F) == 15)
            printf("\n");
    }
    printf("\n");
#endif

#ifdef DATA_RAW
    SYS_DEBUG("%s: send: %d ret=%d\n", __func__, resize, ret);
#endif

    ret = _do_pc_cmd(client->fd, client->buffer, &client->buffer_len);

    return ret;
}
