#include <QDebug>
#include "models.h"
#include "settings.h"
#include "leftmenumodel.h"
#include "rightmenumodel.h"
#include "QSwitcher.h"
#include "fpga.h"
#include "osee_math.h"
#include "MenuEnums.h"
#include "QSwitcher.h"
#include "include/hw_config.h"
#include "myprovider.h"
#include <QtConcurrent>
#include "AuxCallback.h"
#include "media_sd.h"
#include <QThread>
#include "rv_switch_api.h"
#include "include/net_opr.h"
#include "include/files.h"
#include "KeyImplement.h"
#include "include/qthread_uevent.h"

#include "include/str2umd.h"
#include "include/hdmi_in.h"
#include <QtConcurrent>
#include <QDateTime>
#include "include/hdmi_out.h"
#include <QTranslator>
#include "ndi_license.h"
#include "profile_include.h"

extern Settings *settings;
extern LeftMenuModel *leftMenuModel;
extern RightMenuModel *rightMenuModel;
extern QSwitcher *switcher;
extern struct fpga_t g_fpga;
extern MyProvider *myProvider;
extern Media_sd *media_sd;
extern bool init_settings_is_ok;
extern Profile *profile;

Models::Models(QObject *parent) : QObject(parent)
{
    init_connect();
}

void Models::init_connect()
{
    //color back
    connect(this,&Models::colorBackHue,this,&Models::setColorBackHue);
    connect(this,&Models::colorBackSaturation,this,&Models::setColorBackSaturation);
    connect(this,&Models::colorBackBrightness,this,&Models::setColorBackBrightness);

    //supersource
    connect(this,&Models::superSourceEnable,this,&Models::setSuperSourceEnable);
    connect(this,&Models::superSourceSource1,this,&Models::setSuperSourceSource1);
    connect(this,&Models::superSourceSource2,this,&Models::setSuperSourceSource2);
    connect(this,&Models::superSourceBackground,this,&Models::setSuperSourceBackground);
    connect(this,&Models::superSourceControlStyle,this,&Models::setSuperSourceControlStyle);
    connect(this,&Models::superSourceControlYPosition,this,&Models::setSuperSourceControlYPosition);
    connect(this,&Models::superSourceMaskEnable,this,&Models::setSuperSourceMaskEnable);
    connect(this,&Models::superSourceMaskHStart,this,&Models::setSuperSourceMaskHStart);
    connect(this,&Models::superSourceMaskVStart,this,&Models::setSuperSourceMaskVStart);
    connect(this,&Models::superSourceMaskHEnd,this,&Models::setSuperSourceMaskHEnd);
    connect(this,&Models::superSourceMaskVEnd,this,&Models::setSuperSourceMaskVEnd);
    connect(this,&Models::superSourceBorderWidth,this,&Models::setSuperSourceBorderWidth);
    connect(this,&Models::superSourceBorderColorHue,this,&Models::setSuperSourceBorderColorHue);
    connect(this,&Models::superSourceBorderColorSaturation,this,&Models::setSuperSourceBorderColorSaturation);
    connect(this,&Models::superSourceBorderColorBrightness,this,&Models::setSuperSourceBorderColorBrightness);

    //key type
    connect(this,&Models::keyType,this,&Models::setKeyType);

    //keys
    connect(this,&Models::keyMaskEnable,this,&Models::setKeyMaskEnable);
    connect(this,&Models::keyMaskHStart,this,&Models::setKeyMaskHStart);
    connect(this,&Models::keyMaskVStart,this,&Models::setKeyMaskVStart);
    connect(this,&Models::keyMaskHEnd,this,&Models::setKeyMaskHEnd);
    connect(this,&Models::keyMaskVEnd,this,&Models::setKeyMaskVEnd);
    connect(this,&Models::keyResize,this,&Models::setKeyResize);
    connect(this,&Models::keySize,this,&Models::setKeySize);
    connect(this,&Models::keyXPosition,this,&Models::setKeyXPosition);
    connect(this,&Models::keyYPosition,this,&Models::setKeyYPosition);
    connect(this,&Models::keySourceFill,this,&Models::setKeySourceFill);
    connect(this,&Models::keySourceKey,this,&Models::setKeySourceKey);
    connect(this,&Models::keyClip,this,&Models::setKeyClip);
    connect(this,&Models::keyGain,this,&Models::setKeyGain);
    connect(this,&Models::keyShapedKey,this,&Models::setKeyShapedKey);
    connect(this,&Models::keyInvert,this,&Models::setKeyInvert);

    //luma key


    //chroma key


    //key pattern


    //pip

    //
}


//改动菜单结构，语言切换这里需要修改
void Models::changeLanguage(int language_index)
{
    int index = 0;

    if(language_index = -1)
        index = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_LANGUAGE]->third[SETTING_LANGUAGE_LANGUAGE]->current.toInt();
    else
        index = language_index;

    static QTranslator * translator = nullptr;
    if(!translator){
        translator = new QTranslator();
    }
    qApp->removeTranslator(translator);


    QString language;
    switch (index) {
    case LANGUAGE_ENGLISH:
        language = "./english.qm";
        break;
    case LANGUAGE_CHINESE:
        language = "./chinese.qm";
        break;
    default:
        language = "./english.qm";
        break;
    }

    qDebug() << translator->load(language);
    qDebug() << qApp->installTranslator(translator);

    //first
    {
        languages[settings->listFirst()[MENU_FIRST_COLOR_BACK]->name] = tr("Color Back");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->name] = tr("Super Source");
        languages[settings->listFirst()[MENU_FIRST_KEY_TYPE]->name] = tr("Key Type");
        languages[settings->listFirst()[MENU_FIRST_LUMA_KEY]->name] = tr("Luma Key");
        languages[settings->listFirst()[MENU_FIRST_CHROMA_KEY]->name] = tr("Chroma Key");
        languages[settings->listFirst()[MENU_FIRST_KEY_PATTERN]->name] = tr("Key Pattern");
        languages[settings->listFirst()[MENU_FIRST_PIP]->name] = tr("PIP");
        languages[settings->listFirst()[MENU_FIRST_TRANSITION]->name] = tr("Transition");
        languages[settings->listFirst()[MENU_FIRST_DSK]->name] = tr("DSK");
        languages[settings->listFirst()[MENU_FIRST_FTB]->name]= tr("FTB");
        languages[settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->name] = tr("Audio Mixer");
        languages[settings->listFirst()[MENU_FIRST_STILL_GENERATOR]->name] = tr("Still Generator");
        languages[settings->listFirst()[MENU_FIRST_MACRO]->name] = tr("Macro");
        languages[settings->listFirst()[MENU_FIRST_STREAM]->name] = tr("Stream");
        languages[settings->listFirst()[MENU_FIRST_PLAYBACK]->name] = tr("Playback");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->name] = tr("Setting");
    }

    //second
    {
        languages[settings->listFirst()[MENU_FIRST_COLOR_BACK]->second[COLOR_BACK_COLOR1]->name] = tr("color 1");
        languages[settings->listFirst()[MENU_FIRST_COLOR_BACK]->second[COLOR_BACK_COLOR2]->name] = tr("color 2");

        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_ENABLE]->name] = tr("enable");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_SOURCE]->name] = tr("source");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_CONTROL]->name] = tr("control");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_MASK1]->name] = tr("mask 1");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_MASK2]->name] = tr("mask 2");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_BORDER1]->name] = tr("border 1");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_BORDER2]->name] = tr("border 2");

        languages[settings->listFirst()[MENU_FIRST_KEY_TYPE]->second[KEY_TYPE_TYPE]->name] = tr("type");

        languages[settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_SOURCE]->name] = tr("source");
        languages[settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_MASK]->name] = tr("mask");
        languages[settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_CONTROL]->name] = tr("control");
        languages[settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_RESIZE]->name] = tr("resize");

        languages[settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_SOURCE]->name] = tr("source");
        languages[settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_MASK]->name] = tr("mask");
        languages[settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_RESIZE]->name] = tr("resize");
        languages[settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_CONTROL]->name] = tr("control");

        languages[settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_SOURCE]->name] = tr("source");
        languages[settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_WIPE]->name] = tr("wipe");
        languages[settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_MASK]->name] = tr("mask");
        languages[settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_RESIZE]->name] = tr("resize");

        languages[settings->listFirst()[MENU_FIRST_PIP]->second[PIP_SOURCE]->name] = tr("source");
        languages[settings->listFirst()[MENU_FIRST_PIP]->second[PIP_SIZE_POSITION]->name] = tr("size/position");
        languages[settings->listFirst()[MENU_FIRST_PIP]->second[PIP_MASK]->name] = tr("mask");
        languages[settings->listFirst()[MENU_FIRST_PIP]->second[PIP_BOARD]->name] = tr("border");
        languages[settings->listFirst()[MENU_FIRST_PIP]->second[PIP_COLOR]->name] = tr("color");

        languages[settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_MIX]->name] = tr("mix");
        languages[settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_DIP]->name] = tr("dip");
        languages[settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->name] = tr("wipe");

        languages[settings->listFirst()[MENU_FIRST_DSK]->second[DSK_SOURCE]->name] = tr("source");
        languages[settings->listFirst()[MENU_FIRST_DSK]->second[DSK_MASK]->name] = tr("mask");
        languages[settings->listFirst()[MENU_FIRST_DSK]->second[DSK_CONTROL]->name] = tr("control");
        languages[settings->listFirst()[MENU_FIRST_DSK]->second[DSK_RATE]->name] = tr("rate");

        languages[settings->listFirst()[MENU_FIRST_FTB]->second[MENU_SECOND_FTB_RATE]->name] = tr("rate");
        languages[settings->listFirst()[MENU_FIRST_FTB]->second[MENU_SECOND_FTB_AUDIO]->name] = tr("audio");

        languages[settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_MIC1]->name] = tr("mic 1");
        languages[settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_MIC2]->name] = tr("mic 2");
        languages[settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_IN1]->name] = tr("in 1");
        languages[settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_IN2]->name] = tr("in 2");
        languages[settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_IN3]->name] = tr("in 3");
        languages[settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_IN4]->name] = tr("in 4");
        languages[settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_AUX]->name] = tr("aux");
        languages[settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_PGM]->name] = tr("pgm");
        languages[settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_MONITOR]->name] = tr("monitor");

        languages[settings->listFirst()[MENU_FIRST_STILL_GENERATOR]->second[STILL_GENERATE_SELECTION]->name] = tr("still selection");
        languages[settings->listFirst()[MENU_FIRST_STILL_GENERATOR]->second[STILL_GENERATE_UPLOAD]->name] = tr("still upload");

        languages[settings->listFirst()[MENU_FIRST_MACRO]->second[MACRO_MACRO]->name] = tr("macro");

        languages[settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->name] = tr("stream 1");
        languages[settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM2]->name] = tr("stream 2");
        languages[settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM3]->name] = tr("stream 3");

        languages[settings->listFirst()[MENU_FIRST_PLAYBACK]->second[PLAYBACK_PLAYBACK]->name] = tr("playback");

        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_VERSION]->name] = tr("version");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_SRC_NAME]->name] = tr("src name");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_MV_METER]->name] = tr("mv meter");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_MV_LAYOUT]->name] = tr("mv layout");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_MARKER]->name] = tr("marker");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_MIC_INPUT]->name] = tr("mic input");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_RECORD_FILE]->name] = tr("record file");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_AUX_SOURCE]->name] = tr("src selection");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_FORMAT]->name] = tr("out format");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_SOURCE]->name] = tr("out source");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_QUALITY]->name] = tr("quality");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->name] = tr("network");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_PANEL]->name] = tr("panel");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_LANGUAGE]->name] = tr("language");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_RESET]->name] = tr("reset");
    }

    //third
    {
        // key
        languages[settings->listFirst()[MENU_FIRST_COLOR_BACK]->second[COLOR_BACK_COLOR1]->third[COLORBACK1_HUE]->name] = tr("Hue");
        languages[settings->listFirst()[MENU_FIRST_COLOR_BACK]->second[COLOR_BACK_COLOR1]->third[COLORBACK1_SATURATION]->name] = tr("Saturation");
        languages[settings->listFirst()[MENU_FIRST_COLOR_BACK]->second[COLOR_BACK_COLOR1]->third[COLORBACK1_BRIGHTNESS]->name] = tr("Brightness");

        languages[settings->listFirst()[MENU_FIRST_COLOR_BACK]->second[COLOR_BACK_COLOR2]->third[COLORBACK2_HUE]->name] = tr("Hue");
        languages[settings->listFirst()[MENU_FIRST_COLOR_BACK]->second[COLOR_BACK_COLOR2]->third[COLORBACK2_SATURATION]->name] = tr("Saturation");
        languages[settings->listFirst()[MENU_FIRST_COLOR_BACK]->second[COLOR_BACK_COLOR2]->third[COLORBACK2_BRIGHTNESS]->name] = tr("Brightness");


        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_ENABLE]->third[SUPER_ENABLE]->name] = tr("Enable");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_SOURCE]->third[SUPER_SOURCE_SOURCE_SOURCE1]->name] = tr("Source 1");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_SOURCE]->third[SUPER_SOURCE_SOURCE_SOURCE2]->name] = tr("Source 2");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_SOURCE]->third[SUPER_SOURCE_SOURCE_BACKGROUND]->name] = tr("Background");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_CONTROL]->third[SUPER_CONTROL_STYLE]->name] = tr("Style");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_CONTROL]->third[SUPER_CONTROL_Y_POSITION]->name] = tr("Y Position");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_MASK1]->third[SUPER_MASK1_ENABLE]->name] = tr("Enable");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_MASK1]->third[SUPER_MASK1_H_START]->name] = tr("H Start");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_MASK1]->third[SUPER_MASK1_V_START]->name] = tr("V Start");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_MASK1]->third[SUPER_MASK1_H_END]->name] = tr("H End");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_MASK1]->third[SUPER_MASK1_V_END]->name] = tr("V End");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_MASK2]->third[SUPER_MASK2_ENABLE]->name] = tr("Enable");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_MASK2]->third[SUPER_MASK2_H_START]->name] = tr("H Start");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_MASK2]->third[SUPER_MASK2_V_START]->name] = tr("V Start");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_MASK2]->third[SUPER_MASK2_H_END]->name] = tr("H End");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_MASK2]->third[SUPER_MASK2_V_END]->name] = tr("V End");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_BORDER1]->third[SUPER_BORDER1_WIDTH]->name] = tr("Border");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_BORDER2]->third[SUPER_BORDER2_WIDTH]->name] = tr("Border");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_BORDER2]->third[SUPER_BORDER1_HUE]->name] = tr("Hue");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_BORDER2]->third[SUPER_BORDER1_SATURATION]->name] = tr("Saturation");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_BORDER2]->third[SUPER_BORDER1_BRIGHTNESS]->name] = tr("Brightness");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_BORDER2]->third[SUPER_BORDER2_HUE]->name] = tr("Hue");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_BORDER2]->third[SUPER_BORDER2_SATURATION]->name] = tr("Saturation");
        languages[settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_BORDER2]->third[SUPER_BORDER2_BRIGHTNESS]->name] = tr("Brightness");

        languages[settings->listFirst()[MENU_FIRST_KEY_TYPE]->second[KEY_TYPE_TYPE]->third[TYPE_TYPE]->name] = tr("Type");

        languages[settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_SOURCE]->third[LUMA_KEY_SOURCE_FILL]->name] = tr("Fill");
        languages[settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_SOURCE]->third[LUMA_KEY_SOURCE_KEY]->name] = tr("Key");
        languages[settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_CONTROL]->third[LUMA_KEY_CONTROL_SHAPED_KEY]->name] = tr("Shaped Key");
        languages[settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_CONTROL]->third[LUMA_KEY_CONTROL_CLIP]->name] = tr("Clip");
        languages[settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_CONTROL]->third[LUMA_KEY_CONTROL_GAIN]->name] = tr("Gain");
        languages[settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_CONTROL]->third[LUMA_KEY_CONTROL_INVERT]->name] = tr("Invert");
        languages[settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_RESIZE]->third[LUMA_KEY_RESIZE_RESIZE]->name] = tr("Resize");
        languages[settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_RESIZE]->third[LUMA_KEY_RESIZE_SIZE]->name] = tr("Size");
        languages[settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_RESIZE]->third[LUMA_KEY_RESIZE_X_POSITION]->name] = tr("X Position");
        languages[settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_RESIZE]->third[LUMA_KEY_RESIZE_Y_POSITION]->name] = tr("Y Position");

        languages[settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_CONTROL]->third[CHROMA_KEY_CONTROL_SMP_X_POSITION]->name] = tr("SMP X Position");
        languages[settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_CONTROL]->third[CHROMA_KEY_CONTROL_SMP_Y_POSITION]->name] = tr("SMP Y Position");
        languages[settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_CONTROL]->third[CHROMA_KEY_CONTROL_SMP_ENABLE]->name] = tr("Sample");
        languages[settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_CONTROL]->third[CHROMA_KEY_CONTROL_Foreground]->name] = tr("Foreground");
        languages[settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_CONTROL]->third[CHROMA_KEY_CONTROL_Background]->name] = tr("Background");
        languages[settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_CONTROL]->third[CHROMA_KEY_CONTROL_KeyEdge]->name] = tr("KeyEdge");

        languages[settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_WIPE]->third[KEY_PATTERN_WIPE_PATTERN]->name] = tr("Pattern");
        languages[settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_WIPE]->third[KEY_PATTERN_WIPE_SYMMERTRY]->name] = tr("Symmetry");
        languages[settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_WIPE]->third[KEY_PATTERN_WIPE_SOFTNESS]->name] = tr("Softness");

        languages[settings->listFirst()[MENU_FIRST_PIP]->second[PIP_SOURCE]->third[PIP_SOURCE_FILL]->name] = tr("PIP");
        languages[settings->listFirst()[MENU_FIRST_PIP]->second[PIP_SIZE_POSITION]->third[PIP_SIZE_SIZE]->name] = tr("PIP Size");
        languages[settings->listFirst()[MENU_FIRST_PIP]->second[PIP_BOARD]->third[PIP_BORDER_WIDTH]->name] = tr("Width");

        languages[settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_MIX]->third[FTB_RATE_RATE]->name] = tr("Rate");
        languages[settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_DIP]->third[TRANSITION_DIP_SOURCE]->name] = tr("Source");
        languages[settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_DIRECTION]->name] = tr("Direction");
        languages[settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_FILL_SOURCE]->name] = tr("Fill Source");

        languages[settings->listFirst()[MENU_FIRST_FTB]->second[MENU_SECOND_FTB_AUDIO]->third[FTB_AUDIO_AFV]->name] = tr("AFV");

        languages[settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_MIC1]->third[MIC1_FADER]->name] = tr("Fader");
        languages[settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_MIC1]->third[MIC1_BALANCE]->name] = tr("Balance");
        languages[settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_MIC1]->third[MIC1_INPUT]->name] = tr("Input");
        languages[settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_MIC1]->third[MIC1_DELAY]->name] = tr("Delay");
        languages[settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_MONITOR]->third[MONITOR_LEVEL]->name] = tr("Level");

        languages[settings->listFirst()[MENU_FIRST_STILL_GENERATOR]->second[STILL_GENERATE_SELECTION]->third[STILL_SELECTION_STILL1]->name] = tr("Still 1");
        languages[settings->listFirst()[MENU_FIRST_STILL_GENERATOR]->second[STILL_GENERATE_SELECTION]->third[STILL_SELECTION_STILL2]->name] = tr("Still 2");
        languages[settings->listFirst()[MENU_FIRST_STILL_GENERATOR]->second[STILL_GENERATE_UPLOAD]->third[STILL_UPLOAD_LOCATION]->name] = tr("Location");
        languages[settings->listFirst()[MENU_FIRST_STILL_GENERATOR]->second[STILL_GENERATE_UPLOAD]->third[STILL_UPLOAD_LOAD_PICTURE]->name] = tr("Load Picture");

        languages[settings->listFirst()[MENU_FIRST_MACRO]->second[MACRO_MACRO]->third[MENU_THIRD_MACRO_SLEEP]->name] = tr("Sleep");
        languages[settings->listFirst()[MENU_FIRST_MACRO]->second[MACRO_MACRO]->third[MENU_THIRD_MACRO_IMPORT]->name] = tr("Import");
        languages[settings->listFirst()[MENU_FIRST_MACRO]->second[MACRO_MACRO]->third[MENU_THIRD_MACRO_EXPORT]->name] = tr("Export");

        languages[settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_PLATFORM]->name] = tr("Platform");
        languages[settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_SERVER]->name] = tr("Server");
        languages[settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_KEY]->name] = tr("Key / RTMP URL");
        languages[settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_UPLOAD_KEY]->name] = tr("Upload Key / RTMP URL");
        languages[settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_OUTPUT]->name] = tr("Output");

        languages[settings->listFirst()[MENU_FIRST_PLAYBACK]->second[PLAYBACK_PLAYBACK]->third[MENU_THIRD_PLAYBACK_MODE]->name] = tr("Playback Mode");

        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_VERSION]->third[VERSION_VERSION]->name] = tr("Version");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_VERSION]->third[VERSION_BUILD_INFO]->name] = tr("Build Info");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_VERSION]->third[VERSION_DEVICE_ID]->name] = tr("Device Id");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_MV_LAYOUT]->third[MV_LAYOUT_SWAP]->name] = tr("PGM/PVW SWAP");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_MIC_INPUT]->third[MENU_THIRD_MIC_INPUT_MIC_1_INPUT]->name] = tr("Mic 1 Input");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_MIC_INPUT]->third[MENU_THIRD_MIC_INPUT_MIC_2_INPUT]->name] = tr("Mic 2 Input");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_RECORD_FILE]->third[SETTING_RECORD_FILE_NAME]->name] = tr("File Name");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_FORMAT]->third[SETTING_OUT_FORMAT_FORMAT]->name] = tr("Format");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_FORMAT]->third[SETTING_OUT_FORMAT_OUTPUT1_COLOR_SPACE]->name] = tr("Output 1 Color Space");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_FORMAT]->third[SETTING_OUT_FORMAT_OUTPUT2_COLOR_SPACE]->name] = tr("Output 2 Color Space");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_QUALITY]->third[QUALITY_RECORDING]->name] = tr("Recording");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_QUALITY]->third[QUALITY_STREAMING]->name] = tr("Streaming");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_PROTOCOL]->name] = tr("Protocol");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_IP_ADDRESS]->name] = tr("IP Address");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_SUBNET_MASK]->name] = tr("Subnet Mask");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_GATEWAY]->name] = tr("Gateway");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_PRIMARY_DNS]->name] = tr("Primary DNS");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_SECONDAY_DNS]->name] = tr("Secondary DNS");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_PANEL]->third[MENU_THIRD_PANEL_BUTTON_BRIGHTNESS]->name] = tr("Button Brightness");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_LANGUAGE]->third[SETTING_LANGUAGE_LANGUAGE]->name] = tr("Language");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_RESET]->third[SETTING_RESET_FACTORY]->name] = tr("Factory Reset");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_RESET]->third[SETTING_RESET_SD_FORMAT]->name] = tr("SD Format");

        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_SRC_NAME]->third[SRC_NAME_PGM]->name] = tr("PGM");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_SRC_NAME]->third[SRC_NAME_PVW]->name] = tr("PVW");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_SRC_NAME]->third[SRC_NAME_IN1]->name] = tr("In 1");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_SRC_NAME]->third[SRC_NAME_IN2]->name] = tr("In 2");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_SRC_NAME]->third[SRC_NAME_IN3]->name] = tr("In 3");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_SRC_NAME]->third[SRC_NAME_IN4]->name] = tr("In 4");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_SRC_NAME]->third[SRC_NAME_AUX]->name] = tr("Aux");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_SRC_NAME]->third[SRC_NAME_STILL1]->name] = tr("Still 1");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_SRC_NAME]->third[SRC_NAME_STILL2]->name] = tr("Still 2");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_SOURCE]->third[SETTING_OUT_SOURCE_HDMI1]->name] = tr("HDMI 1");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_SOURCE]->third[SETTING_OUT_SOURCE_HDMI2]->name] = tr("HDMI 2");
        languages[settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_SOURCE]->third[SETTING_OUT_SOURCE_AUX]->name] = tr("Streaming / Recording / UVC");




        //value
        languages["Off"] = tr("Off");
        languages["On"] = tr("On");
        languages["Still 1 Key"] = tr("Still 1 Key");
        languages["Still 2 Key"] = tr("Still 2 Key");
        languages["Color Bar"] = tr("Color Bar");
        languages["Color 1"] = tr("Color 1");
        languages["Color 2"] = tr("Color 2");
        languages["Black"] = tr("Black");
        languages["Zoom in"] = tr("Zoom in");
        languages["Crop-Zoom in"] = tr("Crop-Zoom in");
        languages["Zoom in-Crop"] = tr("Zoom in-Crop");
        languages["Crop"] = tr("Crop");
        languages["0.25"] = tr("0.25");
        languages["0.33"] = tr("0.33");
        languages["0.50"] = tr("0.50");
        languages["Disable"] = tr("Disable");
        languages["Enable"] = tr("Enable");
        languages["Normal"] = tr("Normal");
        languages["Inverse"] = tr("Inverse");
        languages["Mic 1"] = tr("Mic 1");
        languages["Mic 2"] = tr("Mic 2");
        languages["Play Once"] = tr("Play Once");
        languages["Repeat"] = tr("Repeat");
        languages["Sequential"] = tr("Sequential");
        languages["PGM|PVW"] = tr("PGM|PVW");
        languages["PVW|PGM"] = tr("PVW|PGM");
        languages["Mic+power"] = tr("Mic+power");
        languages["Mic"] = tr("Mic");
        languages["Line"] = tr("Line");
        languages["HDMI"] = tr("HDMI");
        languages["SDI"] = tr("SDI");
        languages["SD Card"] = tr("SD Card");
        languages["USB Camera"] = tr("USB Camera");
        languages["NDI"] = tr("NDI");
        languages["1080p24"] = tr("1080p24");
        languages["1080p25"] = tr("1080p25");
        languages["1080p30"] = tr("1080p30");
        languages["1080p50"] = tr("1080p50");
        languages["1080p60"] = tr("1080p60");
        languages["Auto"] = tr("Auto");
        languages["RGB Full"] = tr("RGB Full");
        languages["RGB Limit"] = tr("RGB Limit");
        languages["YCbCr422 Full"] = tr("YCbCr422 Full");
        languages["YCbCr422 Limit"] = tr("YCbCr422 Limit");
        languages["YCbCr444 Full"] = tr("YCbCr444 Full");
        languages["YCbCr444 Limit"] = tr("YCbCr444 Limit");
        languages["Multiview"] = tr("Multiview");
        languages["Low"] = tr("Low");
        languages["Medium"] = tr("Medium");
        languages["High"] = tr("High");
        languages["DHCP"] = tr("DHCP");
        languages["Static IP"] = tr("Static IP");
        languages["English"] = tr("English");
        languages[("简体中文")] = tr("简体中文");
    }

    leftMenuModel->init();

    m_engine->retranslate();
}

