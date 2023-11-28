﻿#ifndef MENUENUMS_H
#define MENUENUMS_H

#include <QObject>

//-------------------------------FIRST---------------------------------//

enum MENU_FIRST{
    MENU_FIRST_COLOR_BACK = 0,
    MENU_FIRST_SUPER_SOURCE,
    MENU_FIRST_KEY_TYPE,
    MENU_FIRST_LUMA_KEY,
    MENU_FIRST_CHROMA_KEY,
    MENU_FIRST_KEY_PATTERN,
    MENU_FIRST_PIP,
    MENU_FIRST_TRANSITION,
    MENU_FIRST_DSK,
    MENU_FIRST_FTB,
    MENU_FIRST_AUDIO_MIXER,
    MENU_FIRST_STILL_GENERATOR,
    MENU_FIRST_MACRO,
    MENU_FIRST_STREAM,
    MENU_FIRST_PLAYBACK,
    MENU_FIRST_SETTING,
    MENU_FIRST_MAX
};

//-------------------------------SECOND---------------------------------//
//color back
enum MENU_SECOND_COLOR_BACK{
    COLOR_BACK_COLOR1 = 0,
    COLOR_BACK_COLOR2,
    COLOR_BACK_MAX
};
//super source
enum MENU_SECOND_SUPER_SOURCE{
    SUPER_SOURCE_ENABLE = 0,
    SUPER_SOURCE_SOURCE,
    SUPER_SOURCE_CONTROL,
    SUPER_SOURCE_MASK1,
    SUPER_SOURCE_MASK2,
    SUPER_SOURCE_BORDER1,
    SUPER_SOURCE_BORDER2,
    SUPER_SOURCE_MAX
};
//key type
enum MENU_SECOND_KEY_TYPE{
    KEY_TYPE_TYPE = 0,
    KEY_TYPE_MAX
};
//luma key
enum MENU_SECOND_LUMA_KEY{
    LUMA_KEY_SOURCE = 0,
    LUMA_KEY_MASK,
    LUMA_KEY_CONTROL,
    LUMA_KEY_RESIZE,
    LUMA_KEY_MAX
};
//chroma key
enum MENU_SECOND_CHROMA_KEY{
    CHROMA_KEY_SOURCE = 0,
    CHROMA_KEY_MASK,
    CHROMA_KEY_RESIZE,
    CHROMA_KEY_CONTROL,
    CHROMA_KEY_MAX
};
//key pattern
enum MENU_SECOND_PATTERN{
    PATTERN_SOURCE = 0,
    PATTERN_WIPE,
    PATTERN_MASK,
    PATTERN_RESIZE,
    PATTERN_MAX
};
//pip
enum MENU_SECOND_PIP{
    PIP_SOURCE = 0,
    PIP_SIZE_POSITION,
    PIP_MASK,
    PIP_BOARD,
    PIP_COLOR,
    PIP_MAX
};
//transition
enum MENU_SECOND_TRANSITION{
    TRANSITION_MIX = 0,
    TRANSITION_DIP,
    TRANSITION_WIPE,
    TRANSITION_MAX
};
//dsk
enum MENU_SECOND_DSK{
    DSK_SOURCE = 0,
    DSK_MASK,
    DSK_CONTROL,
    DSK_RATE,
    DSK_MAX
};
//ftb
enum MENU_SECOND_FTB{
    MENU_SECOND_FTB_RATE = 0,
    MENU_SECOND_FTB_AUDIO,
    FTB_MAX
};
//audio mixer
enum MENU_SECOND_AUDIO_MIXER{
    AUDIO_MIXER_MIC1 = 0,
    AUDIO_MIXER_MIC2,
    AUDIO_MIXER_IN1,
    AUDIO_MIXER_IN2,
    AUDIO_MIXER_IN3,
    AUDIO_MIXER_IN4,
    AUDIO_MIXER_AUX,
    AUDIO_MIXER_PGM,
    AUDIO_MIXER_MONITOR,
    AUDIO_MIXER_MAX
};
//still generate
enum MENU_SECOND_STILL_GENERATE{
    STILL_GENERATE_SELECTION = 0,
    STILL_GENERATE_UPLOAD,
    STILL_GENERATE_MAX
};
//macro
enum MENU_SECOND_MACRO{
    MACRO_MACRO = 0,
    MACRO_MAX,
};
//stream
enum MENU_SECOND_STREAM{
    STREAM_STREAM1 = 0,
    STREAM_STREAM2,
    STREAM_STREAM3,
    STREAM_MAX
};
//playback
enum MENU_SECOND_PLAYBACK{
    PLAYBACK_PLAYBACK = 0,
    PLAYBACK_MAX
};
//setting
enum MENU_SECOND_SETTING{
    SETTING_VERSION = 0,
    SETTING_SRC_NAME,
    SETTING_MV_METER,
    SETTING_MV_LAYOUT,
    SETTING_MARKER,
    SETTING_MIC_INPUT,
    SETTING_RECORD_FILE,
    SETTING_AUX_SOURCE,
    SETTING_OUT_FORMAT,
    SETTING_OUT_SOURCE,
    SETTING_QUALITY,
    SETTING_NETWORK,
    SETTING_PANEL,
    SETTING_LANGUAGE,
    SETTING_RESET,
    SETTING_MAX
};

