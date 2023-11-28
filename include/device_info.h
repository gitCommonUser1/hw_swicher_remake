#ifndef _DEVICE_INFO_H
#define _DEVICE_INFO_H

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief get_version 获取系统版本号
 * @return
 */
char *get_version();

/**
 * @brief get_buildinfo 获取buildinfo
 * @return
 */
char *get_buildinfo();

/**
 * @brief get_device_id 获取设备ID
 * @return
 */
char *get_device_id();

/**
 * @brief set_device_id 设置设备ID
 * @param device_str
 * @return
 */
int set_device_id(const char *device_str);

/**
 * @brief get_device_mac 获取设备MAC地址
 * @param mac
 * @return
 */
int get_device_mac(uint8_t mac[6]);

/**
 * @brief set_device_mac 设置设备MAC地址
 * @param mac
 * @return
 */
int set_device_mac(const uint8_t mac[6]);

/**
 * @brief get_hostname: 获取设备名
 * @return
 */
char* get_hostname();

/**
 * @brief set_hostname: 设置设备名
 * @param hostname
 * @return
 */
int set_hostname(char *hostname);

/**
 * @brief get_cpu_serial
 * @return
 */
uint64_t get_cpu_serial();

/**
 * @brief get_fpga_id
 * @return
 */
uint32_t get_fpga_id();

/**
 * @brief set_license
 * @param buf
 * @param len
 * @return
 */
int set_license(const uint8_t *buf, const int len);

/**
 * @brief get_license
 * @param buf
 * @param len
 * @return
 */
int get_license(uint8_t **buf, int *len);

/**
 * @brief get_license_status
 * @return
 */
int get_license_status();

#ifdef __cplusplus
}
#endif

#endif //_DEVICE_INFO_H
