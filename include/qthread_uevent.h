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

#ifndef __UEVENT_H__
#define __UEVENT_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <QThread>

#include "osee_syslog.h"

struct uevent {
	const char *action;
	const char *path;
	const char *subsystem;
	const char *usb_state;
	const char *device_name;
};


class QThreadUevent : public QThread
{
    Q_OBJECT

public:
    QThreadUevent(QObject *parent = 0);
    ~QThreadUevent();

    void run();

private:
    void parse_event(const char *msg, int len, struct uevent *uevent);

    void handle_block_event(struct uevent *uevent);
    void handle_mmc_event(struct uevent *uevent);
    void handle_net_event(struct uevent *uevent);

    void send_sd_change(bool is_online)
    {
        SYS_DEBUG("%s: online:%d\n", __func__, is_online);

        emit emit_sd_change(is_online);
    }

    void send_net_change(bool is_online, char *netif, char *dev)
    {
        SYS_DEBUG("%s: online:%d netif:%s dev:%s\n", __func__,
               is_online, netif, dev);

        emit emit_net_change(is_online, netif, dev);
    }

signals:
    void emit_sd_change(bool is_online);

    /**
     * @brief emit_net_change
     * @param is_online
     * @param netif
     * @param dev: "usb_in", "usb_out"
     */
    void emit_net_change(bool is_online, QString netif, QString dev);
};


extern QThreadUevent qthread_uevent;

#endif