//-------------------------------THIRD---------------------------------//

/*color back*/

// color back 1
enum MENU_THIRD_COLOR1{
    COLORBACK1_HUE = 0,
    COLORBACK1_SATURATION,
    COLORBACK1_BRIGHTNESS,
    COLORBACK1_MAX
};
//color back 2
enum MENU_THIRD_COLOR2{
    COLORBACK2_HUE = 0,
    COLORBACK2_SATURATION,
    COLORBACK2_BRIGHTNESS,
    COLORBACK2_MAX
};


/*super source*/

//enable
enum MENU_THIRD_SUPER_ENABLE{
    SUPER_ENABLE = 0,
    SUPER_MAX
};
//source
enum MENU_THIRD_SUPER_SOURCE{
    SUPER_SOURCE_SOURCE_SOURCE1 = 0,
    SUPER_SOURCE_SOURCE_SOURCE2,
    SUPER_SOURCE_SOURCE_BACKGROUND,
    SUPER_SOURCE_SOURCE_MAX
};
//control
enum MENU_THIRD_SUPER_CONTROL{
    SUPER_CONTROL_STYLE = 0,
    SUPER_CONTROL_Y_POSITION,
    SUPER_CONTROL_MAX
};
//mask1
enum MENU_THIRD_SUPER_MASK1{
    SUPER_MASK1_ENABLE = 0,
    SUPER_MASK1_H_START,
    SUPER_MASK1_V_START,
    SUPER_MASK1_H_END,
    SUPER_MASK1_V_END,
    SUPER_MASK1_MAX
};
//mask2
enum MENU_THIRD_SUPER_MASK2{
    SUPER_MASK2_ENABLE = 0,
    SUPER_MASK2_H_START,
    SUPER_MASK2_V_START,
    SUPER_MASK2_H_END,
    SUPER_MASK2_V_END,
    SUPER_MASK2_MAX
};
//border1
enum MENU_THIRD_SUPER_BORDER1{
    SUPER_BORDER1_WIDTH = 0,
    SUPER_BORDER1_HUE,
    SUPER_BORDER1_SATURATION,
    SUPER_BORDER1_BRIGHTNESS,
    SUPER_BORDER1_MAX
};
//border2
enum MENU_THIRD_SUPER_BORDER2{
    SUPER_BORDER2_WIDTH = 0,
    SUPER_BORDER2_HUE,
    SUPER_BORDER2_SATURATION,
    SUPER_BORDER2_BRIGHTNESS,
    SUPER_BORDER2_MAX
};


/*key type*/

//type
enum MENU_THIRD_TYPE{
    TYPE_TYPE = 0,
    TYPE_MAX
};


/*luma key*/

