#ifndef _PC_CMD_H
#define _PC_CMD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <arpa/inet.h>

#include "crc16.h"


#define QT_CTRL_HEAD						0xEBA6

//General
#define QT_CMD_GROUP_0						0

#define QT_CMD_SET_SN				1
#define QT_CMD_SET_MAC				2
#define QT_CMD_GET_SN				3
#define QT_CMD_GET_MAC				4
#define QT_CMD_GET_ID				5
#define QT_CMD_SET_LICENSE			0x10
#define QT_CMD_GET_LICENSE			0x11
#define QT_CMD_GET_LICENSE_STATUS	0x12


#pragma   pack(1)
struct pc_str8_t {
    uint8_t len;
    uint8_t data[0];
};
#pragma   pack()

#pragma   pack(1)
struct pc_str16_t {
    uint16_t len;
    uint8_t data[0];
};
#pragma   pack()

#pragma   pack(1)
struct pc_str32_t {
    uint32_t len;
    uint8_t data[0];
};
#pragma   pack()

/************************************/
#define CMD_SET_SN                   1
#define CMD_SET_MAC                  2
#define CMD_GET_SN                   3
#define CMD_GET_MAC                  4
#define CMD_GET_ID                   5
#define CMD_SET_LICENSE              6
#define CMD_GET_LICENSE              7
#define CMD_GET_LICENSE_STATUS       8

/************************************/
#pragma   pack(1)
struct pc_cmd_head_t {
    uint16_t header;
    uint8_t data[0];
};
#pragma   pack()


#pragma   pack(1)
struct pc_ctrl_head_t {
    uint16_t header;
    uint8_t gid;
    uint8_t cid;
    uint32_t len;
    uint8_t data[0];
};
#pragma   pack()

#pragma   pack(1)
struct re_pc_ctrl_head_t {
    uint16_t header;
    uint8_t gid;
    uint8_t cid;
    uint8_t re_type;
    uint8_t result;
    uint32_t len;
    uint8_t data[0];
};
#pragma   pack()

#define to_msg_len(n)	((n) + sizeof(struct pc_ctrl_head_t) + 2)
#define to_remsg_len(n) ((n) + sizeof(struct re_pc_ctrl_head_t) + 2)

#define pack_crc16(data)        (crc16_modbus((data), ((ntohl(((struct pc_ctrl_head_t *)(data))->len) - 2) + sizeof(struct pc_ctrl_head_t))))
#define pack_reply_crc16(data)  (crc16_modbus((data), ((ntohl(((struct re_pc_ctrl_head_t *)(data))->len) - 2) + sizeof(struct re_pc_ctrl_head_t))))

static inline void pack_act(uint8_t *data)
{
        uint16_t crc = 0;
        uint32_t len = 0;
        len = ntohl(((struct pc_ctrl_head_t *)(data))->len);
        len += 2;
        (((struct pc_ctrl_head_t *)(data))->len) = htonl(len);
        crc = pack_crc16(data);
        (((struct pc_ctrl_head_t *)(data))->data[len - 1]) = (crc & 0xFF);
        (((struct pc_ctrl_head_t *)(data))->data[len - 2]) = ((crc >> 8) & 0xFF);
}

static inline void pack_reply_act(uint8_t *data)
{
        uint16_t crc = 0;
        uint32_t len = 0;
        len = ntohl(((struct re_pc_ctrl_head_t *)(data))->len);
        len += 2;
        (((struct re_pc_ctrl_head_t *)(data))->len) = htonl(len);
        crc = pack_reply_crc16(data);
        (((struct re_pc_ctrl_head_t *)(data))->data[len - 1]) = (crc & 0xFF);
        (((struct re_pc_ctrl_head_t *)(data))->data[len - 2]) = ((crc >> 8) & 0xFF);
}

#define get_msg_len(data)			(sizeof(struct pc_ctrl_head_t) + ntohl(((struct pc_ctrl_head_t *)(data))->len))
#define get_msg_data_len(data)		(ntohl(((struct pc_ctrl_head_t *)(data))->len) - 2)

#define get_re_msg_len(data)        (sizeof(struct re_pc_ctrl_head_t) + ntohl(((struct re_pc_ctrl_head_t *)(data))->len))
#define get_re_msg_data_len(data)	(ntohl(((struct re_pc_ctrl_head_t *)(data))->len) - 2)

#pragma   pack(1)
struct msg_wait_t {
    uint8_t gid;
    uint8_t cid;
    uint8_t reply;//0,未应答;1,已应答
    uint8_t re_type;
    uint8_t result;
    int msg_size;
    uint8_t *msg;
};
#pragma   pack()

#define msg_wait_clean(msg_wait)	do { \
	(msg_wait).gid = 0; \
	(msg_wait).cid = 0; \
	(msg_wait).reply = 0; \
	(msg_wait).re_type = 0; \
	(msg_wait).result = 0; \
	(msg_wait).msg_size = 0; \
	if ((msg_wait).msg) {free((msg_wait).msg); (msg_wait).msg = NULL;} \
} while(0)

#define msg_wait_stop(msg_wait)	do { \
	(msg_wait).gid = 0; \
	(msg_wait).cid = 0; \
} while(0)

#define IS_MSG_WAIT_RETURN(msg_wait)	((msg_wait).reply)

/*************************************/
extern struct msg_wait_t g_msg_wait;

/*************************************/
/**
 * @brief get_live_version
 * @param time_out  =0:忽略(单位：毫秒)
 * @return
 * 关联变量： g_live_version
 */
int get_live_version(const int fd, int time_out);



#ifdef __cplusplus
}
#endif

#endif //_PC_CMD_H
