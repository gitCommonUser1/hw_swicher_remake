#ifndef ROUTE_H
#define ROUTE_H

#include <stdint.h>
#include <QObject>
#include <QThread>

#include "osee_syslog.h"

class QThreadRoute : public QThread
{
    Q_OBJECT
public:
    QThreadRoute(QObject *parent = 0);
    ~QThreadRoute();

    void run();

private:
    int receive_netlink_message(const int fd, struct nlmsghdr **nl);

    void parse_rtattr(struct rtattr **tb, int max, struct rtattr *attr, int len);
    int os_index_to_name(int ifindex, char *ifname, int ifname_size);

    void nl_netifinfo_handle(struct nlmsghdr *nlh);
    void nl_netifaddr_handle(struct nlmsghdr *nlh);
    void nl_netroute_handle(struct nlmsghdr *nlh);

    void send_netlink_change(uint32_t flags, char *netif)
    {
        if (netif == NULL
                || strlen(netif) <= 0)
            return;

        emit emit_netlink_change(flags, netif);
    }

    void send_netaddr_change(bool is_new, char *netif, char *ipaddr, int prefixlen)
    {
        if (netif == NULL
                || strlen(netif) <= 0)
            return;

        //SYS_DEBUG("%s: add:%d %s %s %d\n", __func__, is_new, netif, ipaddr, prefixlen);

        emit emit_netaddr_change(is_new, netif, ipaddr, prefixlen);
    }

    void send_netroute_change(bool is_add, char *netif, char *gateway)
    {
        if (netif == NULL
                || strlen(netif) <= 0)
            return;

        //SYS_DEBUG("%s: add:%d netif:%s gw:%s\n", __func__, is_add, netif, gateway);

        emit emit_netroute_change(is_add, netif, gateway);
    }

signals:
    /**
     * @brief emit_netlink_change
     * @param is_link
     * @param netif
     */
    void emit_netlink_change(uint32_t flags, QString netif);

    /**
     * @brief emit_netaddr_change
     * @param is_new
     * @param netif
     * @param ipaddr
     * @param prefixlen: 地址的地址掩码长度
     */
    void emit_netaddr_change(bool is_new, QString netif, QString ipaddr, int prefixlen);

    /**
     * @brief emit_netroute_change
     * @param is_add
     * @param netif
     * @param gateway
     */
    void emit_netroute_change(bool is_add, QString netif, QString gateway);
};

extern QThreadRoute qthread_route;

#endif // ROUTE_H