//source
enum MENU_THIRD_LUMA_KEY_SOURCE{
    LUMA_KEY_SOURCE_FILL = 0,
    LUMA_KEY_SOURCE_KEY
};
//mask
enum MENU_THIRD_LUMA_KEY_MASK{
    LUMA_KEY_MASK_ENABLE = 0,
    LUMA_KEY_MASK_H_START,
    LUMA_KEY_MASK_V_START,
    LUMA_KEY_MASK_H_END,
    LUMA_KEY_MASK_V_END
};
//control
enum MENU_THIRD_LUMA_KEY_CONTROL{
    LUMA_KEY_CONTROL_SHAPED_KEY = 0,
    LUMA_KEY_CONTROL_CLIP,
    LUMA_KEY_CONTROL_GAIN,
    LUMA_KEY_CONTROL_INVERT
};
//resize
enum MENU_THIRD_LUMA_KEY_RESIZE{
    LUMA_KEY_RESIZE_RESIZE = 0,
    LUMA_KEY_RESIZE_SIZE,
    LUMA_KEY_RESIZE_X_POSITION,
    LUMA_KEY_RESIZE_Y_POSITION,
    LUMA_KEY_RESIZE_MAX
};

/*chroma key*/

//source
enum MENU_THIRD_CHROMA_KEY_SOURCE{
    CHROMA_KEY_SOURCE_FILL = 0
};
//mask
enum MENU_THIRD_CHROMA_KEY_MASK{
    CHROMA_KEY_MASK_ENABLE = 0,
    CHROMA_KEY_MASK_H_START,
    CHROMA_KEY_MASK_V_START,
    CHROMA_KEY_MASK_H_END,
    CHROMA_KEY_MASK_V_END
};
//resize
enum MENU_THIRD_CHROMA_KEY_RESIZE{
    CHROMA_KEY_RESIZE_RESIZE = 0,
    CHROMA_KEY_RESIZE_SIZE,
    CHROMA_KEY_RESIZE_X_POSITION,
    CHROMA_KEY_RESIZE_Y_POSITION,
    CHROMA_KEY_RESIZE_MAX
};
//control
enum MENU_THIRD_CHROMA_KEY_CONTROL{
    CHROMA_KEY_CONTROL_SMP_X_POSITION = 0,
    CHROMA_KEY_CONTROL_SMP_Y_POSITION,
    CHROMA_KEY_CONTROL_SMP_ENABLE,
    CHROMA_KEY_CONTROL_Foreground,
    CHROMA_KEY_CONTROL_Background,
    CHROMA_KEY_CONTROL_KeyEdge,
    CHROMA_KEY_CONTROL_MAX
};


/*key pattern*/

//source
enum KEY_PATTERN_CHROMA_KEY_SOURCE{
    KEY_PATTERN_SOURCE_FILL = 0
};
//wipe
enum MENU_THIRD_KEY_PATTERN_WIPE{
    KEY_PATTERN_WIPE_PATTERN = 0,
    KEY_PATTERN_WIPE_SIZE,
    KEY_PATTERN_WIPE_POS_X,
    KEY_PATTERN_WIPE_POS_Y,
    KEY_PATTERN_WIPE_SYMMERTRY,
    KEY_PATTERN_WIPE_SOFTNESS,
    KEY_PATTERN_WIPE_MAX
};
//mask
enum MENU_THIRD_KEY_PATTERN_MASK{
    KEY_PATTERN_MASK_ENABLE = 0,
    KEY_PATTERN_MASK_H_START,
    KEY_PATTERN_MASK_V_START,
    KEY_PATTERN_MASK_H_END,
    KEY_PATTERN_MASK_V_END
};
//resize
enum MENU_THIRD_KEY_PATTERN_RESIZE{
    KEY_PATTERN_RESIZE_RESIZE = 0,
    KEY_PATTERN_RESIZE_SIZE,
    KEY_PATTERN_RESIZE_X_POSITION,
    KEY_PATTERN_RESIZE_Y_POSITION,
    KEY_PATTERN_RESIZE_MAX
};


/*pip*/