#define KEYPRESSSLEEP 50000
void Models::keyClick(int keyIndex)
{
    QSwitcher::set_softkey(keyIndex,1);
    usleep(KEYPRESSSLEEP);
    QSwitcher::set_softkey(keyIndex,0);
}

void Models::initMenu()
{
    setMenu();
}

void Models::setMenu(int first, int second, int third)
{
    if(first == -1)
    {
        settings->setLastThirdUnfold(-1);
        settings->setRightListViewCurrent(-1);
        settings->setLastSecondUnfold(-1);
        settings->setLastFirstUnfold(-1);
        settings->setLeftListViewCurrent(0);
    }
    else
    {
        if(settings->lastFirstUnfold() != first)
        {
            settings->setLastFirstUnfold(-1);
            settings->setLastFirstUnfold(first);
        }
        if(settings->lastSecondUnfold() != second)
        {
            if(second == -1)
            {
                settings->setLeftListViewCurrent(first);
            }
            else
            {
                settings->setLeftListViewCurrent(first + second + 1);//
                settings->setLastSecondUnfold(second);
                if(third == -1)
                {
                    settings->setLastThirdUnfold(-1);
                    settings->setRightListViewCurrent(0);
                }
                else
                {
                    settings->setLastThirdUnfold(third);
                    settings->setRightListViewCurrent(third);
                }
            }
        }
    }
}

void Models::menuUnfold(int index)
{
    for(int i = 0;i < leftMenuModel->m_menu[index].second.size();++i)
    {
        MenuItem item(leftMenuModel->m_menu[index].second[i],MENU_LEVEL_SECOND,i);
        leftMenuModel->insertItem(item,index+1+i);
    }
    leftMenuModel->setUnfold(index,1);
//    settings->setLastFirstUnfold(index);
}

void Models::menuFold()
{
    for(int i = leftMenuModel->m_items.size() - 1;i >= 0;--i)
    {
        if(leftMenuModel->m_items[i].unfold() != 0)
        {
            leftMenuModel->setUnfold(i,0);
        }
        if(leftMenuModel->m_items[i].level() == MENU_LEVEL_SECOND)
        {
            leftMenuModel->removeItem(i);
        }
    }
//    settings->setLastFirstUnfold(-1);
}

void Models::setMenuSize(int size)
{
    if(settings->audioVisible()){
        size += 2;
    }
    fpga_write(&g_fpga, FPGA_OSD_CTRL, size);
}

void Models::setUMDVisivle(int visible)
{
    int value = 0;
    if(visible == 0){
        value = 0x1f7;
    }else{
        value = 0x1ff;
    }
    fpga_write(&g_fpga, UMD_DISP, value);
}

void Models::setMvMeterVisivle(int visible)
{
    int pgm = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_MV_METER]->third[MV_METER_PGM]->current.toInt();
    int in1 = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_MV_METER]->third[MV_METER_IN1]->current.toInt();
    int in2 = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_MV_METER]->third[MV_METER_IN2]->current.toInt();
    int in3 = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_MV_METER]->third[MV_METER_IN3]->current.toInt();
    int in4 = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_MV_METER]->third[MV_METER_IN4]->current.toInt();
    int aux = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_MV_METER]->third[MV_METER_AUX]->current.toInt();

    int value = 0;
    in1 == MENU_ON?value += 1:value += 0;
    in2 == MENU_ON?value += 2:value += 0;
    in3 == MENU_ON?value += 4:value += 0;
//    in4 == MENU_ON?value += 8:value += 0;
    aux == MENU_ON?value += 16:value += 0;
    pgm == MENU_ON?value += 32:value += 0;
    if(visible != 0 && in4 == MENU_ON){
        value += 8;
    }
    fpga_write(&g_fpga,AUDIO_MET_DISP,value);
}

void Models::setMenuVisible(int visible)
{
    if(visible){
        //show
        if(settings->audioMixerSelectedIndex() != -1){
            setMenu(MENU_FIRST_AUDIO_MIXER,settings->audioMixerSelectedIndex());
        }
    }else{
        //hide
        initMenu();
    }
    settings->setMenuVisible(visible);
}

//
void Models::menuKnob(int value)
{
    if(!settings->menuVisible())
        return ;

    if(settings->lastThirdUnfold() != -1)
    {
        //
        if(settings->listFirst()[settings->lastFirstUnfold()]->second[settings->lastSecondUnfold()]->third[settings->lastThirdUnfold()]->type == DATATYPE_TEXT)
            return ;
        int first = settings->lastFirstUnfold();
        int second = settings->lastSecondUnfold();
        int third = settings->lastThirdUnfold();
        double current = settings->listFirst()[first]->second[second]->third[third]->current.toDouble();
        double step = settings->listFirst()[first]->second[second]->third[third]->step.toDouble();
        double finalValue = dround(current + value * step,2);
        // doWork
        settings->listFirst()[first]->second[second]->third[third]->doWork(finalValue);
        return ;
    }

    if(settings->lastSecondUnfold() != -1)
    {
        int finalValue = settings->rightListViewCurrent() + value;
        if(finalValue < 0)
            finalValue = 0;
        if(finalValue >= rightMenuModel->length())
            finalValue = rightMenuModel->length() - 1;
        settings->setRightListViewCurrent(finalValue);
    }
    else
    {
        int finalValue = settings->leftListViewCurrent() + value;
        if(finalValue < 0)
            finalValue = 0;
        if(finalValue >= leftMenuModel->length())
            finalValue = leftMenuModel->length() - 1;
        settings->setLeftListViewCurrent(finalValue);
    }
}

void Models::flushRightMenu()
{
    rightMenuModel->init_items();
}

void Models::changeRightMenu(int first,int second,int third)
{
    rightMenuModel->change_item(first,second,third);
}

void Models::enterRightMenu()
{
    qDebug() << rightMenuModel->length() ;
    if(rightMenuModel->length() == 0)
        return ;

    settings->setLastSecondUnfold(settings->leftListViewCurrent() - settings->lastFirstUnfold() - 1);
    settings->setRightListViewCurrent(0);
}

void Models::quitRightMenu()
{
    settings->setRightListViewCurrent(-1);
    settings->setLastSecondUnfold(-1);
}

void Models::chooseRightMenu()
{
    settings->setLastThirdUnfold(settings->rightListViewCurrent());
}

void Models::cancelRightMenu()
{
    settings->setLastThirdUnfold(-1);
}

void Models::resetFactory()
{
    system("rm " QTKMS_FILE_PATH "; sync");
    system("echo 1 > " FACTORY_RESET_FILE_PATH "; sync");
    system("killall qt_monitor.sh");
    system("reboot");
}

void Models::menuAudioSelectChanged(int index)
{
//    for(int i = KEY_LED_AUDIO_MIC1;i <= KEY_LED_AUDIO_PGM;++i)
//    {
//        if(QSwitcher::get_led(i))
//        {
//            QSwitcher::set_led(KEY_LED_AUDIO_MIC1 + i,SWITCHER_LED_OFF);
//            break;
//        }
//    }
//    QSwitcher::set_led(KEY_LED_AUDIO_AFV,SWITCHER_LED_OFF);
//    QSwitcher::set_led(KEY_LED_AUDIO_ON,SWITCHER_LED_OFF);
//    if(index != -1)
//    {
//        QSwitcher::set_led(KEY_LED_AUDIO_MIC1 + index,SWITCHER_LED_ON);

//        int value = AUDIO_OFF;
//        if(index >= AUDIO_MIXER_MIC1 && index <= AUDIO_MIXER_MIC2){
//            value = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[index]->third[MIC_ENABLE]->current.toInt();
//            if(value == AUDIO_ON)
//                QSwitcher::set_led(KEY_LED_AUDIO_ON,SWITCHER_LED_ON);
//        }
//        else if(index >= AUDIO_MIXER_IN1 && index <= AUDIO_MIXER_AUX){
//            value = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[index]->third[IN_ENABLE]->current.toInt();
//            if(value == AUDIO_ON)
//                QSwitcher::set_led(KEY_LED_AUDIO_ON,SWITCHER_LED_ON);
//            else if(value == AUDIO_AFV)
//                QSwitcher::set_led(KEY_LED_AUDIO_AFV,SWITCHER_LED_ON);
//        }
//    }
//    else
//    {

//    }
}

void Models::menuAudioEnableChnaged(int second,int third)
{
//    int value = AUDIO_OFF;
//    if(second >= AUDIO_MIXER_MIC1 && second <= AUDIO_MIXER_MIC2 && third == MIC_ENABLE){
//        value = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[second]->third[third]->current.toInt();
//        if(value == AUDIO_ON)
//            QSwitcher::set_led(KEY_LED_AUDIO_ON,SWITCHER_LED_ON);
//        else
//            QSwitcher::set_led(KEY_LED_AUDIO_ON,SWITCHER_LED_OFF);
//    }else if(second >= AUDIO_MIXER_IN1 && second <= AUDIO_MIXER_AUX && third == IN_ENABLE){
//        value = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[second]->third[third]->current.toInt();
//        if(value == AUDIO_ON){
//            QSwitcher::set_led(KEY_LED_AUDIO_ON,SWITCHER_LED_ON);
//            QSwitcher::set_led(KEY_LED_AUDIO_AFV,SWITCHER_LED_OFF);
//        }else if(value == AUDIO_AFV){
//            QSwitcher::set_led(KEY_LED_AUDIO_ON,SWITCHER_LED_OFF);
//            QSwitcher::set_led(KEY_LED_AUDIO_AFV,SWITCHER_LED_ON);
//        }else{
//            QSwitcher::set_led(KEY_LED_AUDIO_ON,SWITCHER_LED_OFF);
//            QSwitcher::set_led(KEY_LED_AUDIO_AFV,SWITCHER_LED_OFF);
//        }
//    }
}

void Models::closeAllAudioLed()
{
    for(int i = KEY_LED_AUDIO_MIC1;i <= KEY_LED_AUDIO_PGM;++i)
    {
        QSwitcher::set_led(i,SWITCHER_LED_OFF);
    }
    QSwitcher::set_led(KEY_LED_AUDIO_AFV,SWITCHER_LED_OFF);
    QSwitcher::set_led(KEY_LED_AUDIO_ON,SWITCHER_LED_OFF);
}

void Models::setMonitorLevel(int value)
{
//    if(value > 0)
//    {
//        settings->addMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_MONITOR,MONITOR_LEVEL);
//    }
//    else if(value < 0)
//    {
//        settings->subMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_MONITOR,MONITOR_LEVEL);
//    }

    double current = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_MONITOR]->third[MONITOR_LEVEL]->current.toDouble();
    double step = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_MONITOR]->third[MONITOR_LEVEL]->step.toDouble();
    double finalValue = dround(current + value * step,2);
    settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_MONITOR,MONITOR_LEVEL,finalValue);
}

void Models::setAudioFader(int value)
{
//    int index = settings->lastSecondUnfold();
//    if(index == -1)
//        return ;

//    if(index >= AUDIO_MIXER_MIC1 && index <= AUDIO_MIXER_PGM)
//    {
//        double current = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[index]->third[IN_FADER]->current.toDouble();
//        double step = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[index]->third[IN_FADER]->step.toDouble();
//        double finalValue = dround(current + value * step,2);
//        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,index,IN_FADER,finalValue);
//    }
}

void Models::initDHCPNetworkData()
{
    int protocol = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_PROTOCOL]->current.toInt();
    if(protocol == NETWORK_PROTOCOL_DHCP)
    {
        char ipaddr[16],c_dns1[16],c_dns2[16];
        get_ipaddr(ETHERNET_NETIF, ipaddr, 16);
        settings->setMenuDHCPNetworkIpAddress(ipaddr);

        get_netmask(ETHERNET_NETIF, ipaddr, 16);
        settings->setMenuDHCPNetworkMask(ipaddr);

        get_gateway(ETHERNET_NETIF, ipaddr, 16);
        settings->setMenuDHCPNetworkGateway(ipaddr);

        get_dns(ETHERNET_NETIF, c_dns1, 16, c_dns2, 16);
        settings->setMenuDHCPNetworkPrimaryDNS(c_dns1);
        settings->setMenuDHCPNetworksecondayDNS(c_dns2);
    }
}

int Models::getPGMTally()
{
    uint16_t index;
    fpga_read(&g_fpga,FPGA_T_BAR,&index);
    return index;
}

