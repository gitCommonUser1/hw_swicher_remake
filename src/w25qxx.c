/*
 * w25qxx.c
 *
 * Copyright (C) 2016      Osee Technology Co. Ltd.
 *
 */

#include <stdint.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <getopt.h> 
#include <fcntl.h> 
#include <sys/ioctl.h> 
#include <linux/types.h> 
#include <linux/spi/spidev.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "gpio.h"

#include "spidev.h"
#include "w25qxx.h"


struct spidev_t g_fpga_flash_spidev = {
	.name = "FPGA-Flash-spidev",

	.spi = "/dev/spidev0.0",

	.fd = -1,
};

struct flash_t g_fpga_flash = {
	.name = "FPGA-Flash",

	.spidev = &g_fpga_flash_spidev,

	.isp_gpio = 66, //gpio2_A2

	.id = 0,
	.capacity = 0,
};


static inline uint8_t w25Qxx_read_state1(struct flash_t *flash)
{
	uint8_t tx[] = {w25Qxx_RDSR, 0};

	spidev_write_read(flash->spidev, tx, 2);

	return tx[1];
}

static inline uint8_t w25Qxx_read_state2(struct flash_t *flash)
{
	uint8_t tx[] = {0x35, 0};

	spidev_write_read(flash->spidev, tx, 2);

	return tx[1];
}


#define is_w25Qxx_busy(spidev)		(w25Qxx_read_state1((spidev)) & 0x01)


static inline void w25Qxx_write_enable(struct flash_t *flash)
{
	uint8_t tx[] = {w25Qxx_WREN, 0};
	spidev_write(flash->spidev, tx, 1);
}

static inline void w25Qxx_write_disable(struct flash_t *flash)
{
	uint8_t tx[] = {w25Qxx_WRDI, 0};
	spidev_write(flash->spidev, tx, 1);
}

static inline void w25Qxx_write_SR(struct flash_t *flash, uint8_t sr1, uint8_t sr2)
{
	uint8_t tx[] = {w25Qxx_WRSR, 0, 0};

	tx[0] = w25Qxx_EWSR;
	spidev_write(flash->spidev, tx, 1);

	tx[0] = w25Qxx_WRSR;
	tx[1] = sr1;
	tx[2] = sr2;

	spidev_write(flash->spidev, tx, 3);
}

int w25Qxx_read_data(struct flash_t *flash, uint32_t Addr, uint8_t *buf, int len)
{
	uint8_t *tx;

	len = len <= 4092 ? len : 4092;
	tx = malloc(len + 4);

	tx[0] = w25Qxx_NORM_READ;
	tx[1] = (Addr & 0xff0000) >> 16;
	tx[2] = (Addr & 0xff00) >> 8;
	tx[3] = Addr & 0xff;

	spidev_write_read(flash->spidev, tx, len + 4);

	memcpy(buf, &tx[4], len);

	free(tx);

	return len;
}

int w25Qxx_read_data_fixed(struct flash_t *flash, uint32_t Addr, uint8_t *buf, int len)
{
	buf[0] = w25Qxx_NORM_READ;
	buf[1] = (Addr & 0xff0000) >> 16;
	buf[2] = (Addr & 0xff00) >> 8;
	buf[3] = Addr & 0xff;

	spidev_write_read(flash->spidev, buf, len + 4);

	return len;
}

int w25Qxx_write_page(struct flash_t *flash, uint32_t Addr, uint8_t *buf, int len)
{
	uint8_t tx[260];

	w25Qxx_write_enable(flash);

	tx[0] = w25Qxx_PP;
	tx[1] = (Addr & 0xFF0000) >> 16;
	tx[2] = (Addr & 0xFF00) >> 8;
	tx[3] = Addr & 0xFF;
	memcpy(&tx[4], buf, len);

	spidev_write(flash->spidev, tx, (len + 4));

	while(is_w25Qxx_busy(flash));

	return 256;
}

int w25Qxx_write_page_fixed(struct flash_t *flash, uint32_t Addr, uint8_t *buf, int len)
{
	w25Qxx_write_enable(flash);

	buf[0] = w25Qxx_PP;
	buf[1] = (Addr & 0xFF0000) >> 16;
	buf[2] = (Addr & 0xFF00) >> 8;
	buf[3] = Addr & 0xFF;

	spidev_write(flash->spidev, buf, (len + 4));

	while(is_w25Qxx_busy(flash));

    return len;
}


