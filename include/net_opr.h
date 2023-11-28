#ifndef _NET_OPR_H
#define _NET_OPR_H

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

#define ETHERNET_NETIF  "eth0"

#define DNS_FILE    "/etc/resolv.conf"
#define ROUTE_FILE  "/proc/net/route"

#define DHCP_CONF   "/etc/dhcpcd.conf"
#define DNS_PATH    "/run/dhcpcd/resolv.conf"


/**
 * @brief is_valid_ip: ip地址是否有效
 * @param ipaddr
 * @return
 */
int is_valid_ip(const char *ipaddr);

/**
 * @brief is_valid_netmask: 子网掩码是否有效
 * @param netmask
 * @return
 */
int is_valid_netmask(const char *netmask);

/**
 * @brief is_same_network: ip，网关是否处于同一网段
 * @param ipaddr
 * @param netmask
 * @param gateway
 * @return
 */
int is_same_network(const char *ipaddr, const char *netmask, const char *gateway);

/**
 * @brief prefixlen2mask: 子网掩码长度转换成字符串形式的子网掩码
 * @param prefixlen
 * @param netmask
 * @param netmask_len
 * @return
 */
int prefixlen2mask(uint8_t prefixlen, char *netmask, int netmask_len);

/**
 * @brief get_netstatus: 获取当前网卡状态
 * @param netif
 * @return ifr_flags
 */
int get_netstatus(const char *netif, uint32_t *flags);

/**
 * @brief get_netspeedmode: 获取当前网卡速度模式（10/100/1000)
 * @param netif
 * @return
 */
int get_netspeedmode(const char *netif);

/**
 * @brief _is_net_loopback: 是否为 回环接口(lo)
 * @param flags
 * @return
 */
int _is_net_loopback(uint32_t flags);

/**
 * @brief _is_net_up: 是否使能该网卡
 * @param flags
 * @return
 */
int _is_net_up(uint32_t flags);

/**
 * @brief _is_net_link: 网卡是否物理连通
 * @param flags
 * @return
 */
int _is_net_link(uint32_t flags);

/**
 * @brief is_net_up: 是否使能该网卡
 * @param netif
 * @return
 */
int is_net_up(const char *netif);

/**
 * @brief is_net_link: 网卡是否物理连通
 * @param netif
 * @return
 */
int is_net_link(const char *netif);

/**
 * @brief get_ipaddr: 获取ip地址
 * @param netif: 网卡名称
 * @param ipaddr
 * @param ipaddr_len
 * @return
 */
int get_ipaddr(const char *netif, char *ipaddr, int ipaddr_len);

/**
 * @brief get_netmask: 获取子网掩码
 * @param netif: 网卡名称
 * @param mask
 * @param mask_len
 * @return
 */
int get_netmask(const char *netif, char *mask, int mask_len);

/**
 * @brief get_gateway: 获取网关地址
 * @param netif: 网卡名称
 * @param gateway
 * @param gateway_len
 * @return
 */
int get_gateway(const char *netif, char *gateway, int gateway_len);

/**
 * @brief get_dns: 获取DNS地址
 * @param netif: 网卡名称
 * @param dns1
 * @param dns1_len
 * @param dns2
 * @param dns2_len
 * @return
 */
int get_dns(const char *netif, char *dns1, int dns1_len, char *dns2, int dns2_len);

/**
 * @brief set_ip_mask: 同时设置ip，子网掩码
 * @param netif: 网卡名称
 * @param ip
 * @param mask
 * @return
 */
int set_ip_mask(const char *netif, const char *ip, const char *mask);

/**
 * @brief set_net_up 使能网卡
 * @param netif: 网卡名称
 * @return
 */
int set_net_up(const char *netif);

/**
 * @brief set_net_down 停用网卡
 * @param netif: 网卡名称
 * @return
 */
int set_net_down(const char *netif);

/**
 * @brief set_ipaddr: 设置ip
 * @param netif
 * @param ip
 * @return
 */
int set_ipaddr(const char *netif, const char *ip);

/**
 * @brief set_netmask: 设置子网掩码
 * @param netif: 网卡名称
 * @param mask
 * @return
 */
int set_netmask(const char *netif, const char *mask);

/**
 * @brief set_gateway: 设置网关地址
 * @param netif: 网卡名称
 * @param gateway
 * @return
 */
int set_gateway(const char *netif, const char *gateway);

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
                                const char *dns2);

#ifdef __cplusplus
}
#endif

#endif //_NET_OPR_H