void Models::setAudioVolumn(int index)
{
//    //这里有菜单的每一级index来查找需要修改的音频，后续PC发起时，需要将菜单的选项先设置好，否则出错
//    int fpga_left = -1;
//    int fpga_right = -1;
//    switch (index) {
//    case AUDIO_MIXER_MIC1:
//        //mic 1
//        fpga_left = FPGA_MIC1_L_LEV;
//        fpga_right = FPGA_MIC1_R_LEV;
//        break;
//    case AUDIO_MIXER_MIC2:
//        //mic 2
//        fpga_left = FPGA_MIC2_L_LEV;
//        fpga_right = FPGA_MIC2_R_LEV;
//        break;
//    case AUDIO_MIXER_IN1:
//        //in 1
//        fpga_left = FPGA_IN1_L_LEV;
//        fpga_right = FPGA_IN1_R_LEV;
//        break;
//    case AUDIO_MIXER_IN2:
//        //in 2
//        fpga_left = FPGA_IN2_L_LEV;
//        fpga_right = FPGA_IN2_R_LEV;
//        break;
//    case AUDIO_MIXER_IN3:
//        //in 3
//        fpga_left = FPGA_IN3_L_LEV;
//        fpga_right = FPGA_IN3_R_LEV;
//        break;
//    case AUDIO_MIXER_IN4:
//        //in 4
//        fpga_left = FPGA_IN4_L_LEV;
//        fpga_right = FPGA_IN4_R_LEV;
//        break;
//    case AUDIO_MIXER_AUX:
//        //aux
//        fpga_left = FPGA_AUX_L_LEV;
//        fpga_right = FPGA_AUX_R_LEV;
//        break;
//    case AUDIO_MIXER_PGM:
//        //pgm
//        fpga_left = FPGA_MASTER_LEV;
////        left = getAudioRegValue(left);
//        break;
//    }
//    double input,fader,balance,left,right;
//    int left_value,right_value;

//    fader = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[index]->third[MIC_FADER]->current.toDouble();

//    QString sFader;
//    if(fader >= 0){
//        sFader += '+';
//        if(fader < 10)
//            sFader += '0';
//    }
//    sFader += QString::number(dround(fader,3),'f',2);
//    if(sFader.indexOf('-') != -1){
//        sFader.replace('-',"\u2212");
//        if(fader > -10)
//            sFader.insert(1,'0');
//    }
//    settings->setAudioNumbers(index,sFader);

//    if( fpga_left != FPGA_MASTER_LEV)
//    {
//        input = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[index]->third[MIC_INPUT]->current.toDouble();
//        balance = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[index]->third[MIC_BALANCE]->current.toDouble();
//        if(index == AUDIO_MIXER_MIC1 || index == AUDIO_MIXER_MIC2)
//        {
//            if(index == AUDIO_MIXER_MIC1)
//            {
//                rv_switch_set_mic0_volume(input,input);
//            }
//            else
//            {
//                rv_switch_set_mic1_volume(input,input);
//            }
//            input = 0;
//        }
//        left = input + fader;
//        right = input + fader;
//        balance < 0?left += balance:right -= balance;
//    }
//    else
//    {
//        left = fader;
//    }
//    left_value = getAudioRegValue(left);
//    right_value = getAudioRegValue(right);

//    if(fpga_left != -1)
//        fpga_write(&g_fpga,fpga_left,left_value);
//    if(fpga_right != -1)
//        fpga_write(&g_fpga,fpga_right,right_value);
}

void Models::setAudioDelay(int index)
{
//    int value= settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[index]->third[MIC_DELAY]->current.toInt();
//    int fpag_value = -1;
//    switch (index) {
//    case 0:
//        fpag_value = FPGA_MIC1_DELAY;
//        break;
//    case 1:
//        fpag_value = FPGA_MIC2_DELAY;
//        break;
//    }

//    if(fpag_value != -1)
//        fpga_write(&g_fpga,fpag_value,value);
}

void Models::setAudioEnable()
{
    setAFV();
    setAudioOn();
}

void Models::setAFV()
{
//    int flag = 0;

//    int in1 = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_IN1]->third[IN_ENABLE]->current.toInt();
//    int in2 = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_IN2]->third[IN_ENABLE]->current.toInt();
//    int in3 = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_IN3]->third[IN_ENABLE]->current.toInt();
//    int in4 = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_IN4]->third[IN_ENABLE]->current.toInt();
//    int aux = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_AUX]->third[IN_ENABLE]->current.toInt();

//    if(in1 == AUDIO_AFV)
//        flag += 1;
//    if(in2 == AUDIO_AFV)
//        flag += 2;
//    if(in3 == AUDIO_AFV)
//        flag += 4;
//    if(in4 == AUDIO_AFV)
//        flag += 8;
//    if(aux == AUDIO_AFV)
//        flag += 16;

//    settings->setAudioAfvFlag(flag);

//    fpga_write(&g_fpga,FPGA_AUDIO_AFV,flag);
}

void Models::setAudioOn()
{
//    int flag = 0;

//    int mic1 = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_MIC1]->third[MIC_ENABLE]->current.toInt();
//    int mic2 = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_MIC2]->third[MIC_ENABLE]->current.toInt();
//    int in1 = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_IN1]->third[IN_ENABLE]->current.toInt();
//    int in2 = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_IN2]->third[IN_ENABLE]->current.toInt();
//    int in3 = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_IN3]->third[IN_ENABLE]->current.toInt();
//    int in4 = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_IN4]->third[IN_ENABLE]->current.toInt();
//    int aux = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_AUX]->third[IN_ENABLE]->current.toInt();

//    if(in1 == AUDIO_ON)
//        flag += 1;
//    if(in2 == AUDIO_ON)
//        flag += 2;
//    if(in3 == AUDIO_ON)
//        flag += 4;
//    if(in4 == AUDIO_ON)
//        flag += 8;
//    if(aux == AUDIO_ON)
//        flag += 16;
//    if(mic1 == AUDIO_ON)
//        flag += 32;
//    if(mic2 == AUDIO_ON)
//        flag += 64;

//    settings->setAudioOnFlag(flag);

//    fpga_write(&g_fpga,FPGA_AUDIO_ON,flag);
}

void Models::setAudioMonitorLevel()
{
    auto item = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_MONITOR]->third[MONITOR_LEVEL];
//    //发0~50 对应-50~0  所以需要加上50
//    int value = item->current.toInt() + (item->max.toInt() - item->min.toInt());
//    fpga_write(&g_fpga,MON_LEVEL,value);

    double p = item->current.toDouble();
    int value = getMonitorRegValue(p);
    fpga_write(&g_fpga,MON_LEVEL,value);
}

void Models::setAudioMonitorSource()
{
    int index = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_MONITOR]->third[MONITOR_SOURCE]->current.toInt();
    fpga_write(&g_fpga,MON_SOURCE,index);
}

void Models::setDSKSource()
{
//    DSKSource dsk;
//    int fill = settings->listFirst()[MENU_FIRST_DSK]->second[DSK_SOURCE]->third[SOURCE_FILL]->current.toInt();
//    int key = settings->listFirst()[MENU_FIRST_DSK]->second[DSK_SOURCE]->third[SOURCE_KEY]->current.toInt();

//    //屏蔽color1、color2
//    if(key > INPUT_SOURCE_STILL2_KEY)
//        key +=2;

//    dsk.fill = fill;
//    dsk.key = key;

//    fpga_write(&g_fpga,FPGA_DSK_SRC_SEL,*(uint16_t*)&dsk);
}

void Models::setDSKMask(int index)
{
//    int fpga_value = -1;
//    float p;
//    int value;
//    int per;
//    switch (index) {
//    case MASK_H_START:
//        fpga_value = FPGA_DSK_MASK_HSTART;
//        per = settings->listFirst()[MENU_FIRST_DSK]->second[DSK_MASK]->third[MASK_H_START]->current.toInt();
//        p = per / 100.0;
//        p = (MASK_H_MAX - MASK_H_MIN) * p - MASK_H_MAX;
//        value = getMaskPositionH(p);
//        break;
//    case MASK_V_START:
//        fpga_value = FPGA_DSK_MASK_VSTART;
//        per = settings->listFirst()[MENU_FIRST_DSK]->second[DSK_MASK]->third[MASK_V_START]->current.toInt();
//        p = per / 100.0;
//        p = (MASK_V_MAX - MASK_V_MIN) * p - MASK_V_MAX;
//        value = getMaskPositionV(p);
//        break;
//    case MASK_H_END:
//        fpga_value = FPGA_DSK_MASK_HEND;
//        per = settings->listFirst()[MENU_FIRST_DSK]->second[DSK_MASK]->third[MASK_H_END]->current.toInt();
//        p = per / 100.0;
//        p = (MASK_H_MAX - MASK_H_MIN) * p - MASK_H_MAX;
//        value = getMaskPositionH(p);
//        break;
//    case MASK_V_END:
//        fpga_value = FPGA_DSK_MASK_VEND;
//        per = settings->listFirst()[MENU_FIRST_DSK]->second[DSK_MASK]->third[MASK_V_END]->current.toInt();
//        p = per / 100.0;
//        p = (MASK_V_MAX - MASK_V_MIN) * p - MASK_V_MAX;
//        value = getMaskPositionV(p);
//        break;
//    }

//    if(fpga_value != -1)
//        fpga_write(&g_fpga,fpga_value,value);
}

void Models::setDSKRate()
{
    float value = settings->listFirst()[MENU_FIRST_DSK]->second[DSK_RATE]->third[DSK_RATE_RATE]->current.toFloat();
    int outFormat = getOutFormat(settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_FORMAT]->third[SETTING_OUT_FORMAT_FORMAT]->current.toInt());
    int fpga_value = getFTBRateValue(value,outFormat);
    fpga_write(&g_fpga,FPGA_DSK_RATE,fpga_value);
}

void Models::setDSKCtrl()
{
//    int value = 0;
//    int mask = settings->listFirst()[MENU_FIRST_DSK]->second[DSK_MASK]->third[MASK_ENABLE]->current.toInt();
//    int shaped = settings->listFirst()[MENU_FIRST_DSK]->second[DSK_CONTROL]->third[CONTROL_SHAPED_KEY]->current.toInt();
//    int invert = settings->listFirst()[MENU_FIRST_DSK]->second[DSK_CONTROL]->third[CONTROL_INVERT]->current.toInt();

//    if(mask != 0)
//        value += 4;
//    if(shaped != 0)
//        value += 1;
//    if(invert != 0)
//        value += 2;

//    fpga_write(&g_fpga,FPGA_DSK_CTRL,value);
}

void Models::setDSKGain()
{
//    float value = settings->listFirst()[MENU_FIRST_DSK]->second[DSK_CONTROL]->third[CONTROL_GAIN]->current.toFloat();
//    int fpga_value = getKeyGain(value);
//    fpga_write(&g_fpga,FPGA_DSK_GAIN,fpga_value);
}

void Models::setDSKClip()
{
//    float value = settings->listFirst()[MENU_FIRST_DSK]->second[DSK_CONTROL]->third[CONTROL_CLIP]->current.toFloat();
//    int fpga_value = getKeyClip(value);
//    fpga_write(&g_fpga,FPGA_DSK_CLIP,fpga_value);
}

void Models::setLumaKeySource()
{
//    u_int8_t fill_index = settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_SOURCE]->third[LUMA_KEY_SOURCE_FILL]->current.toInt();
//    u_int8_t key_index = settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_SOURCE]->third[LUMA_KEY_SOURCE_KEY]->current.toInt();

//    //屏蔽color1、color2
//    if(key_index > INPUT_SOURCE_STILL2_KEY)
//        key_index +=2;

//    LumaKeySource source = {fill_index,key_index};
//    fpga_write(&g_fpga,LUMA_SRC_SEL,*(u_int16_t*)&source);
}

void Models::setLumaKeyCtrl()
{
//    LumaKeyCtrl ctl;
//    int shapedKeyOn = settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_CONTROL]->third[CONTROL_SHAPED_KEY]->current.toInt();
//    shapedKeyOn == 0?ctl.shaped = 0:ctl.shaped = 1;
//    int InvertOn = settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_CONTROL]->third[CONTROL_INVERT]->current.toInt();
//    InvertOn == 0?ctl.invert = 0:ctl.invert = 1;
//    int maskOn = settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_MASK]->third[MASK_ENABLE]->current.toInt();
//    maskOn == 0?ctl.mask = 0:ctl.mask = 1;
//    int resizeOn = settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_RESIZE]->third[RESIZE_RESIZE]->current.toInt();
//    resizeOn == 0?ctl.resize = 0:ctl.resize = 1;

//    //
//    fpga_write(&g_fpga,LUMA_KEY_CTRL,*(u_int16_t*)&ctl);
}

void Models::setLumaKeyPosition(int index)
{
//    int fpga_value = -1;
//    float p;
//    int value;
//    int per;
//    switch (index) {
//    case MASK_H_START:
//        fpga_value = LUMA_KEY_MASK_HSTART;
//        per = settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_MASK]->third[MASK_H_START]->current.toInt();
//        p = per / 100.0;
//        p = (MASK_H_MAX - MASK_H_MIN) * p - MASK_H_MAX;
//        value = getMaskPositionH(p);
//        break;
//    case MASK_V_START:
//        fpga_value = LUMA_KEY_MASK_VSTART;
//        per = settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_MASK]->third[MASK_V_START]->current.toInt();
//        p = per / 100.0;
//        p = (MASK_V_MAX - MASK_V_MIN) * p - MASK_V_MAX;
//        value = getMaskPositionV(p);
//        break;
//    case MASK_H_END:
//        fpga_value = LUMA_KEY_MASK_HEND;
//        per = settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_MASK]->third[MASK_H_END]->current.toInt();
//        p = per / 100.0;
//        p = (MASK_H_MAX - MASK_H_MIN) * p - MASK_H_MAX;
//        value = getMaskPositionH(p);
//        break;
//    case MASK_V_END:
//        fpga_value = LUMA_KEY_MASK_VEND;
//        per = settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_MASK]->third[MASK_V_END]->current.toInt();
//        p = per / 100.0;
//        p = (MASK_V_MAX - MASK_V_MIN) * p - MASK_V_MAX;
//        value = getMaskPositionV(p);
//        break;
//    }
//    if(fpga_value != -1)
//    {
//        fpga_write(&g_fpga,fpga_value,value);
//    }
}

void Models::setLumaKeyClip()
{
//    float value = settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_CONTROL]->third[CONTROL_CLIP]->current.toFloat();
//    int fpga_value = getKeyClip(value);
//    fpga_write(&g_fpga,LUMA_KEY_CLIP,fpga_value);
}

void Models::setLumaKeyGain()
{
//    float value = settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_CONTROL]->third[CONTROL_GAIN]->current.toFloat();
//    int fpga_value = getKeyGain(value);
//    fpga_write(&g_fpga,LUMA_KEY_GAIN,fpga_value);
}

void Models::setLumaKeyResizeSize()
{
//    int value = settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_RESIZE]->third[RESIZE_SIZE]->current.toInt();
//    int fpga_value;
//    switch (value) {
//    case KEY_SIZE_0_25:
//        fpga_value = 0;
//        break;
//    case KEY_SIZE_0_33:
//        fpga_value = 1;
//        break;
//    case KEY_SIZE_0_50:
//        fpga_value = 2;
//        break;
//    }
//    fpga_write(&g_fpga,LUMA_KEY_SIZE,fpga_value);
}

void Models::setLumaKeyResizePosition(int index)
{
//    int fpga_value = -1;
//    float p;
//    int value;
//    switch (index) {
//    case RESIZE_X_POSITION:
//        fpga_value = LUMA_KEY_H_POS;
//        p = settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_RESIZE]->third[RESIZE_X_POSITION]->current.toFloat();
//        value = getMaskPositionH(p);
//        break;
//    case RESIZE_Y_POSITION:
//        fpga_value = LUMA_KEY_V_POS;
//        p = settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_RESIZE]->third[RESIZE_Y_POSITION]->current.toFloat();
//        value = getMaskPositionV(p);
//        break;
//    }
//    if(fpga_value != -1)
//    {
//        fpga_write(&g_fpga,fpga_value,value);
//    }
}

void Models::setKeyPatternSource()
{
//    u_int8_t fill_index = settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_SOURCE]->third[SOURCE_FILL]->current.toInt();
//    fpga_write(&g_fpga,PATTERN_SRC_SEL,fill_index);
}

void Models::setKeyPatternWipePattern()
{
//    int pattern_index = settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_WIPE]->third[KEY_PATTERN_WIPE_PATTERN]->current.toInt();
//    int pattern_resize = settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_RESIZE]->third[RESIZE_RESIZE]->current.toInt();
//    int pattern_mask = settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_MASK]->third[MASK_ENABLE]->current.toInt();

////    //send settings signal
//    settings->setKeyPatternWipePatternIndex(pattern_index);
//    int value = 0;
//    pattern_resize != 0?value += 0x40:value += 0;
//    pattern_mask != 0?value += 0x20:value += 0;
//    value += pattern_index;
//    fpga_write(&g_fpga,PATTERN_CTRL,value);
}

void Models::setKeyPatternResizeSize()
{
//    int value = settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_RESIZE]->third[RESIZE_SIZE]->current.toInt();
//    int fpga_value;
//    switch (value) {
//    case KEY_SIZE_0_25:
//        fpga_value = 0;
//        break;
//    case KEY_SIZE_0_33:
//        fpga_value = 1;
//        break;
//    case KEY_SIZE_0_50:
//        fpga_value = 2;
//        break;
//    }
//    fpga_write(&g_fpga,PATTERN_SIZE,fpga_value);
}

void Models::setKeyPatternResizePosition(int index)
{
//    int fpga_value = -1;
//    float p;
//    int value;
//    switch (index) {
//    case RESIZE_X_POSITION:
//        fpga_value = PATTERN_H_POS;
//        p = settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_RESIZE]->third[RESIZE_X_POSITION]->current.toFloat();
//        value = getMaskPositionH(p);
//        break;
//    case RESIZE_Y_POSITION:
//        fpga_value = PATTERN_V_POS;
//        p = settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_RESIZE]->third[RESIZE_Y_POSITION]->current.toFloat();
//        value = getMaskPositionV(p);
//        break;
//    }
//    if(fpga_value != -1)
//    {
//        fpga_write(&g_fpga,fpga_value,value);
//    }
}

