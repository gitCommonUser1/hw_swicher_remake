#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>          /* See NOTES */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <net/route.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "osee_errors.h"
#include "osee_syslog.h"

#include "net_opr.h"


#define DNS_FILE    "/etc/resolv.conf"
#define ROUTE_FILE  "/proc/net/route"

#define DHCP_CONF   "/etc/dhcpcd.conf"
#define DNS_PATH    "/run/dhcpcd/resolv.conf"

#define SYS_NET     "/sys/class/net"


/**
 * @brief is_valid_ip: ip地址是否有效
 * @param ipaddr
 * @return
 */
int is_valid_ip(const char *ipaddr)
{
    int ret = -1;
    struct in_addr in;

    if (ipaddr == NULL
            || strlen(ipaddr) <= 0)
        return 0;

    ret = inet_aton(ipaddr, &in);
    if (ret == 0)
        return 0;

    return 1;
}

/**
 * @brief is_valid_netmask: 子网掩码是否有效
 * @param netmask
 * @return
 */
int is_valid_netmask(const char *netmask)
{
    uint32_t mask = 0;
    int i;
    int n[4];

    if (netmask == NULL
            || strlen(netmask) <= 0)
        return 0;

    if (! is_valid_ip(netmask))
        return 0;

    sscanf(netmask, "%u.%u.%u.%u", &n[3], &n[2], &n[1], &n[0]);

    mask = 0;
    for(i = 0; i < 4; i++)
        mask |= n[i] << (i * 8);

    mask = ~mask + 1;
    if ((mask & (mask - 1)) == 0)
        return 1;

    return 0;
}

/**
 * @brief is_same_network: ip，网关是否处于同一网段
 * @param ipaddr
 * @param netmask
 * @param gateway
 * @return
 */
int is_same_network(const char *ipaddr, const char *netmask, const char *gateway)
{
    int ret = 0;

    uint32_t mask = 0;
    int i;
    int n[4];

    struct in_addr in_ipaddr;
    struct in_addr in_netmask;
    struct in_addr in_gateway;

    if (ipaddr == NULL
            || strlen(ipaddr) <= 0
            || netmask == NULL
            || strlen(netmask) <= 0
            || gateway == NULL
            || strlen(gateway) <= 0)
        return 0;

    ret = inet_aton(ipaddr, &in_ipaddr);
    if (ret == 0)
        return 0;

    ret = inet_aton(gateway, &in_gateway);
    if (ret == 0)
        return 0;

    ret = inet_aton(netmask, &in_netmask);
    if (ret == 0)
        return 0;

    //检查netmask是否有效
    sscanf(netmask, "%u.%u.%u.%u", &n[3], &n[2], &n[1], &n[0]);

    mask = 0;
    for(i = 0; i < 4; i++)
        mask |= n[i] << (i * 8);

    mask = ~mask + 1;
    if ((mask & (mask - 1)) != 0)
        return 0;

    //printf("%s: %.8X %.8X %.8X\n", __func__, in_ipaddr.s_addr, in_netmask.s_addr, in_gateway.s_addr);

    if ((in_ipaddr.s_addr & in_netmask.s_addr) == (in_gateway.s_addr & in_netmask.s_addr))
        return 1;

    return 0;
}

/**
 * @brief prefixlen2mask: 子网掩码长度转换成字符串形式的子网掩码
 * @param prefixlen
 * @param netmask
 * @param netmask_len
 * @return
 */
