#ifndef _SPIDEV_H
#define _SPIDEV_H

#include <stdint.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <getopt.h> 
#include <fcntl.h> 
#include <pthread.h>
#include <sys/ioctl.h> 
#include <linux/types.h> 
#include <linux/spi/spidev.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
   这里整理下ioctl的命令:

   SPI_IOC_RD_MODE     //读 模式 
   SPI_IOC_RD_LSB_FIRST    //读 LSB 
   SPI_IOC_RD_BITS_PER_WORD    //读 每字多少位 
   SPI_IOC_RD_MAX_SPEED_HZ //读 最大速率 
   SPI_IOC_WR_MODE     //写 模式 
   SPI_IOC_WR_LSB_FIRST    //写 LSB 
   SPI_IOC_WR_BITS_PER_WORD    //写 每字多少位 
   SPI_IOC_WR_MAX_SPEED_HZ //写 最大速率 
   SPI_IOC_MESSAGE(n)      //传输n个数据包 
   */

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0])) 


static uint8_t mode = 0; 
static uint8_t bits = 8; 
static uint32_t speed = 6000000;
static uint16_t delay = 0; 


struct spidev_t {
    const char *name;

    const char *spi;

	int fd;

    pthread_mutex_t mutex;
};


static void pabort(const char *s) 
{ 
	perror(s); 
    abort();
}

static int spidev_open(const char *spidevice)
{
    int ret = 0;
    int fd = -1;

	fd = open(spidevice, O_RDWR);  //打开设备文件 
	if (fd < 0) 
    {
        perror("can't open spidevice");
        return fd;
    }

#if 1
	/*
	 * spi mode //设置spi设备模式
	 */ 
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);	//写模式 
	if (ret == -1) 
    {
        perror("can't set spi mode");
        return ret;
    }

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);	//读模式 
    if (ret == -1)
    {
        perror("can't get spi mode");
        return ret;
    }

	/*
	 * bits per word	//设置每个字含多少位
	 */ 
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);	//写 每个字含多少位 
    if (ret == -1)
    {
        perror("can't set bits per word");
        return ret;
    }

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);	//读 每个字含多少位 
    if (ret == -1)
    {
        perror("can't get bits per word");
        return ret;
    }

	/*
	 * max speed hz 	//设置速率
	 */ 
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);	//写速率 
    if (ret == -1)
    {
        perror("can't set max speed hz");
        return ret;
    }

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);	//读速率 
    if (ret == -1)
    {
        perror("can't get max speed hz");
        return ret;
    }
#endif

	return fd;
}

static int spidev_init(struct spidev_t *spidev)
{
    if (spidev->fd > 0)
    {
        close(spidev->fd);

        spidev->fd = -1;
    }

    spidev->fd = spidev_open(spidev->spi);
    if (spidev->fd < 0)
    {
        fprintf(stderr, "%s: open %s error !\n", __func__, spidev->spi);

        return spidev->fd;
    }

    pthread_mutex_init(&spidev->mutex, NULL);

    return 0;
}

static void spidev_exit(struct spidev_t *spidev)
{
    if (spidev->fd > 0)
    {
        close(spidev->fd);

        spidev->fd = -1;
    }

    pthread_mutex_destroy(&spidev->mutex);
}

    static
int spidev_write_read(struct spidev_t *spidev, uint8_t *buf, int len)
{ 
	int ret = 0; 

	struct spi_ioc_transfer tr = {	//声明并初始化spi_ioc_transfer结构体 
		.tx_buf = (unsigned long)buf, 
		.rx_buf = (unsigned long)buf, 
		.len = (uint32_t)len, //ARRAY_SIZE(tx),
        .speed_hz = speed,
		.delay_usecs = delay, 
		.bits_per_word = bits, 
	};

    //pthread_mutex_lock(&spidev->mutex);

	//SPI_IOC_MESSAGE(1)的1表示spi_ioc_transfer的数量 
    ret = ioctl(spidev->fd, SPI_IOC_MESSAGE(1), &tr);	//ioctl默认操作,传输数据
	if (ret < 1) 
        perror("can't send spi message");

    //pthread_mutex_unlock(&spidev->mutex);

	return ret; 
} 

    static
int spidev_write(struct spidev_t *spidev, uint8_t *buf, int len)
{ 
	int ret = 0; 

	struct spi_ioc_transfer tr = {	//声明并初始化spi_ioc_transfer结构体 
		.tx_buf = (unsigned long)buf, 
		.rx_buf = (unsigned long)0, 
		.len = (uint32_t)len, //ARRAY_SIZE(tx),
        .speed_hz = speed,
        .delay_usecs = delay,
		.bits_per_word = bits, 
	}; 

    //pthread_mutex_lock(&spidev->mutex);

	//SPI_IOC_MESSAGE(1)的1表示spi_ioc_transfer的数量 
    ret = ioctl(spidev->fd, SPI_IOC_MESSAGE(1), &tr);	//ioctl默认操作,传输数据
	if (ret < 1) 
        perror("can't send spi message");

    //pthread_mutex_unlock(&spidev->mutex);

	return ret; 
}

#ifdef __cplusplus
}
#endif

#endif