void Models::setKeyPatternWipePosition()
{
    int pattern_index = settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_WIPE]->third[KEY_PATTERN_WIPE_PATTERN]->current.toInt();
    float posX = settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_WIPE]->third[KEY_PATTERN_WIPE_POS_X]->current.toFloat();
    float posXMax = settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_WIPE]->third[KEY_PATTERN_WIPE_POS_X]->max.toFloat();
    float posXMin = settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_WIPE]->third[KEY_PATTERN_WIPE_POS_X]->min.toFloat();
    posX = (posX - posXMin) / (posXMax - posXMin);

    float posY = settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_WIPE]->third[KEY_PATTERN_WIPE_POS_Y]->current.toFloat();
    float posYMax = settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_WIPE]->third[KEY_PATTERN_WIPE_POS_Y]->max.toFloat();
    float posYMin = settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_WIPE]->third[KEY_PATTERN_WIPE_POS_Y]->min.toFloat();
    posY = (posY - posYMin) / (posYMax - posYMin);

    float symmertry = settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_WIPE]->third[KEY_PATTERN_WIPE_SYMMERTRY]->current.toFloat();
    float symmertryMax = settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_WIPE]->third[KEY_PATTERN_WIPE_SYMMERTRY]->max.toFloat();
    float symmertryMin = settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_WIPE]->third[KEY_PATTERN_WIPE_SYMMERTRY]->min.toFloat();

    symmertry = (symmertry - symmertryMin) / (symmertryMax - symmertryMin);

    qDebug() << "posX:" << posX << "   posY:" << posY << "   symmertry:" << symmertry;

    //抽象对象
    TransitionWipePositionClass *pos;

    switch (pattern_index) {
    case LEFT_TO_RIGHT:
        pos = new TransitionWipePositionLeftToRight(posX,posY,symmertry);
        break;
    case TOP_TO_BOTTOM:
        pos = new TransitionWipePositionTopToBottom(posX,posY,symmertry);
        break;
    case VERTICAL_CENTER:
        pos = new TransitionWipePositionVerticalCenter(posX,posY,symmertry);
        break;
    case HORIZONTAL_CENTER:
        pos = new TransitionWipePositionHorizontalCenter(posX,posY,symmertry);
        break;
    case CROSS_CENTER:
        pos = new TransitionWipePositionCrossCenter(posX,posY,symmertry);
        break;
    case SQUARE_CENTER:
        pos = new TransitionWipePositionSquareCenter(posX,posY,symmertry);
        break;
    case DIAMOND:
        pos = new TransitionWipePositionDiamond(posX,posY,symmertry);
        break;
    case CIRCLE:
        pos = new TransitionWipePositionCircle(posX,posY,symmertry);
        break;
    case SQUARE_TOP_LEFT:
        pos = new TransitionWipePositionSquareTopLeft(posX,posY,symmertry);
        break;
    case SQUARE_TOP_RIGHT:
        pos = new TransitionWipePositionSquareTopRight(posX,posY,symmertry);
        break;
    case SQUARE_BOTTOM_RIGHT:
        pos = new TransitionWipePositionSquareBottomRight(posX,posY,symmertry);
        break;
    case SQUARE_BOTTOM_LEFT:
        pos = new TransitionWipePositionSquareBottomLeft(posX,posY,symmertry);
        break;
    case SQUARE_TOP_CENTER:
        pos = new TransitionWipePositionSquareTopCenter(posX,posY,symmertry);
        break;
    case SQUARE_LEFT_CENTER:
        pos = new TransitionWipePositionSquareLeftCenter(posX,posY,symmertry);
        break;
    case SQUARE_BOTTOM_CENTER:
        pos = new TransitionWipePositionSquareBottomCenter(posX,posY,symmertry);
        break;
    case SQUARE_RIGHT_CENTER:
        pos = new TransitionWipePositionSquareRightCentert(posX,posY,symmertry);
        break;
    case BOTTOM_RIGHT_ANGLE:
        pos = new TransitionWipePositionBottomRightAngle(posX,posY,symmertry);
        break;
    case BOTTOM_LEFT_ANGLE:
        pos = new TransitionWipePositionBottomLeftAngle(posX,posY,symmertry);
        break;
    }

    pos->checkout();
    fpga_write(&g_fpga,PATTERN_POSX,pos->reg_h_pos);
    fpga_write(&g_fpga,PATTERN_POSY,pos->reg_v_pos);
    fpga_write(&g_fpga,PATTERN_H_PARAM,pos->reg_h_param);
    fpga_write(&g_fpga,PATTERN_V_PARAM,pos->reg_v_param);

    delete pos;
}

void Models::setKeyPatternWipeSoftness()
{
    float softness = settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_WIPE]->third[KEY_PATTERN_WIPE_SOFTNESS]->current.toFloat();
    float softnessMax = settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_WIPE]->third[KEY_PATTERN_WIPE_SOFTNESS]->max.toFloat();;
    float softnessMin = settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_WIPE]->third[KEY_PATTERN_WIPE_SOFTNESS]->min.toFloat();;

    softness = (softness - softnessMin) / (softnessMax - softnessMin);
    int value = getWipeSoftness(softness);
    fpga_write(&g_fpga,PATTERN_SOFTNESS,value);
}

void Models::setKeyPatternWipeSize()
{
#define PATTERN_WIPE_SIZE_PERCENT 2.55
    int size = settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_WIPE]->third[KEY_PATTERN_WIPE_SIZE]->current.toInt();
    size = size * PATTERN_WIPE_SIZE_PERCENT;
    fpga_write(&g_fpga,PATTERN_WIPE_SIZE,size);
}

void Models::setKeyPatternMask(int index)
{
//    int fpga_value = -1;
//    float p;
//    int value;
//    int per;
//    switch (index) {
//    case MASK_H_START:
//        fpga_value = PATTERN_MASK_HSTART;
//        per = settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_MASK]->third[MASK_H_START]->current.toInt();
//        p = per / 100.0;
//        p = (MASK_H_MAX - MASK_H_MIN) * p - MASK_H_MAX;
//        value = getMaskPositionH(p);
//        break;
//    case MASK_V_START:
//        fpga_value = PATTERN_MASK_VSTART;
//        per = settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_MASK]->third[MASK_V_START]->current.toInt();
//        p = per / 100.0;
//        p = (MASK_V_MAX - MASK_V_MIN) * p - MASK_V_MAX;
//        value = getMaskPositionV(p);
//        break;
//    case MASK_H_END:
//        fpga_value = PATTERN_MASK_HEND;
//        per = settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_MASK]->third[MASK_H_END]->current.toInt();
//        p = per / 100.0;
//        p = (MASK_H_MAX - MASK_H_MIN) * p - MASK_H_MAX;
//        value = getMaskPositionH(p);
//        break;
//    case MASK_V_END:
//        fpga_value = PATTERN_MASK_VEND;
//        per = settings->listFirst()[MENU_FIRST_KEY_PATTERN]->second[PATTERN_MASK]->third[MASK_V_END]->current.toInt();
//        p = per / 100.0;
//        p = (MASK_V_MAX - MASK_V_MIN) * p - MASK_V_MAX;
//        value = getMaskPositionV(p);
//        break;
//    }

//    if(fpga_value != -1)
//        fpga_write(&g_fpga,fpga_value,value);
}

void Models::setChromaKeySource()
{
    int fill_index = settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_SOURCE]->third[CHROMA_KEY_SOURCE_FILL]->current.toInt();
    fpga_write(&g_fpga,CHROMA_SRC_SEL,fill_index);
}

void Models::setChromaKeyCtrl()
{
//    int enable = settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_RESIZE]->third[RESIZE_RESIZE]->current.toInt();
//    int mask = settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_MASK]->third[MASK_ENABLE]->current.toInt();
//    int smp = settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_CONTROL]->third[CHROMA_KEY_CONTROL_SMP_ENABLE]->current.toInt();
//    int value = 0;
//    enable > 0?value += 16:value += 0;
//    mask > 0?value += 2:value += 0;
//    smp > 0?value += 4:value +=0;
//    int G = settings->color5Data()[1];
//    int B = settings->color5Data()[2];
//    G > B ?value += 1:value +=0;
//    fpga_write(&g_fpga,CHROMA_CTRL,value);
}

void Models::setChromaKeyCtrlSample()
{
//    int enable = settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_RESIZE]->third[RESIZE_RESIZE]->current.toInt();
//    int mask = settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_MASK]->third[MASK_ENABLE]->current.toInt();
//    int smp = settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_CONTROL]->third[CHROMA_KEY_CONTROL_SMP_ENABLE]->current.toInt();
//    int value = 0;
//    enable > 0?value += 16:value += 0;
//    mask > 0?value += 2:value += 0;
//    smp > 0?value += 4:value +=0;

//    static QTimer *timer = nullptr;
//    if(timer == nullptr)
//    {
//        timer = new QTimer();
//        connect(timer,&QTimer::timeout,this,[=](){
//            setChromaKeySampleColor();
//        });
//    }
//    if(smp > 0)
//    {
//        timer->start(50);
//    }
//    else
//    {
//        timer->stop();
//        //set data
//        setChromaKeyProfile();
//        //save color
//        settings->saveSMPColorAndColor5ToFile();
//    }

//    int G = settings->color5Data()[1];
//    int B = settings->color5Data()[2];
//    G > B ?value += 1:value +=0;
//    fpga_write(&g_fpga,CHROMA_CTRL,value);
}

void Models::setChromaKeyMask(int index)
{
//    int fpga_value = -1;
//    float p;
//    int value;
//    int per;
//    switch (index) {
//    case MASK_H_START:
//        fpga_value = CHROMA_MASK_HSTART;
//        per = settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_MASK]->third[MASK_H_START]->current.toInt();
//        p = per / 100.0;
//        p = (MASK_H_MAX - MASK_H_MIN) * p - MASK_H_MAX;
//        value = getMaskPositionH(p);
//        break;
//    case MASK_V_START:
//        fpga_value = CHROMA_MASK_VSTART;
//        per = settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_MASK]->third[MASK_V_START]->current.toInt();
//        p = per / 100.0;
//        p = (MASK_V_MAX - MASK_V_MIN) * p - MASK_V_MAX;
//        value = getMaskPositionV(p);
//        break;
//    case MASK_H_END:
//        fpga_value = CHROMA_MASK_HEND;
//        per = settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_MASK]->third[MASK_H_END]->current.toInt();
//        p = per / 100.0;
//        p = (MASK_H_MAX - MASK_H_MIN) * p - MASK_H_MAX;
//        value = getMaskPositionH(p);
//        break;
//    case MASK_V_END:
//        fpga_value = CHROMA_MASK_VEND;
//        per = settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_MASK]->third[MASK_V_END]->current.toInt();
//        p = per / 100.0;
//        p = (MASK_V_MAX - MASK_V_MIN) * p - MASK_V_MAX;
//        value = getMaskPositionV(p);
//        break;
//    }

//    if(fpga_value != -1)
//        fpga_write(&g_fpga,fpga_value,value);
}

void Models::setChromaKeyResizeSize()
{
//    int value = settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_RESIZE]->third[RESIZE_SIZE]->current.toInt();
//    int fpga_value;
//    switch (value) {
//    case KEY_SIZE_0_25:
//        fpga_value = 0;
//        break;
//    case KEY_SIZE_0_33:
//        fpga_value = 1;
//        break;
//    case KEY_SIZE_0_50:
//        fpga_value = 2;
//        break;
//    }
//    fpga_write(&g_fpga,CHROMA_SIZE,fpga_value);
}

void Models::setChromaKeyResizePosition(int index)
{
//    int fpga_value = -1;
//    float p;
//    int value;
//    switch (index) {
//    case RESIZE_X_POSITION:
//        fpga_value = CHROMA_H_POS;
//        p = settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_RESIZE]->third[RESIZE_X_POSITION]->current.toFloat();
//        value = getMaskPositionH(p);
//        break;
//    case RESIZE_Y_POSITION:
//        fpga_value = CHROMA_V_POS;
//        p = settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_RESIZE]->third[RESIZE_Y_POSITION]->current.toFloat();
//        value = getMaskPositionV(p);
//        break;
//    }
//    if(fpga_value != -1)
//    {
//        fpga_write(&g_fpga,fpga_value,value);
//    }
}

void Models::setChromaKeyProfile()
{
//    float foreground = settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_CONTROL]->third[CHROMA_KEY_CONTROL_Foreground]->current.toInt() / 100.0;
//    float background = settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_CONTROL]->third[CHROMA_KEY_CONTROL_Background]->current.toInt() / 100.0;
//    float keyedge = settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_CONTROL]->third[CHROMA_KEY_CONTROL_KeyEdge]->current.toInt() / 100.0;

//    float bgclear = 2.1;
//    float blkgloss = 1;
//    float green_suppression_ratio = 1.5;
//    float background_clean = foreground * 1.5 + 1;
//    float key_edge = 60;

//    int R = settings->color5Data()[0];
//    int G = settings->color5Data()[1];
//    int B = settings->color5Data()[2];
//    int Y = settings->chromakeySMPColorYcbcr()[0];
//    int cb = settings->chromakeySMPColorYcbcr()[1];
//    int cr = settings->chromakeySMPColorYcbcr()[2];

//    int enable = settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_RESIZE]->third[RESIZE_RESIZE]->current.toInt();
//    int mask = settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_MASK]->third[MASK_ENABLE]->current.toInt();
//    int smp = settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_CONTROL]->third[CHROMA_KEY_CONTROL_SMP_ENABLE]->current.toInt();
//    int value = 0;
//    enable > 0?value += 16:value +=0;
//    mask > 0?value += 2:value += 0;
//    smp > 0?value += 4:value +=0;
//    G > B ?value += 1:value +=0;
////    fpga_write(&g_fpga,CHROMA_CTRL,value);

//    qDebug() << "bgclear:" << bgclear;
//    qDebug() << "blkgloss:" << blkgloss;
//    qDebug() << "green_suppression_ratio:" << green_suppression_ratio;
//    qDebug() << "background_clean:" << background_clean;
//    qDebug() << "key_edge:" << key_edge;
//    qDebug() << "foreground:" << foreground;
//    qDebug() << "background:" << background;
//    qDebug() << "keyedge:" << keyedge;

//    auto chromaKeyData = getChromaKeyData(Y,cb,cr,bgclear,blkgloss,green_suppression_ratio,background_clean,key_edge,background,keyedge);
//    chromaKeyData.print();

//    fpga_write(&g_fpga,CHROMA_YS,chromaKeyData.ck_ys);
//    fpga_write(&g_fpga,CHROMA_K1,chromaKeyData.k1);
//    fpga_write(&g_fpga,CHROMA_K2,chromaKeyData.k2);
//    fpga_write(&g_fpga,CHROMA_K3,chromaKeyData.k3);
//    fpga_write(&g_fpga,CHROMA_K4,chromaKeyData.k4);
//    fpga_write(&g_fpga,CHROMA_K5,chromaKeyData.k5);
//    fpga_write(&g_fpga,CHROMA_K6,chromaKeyData.k6);
//    fpga_write(&g_fpga,CHROMA_K7,chromaKeyData.k7);
//    fpga_write(&g_fpga,CHROMA_K10,chromaKeyData.k10);
//    fpga_write(&g_fpga,CHROMA_K11,chromaKeyData.k11);
//    fpga_write(&g_fpga,CHROMA_K12,chromaKeyData.k12);
//    fpga_write(&g_fpga,ADDR_MATTE_GAIN,chromaKeyData.gain);
//    fpga_write(&g_fpga,ADDR_MATTE_CLIP,chromaKeyData.clip);

//    value += 8;
//    fpga_write(&g_fpga,CHROMA_CTRL,value);
////    usleep(50000);
////    QTimer::singleShot(50,this,[=](){
////        fpga_write(&g_fpga,CHROMA_CTRL,value - 8);
////    });
//    fpga_write(&g_fpga,CHROMA_CTRL,value - 8);
}

void Models::setChromaKeySMPPosition()
{
    float x = settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_CONTROL]->third[CHROMA_KEY_CONTROL_SMP_X_POSITION]->current.toFloat();
    float y = settings->listFirst()[MENU_FIRST_CHROMA_KEY]->second[CHROMA_KEY_CONTROL]->third[CHROMA_KEY_CONTROL_SMP_Y_POSITION]->current.toFloat();

    int x_value = getMaskPositionH(x);
    int y_value = getMaskPositionV(y);

    fpga_write(&g_fpga,CHROMA_SMP_X_POS,x_value);
    fpga_write(&g_fpga,CHROMA_SMP_Y_POS,y_value);
}

void Models::setChromaKeySampleColor()
{
    uint16_t Y,C;
    fpga_read(&g_fpga,CHROMA_SMP_Y,&Y);
    fpga_read(&g_fpga,CHROMA_SMP_C,&C);
    int R,G,B;
    int cb = C >> 8;
    int cr = C & 0xFF;
    QList<int> ycbcr;
    ycbcr << Y << cb << cr;
    settings->setChromakeySMPColorYcbcr(ycbcr);
    ycbcr2Rgb(Y,cb,cr,&R,&G,&B);
    QList<int>data;
    data << R << G << B;
    settings->setColor5Data(data);
}

void Models::setPipSource()
{
    int index = settings->listFirst()[MENU_FIRST_PIP]->second[PIP_SOURCE]->third[PIP_SOURCE_FILL]->current.toInt();
    fpga_write(&g_fpga,PIP_SRC_SEL,index);
}

void Models::setPipSize()
{
    int value = settings->listFirst()[MENU_FIRST_PIP]->second[PIP_SIZE_POSITION]->third[PIP_SIZE_SIZE]->current.toInt();
    int fpga_value;
    switch (value) {
    case KEY_SIZE_0_25:
        fpga_value = 0;
        break;
    case KEY_SIZE_0_33:
        fpga_value = 1;
        break;
    case KEY_SIZE_0_50:
        fpga_value = 2;
        break;
    }
    fpga_write(&g_fpga,PIP_SIZE,fpga_value);
}

void Models::setPipCtrl()
{
//    int value = 0;
//    int mask = settings->listFirst()[MENU_FIRST_PIP]->second[PIP_MASK]->third[MASK_ENABLE]->current.toInt();
//    int board = settings->listFirst()[MENU_FIRST_PIP]->second[PIP_BOARD]->third[PIP_BOARD_ENABLE]->current.toInt();
//    mask > 0 ?value += 1:value += 0;
//    board > 0 ?value += 2:value += 0;

//    fpga_write(&g_fpga,PIP_CTRL,value);
}

void Models::setPipPosition(int index)
{
    int fpga_value = -1;
    float p;
    int value;
    switch (index) {
    case PIP_SIZE_X_POSITION:
        fpga_value = PIP_H_POS;
        p = settings->listFirst()[MENU_FIRST_PIP]->second[PIP_SIZE_POSITION]->third[PIP_SIZE_X_POSITION]->current.toFloat();
        value = getMaskPositionH(p);
        break;
    case PIP_SIZE_Y_POSITION:
        fpga_value = PIP_V_POS;
        p = settings->listFirst()[MENU_FIRST_PIP]->second[PIP_SIZE_POSITION]->third[PIP_SIZE_Y_POSITION]->current.toFloat();
        value = getMaskPositionV(p);
        break;
    }
    if(fpga_value != -1)
    {
        fpga_write(&g_fpga,fpga_value,value);
    }
}

void Models::setPipMask(int index)
{
//    int fpga_value = -1;
//    float p;
//    int value;
//    int per;
//    switch (index) {
//    case MASK_H_START:
//        fpga_value = PIP_MASK_HSTART;
//        per = settings->listFirst()[MENU_FIRST_PIP]->second[PIP_MASK]->third[MASK_H_START]->current.toInt();
//        p = per / 100.0;
//        p = (MASK_H_MAX - MASK_H_MIN) * p - MASK_H_MAX;
//        value = getMaskPositionH(p);
//        break;
//    case MASK_V_START:
//        fpga_value = PIP_MASK_VSTART;
//        per = settings->listFirst()[MENU_FIRST_PIP]->second[PIP_MASK]->third[MASK_V_START]->current.toInt();
//        p = per / 100.0;
//        p = (MASK_V_MAX - MASK_V_MIN) * p - MASK_V_MAX;
//        value = getMaskPositionV(p);
//        break;
//    case MASK_H_END:
//        fpga_value = PIP_MASK_HEND;
//        per = settings->listFirst()[MENU_FIRST_PIP]->second[PIP_MASK]->third[MASK_H_END]->current.toInt();
//        p = per / 100.0;
//        p = (MASK_H_MAX - MASK_H_MIN) * p - MASK_H_MAX;
//        value = getMaskPositionH(p);
//        break;
//    case MASK_V_END:
//        fpga_value = PIP_MASK_VEND;
//        per = settings->listFirst()[MENU_FIRST_PIP]->second[PIP_MASK]->third[MASK_V_END]->current.toInt();
//        p = per / 100.0;
//        p = (MASK_V_MAX - MASK_V_MIN) * p - MASK_V_MAX;
//        value = getMaskPositionV(p);
//        break;
//    }

//    if(fpga_value != -1)
//        fpga_write(&g_fpga,fpga_value,value);
}

void Models::setPipBoard()
{
    int value = settings->listFirst()[MENU_FIRST_PIP]->second[PIP_BOARD]->third[PIP_BORDER_WIDTH]->current.toInt();

    fpga_write(&g_fpga,FPGA_PIP_BOARD,value);
}