int prefixlen2mask(uint8_t prefixlen, char *netmask, int netmask_len)
{
    if (netmask == NULL
            || netmask_len <= 0)
        return -ERROR_NULL;

    memset(netmask, 0, netmask_len);

    if (prefixlen > 32)
        return -ERROR_DATA;

    if (prefixlen == 32)
    {
        snprintf(netmask, netmask_len, "255.255.255.255");
    }
    if (prefixlen == 24)
    {
        snprintf(netmask, netmask_len, "255.255.255.0");
    }
    if (prefixlen == 16)
    {
        snprintf(netmask, netmask_len, "255.255.0.0");
    }
    if (prefixlen == 8)
    {
        snprintf(netmask, netmask_len, "255.0.0.0");
    }
    if (prefixlen == 0)
    {
        snprintf(netmask, netmask_len, "0.0.0.0");
    }
    else
    {
        uint32_t _mask = 0;
        for(int i = 0; i < prefixlen; i++)
        {
            _mask >>= 1;
            _mask |= 0x80000000;
        }

        snprintf(netmask, netmask_len, "%d.%d.%d.%d",
                 (_mask >> 24) & 0xFF,
                 (_mask >> 16) & 0xFF,
                 (_mask >> 8) & 0xFF,
                 _mask & 0xFF);
    }

    return 0;
}

/**
 * @brief get_netstatus: 获取当前网卡状态
 * @param netif
 * @return ifr_flags
 */
int get_netstatus(const char *netif, uint32_t *flags)
{
    struct ifreq ifr_get_ip;

    int fd = 0;
    int ret = -1;

    if (netif == NULL
            || strlen(netif) <= 0)
        return -ERROR_PARAM_NULL;

    if (flags == NULL)
        return -ERROR_NULL;

    *flags = 0;

    if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        SYS_ERR("%s: socket %s error(%d - %s)\n", __func__, netif, errno, strerror(errno));

        return -ERROR_SOCKET;
    }

    memset((uint8_t *)&ifr_get_ip, 0, sizeof(struct ifreq));
    snprintf(ifr_get_ip.ifr_name, sizeof(ifr_get_ip.ifr_name) - 1, "%s", netif);

    ret = ioctl(fd, SIOCGIFFLAGS, &ifr_get_ip);
    close(fd);
    if(ret < 0)
    {
        SYS_ERR("%s: ioctl %s error(%d - %s)\n", __func__, netif, errno, strerror(errno));

        return -ERROR_IOCTL;
    }

    *flags = ifr_get_ip.ifr_flags;

    return 0;
}

/**
 * @brief get_netspeedmode: 获取当前网卡速度模式（10/100/1000)
 * @param netif
 * @return
 */
int get_netspeedmode(const char *netif)
{
    char path[128];

    int fd = -1;
    char buf[64];
    int r_size = -1;

    int speed = 0;

    if (netif == NULL
            || strlen(netif) <= 0)
        return -ERROR_PARAM_NULL;

    sprintf(path, "%s/%s/speed", SYS_NET, netif);
    if (access(path, R_OK) < 0)
    {
        return -ERROR_NO_EXIST;
    }

    if (! is_net_link(netif))
    {
        return -ERROR_SOCKET;
    }

    fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        SYS_ERR("%s: open %s error(%d - %s)\n", __func__, path, errno, strerror(errno));

        return -ERROR_OPEN;
    }

    r_size = read(fd, buf, 64);
    if (r_size <= 0)
    {
        SYS_ERR("%s: read %s error(%d - %s)\n", __func__, path, errno, strerror(errno));

        return -ERROR_READ;
    }

    if (1 != sscanf(buf, "%d", &speed))
    {
        return -ERROR_DATA;
    }

    return speed;
}

/**
 * @brief _is_net_loopback: 是否为 回环接口(lo)
 * @param flags
 * @return
 */
int _is_net_loopback(uint32_t flags)
{
    return (((flags) & IFF_LOOPBACK) == IFF_LOOPBACK);
}

/**
 * @brief _is_net_up: 是否使能该网卡
 * @param flags
 * @return
 */
int _is_net_up(uint32_t flags)
{
    return (((flags) & IFF_UP) == IFF_UP);
}

/**
 * @brief _is_net_link: 网卡是否物理连通
 * @param flags
 * @return
 */
int _is_net_link(uint32_t flags)
{
    return (((flags) & IFF_RUNNING) == IFF_RUNNING);
}

/**
 * @brief is_net_up: 是否使能该网卡
 * @param netif
 * @return
 */