int w25Qxx_chip_erase(struct flash_t *flash)
{
	uint8_t tx[1] = {w25Qxx_CHIP_ERASE};
	int capacity = w25Qxx_GetCapacity(flash);

	w25Qxx_write_SR(flash, 0, 0);

	w25Qxx_write_enable(flash);

	spidev_write(flash->spidev, tx, 1);

	while(is_w25Qxx_busy(flash));

	return capacity;
}

int w25Qxx_erase(struct flash_t *flash, uint32_t startAddr, int size)
{
	uint32_t addr = 0;
	int last = 0;
	int total = 0;

	uint8_t tx[4] = {w25Qxx_BE_64K, 0, 0, 0};

	if (size <= 0)
		return 0;

	if (size >= (flash->capacity ? flash->capacity : SIZE_ALL))
	{
		return w25Qxx_chip_erase(flash);
	}

	addr = startAddr & 0xFFF000;
	total = last = size + (startAddr - addr);

	while(last > 0)
	{	
		w25Qxx_write_enable(flash);

		//if (last >= SIZE_64K)
		{
			tx[0] = w25Qxx_BE_64K;
			tx[1] = (addr & 0xFF0000) >> 16;
			tx[2] = 0x00;
			tx[3] = 0x00;
			spidev_write(flash->spidev, tx, 4);

			addr += SIZE_64K;
			last -= SIZE_64K;
		}
		/*else if (last >= SIZE_32K)
		  {
		  tx[0] = w25Qxx_BE_32K;
		  tx[1] = (addr & 0xFF0000) >> 16;
		  tx[2] = (addr & 0x8000) >> 8;
		  tx[3] = 0x00;
		  spidev_write(spidev->fd, tx, 4);

		  addr += SIZE_32K;
		  last -= SIZE_32K;
		  }
		  else
		  {
		  tx[0] = w25Qxx_BE_4K;
		  tx[1] = (addr & 0xFF0000) >> 16;
		  tx[2] = (addr & 0xF000) >> 8;
		  tx[3] = 0x00;
		  spidev_write(spidev->fd, tx, 4);

		  addr += SIZE_4K;
		  last -= SIZE_4K;
		  }*/

		while(is_w25Qxx_busy(flash));

		w25Qxx_write_disable(flash);
	}

	//printf("%s: %#x, %#x ok\n", __func__, startAddr & 0xFFF000, total - last);

	return (total - last);
}

uint32_t w25Qxx_ReadID(struct flash_t *flash)
{
	uint32_t id = 0;
	uint8_t tx[4] = {w25Qxx_RDID, 0, 0, 0};

	spidev_write_read(flash->spidev, tx, 4);

	id = (tx[1] << 16) | (tx[2] << 8) | tx[3];

	//printf("ID=%Xh\n", id);

	return id;
}

uint32_t w25Qxx_GetCapacity(struct flash_t *flash)
{
	uint32_t Id = 0;
	uint32_t capacity = 0;

	Id = w25Qxx_ReadID(flash);

	capacity = 1 << (Id & 0xFF);

#ifdef _DEBUG
	printf("Flash: ID=%Xh, Size=%#x.\n", Id, capacity);
#endif

	return capacity;
}

int w25qxx_isp_enable(struct flash_t *flash)
{
	int ret = 0;

	ret = gpio_int(flash->isp_gpio, GPIO_OUT);
	if (ret < 0)
		return ret;

	ret = set_gpio_value(flash->isp_gpio, 1);

	return ret;
}

int w25qxx_isp_disable(struct flash_t *flash)
{
	int ret = 0;

	ret = gpio_int(flash->isp_gpio, GPIO_OUT);
	if (ret < 0)
		return ret;

	ret = set_gpio_value(flash->isp_gpio, 0);

	return ret;
}

int w25Qxx_init(struct flash_t *flash)
{
	spidev_init(flash->spidev);

	w25qxx_isp_enable(flash);

	flash->id = w25Qxx_ReadID(flash);
	flash->capacity = w25Qxx_GetCapacity(flash);

	printf("%s ID=%Xh size=0x%X bytes.\n", flash->name, flash->id, flash->capacity);

	return 0;
}

void w25Qxx_close(struct flash_t *flash)
{
	spidev_exit(flash->spidev);

	w25qxx_isp_disable(flash);
}