void Models::setPipColor(int index)
{
//    int Hue = settings->listFirst()[MENU_FIRST_PIP]->second[PIP_COLOR]->third[COLOR_HUE]->current.toInt();
//    int Saturation = settings->listFirst()[MENU_FIRST_PIP]->second[PIP_COLOR]->third[COLOR_SATURATION]->current.toInt();
//    int Brightness = settings->listFirst()[MENU_FIRST_PIP]->second[PIP_COLOR]->third[COLOR_BRIGHTNESS]->current.toInt();

//    QList<int> data;
//    data << Hue << Saturation << Brightness;
//    settings->setColor4Data(data);

//    auto color = getColorYC(Hue,Saturation,Brightness);

//    fpga_write(&g_fpga,PIP_COLOR_Y,color.Y);
//    fpga_write(&g_fpga,PIP_COLOR_C,*(uint16_t *)&(color.Cr));
}

void Models::setFtbRate()
{
    float value = settings->listFirst()[MENU_FIRST_FTB]->second[MENU_SECOND_FTB_RATE]->third[FTB_RATE_RATE]->current.toFloat();
    int outFormat = getOutFormat(settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_FORMAT]->third[SETTING_OUT_FORMAT_FORMAT]->current.toInt());
    int fpga_value = getFTBRateValue(value,outFormat);
    fpga_write(&g_fpga,FTB_RATE,fpga_value);
}

void Models::setFtbAFV()
{
    int value = settings->listFirst()[MENU_FIRST_FTB]->second[MENU_SECOND_FTB_AUDIO]->third[FTB_AUDIO_AFV]->current.toInt();
    fpga_write(&g_fpga,FTB_AFV,value);
}

void Models::setMixRate()
{
    float value = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_MIX]->third[TRANSITION_MIX_RATE]->current.toFloat();
    int outFormat = getOutFormat(settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_FORMAT]->third[SETTING_OUT_FORMAT_FORMAT]->current.toInt());
    int fpga_value = getFTBRateValue(value,outFormat);
    fpga_write(&g_fpga,MIX_RATE,fpga_value);
}

void Models::setDipSource()
{
    int value = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_DIP]->third[TRANSITION_DIP_SOURCE]->current.toInt();
    fpga_write(&g_fpga,DIP_SRC_SEL,value);
}

void Models::setDipRate()
{
    float value = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_DIP]->third[TRANSITION_DIP_RATE]->current.toFloat();
    int outFormat = getOutFormat(settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_FORMAT]->third[SETTING_OUT_FORMAT_FORMAT]->current.toInt());
    int fpga_value = getFTBRateValue(value,outFormat);
    fpga_write(&g_fpga,DIP_RATE,fpga_value);
}

void Models::setTransitionWipeRate()
{
    float value = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_RATE]->current.toFloat();
    int outFormat = getOutFormat(settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_FORMAT]->third[SETTING_OUT_FORMAT_FORMAT]->current.toInt());
    int fpga_value = getFTBRateValue(value,outFormat);
    fpga_write(&g_fpga,WIPE_RATE,fpga_value);
}

void Models::setTransitionWipePattern()
{
    int derection = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_DIRECTION]->current.toInt();
    int pattern_index = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_PATTERN]->current.toInt();

    //send settings signal
    settings->setTransitionWipePatternIndex(pattern_index);
    int value = 0;
    derection != 0?value += 0x20:value += 0;
    value += pattern_index;
    fpga_write(&g_fpga,WIPE_PATTERN,value);
}

void Models::setTransitionWipePosition()
{
    int pattern_index = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_PATTERN]->current.toInt();
    float posX = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_POSITIONX]->current.toFloat();
    float posXMax = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_POSITIONX]->max.toFloat();
    float posXMin = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_POSITIONX]->min.toFloat();
    posX = (posX - posXMin) / (posXMax - posXMin);

    float posY = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_POSITIONY]->current.toFloat();
    float posYMax = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_POSITIONX]->max.toFloat();
    float posYMin = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_POSITIONX]->min.toFloat();
    posY = (posY - posYMin) / (posYMax - posYMin);

    float symmertry = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_SYMMERTRY]->current.toFloat();
    float symmertryMax = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_SYMMERTRY]->max.toFloat();
    float symmertryMin = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_SYMMERTRY]->min.toFloat();

    symmertry = (symmertry - symmertryMin) / (symmertryMax - symmertryMin);

//    float symmertryCircle = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_SYMMERTRY_CIRCLE]->current.toFloat();
//    float symmertryCircleMax = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_SYMMERTRY_CIRCLE]->max.toFloat();
//    float symmertryCircleMin = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_SYMMERTRY_CIRCLE]->min.toFloat();

//    symmertryCircle = (symmertryCircle - symmertryCircleMin) / (symmertryCircleMax - symmertryCircleMin);

    //抽象对象
    TransitionWipePositionClass *pos;

    switch (pattern_index) {
    case LEFT_TO_RIGHT:
        pos = new TransitionWipePositionLeftToRight(posX,posY,symmertry);
        break;
    case TOP_TO_BOTTOM:
        pos = new TransitionWipePositionTopToBottom(posX,posY,symmertry);
        break;
    case VERTICAL_CENTER:
        pos = new TransitionWipePositionVerticalCenter(posX,posY,symmertry);
        break;
    case HORIZONTAL_CENTER:
        pos = new TransitionWipePositionHorizontalCenter(posX,posY,symmertry);
        break;
    case CROSS_CENTER:
        pos = new TransitionWipePositionCrossCenter(posX,posY,symmertry);
        break;
    case SQUARE_CENTER:
        pos = new TransitionWipePositionSquareCenter(posX,posY,symmertry);
        break;
    case DIAMOND:
        pos = new TransitionWipePositionDiamond(posX,posY,symmertry);
        break;
    case CIRCLE:
        pos = new TransitionWipePositionCircle(posX,posY,symmertry);//symmertryCircle
        break;
    case SQUARE_TOP_LEFT:
        pos = new TransitionWipePositionSquareTopLeft(posX,posY,symmertry);
        break;
    case SQUARE_TOP_RIGHT:
        pos = new TransitionWipePositionSquareTopRight(posX,posY,symmertry);
        break;
    case SQUARE_BOTTOM_RIGHT:
        pos = new TransitionWipePositionSquareBottomRight(posX,posY,symmertry);
        break;
    case SQUARE_BOTTOM_LEFT:
        pos = new TransitionWipePositionSquareBottomLeft(posX,posY,symmertry);
        break;
    case SQUARE_TOP_CENTER:
        pos = new TransitionWipePositionSquareTopCenter(posX,posY,symmertry);
        break;
    case SQUARE_LEFT_CENTER:
        pos = new TransitionWipePositionSquareLeftCenter(posX,posY,symmertry);
        break;
    case SQUARE_BOTTOM_CENTER:
        pos = new TransitionWipePositionSquareBottomCenter(posX,posY,symmertry);
        break;
    case SQUARE_RIGHT_CENTER:
        pos = new TransitionWipePositionSquareRightCentert(posX,posY,symmertry);
        break;
    case BOTTOM_RIGHT_ANGLE:
        pos = new TransitionWipePositionBottomRightAngle(posX,posY,symmertry);
        break;
    case BOTTOM_LEFT_ANGLE:
        pos = new TransitionWipePositionBottomLeftAngle(posX,posY,symmertry);
        break;
    }

    pos->checkout();
    fpga_write(&g_fpga,WIPE_POSX,pos->reg_h_pos);
    fpga_write(&g_fpga,WIPE_POSY,pos->reg_v_pos);
    fpga_write(&g_fpga,WIPE_H_PARAM,pos->reg_h_param);
    fpga_write(&g_fpga,WIPE_V_PARAM,pos->reg_v_param);

    delete pos;
}

void Models::setTransitionWipeSoftness()
{
    float softness = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_SOFTNESS]->current.toFloat();
    float softnessMax = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_SOFTNESS]->max.toFloat();;
    float softnessMin = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_SOFTNESS]->min.toFloat();;

    softness = (softness - softnessMin) / (softnessMax - softnessMin);
    int value = getWipeSoftness(softness);
    fpga_write(&g_fpga,WIPE_SOFTNESS,value);
}

void Models::setTransitionWipeBoard()
{
    float softness = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_SOFTNESS]->current.toFloat();
    float softnessMax = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_SOFTNESS]->max.toFloat();;
    float softnessMin = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_SOFTNESS]->min.toFloat();;

    softness = (softness - softnessMin) / (softnessMax - softnessMin);
    int value = getWipeSoftness(softness);

    float board = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_BOARD]->current.toFloat();
    float boardMax = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_BOARD]->max.toFloat();;
    float boardMin = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_BOARD]->min.toFloat();;

    board = (board - boardMin) / (boardMax - boardMin);
    value = (int)(value * board + 0.5);
    fpga_write(&g_fpga,WIPE_BOARD,value);
}

void Models::setTransitionWipeFillSource()
{
    int index = settings->listFirst()[MENU_FIRST_TRANSITION]->second[TRANSITION_WIPE]->third[TRANSITION_WIPE_FILL_SOURCE]->current.toInt();
    fpga_write(&g_fpga,WIPE_BOARD_SRC_SEL,index);
}

void Models::setStillSelection()
{
    int value;
    int still1 = settings->listFirst()[MENU_FIRST_STILL_GENERATOR]->second[STILL_GENERATE_SELECTION]->third[STILL_SELECTION_STILL1]->current.toInt();
    int still2 = settings->listFirst()[MENU_FIRST_STILL_GENERATOR]->second[STILL_GENERATE_SELECTION]->third[STILL_SELECTION_STILL2]->current.toInt();
    int location_index = settings->listFirst()[MENU_FIRST_STILL_GENERATOR]->second[STILL_GENERATE_UPLOAD]->third[STILL_UPLOAD_LOCATION]->current.toInt();
    location_index = location_index << 10;
    still2 = still2 << 5;
    value = still1 + still2 + location_index;
    fpga_write(&g_fpga,STILL_SEL,value);
}

void Models::setStillUpload()
{
    QString name = settings->listFirst()[MENU_FIRST_STILL_GENERATOR]->second[STILL_GENERATE_UPLOAD]->third[STILL_UPLOAD_LOAD_PICTURE]->getText();

    if(name == "")
        return ;

    int location_index = settings->listFirst()[MENU_FIRST_STILL_GENERATOR]->second[STILL_GENERATE_UPLOAD]->third[STILL_UPLOAD_LOCATION]->current.toInt();
    int still1_index = settings->listFirst()[MENU_FIRST_STILL_GENERATOR]->second[STILL_GENERATE_SELECTION]->third[STILL_SELECTION_STILL1]->current.toInt();
    int still2_index = settings->listFirst()[MENU_FIRST_STILL_GENERATOR]->second[STILL_GENERATE_SELECTION]->third[STILL_SELECTION_STILL2]->current.toInt();
    still2_index = still2_index << 5;

    //设置加载文件名
    myProvider->setStillImageFileName(SD_IMAGE_PATH + name);
    //1.设置配置文件中location 位置对应文件名
    settings->setStillImages(location_index,name);
    //2.copy SD卡文件到userdata
    QString cmd = "cp " SD_IMAGE_PATH + name + " " STILLPATH + name;
    std::string s_cmd = cmd.toStdString();
    system(s_cmd.data());

    location_index = location_index << 10;

    QtConcurrent::run([=]() {
        //set still1 index = location index
//        fpga_write(&g_fpga,STILL_SEL,still2_index + location_index);//still2 5-9  still1  0-4

        //set upload
        fpga_write(&g_fpga,STILL_SEL,still1_index + still2_index + location_index);//still2 5-9  still1  0-4

        //开始上传
        fpga_write(&g_fpga,STILL_UPLOAD,1 << 2);//100
        //上传fill
        fpga_write(&g_fpga,STILL_UPLOAD,(1 << 2) + (1 << 1));//110
        emit loadStillImage(1,"fill");
        usleep(1000000);
        //上传fill 结束
//        fpga_write(&g_fpga,STILL_UPLOAD,1 << 2);//100
//        emit loadStillImage(0,"fill");
//        usleep(50000);
        //上传key
        fpga_write(&g_fpga,STILL_UPLOAD,(1 << 2) + 1);//101
        emit loadStillImage(1,"key");
        usleep(1500000);
        fpga_write(&g_fpga,STILL_UPLOAD,1 << 2);//100
        fpga_write(&g_fpga,STILL_SEL,still2_index + still1_index);
        emit loadStillImage(0,"key");
        usleep(1000000);
        //上传key 结束
        fpga_write(&g_fpga,STILL_UPLOAD,0);//000
    });
}

void Models::initStillUpload()
{
    QMutex mutex;
    QString fileName;
    int still1_index = settings->listFirst()[MENU_FIRST_STILL_GENERATOR]->second[STILL_GENERATE_SELECTION]->third[STILL_SELECTION_STILL1]->current.toInt();
    int still2_index = settings->listFirst()[MENU_FIRST_STILL_GENERATOR]->second[STILL_GENERATE_SELECTION]->third[STILL_SELECTION_STILL2]->current.toInt();
    still2_index = still2_index << 5;
    for(int i = 0;i < STILLMAX;++i)
    {
        fileName = settings->stillImages()[i];
        if(fileName != "")
        {
            initStillMutex = 1;
            QtConcurrent::run([=]() {
                myProvider->setStillImageFileName(STILLPATH + fileName);
                qDebug() << "i:" << i << "   STILLPATH + fileName:" << STILLPATH + fileName;
                //set still1 index = location index
                fpga_write(&g_fpga,STILL_SEL,still2_index + i);
                //开始上传
                fpga_write(&g_fpga,STILL_UPLOAD,1 << 2);
                //上传fill
                fpga_write(&g_fpga,STILL_UPLOAD,(1 << 2) + (1 << 1));
                emit loadStillImage(1,"fill");
                usleep(1000000);
//                上传fill 结束
//                fpga_write(&g_fpga,STILL_UPLOAD,1 << 2);
//                emit loadStillImage(0,"fill");
//                usleep(500000);
                //上传key
                fpga_write(&g_fpga,STILL_UPLOAD,(1 << 2) + 1);
                emit loadStillImage(1,"key");
                usleep(1500000);

                //set still1 index = still1 index
                fpga_write(&g_fpga,STILL_SEL,still2_index + still1_index);
                fpga_write(&g_fpga,STILL_UPLOAD,1 << 2);//100
                emit loadStillImage(0,"key");

                usleep(1000000);
                //上传key 结束
                fpga_write(&g_fpga,STILL_UPLOAD,0);


                initStillMutex = 0;
            });
            while(initStillMutex){
                usleep(2000);
            }
        }
    }
}

void Models::initStillUpLoadRvSwitch()
{
    const char *png_path[STILLMAX] = {0};
    std::string list[STILLMAX];
    for(int i = 0;i < STILLMAX;++i)
    {
        QString fileName = settings->stillImages()[i];
        fileName = fileName.toLocal8Bit();
        if(fileName != "")
        {
            list[i] = STILLPATH + fileName.toStdString();
            png_path[i] = list[i].c_str();
        }
    }
    rv_switch_still_upload(png_path,STILLMAX);
}

#define LOADSLEEP 100
void Models::loadStill1()
{
    qDebug() << "loadStill1";
    settings->setLoadStillProgress(2);

    QString name = settings->listFirst()[MENU_FIRST_STILL_GENERATOR]->second[STILL_GENERATE_UPLOAD]->third[STILL_UPLOAD_LOAD_PICTURE]->getText();

    if(name == "")
        return ;

    int location_index = settings->listFirst()[MENU_FIRST_STILL_GENERATOR]->second[STILL_GENERATE_UPLOAD]->third[STILL_UPLOAD_LOCATION]->current.toInt();

    QImage image(SD_IMAGE_PATH + name);
    if(image.width() == 1920 && image.height() == 1080)
    {

        //2.copy SD卡文件到userdata
        QString cmd = "cp \'" SD_IMAGE_PATH + name + "\'  \'" STILLPATH + name + "\'";
        qDebug() << "_____-cmd:" << cmd;
        std::string s_cmd = cmd.toStdString();
        system(s_cmd.data());
        system("sync");

        //设置加载文件名
        myProvider->setStillImageFileName(SD_IMAGE_PATH + name);
        //1.设置配置文件中location 位置对应文件名
        settings->setStillImages(location_index,name);

        fpga_write(&g_fpga,STILL_UPLOAD,(1 << 2) + (1 << 1));//110
        emit loadStillImage(1,"fill");
    }
    else
    {
        QTimer::singleShot(10,this,[=](){
            QImage image1 = image.scaled(1920,1080);
            image1.save(STILLPATH + name);
            system("sync");
            //设置加载文件名
            myProvider->setStillImageFileName(STILLPATH + name);
            //1.设置配置文件中location 位置对应文件名
            settings->setStillImages(location_index,name);
            fpga_write(&g_fpga,STILL_UPLOAD,(1 << 2) + (1 << 1));//110
            emit loadStillImage(1,"fill");
        });
    }

    //开始上传
//    fpga_write(&g_fpga,STILL_UPLOAD,1 << 2);//100
    //上传fill
//    fpga_write(&g_fpga,STILL_UPLOAD,(1 << 2) + (1 << 1));//110
//    emit loadStillImage(1,"fill");
}

void Models::loadStill2()
{
    qDebug() << "loadStill2";
    settings->setLoadStillProgress(3);
    //上传fill 结束
//        fpga_write(&g_fpga,STILL_UPLOAD,1 << 2);//100
//        emit loadStillImage(0,"fill");
//        usleep(50000);
    //上传key

    QTimer::singleShot(LOADSLEEP,this,[=](){
        fpga_write(&g_fpga,STILL_UPLOAD,(1 << 2) + 1);//101
        emit loadStillImage(1,"key");
    });

}

void Models::loadStill3()
{
    qDebug() << "loadStill3";
    settings->setLoadStillProgress(4);

    QTimer::singleShot(LOADSLEEP,this,[=](){
        fpga_write(&g_fpga,STILL_UPLOAD,1 << 2);//100
        emit loadStillImage(0,"key");
    });
}

void Models::loadStill4()
{
    qDebug() << "loadStill4";

    QTimer::singleShot(LOADSLEEP,this,[=](){
        fpga_write(&g_fpga,STILL_UPLOAD,0);//000
    });

    settings->setLoadStillProgress(1);
}

int Models::playStart()
{
    if(settings->reallyAuxSourceIndex() != AUX_SOURCE_SD_CARD)
        return -1;
    if(!media_sd->is_online())
        return -1;

    if((settings->playListCurrent() >= settings->playList().size() && settings->playList().size() != 0) || settings->playList().size() == 0)
        return -1;

    QString play = SD_VIDEO_PATH + settings->playList()[settings->playListCurrent()];

    if(play == settings->recordFileName())
        return -1;
    //
    play = play.toLocal8Bit();

    std::string s_play = play.toStdString();
    rv_switch_mp4_open((char*)(s_play.data()),(rk_switch_cb)(playCallback),0);
    return 0;
}

void Models::playPause(int index)
{
    rv_switch_mp4_pause(index,(rk_switch_cb)(playCallback),0);
}

void Models::playStop()
{
    qDebug() << "<<<<<<<<<<<playStop";
    rv_switch_mp4_close((rk_switch_cb)(playCallback),0);
}

void Models::setPlay(int status)
{
    if(settings->reallyAuxSourceIndex() != AUX_SOURCE_SD_CARD)
        return ;
    if(!media_sd->is_online())
        return ;

    if(status)
    {
        if(settings->playLedStatus() == E_STATUS_MP4_CLOSE)
            return ;
        playPause(1);
    }
    else
    {
        if(settings->playLedStatus() == E_STATUS_MP4_CLOSE){
            if(!playStart()){
                playPause(0);
            }
        }else if(settings->playLedStatus() == E_STATUS_MP4_PAUSE){
            playPause(0);
        }
    }
}

void Models::recordStart()
{
    if(!media_sd->is_online())
        return ;
    int second = models->sd_remaintime_calc();
    if(second < 1)
        return ;

    if(settings->recordLedStatus() == E_STATUE_SUCCESS)
        return ;

    if(settings->recordFileName() != "")
        return ;

    QString fileName = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_RECORD_FILE]->third[SETTING_RECORD_FILE_NAME]->current.toString();
    fileName = SD_VIDEO_PATH + fileName + "_" + QString::number(QDateTime::currentMSecsSinceEpoch()) + ".mp4";
    settings->setRecordFileName(fileName);
    std::string s_fileName = fileName.toStdString();
    rv_switch_record_start0((char*)(s_fileName.data()),(rk_switch_cb)(recordStartCallback),0);
}

