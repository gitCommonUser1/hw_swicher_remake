#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>

#include "hw_config.h"
#include "general.h"

#include "datafile.h"
#include "osee_errors.h"
#include "rv_switch_api.h"

#include "w25qxx.h"
#include "fpga.h"

#include "device_info.h"


/**
 * @brief get_version 获取系统版本号
 * @return
 */
char *get_version()
{
	char *version = NULL;
	char buf[128];

	int fd = -1;

	if (access(OEM_VERSION, R_OK) != 0)
		return NULL;

	fd = open(OEM_VERSION, O_RDONLY);
	if (fd < 0)
	{
		SYS_ERR("%s: open %s error(%d - %s)\n", __func__, OEM_VERSION, errno, strerror(errno));

		return NULL;
	}

	int ret = read(fd, buf, 128);
	if (ret < 0)
	{
		SYS_ERR("%s: read %s error(%d - %s)\n", __func__, OEM_VERSION, errno, strerror(errno));

		close(fd);
		return NULL;
	}
	buf[ret] = '\0';

	uln(buf);

	close(fd);

	version = (char *)malloc(strlen(buf) + 1);
	if (version == NULL)
	{
		SYS_ERR("%s: malloc error(%d - %s)\n", __func__, errno, strerror(errno));
		return NULL;
	}

	strcpy(version, buf);
	version[strlen(buf)] = '\0';

	return version;
}

/**
 * @brief get_buildinfo 获取buildinfo
 * @return
 */
char *get_buildinfo()
{
	int ret = 0;

	char buf[128];
	char result[64];
	char *buildinfo = NULL;

	int kernelNum = 0;
	int fgpaNum = 0;

	ret = executeCMD("uname -r|awk -F '-' '{print $2}'",
			result, 64);
	if (ret == 0)
	{
		if (strlen(result) > 0)
			kernelNum = atoi(result);
	}

	ret = executeCMD("fpga -r 0",
			result, 64);
	if (ret == 0)
	{
		if (strlen(result) > 0)
			fgpaNum = atoi(&result[13]);
	}

	unsigned char rvNum = rv_switch_get_verion();

	sprintf(buf, "%.2X%.2X%.2X%.2X", kernelNum,OSEE_HW_VERSION_PATCH,rvNum,fgpaNum);

	//sprintf(&buf[strlen(buf)], "%.2X", g_live_version.patch);
	//    sprintf(&buf[strlen(buf)], "%.2X", OSEE_HW_VERSION_PATCH);

	buildinfo = (char *)malloc(strlen(buf) + 1);
	strcpy(buildinfo, buf);
	buildinfo[strlen(buf)] = '\0';

	return buildinfo;
}

#define MISC_FILE_PATH "/dev/block/by-name/misc"
#define MISC_MSG_SIZE   64
#define MISC_MSG_OFFSET (2 * 1024 - MISC_MSG_SIZE)

#define MISC_MAC_SIZE   16
#define MISC_MAC_OFFSET (MISC_MSG_OFFSET - MISC_MAC_SIZE)

/**
 * @brief get_device_id 获取设备ID
 * @return
 */
char *get_device_id()
{
	int ret = 0;
	char *device_str = NULL;

	uint8_t buf[MISC_MSG_SIZE];
	struct file_data_t *p_data = (struct file_data_t *)buf;

	int fd = -1;

	if (access(MISC_FILE_PATH, R_OK) != 0)
		return NULL;

	fd = open(MISC_FILE_PATH, O_RDONLY);
	if (fd < 0)
	{
		SYS_ERR("%s: open %s error(%d - %s)\n", __func__, MISC_FILE_PATH, errno, strerror(errno));

		return NULL;
	}

	lseek(fd, MISC_MSG_OFFSET, SEEK_SET);

	ret = read(fd, buf, MISC_MSG_SIZE);
	if (ret != MISC_MSG_SIZE)
	{
		SYS_ERR("%s: read %s error(%d - %s)\n", __func__, MISC_FILE_PATH, errno, strerror(errno));

		close(fd);
		return NULL;
	}
	close(fd);

	if (0 != check_crc(p_data, MISC_MSG_SIZE))
	{
		return NULL;
	}

	device_str = (char *)malloc(p_data->len + 1);
	memmove(device_str, p_data->data, p_data->len);
	device_str[p_data->len] = '\0';

	return device_str;
}

/**
 * @brief set_device_id 设置设备ID
 * @param device_str
 * @return
 */