int is_net_up(const char *netif)
{
    struct ifreq ifr_get_ip;
    int fd = 0;
    int ret = -1;

    if (netif == NULL
            || strlen(netif) <= 0)
        return -ERROR_PARAM_NULL;

    if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return -ERROR_SOCKET;

    memset((uint8_t *)&ifr_get_ip, 0, sizeof(struct ifreq));
    snprintf(ifr_get_ip.ifr_name, sizeof(ifr_get_ip.ifr_name) - 1, "%s", netif);

    ret = ioctl(fd, SIOCGIFFLAGS, &ifr_get_ip);
    if(ret < 0)
    {
        SYS_ERR("%s: ioctl %s error(%d - %s)\n", __func__, netif, errno, strerror(errno));

        close(fd);
        return -ERROR_IOCTL;
    }
    close(fd);

    if ((ifr_get_ip.ifr_flags & IFF_UP) == IFF_UP)
        return 1;

    return 0;
}

/**
 * @brief is_net_link: 网卡是否物理连通
 * @param netif
 * @return
 */
int is_net_link(const char *netif)
{
    struct ifreq ifr_get_ip;
    int fd = 0;
    int ret = -1;

    if (netif == NULL
            || strlen(netif) <= 0)
        return -ERROR_PARAM_NULL;

    if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        SYS_ERR("%s: socket %s error(%d - %s)\n", __func__, netif, errno, strerror(errno));
        return -ERROR_SOCKET;
    }

    memset((uint8_t *)&ifr_get_ip, 0, sizeof(struct ifreq));
    snprintf(ifr_get_ip.ifr_name, sizeof(ifr_get_ip.ifr_name) - 1, "%s", netif);

    ret = ioctl(fd, SIOCGIFFLAGS, &ifr_get_ip);
    if(ret < 0)
    {
        SYS_ERR("%s: ioctl %s error(%d - %s)\n", __func__, netif, errno, strerror(errno));

        close(fd);
        return -ERROR_IOCTL;
    }
    close(fd);

    if ((ifr_get_ip.ifr_flags & IFF_RUNNING) == IFF_RUNNING)
        return 1;

    return 0;
}

/**
 * @brief get_ipaddr: 获取ip地址
 * @param netif: 网卡名称
 * @param ipaddr
 * @param ipaddr_len
 * @return
 */
int get_ipaddr(const char *netif, char *ipaddr, int ipaddr_len)
{
    struct ifreq ifr_get_ip;
    struct sockaddr_in *sin_addr;
    int fd = 0;
    int ret = -1;

    if (netif == NULL
            || strlen(netif) <= 0)
        return -ERROR_PARAM_NULL;

    if (ipaddr == NULL
            || ipaddr_len <= 0)
        return -ERROR_NULL;

    memset(ipaddr, 0, ipaddr_len);

    if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return -ERROR_SOCKET;

    memset((uint8_t *)&ifr_get_ip, 0, sizeof(struct ifreq));
    snprintf(ifr_get_ip.ifr_name, sizeof(ifr_get_ip.ifr_name) - 1, "%s", netif);

    ret = ioctl(fd, SIOCGIFADDR, &ifr_get_ip);
    if(ret < 0)
    {
        SYS_ERR("%s: ioctl %s error(%d - %s)\n", __func__, netif, errno, strerror(errno));

        close(fd);
        return -ERROR_IOCTL;
    }
    close(fd);

    sin_addr = (struct sockaddr_in *)&ifr_get_ip.ifr_addr;

    snprintf(ipaddr, ipaddr_len, "%s", inet_ntoa(sin_addr->sin_addr));

    //SYS_DEBUG("%s: %s\n", __func__, inet_ntoa(sin_addr->sin_addr));

    return 0;
}

/**
 * @brief get_netmask: 获取子网掩码
 * @param netif: 网卡名称
 * @param mask
 * @param mask_len
 * @return
 */