//source
enum MENU_THIRD_PIP_SOURCE{
    PIP_SOURCE_FILL = 0,
    PIP_SOURCE_MAX
};
//size
enum MENU_THIRD_PIP_SIZE{
    PIP_SIZE_SIZE = 0,
    PIP_SIZE_X_POSITION,
    PIP_SIZE_Y_POSITION,
    PIP_SIZE_MAX
};
//mask
enum MENU_THIRD_PIP_MASK{
    PIP_MASK_ENABLE = 0,
    PIP_MASK_H_START,
    PIP_MASK_V_START,
    PIP_MASK_H_END,
    PIP_MASK_V_END
};
//border
enum MENU_THIRD_PIP_BORDER{
    PIP_BORDER_ENABLE = 0,
    PIP_BORDER_WIDTH,
    PIP_BORDER_MAX
};
//color
enum MENU_THIRD_PIP_COLOR{
    PIP_COLOR_HUE = 0,
    PIP_COLOR_SATURATION = 0,
    PIP_COLOR_BRIGHTNESS = 0,
};


/*transition*/

//mix
enum MENU_THIRD_TRANSITION_MIX{
    TRANSITION_MIX_RATE = 0,
    TRANSITION_MIX_MAX
};
//dip
enum MENU_THIRD_TRANSITION_DIP{
    TRANSITION_DIP_SOURCE = 0,
    TRANSITION_DIP_RATE,
    TRANSITION_DIP_MAX
};
//wipe
enum MENU_THIRD_TRANSITION_WIPE{
    TRANSITION_WIPE_PATTERN = 0,
    TRANSITION_WIPE_RATE,
    TRANSITION_WIPE_POSITIONX,
    TRANSITION_WIPE_POSITIONY,
    TRANSITION_WIPE_DIRECTION,
    TRANSITION_WIPE_SYMMERTRY,
//    TRANSITION_WIPE_SYMMERTRY_CIRCLE,
    TRANSITION_WIPE_SOFTNESS,
    TRANSITION_WIPE_BOARD,
    TRANSITION_WIPE_FILL_SOURCE,
    TRANSITION_WIPE_MAX
};


/*dsk*/

//source
enum MENU_THIRD_DSK_SOURCE{
    DSK_SOURCE_FILL = 0,
    DSK_SOURCE_KEY
};
//mask
enum MENU_THIRD_DSK_MASK{
    DSK_MASK_ENABLE = 0,
    DSK_MASK_H_START,
    DSK_MASK_V_START,
    DSK_MASK_H_END,
    DSK_MASK_V_END
};
//control
enum MENU_THIRD_DSK_CONTROL{
    DSK_CONTROL_SHAPED_KEY = 0,
    DSK_CONTROL_CLIP,
    DSK_CONTROL_GAIN,
    DSK_CONTROL_INVERT
};
//rate
enum MENU_THIRD_DSK_RATE{
    DSK_RATE_RATE = 0,
    DSK_RATE_MAX
};


/*ftb*/

//rate
enum MENU_THIRD_FTB_RATE{
    FTB_RATE_RATE = 0,
    FTB_RATE_MAX
};
//audio
enum MENU_THIRD_FTB_AUDIO{
    FTB_AUDIO_AFV = 0,
    FTB_AUDIO_MAX
};


/*audio mixer*/

//mic1
enum MENU_THIRD_MIC1{
    MIC1_FADER = 0,
    MIC1_BALANCE,
    MIC1_INPUT,
    MIC1_DELAY,
    MIC1_ENABLE,
    MIC1_MAX
};
//mic2
enum MENU_THIRD_MIC2{
    MIC2_FADER = 0,
    MIC2_BALANCE,
    MIC2_INPUT,
    MIC2_DELAY,
    MIC2_ENABLE,
    MIC2_MAX
};
//in1
enum MENU_THIRD_IN1{
    IN1_FADER = 0,
    IN1_BALANCE,
    IN1_INPUT,
    IN1_ENABLE,
    IN1_MAX
};
//in2
enum MENU_THIRD_IN2{
    IN2_FADER = 0,
    IN2_BALANCE,
    IN2_INPUT,
    IN2_ENABLE,
    IN2_MAX
};
//in3
enum MENU_THIRD_IN3{
    IN3_FADER = 0,
    IN3_BALANCE,
    IN3_INPUT,
    IN3_ENABLE,
    IN3_MAX
};
//in4
enum MENU_THIRD_IN4{
    IN4_FADER = 0,
    IN4_BALANCE,
    IN4_INPUT,
    IN4_ENABLE,
    IN4_MAX
};
//aux
enum MENU_THIRD_AUX{
    AUX_FADER = 0,
    AUX_BALANCE,
    AUX_INPUT,
    AUX_ENABLE,
    AUX_MAX
};
//pgm
enum MENU_THIRD_PGM{
    PGM_FADER = 0,
    PGM_MAX
};
//monitor
enum MENU_THIRD_MONITOR{
    MONITOR_LEVEL = 0,
    MONITOR_SOURCE,
    MONITOR_MAX
};