void Models::recordStop()
{
    if(settings->recordFileName() == "")
        return ;

    if(settings->recordLedStatus() == E_STATUS_FAILED)
        return ;

    static bool recordMinFlag = false;
    if(!recordMinFlag && settings->recordSecond() < 2){
        recordMinFlag = true;
        QtConcurrent::run([=](){
            while(1){
                if(settings->recordSecond() >= 2 || !media_sd->is_online()){
                    recordMinFlag = false;
                    settings->setRecordFileName("");
                    rv_switch_record_stop0((rk_switch_cb)(recordStopCallback),0);
                    break;
                }
                usleep(100);
            }
        });
        return ;
    }
    if(recordMinFlag)
        return ;

    settings->setRecordFileName("");
    rv_switch_record_stop0((rk_switch_cb)(recordStopCallback),0);
}

void Models::setRecord(int flag)
{
    if(flag)
    {
        recordStart();
    }
    else
    {
        recordStop();
    }
}

void Models::setPlayNext()
{
    qDebug() << "setPlayNext()";

    if(settings->playListCurrent() >= settings->playList().size() - 1)
        return ;

    if(SD_VIDEO_PATH + settings->playList()[settings->playListCurrent() + 1] == settings->recordFileName())
        return ;

    settings->setPlayListCurrent(settings->playListCurrent() + 1);
    int playback_mode = settings->listFirst()[MENU_FIRST_PLAYBACK]->second[PLAYBACK_PLAYBACK]->third[MENU_THIRD_PLAYBACK_MODE]->current.toInt();
    if(playback_mode == SEQUENTIAL && settings->playLedStatus() != E_STATUS_MP4_CLOSE){
        settings->setPlayAutoNextFlag(1);
        playStop();
    }
    else
        playStart();
}

void Models::setPlayPrevious()
{
    qDebug() << "setPlayPrevious()";

    if(settings->playListCurrent() == 0)
        return ;

    if(SD_VIDEO_PATH + settings->playList()[settings->playListCurrent() - 1] == settings->recordFileName())
        return ;

    settings->setPlayListCurrent(settings->playListCurrent() - 1);
    int playback_mode = settings->listFirst()[MENU_FIRST_PLAYBACK]->second[PLAYBACK_PLAYBACK]->third[MENU_THIRD_PLAYBACK_MODE]->current.toInt();
    if(playback_mode == SEQUENTIAL && settings->playLedStatus() != E_STATUS_MP4_CLOSE){
        settings->setPlayAutoNextFlag(1);
        playStop();
    }
    else
        playStart();
}

//弃用
void Models::autoPlayNext()
{
    if(settings->reallyAuxSourceIndex() != AUX_SOURCE_SD_CARD)
        return ;
    if(!media_sd->is_online())
        return ;

    if(settings->playListCurrent() >= settings->playList().size() - 1)
        return ;

    if(SD_VIDEO_PATH + settings->playList()[settings->playListCurrent() + 1] == settings->recordFileName())
        return ;

    settings->setPlayListCurrent(settings->playListCurrent() + 1);
    playStart();
}

void Models::setLiveStatus(int status)
{
    settings->setLiveStatus(status);
}

void Models::openAllStream()
{
    STREAM_PROFILE bitrate1,bitrate2,bitrate3;
    QString rtmp1,rtmp2,rtmp3;
    std::string s_rtmp1,s_rtmp2,s_rtmp3;
    bool enable1,enable2,enable3;
    bitrate1 = settings->getStreamBitrateIndex(STREAM_STREAM1);
    bitrate2 = settings->getStreamBitrateIndex(STREAM_STREAM2);
    bitrate3 = settings->getStreamBitrateIndex(STREAM_STREAM3);
    rtmp1 = settings->getStreamUrlIndex(STREAM_STREAM1);
    s_rtmp1 = rtmp1.toStdString();
    rtmp2 = settings->getStreamUrlIndex(STREAM_STREAM2);
    s_rtmp2 = rtmp2.toStdString();
    rtmp3 = settings->getStreamUrlIndex(STREAM_STREAM3);
    s_rtmp3 = rtmp3.toStdString();
    enable1 = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_OUTPUT]->current.toInt() == OUTPUT_ENABLE;
    enable2 = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM2]->third[MENU_THIRD_STREAM_OUTPUT]->current.toInt() == OUTPUT_ENABLE;
    enable3 = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM3]->third[MENU_THIRD_STREAM_OUTPUT]->current.toInt() == OUTPUT_ENABLE;
    if(enable1)
    {
        qDebug() << "rtmp1:" << rtmp1 << "  bitrate:" << bitrate1.bitrate << "  audio_bitrate:"<< bitrate1.audio_bitrate;
        rv_switch_push_start0((char*)(s_rtmp1.data()),(rk_switch_cb)(pushCallBack1),0);
    }
    if(enable2)
    {
        qDebug() << "rtmp2:" << rtmp2 << "  bitrate:" << bitrate2.bitrate << "  audio_bitrate:"<< bitrate2.audio_bitrate;
        rv_switch_push_start1((char*)(s_rtmp2.data()),(rk_switch_cb)(pushCallBack2),0);
    }
    if(enable3)
    {
        qDebug() << "rtmp3:" << rtmp3 << "  bitrate:" << bitrate3.bitrate << "  audio_bitrate:"<< bitrate3.audio_bitrate;
        rv_switch_push_start2((char*)(s_rtmp3.data()),(rk_switch_cb)(pushCallBack3),0);
    }
}

void Models::closeAllStream()
{
    bool enable1,enable2,enable3;
    enable1 = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_OUTPUT]->current.toInt() == OUTPUT_ENABLE;
    enable2 = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM2]->third[MENU_THIRD_STREAM_OUTPUT]->current.toInt() == OUTPUT_ENABLE;
    enable3 = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM3]->third[MENU_THIRD_STREAM_OUTPUT]->current.toInt() == OUTPUT_ENABLE;
    if(enable1)
    {
        rv_switch_push_stop0(pushCallBack1,0);
    }
    if(enable2)
    {
        rv_switch_push_stop1(pushCallBack2,0);
    }
    if(enable3)
    {
        rv_switch_push_stop2(pushCallBack3,0);
    }
}

void Models::setStreamOutput()
{
    int output1 = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_OUTPUT]->current.toInt();
    int output2 = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM2]->third[MENU_THIRD_STREAM_OUTPUT]->current.toInt();
    int output3 = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM3]->third[MENU_THIRD_STREAM_OUTPUT]->current.toInt();

    auto list = settings->streamOutputList();
    list.clear();
    list.push_back(output1);
    list.push_back(output2);
    list.push_back(output3);

    settings->setStreamOutputList(list);

//    if(output1 == 0 && output2 == 0 && output3 == 0)
//    {
//        if(settings->liveStatus() != 0)
//        {
//            settings->setLiveStatus(0);
//        }
    //    }
}

void Models::setSrcName(int third)
{
    QString text = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_SRC_NAME]->third[third]->getText();
    std::string s_text = text.toUtf8().toStdString();
    auto name = s_text.data();
    struct umd_config_t cfg = {
        .font = FONT_PATH,
        .font_size = 20,

        .italic = 0,
        .bold = 0,
        .align = ALIGN_CENTER,

        .width = 160,
        .height = 40,
    };

    if(third > SRC_NAME_PVW){
        cfg.width = 128;
        cfg.height = 30;
        cfg.font_size = 16;
    }

    int fpga_add = 0;
    switch (third) {
    case SRC_NAME_PGM:
        fpga_add = UMD_PGM;
        break;
    case SRC_NAME_PVW:
        fpga_add = UMD_PVW;
        break;
    case SRC_NAME_IN1:
        fpga_add = UMD_IN1;
        break;
    case SRC_NAME_IN2:
        fpga_add = UMD_IN2;
        break;
    case SRC_NAME_IN3:
        fpga_add = UMD_IN3;
        break;
    case SRC_NAME_IN4:
        fpga_add = UMD_IN4;
        break;
    case SRC_NAME_AUX:
        fpga_add = UMD_AUX;
        break;
    case SRC_NAME_STILL1:
        fpga_add = UMD_STILL1;
        break;
    case SRC_NAME_STILL2:
        fpga_add = UMD_STILL2;
        break;
    default:
        return ;
    }
    str2umd(name,strlen(name),&cfg);
    fpga_write_buffer(&g_fpga,fpga_add,(uint8_t*)cfg.buffer,cfg.bufsize);
    free(cfg.buffer);
}

void Models::setPlaybackMode()
{
    int mode = settings->listFirst()[MENU_FIRST_PLAYBACK]->second[PLAYBACK_PLAYBACK]->third[MENU_THIRD_PLAYBACK_MODE]->current.toInt();
    if(mode == PLAY_ONCE){
        rv_switch_mp4_replay(0);
    }else if(mode == REPEAT){
        rv_switch_mp4_replay(1);
    }else if(mode == SEQUENTIAL){
        rv_switch_mp4_replay(0);
    }
}

void Models::setAuxSource()
{
    int index = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_AUX_SOURCE]->third[SETTING_AUX_SOURCE_SOURCE]->current.toInt();

    if(index == AUX_SOURCE_NDI){
        if(get_ndi_license_state()){
            messageDialogControl->dialogShow(QObject::tr("No NDI license, please purchase license at www.osee-tech.com."),{QObject::tr("Cancel")},MessageDialogControl::MESSAGE_SD_FORMAT);
            settings->setMenuValue(MENU_FIRST_SETTING,SETTING_AUX_SOURCE,SETTING_AUX_SOURCE_SOURCE,settings->reallyAuxSourceIndex());
            return ;
        }
    }
    //set really aux source
    settings->setReallyAuxSourceIndex(index);
    switch (index) {
    case AUX_SOURCE_SD_CARD:
        rv_switch_usb_camera_stop0(0,0);
        osee_ndi_deinit();
        if(settings->playList().size() != 0 && settings->playLedStatus() == E_STATUS_MP4_CLOSE){
            playPause(1);
            playStart();
        }
        break;
    case AUX_SOURCE_USB_CAMERA:
        playStop();
        osee_ndi_deinit();
        rv_switch_usb_camera_start0(0,0);
        break;
    case AUX_SOURCE_NDI:
        playStop();
        rv_switch_usb_camera_stop0(0,0);
        if(ndi->lastConnectItem().url() != "")
            ndi->connectNdi(ndi->lastConnectItem());
        break;
    }
}

void Models::setSrcSelection(int third)
{
    int value = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_AUX_SOURCE]->third[third]->current.toInt();
    if(third == SETTING_AUX_SOURCE_IN1)
        settings->setReallySrcSelectionIn1(value);
    else if(third == SETTING_AUX_SOURCE_IN2)
        settings->setReallySrcSelectionIn2(value);
    else if(third == SETTING_AUX_SOURCE_IN3)
        settings->setReallySrcSelectionIn3(value);
    else if(third == SETTING_AUX_SOURCE_IN4)
        settings->setReallySrcSelectionIn4(value);
    set_hdmi_in_colorspace(third,(hdmi_in_colorspace_t)value);
}

void Models::setMvMeter()
{
    int pgm = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_MV_METER]->third[MV_METER_PGM]->current.toInt();
    int in1 = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_MV_METER]->third[MV_METER_IN1]->current.toInt();
    int in2 = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_MV_METER]->third[MV_METER_IN2]->current.toInt();
    int in3 = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_MV_METER]->third[MV_METER_IN3]->current.toInt();
    int in4 = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_MV_METER]->third[MV_METER_IN4]->current.toInt();
    int aux = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_MV_METER]->third[MV_METER_AUX]->current.toInt();

    int value = 0;
    in1 == MENU_ON?value += 1:value += 0;
    in2 == MENU_ON?value += 2:value += 0;
    in3 == MENU_ON?value += 4:value += 0;
    in4 == MENU_ON?value += 8:value += 0;
    aux == MENU_ON?value += 16:value += 0;
    pgm == MENU_ON?value += 32:value += 0;

    fpga_write(&g_fpga,AUDIO_MET_DISP,value);
}

void Models::setMvLayout()
{

}

void Models::setMarket()
{
    int pvw_pos = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_MV_LAYOUT]->third[MV_LAYOUT_SWAP]->current.toInt();
    int market = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_MARKER]->third[MARKER_PVW]->current.toInt();

    int value = 0;
    pvw_pos == MV_LAYOUT_SWAP_PVW_PGM?value += 2:value += 0;
    market == MENU_ON?value += 1:value += 0;

    fpga_write(&g_fpga,PVW_CTRL,value);
}

void Models::setMicInput(int third)
{
    int index = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_MIC_INPUT]->third[third]->current.toInt();
    if(third == MENU_THIRD_MIC_INPUT_MIC_1_INPUT)
    {
        rv_switch_set_mic0(index);
    }
    else if(third == MENU_THIRD_MIC_INPUT_MIC_2_INPUT)
    {
        rv_switch_set_mic1(index);
    }
}

int Models::getOutFormatIndexForEnum(int index)
{
    int value = 0;
    switch (index) {
    case OUT_FORMAT_1080P24:
        value = 0x0;
        break;
    case OUT_FORMAT_1080P25:
        value = 0x1;
        break;
    case OUT_FORMAT_1080P30:
        value = 0x2;
        break;
    case OUT_FORMAT_1080P50:
        value = 0x3;
        break;
    case OUT_FORMAT_1080P60:
        value = 0x4;
        break;
    }
    if(index < OUT_FORMAT_1080P24)
        value = 0x0;
    if(index > OUT_FORMAT_1080P60)
        value = 0x4;

    return value;
}

int Models::getOutFormat(int index)
{
    int value = 30;
    switch (index) {
    case OUT_FORMAT_1080P24:
        value = 24;
        break;
    case OUT_FORMAT_1080P25:
        value = 25;
        break;
    case OUT_FORMAT_1080P30:
        value = 30;
        break;
    case OUT_FORMAT_1080P50:
        value = 50;
        break;
    case OUT_FORMAT_1080P60:
        value = 60;
        break;
    }
    if(index < OUT_FORMAT_1080P24)
        value = 24;
    if(index > OUT_FORMAT_1080P60)
        value = 60;

    return value;
}

void Models::setOutFormat()
{
    int index = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_FORMAT]->third[SETTING_OUT_FORMAT_FORMAT]->current.toInt();

    settings->setReallyOutFormat(index);

    int fpga_value = getOutFormatIndexForEnum(index);

    //开机初始化不执行
    if(!init_settings_is_ok)
    {
        return ;
    }

    //set qt resolution
    rv_switch_resolutionset(fpga_value);
}

void Models::setOutSource()
{
    int hdmi1 = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_SOURCE]->third[SETTING_OUT_SOURCE_HDMI1]->current.toInt();
//    int hdmi2 = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_SOURCE]->third[SETTING_OUT_SOURCE_HDMI2]->current.toInt();
    //hdmi2 = mutilView
    int hdmi2 = 7;
    int uvc = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_SOURCE]->third[SETTING_OUT_SOURCE_AUX]->current.toInt();

    settings->setReallyOutSourceHDMI1(hdmi1);
    settings->setReallyOutSourceHDMI2(hdmi2);
    settings->setReallyOutSourceUVC(uvc);

    OutSource source;
    source.hdmi1 = hdmi1;
    source.hdmi2 = hdmi2;
    source.uvc = uvc;

    qDebug() << "hdmi1:" << hdmi1 << "hdmi2:" << hdmi2 << "uvc:" << uvc;

    fpga_write(&g_fpga,OUT_SEL,*(uint16_t*)&source);
}

void Models::setColorSpace(int third)
{
    int index = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_FORMAT]->third[third]->current.toInt();
    if(third == SETTING_OUT_FORMAT_OUTPUT1_COLOR_SPACE)
        settings->setReallyOutput1ColorSpace(index);
    else if(third == SETTING_OUT_FORMAT_OUTPUT2_COLOR_SPACE)
        settings->setReallyOutput2ColorSpace(index);

    set_hdmi_out_colorspace(third - SETTING_OUT_FORMAT_OUTPUT1_COLOR_SPACE,(hdmi_out_colorspace_t)index);
}

void Models::setQuality(int third)
{
    STREAM_PROFILE profile;
    if(third == QUALITY_RECORDING)
    {
        profile = settings->getRecordBitrate();
        rv_switch_set_record_bitrate(profile.bitrate);
    }
    else if(third == QUALITY_STREAMING)
    {
        profile = settings->getStreamBitrateIndex(0);
        rv_switch_set_push_bitrate(profile.bitrate);
    }

    int second = sd_remaintime_calc();
    media_sd->checkGbFree(second);

    rv_switch_set_aac_bitrate(profile.audio_bitrate);
}

void Models::setProtocol()
{
    int index = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_PROTOCOL]->current.toInt();
    set_ipaddr(ETHERNET_NETIF, "0.0.0.0");
    if(index == NETWORK_PROTOCOL_DHCP)
    {
        set_static_interface(ETHERNET_NETIF,
                             NULL,
                             NULL,
                             NULL,
                             NULL,
                             NULL);
        settings->setMenuDHCPNetwork("","","","","");
    }
    else if(index == NETWORK_PROTOCOL_STATIC_IP)
    {
        QString ipAddress = settings->getIniValue(MENU_FIRST_SETTING,SETTING_NETWORK,NETWORK_IP_ADDRESS).toString();
        std::string s_ipAddress = ipAddress.toStdString();

        QString mask = settings->getIniValue(MENU_FIRST_SETTING,SETTING_NETWORK,NETWORK_SUBNET_MASK).toString();
        std::string s_mask = mask.toStdString();

        QString gateway = settings->getIniValue(MENU_FIRST_SETTING,SETTING_NETWORK,NETWORK_GATEWAY).toString();
        std::string s_gateway = gateway.toStdString();

        QString dns1 = settings->getIniValue(MENU_FIRST_SETTING,SETTING_NETWORK,NETWORK_PRIMARY_DNS).toString();
        std::string s_dns1 = dns1.toStdString();

        QString dns2 = settings->getIniValue(MENU_FIRST_SETTING,SETTING_NETWORK,NETWORK_SECONDAY_DNS).toString();
        std::string s_dns2 = dns2.toStdString();
        set_static_interface(ETHERNET_NETIF,
                            ipAddress != "" ?s_ipAddress.data() : "0.0.0.0",
                            s_mask.data(),
                            s_gateway.data(),
                            s_dns1.data(),
                            s_dns2.data());
        settings->setMenuDHCPNetwork(ipAddress,mask,gateway,dns1,dns2);
        set_ipaddr(ETHERNET_NETIF, ipAddress != "" ?s_ipAddress.data() : "0.0.0.0");
        set_netmask(ETHERNET_NETIF, s_mask.data());
        set_gateway(ETHERNET_NETIF, s_gateway.data());
    }
    system("/etc/init.d/S41dhcpcd restart > /dev/null 2>&1 &");
}



void Models::setNetwork(int third)
{
    int index = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_PROTOCOL]->current.toInt();
    if(index == NETWORK_PROTOCOL_DHCP)
        return ;

    qDebug() << "setNetwork:" << third;
    QString dns1 = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_PRIMARY_DNS]->current.toString();
    QString dns2 = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_SECONDAY_DNS]->current.toString();

    QString ipAddress = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_IP_ADDRESS]->current.toString();
    std::string s_ipAddress = ipAddress.toStdString();

    QString mask = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_SUBNET_MASK]->current.toString();
    std::string s_mask = mask.toStdString();

    QString gateway = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_GATEWAY]->current.toString();
    std::string s_gateway = gateway.toStdString();

    qDebug() << "dns1:" << dns1;
    qDebug() << "dns2:" << dns2;
    qDebug() << "ipAddress:" << ipAddress;
    qDebug() << "mask:" << mask;
    qDebug() << "gateway:" << gateway;

    std::string s_dns1 = dns1.toStdString();
    std::string s_dns2 = dns2.toStdString();

    set_static_interface(ETHERNET_NETIF,
                         ipAddress != "" ? s_ipAddress.data() : "0.0.0.0",
                         s_mask.data(),
                         s_gateway.data(),
                         s_dns1.data(),
                         s_dns2.data());

    set_ipaddr(ETHERNET_NETIF, ipAddress != "" ?s_ipAddress.data() : "0.0.0.0");
    set_netmask(ETHERNET_NETIF, s_mask.data());
    set_gateway(ETHERNET_NETIF, s_gateway.data());

    if(third == NETWORK_PRIMARY_DNS || third == NETWORK_SECONDAY_DNS)
        system("/etc/init.d/S41dhcpcd restart > /dev/null 2>&1 &");
}