int get_netmask(const char *netif, char *mask, int mask_len)
{
    struct ifreq ifr_get_ip;
    struct sockaddr_in *sin_addr;
    int fd = 0;
    int ret = -1;

    if (netif == NULL
            || strlen(netif) <= 0)
        return -ERROR_PARAM_NULL;

    if (mask == NULL
            || mask_len <= 0)
        return -ERROR_NULL;

    memset(mask, 0, mask_len);

    if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return -ERROR_SOCKET;

    memset((uint8_t *)&ifr_get_ip, 0, sizeof(struct ifreq));
    snprintf(ifr_get_ip.ifr_name, sizeof(ifr_get_ip.ifr_name) - 1, "%s", netif);

    //get netmask
    ret = ioctl(fd, SIOCGIFNETMASK, &ifr_get_ip);
    if(ret < 0)
    {
        SYS_ERR("%s: ioctl %s error(%d - %s)\n", __func__, netif, errno, strerror(errno));
        close(fd);
        return -ERROR_IOCTL;
    }
    close(fd);
    sin_addr = (struct sockaddr_in *)&ifr_get_ip.ifr_addr;

    snprintf(mask, mask_len, "%s", inet_ntoa(sin_addr->sin_addr));

    return 0;
}

/**
 * @brief get_gateway: 获取网关地址
 * @param netif: 网卡名称
 * @param gateway
 * @param gateway_len
 * @return
 */
int get_gateway(const char *netif, char *gateway, int gateway_len)
{
#define HSTRS_LEN   256

    FILE *fp;
    char hstrs[HSTRS_LEN] = {0};

    char iface[16];
    uint32_t dest_addr = 0, gate_addr = 0;

    int i, n[4];

    if (netif == NULL
            || strlen(netif) <= 0)
        return -ERROR_PARAM_NULL;

    if (gateway == NULL
            || gateway_len <= 0)
        return -ERROR_NULL;

    memset(gateway, 0, gateway_len);

    fp = fopen(ROUTE_FILE, "r");
    if(fp == NULL)
        return -ERROR_OPEN;

    fgets(hstrs, HSTRS_LEN, fp);

    while ( ! feof(fp))
    {
        if (NULL == fgets(hstrs, HSTRS_LEN, fp))
            break;

        if (strlen(hstrs) == 0)
            continue;

        if(sscanf(hstrs, "%s\t%X\t%X", iface, &dest_addr, &gate_addr) != 3
                || dest_addr != 0
                || strcmp(iface, netif) != 0)
            continue;

        for (i = 0; i < 4; i++)
        {
            n[i] = gate_addr & 0xFF;
            gate_addr >>= 8;
        }

        snprintf(gateway, gateway_len, "%d.%d.%d.%d", n[0], n[1], n[2], n[3]);
        break;
    }

    fclose(fp);

    return 0;
}

/**
 * @brief get_dns: 获取DNS地址
 * @param netif: 网卡名称
 * @param dns1
 * @param dns1_len
 * @param dns2
 * @param dns2_len
 * @return
 */
int get_dns(const char *netif, char *dns1, int dns1_len, char *dns2, int dns2_len)
{
#define HSTRS_LEN   256

    FILE *fp;
    char hstrs[HSTRS_LEN] = {0};

    char iface[16];
    char ipaddr[16];

    if (netif == NULL
            || strlen(netif) <= 0
            || dns1 == NULL
            || dns1_len <= 0
            || dns2 == NULL
            || dns2_len <= 0)
        return -ERROR_NULL;

    memset(dns1, 0, dns1_len);
    memset(dns2, 0, dns2_len);

    sprintf(hstrs, "%s/%s.dhcp", DNS_PATH, netif);

    if (access(hstrs, R_OK) != 0)
    {
        return -ERROR_CANNOT_ACCESS;
    }

    fp = fopen(hstrs, "r");
    if(fp == NULL)
        return -ERROR_OPEN;

    int dns = 0;
    while ( ! feof(fp))
    {
        if (NULL == fgets(hstrs, HSTRS_LEN, fp))
            break;

        if (strlen(hstrs) == 0)
            continue;

        if (hstrs[0] == '#')
            continue;

        if(sscanf(hstrs, "%s %s", iface, ipaddr) != 2
                || strcmp(iface, "nameserver") != 0)
            continue;

        if (! is_valid_ip(ipaddr))
            continue;

        if (dns == 0)
            snprintf(dns1, dns1_len, "%s", ipaddr);
        else if (dns == 1)
            snprintf(dns2, dns2_len, "%s", ipaddr);

        dns++;
        if (dns >= 2)
            break;
    }

    fclose(fp);

    return 0;
}