int set_device_id(const char *device_str)
{
	int ret = 0;

	uint8_t buf[MISC_MSG_SIZE];
	struct file_data_t *p_data = (struct file_data_t *)buf;

	int fd = -1;

	if (device_str == NULL
			|| strlen(device_str) <= 0)
	{
		return -ERROR_NULL;
	}

	if (strlen(device_str) > (MISC_MSG_SIZE - sizeof(struct file_data_t)))
		return -ERROR_CMD_LONG;

	if (access(MISC_FILE_PATH, R_OK) != 0)
		return -ERROR_CANNOT_ACCESS;

	fd = open(MISC_FILE_PATH, O_RDWR | O_CREAT, 0644);
	if (fd < 0)
	{
		SYS_ERR("%s: open %s error(%d - %s)\n", __func__, MISC_FILE_PATH, errno, strerror(errno));

		return -ERROR_OPEN;
	}

	lseek(fd, MISC_MSG_OFFSET, SEEK_SET);

	p_data->len = strlen(device_str);
	strcpy((char *)p_data->data, device_str);
	fill_crc(p_data);

	ret = write(fd, buf, MISC_MSG_SIZE);
	if (ret != MISC_MSG_SIZE)
	{
		SYS_ERR("%s: write %s error(%d - %s)\n", __func__, MISC_FILE_PATH, errno, strerror(errno));

		close(fd);

		return -ERROR_WRITE;
	}

	fsync(fd);
	close(fd);

	fd = open(MISC_FILE_PATH, O_RDONLY);
	if (fd < 0)
	{
		SYS_ERR("%s: open %s error(%d - %s)\n", __func__, MISC_FILE_PATH, errno, strerror(errno));

		return -ERROR_OPEN;
	}

	lseek(fd, MISC_MSG_OFFSET, SEEK_SET);

	ret = read(fd, buf, MISC_MSG_SIZE);
	if (ret != MISC_MSG_SIZE)
	{
		SYS_ERR("%s: read %s error(%d - %s)\n", __func__, MISC_FILE_PATH, errno, strerror(errno));

		close(fd);
		return -ERROR_READ;
	}

	close(fd);

	if (0 != check_crc(p_data, MISC_MSG_SIZE))
	{
		return -ERROR_DATA;
	}

	if (p_data->len != strlen(device_str)
			|| strncmp((char *)p_data->data, device_str, p_data->len))
	{
		return -ERROR_DATA;
	}

	return 0;
}

/**
 * @brief get_device_mac 获取设备MAC地址
 * @param mac
 * @return
 */
int get_device_mac(uint8_t mac[6])
{
	int ret = 0;

	uint8_t buf[MISC_MAC_SIZE];
	struct file_data_t *p_data = (struct file_data_t *)buf;

	int fd = -1;

	memset(mac, 0, 6);

	if (access(MISC_FILE_PATH, R_OK) != 0)
		return -ERROR_CANNOT_ACCESS;

	fd = open(MISC_FILE_PATH, O_RDONLY);
	if (fd < 0)
	{
		SYS_ERR("%s: open %s error(%d - %s)\n", __func__, MISC_FILE_PATH, errno, strerror(errno));

		return -ERROR_OPEN;
	}

	lseek(fd, MISC_MAC_OFFSET, SEEK_SET);

	ret = read(fd, buf, MISC_MAC_SIZE);
	if (ret != MISC_MAC_SIZE)
	{
		SYS_ERR("%s: read %s error(%d - %s)\n", __func__, MISC_FILE_PATH, errno, strerror(errno));

		close(fd);
		return -ERROR_READ;
	}
	close(fd);

	if (p_data->len != 6)
	{
		return -ERROR_DATA;
	}

	if (0 != check_crc(p_data, MISC_MAC_SIZE))
	{
		return -ERROR_DATA;
	}

	memmove(mac, p_data->data, p_data->len);

	return 0;
}

/**
 * @brief set_device_mac 设置设备MAC地址
 * @param mac
 * @return
 */
