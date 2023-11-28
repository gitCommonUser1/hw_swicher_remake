#ifndef _FPGA_H 
#define _FPGA_H 

#include "spidev.h"


#ifdef __cplusplus
extern "C" {
#endif

#define FPGA_OPR_RW_MASK	0x8000
#define FPGA_OPR_R			0x8000
#define FPGA_OPR_W			0x0000

#define FPGA_OPR_BURST_MASK	0x4000
#define FPGA_OPR_BURST		0x4000
#define FPGA_OPR_NOBURST	0x0000

#define FPGA_OPR_REGS_MASK	0x3FFF


#define FPGA_KEY_STATUS0    1

#pragma pack(1)
struct fpga_key0_t {
    uint16_t key : 8;
    uint16_t key1_change : 1;
    uint16_t key2_change : 1;
    uint16_t key3_change : 1;
    uint16_t rotray_change : 1;
    uint16_t reserved : 4;
};
#pragma pack()

#define FPGA_KEY_STATUS1    2
#define FPGA_KEY_STATUS2    3
#define FPGA_KEY_STATUS3    4

#define FPGA_KNOB_VAL       5


#pragma pack(1)
struct fpga_knob_t {
    uint8_t menu;
    uint8_t audio;
};
#pragma pack()


#define FPGA_LED_STATUS0    6
#define FPGA_LED_STATUS1    7
#define FPGA_LED_STATUS2    8
#define FPGA_LED_STATUS3    9

#define FPGA_T_BAR          0x0A

#define VIDEO_OUT_FORMAT 0xe
#define OUT_SEL 0xf
#define HDMI_SDI_SEL 0x10

//Color Back
#define COLOR1_Y 0x14
#define COLOR1_C 0x15
#define COLOR2_Y 0x16
#define COLOR2_C 0x17

//super source
#define SUPER_SRC_SEL 0xc0
#define SUPER_SRC_CTRL 0xc1
#define SUPER_SRC1_MASK_HSTART 0xc2
#define SUPER_SRC1_MASK_VSTART 0xc3
#define SUPER_SRC1_MASK_HEND 0xc4
#define SUPER_SRC1_MASK_VEND 0xc5
#define SUPER_SRC1_BORDER 0xc6
#define SUPER_SRC1_BORDER_Y 0xc7
#define SUPER_SRC1_BORDER_C 0xc8
#define SUPER_SRC_POS 0xc9
#define SUPER_SRC2_MASK_HSTART 0xca
#define SUPER_SRC2_MASK_VSTART 0xcb
#define SUPER_SRC2_MASK_HEND 0xcc
#define SUPER_SRC2_MASK_VEND 0xcd
#define SUPER_SRC2_BORDER 0xce
#define SUPER_SRC2_BORDER_Y 0xcf
#define SUPER_SRC2_BORDER_C 0xd0

//Key Type
#define UPSTREAM_KEY_TYPE 0x1f

//Luma Key
#define LUMA_SRC_SEL 0x20
#define LUMA_KEY_CTRL 0x21
#define LUMA_KEY_GAIN 0x22
#define LUMA_KEY_CLIP 0x23
#define LUMA_KEY_MASK_HSTART 0x24
#define LUMA_KEY_MASK_VSTART 0x25
#define LUMA_KEY_MASK_HEND 0x26
#define LUMA_KEY_MASK_VEND 0x27
#define LUMA_KEY_H_POS 0x28
#define LUMA_KEY_V_POS 0x29
#define LUMA_KEY_SIZE 0x2a

//Key Pattern
#define PATTERN_SRC_SEL 0x58
#define PATTERN_CTRL 0x59
#define PATTERN_WIPE_SIZE 0x5a
#define PATTERN_POSX 0x5b
#define PATTERN_POSY 0x5c
#define PATTERN_H_PARAM 0x5d
#define PATTERN_V_PARAM 0x5e
#define PATTERN_SOFTNESS 0x5f
#define PATTERN_MASK_HSTART 0x60
#define PATTERN_MASK_VSTART 0x61
#define PATTERN_MASK_HEND 0x62
#define PATTERN_MASK_VEND 0x63
#define PATTERN_H_POS 0x64
#define PATTERN_V_POS 0x65
#define PATTERN_SIZE 0x66

//chroma key
#define CHROMA_SRC_SEL 0x34
#define CHROMA_SMP_X_POS 0x38
#define CHROMA_SMP_Y_POS 0x39
#define CHROMA_SMP_Y 0x3a
#define CHROMA_SMP_C 0x3b
#define CHROMA_YS 0x3c
#define CHROMA_K1 0x3d
#define CHROMA_K2 0x3e
#define CHROMA_K3 0x3f
#define CHROMA_K4 0x40
#define CHROMA_K5 0x41
#define CHROMA_K5 0x41
#define CHROMA_K6 0x42
#define CHROMA_K7 0x43
#define CHROMA_K10 0x44
#define CHROMA_K11 0x45
#define CHROMA_K12 0x46
#define CHROMA_CTRL 0x47
#define CHROMA_H_POS 0x4c
#define CHROMA_V_POS 0x4d
#define CHROMA_SIZE 0x4e
#define ADDR_MATTE_GAIN 0x4f
#define ADDR_MATTE_CLIP 0x50
#define CHROMA_MASK_HSTART 0x48
#define CHROMA_MASK_VSTART 0x49
#define CHROMA_MASK_HEND 0x4a
#define CHROMA_MASK_VEND 0x4b



//pip
#define PIP_CTRL 0xa8
#define PIP_SRC_SEL 0xa9
//#define PBP_R_SRC_SEL 0xaa
#define PIP_H_POS 0xab
#define PIP_V_POS 0xac
#define PIP_SIZE 0xad
#define PIP_MASK_HSTART 0xb3
#define PIP_MASK_VSTART 0xb4
#define PIP_MASK_HEND 0xb5
#define PIP_MASK_VEND 0xb6
#define FPGA_PIP_BOARD 0xb7
#define PIP_COLOR_Y 0xb8
#define PIP_COLOR_C 0xb9


//Audio Mixer
#define FPGA_IN1_L_LEV 0x70
#define FPGA_IN1_R_LEV 0x71
#define FPGA_IN2_L_LEV 0x72
#define FPGA_IN2_R_LEV 0x73
#define FPGA_IN3_L_LEV 0x74
#define FPGA_IN3_R_LEV 0x75
#define FPGA_IN4_L_LEV 0x76
#define FPGA_IN4_R_LEV 0x77
#define FPGA_AUX_L_LEV 0x78
#define FPGA_AUX_R_LEV 0x79
#define FPGA_MIC1_L_LEV 0x7a
#define FPGA_MIC1_R_LEV 0x7b
#define FPGA_MIC2_L_LEV 0x7c
#define FPGA_MIC2_R_LEV 0x7d
#define FPGA_MASTER_LEV 0x7e
#define FPGA_MIC1_DELAY 0x7f
#define FPGA_MIC2_DELAY 0x80
#define FPGA_AUDIO_ON 0x81
#define FPGA_AUDIO_AFV 0x82
#define MON_LEVEL 0x83
#define MON_SOURCE 0x84

//dsk
#define FPGA_DSK_RATE 0x96
#define FPGA_DSK_SRC_SEL 0x97
#define FPGA_DSK_CTRL 0x98
#define FPGA_DSK_GAIN 0x99
#define FPGA_DSK_CLIP 0x9a
#define FPGA_DSK_MASK_HSTART 0x9b
#define FPGA_DSK_MASK_VSTART 0x9c
#define FPGA_DSK_MASK_HEND 0x9d
#define FPGA_DSK_MASK_VEND 0x9e

//ftb
#define FTB_AFV 0x9f
#define FTB_RATE 0xa0

//transition
#define DIP_SRC_SEL 0x88
#define MIX_RATE 0x89
#define DIP_RATE 0x8a
#define WIPE_RATE 0x8b
#define WIPE_PATTERN 0x8c
#define WIPE_POSX 0x8d
#define WIPE_POSY 0x8e
#define WIPE_H_PARAM 0x8f
#define WIPE_V_PARAM 0x90
#define WIPE_SOFTNESS 0x91
#define WIPE_BOARD 0x92
#define WIPE_BOARD_SRC_SEL 0x93


//still
#define STILL_UPLOAD 0x18
#define STILL_SEL 0x19

#define FPGA_OSD_CTRL   0xA00

#define MIC_MUTE 0xa01
#define AUDIO_MET_DISP 0xa08
#define UMD_DISP 0xa09
#define PVW_CTRL 0xa0a

#define UMD_PGM 0x0000
#define UMD_PVW 0x0400
#define UMD_IN1 0x0800
#define UMD_IN2 0x0a00
#define UMD_IN3 0x0c00
#define UMD_IN4 0x0e00
#define UMD_AUX 0x1000
#define UMD_STILL1 0x1200
#define UMD_STILL2 0x1400

#pragma pack(1)
struct fpga_osd_ctrl_t {
    uint16_t osd_size : 1;
    uint16_t audio_meter : 1;
    uint16_t reserv : 14;
};
#pragma pack()

enum menu_osd_t {
    MENU_OSD_SMALL = 0,
    MENU_OSD_LARGE,
    MENU_OSD_AUDIO,
    MENU_OSD_AUDIO_LARGE,
};


struct fpga_t {
	const char *name;

	struct spidev_t *spidev;
};


int fpga_read(struct fpga_t *fpga, uint16_t addr, uint16_t *data);
int fpga_read_buffer(struct fpga_t *fpga, uint16_t start_addr, uint8_t *buffer, int len);

int fpga_write(struct fpga_t *fpga, uint16_t addr, uint16_t data);
int fpga_write_buffer(struct fpga_t *fpga, uint16_t start_addr, uint8_t *buffer, int len);

int fpga_init(struct fpga_t *fpga) ;

void fpga_close(struct fpga_t *fpga);


extern struct spidev_t g_fpga_spidev;
extern struct fpga_t g_fpga;


#ifdef __cplusplus
}
#endif

#endif //_FPGA_H