/**
 * @brief set_ip_mask: 同时设置ip，子网掩码
 * @param netif: 网卡名称
 * @param ip
 * @param mask
 * @return
 */
int set_ip_mask(const char *netif, const char *ip, const char *mask)
{
    struct ifreq ifr_set_ip;
    struct sockaddr_in *sin_addr;
    int fd = 0;
    int ret = 0;

    if (netif == NULL
            || strlen(netif) <= 0
            || ip == NULL
            || strlen(ip) <= 0
            || mask == NULL
            || strlen(mask) <= 0)
        return -ERROR_PARAM_NULL;

    if ((! is_valid_ip(ip))
            || (! is_valid_netmask(mask)))
        return -ERROR_DATA;

    if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return -ERROR_SOCKET;

    memset(&ifr_set_ip, 0, sizeof(struct ifreq));
    snprintf(ifr_set_ip.ifr_name, sizeof(ifr_set_ip.ifr_name) - 1, "%s", netif);

    sin_addr = (struct sockaddr_in *)&ifr_set_ip.ifr_addr;
    sin_addr->sin_family = AF_INET;
    ret = inet_aton(ip, &sin_addr->sin_addr);
    if (ret < 0)
    {
        close(fd);
        return -ERROR_DATA;
    }

    ret = ioctl(fd, SIOCSIFADDR, &ifr_set_ip);
    if(ret < 0)
    {
        SYS_ERR("%s: ioctl %s SIOCSIFADDR error(%d - %s)\n", __func__, netif, errno, strerror(errno));
        close(fd);
        return -ERROR_IOCTL;
    }

    sin_addr->sin_family = AF_INET;
    ret = inet_aton(mask, &sin_addr->sin_addr);
    if (ret < 0)
    {
        SYS_ERR("%s: inet_aton %s error(%d - %s)\n", __func__, netif, errno, strerror(errno));
        close(fd);
        return -ERROR_DATA;
    }

    ret = ioctl(fd, SIOCSIFNETMASK, &ifr_set_ip);
    if(ret < 0)
    {
        SYS_ERR("%s: ioctl %s SIOCSIFNETMASK error(%d - %s)\n", __func__, netif, errno, strerror(errno));
        close(fd);
        return -ERROR_IOCTL;
    }

    ifr_set_ip.ifr_flags |= IFF_UP | IFF_RUNNING;

    ret = ioctl(fd, SIOCSIFFLAGS, &ifr_set_ip);
    if(ret  < 0)
    {
        SYS_ERR("%s: ioctl %s SIOCSIFFLAGS error(%d - %s)\n", __func__, netif, errno, strerror(errno));
        perror(ifr_set_ip.ifr_name);

        close(fd);
        return -ERROR_IOCTL;
    }

    close(fd);

    return 0;
}

/**
 * @brief set_net_up 使能网卡
 * @param netif: 网卡名称
 * @return
 */
int set_net_up(const char *netif)
{
    struct ifreq ifr_set_ip;
    struct sockaddr_in *sin_addr;
    int fd = 0;
    int ret = 0;

    if (netif == NULL
            || strlen(netif) <= 0)
        return -ERROR_PARAM_NULL;

    if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return -ERROR_SOCKET;

    memset(&ifr_set_ip, 0, sizeof(struct ifreq));
    snprintf(ifr_set_ip.ifr_name, sizeof(ifr_set_ip.ifr_name) - 1, "%s", netif);

    sin_addr = (struct sockaddr_in *)&ifr_set_ip.ifr_addr;
    sin_addr->sin_family = AF_INET;

    ret = ioctl(fd, SIOCGIFFLAGS, &ifr_set_ip);
    if(ret  < 0)
    {
        SYS_ERR("%s: ioctl %s error(%d - %s)\n", __func__, netif, errno, strerror(errno));
        perror(ifr_set_ip.ifr_name);

        close(fd);
        return -ERROR_IOCTL;
    }

    ifr_set_ip.ifr_flags |= IFF_UP | IFF_RUNNING;

    ret = ioctl(fd, SIOCSIFFLAGS, &ifr_set_ip);
    if(ret  < 0)
    {
        SYS_ERR("%s: ioctl %s error(%d - %s)\n", __func__, netif, errno, strerror(errno));
        perror(ifr_set_ip.ifr_name);

        close(fd);
        return -ERROR_IOCTL;
    }

    close(fd);

    return 0;
}