void Models::setMSleep(int value)
{
    qDebug() << "setSleep:" <<  value;
//    QThread::msleep(value);
}

void Models::setUserWait()
{

}

void Models::setPlatform(int second)
{
    QString platform = settings->listFirst()[MENU_FIRST_STREAM]->second[second]->third[MENU_THIRD_STREAM_PLATFORM]->getText();
    qDebug() << "platform:" << platform;

    auto servers = settings->streamData[platform].name_url;
    auto third = &settings->listFirst()[MENU_FIRST_STREAM]->second[second]->third[MENU_THIRD_STREAM_SERVER];
    (*third)->list_text.clear();
    for(auto it = servers.begin();it != servers.end(); it++)
    {
        (*third)->list_text << it->first;
    }
    (*third)->max = (*third)->list_text.size() - 1;
    int current = settings->getIniValue(MENU_FIRST_STREAM,second,MENU_THIRD_STREAM_SERVER).toInt();
    if(current > (*third)->max.toInt())
        current = 0;

    settings->setMenuValue(MENU_FIRST_STREAM,second,MENU_THIRD_STREAM_SERVER,current);
}

void Models::setStreamKey(int second)
{

}

void Models::setLoadStreamKey(int second)
{
    QString fileName = settings->listFirst()[MENU_FIRST_STREAM]->second[second]->third[MENU_THIRD_STREAM_UPLOAD_KEY]->getText();
    fileName = SD_STREAM_KEY_PATH + fileName;
    qDebug() << fileName;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "_______setLoadStreamKey  open  file  fail!!!!!";
        return ;
    }
    //从第一个不是空格、回车的字符开始，遇到空格回车结束
    QTextStream in(&file);
    char c;
    QByteArray array;
    bool start = false;
    while(!in.atEnd())
    {
        in >> c;
        qDebug() << "c:" <<c;
        if(c == ' ' || c == '\t' || c == '\n' || c == '\r')
        {
            if(start)
                break;
        }
        else
        {
            if(!start)
                start = !start;
            if(start)
            {
                array.append(c);
            }
        }
    }
    QString text = array;
    qDebug() << text;
    settings->setMenuValue(MENU_FIRST_STREAM,second,MENU_THIRD_STREAM_KEY,text);
    file.close();
}

void Models::setButtonBrightness()
{
    int value = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_PANEL]->third[MENU_THIRD_PANEL_BUTTON_BRIGHTNESS]->current.toInt();
    QSwitcher::set_leds_brightness(value);
}

void Models::setPgmIndex(int index)
{
    qDebug() << "setPgmIndex:" << index;
//    QSwitcher::set_softkey(KEY_LED_PGM_1 + index,1);
    keyClick(KEY_LED_PGM_1 + index);
}

void Models::setPvwIndex(int index)
{
    qDebug() << "setPvwIndex:" << index;
//    QSwitcher::set_softkey(KEY_LED_PVW_1 + index,1);
    keyClick(KEY_LED_PVW_1 + index);
}

void Models::setFtb()
{
    qDebug() << "setFtb:";
//    QSwitcher::set_softkey(KEY_LED_TRANS_FTB,1);
    keyClick(KEY_LED_TRANS_FTB);
}

void Models::setCutTransition()
{
    qDebug() << "setCutTransition:";
//    QSwitcher::set_softkey(KEY_LED_TRANS_CUT,1);
    keyClick(KEY_LED_TRANS_CUT);
}

void Models::setAutoTransition()
{
    qDebug() << "setAutoTransition:";
//    QSwitcher::set_softkey(KEY_LED_TRANS_AUTO,1);
    keyClick(KEY_LED_TRANS_AUTO);
}

void Models::setPrev(int status)
{
    if(status == SWITCHER_LED_OFF && QSwitcher::get_led(KEY_LED_TRANS_PREVIEW) != SWITCHER_LED_OFF){
//        QSwitcher::set_softkey(KEY_LED_TRANS_PREVIEW,1);
        keyClick(KEY_LED_TRANS_PREVIEW);
        qDebug() << "setPrev!!!" ;
    }else if(status != SWITCHER_LED_OFF && QSwitcher::get_led(KEY_LED_TRANS_PREVIEW) == SWITCHER_LED_OFF){
//        QSwitcher::set_softkey(KEY_LED_TRANS_PREVIEW,1);
        keyClick(KEY_LED_TRANS_PREVIEW);
        qDebug() << "setPrev!!!" ;
    }
}

void Models::setTransitionIndex(int index)
{
    qDebug() << "setTransitionIndex:" << index;
}

void Models::setTransitionPosition(int value)
{
    qDebug() << "setTransitionPosition:" << value;
    fpga_write(&g_fpga, FPGA_T_BAR, value);
}

void Models::setTransitionSource(int source)
{
    if(source & 0x4){
        //bkgd open
        if(QSwitcher::get_led(KEY_LED_BKGD) == SWITCHER_LED_OFF){
//            QSwitcher::set_softkey(KEY_LED_BKGD,1);
            keyClick(KEY_LED_BKGD);
            qDebug() << "bkgd open";
        }
    }else{
        //bkgd close
        if(QSwitcher::get_led(KEY_LED_BKGD) != SWITCHER_LED_OFF){
//            QSwitcher::set_softkey(KEY_LED_BKGD,1);
            keyClick(KEY_LED_BKGD);
            qDebug() << "bkgd close";
        }
    }
    if(source & 0x2){
        //dsk open
        if(QSwitcher::get_led(KEY_LED_DSK) == SWITCHER_LED_OFF){
//            QSwitcher::set_softkey(KEY_LED_DSK,1);
            keyClick(KEY_LED_DSK);
            qDebug() << "dsk open";
        }
    }else{
        //dsk close
        if(QSwitcher::get_led(KEY_LED_DSK) != SWITCHER_LED_OFF){
//            QSwitcher::set_softkey(KEY_LED_DSK,1);
            keyClick(KEY_LED_DSK);
            qDebug() << "dsk close";
        }
    }
    if(source & 0x1){
        //key open
        if(QSwitcher::get_led(KEY_LED_KEY) == SWITCHER_LED_OFF){
//            QSwitcher::set_softkey(KEY_LED_KEY,1);
            keyClick(KEY_LED_KEY);
            qDebug() << "key open";
        }
    }else{
        //key close
        if(QSwitcher::get_led(KEY_LED_KEY) != SWITCHER_LED_OFF){
//            QSwitcher::set_softkey(KEY_LED_KEY,1);
            keyClick(KEY_LED_KEY);
            qDebug() << "key close";
        }
    }
}

void Models::setKeyOnAir(int status)
{
    if(status == SWITCHER_LED_OFF && QSwitcher::get_led(KEY_LED_KEY_ON_AIR) != SWITCHER_LED_OFF){
//        QSwitcher::set_softkey(KEY_LED_KEY_ON_AIR,1);
        keyClick(KEY_LED_KEY_ON_AIR);
        qDebug() << "setKeyOnAir!!!" ;
    }else if(status != SWITCHER_LED_OFF && QSwitcher::get_led(KEY_LED_KEY_ON_AIR) == SWITCHER_LED_OFF){
//        QSwitcher::set_softkey(KEY_LED_KEY_ON_AIR,1);
        keyClick(KEY_LED_KEY_ON_AIR);
        qDebug() << "setKeyOnAir!!!" ;
    }
}

void Models::setDskOnAir(int status)
{
    if(status == SWITCHER_LED_OFF && QSwitcher::get_led(KEY_LED_DSK_ON_AIR) != SWITCHER_LED_OFF){
//        QSwitcher::set_softkey(KEY_LED_DSK_ON_AIR,1);
        keyClick(KEY_LED_DSK_ON_AIR);
        qDebug() << "setDskOnAir!!!" ;
    }else if(status != SWITCHER_LED_OFF && QSwitcher::get_led(KEY_LED_DSK_ON_AIR) == SWITCHER_LED_OFF){
//        QSwitcher::set_softkey(KEY_LED_DSK_ON_AIR,1);
        keyClick(KEY_LED_DSK_ON_AIR);
        qDebug() << "setDskOnAir!!!" ;
    }
}

int Models::sd_remaintime_calc()
{
    media_sd->current_usage();
    auto really_free = _getfree_recordreally(media_sd->bfree,settings->recordSecond());
    STREAM_PROFILE profile = settings->getRecordBitrate();
    int rate = profile.bitrate / 8;
    int second = really_free / rate;
    return second;
}

void Models::sd_format()
{
    //
#define ReTryMax 4
    messageDialogControl->setProcessPercent(0);
    emit qthread_uevent.emit_sd_change(false);
    QtConcurrent::run([=](){
        usleep(500000);
        int ret;
        for(int i = 0;i < ReTryMax;++i){
            ret = media_sd->format_exfat(0);
            qDebug() << "format_exfat(0): " << ret;
            usleep(500000);
            if(ret == 0)
                break;
        }
        if(ret != 0){
            messageDialogControl->dialogShow(tr("Action has been unsuccessful!"),{tr("Cancel"),tr("Try Again")},MessageDialogControl::MESSAGE_SD_FORMAT);
            return ;
        }
        messageDialogControl->setProcessPercent(10);

        for(int i = 0;i < ReTryMax;++i){
            ret = media_sd->format_exfat(1);
            qDebug() << "format_exfat(1): " << ret;
            usleep(500000);
            if(ret == 0)
                break;
        }
        if(ret != 0){
            messageDialogControl->dialogShow(tr("Action has been unsuccessful!"),{tr("Cancel"),tr("Try Again")},MessageDialogControl::MESSAGE_SD_FORMAT);
            return ;
        }
        messageDialogControl->setProcessPercent(20);

        for(int i = 0;i < ReTryMax;++i){
            ret = media_sd->format_exfat(2);
            qDebug() << "format_exfat(2): " << ret;
            usleep(500000);
            if(ret == 0)
                break;
        }
        if(ret != 0){
            messageDialogControl->dialogShow(tr("Action has been unsuccessful!"),{tr("Cancel"),tr("Try Again")},MessageDialogControl::MESSAGE_SD_FORMAT);
            return ;
        }
        messageDialogControl->setProcessPercent(90);

        for(int i = 0;i < ReTryMax;++i){
            ret = media_sd->format_exfat(3);
            qDebug() << "format_exfat(3): " << ret;
            usleep(500000);
            if(ret == 0)
                break;
        }
        if(ret != 0){
            messageDialogControl->dialogShow(tr("Action has been unsuccessful!"),{tr("Cancel"),tr("Try Again")},MessageDialogControl::MESSAGE_SD_FORMAT);
            return ;
        }
        messageDialogControl->setProcessPercent(100);

        emit qthread_uevent.emit_sd_change(true);
    });


    //QTimer function
    //    int static process_index = 0;

    //    switch (process_index) {
    //    case 0:{
    //        messageDialogControl->setProcessPercent(0);
    //        int ret;
    //        ret = media_sd->format_exfat(0);
    //        qDebug() << "format_exfat(0): " << ret;
    //        if(ret != 0){
    //            messageDialogControl->dialogShow("Action has been unsuccessful!",{"Cancel","Try Again"},MessageDialogControl::MESSAGE_SD_FORMAT);
    //            process_index = 0;
    //            return ;
    //        }
    //        emit qthread_uevent.emit_sd_change(false);
    //        process_index++;
    //        messageDialogControl->setProcessPercent(10);
    //    }
    //        break;
    //    case 1:{
    //        int ret;
    //        ret = media_sd->format_exfat(1);
    //        qDebug() << "format_exfat(1): " << ret;
    //        if(ret != 0){
    //            messageDialogControl->dialogShow("Action has been unsuccessful!",{"Cancel","Try Again"},MessageDialogControl::MESSAGE_SD_FORMAT);
    //            process_index = 0;
    //            return ;
    //        }
    //        process_index++;
    //        messageDialogControl->setProcessPercent(20);
    //    }
    //        break;
    //    case 2:{
    //        int ret;
    //        ret = media_sd->format_exfat(2);
    //        qDebug() << "format_exfat(2): " << ret;
    //        if(ret != 0){
    //            messageDialogControl->dialogShow("Action has been unsuccessful!",{"Cancel","Try Again"},MessageDialogControl::MESSAGE_SD_FORMAT);
    //            process_index = 0;
    //            return ;
    //        }
    //        process_index++;
    //        messageDialogControl->setProcessPercent(90);
    //    }
    //        break;
    //    case 3:{
    //        int ret;
    //        ret = media_sd->format_exfat(3);
    //        qDebug() << "format_exfat(3): " << ret;
    //        if(ret != 0){
    //            messageDialogControl->dialogShow("Action has been unsuccessful!",{"Cancel","Try Again"},MessageDialogControl::MESSAGE_SD_FORMAT);
    //            process_index = 0;
    //            return ;
    //        }
    //        process_index = 0;
    //        messageDialogControl->setProcessPercent(100);
    //        emit qthread_uevent.emit_sd_change(true);
    //        return ;
    //    }
    //        break;
    //    default:
    //        break;
    //    }

    //    QTimer::singleShot(500,this,[=](){
    //         sd_format();
    //    });

}

void Models::macroImport()
{

}

void Models::macroExport()
{

}

void Models::setColorBackHue(int colorIndex, int hue)
{
    ColorBack *m_colorBack = nullptr;
    if(colorIndex == ColorBacks::COLOR1)
    {
        m_colorBack = profile->colorBacks()->colorBack1();
    }
    else if(colorIndex == ColorBacks::COLOR2)
    {
        m_colorBack = profile->colorBacks()->colorBack2();
    }
    if(m_colorBack != nullptr)
    {
        if(m_colorBack->hue() != hue)
        {
            m_colorBack->setHue(hue);
            return ;
        }
        setColorBack(colorIndex);
    }
}

void Models::setColorBackSaturation(int colorIndex, int saturation)
{
    ColorBack *m_colorBack = nullptr;
    if(colorIndex == ColorBacks::COLOR1)
    {
        m_colorBack = profile->colorBacks()->colorBack1();
    }
    else if(colorIndex == ColorBacks::COLOR2)
    {
        m_colorBack = profile->colorBacks()->colorBack2();
    }
    if(m_colorBack != nullptr)
    {
        if(m_colorBack->saturation() != saturation)
        {
            m_colorBack->setSaturation(saturation);
            return ;
        }
        setColorBack(colorIndex);
    }
}

void Models::setColorBackBrightness(int colorIndex, int brightness)
{

    ColorBack *m_colorBack = nullptr;
    if(colorIndex == ColorBacks::COLOR1)
    {
        m_colorBack = profile->colorBacks()->colorBack1();
    }
    else if(colorIndex == ColorBacks::COLOR2)
    {
        m_colorBack = profile->colorBacks()->colorBack2();
    }
    if(m_colorBack != nullptr)
    {
        if(m_colorBack->brightness() != brightness)
        {
            m_colorBack->setBrightness(brightness);
            return ;
        }
        setColorBack(colorIndex);
    }
}

void Models::setColorBack(int colorIndex)
{
    int hue = 0;
    int saturation = 0;
    int brightness = 0;
    int fpga_value_y = -1;
    int fpga_value_c = -1;
    if(colorIndex == ColorBacks::COLOR1){
        hue = profile->colorBacks()->colorBack1()->hue();
        saturation = profile->colorBacks()->colorBack1()->saturation();
        brightness = profile->colorBacks()->colorBack1()->brightness();
        settings->setColor1Data({hue,saturation,brightness});
        fpga_value_y = COLOR1_Y;
        fpga_value_c = COLOR1_C;
    }else if(colorIndex == ColorBacks::COLOR2){
        hue = profile->colorBacks()->colorBack2()->hue();
        saturation = profile->colorBacks()->colorBack2()->saturation();
        brightness = profile->colorBacks()->colorBack2()->brightness();
        settings->setColor2Data({hue,saturation,brightness});
        fpga_value_y = COLOR2_Y;
        fpga_value_c = COLOR2_C;
    }
    ColorYCbCr color = getColorYC(hue,saturation,brightness);
    if(fpga_value_y != -1 && fpga_value_c != -1)
    {
        fpga_write(&g_fpga,fpga_value_y,color.Y);
        fpga_write(&g_fpga,fpga_value_c,*(uint16_t *)&(color.Cr));
    }
}

void Models::setSuperSourceEnable(int enable)
{
//    if(!settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_ENABLE,SUPER_ENABLE,enable))
        return ;
    setSuperSourceCtrl();
}

void Models::setSuperSourceSource1(int source1)
{
//    if(!settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_SOURCE,SUPER_SOURCE_SOURCE_SOURCE1,source1))
        return ;
    setSuperSource();
}

void Models::setSuperSourceSource2(int source2)
{
//    if(!settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_SOURCE,SUPER_SOURCE_SOURCE_SOURCE2,source2))
        return ;
    setSuperSource();
}

void Models::setSuperSourceBackground(int background)
{
//    if(!settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_SOURCE,SUPER_SOURCE_SOURCE_BACKGROUND,background))
        return ;
    setSuperSource();
}

void Models::setSuperSourceControlStyle(int style)
{
//    if(!settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_CONTROL,SUPER_CONTROL_STYLE,style))
        return ;
    setSuperSourceCtrl();
}

void Models::setSuperSourceControlYPosition(int YPosition)
{
//    if(!settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_CONTROL,SUPER_CONTROL_Y_POSITION,YPosition))
        return ;
    float p;
    p = YPosition / 100.0;
    p = (MASK_V_MAX - MASK_V_MIN) * p - MASK_V_MAX;
    int value = getMaskPositionV(p);
    fpga_write(&g_fpga,SUPER_SRC_POS,value);
}

void Models::setSuperSourceMaskEnable(int maskIndex, int enable)
{
    int third = 0;
    if(maskIndex == SUPER_SOURCE_MASK1)
        third = SUPER_MASK1_ENABLE;
    else if(maskIndex == SUPER_SOURCE_MASK2)
        third = SUPER_MASK2_ENABLE;
//    if(!settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,maskIndex,third,enable))
        return ;
    setSuperSourceCtrl();
}

void Models::setSuperSourceMaskHStart(int maskIndex, int HStart)
{
    int third = 0;
    if(maskIndex == SUPER_SOURCE_MASK1)
        third = SUPER_MASK1_H_START;
    else if(maskIndex == SUPER_SOURCE_MASK2)
        third = SUPER_MASK2_H_START;
//    if(!settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,maskIndex,third,HStart))
        return ;
    setSuperSourceMask(maskIndex,third);
}

void Models::setSuperSourceMaskVStart(int maskIndex, int VStart)
{
    int third = 0;
    if(maskIndex == SUPER_SOURCE_MASK1)
        third = SUPER_MASK1_V_START;
    else if(maskIndex == SUPER_SOURCE_MASK2)
        third = SUPER_MASK2_V_START;
//    if(!settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,maskIndex,third,VStart))
        return ;
    setSuperSourceMask(maskIndex,third);
}

void Models::setSuperSourceMaskHEnd(int maskIndex, int HEnd)
{
    int third = 0;
    if(maskIndex == SUPER_SOURCE_MASK1)
        third = SUPER_MASK1_H_END;
    else if(maskIndex == SUPER_SOURCE_MASK2)
        third = SUPER_MASK2_H_END;
//    if(!settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,maskIndex,third,HEnd))
        return ;
    setSuperSourceMask(maskIndex,third);
}

