#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/route.h>

#include "osee_errors.h"
#include "osee_syslog.h"

#include "general.h"

#include "net_opr.h"

#include "qthread_route.h"

#include "zlog.h"
//extern zlog_category_t *out_init;


QThreadRoute qthread_route;

extern bool g_running;


QThreadRoute::QThreadRoute(QObject *parent) : QThread(parent)
{
    qRegisterMetaType<uint8_t>("uint8_t");
    qRegisterMetaType<uint16_t>("uint16_t");
    qRegisterMetaType<uint32_t>("uint32_t");
}

QThreadRoute::~QThreadRoute()
{

}

int QThreadRoute::receive_netlink_message(const int fd, struct nlmsghdr **nl)
{
#define MAX_MSG_SIZE 1024
    struct iovec iov; // 使用 iovec 进行接收
    struct msghdr msg = {NULL, 0, &iov, 1, NULL, 0, 0}; // 初始化 msghdr
    int length;

    *nl = NULL;
    if ((*nl = (struct nlmsghdr *) malloc(MAX_MSG_SIZE)) == NULL )
        return 0;

    iov.iov_base = *nl;    // 封装 nlmsghdr
    iov.iov_len = MAX_MSG_SIZE; // 指定长度

    length = recvmsg(fd, &msg, 0);

    if(length <= 0)
        free(*nl);

    return length;
}

void QThreadRoute::parse_rtattr(struct rtattr **tb, int max, struct rtattr *attr, int len)
{
    for (; RTA_OK(attr, len); attr = RTA_NEXT(attr, len)) {
        if (attr->rta_type <= max) {
            tb[attr->rta_type] = attr;
        }
    }
}

int QThreadRoute::os_index_to_name(int ifindex, char *ifname, int ifname_size)
{
    int sockfd;
    struct ifconf ifc;
    char buf[4000] = {0x0};
    int i = 0;

    if (ifname == NULL
            || ifname_size <= 0)
    {
        return -ERROR_NULL;
    }

    memset(ifname, 0, ifname_size);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        SYS_ERR("in %s in %d socket error",__FUNCTION__,__LINE__);
        return -ERROR_SOCKET;
    }

    /* Init struct ifconf. */
    ifc.ifc_len = 4000;
    ifc.ifc_buf = (caddr_t)buf;

    /* Get all interface info. */
    if (ioctl(sockfd, SIOCGIFCONF, &ifc) < 0)
    {
        SYS_ERR("in %s in %d ioctl execute failed\n",__FUNCTION__,__LINE__);
        close(sockfd);
        return -ERROR_IOCTL;
    }

    /* Match ifindex for get ifname. */
    struct ifreq *ifr;
    struct ifreq ifrcopy;
    ifr = (struct ifreq*)buf;
    for(i = (ifc.ifc_len/sizeof(struct ifreq)); i > 0; i--)
    {
        //SYS_DEBUG("%s: i=%d [%s]\n", __func__, i, ifr->ifr_ifrn.ifrn_name);

        ifrcopy = *ifr;
        if(ioctl(sockfd, SIOCGIFINDEX, &ifrcopy) < 0)
        {
            SYS_ERR("in %s in %d ioctl execute failed\n",__FUNCTION__,__LINE__);
            close(sockfd);
            return -ERROR_IOCTL;
        }
        //SYS_DEBUG("%s: (%d)\n", __func__, ifrcopy.ifr_ifru.ifru_ivalue);

        if(ifrcopy.ifr_ifru.ifru_ivalue == ifindex)
        {
            snprintf(ifname, ifname_size, "%s", ifr->ifr_ifrn.ifrn_name);

            //SYS_DEBUG("%d: ifindex[%d] to ifname[%s]\n", i, ifindex, ifname);
            close(sockfd);

            return 1;
        }
        ifr++;
    }

    close(sockfd);

    return 0;
}

void QThreadRoute::nl_netroute_handle(struct nlmsghdr *nlh)
{
    int len;
    struct rtattr *tb[RTA_MAX + 1];
    struct rtmsg *rt;

    bool is_add = false;
    char gateway[16] = {0};
    char netif[16] = {0};

    switch (nlh->nlmsg_type)
    {
    case RTM_NEWROUTE:
        is_add = true;
        break;
    case RTM_DELROUTE:
        is_add = false;
        break;
    default:
        return;
    }

    rt = (struct rtmsg*)NLMSG_DATA(nlh);
    len = nlh->nlmsg_len - NLMSG_SPACE(sizeof(*rt));
    if (len <= 0)
        return;

    if (rt->rtm_table != RT_TABLE_MAIN)
        return;

    if (rt->rtm_family != AF_INET)
        return;

    bzero(tb, sizeof(tb));
    parse_rtattr(tb, RTA_MAX, RTM_RTA(rt), len);

    if (tb[RTA_GATEWAY] == NULL)
        return;

    inet_ntop(rt->rtm_family, RTA_DATA(tb[RTA_GATEWAY]), gateway, sizeof(gateway));

    if (tb[RTA_OIF] != NULL)
    {
        int oif = *(int *)RTA_DATA(tb[RTA_OIF]);
        int ret = os_index_to_name(oif, netif, sizeof(netif));

        //SYS_DEBUG("RTA_OIF: %d(%s(%d))\n", *(int *)RTA_DATA(tb[RTA_OIF]), netif, ret);
    }

    send_netroute_change(is_add, netif, gateway);
}