/**
 * @brief set_net_down 停用网卡
 * @param netif: 网卡名称
 * @return
 */
int set_net_down(const char *netif)
{
    struct ifreq ifr_set_ip;
    struct sockaddr_in *sin_addr;
    int fd = 0;
    int ret = 0;

    if (netif == NULL
            || strlen(netif) <= 0)
        return -ERROR_PARAM_NULL;

    if (strcmp(netif, "lo") == 0)
        return -ERROR_CANNOT_ACCESS;

    if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return -ERROR_SOCKET;

    memset(&ifr_set_ip, 0, sizeof(struct ifreq));
    snprintf(ifr_set_ip.ifr_name, sizeof(ifr_set_ip.ifr_name) - 1, "%s", netif);

    sin_addr = (struct sockaddr_in *)&ifr_set_ip.ifr_addr;
    sin_addr->sin_family = AF_INET;

    ret = ioctl(fd, SIOCGIFFLAGS, &ifr_set_ip);
    if(ret  < 0)
    {
        SYS_ERR("%s: ioctl %s SIOCGIFFLAGS error(%d - %s)\n", __func__, netif, errno, strerror(errno));
        perror(ifr_set_ip.ifr_name);

        close(fd);
        return -ERROR_IOCTL;
    }

    ifr_set_ip.ifr_flags &= ~(IFF_UP);

    ret = ioctl(fd, SIOCSIFFLAGS, &ifr_set_ip);
    if(ret  < 0)
    {
        SYS_ERR("%s: ioctl %s SIOCSIFFLAGS error(%d - %s)\n", __func__, netif, errno, strerror(errno));
        perror(ifr_set_ip.ifr_name);

        close(fd);
        return -ERROR_IOCTL;
    }

    close(fd);

    return 0;
}

/**
 * @brief set_ipaddr: 设置ip
 * @param netif
 * @param ip
 * @return
 */
int set_ipaddr(const char *netif, const char *ip)
{
    struct ifreq ifr_set_ip;
    struct sockaddr_in *sin_addr;
    int fd = 0;
    int ret = 0;

    if (netif == NULL
            || strlen(netif) <= 0
            || ip == NULL
            || strlen(ip) <= 0)
        return -ERROR_PARAM_NULL;

    if (! is_valid_ip(ip))
        return -ERROR_DATA;

    if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return -ERROR_SOCKET;

    memset(&ifr_set_ip, 0, sizeof(struct ifreq));
    snprintf(ifr_set_ip.ifr_name, sizeof(ifr_set_ip.ifr_name) - 1, "%s", netif);

    sin_addr = (struct sockaddr_in *)&ifr_set_ip.ifr_addr;
    sin_addr->sin_family = AF_INET;
    if (inet_aton(ip, &sin_addr->sin_addr) < 0)
    {
        close(fd);
        return -ERROR_DATA;
    }

    ret = ioctl(fd, SIOCSIFADDR, &ifr_set_ip);
    if(ret  < 0)
    {
        SYS_ERR("%s: ioctl %s error(%d - %s)\n", __func__, netif, errno, strerror(errno));
        close(fd);
        return -ERROR_IOCTL;
    }

    close(fd);

    return 0;
}

/**
 * @brief set_netmask: 设置子网掩码
 * @param netif: 网卡名称
 * @param mask
 * @return
 */
