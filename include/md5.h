/*
 * This file was transplanted with slight modifications from Linux sources
 * (fs/cifs/md5.h) into U-Boot by Bartlomiej Sieka <tur@semihalf.com>.
 */

#ifndef _MD5_H
#define _MD5_H

#include "general.h"

#ifdef __cplusplus
extern "C" {
#endif

struct MD5Context {
    uint32_t buf[4];
    uint32_t bits[2];
    union {
        uint8_t in[64];
        uint32_t in32[16];
    };
};

/*
 * Calculate and store in 'output' the MD5 digest of 'len' bytes at
 * 'input'. 'output' must have enough space to hold 16 bytes.
 */
void md5 (uint8_t *input, int len, uint8_t output[16]);

/*
 * Calculate and store in 'output' the MD5 digest of 'len' bytes at 'input'.
 * 'output' must have enough space to hold 16 bytes. If 'chunk' Trigger the
 * watchdog every 'chunk_sz' bytes of input processed.
 */
void md5_wd (uint8_t *input, int len, uint8_t output[16],
        uint32_t chunk_sz);

#ifdef __cplusplus
}
#endif

#endif /* _MD5_H */
