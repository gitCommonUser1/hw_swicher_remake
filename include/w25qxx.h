#ifndef _W25QXX_H
#define _W25QXX_H

#include "spidev.h"

#ifdef __cplusplus
extern "C" {
#endif


	/* Flash opcodes. */
#define w25Qxx_WREN             0x06    /* Write enable */
#define w25Qxx_WRDI				0x04	/* write disable */
#define w25Qxx_RDSR             0x05    /* Read status register */
#define w25Qxx_WRSR             0x01    /* Write status register 1 byte */
#define w25Qxx_NORM_READ		0x03    /* Read data bytes (low frequency) */
#define w25Qxx_PP               0x02    /* byte program */
#define w25Qxx_CHIP_ERASE       0xC7    /* Erase whole flash chip */

#define w25Qxx_BE_4K            0x20    /* Erase 4KiB block */
#define w25Qxx_RDID             0x9F    /* Read JEDEC ID */
#define w25Qxx_EWSR				0x50	/* enable write status register*/

#define w25Qxx_BE_32K			0x52    /* Erase 32KiB block */
#define w25Qxx_SE				0xD8    /* Sector erase (usually 64KiB) */
#define w25Qxx_FAST_READ		0x0B    /* Read data bytes (high frequency) */
#define w25Qxx_AAI				0xAD	/* auto address increment program */ 
#define w25Qxx_BE_64K			0xD8    /* Erase 64KiB block */

	/* SIZE */
#define SIZE_ALL				0x1000000	
#define SIZE_64K				0x10000	// 64K
#define SIZE_32K				0x8000	// 32K
#define SIZE_4K					0x1000	// 4K

#define W25QXX_ADDR_UNKNOWN		0x1000000

	/* FLASH TYPE */
#define w25Qxx					0x00EF4014
#define W25Q16					0x00EF4015
#define W25Q32					0x00EF4016
#define W25Q32FV				0x00EF4016
#define W25X64BV				0x00EF3017
#define W25Q64FV				0x00EF4017
#define W25Q64BV				0x00EF4017
#define W25Q128FV				0x00EF4018
#define W25Q256FV				0x00EF4019


	struct flash_t {
		const char *name;

		struct spidev_t *spidev;

		const int isp_gpio;

		uint32_t id;
		int capacity;
	};


	extern struct spidev_t g_fpga_spidev;
	extern struct flash_t g_fpga_flash;


	int w25Qxx_read_data(struct flash_t *flash, uint32_t Addr, uint8_t *buf, int len);
	int w25Qxx_write_page(struct flash_t *flash, uint32_t Addr, uint8_t *buf, int len);

#define get_w25Qxx_rw_buffer(buf)			(&(buf)[4])
#define to_w25Qxx_rw_buffer_size(size)		((size) + 4)

	int w25Qxx_read_data_fixed(struct flash_t *flash, uint32_t Addr, uint8_t *buf, int len);
	int w25Qxx_write_page_fixed(struct flash_t *flash, uint32_t Addr, uint8_t *buf, int len);


	int w25Qxx_chip_erase(struct flash_t *flash);
	int w25Qxx_erase(struct flash_t *flash, uint32_t startAddr, int size);

	uint32_t w25Qxx_ReadID(struct flash_t *flash);
	uint32_t w25Qxx_GetCapacity(struct flash_t *flash);

	int w25qxx_isp_enable(struct flash_t *flash);
	int w25qxx_isp_disable(struct flash_t *flash);

	int w25Qxx_init(struct flash_t *flash);
	void w25Qxx_close(struct flash_t *flash);


#ifdef __cplusplus
}
#endif


#endif