int set_netmask(const char *netif, const char *mask)
{
    struct ifreq ifr_set_ip;
    struct sockaddr_in *sin_addr;
    int fd = 0;
    int ret = 0;

    if (netif == NULL
            || strlen(netif) <= 0
            || mask == NULL
            || strlen(mask) <= 0)
        return -ERROR_PARAM_NULL;

    if (! is_valid_netmask(mask))
        return -ERROR_DATA;

    if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return -ERROR_SOCKET;

    memset(&ifr_set_ip, 0, sizeof(struct ifreq));
    snprintf(ifr_set_ip.ifr_name, sizeof(ifr_set_ip.ifr_name) - 1, "%s", netif);

    sin_addr = (struct sockaddr_in *)&ifr_set_ip.ifr_addr;
    sin_addr->sin_family = AF_INET;
    if (inet_aton(mask, &sin_addr->sin_addr) < 0)
    {
        close(fd);
        return -ERROR_DATA;
    }

    ret = ioctl(fd, SIOCSIFNETMASK, &ifr_set_ip);
    if(ret < 0)
    {
        SYS_ERR("%s: ioctl %s error(%d - %s)\n", __func__, netif, errno, strerror(errno));
        close(fd);
        return -ERROR_IOCTL;
    }

    close(fd);

    return 0;
}

/**
 * @brief set_gateway: 设置网关地址
 * @param netif: 网卡名称
 * @param gateway
 * @return
 */
int set_gateway(const char *netif, const char *gateway)
{
    struct rtentry rt;
    struct sockaddr_in *sin_addr;
    int fd = 0;
    int ret = 0;

    if (netif == NULL
            || strlen(netif) <= 0
            || gateway == NULL
            || strlen(gateway) <= 0)
        return -ERROR_PARAM_NULL;

    if (! is_valid_ip(gateway))
        return -ERROR_DATA;

    if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return -ERROR_SOCKET;

    memset(&rt, 0, sizeof(struct rtentry));

    sin_addr = (struct sockaddr_in *)&rt.rt_gateway;
    sin_addr->sin_family = AF_INET;
    if (inet_aton(gateway, &sin_addr->sin_addr) < 0)
    {
        close(fd);
        return -ERROR_DATA;
    }

    ((struct sockaddr_in *)&rt.rt_dst)->sin_family = AF_INET;
    ((struct sockaddr_in *)&rt.rt_genmask)->sin_family = AF_INET;

    rt.rt_flags = RTF_GATEWAY;

    ret = ioctl(fd, SIOCADDRT, &rt);
    if(ret < 0)
    {
        SYS_ERR("%s: ioctl %s error(%d - %s)\n", __func__, netif, errno, strerror(errno));
        close(fd);
        return -ERROR_IOCTL;
    }

    close(fd);

    return 0;
}

/**
 * @brief set_static_interface: 设置DHCP获取ip方式
 *          （ipaddr存在且有效时为static方式，否则为DHCP方式）
 * @param netif: 网卡名（不能为空）
 * @param ipaddr: ipaddr存在且有效时为static方式，否则为DHCP方式
 *                  （当没有ip，但仍想设置成static方式时，此字段设置为“0.0.0.0”）
 * @param netmask
 * @param gateway
 * @param dns1
 * @param dns2
 * @return
 */