void Models::setSuperSourceMaskVEnd(int maskIndex, int VEnd)
{
    int third = 0;
    if(maskIndex == SUPER_SOURCE_MASK1)
        third = SUPER_MASK1_V_END;
    else if(maskIndex == SUPER_SOURCE_MASK2)
        third = SUPER_MASK2_V_END;
//    if(!settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,maskIndex,third,VEnd))
        return ;
    setSuperSourceMask(maskIndex,third);
}

void Models::setSuperSourceBorderWidth(int borderIndex, int width)
{
    int third = 0;
    if(borderIndex == SUPER_SOURCE_BORDER1)
        third = SUPER_BORDER1_WIDTH;
    else if(borderIndex == SUPER_SOURCE_BORDER2)
        third = SUPER_BORDER2_WIDTH;
//    if(!settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,borderIndex,third,width))
        return ;
    int fpga_value = -1;
    if(borderIndex == SUPER_SOURCE_BORDER1)
        fpga_value = SUPER_SRC1_BORDER;
    else if(borderIndex == SUPER_SOURCE_BORDER1)
       fpga_value = SUPER_SRC2_BORDER;

    if(fpga_value != -1)
        fpga_write(&g_fpga,fpga_value,width);
}

void Models::setSuperSourceBorderColorHue(int borderIndex, int hue)
{
    int third = 0;
    if(borderIndex == SUPER_SOURCE_BORDER1)
        third = SUPER_BORDER1_HUE;
    else if(borderIndex == SUPER_SOURCE_BORDER2)
        third = SUPER_BORDER2_HUE;
//    if(!settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,borderIndex,third,hue))
        return ;
    setSuperSourceColor(borderIndex);
}

void Models::setSuperSourceBorderColorSaturation(int borderIndex, int saturation)
{
    int third = 0;
    if(borderIndex == SUPER_SOURCE_BORDER1)
        third = SUPER_BORDER1_SATURATION;
    else if(borderIndex == SUPER_SOURCE_BORDER2)
        third = SUPER_BORDER2_SATURATION;
//    if(!settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,borderIndex,third,saturation))
        return ;
    setSuperSourceColor(borderIndex);
}

void Models::setSuperSourceBorderColorBrightness(int borderIndex, int brightness)
{
    int third = 0;
    if(borderIndex == SUPER_SOURCE_BORDER1)
        third = SUPER_BORDER1_BRIGHTNESS;
    else if(borderIndex == SUPER_SOURCE_BORDER2)
        third = SUPER_BORDER2_BRIGHTNESS;
//    if(!settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,borderIndex,third,brightness))
        return ;
    setSuperSourceColor(borderIndex);
}

void Models::setSuperSourceCtrl()
{
    int value = 0;
    int enable = settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_ENABLE]->third[SUPER_ENABLE]->current.toInt();
    int mask1 = settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_MASK1]->third[SUPER_MASK1_ENABLE]->current.toInt();
    int mask2 = settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_MASK2]->third[SUPER_MASK2_ENABLE]->current.toInt();
    int style = settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_CONTROL]->third[SUPER_CONTROL_STYLE]->current.toInt();

    if(style == CROP_ZOOM)
        value += 8;
    else if(style == ZOOM_IN_CROP)
        value += 16;
    else if(style == CROP)
        value += 24;

    enable > 0?value += 2: value += 0;
    mask1 > 0?value += 4: value += 0;
    mask2 > 0?value += 1: value += 0;

    fpga_write(&g_fpga,SUPER_SRC_CTRL,value);
}

void Models::setSuperSource()
{
    int source1 = settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_SOURCE]->third[SUPER_SOURCE_SOURCE_SOURCE1]->current.toInt();
    int source2 = settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_SOURCE]->third[SUPER_SOURCE_SOURCE_SOURCE2]->current.toInt();
    int background = settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_SOURCE]->third[SUPER_SOURCE_SOURCE_BACKGROUND]->current.toInt();

    SuperSourceSource source;
    source.source1 = source1;
    source.source2 = source2;
    source.background = background;

    fpga_write(&g_fpga,SUPER_SRC_SEL,*(u_int16_t*)&source);
}

void Models::setSuperSourceMask(int maskIndex,int PositionIndex)
{
    int fpga_value = -1;
    float p;
    int value;
    int per;

    if(maskIndex == SUPER_SOURCE_MASK1)
    {
        switch (PositionIndex) {
        case SUPER_MASK1_H_START:
            //定义域取负
            fpga_value = SUPER_SRC1_MASK_HSTART;
            per = settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[maskIndex]->third[SUPER_MASK1_H_START]->current.toInt();
            p = per / 100.0;
            p = (MASK_H_MAX - MASK_H_MIN) * p - MASK_H_MAX;
            value = getMaskPositionH(p);
            break;
        case SUPER_MASK1_V_START:
            fpga_value = SUPER_SRC1_MASK_VSTART;
            per = settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[maskIndex]->third[SUPER_MASK1_V_START]->current.toInt();
            p = per / 100.0;
            p = (MASK_V_MAX - MASK_V_MIN) * p - MASK_V_MAX;
            value = getMaskPositionV(p);
            break;
        case SUPER_MASK1_H_END:
            //定义域取负
            fpga_value = SUPER_SRC1_MASK_HEND;
            per = settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[maskIndex]->third[SUPER_MASK1_H_END]->current.toInt();
            p = per / 100.0;
            p = (MASK_H_MAX - MASK_H_MIN) * p - MASK_H_MAX;
            value = getMaskPositionH(p);
            break;
        case SUPER_MASK1_V_END:
            fpga_value = SUPER_SRC1_MASK_VEND;
            per = settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[maskIndex]->third[SUPER_MASK1_V_END]->current.toInt();
            p = per / 100.0;
            p = (MASK_V_MAX - MASK_V_MIN) * p - MASK_V_MAX;
            value = getMaskPositionV(p);
            break;
        }
    }else if(maskIndex == SUPER_SOURCE_MASK2)
    {
        switch (PositionIndex) {
        case SUPER_MASK2_H_START:
            //定义域取负
            fpga_value = SUPER_SRC2_MASK_HSTART;
            per = settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[maskIndex]->third[SUPER_MASK2_H_START]->current.toInt();
            p = per / 100.0;
            p = (MASK_H_MAX - MASK_H_MIN) * p - MASK_H_MAX;
            value = getMaskPositionH(p);
            break;
        case SUPER_MASK2_V_START:
            fpga_value = SUPER_SRC2_MASK_VSTART;
            per = settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[maskIndex]->third[SUPER_MASK2_V_START]->current.toInt();
            p = per / 100.0;
            p = (MASK_V_MAX - MASK_V_MIN) * p - MASK_V_MAX;
            value = getMaskPositionV(p);
            break;
        case SUPER_MASK2_H_END:
            //定义域取负
            fpga_value = SUPER_SRC2_MASK_HEND;
            per = settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[maskIndex]->third[SUPER_MASK2_H_END]->current.toInt();
            p = per / 100.0;
            p = (MASK_H_MAX - MASK_H_MIN) * p - MASK_H_MAX;
            value = getMaskPositionH(p);
            break;
        case SUPER_MASK2_V_END:
            fpga_value = SUPER_SRC2_MASK_VEND;
            per = settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[maskIndex]->third[SUPER_MASK2_V_END]->current.toInt();
            p = per / 100.0;
            p = (MASK_V_MAX - MASK_V_MIN) * p - MASK_V_MAX;
            value = getMaskPositionV(p);
            break;
        }
    }
    if(fpga_value != -1)
        fpga_write(&g_fpga,fpga_value,value);
}

void Models::setSuperSourceColor(int colorIndex)
{
    int Hue = 0;
    int Saturation = 0;
    int Brightness = 0;
    int fpga_value_y = -1;
    int fpga_value_c = -1;
    if(colorIndex == SUPER_SOURCE_BORDER1){
        Hue = settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[colorIndex]->third[SUPER_BORDER1_HUE]->current.toInt();
        Saturation = settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[colorIndex]->third[SUPER_BORDER1_SATURATION]->current.toInt();
        Brightness = settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[colorIndex]->third[SUPER_BORDER1_BRIGHTNESS]->current.toInt();
        settings->setColor3Data({Hue,Saturation,Brightness});

        fpga_value_y = SUPER_SRC1_BORDER_Y;
        fpga_value_c = SUPER_SRC1_BORDER_C;

    }else if(colorIndex == SUPER_SOURCE_BORDER2){
        Hue = settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[colorIndex]->third[SUPER_BORDER2_HUE]->current.toInt();
        Saturation = settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[colorIndex]->third[SUPER_BORDER2_SATURATION]->current.toInt();
        Brightness = settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[colorIndex]->third[SUPER_BORDER2_BRIGHTNESS]->current.toInt();
        settings->setColor4Data({Hue,Saturation,Brightness});

        fpga_value_y = SUPER_SRC2_BORDER_Y;
        fpga_value_c = SUPER_SRC2_BORDER_C;
    }

    ColorYCbCr color = getColorYC(Hue,Saturation,Brightness);
    if(fpga_value_y != -1 && fpga_value_c != -1)
    {
        fpga_write(&g_fpga,fpga_value_y,color.Y);
        fpga_write(&g_fpga,fpga_value_c,*(uint16_t *)&(color.Cr));
    }
}

void Models::setKeyType(int type)
{
//    if(!settings->setMenuValue(MENU_FIRST_KEY_TYPE,KEY_TYPE_TYPE,TYPE_TYPE,type))
        return ;
    fpga_write(&g_fpga,UPSTREAM_KEY_TYPE,type);
}

void Models::setKeyMaskEnable(int key, int enable)
{
//    int second = 0;
//    int third = 0;
//    int fpga_value = -1;
//    switch (key) {
//    case MENU_FIRST_LUMA_KEY:
//        second = LUMA_KEY_MASK;
//        third = LUMA_KEY_MASK_ENABLE;
//        break;
//    case MENU_FIRST_CHROMA_KEY:
//        second = CHROMA_KEY_MASK;
//        third = CHROMA_KEY_MASK_ENABLE;
//        break;
//    case MENU_FIRST_KEY_PATTERN:
//        second = PATTERN_MASK;
//        third = KEY_PATTERN_MASK_ENABLE;
//        break;
//    case MENU_FIRST_PIP:
//        second = PIP_MASK;
//        third = PIP_MASK_ENABLE;
//        break;
//    }
}

void Models::setKeyMaskHStart(int key, int hStart)
{

}

void Models::setKeyMaskVStart(int key, int vStart)
{

}

void Models::setKeyMaskHEnd(int key, int hEnd)
{

}

void Models::setKeyMaskVEnd(int key, int vEnd)
{

}

void Models::setKeyResize(int key, int resize)
{

}

void Models::setKeySize(int key, int size)
{

}

void Models::setKeyXPosition(int key, float xPosition)
{

}

void Models::setKeyYPosition(int key, float yPosition)
{

}

void Models::setKeySourceFill(int key, int fill)
{
    int fpga_value = -1;
    u_int16_t value = fill;;
    u_int8_t key_index = 0;
    LumaKeySource source = {};
    int second = 0;
    int third = 0;
    switch (key) {
    case MENU_FIRST_LUMA_KEY:
        second = LUMA_KEY_SOURCE;
        third = LUMA_KEY_SOURCE_FILL;
        fpga_value = LUMA_SRC_SEL;
        key_index = settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_SOURCE]->third[LUMA_KEY_SOURCE_KEY]->current.toInt();
        //屏蔽color1、color2
        if(key_index > INPUT_SOURCE_STILL2_KEY)
            key_index +=2;
        source = {(u_int8_t)fill,key_index};
        value = *(u_int16_t*)&source;
        break;
    case MENU_FIRST_CHROMA_KEY:
        second = CHROMA_KEY_SOURCE;
        third = CHROMA_KEY_SOURCE_FILL;
        fpga_value = CHROMA_SRC_SEL;
        break;
    case MENU_FIRST_KEY_PATTERN:
        second = PATTERN_SOURCE;
        third = KEY_PATTERN_SOURCE_FILL;
        fpga_value = PATTERN_SRC_SEL;
        break;
    case MENU_FIRST_PIP:
        second = PIP_SOURCE;
        third = PIP_SOURCE_FILL;
        fpga_value = PIP_SRC_SEL;
        break;
    }
//    if(!settings->setMenuValue(key,second,third,fill))
        return ;
    if(fpga_value != -1)
        fpga_write(&g_fpga,fpga_value,value);
}

void Models::setKeySourceKey(int key, int sourceKey)
{
    if(key == MENU_FIRST_LUMA_KEY)
    {
    //    if(!settings->setMenuValue(key,LUMA_KEY_SOURCE,LUMA_KEY_SOURCE_KEY,sourceKey))
            return ;
        int fill = settings->listFirst()[MENU_FIRST_LUMA_KEY]->second[LUMA_KEY_SOURCE]->third[LUMA_KEY_SOURCE_FILL]->current.toInt();
        LumaKeySource source = {(u_int8_t)fill,(u_int8_t)sourceKey};
        fpga_write(&g_fpga,LUMA_SRC_SEL,*(u_int16_t*)&source);
    }
}

void Models::setKeyClip(int key, int clip)
{

}

void Models::setKeyGain(int key, int gain)
{

}

void Models::setKeyShapedKey(int key, int enable)
{

}

void Models::setKeyInvert(int key, int enable)
{

}


//QString Models::parameterToString(QMetaMethod func, int parameter)
//{
//    if(func == QMetaMethod::fromSignal(&Models::liveStatus) || func == QMetaMethod::fromSignal(&Models::record) || func == QMetaMethod::fromSignal(&Models::prev) || func == QMetaMethod::fromSignal(&Models::keyOnAir) || func == QMetaMethod::fromSignal(&Models::dskOnAir)){
//        if(parameter != 0){
//            return "true";
//        }else{
//            return "false";
//        }
//    }else if(func == QMetaMethod::fromSignal(&Models::play)){
//        if(parameter == 0){
//            return "true";
//        }else{
//            return "false";
//        }
//    }else if(func == QMetaMethod::fromSignal(&Models::pgmIndex) || func == QMetaMethod::fromSignal(&Models::pvwIndex)){
//        if(parameter == 0){
//            return "In 1";
//        }else if(parameter == 1){
//            return "In 2";
//        }else if(parameter == 2){
//            return "In 3";
//        }else if(parameter == 3){
//            return "In 4";
//        }else if(parameter == 4){
//            return "Aux";
//        }else if(parameter == 5){
//            return "Black";
//        }else if(parameter == 6){
//            return "Still 1";
//        }else if(parameter == 7){
//            return "Still 2";
//        }
//    }else if(func == QMetaMethod::fromSignal(&Models::transitionIndex)){
//        if(parameter == 0){
//            return "Mix";
//        }else if(parameter == 1){
//            return "Dip";
//        }else if(parameter == 2){
//            return "Wipe";
//        }
//    }else if(func == QMetaMethod::fromSignal(&Models::transitionSource)){
//        QString ret = "";
//        if(parameter & 0x4){
//            ret += "BKGD,";
//        }
//        if(parameter & 0x2){
//            ret += "DSK,";
//        }
//        if(parameter & 0x1){
//            ret += "KEY,";
//        }
//        if(ret.size() > 0 && ret[ret.size() - 1] == ','){
//            ret = ret.mid(0,ret.size() - 1);
//        }
//        return ret;
//    }
//    else{
//        return QString::number(parameter);
//    }
//}

//int Models::parameterToInt(QMetaMethod func, QString parameter)
//{
//    if(func == QMetaMethod::fromSignal(&Models::liveStatus) || func == QMetaMethod::fromSignal(&Models::record) || func == QMetaMethod::fromSignal(&Models::prev) || func == QMetaMethod::fromSignal(&Models::keyOnAir) || func == QMetaMethod::fromSignal(&Models::dskOnAir)){
//        if(parameter == "true"){
//            return 1;
//        }else if(parameter == "false"){
//            return 0;
//        }else{
//            return -1;
//        }
//    }else if(func == QMetaMethod::fromSignal(&Models::play)){
//        if(parameter == "true"){
//            return 0;
//        }else if(parameter == "false"){
//            return 1;
//        }else{
//            return -1;
//        }
//    }else if(func == QMetaMethod::fromSignal(&Models::pgmIndex) || func == QMetaMethod::fromSignal(&Models::pvwIndex)){
//        if(parameter == "In 1"){
//            return 0;
//        }else if(parameter == "In 2"){
//            return 1;
//        }else if(parameter == "In 3"){
//            return 2;
//        }else if(parameter == "In 4"){
//            return 3;
//        }else if(parameter == "Aux"){
//            return 4;
//        }else if(parameter == "Black"){
//            return 5;
//        }else if(parameter == "Still 1"){
//            return 6;
//        }else if(parameter == "Still 2"){
//            return 7;
//        }else{
//            return -1;
//        }
//    }else if(func == QMetaMethod::fromSignal(&Models::transitionIndex)){
//        if(parameter == "Mix"){
//            return 0;
//        }else if(parameter == "Dip"){
//            return 1;
//        }else if(parameter == "Wipe"){
//            return 2;
//        }else{
//            return -1;
//        }
//    }else if(func == QMetaMethod::fromSignal(&Models::transitionSource)){
//        auto list = parameter.split(",");
//        int value = 0;
//        for(int i = 0;i < list.size();++i){
//            if(list[i] == "KEY"){
//                value += 1;
//            }else if(list[i] == "DSK"){
//                value += 2;
//            }else if(list[i] == "BKGD"){
//                value += 4;
//            }
//        }
//        return value;
//    }
//    else if(func == QMetaMethod::fromSignal(&Models::transitionPosition)){
//        bool flag = true;
//        int value = parameter.toInt(&flag);
//        if(!flag)
//            return -1;
//        if(value >= 0 && value <= 255)
//            return value;
//        else
//            return -1;
//    }else if(func == QMetaMethod::fromSignal(&Models::mSleep)){
//        bool flag = true;
//        int value = parameter.toInt(&flag);
//        if(!flag)
//            return -1;
//        if(value >= 0 && value <= settings->listFirst()[MENU_FIRST_MACRO]->second[MACRO_MACRO]->third[MENU_THIRD_MACRO_SLEEP]->max.toInt())
//            return value;
//        else
//            return -1;
//    }
//    else{
//        return -1;
//    }
//}

//int Models::isKeyMehotd(QMetaMethod func)
//{
//    if(func == QMetaMethod::fromSignal(&Models::liveStatus) ||
//       func == QMetaMethod::fromSignal(&Models::record) ||
//       func == QMetaMethod::fromSignal(&Models::prev) ||
//       func == QMetaMethod::fromSignal(&Models::keyOnAir) ||
//       func == QMetaMethod::fromSignal(&Models::dskOnAir) ||
//       func == QMetaMethod::fromSignal(&Models::play) ||
//       func == QMetaMethod::fromSignal(&Models::pgmIndex) ||
//       func == QMetaMethod::fromSignal(&Models::pvwIndex) ||
//       func == QMetaMethod::fromSignal(&Models::transitionIndex) ||
//       func == QMetaMethod::fromSignal(&Models::transitionSource) ||
//       func == QMetaMethod::fromSignal(&Models::transitionPosition) ||
//       func == QMetaMethod::fromSignal(&Models::playNext) ||
//       func == QMetaMethod::fromSignal(&Models::playPrevious) ||
//       func == QMetaMethod::fromSignal(&Models::ftb) ||
//       func == QMetaMethod::fromSignal(&Models::autoTransition) ||
//       func == QMetaMethod::fromSignal(&Models::cutTransition) ||
//       func == QMetaMethod::fromSignal(&Models::mSleep) ||
//       func == QMetaMethod::fromSignal(&Models::userWait)
//        ){
//        return 0;
//    }else{
//        return -1;
//    }
//}