/*still generator*/

//still selection
enum MENU_THIRD_STILL_SELECTION{
    STILL_SELECTION_STILL1 = 0,
    STILL_SELECTION_STILL2,
    STILL_SELECTION_MAX
};
//still upload
enum MENU_THIRD_STILL_UPLOAD{
    STILL_UPLOAD_LOCATION = 0,
    STILL_UPLOAD_LOAD_PICTURE,
    STILL_UPLOAD_CAPTURE,
    STILL_UPLOAD_MAX
};


/*macro*/

//macro
enum MENU_THIRD_MACRO{
    MENU_THIRD_MACRO_SLEEP = 0,
    MENU_THIRD_MACRO_IMPORT,
    MENU_THIRD_MACRO_EXPORT,
    MENU_THIRD_MACRO_MAX
};


/*stream*/
//此条枚举被3个Stream共用，特例特例特例！！！
//此条枚举被3个Stream共用，特例特例特例！！！
//此条枚举被3个Stream共用，特例特例特例！！！
//
enum MENU_THIRD_STREAM{
    MENU_THIRD_STREAM_PLATFORM = 0,
    MENU_THIRD_STREAM_SERVER,
    MENU_THIRD_STREAM_KEY,
    MENU_THIRD_STREAM_UPLOAD_KEY,
    MENU_THIRD_STREAM_OUTPUT,
    MENU_THIRD_STREAM_MAX
};


/*playback*/

//playback
enum MENU_THIRD_PLAYBACK{
    MENU_THIRD_PLAYBACK_MODE = 0,
    MENU_THIRD_PLAYBACK_MAX
};


/*setting*/

