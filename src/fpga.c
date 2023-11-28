#include <stdio.h>
#include <string.h>

#include "spidev.h"
#include "fpga.h"

struct spidev_t g_fpga_spidev = {                                                  
	.name = "FPGA-spidev",

	.spi = "/dev/spidev1.0",

	.fd = -1,
};                                                                                 

struct fpga_t g_fpga = {                                                           
	.name = "FPGA",

	.spidev = &g_fpga_spidev,
}; 


int fpga_read(struct fpga_t *fpga, uint16_t addr, uint16_t *data)
{
	int ret = 0;
	uint8_t tx[4] = {0, 0, 0, 0};

	if (data == NULL)
		return -1;

	addr = FPGA_OPR_R | (addr & FPGA_OPR_REGS_MASK);

	tx[0] = (addr >> 8);
	tx[1] = addr & 0xFF;

	ret = spidev_write_read(fpga->spidev, tx, 4);
	if (ret < 4)
		return -2;

	*data = (uint16_t)(tx[2] << 8) | (uint16_t)tx[3];

	return 2;
}

int fpga_read_buffer(struct fpga_t *fpga, uint16_t start_addr, uint8_t *buffer, int len)
{
	int ret = 0;

	if (buffer == NULL)
		return -1;

	if (len <= 0)
		return -2;

	uint8_t *tx = malloc(2 + len);
	if (tx == NULL)
		return -4;

	start_addr = FPGA_OPR_R | FPGA_OPR_BURST | (start_addr & FPGA_OPR_REGS_MASK);

	tx[0] = (start_addr >> 8);
	tx[1] = start_addr & 0xFF;
	memset(&tx[2], 0, len);

	ret = spidev_write_read(fpga->spidev, tx, 2 + len);
	if (ret < (2 + len))
		return -5;

	memmove(buffer, &tx[2], len);

	return len;
}

int fpga_write(struct fpga_t *fpga, uint16_t addr, uint16_t data)
{

    printf("write:%x,value:%d\n",addr,data);

	int ret = 0;
	uint8_t tx[4] = {0, 0, 0, 0};

	addr = FPGA_OPR_W | (addr & FPGA_OPR_REGS_MASK);

	tx[0] = (addr >> 8);
	tx[1] = addr & 0xFF;
	tx[2] = (data >> 8) & 0xFF;
	tx[3] = data & 0xFF;

	ret = spidev_write(fpga->spidev, tx, 4);
	if (ret < 4)
		return -1; 

	return 2;
}

int fpga_write_buffer(struct fpga_t *fpga, uint16_t start_addr, uint8_t *buffer, int len)
{
	int ret = 0;

	if (buffer == NULL)
		return -1;

	if (len <= 0)
		return -2;

	uint8_t *tx = malloc(2 + len);
	if (tx == NULL)
		return -4;

	start_addr = FPGA_OPR_W | FPGA_OPR_BURST | (start_addr & FPGA_OPR_REGS_MASK);

	tx[0] = (start_addr >> 8);
	tx[1] = start_addr & 0xFF;
	memmove(&tx[2], buffer, len);

	ret = spidev_write(fpga->spidev, tx, len + 2);
	free(tx);
	if (ret < (len + 2))
		return -5; 

	return len;
}

int fpga_init(struct fpga_t *fpga)
{
	if (fpga->spidev->fd > 0)
		return 0;

	return spidev_init(fpga->spidev);
}

void fpga_close(struct fpga_t *fpga)
{
	spidev_exit(fpga->spidev);
}