void QThreadRoute::nl_netifinfo_handle(struct nlmsghdr *nlh)
{
    int len;
    struct rtattr *tb[IFLA_MAX + 1];
    struct ifinfomsg *ifinfo;

    char netif[16] = {0};

    ifinfo = (struct ifinfomsg *)NLMSG_DATA(nlh);
    len = nlh->nlmsg_len - NLMSG_SPACE(sizeof(*ifinfo));
    if (len <= 0)
        return;


    //SYS_DEBUG("%s: index:%d %.8X %.8X\n", __func__, ifinfo->ifi_index, ifinfo->ifi_flags, ifinfo->ifi_change);

    /* Return if loopback */
    //if (ifinfo->ifi_type == ARPHRD_LOOPBACK)
    if (ifinfo->ifi_flags & IFF_LOOPBACK)
        return;

    bzero(tb, sizeof(tb));
    parse_rtattr(tb, IFLA_MAX, IFLA_RTA (ifinfo), len);

    if(tb[IFLA_IFNAME] == NULL)
        return;

    snprintf(netif, sizeof(netif), "%s", RTA_DATA(tb[IFLA_IFNAME]));

    send_netlink_change(ifinfo->ifi_flags, netif);
}

void QThreadRoute::nl_netifaddr_handle(struct nlmsghdr *nlh)
{
    int len;
    struct rtattr *tb[IFA_MAX + 1];
    struct ifaddrmsg *ifaddr;

    bool is_new = false;
    char netif[16] = {0};
    char ipaddr[16] = {0};

    switch (nlh->nlmsg_type)
    {
    case RTM_NEWADDR:
        is_new = true;
        break;
    case RTM_DELADDR:
        is_new = false;
        break;
    default:
        return;
        break;
    }

    ifaddr = (struct ifaddrmsg *)NLMSG_DATA(nlh);
    len = nlh->nlmsg_len - NLMSG_SPACE(sizeof(*ifaddr));
    if (len <= 0)
        return;

    if (ifaddr->ifa_family != AF_INET)
        return;

    bzero(tb, sizeof(tb));
    parse_rtattr(tb, IFA_MAX, IFA_RTA (ifaddr), len);

    if (tb[IFA_LABEL] == NULL)
        return;

    snprintf(netif, sizeof(netif), "%s", RTA_DATA(tb[IFA_LABEL]));

    if (tb[IFA_ADDRESS] != NULL) {
        inet_ntop(ifaddr->ifa_family, RTA_DATA(tb[IFA_ADDRESS]), ipaddr, sizeof(ipaddr));
    }

    send_netaddr_change(is_new, netif, ipaddr, ifaddr->ifa_prefixlen);
}

void QThreadRoute::run()
{
    int sockfd;
    int len = 0;
    char buf[4096 + 2];
    struct iovec iov;
    struct msghdr msg;
    struct sockaddr_nl sa;
    struct nlmsghdr *nl;

    prctl(PR_SET_NAME, "QThreadRoute", 0, 0, 0);

    sockfd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (sockfd == -1) {
        SYS_ERR("socket creating failed:%s\n", strerror(errno));
        goto QThreadRoute_error;
    }

    memset(&sa, 0, sizeof(sa));
    sa.nl_family = AF_NETLINK;
    sa.nl_groups = RTMGRP_LINK | RTMGRP_IPV4_IFADDR | RTMGRP_IPV4_ROUTE;
    sa.nl_pid = 0;

    if (bind(sockfd, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
        SYS_ERR("bind error:%s\n", strerror(errno));
        goto QThreadRoute_error;
    }

    SYS_INFO("NETLINK_ROUTE create fd(%d)\n", sockfd);

    memset(&msg, 0, sizeof(msg));
    iov.iov_base = (void *)buf;
    iov.iov_len = sizeof(buf);
    msg.msg_name = (void *)&sa;
    msg.msg_namelen = sizeof(sa);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

//    zlog_info(out_init,"QThreadRoute running!");

    while (g_running) {
        len = recvmsg(sockfd, &msg, 0);
        if (len <= 0)
            continue;

        nl = (struct nlmsghdr *)buf;
        for( ; NLMSG_OK(nl, len); nl = NLMSG_NEXT(nl, len))
        {
            //SYS_DEBUG("%s: nlmsg_type:%d\n", __func__, nl->nlmsg_type);

            switch (nl->nlmsg_type)
            {
            case NLMSG_DONE:
            case NLMSG_ERROR:
                break;
            case RTM_NEWLINK:
            case RTM_DELLINK:
                nl_netifinfo_handle(nl);
                break;
            case RTM_NEWADDR:
            case RTM_DELADDR:
                nl_netifaddr_handle(nl);
                break;
            case RTM_NEWROUTE:
            case RTM_DELROUTE:
                nl_netroute_handle(nl);
                break;
            }
        }
    }

QThreadRoute_error:
    SYS_NOTICE("###################QThreadRoute %s EXIT ###############\n", __func__);
//    zlog_info(out_init,"QThreadRout exit!");
    if (sockfd > 0)
    {
        SYS_INFO("route disconnect from (%d)\n",
                sockfd);

        close(sockfd);
        sockfd = -1;
    }

    pthread_detach(pthread_self());
    pthread_exit(NULL);

}