int set_device_mac(const uint8_t mac[6])
{
	int ret = 0;

	uint8_t buf[MISC_MAC_SIZE];
	struct file_data_t *p_data = (struct file_data_t *)buf;

	int fd = -1;

	if (access(MISC_FILE_PATH, R_OK) != 0)
		return -ERROR_CANNOT_ACCESS;

	fd = open(MISC_FILE_PATH, O_RDWR | O_CREAT, 0644);
	if (fd < 0)
	{
		SYS_ERR("%s: open %s error(%d - %s)\n", __func__, MISC_FILE_PATH, errno, strerror(errno));

		return -ERROR_OPEN;
	}

	lseek(fd, MISC_MAC_OFFSET, SEEK_SET);

	p_data->len = 6;
	memmove((void *)p_data->data, (void *)mac, 6);
	fill_crc(p_data);

	ret = write(fd, buf, MISC_MAC_SIZE);
	if (ret != MISC_MAC_SIZE)
	{
		SYS_ERR("%s: write %s error(%d - %s)\n", __func__, MISC_FILE_PATH, errno, strerror(errno));

		close(fd);

		return -ERROR_WRITE;
	}

	fsync(fd);
	close(fd);

	fd = open(MISC_FILE_PATH, O_RDONLY);
	if (fd < 0)
	{
		SYS_ERR("%s: open %s error(%d - %s)\n", __func__, MISC_FILE_PATH, errno, strerror(errno));

		return -ERROR_OPEN;
	}

	lseek(fd, MISC_MAC_OFFSET, SEEK_SET);

	ret = read(fd, buf, MISC_MAC_SIZE);
	if (ret != MISC_MAC_SIZE)
	{
		SYS_ERR("%s: read %s error(%d - %s)\n", __func__, MISC_FILE_PATH, errno, strerror(errno));

		close(fd);
		return -ERROR_READ;
	}

	close(fd);

	if (p_data->len != 6)
		return -ERROR_DATA;

	if (0 != check_crc(p_data, MISC_MAC_SIZE))
	{
		return -ERROR_DATA;
	}

	if (memcmp ((void *)p_data->data, (void *)mac, 6))
	{
		return -ERROR_DATA;
	}

	return 0;
}

#define SET_HOSTNAME    "/usr/bin/set_hostname.sh"
#define AVAHI_CONFIG    "/etc/avahi/avahi-daemon.conf"

char* get_hostname()
{
#define HSTRS_LEN   256

	FILE *fp;
	char hstrs[HSTRS_LEN] = {0};

	char *hostname = NULL;

	if (access(AVAHI_CONFIG, R_OK) < 0)
		return NULL;

	fp = fopen(AVAHI_CONFIG, "r");
	if(fp == NULL)
		return NULL;

	while ( ! feof(fp))
	{
		if (NULL == fgets(hstrs, HSTRS_LEN, fp))
			break;

		if (strlen(hstrs) == 0)
			continue;

		if (hstrs[0] == '#')
			continue;

		if (strncmp(hstrs, "host-name=", 10) != 0)
			continue;

		uln(hstrs);

		hostname = (char *)malloc(strlen(hstrs) - 10 + 1);
		if (hostname == NULL)
			return NULL;

		strcpy(hostname, &hstrs[10]);

		fclose(fp);

		return hostname;
	}

	fclose(fp);

	return NULL;
}

int set_hostname(char *hostname)
{
	int ret = 0;
	char *cmd = NULL;

	int start = 0, end = 0;

	if (hostname == NULL
			|| strlen(hostname) <= 0)
	{
		return -ERROR_NULL;
	}

	//删除最后面的空格 \n \r \t
	for(end = strlen(hostname) - 1; end >= 0; end--)
	{
		if (hostname[end] != '\n'
				&& hostname[end] != '\r'
				&& hostname[end] != ' '
				&& hostname[end] != '\t')
		{
			break;
		}
	}

	//删除最前面的空格 \n \r \t
	for(start = 0; start < end; start++)
	{
		if (hostname[start] != '\n'
				&& hostname[start] != '\r'
				&& hostname[start] != ' '
				&& hostname[start] != '\t')
		{
			break;
		}
	}

	if ((end - start + 1) <= 0)
		return -ERROR_NULL;

	if (access(SET_HOSTNAME, X_OK) < 0)
	{
		return -ERROR_NO_EXIST;
	}

	int cmd_len = strlen(SET_HOSTNAME) + 3 + end - start;

	cmd = (char *)malloc(cmd_len);
	if (cmd == NULL)
	{
		return -ERROR_MEM;
	}

	snprintf(cmd, cmd_len, "%s %s", SET_HOSTNAME, &hostname[start]);
	ret = system(cmd);

	if (cmd)
		free(cmd);

	return ret;
}

/**
 * @brief get_cpu_serial
 * @return
 */
uint64_t get_cpu_serial()
{
#define HSTRS_LEN   256
	FILE *fp;
	char hstrs[HSTRS_LEN] = "";

	uint64_t num = 0;

	fp = fopen("/proc/cpuinfo", "r");
	if (fp == NULL)
	{
		printf("%s: fopen /proc/cpuinfo error (%d - %s)!\n", __func__, errno, strerror(errno));

		return num;
	}

	while(! feof(fp))
	{
		if (NULL == fgets(hstrs, HSTRS_LEN, fp))
			break;

		if (strncmp(hstrs, "Serial", 6) != 0)
			continue;

		char *p = strchr(hstrs, ':');
		if (p == NULL)
			break;

		p++;

		while(*p != '\0'
				&& *p == ' ')
			p++;

		sscanf(p, "%16llx", &num);

		break;
	}

	fclose(fp);

	printf("%s: %#llX\n", __func__, num);

	return num;
}

