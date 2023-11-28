/*
 * Copyright (C) 2019 Rockchip Electronics Co., Ltd.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL), available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/prctl.h>

#include <linux/netlink.h>
#include <sys/socket.h>
#include <sys/time.h>

#include "osee_errors.h"
#include "osee_syslog.h"

#include "files.h"

//#include "zlog.h"
//extern zlog_category_t *out_init;

#include "qthread_uevent.h"

#define UEVENT_SUBSYSTEM_ANDROID		"android_usb"
#define UEVENT_SUBSYSTEM_BLOCK          "block"
#define UEVENT_SUBSYSTEM_MMC            "mmc"
#define UEVENT_SUBSYSTEM_NET            "net"


extern bool g_running;

QThreadUevent qthread_uevent;

void QThreadUevent::handle_block_event(struct uevent *uevent)
{
    if (strcmp(uevent->subsystem, UEVENT_SUBSYSTEM_BLOCK))
        return;

    if ((strcmp(uevent->device_name, "mmcblk2") == 0)
        && (strcmp(uevent->action, "remove") == 0))
    {
        send_sd_change(false);
    }
}

void QThreadUevent::handle_mmc_event(struct uevent *uevent)
{
    if (strcmp(uevent->subsystem, UEVENT_SUBSYSTEM_MMC))
        return;

    if (strcmp(uevent->action, "bind") == 0)
    {
        send_sd_change(true);
    }
}

void QThreadUevent::handle_net_event(struct uevent *uevent)
{
    if (strcmp(uevent->subsystem, UEVENT_SUBSYSTEM_NET))
        return;

    if (strcmp(uevent->action, "add") == 0)
    {
        if (strncmp(uevent->path, "/devices/platform/ffe00000.usb", 30) == 0)
        {
            char *netif = get_filename(uevent->path);

            send_net_change(true, netif, "usb_in");
        }
        else if (strncmp(uevent->path, "/devices/platform/usb0/ffd00000.dwc3", 36) == 0)
        {
            char *netif = get_filename(uevent->path);

            send_net_change(true, netif, "usb_out");
        }
        else
        {
            char *netif = get_filename(uevent->path);

            send_net_change(true, netif, NULL);
        }
    }
    else if (strcmp(uevent->action, "remove") == 0)
    {
        if (strncmp(uevent->path, "/devices/platform/ffe00000.usb", 30) == 0)
        {
            char *netif = get_filename(uevent->path);

            send_net_change(false, netif, "usb_in");
        }
        else if (strncmp(uevent->path, "/devices/platform/usb0/ffd00000.dwc3", 36) == 0)
        {
            char *netif = get_filename(uevent->path);

            send_net_change(false, netif, "usb_out");
        }
        else
        {
            char *netif = get_filename(uevent->path);

            send_net_change(false, netif, NULL);
        }
    }
}

void QThreadUevent::parse_event(const char *msg, int len, struct uevent *uevent)
{
    if (msg == NULL
		|| len <= 0)
		return;
	
	uevent->action = "";
	uevent->path = "";
	uevent->subsystem = "";
	uevent->usb_state = "";
	uevent->device_name = "";

	while(*msg) 
	{
		if(!strncmp(msg, "ACTION=", 7)) {
			msg += 7;
			uevent->action = msg;
		} else if(!strncmp(msg, "DEVPATH=", 8)) {
			msg += 8;
			uevent->path = msg;
		} else if(!strncmp(msg, "SUBSYSTEM=", 10)) {
			msg += 10;
			uevent->subsystem = msg;
		} else if(!strncmp(msg, "USB_STATE=", 10)) {
			msg += 10;
			uevent->usb_state = msg;
		} else if(!strncmp(msg, "DEVNAME=", 8)) {
			msg += 8;
			uevent->device_name = msg;
		}
		/* advance to after the next \0 */
		while(*msg++)
			;
	}

	if (uevent
		&& uevent->subsystem
		&& strlen(uevent->subsystem) > 0)
	{
        SYS_DEBUG("event { '%s', '%s', '%s', '%s', '%s' }\n",
                uevent->action, uevent->path, uevent->subsystem, uevent->usb_state, uevent->device_name);

        if (strcmp(uevent->subsystem, UEVENT_SUBSYSTEM_BLOCK) == 0)
        {
            handle_block_event(uevent);
        }
        else if (strcmp(uevent->subsystem, UEVENT_SUBSYSTEM_MMC) == 0)
        {
            handle_mmc_event(uevent);
        }
        else if (strcmp(uevent->subsystem, UEVENT_SUBSYSTEM_NET) == 0)
        {
            handle_net_event(uevent);
        }
	}
}

void QThreadUevent::run()
{
	int sockfd;
    int len;
	char buf[1024 + 2];
	struct iovec iov;
	struct msghdr msg;
	struct sockaddr_nl sa;
    struct uevent uevent;

    prctl(PR_SET_NAME, "QThreadUevent", 0, 0, 0);

    sockfd = socket(AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT);
    if (sockfd == -1) {
        SYS_ERR("socket creating failed:%s\n", strerror(errno));
        goto err_event_monitor;
    }

	memset(&sa, 0, sizeof(sa));
	sa.nl_family = AF_NETLINK;
	sa.nl_groups = NETLINK_KOBJECT_UEVENT;
	sa.nl_pid = 0;

    if (bind(sockfd, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
        SYS_ERR("bind error:%s\n", strerror(errno));
        goto err_event_monitor;
    }

    SYS_INFO("NETLINK_KOBJECT_UEVENT create fd(%d)\n", sockfd);

	memset(&msg, 0, sizeof(msg));
	iov.iov_base = (void *)buf;
	iov.iov_len = sizeof(buf);
	msg.msg_name = (void *)&sa;
	msg.msg_namelen = sizeof(sa);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;

    //zlog_info(out_init,"QThreadUevent running!");
	while (g_running) {
		len = recvmsg(sockfd, &msg, 0);
		if (len < 0) {
            SYS_ERR("receive error\n");
        } else if (len < 32 || len >= sizeof(buf)) {
            SYS_INFO("invalid message");
		} else {
			buf[len] = '\0';
			buf[len + 1] = '\0';
            parse_event(buf, len + 2, &uevent);
		}
	}

err_event_monitor:
    SYS_NOTICE("###################QThreadUevent %s EXIT ###############\n", __func__);
    //zlog_info(out_init,"QThreadUevent exit!");
	pthread_detach(pthread_self());
	pthread_exit(NULL);
}

QThreadUevent::QThreadUevent(QObject *parent) : QThread(parent)
{
    qRegisterMetaType<uint8_t>("uint8_t");
}

QThreadUevent::~QThreadUevent()
{
}