//version
enum SETTING_VERSION{
    VERSION_VERSION = 0,
    VERSION_BUILD_INFO,
    VERSION_DEVICE_ID,
    VERSION_MAX
};
//src name
enum MENU_THIRD_SRC_NAME{
    SRC_NAME_PGM = 0,
    SRC_NAME_PVW,
    SRC_NAME_IN1,
    SRC_NAME_IN2,
    SRC_NAME_IN3,
    SRC_NAME_IN4,
    SRC_NAME_AUX,
    SRC_NAME_STILL1,
    SRC_NAME_STILL2,
    SRC_NAME_MAX
};
//mv meter
enum MENU_THIRD_MV_METER{
    MV_METER_PGM = 0,
    MV_METER_IN1,
    MV_METER_IN2,
    MV_METER_IN3,
    MV_METER_IN4,
    MV_METER_AUX,
    MV_METER_MAX,
};
//mv layout
enum MENU_THIRD_MV_LAYOUT{
    MV_LAYOUT_SWAP = 0,
    MV_LAYOUT_MAX
};
//marker
enum MENU_THIRD_MARKER{
    MARKER_PVW = 0,
    MARKER_MAX
};
//mic input
enum MENU_THIRD_MIC_INPUT{
    MENU_THIRD_MIC_INPUT_MIC_1_INPUT = 0,
    MENU_THIRD_MIC_INPUT_MIC_2_INPUT,
    MENU_THIRD_MIC_INPUT_MAX
};
//record file
enum MENU_THIRD_SETTING_RECODE_FILE{
    SETTING_RECORD_FILE_NAME = 0,
    SETTING_RECORD_FILE_MAX
};
//src selection
enum MENU_THIRD_SETTING_AUX_SOURCE{
    SETTING_AUX_SOURCE_IN1 = 0,
    SETTING_AUX_SOURCE_IN2 ,
    SETTING_AUX_SOURCE_IN3 ,
    SETTING_AUX_SOURCE_IN4 ,
    SETTING_AUX_SOURCE_SOURCE,
    SETTING_AUX_SOURCE_MAX
};
//out format
enum MENU_THIRD_SETTING_OUT_FORMAT{
    SETTING_OUT_FORMAT_FORMAT = 0,
    SETTING_OUT_FORMAT_OUTPUT1_COLOR_SPACE,
    SETTING_OUT_FORMAT_OUTPUT2_COLOR_SPACE,
    SETTING_OUT_FORMAT_MAX
};
//out source
enum MENU_THIRD_SETTING_OUT_SOURCE{
    SETTING_OUT_SOURCE_HDMI1 = 0,
    SETTING_OUT_SOURCE_HDMI2,
    SETTING_OUT_SOURCE_AUX,
    SETTING_OUT_SOURCE_MAX
};
//quality
enum MENU_THIRD_QUALITY{
    QUALITY_RECORDING = 0,
    QUALITY_STREAMING,
    QUALITY_MAX
};
//network
enum MENU_THIRD_SETTING_NETWORK{
    NETWORK_PROTOCOL = 0,
    NETWORK_IP_ADDRESS,
    NETWORK_SUBNET_MASK,
    NETWORK_GATEWAY,
    NETWORK_PRIMARY_DNS,
    NETWORK_SECONDAY_DNS,
    NETWORK_MAX
};
//panel
enum MENU_THIRD_PANEL{
    MENU_THIRD_PANEL_BUTTON_BRIGHTNESS = 0,
    MENU_THIRD_PANEL_MAX
};
//language
enum SETTING_LANGUAGE{
    SETTING_LANGUAGE_LANGUAGE = 0,
    SETTING_LANGUAGE_MAX
};
//reset
enum MENU_THIRD_SETTING_RESET{
    SETTING_RESET_FACTORY = 0,
    SETTING_RESET_SD_FORMAT,
    SETTING_RESET_MAX
};














//-------------------------------OTHER---------------------------------//


enum AUDIO_MIXER_ENABLE{
    AUDIO_OFF= 0,
    AUDIO_ON,
    AUDIO_AFV,
    AUDIO_MAX
};

enum AUDIO_MIXER_MONITOR_SOURCE{
    AUDIO_SOURCE_IN1 = 0,
    AUDIO_SOURCE_IN2,
    AUDIO_SOURCE_IN3,
    AUDIO_SOURCE_IN4,
    AUDIO_SOURCE_AUX,
    AUDIO_SOURCE_MIC1,
    AUDIO_SOURCE_MIC2,
    AUDIO_SOURCE_PGM
};




//Transition Type
enum TRANSITION_TYPE{
    LEFT_TO_RIGHT = 0,
    TOP_TO_BOTTOM,
    VERTICAL_CENTER,
    HORIZONTAL_CENTER,
    CROSS_CENTER,
    SQUARE_CENTER,
    DIAMOND,
    CIRCLE,
    SQUARE_TOP_LEFT,
    SQUARE_TOP_RIGHT,
    SQUARE_BOTTOM_RIGHT,
    SQUARE_BOTTOM_LEFT,
    SQUARE_TOP_CENTER,
    SQUARE_LEFT_CENTER,
    SQUARE_BOTTOM_CENTER,
    SQUARE_RIGHT_CENTER,
    BOTTOM_RIGHT_ANGLE,
    BOTTOM_LEFT_ANGLE
};





enum AUX_SOURCE_INDEX{
    AUX_SOURCE_SD_CARD = 0,
    AUX_SOURCE_USB_CAMERA,
    AUX_SOURCE_NDI,
    AUX_SOURCE_MAX
};

enum MV_LAYOUT_PGM_PVW_SWAP{
    MV_LAYOUT_SWAP_PGM_PVW = 0,
    MV_LAYOUT_SWAP_PVW_PGM,
    MV_LAYOUT_SWAP_MAX
};