/**
 * @brief get_fpga_id
 * @return
 */
uint32_t get_fpga_id()
{
    uint16_t tmp;
    uint32_t id = 0;

    fpga_read(&g_fpga, 0xD1, &tmp);
    id |= (uint32_t)tmp;

    fpga_read(&g_fpga, 0xD2, &tmp);
    id |= (uint32_t)tmp << 16;

    return id;
}

/**
 * @brief set_license
 * @param buf
 * @param len
 * @return
 */
int set_license(const uint8_t *buf, const int len)
{
#define LICENSE_START   0x7FF000
	int ret = 0;

	int flashaddr = LICENSE_START;
	int last;
	int n;
	uint8_t tbuf[to_w25Qxx_rw_buffer_size(256)];

	uint8_t *buffer = NULL;
	struct file_data_t *p_data = NULL;

	ret = w25Qxx_init(&g_fpga_flash);
	if (ret < 0)
	{
		printf("%s: w25Qxx_init error(%d) !\n", __func__, ret);
		return ret;
	}

	if (g_fpga_flash.capacity < (LICENSE_START + len))
	{
		printf("%s: capacity:%d is not enough !!!\n", __func__, g_fpga_flash.capacity);

		return -ERROR_OUTRANGE;
	}

	buffer = (uint8_t *)malloc(sizeof(struct file_data_t) + len);
	if (buffer == NULL)
	{
		printf("%s: malloc buffer %d error(%d - %s)\n", __func__,
				sizeof(struct file_data_t) + len,
				errno, strerror(errno));

		w25Qxx_close(&g_fpga_flash);

		return -ERROR_MEM;
	}

	p_data = (struct file_data_t *)buffer;

	p_data->len = len;
	memmove(p_data->data, buf, len);
	fill_crc(p_data);

	ret = w25Qxx_erase(&g_fpga_flash, LICENSE_START, 4096);
	if (ret < 0)
	{
		printf("%s: w25Qxx_erase error(%d) !\n", __func__, ret);
		return ret;
	}

	flashaddr = LICENSE_START;
	last = sizeof(struct file_data_t) + len;
	while (last > 0)
	{
		n = (256 <= last) ? 256 : last;

		memmove(get_w25Qxx_rw_buffer(tbuf), (uint8_t *)&buffer[flashaddr - LICENSE_START], n);
		if (n < 256)
			memset(&buf[to_w25Qxx_rw_buffer_size(n)], 0xFF, 256 - n);

		w25Qxx_write_page_fixed(&g_fpga_flash, flashaddr, tbuf, 256);

		last -= 256;
		flashaddr += 256;

		//printf("%s: w[%.2f%%]\r", __func__, ((float)(imagesize - last) / imagesize) * 100);
	}

	w25Qxx_close(&g_fpga_flash);

	if (buffer)
		free(buffer);

    system("go");

	return 0;
}

/**
 * @brief get_license
 * @param buf
 * @param len
 * @return
 */
int get_license(uint8_t **buf, int *len)
{
#define LICENSE_START   0x7FF000
    int ret = 0;

    uint8_t *buffer = NULL;
    struct file_data_t *p_data = NULL;

    *buf = NULL;
    *len = 0;

    ret = w25Qxx_init(&g_fpga_flash);
    if (ret < 0)
    {
        printf("%s: w25Qxx_init error(%d) !\n", __func__, ret);
        return ret;
    }

    int size = to_w25Qxx_rw_buffer_size(sizeof(struct file_data_t) + 256);
    buffer = (uint8_t *)malloc(size);
    if (buffer == NULL)
    {
        printf("%s: malloc buffer %d error(%d - %s)\n", __func__,
                sizeof(struct file_data_t) + len,
                errno, strerror(errno));

        w25Qxx_close(&g_fpga_flash);

        return -ERROR_MEM;
    }

    ret = w25Qxx_read_data_fixed(&g_fpga_flash, LICENSE_START, buffer, 256);

    p_data = (struct file_data_t *)get_w25Qxx_rw_buffer(buffer);

    if (check_crc(p_data, 256) == 0)
    {
        *buf = malloc(p_data->len);
        memmove(*buf, p_data->data, p_data->len);

        *len = p_data->len;
    }

    w25Qxx_close(&g_fpga_flash);

    if (buffer)
        free(buffer);

    return 0;
}

int get_license_status()
{
    uint16_t tmp;

    fpga_read(&g_fpga, 0xD3, &tmp);

    return (tmp & 0x01);
}