int set_static_interface(const char *netif,
                                const char *ipaddr,
                                const char *netmask,
                                const char *gateway,
                                const char *dns1,
                                const char *dns2)
{
    int fd = -1;
    FILE *fp;
    char hstrs[HSTRS_LEN] = {0};

    char iface[16];
    int _mask = -1;

    long off = 0;
    long _start = 0, _end = 0;

    if (netif == NULL
            || strlen(netif) <= 0)
        return -ERROR_NULL;

    if (access(DHCP_CONF, R_OK) != 0)
    {
        return -ERROR_CANNOT_ACCESS;
    }

    fp = fopen(DHCP_CONF, "rw+b");
    if(fp == NULL)
        return -ERROR_OPEN;

    fd = fileno(fp);

    _start = -1;
    _end = -1;

    int found = 0;
    while ( ! feof(fp))
    {
        off = ftell(fp);

        if (NULL == fgets(hstrs, HSTRS_LEN, fp))
            break;

        if (strlen(hstrs) == 0)
            continue;

        if (found)
        {
            if (strncmp(hstrs, "static ", 7) != 0)
            {
                _end = ftell(fp);

                break;
            }
        }
        else
        {
            if (hstrs[0] == '#')
                continue;

            if (strncmp(hstrs, "interface ", 10) != 0)
                continue;

            if ((sscanf(hstrs, "interface %s", iface) == 1)
                    && (strcmp(iface, netif) == 0))
            {
                found = 1;

                _start = off;
            }
        }
    }

    //printf("%s: start:%d end:%d\n", __func__, _start, _end);

    if (found)
    {
        fseek(fp, 0, SEEK_END);
        off = ftell(fp);

        if (_end < 0)
            _end = off;

        if (off != _end)
        {
            uint8_t *buf;
            int r, w;
            long pos_r, pos_w;
            int last = off - _end;

            if (last < 4096)
                buf = (uint8_t *)malloc(last);
            else
                buf = (uint8_t *)malloc(4096);

            if (buf)
            {
                pos_w = _start;
                pos_r = _end;

                while(last > 0)
                {
                    r = last < 4096 ? last : 4096;

                    fseek(fp, pos_r, SEEK_SET);
                    w = fread(buf, r, 1, fp);
                    if (w != r)
                    {

                    }
                    pos_r += w;

                    fseek(fp, pos_w, SEEK_SET);
                    r = fwrite(buf, 1, w, fp);
                    if (r != w)
                    {

                    }
                    pos_w += r;
                    last -= r;
                }

                free(buf);

                if(fd > 0)
                {
                    ftruncate(fd, off - (_end - _start));
                }
            }
            else
            {
                if(fd > 0)
                {
                    ftruncate(fd, _start);
                }
            }
        }
        else
        {
            if(fd > 0)
            {
                ftruncate(fd, _start);
            }
        }
    }

    if ((ipaddr && is_valid_ip(ipaddr))
            || (dns1 && is_valid_ip(dns1))
            || (dns2 && is_valid_ip(dns2)))
    {
        fseek(fp, 0, SEEK_END);
        fprintf(fp, "interface %s\n", netif);

        _mask = -1;
        if (netmask
                && is_valid_netmask(netmask))
        {
            int n[4];
            uint32_t mask = 0;
            sscanf(netmask, "%u.%u.%u.%u", &n[3], &n[2], &n[1], &n[0]);

            mask = 0;
            for(int i = 0; i < 4; i++)
                mask |= n[i] << (i * 8);

            _mask = 0;
            for(int i = 0; i < 32; i++)
            {
                if (mask & 1)
                    _mask++;

                mask >>= 1;
            }
        }

        if (_mask < 0)
        {
            struct in_addr in;
            inet_aton(ipaddr, &in);

            int n1 = in.s_addr & 0xFF;
            if (1 <= n1
                    && n1 < 127)
                _mask = 8;
            else if (128 <= n1
                     && n1 < 192)
                _mask = 16;
            else if (192 <= n1
                     && n1 < 224)
                _mask = 24;
            else
                _mask = 32;
        }

        if (ipaddr)
        {
            if (is_valid_ip(ipaddr))
                fprintf(fp, "static ip_address=%s/%d\n", ipaddr, _mask);
            else
                fprintf(fp, "static ip_address=0.0.0.0/32\n");
        }

        if (gateway
                && is_valid_ip(gateway))
            fprintf(fp, "static routers=%s\n", gateway);

        if (dns1
                && dns2
                && is_valid_ip(dns1)
                && is_valid_ip(dns2))
            fprintf(fp, "static domain_name_servers=%s %s\n", dns1, dns2);
        else if (dns1
                 && is_valid_ip(dns1))
            fprintf(fp, "static domain_name_servers=%s\n", dns1);
        else if (dns2
                 && is_valid_ip(dns2))
            fprintf(fp, "static domain_name_servers=%s\n", dns2);
    }

    fflush(fp);
    if(fd > 0)
    {
        fsync(fd);
    }

    fclose(fp);

    return 0;
}