enum SRC_SELECTION_INDEX{
    SRC_SELECTION_HDMI = 0,
    SRC_SELECTION_SDI,
    SRC_SELECTION_MAX
};




enum NETWORK_PROTOCOL_INDEX{
    NETWORK_PROTOCOL_DHCP = 0,
    NETWORK_PROTOCOL_STATIC_IP,
    NETWORK_PROTOCOL_MAX
};





enum MENU_THIRD_SETTING_LANGUAGE{
    LANGUAGE_ENGLISH = 0,
    LANGUAGE_CHINESE,
    LANGUAGE_MAX
};


//Key size
enum KEY_RESIZE_SIZE{
    KEY_SIZE_0_25 = 0,
    KEY_SIZE_0_33,
    KEY_SIZE_0_50,
    KEY_SIZE_MAX
};

//enable
enum STREAM_OUTPUT_ENABLE{
    OUTPUT_ENABLE = 0,
    OUTPUT_DISABLE ,
    OUTPUT_MAX
};

enum SUPER_SOURCE_MODE{
    ZOOM_IN = 0,
    CROP_ZOOM,
    ZOOM_IN_CROP,
    CROP,
    SUPER_SOURCE_MODE_MAX
};


//source
//In 1, In 2, In 3, In 4, Aux, Still 1, Still 1 Key, Still 2, Still 2 Key, Color 1, Color 2, Color Bar, Black
enum INPUT_SOURCE{
    INPUT_SOURCE_IN1 = 0,
    INPUT_SOURCE_IN2,
    INPUT_SOURCE_IN3,
    INPUT_SOURCE_IN4,
    INPUT_SOURCE_AUX,
    INPUT_SOURCE_STILL1,
    INPUT_SOURCE_STILL1_KEY,
    INPUT_SOURCE_STILL2,
    INPUT_SOURCE_STILL2_KEY,
    INPUT_SOURCE_COLOR1,
    INPUT_SOURCE_COLOR2,
    INPUT_SOURCE_COLOR_BAR,
    INPUT_SOURCE_BLACK,
    INPUT_SOURCE_MAX
};



enum MIC_INPUT{
    MIC_INPUT_MIC_POWER = 0,
    MIC_INPUT_MIC,
    MIC_INPUT_LINE,
    MIC_INPUT_MAX
};

enum OUT_FORMAT{
    OUT_FORMAT_1080P24 = 0,
    OUT_FORMAT_1080P25,
    OUT_FORMAT_1080P30,
    OUT_FORMAT_1080P50,
    OUT_FORMAT_1080P60,
    OUT_FORMAT_MAX,
};

enum OUT_COLOR_SPACE{
    COLOR_SPACE_AUTO = 0,
    COLOR_SPACE_RGB_FULL,
    COLOR_SPACE_RGB_LIMIT,
    COLOR_SPACE_YCBCR422_FULL,
    COLOR_SPACE_YCBCR422_LIMIT,
    COLOR_SPACE_YCBCR444_FULL,
    COLOR_SPACE_YCBCR444_LIMIT,
    COLOR_SPACE_MAX
};

enum OUT_SOURCE{
    OUT_SOURCE_IN1 = 0,
    OUT_SOURCE_IN2,
    OUT_SOURCE_IN3,
    OUT_SOURCE_IN4,
    OUT_SOURCE_AXU,
    OUT_SOURCE_PGM,
    OUT_SOURCE_PVW,
    OUT_SOURCE_MULTIVIEW,
    OUT_SOURCE_MAX
};


//quality
enum QUALITY{
    QUALITY_HIGN = 0,
    QUALITY_MID,
    QUALITY_LOW
};

enum PLAYBACK_MODES{
    PLAY_ONCE = 0,
    REPEAT,
    SEQUENTIAL
};

enum MENU_ON_OFF{
    MENU_ON = 0,
    MENU_OFF,
    MENU_ON_OFF_MAX
};





/*---------------------------------------------------------------------------------*/

#endif // MENUENUMS_H
