#include "settings.h"
#include <QDebug>
#include <QFile>
#include <unistd.h>
#include <QDir>
#include "MenuThirdImplement.h"
#include "MenuEnums.h"
#include "include/hw_config.h"
#include "rv_switch_api.h"
#include "fpga.h"
#include "device_info.h"
#include "pccmd.h"
#include "KeyImplement.h"
#include "xmlprocessor.h"

bool Settings::showColorBack1Flag()
{
    if(lastFirstUnfold() == MENU_FIRST_COLOR_BACK && leftListViewCurrent() == COLOR_BACK_COLOR1 + MENU_FIRST_COLOR_BACK + 1){
        return true;
    }else{
        return false;
    }
}

bool Settings::showColorBack2Flag()
{
    if(lastFirstUnfold() == MENU_FIRST_COLOR_BACK && leftListViewCurrent() == COLOR_BACK_COLOR2 + MENU_FIRST_COLOR_BACK +1){
        return true;
    }else{
        return false;
    }
}

bool Settings::showSuperSourceBorder1ColorFlag()
{
    if(lastFirstUnfold() == MENU_FIRST_SUPER_SOURCE && leftListViewCurrent() == SUPER_SOURCE_BORDER1 + MENU_FIRST_SUPER_SOURCE + 1){
        return true;
    }else{
        return false;
    }
}

bool Settings::showSuperSourceBorder2ColorFlag()
{
    if(lastFirstUnfold() == MENU_FIRST_SUPER_SOURCE && leftListViewCurrent() == SUPER_SOURCE_BORDER2 + MENU_FIRST_SUPER_SOURCE + 1){
        return true;
    }else{
        return false;
    }
}

bool Settings::showChromaKeyControlColorFlag()
{
    if(lastFirstUnfold() == MENU_FIRST_CHROMA_KEY && leftListViewCurrent() == CHROMA_KEY_CONTROL + MENU_FIRST_CHROMA_KEY +1){
        return true;
    }else{
        return false;
    }
}

bool Settings::showPIPBorderColorFLag()
{
    if(lastFirstUnfold() == MENU_FIRST_PIP && leftListViewCurrent() == PIP_COLOR + MENU_FIRST_PIP + 1){
        return true;
    }else{
        return false;
    }
}

bool Settings::isTransitionSelect()
{
    if(lastFirstUnfold() == MENU_FIRST_TRANSITION){
        return true;
    }else{
        return false;
    }
}

bool Settings::isKeyPatternSelect()
{
    if(lastFirstUnfold() == MENU_FIRST_KEY_PATTERN){
        return true;
    }else{
        return false;
    }
}


Settings::Settings(QObject *parent) : QObject(parent)
{
    QDir dir (DATA_PATH);
    if(!dir.exists())
    {
        dir.mkdir(DATA_PATH);
    }
    QDir dir2 (STILLPATH);
    if(!dir2.exists())
    {
        dir2.mkdir(STILLPATH);
    }
    QDir dir3 (MACRO_PATH);
    if(!dir3.exists())
    {
        dir3.mkdir(MACRO_PATH);
    }
    init_settings();
}

void Settings::init_settings()
{
    initStreamData();
    initRecordData();
//    init_menuStatus();
    init_menuData();
    init_keyData();
    init_stillImages();
    init_PgmTallyTimer();

    connect(&pccmd,SIGNAL(emit_sn_change(QString)),this,SLOT(deviceid_change(QString)));
}

void Settings::init_stillImages()
{
    QSettings set(DATA_STILL_PATH,QSettings::IniFormat);
    for(int i = 0;i < STILLMAX;++i)
    {
        appendStillImages(set.value("still/" + QString::number(i)).toString());
    }
}

void Settings::init_chromaColor()
{
    int y = getIniValue(CHROMAKEY_SMP_COLOR_PATH,"y").toInt();
    int cb = getIniValue(CHROMAKEY_SMP_COLOR_PATH,"cb").toInt();
    int cr = getIniValue(CHROMAKEY_SMP_COLOR_PATH,"cr").toInt();
    int r = getIniValue(CHROMAKEY_SMP_COLOR_PATH,"r").toInt();
    int g = getIniValue(CHROMAKEY_SMP_COLOR_PATH,"g").toInt();
    int b = getIniValue(CHROMAKEY_SMP_COLOR_PATH,"b").toInt();

    QList<int>rgb,ycbcr;
    rgb << r << g << b;
    ycbcr << y << cb << cr;

    setChromakeySMPColorYcbcr(ycbcr);
    setColor5Data(rgb);
}

void Settings::reset_settings()
{

}

void Settings::init_menuStatus()
{
    setKeyIsPressed(0);

    setLastFirstUnfold(-1);
    setLastThirdUnfold(-1);
    setLastSecondUnfold(-1);
    setLeftListViewCurrent(0);
    setRightListViewCurrent(-1);
    setAudioVisible(1);
    setMenuVisible(MENU_HIDE);
    setMenuSize(0);

    setAudioMixerSelectedIndex(-1);
    setKeyboardVisible(0);
    setKeyboardType(KeyboardEvent::KEYBOARD_NORMAL);
    setKeyboardCurrentIndex(0);
    setKeyboardKetMoveFlag(0);

    setRecordSecond(0);
    setRecordTimeStr("00:00:00");
    setLiveSecond(0);
    setLiveTimeStr("00:00:00");

    setLiveStreamStatus1(E_STATUS_CLOSE);
    setLiveStreamStatus2(E_STATUS_CLOSE);
    setLiveStreamStatus3(E_STATUS_CLOSE);
    setLiveStatus(0);
    setLiveLedStatus(E_STATUS_CLOSE);

    setReallyOutFormat(listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_FORMAT]->third[SETTING_OUT_FORMAT_FORMAT]->current.toInt());
    setReallySrcSelectionIn1(listFirst()[MENU_FIRST_SETTING]->second[SETTING_AUX_SOURCE]->third[SETTING_AUX_SOURCE_IN1]->current.toInt());
    setReallySrcSelectionIn2(listFirst()[MENU_FIRST_SETTING]->second[SETTING_AUX_SOURCE]->third[SETTING_AUX_SOURCE_IN2]->current.toInt());
    setReallySrcSelectionIn3(listFirst()[MENU_FIRST_SETTING]->second[SETTING_AUX_SOURCE]->third[SETTING_AUX_SOURCE_IN3]->current.toInt());
    setReallySrcSelectionIn4(listFirst()[MENU_FIRST_SETTING]->second[SETTING_AUX_SOURCE]->third[SETTING_AUX_SOURCE_IN4]->current.toInt());


//    setPlayLedStatus(E_STATUS_MP4_CLOSE);
    //不发送信号，但是初始化，避免顺序播放逻辑导致开机自动播放
    m_playLedStatus = E_STATUS_MP4_CLOSE;
    setRecordLedStatus(E_STATUS_FAILED);
    setPlayListCurrent(0);

    setPlayAutoNextFlag(0);

    setKeyboardReg(-1);

    setListDialogVisible(0);
}

void Settings::new_menuData()
{
    for(int i = MENU_FIRST_COLOR_BACK;i < MENU_FIRST_MAX;++i)
    {
        switch (i) {
        case MENU_FIRST_COLOR_BACK:
            new_ColorBack();
            break;
        case MENU_FIRST_SUPER_SOURCE:
            new_SuperSource();
            break;
        case MENU_FIRST_KEY_TYPE:
            new_KeyType();
            break;
        case MENU_FIRST_LUMA_KEY:
            new_LumaKey();
            break;
        case MENU_FIRST_CHROMA_KEY:
            new_ChromaKey();
            break;
        case MENU_FIRST_KEY_PATTERN:
            new_KeyPattern();
            break;
        case MENU_FIRST_PIP:
            new_PIP();
            break;
        case MENU_FIRST_TRANSITION:
            new_Transition();
            break;
        case MENU_FIRST_DSK:
            new_DSK();
            break;
        case MENU_FIRST_FTB:
            new_FTB();
            break;
        case MENU_FIRST_AUDIO_MIXER:
            new_AudioMixer();
            break;
        case MENU_FIRST_STILL_GENERATOR:
            new_StillGenerator();
            break;
        case MENU_FIRST_MACRO:
            new_Macro();
            break;
        case MENU_FIRST_STREAM:
            new_Stream();
            break;
        case MENU_FIRST_PLAYBACK:
            new_Playback();
            break;
        case MENU_FIRST_SETTING:
            new_Setting();
            break;
        }
    }
}


void Settings::new_AudioMixer()
{
    MenuFirst *first = new MenuFirst;
    m_listFirst.append(first);
    first->name = tr("Audio Mixer");
    first->ss_name = ("Audio Mixer");


    QList<MenuSecond*>list_second;

    for(int i = AUDIO_MIXER_MIC1;i < AUDIO_MIXER_MAX;++i)
    {
        switch (i) {
        case AUDIO_MIXER_MIC1:
            list_second.append(new_Mic1());
            break;
        case AUDIO_MIXER_MIC2:
            list_second.append(new_Mic2());
            break;
        case AUDIO_MIXER_IN1:
            list_second.append(new_In1());
            break;
        case AUDIO_MIXER_IN2:
            list_second.append(new_In2());
            break;
        case AUDIO_MIXER_IN3:
            list_second.append(new_In3());
            break;
        case AUDIO_MIXER_IN4:
            list_second.append(new_In4());
            break;
        case AUDIO_MIXER_AUX:
            list_second.append(new_Aux());
            break;
        case AUDIO_MIXER_PGM:
            list_second.append(new_PGM());
            break;
        case AUDIO_MIXER_MONITOR:
            list_second.append(new_Monitor());
            break;
        }
    }
    first->second = list_second;
}

void Settings::new_StillGenerator()
{
    MenuFirst *first = new MenuFirst;
    m_listFirst.append(first);
    first->name = tr("Still Generator");
    first->ss_name = ("Still Generator");

    QList<MenuSecond *>list_second;
    for(int i = STILL_GENERATE_SELECTION;i < STILL_GENERATE_MAX;++i)
    {
        switch (i) {
        case STILL_GENERATE_SELECTION:
            list_second.append(new_Still_Selection());
            break;
        case STILL_GENERATE_UPLOAD:
            list_second.append(new_Still_Upload());
            break;
        }
    }
    first->second = list_second;
}

void Settings::new_Macro()
{
    MenuFirst *first = new MenuFirst;
    m_listFirst.append(first);
    first->name = tr("Macro");
    first->ss_name = ("Macro");

    QList<MenuSecond *>list_second;
    for(int i = MACRO_MACRO;i < MACRO_MAX;++i)
    {
        switch (i) {
        case MACRO_MACRO:
            list_second.append(new_Macro_Sleep());
            break;
        }
    }
    first->second = list_second;
}

void Settings::new_Stream()
{
    MenuFirst *first = new MenuFirst;
    m_listFirst.append(first);
    first->name = tr("Stream");
    first->ss_name = ("Stream");

    QList<MenuSecond *>list_second;

    for(int i = STREAM_STREAM1;i < STREAM_MAX;++i)
    {
        switch (i) {
        case STREAM_STREAM1:
            list_second.append(new_Stream1());
            break;
        case STREAM_STREAM2:
            list_second.append(new_Stream2());
            break;
        case STREAM_STREAM3:
            list_second.append(new_Stream3());
            break;
        }
    }
    first->second = list_second;
}

void Settings::new_Playback()
{
    MenuFirst *first = new MenuFirst;
    m_listFirst.append(first);
    first->name = tr("Playback");
    first->ss_name = ("Playback");

    QList<MenuSecond *>list_second;

    for(int i = PLAYBACK_PLAYBACK;i < PLAYBACK_MAX;++i)
    {
        switch (i) {
        case PLAYBACK_PLAYBACK:
            list_second.append(new_Playback_Set());
            break;
        }
    }
    first->second = list_second;
}

void Settings::new_ColorBack()
{
    MenuFirst *first = new MenuFirst;
    m_listFirst.append(first);
    first->name = tr("Color Back");
    first->ss_name = "Color Back";

    QList<MenuSecond *>list_second;

    for(int i = COLOR_BACK_COLOR1;i < COLOR_BACK_MAX;++i)
    {
        switch (i) {
        case COLOR_BACK_COLOR1:
            list_second.append(new_Color1());
            break;
        case COLOR_BACK_COLOR2:
            list_second.append(new_Color2());
            break;
        }
    }
    first->second = list_second;
}

void Settings::new_SuperSource()
{
    MenuFirst *first = new MenuFirst;
    m_listFirst.append(first);
    first->name = tr("Super Source");
    first->ss_name = ("Super Source");

    QList<MenuSecond *>list_second;

    for(int i = SUPER_SOURCE_ENABLE;i < SUPER_SOURCE_MAX;++i)
    {
        switch (i) {
        case SUPER_SOURCE_ENABLE:
            list_second.append(new_SuperSource_Enable());
            break;
        case SUPER_SOURCE_SOURCE:
            list_second.append(new_SuperSource_Source());
            break;
        case SUPER_SOURCE_CONTROL:
            list_second.append(new_SuperSource_Control());
            break;
        case SUPER_SOURCE_MASK1:
            list_second.append(new_SuperSource_Mask1());
            break;
        case SUPER_SOURCE_MASK2:
            list_second.append(new_SuperSource_Mask2());
            break;
        case SUPER_SOURCE_BORDER1:
            list_second.append(new_SuperSource_Board1());
            break;
        case SUPER_SOURCE_BORDER2:
            list_second.append(new_SuperSource_Board2());
            break;
        }
    }
    first->second = list_second;
}

void Settings::new_PIP()
{
    MenuFirst *first = new MenuFirst;
    m_listFirst.append(first);
    first->name = tr("PIP");
    first->ss_name = ("PIP");

    QList<MenuSecond *>list_second;

    for(int i = PIP_SOURCE;i < PIP_MAX;++i)
    {
        switch (i) {
        case PIP_SOURCE:
            list_second.append(new_PIP_Source());
            break;
        case PIP_SIZE_POSITION:
            list_second.append(new_PIP_Size_Position());
            break;
        case PIP_MASK:
            list_second.append(new_PIP_Mask());
            break;
        case PIP_BOARD:
            list_second.append(new_PIP_Board());
            break;
        case PIP_COLOR:
            list_second.append(new_PIP_Color());
            break;
        }
    }
    first->second = list_second;
}

void Settings::new_KeyType()
{
    MenuFirst *first = new MenuFirst;
    m_listFirst.append(first);
    first->name = tr("Key Type");
    first->ss_name = ("Key Type");

    QList<MenuSecond *>list_second;
    for(int i = KEY_TYPE_TYPE;i < KEY_TYPE_MAX;++i)
    {
        switch (i) {
        case KEY_TYPE_TYPE:
            list_second.append(new_type());
            break;
        }
    }
    first->second = list_second;
}

void Settings::new_LumaKey()
{
    MenuFirst *first = new MenuFirst;
    m_listFirst.append(first);
    first->name = tr("Luma Key");
    first->ss_name = ("Luma Key");

    QList<MenuSecond *>list_second;
    for(int i = LUMA_KEY_SOURCE;i < LUMA_KEY_MAX;++i)
    {
        switch (i) {
        case LUMA_KEY_SOURCE:
            list_second.append(new_LumaKey_Source());
            break;
        case LUMA_KEY_MASK:
            list_second.append(new_LumaKey_Mask());
            break;
        case LUMA_KEY_CONTROL:
            list_second.append(new_LumaKey_Control());
            break;
        case LUMA_KEY_RESIZE:
            list_second.append(new_LumaKey_Resize());
            break;
        }
    }
    first->second = list_second;
}

void Settings::new_ChromaKey()
{
    MenuFirst *first = new MenuFirst;
    m_listFirst.append(first);
    first->name = tr("Chroma Key");
    first->ss_name = ("Chroma Key");

    QList<MenuSecond *>list_second;
    for(int i = CHROMA_KEY_SOURCE;i < CHROMA_KEY_MAX;++i)
    {
        switch (i) {
        case CHROMA_KEY_SOURCE:
            list_second.append(new_ChromaKey_Source());
            break;
        case CHROMA_KEY_MASK:
            list_second.append(new_ChromaKey_Mask());
            break;
        case CHROMA_KEY_RESIZE:
            list_second.append(new_ChromaKey_Resize());
            break;
        case CHROMA_KEY_CONTROL:
            list_second.append(new_ChromaKey_Control());
            break;
        }
    }
    first->second = list_second;
}

void Settings::new_KeyPattern()
{
    MenuFirst *first = new MenuFirst;
    m_listFirst.append(first);
    first->name = tr("Key Pattern");
    first->ss_name = ("Key Pattern");

    QList<MenuSecond *>list_second;
    for(int i = PATTERN_SOURCE;i < PATTERN_MAX;++i)
    {
        switch (i) {
        case PATTERN_SOURCE:
            list_second.append(new_KeyPattern_Source());
            break;
        case PATTERN_WIPE:
            list_second.append(new_KeyPattern_Wipe());
            break;
        case PATTERN_MASK:
            list_second.append(new_KeyPattern_Mask());
            break;
        case PATTERN_RESIZE:
            list_second.append(new_KeyPattern_Resize());
            break;
        }
    }
    first->second = list_second;
}

void Settings::new_Transition()
{
    MenuFirst *first = new MenuFirst;
    m_listFirst.append(first);
    first->name = tr("Transition");
    first->ss_name = ("Transition");

    QList<MenuSecond *>list_second;
    for(int i = TRANSITION_MIX;i < TRANSITION_MAX;++i)
    {
        switch (i) {
        case TRANSITION_MIX:
            list_second.append(new_Transition_Mix());
            break;
        case TRANSITION_DIP:
            list_second.append(new_Transition_Dip());
            break;
        case TRANSITION_WIPE:
            list_second.append(new_Transition_Wipe());
            break;
        }
    }
    first->second = list_second;
}

void Settings::new_DSK()
{
    MenuFirst *first = new MenuFirst;
    m_listFirst.append(first);
    first->name = tr("DSK");
    first->ss_name = ("DSK");

    QList<MenuSecond *>list_second;
    for(int i = DSK_SOURCE;i < DSK_MAX;++i)
    {
        switch (i) {
        case DSK_SOURCE:
            list_second.append(new_DSK_Source());
            break;
        case DSK_MASK:
            list_second.append(new_DSK_Mask());
            break;
        case DSK_CONTROL:
            list_second.append(new_DSK_Control());
            break;
        case DSK_RATE:
            list_second.append(new_DSK_Rate());
            break;
        }
    }
    first->second = list_second;
}

void Settings::new_FTB()
{
    MenuFirst *first = new MenuFirst;
    m_listFirst.append(first);
    first->name = tr("FTB");
    first->ss_name = ("FTB");

    QList<MenuSecond *>list_second;
    for(int i = MENU_SECOND_FTB_RATE;i < FTB_MAX;++i)
    {
        switch (i) {
        case MENU_SECOND_FTB_RATE:
            list_second.append(new_FTB_Rate());
            break;
        case MENU_SECOND_FTB_AUDIO:
            list_second.append(new_FTB_Audio());
            break;
        }
    }
    first->second = list_second;
}

void Settings::new_Setting()
{
    MenuFirst *first = new MenuFirst;
    m_listFirst.append(first);
    first->name = tr("Setting");
    first->ss_name = ("Setting");

    QList<MenuSecond *>list_second;
    for(int i = SETTING_VERSION;i < SETTING_MAX;++i)
    {
        switch (i) {
        case SETTING_VERSION:
            list_second.append(new_Setting_Version());
            break;
        case SETTING_SRC_NAME:
            list_second.append(new_Setting_Src_Name());
            break;
        case SETTING_MV_METER:
            list_second.append(new_Setting_Mv_Meter());
            break;
        case SETTING_MV_LAYOUT:
            list_second.append(new_Setting_Mv_Layout());
            break;
        case SETTING_MARKER:
            list_second.append(new_Setting_Market());
            break;
        case SETTING_MIC_INPUT:
            list_second.append(new_Setting_Mic_Input());
            break;
        case SETTING_RECORD_FILE:
            list_second.append(new_Setting_Record_File());
            break;
        case SETTING_AUX_SOURCE:
            list_second.append(new_Setting_Aux_Source());
            break;
        case SETTING_OUT_FORMAT:
            list_second.append(new_Setting_Out_Format());
            break;
        case SETTING_OUT_SOURCE:
            list_second.append(new_Setting_Out_Source());
            break;
        case SETTING_QUALITY:
            list_second.append(new_Setting_Quality());
            break;
        case SETTING_NETWORK:
            list_second.append(new_Setting_Network());
            break;
        case SETTING_PANEL:
            list_second.append(new_Setting_Panel());
            break;
        case SETTING_LANGUAGE:
            list_second.append(new_Setting_Language());
            break;
        case SETTING_RESET:
            list_second.append(new_Setting_Reset());
            break;
        }
    }
    first->second = list_second;
}

MenuSecond *Settings::new_Mic1()
{
    MenuSecond *mic1 = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    mic1->name = tr("mic 1");
    mic1->ss_name = ("mic 1");

    third = new MenuThirdAudioMixerMic1Fader(tr("Fader"),"Fader",-75.0,0,10.0,0.5,DATATYPE_DB);
    list_third.append(third);

    third = new MenuThirdAudioMixerMic1Balance(tr("Balance"),"Balance",-40.0,0,40.0,0.5,DATATYPE_DB);
    list_third.append(third);

    third = new MenuThirdAudioMixerMic1Input(tr("Input"),"Input",-75.0,0,6.0,0.5,DATATYPE_DB);
    list_third.append(third);

    third = new MenuThirdAudioMixerMic1Delay(tr("Delay"),"Delay",0,0,170,10,DATATYPE_MS);
    list_third.append(third);

    third = new MenuThirdAudioMixerMic1Enable(tr("Enable"),"Enable",0,0,2,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    mic1->third = list_third;
    return mic1;
}

MenuSecond *Settings::new_Mic2()
{
    MenuSecond *mic2 = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    mic2->name = tr("mic 2");
    mic2->ss_name = ("mic 2");

    third = new MenuThirdAudioMixerMic2Fader(tr("Fader"),"Fader",-75.0,0,10.0,0.5,DATATYPE_DB);
    list_third.append(third);

    third = new MenuThirdAudioMixerMic2Balance(tr("Balance"),"Balance",-40.0,0,40.0,0.5,DATATYPE_DB);
    list_third.append(third);

    third = new MenuThirdAudioMixerMic2Input(tr("Input"),"Input",-75.0,0,6.0,0.5,DATATYPE_DB);
    list_third.append(third);

    third = new MenuThirdAudioMixerMic2Delay(tr("Delay"),"Delay",0,0,170,10,DATATYPE_MS);
    list_third.append(third);

    third = new MenuThirdAudioMixerMic2Enable(tr("Enable"),"Enable",0,0,2,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    mic2->third = list_third;
    return mic2;
}

MenuSecond *Settings::new_In1()
{
    MenuSecond *in1 = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    in1->name = tr("in 1");
    in1->ss_name = ("in 1");

    third = new MenuThirdAudioMixerIn1Fader(tr("Fader"),"Fader",-75.0,0,10.0,0.5,DATATYPE_DB);
    list_third.append(third);

    third = new MenuThirdAudioMixerIn1Balance(tr("Balance"),"Balance",-40.0,0,40.0,0.5,DATATYPE_DB);
    list_third.append(third);

    third = new MenuThirdAudioMixerIn1Input(tr("Input"),"Input",-75.0,0,6.0,0.5,DATATYPE_DB);
    list_third.append(third);

    third = new MenuThirdAudioMixerIn1Enable(tr("Enable"),"Enable",0,0,2,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On") << tr("AFV");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    in1->third = list_third;
    return in1;
}

MenuSecond *Settings::new_In2()
{
    MenuSecond *in2 = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    in2->name = tr("in 2");
    in2->ss_name = ("in 2");

    third = new MenuThirdAudioMixerIn2Fader(tr("Fader"),"Fader",-75.0,0,10.0,0.5,DATATYPE_DB);
    list_third.append(third);

    third = new MenuThirdAudioMixerIn2Balance(tr("Balance"),"Balance",-40.0,0,40.0,0.5,DATATYPE_DB);
    list_third.append(third);

    third = new MenuThirdAudioMixerIn2Input(tr("Input"),"Input",-75.0,0,6.0,0.5,DATATYPE_DB);
    list_third.append(third);

    third = new MenuThirdAudioMixerIn2Enable(tr("Enable"),"Enable",0,0,2,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On") << tr("AFV");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    in2->third = list_third;
    return in2;
}

MenuSecond *Settings::new_In3()
{
    MenuSecond *in3 = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    in3->name = tr("in 3");
    in3->ss_name = ("in 3");

    third = new MenuThirdAudioMixerIn3Fader(tr("Fader"),"Fader",-75.0,0,10.0,0.5,DATATYPE_DB);
    list_third.append(third);

    third = new MenuThirdAudioMixerIn3Balance(tr("Balance"),"Balance",-40.0,0,40.0,0.5,DATATYPE_DB);
    list_third.append(third);

    third = new MenuThirdAudioMixerIn3Input(tr("Input"),"Input",-75.0,0,6.0,0.5,DATATYPE_DB);
    list_third.append(third);

    third = new MenuThirdAudioMixerIn3Enable(tr("Enable"),"Enable",0,0,2,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On") << tr("AFV");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    in3->third = list_third;
    return in3;
}

MenuSecond *Settings::new_In4()
{
    MenuSecond *in4 = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    in4->name = tr("in 4");
    in4->ss_name = ("in 4");

    third = new MenuThirdAudioMixerIn4Fader(tr("Fader"),"Fader",-75.0,0,10.0,0.5,DATATYPE_DB);
    list_third.append(third);

    third = new MenuThirdAudioMixerIn4Balance(tr("Balance"),"Balance",-40.0,0,40.0,0.5,DATATYPE_DB);
    list_third.append(third);

    third = new MenuThirdAudioMixerIn4Input(tr("Input"),"Input",-75.0,0,6.0,0.5,DATATYPE_DB);
    list_third.append(third);

    third = new MenuThirdAudioMixerIn4Enable(tr("Enable"),"Enable",0,0,2,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On") << tr("AFV");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    in4->third = list_third;
    return in4;
}

MenuSecond *Settings::new_Aux()
{
    MenuSecond *aux = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    aux->name = tr("aux");
    aux->ss_name = ("aux");

    third = new MenuThirdAudioMixerAuxFader(tr("Fader"),"Fader",-75.0,0,10.0,0.5,DATATYPE_DB);
    list_third.append(third);

    third = new MenuThirdAudioMixerAuxBalance(tr("Balance"),"Balance",-40.0,0,40.0,0.5,DATATYPE_DB);
    list_third.append(third);

    third = new MenuThirdAudioMixerAuxInput(tr("Input"),"Input",-75.0,0,6.0,0.5,DATATYPE_DB);
    list_third.append(third);

    third = new MenuThirdAudioMixerAuxEnable(tr("Enable"),"Enable",0,0,2,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On") << tr("AFV");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    aux->third = list_third;
    return aux;
}

MenuSecond *Settings::new_PGM()
{
    MenuSecond *pgm = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    pgm->name = tr("pgm");
    pgm->ss_name = ("pgm");

    third = new MenuThirdAudioMixerPGMFader(tr("Fader"),"Fader",-75.0,0,10.0,0.5,DATATYPE_DB);
    list_third.append(third);

    pgm->third = list_third;
    return pgm;
}

MenuSecond *Settings::new_Monitor()
{
    MenuSecond *monitor = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    monitor->name = tr("monitor");
    monitor->ss_name = ("monitor");

    third = new MenuThirdAudioMixerMonitorLevel(tr("Level"),"Level",-31,-6,0,1,DATATYPE_DB_INT);
    list_third.append(third);

    third = new MenuThirdAudioMixerMonitorSource(tr("Source"),"Source",0,7,2,1,DATATYPE_ENUM);
    third->list_text << tr("In 1") << tr("In 2") << tr("In 3") << tr("In 4") << tr("Aux") << tr("Mic 1") << tr("Mic 2") << tr("PGM");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    monitor->third = list_third;
    return monitor;
}

MenuSecond *Settings::new_Still_Selection()
{
    MenuSecond *monitor = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    monitor->name = tr("still selection");
    monitor->ss_name = ("still selection");

    third = new MenuThirdStillSelectionStill1(tr("Still 1"),"Still 1",0,0,31,1,DATATYPE_NUMBER);
    list_third.append(third);

    third = new MenuThirdStillSelectionStill2(tr("Still 2"),"Still 2",0,1,31,1,DATATYPE_NUMBER);
    list_third.append(third);

    monitor->third = list_third;
    return monitor;
}

MenuSecond *Settings::new_Still_Upload()
{
    MenuSecond *monitor = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    monitor->name = tr("still upload");
    monitor->ss_name = ("still upload");

    third = new MenuThirdStillLocation(tr("Location"),"Location",0,0,31,1,DATATYPE_NUMBER);
    list_third.append(third);

    third = new MenuThirdStillLoadPicture(tr("Load picture"),"Load picture",0,0,0,1,DATATYPE_ICON);
    list_third.append(third);
    third->menuType = ENTER_CALL;

    monitor->third = list_third;
    return monitor;
}

MenuSecond *Settings::new_Macro_Sleep()
{
    MenuSecond *monitor = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    monitor->name = tr("macro");
    monitor->ss_name = ("macro");

    third = new MenuThirdMacroSleep(tr("Sleep"),"Sleep",0,500,10000,50,DATATYPE_MS);
    list_third.append(third);

    third = new MenuThirdMacroImport(tr("Import"),"Import",0,0,0,1,DATATYPE_ICON);
    list_third.append(third);

    third = new MenuThirdMacroExport(tr("Export"),"Export","","macro","","",DATATYPE_TEXT);
    list_third.append(third);

    monitor->third = list_third;
    return monitor;
}

MenuSecond *Settings::new_Stream1()
{
    MenuSecond *monitor = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    monitor->name = tr("stream 1");
    monitor->ss_name = ("stream 1");

    third = new MenuThirdStream1Platform(tr("Platform"),"Platform",0,0,1,1,DATATYPE_ENUM);
    for(auto it = streamData.begin();it != streamData.end();++it){
        third->list_text << it->first;
    }
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdStream1Server(tr("Server"),"Server",0,0,1,1,DATATYPE_ENUM);
    list_third.append(third);

    third = new MenuThirdStream1Key(tr("Key "),"Key ","","","","",DATATYPE_TEXT);
    list_third.append(third);
    third->menuType = EVENT_CALL;

    third = new MenuThirdStream1UploadKey(tr("Upload Key"),"Upload Key",0,0,0,1,DATATYPE_ICON); //DATATYPE_ENUM
    list_third.append(third);
    third->menuType = ENTER_CALL;

    third = new MenuThirdStream1Output(tr("Output"),"Output",0,1,2,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    monitor->third = list_third;
    return monitor;
}

MenuSecond *Settings::new_Stream2()
{
    MenuSecond *monitor = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    monitor->name = tr("stream 2");
    monitor->ss_name = ("stream 2");

    third = new MenuThirdStream2Platform(tr("Platform"),"Platform",0,0,1,1,DATATYPE_ENUM);
    for(auto it = streamData.begin();it != streamData.end();++it){
        third->list_text << it->first;
    }
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdStream2Server(tr("Server"),"Server",0,0,1,1,DATATYPE_ENUM);
    list_third.append(third);

    third = new MenuThirdStream2Key(tr("Key "),"Key ","","","","",DATATYPE_TEXT);
    list_third.append(third);
    third->menuType = EVENT_CALL;

    third = new MenuThirdStream2UploadKey(tr("Upload Key"),"Upload Key",0,0,0,1,DATATYPE_ICON);
    list_third.append(third);
    third->menuType = ENTER_CALL;

    third = new MenuThirdStream2Output(tr("Output"),"Output",0,0,2,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    monitor->third = list_third;
    return monitor;
}

MenuSecond *Settings::new_Stream3()
{
    MenuSecond *monitor = new MenuSecond;
    //这里的Stream 3后面必须加空格否则崩溃，具体原因不知道
    QList<MenuThird*>list_third;
    MenuThird *third;
    monitor->name = tr("stream 3 ");
    monitor->ss_name = ("stream 3 ");

    third = new MenuThirdStream3Platform(tr("Platform"),"Platform",0,0,1,1,DATATYPE_ENUM);
    for(auto it = streamData.begin();it != streamData.end();++it){
        third->list_text << it->first;
    }
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdStream3Server(tr("Server"),"Server",0,0,1,1,DATATYPE_ENUM);
    list_third.append(third);

    third = new MenuThirdStream3Key(tr("Key "),"Key ","","","","",DATATYPE_TEXT);
    list_third.append(third);
    third->menuType = EVENT_CALL;

    third = new MenuThirdStream3UploadKey(tr("Upload Key"),"Upload Key",0,0,0,1,DATATYPE_ICON);
    list_third.append(third);
    third->menuType = ENTER_CALL;

    third = new MenuThirdStream3Output(tr("Output"),"Output",0,0,2,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    monitor->third = list_third;
    return monitor;
}

MenuSecond *Settings::new_Playback_Set()
{
    MenuSecond *monitor = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    monitor->name = tr("playback");
    monitor->ss_name = ("playback");

    third = new MenuThirdPlaybackMode(tr("Playback Mode"),"Playback Mode",0,0,2,1,DATATYPE_ENUM);
    third->list_text << tr("Play Once") << tr("Repeat") << tr("Sequential");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    monitor->third = list_third;
    return monitor;
}

MenuSecond *Settings::new_Setting_Src_Name()
{
    MenuSecond *monitor = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    monitor->name = tr("src name");
    monitor->ss_name = ("src name");

    third = new MenuThirdSettingSrcNamePGM(tr("PGM"),"PGM","","Program","","",DATATYPE_TEXT);
    list_third.append(third);

    third = new MenuThirdSettingSrcNamePVW(tr("PVW"),"PVW","","Preview","","",DATATYPE_TEXT);
    list_third.append(third);

    third = new MenuThirdSettingSrcNameIn1(tr("In 1"),"In 1","","HDMI 1","","",DATATYPE_TEXT);
    list_third.append(third);

    third = new MenuThirdSettingSrcNameIn2(tr("In 2"),"In 2","","HDMI 2","","",DATATYPE_TEXT);
    list_third.append(third);

    third = new MenuThirdSettingSrcNameIn3(tr("In 3"),"In 3","","HDMI 3","","",DATATYPE_TEXT);
    list_third.append(third);

    third = new MenuThirdSettingSrcNameIn4(tr("In 4"),"In 4","","HDMI 4","","",DATATYPE_TEXT);
    list_third.append(third);

    third = new MenuThirdSettingSrcNameAux(tr("Aux"),"Aux","","Aux","","",DATATYPE_TEXT);
    list_third.append(third);

    third = new MenuThirdSettingSrcNameStill1(tr("Still 1"),"Still 1","","Still 1","","",DATATYPE_TEXT);
    list_third.append(third);

    third = new MenuThirdSettingSrcNameStill2(tr("Still 2"),"Still 2","","Still 2","","",DATATYPE_TEXT);
    list_third.append(third);

    monitor->third = list_third;
    return monitor;
}

MenuSecond *Settings::new_Setting_Mv_Meter()
{
    MenuSecond *monitor = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    monitor->name = tr("mv meter");
    monitor->ss_name = ("mv meter");

    third = new MenuThirdSettingMvMeterPGM(tr("PGM"),"PGM",0,0,2,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdSettingMvMeterIn1(tr("In 1"),"In 1",0,1,2,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdSettingMvMeterIn2(tr("In 2"),"In 2",0,1,2,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdSettingMvMeterIn3(tr("In 3"),"In 3",0,1,2,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdSettingMvMeterIn4(tr("In 4"),"In 4",0,1,2,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdSettingMvMeterAux(tr("Aux"),"Aux",0,1,2,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    monitor->third = list_third;
    return monitor;
}

MenuSecond *Settings::new_Setting_Mv_Layout()
{
    MenuSecond *monitor = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    monitor->name = tr("mv layout");
    monitor->ss_name = ("mv layout");

    third = new MenuThirdSettingMvLayout(tr("PGM/PVW SWAP"),"PGM/PVW SWAP",0,0,2,1,DATATYPE_ENUM);
    third->list_text << tr("PGM|PVW") << tr("PVW|PGM") ;
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    monitor->third = list_third;
    return monitor;
}

MenuSecond *Settings::new_Setting_Market()
{
    MenuSecond *monitor = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    monitor->name = tr("marker");
    monitor->ss_name = ("marker");

    third = new MenuThirdSettingMarkerPVW(tr("PVW"),"PVW",0,0,2,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    monitor->third = list_third;
    return monitor;
}

MenuSecond *Settings::new_Setting_Mic_Input()
{
    MenuSecond *monitor = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    monitor->name = tr("mic input");
    monitor->ss_name = ("mic input");

    third = new MenuThirdSettingMicInputMic1(tr("Mic 1 Input"),"Mic 1 Input",0,2,2,1,DATATYPE_ENUM);
    third->list_text << tr("Mic+power") << tr("Mic") << tr("Line") ;
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdSettingMicInputMic2(tr("Mic 2 Input"),"Mic 2 Input",0,2,2,1,DATATYPE_ENUM);
    third->list_text << tr("Mic+power") << tr("Mic") << tr("Line") ;
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    monitor->third = list_third;
    return monitor;
}

MenuSecond *Settings::new_Setting_Record_File()
{
    MenuSecond *monitor = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    monitor->name = tr("record file");
    monitor->ss_name = ("record file");

    third = new MenuThirdSettingRecordFileName(tr("File Name"),"File Name","","","","",DATATYPE_TEXT);
    list_third.append(third);

    monitor->third = list_third;
    return monitor;
}

MenuSecond *Settings::new_Setting_Aux_Source()
{
    MenuSecond *monitor = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    monitor->name = tr("src selection");
    monitor->ss_name = ("src selection");

    third = new MenuThirdSettingSrcSelectionIn1(tr("In 1"),"In 1",0,0,2,1,DATATYPE_ENUM);
    third->list_text << tr("Auto") << tr("RGB Full") << tr("RGB Limit") << tr("YCbCr422 Full") << tr("YCbCr422 Limit") << tr("YCbCr444 Full") << tr("YCbCr444 Limit");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdSettingSrcSelectionIn2(tr("In 2"),"In 2",0,0,2,1,DATATYPE_ENUM);
    third->list_text << tr("Auto") << tr("RGB Full") << tr("RGB Limit") << tr("YCbCr422 Full") << tr("YCbCr422 Limit") << tr("YCbCr444 Full") << tr("YCbCr444 Limit");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdSettingSrcSelectionIn3(tr("In 3"),"In 3",0,0,2,1,DATATYPE_ENUM);
    third->list_text << tr("Auto") << tr("RGB Full") << tr("RGB Limit") << tr("YCbCr422 Full") << tr("YCbCr422 Limit") << tr("YCbCr444 Full") << tr("YCbCr444 Limit");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdSettingSrcSelectionIn4(tr("In 4"),"In 4",0,0,2,1,DATATYPE_ENUM);
    third->list_text << tr("Auto") << tr("RGB Full") << tr("RGB Limit") << tr("YCbCr422 Full") << tr("YCbCr422 Limit") << tr("YCbCr444 Full") << tr("YCbCr444 Limit");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdSettingSrcSelectionAux(tr("Aux"),"Aux",0,0,2,1,DATATYPE_ENUM);
    third->list_text << tr("SD Card") << tr("USB Camera") << tr("NDI");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    monitor->third = list_third;
    return monitor;
}

MenuSecond *Settings::new_Setting_Version()
{
    MenuSecond *version = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    version->name = tr("version");
    version->ss_name = ("version");

    char * str = get_version();
    third = new MenuThirdSettingVersion(tr("Version"),"Version","","","","",DATATYPE_TEXT);
    third->current = QString(str);
    if(str)
    free(str);
    list_third.append(third);

    third = new MenuThirdSettingBuildInfo(tr("Build Info"),"Build Info","","","","",DATATYPE_TEXT);
    str = get_buildinfo();
    third->current = QString(str);
    if(str)
    free(str);
    list_third.append(third);

    third = new MenuThirdSettingDeviceId(tr("Device Id"),"Device Id","","","","",DATATYPE_TEXT);
    str = get_device_id();
    QString sn = QString(str);
    sn = sn.right(9);
    third->current = sn;
    if(str)
    free(str);
    list_third.append(third);

    version->third = list_third;
    return version;
}

MenuSecond *Settings::new_Setting_Out_Format()
{
    MenuSecond *out_format = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    out_format->name = tr("out format");
    out_format->ss_name = ("out format");

    third = new MenuThirdSettingOutFormat(tr("Format"),"Format",0,4,7,1,DATATYPE_ENUM);
    third->list_text << tr("1080p24") << tr("1080p25") << tr("1080p30") << tr("1080p50") << tr("1080p60");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdSettingOutFormatOut1ColorSpace(tr("Output 1 Color Space"),"Output 1 Color Space",0,0,7,1,DATATYPE_ENUM);
    third->list_text << tr("Auto") << tr("RGB Full") << tr("RGB Limit") << tr("YCbCr422 Full") << tr("YCbCr422 Limit") << tr("YCbCr444 Full") << tr("YCbCr444 Limit");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdSettingOutFormatOut2ColorSpace(tr("Output 2 Color Space"),"Output 2 Color Space",0,0,7,1,DATATYPE_ENUM);
    third->list_text << tr("Auto") << tr("RGB Full") << tr("RGB Limit") << tr("YCbCr422 Full") << tr("YCbCr422 Limit") << tr("YCbCr444 Full") << tr("YCbCr444 Limit");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    out_format->third = list_third;
    return out_format;
}

MenuSecond *Settings::new_Setting_Out_Source()
{
    MenuSecond *out_format = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    out_format->name = tr("out source");
    out_format->ss_name = ("out source");

    third = new MenuThirdSettingOutSourceHDMI1(tr("HDMI 1"),"HDMI 1",0,5,2,1,DATATYPE_ENUM);
    third->list_text << tr("In 1") << tr("In 2") << tr("In 3") << tr("In 4") << tr("Aux") << tr("PGM") << tr("PVW") << tr("Multiview");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdSettingOutSourceHDMI2(tr("HDMI 2"),"HDMI 2",0,0,0,1,DATATYPE_ENUM);
    third->list_text << tr("Multiview");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdSettingOutSourceUVC(tr("Streaming / Recording / UVC"),"Streaming / Recording / UVC",0,5,2,1,DATATYPE_ENUM);
    third->list_text << tr("In 1") << tr("In 2") << tr("In 3") << tr("In 4") << tr("Aux") << tr("PGM") << tr("PVW") << tr("Multiview");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    out_format->third = list_third;
    return out_format;
}

MenuSecond *Settings::new_Setting_Quality()
{
    MenuSecond *quality = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    quality->name = tr("quality");
    quality->ss_name = ("quality");

    third = new MenuThirdSettingQualityRecording(tr("Recording"),"Recording",0,2,2,1,DATATYPE_ENUM);
    third->list_text << tr("High") << tr("Medium") << tr("Low");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdSettingQualityStreaming(tr("Streaming"),"Streaming",0,2,2,1,DATATYPE_ENUM);
    third->list_text << tr("High") << tr("Medium") << tr("Low");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    quality->third = list_third;
    return quality;
}

MenuSecond *Settings::new_Setting_Network()
{
    MenuSecond *version = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    version->name = tr("network");
    version->ss_name = ("network");

    third = new MenuThirdSettingNetworkProtocol(tr("Protocol"),"Protocol",0,0,2,1,DATATYPE_ENUM);
    third->list_text << tr("DHCP") << tr("Static IP") ;
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdSettingNetworkIPAddress(tr("IP Address"),"IP Address","","","","",DATATYPE_TEXT);
    list_third.append(third);

    third = new MenuThirdSettingNetworkSubnetMask(tr("Subnet Mask"),"Subnet Mask","","","","",DATATYPE_TEXT);
    list_third.append(third);

    third = new MenuThirdSettingNetworkGateway(tr("Gateway"),"Gateway","","","","",DATATYPE_TEXT);
    list_third.append(third);

    third = new MenuThirdSettingNetworkPrimaryDNS(tr("Primary DNS"),"Primary DNS","","","","",DATATYPE_TEXT);
    list_third.append(third);

    third = new MenuThirdSettingNetworkSecondaryDNS(tr("Secondary DNS"),"Secondary DNS","","","","",DATATYPE_TEXT);
    list_third.append(third);

    version->third = list_third;
    return version;
}

MenuSecond *Settings::new_Setting_Panel()
{
    MenuSecond *panel = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    panel->name = tr("panel");
    panel->ss_name = ("panel");

    third = new MenuThirdSettingPanelButtonBrightness(tr("Button Brightness"),"Button Brightness",0,15,15,1,DATATYPE_NUMBER);
    list_third.append(third);

    panel->third = list_third;
    return panel;
}

MenuSecond *Settings::new_Setting_Language()
{
    MenuSecond *quality = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    quality->name = tr("language");
    quality->ss_name = ("language");

    third = new MenuThirdSettingLanguage(tr("Language"),"Language",0,0,2,1,DATATYPE_ENUM);
    third->list_text << tr("English") << tr("简体中文");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    quality->third = list_third;
    return quality;
}

MenuSecond *Settings::new_Setting_Reset()
{
    MenuSecond *reset = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    reset->name = tr("reset");
    reset->ss_name = ("reset");

    third = new MenuThirdSettingFactoryReset(tr("Factory Reset"),"Factory Reset","","","","",DATATYPE_TEXT);
    list_third.append(third);

    third = new MenuThirdSettingSDFormat(tr("SD Format"),"SD Format","","","","",DATATYPE_TEXT);
    list_third.append(third);

    reset->third = list_third;
    return reset;
}

QString Settings::getStreamUrlIndex(int index)
{
    QString platform,serverName,key,url;
    platform = listFirst()[MENU_FIRST_STREAM]->second[index]->third[MENU_THIRD_STREAM_PLATFORM]->getText();
    serverName = listFirst()[MENU_FIRST_STREAM]->second[index]->third[MENU_THIRD_STREAM_SERVER]->getText();
    key = listFirst()[MENU_FIRST_STREAM]->second[index]->third[MENU_THIRD_STREAM_KEY]->getText();
    QString server = "";
    auto service = streamData[platform];
    if(service.name_url.find(serverName) != service.name_url.end()){
        server = service.name_url[serverName];
    }
    if(server == "" || server.right(1) == "/"){
        url = server + key;
    }else{
        url = server + "/" + key;
    }
    return url;
}

STREAM_PROFILE Settings::getStreamBitrateIndex(int index)
{
    QString platform,quality,currentResolutionStr,currentFpsStr;
    int currentResolution = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_FORMAT]->third[SETTING_OUT_FORMAT_FORMAT]->current.toInt();
    if(currentResolution >= OUT_FORMAT_1080P24 && currentResolution <= OUT_FORMAT_1080P30)
    {
        currentResolutionStr = "1080p";
        currentFpsStr = "30";
    }
    else if(currentResolution >= OUT_FORMAT_1080P50 && currentResolution <= OUT_FORMAT_1080P60)
    {
        currentResolutionStr = "1080p";
        currentFpsStr = "60";
    }
    else
    {
        currentResolutionStr = "1080p";
        currentFpsStr = "60";
    }
    platform = listFirst()[MENU_FIRST_STREAM]->second[index]->third[MENU_THIRD_STREAM_PLATFORM]->getText();
    quality = listFirst()[MENU_FIRST_SETTING]->second[SETTING_QUALITY]->third[QUALITY_STREAMING]->getText();
    auto service = streamData[platform];
    auto list = service.name_config[quality];
    for(int i = 0;i < list.size();++i)
    {
        if(list[i].resolution == currentResolutionStr && list[i].fps == currentFpsStr)
        {
            return list[i];
        }
    }
}

STREAM_PROFILE Settings::getRecordBitrate()
{
    QString quality,currentResolutionStr,currentFpsStr;
    int currentResolution = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_FORMAT]->third[SETTING_OUT_FORMAT_FORMAT]->current.toInt();
    if(currentResolution >= OUT_FORMAT_1080P24 && currentResolution <= OUT_FORMAT_1080P30)
    {
        currentResolutionStr = "1080p";
        currentFpsStr = "30";
    }
    else if(currentResolution >= OUT_FORMAT_1080P50 && currentResolution <= OUT_FORMAT_1080P60)
    {
        currentResolutionStr = "1080p";
        currentFpsStr = "60";
    }
    else
    {
        currentResolutionStr = "1080p";
        currentFpsStr = "60";
    }
    quality = listFirst()[MENU_FIRST_SETTING]->second[SETTING_QUALITY]->third[QUALITY_RECORDING]->getText();
    auto list = recordData[quality];
    for(int i = 0;i < list.size();++i)
    {
        if(list[i].resolution == currentResolutionStr && list[i].fps == currentFpsStr)
        {
            return list[i];
        }
    }
}

QString Settings::platformChangedText(int index,int changeNumber)
{
    MenuThird *third = nullptr;
    if(index == Streams::STREAM1)
    {
        third = listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_PLATFORM];
    }
    else if(index == Streams::STREAM2)
    {
        third = listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM2]->third[MENU_THIRD_STREAM_PLATFORM];
    }
    else if(index == Streams::STREAM3)
    {
        third = listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM3]->third[MENU_THIRD_STREAM_PLATFORM];
    }

    if(third->list_text.size() == 0)
        return "";

    if(changeNumber >= third->max.toInt())
    {
        return third->list_text[third->list_text.size() - 1];
    }
    if(changeNumber < 0)
    {
        return third->list_text[0];
    }
    return third->list_text[changeNumber];
}

QString Settings::serverChangedText(int index, int changeNumber)
{
    MenuThird *third = nullptr;
    if(index == Streams::STREAM1)
    {
        third = listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_SERVER];
    }
    else if(index == Streams::STREAM2)
    {
        third = listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM2]->third[MENU_THIRD_STREAM_SERVER];
    }
    else if(index == Streams::STREAM3)
    {
        third = listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM3]->third[MENU_THIRD_STREAM_SERVER];
    }

    int size = third->list_text.size();
    if(size == 0)
        return "";

    qDebug() << "______:" << third->list_text.size();

    if(changeNumber >= size)
    {
        return third->list_text[size - 1];
    }
    if(changeNumber < 0)
    {
        return third->list_text[0];
    }
    return third->list_text[changeNumber];
}

void Settings::initStreamData()
{
    XmlProcessor::getInstance().readStreamingXml(streamData);
}

void Settings::initRecordData()
{
    XmlProcessor::getInstance().readRecordData(recordData);
}

void Settings::deviceid_change(QString sn)
{
    QString last9_sn = sn.right(9);
    setMenuValue(MENU_FIRST_SETTING, SETTING_VERSION, VERSION_DEVICE_ID, last9_sn);
}

MenuSecond *Settings::new_Color1()
{
    MenuSecond *color1 = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    color1->name = tr("color 1");
    color1->ss_name = ("color 1");

    third = new MenuThirdColorBackColor1Hue(tr("Hue"),"Hue",0,0,359,1,DATATYPE_NUMBER);
    list_third.append(third);

    third = new MenuThirdColorBackColor1Saturation(tr("Saturation"),"Saturation",0,0,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdColorBackColor1Brightness(tr("Brightness"),"Brightness",0,100,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    color1->third = list_third;
    return color1;
}

MenuSecond *Settings::new_Color2()
{
    MenuSecond *color2 = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    color2->name = tr("color 2");
    color2->ss_name = ("color 2");

    third = new MenuThirdColorBackColor2Hue(tr("Hue"),"Hue",0,45,359,1,DATATYPE_NUMBER);
    list_third.append(third);

    third = new MenuThirdColorBackColor2Saturation(tr("Saturation"),"Saturation",0,100,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdColorBackColor2Brightness(tr("Brightness"),"Brightness",0,100,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    color2->third = list_third;
    return color2;
}

MenuSecond *Settings::new_SuperSource_Enable()
{
    MenuSecond *enable = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    enable->name = tr("enable");
    enable->ss_name = ("enable");

    third = new MenuThirdSuperSourceEnable(tr("Enable"),"Enable",0,0,1,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    enable->third = list_third;
    return enable;
}

MenuSecond *Settings::new_SuperSource_Source()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("source");
    type->ss_name = ("source");

    third = new MenuThirdSuperSourceSource1(tr("Source 1"),"Source 1",0,0,3,1,DATATYPE_ENUM);
    third->list_text << tr("In 1") << tr("In 2") << tr("In 3") << tr("In 4") << tr("Aux") << tr("Still 1") << tr("Still 1 Key") << tr("Still 2") << tr("Still 2 Key") << tr("Color 1") << tr("Color 2") << tr("Color Bar") << tr("Black");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdSuperSourceSource2(tr("Source 2"),"Source 2",0,1,3,1,DATATYPE_ENUM);
    third->list_text << tr("In 1") << tr("In 2") << tr("In 3") << tr("In 4") << tr("Aux") << tr("Still 1") << tr("Still 1 Key") << tr("Still 2") << tr("Still 2 Key") << tr("Color 1") << tr("Color 2") << tr("Color Bar") << tr("Black");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdSuperSourceBackground(tr("Background"),"Background",0,5,3,1,DATATYPE_ENUM);
    third->list_text << tr("In 1") << tr("In 2") << tr("In 3") << tr("In 4") << tr("Aux") << tr("Still 1") << tr("Still 1 Key") << tr("Still 2") << tr("Still 2 Key") << tr("Color 1") << tr("Color 2") << tr("Color Bar") << tr("Black");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_SuperSource_Control()
{
    MenuSecond *control = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    control->name = tr("control");
    control->ss_name = ("control");

    third = new MenuThirdSuperSourceStyle(tr("Style"),"Style",0,0,0,1,DATATYPE_ENUM);
    third->list_text << tr("Zoom in") << tr("Crop-Zoom in") << tr("Zoom in-Crop") << tr("Crop") ;
    third->max = third->list_text.size() - 1;
    third->current = third->list_text.indexOf("Zoom in");
    list_third.append(third);

    third = new MenuThirdSuperSourceYPosition(tr("Y Position"),"Y Position",0,50,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);


    control->third = list_third;
    return control;
}

MenuSecond *Settings::new_SuperSource_Mask1()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("mask 1");
    type->ss_name = ("mask 1");

    third = new MenuThirdSuperSourceMask1Enable(tr("Enable"),"Enable",0,0,1,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdSuperSourceMask1HStart(tr("H Start"),"H Start",0,0,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdSuperSourceMask1VStart(tr("V Start"),"V Start",0,0,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdSuperSourceMask1HEnd(tr("H End"),"H End",0,100,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdSuperSourceMask1VEnd(tr("V End"),"V End",0,100,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_SuperSource_Mask2()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("mask 2");
    type->ss_name = ("mask 2");

    third = new MenuThirdSuperSourceMask2Enable(tr("Enable"),"Enable",0,0,1,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdSuperSourceMask2HStart(tr("H Start"),"H Start",0,0,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdSuperSourceMask2VStart(tr("V Start"),"V Start",0,0,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdSuperSourceMask2HEnd(tr("H End"),"H End",0,100,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdSuperSourceMask2VEnd(tr("V End"),"V End",0,100,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_SuperSource_Board1()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("border 1");
    type->ss_name = ("border 1");

    third = new MenuThirdSuperSourceBoard1(tr("Border"),"Border",0,0,10,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdSuperSourceBoardColor1Hue(tr("Hue"),"Hue",0,45,359,1,DATATYPE_NUMBER);
    list_third.append(third);

    third = new MenuThirdSuperSourceBoardColor1Saturation(tr("Saturation"),"Saturation",0,100,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdSuperSourceBoardColor1Brightness(tr("Brightness"),"Brightness",0,100,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_SuperSource_Board2()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("border 2");
    type->ss_name = ("border 2");

    third = new MenuThirdSuperSourceBoard2(tr("Border"),"Border",0,0,10,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdSuperSourceBoardColor2Hue(tr("Hue"),"Hue",0,45,359,1,DATATYPE_NUMBER);
    list_third.append(third);

    third = new MenuThirdSuperSourceBoardColor2Saturation(tr("Saturation"),"Saturation",0,100,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdSuperSourceBoardColor2Brightness(tr("Brightness"),"Brightness",0,100,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_type()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("type");
    type->ss_name = ("type");

    third = new MenuThirdKeyType(tr("Type"),"Type",0,0,3,1,DATATYPE_ENUM);
    third->list_text << tr("Luma Key") << tr("Chroma Key") << tr("Pattern") << tr("PIP");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_LumaKey_Source()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("source");
    type->ss_name = ("source");

    third = new MenuThirdLumaKeySourceFill(tr("Fill"),"Fill",0,7,3,1,DATATYPE_ENUM);
    third->list_text << tr("In 1") << tr("In 2") << tr("In 3") << tr("In 4") << tr("Aux") << tr("Still 1") << tr("Still 1 Key") << tr("Still 2") << tr("Still 2 Key") << tr("Color 1") << tr("Color 2") << tr("Color Bar") << tr("Black");
    third->max = third->list_text.size() - 1;
    third->current = third->list_text.indexOf("Still 2");
    list_third.append(third);

    third = new MenuThirdLumaKeySourceKey(tr("Key"),"Key",0,8,3,1,DATATYPE_ENUM);
    third->list_text << tr("In 1") << tr("In 2") << tr("In 3") << tr("In 4") << tr("Aux") << tr("Still 1") << tr("Still 1 Key") << tr("Still 2") << tr("Still 2 Key") /*<< tr("Color 1") << tr("Color 2") */<< tr("Color Bar") << tr("Black");
    third->max = third->list_text.size() - 1;
    third->current = third->list_text.indexOf("Still 2 Key");
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_LumaKey_Mask()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("mask");
    type->ss_name = ("mask");

    third = new MenuThirdLumaKeyMaskEnable(tr("Enable"),"Enable",0,0,1,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdLumaKeyMaskHStart(tr("H Start"),"H Start",0,0,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdLumaKeyMaskVStart(tr("V Start"),"V Start",0,0,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdLumaKeyMaskHEnd(tr("H End"),"H End",0,100,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdLumaKeyMaskVEnd(tr("V End"),"V End",0,100,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_LumaKey_Control()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("control");
    type->ss_name = ("control");

    third = new MenuThirdLumaKeyShapedKey(tr("Shaped Key"),"Shaped Key",0,0,1,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdLumaKeyClip(tr("Clip"),"Clip",0,15,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdLumaKeyGain(tr("Gain"),"Gain",0,50,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdLumaKeyInvert(tr("Invert"),"Invert",0,0,1,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_LumaKey_Resize()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("resize");
    type->ss_name = ("resize");

    third = new MenuThirdLumaKeyResize(tr("Resize"),"Resize",0,0,1,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdLumaKeySize(tr("Size"),"Size",0,1,0,1,DATATYPE_ENUM);
    third->list_text << tr("0.25") << tr("0.33") << tr("0.50");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdLumaKeyXPosition(tr("X Position"),"X Position",-16.0,10.6,16.0,0.2,DATATYPE_FLOAT);
    list_third.append(third);

    third = new MenuThirdLumaKeyYPosition(tr("Y Position"),"Y Position",-9.0,6,9.0,0.2,DATATYPE_FLOAT);
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_KeyPattern_Source()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("source");
    type->ss_name = ("source");

    third = new MenuThirdKeyPatternSourceFill(tr("Fill"),"Fill",0,0,3,1,DATATYPE_ENUM);
    third->list_text << tr("In 1") << tr("In 2") << tr("In 3") << tr("In 4") << tr("Aux") << tr("Still 1") << tr("Still 1 Key") << tr("Still 2") << tr("Still 2 Key") << tr("Color 1") << tr("Color 2") << tr("Color Bar") << tr("Black");
    third->max = third->list_text.size() - 1;
    third->current = third->list_text.indexOf("Still 2");
    list_third.append(third);

//    third = new MenuThirdKeyPatternSource(tr("Key"),0,0,3,1,DATATYPE_ENUM);
//    third->list_text << tr("In 1") << tr("In 2") << tr("In 3") << tr("In 4") << tr("Aux") << tr("Still 1") << tr("Still 1 Key") << tr("Still 2") << tr("Still 2 Key") << tr("Color 1") << tr("Color 2") << tr("Color Bar") << tr("Black");
//    third->max = third->list_text.size() - 1;
//    third->current = third->list_text.indexOf("Still 2 Key");
//    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_KeyPattern_Wipe()
{
#define PATTERN_PATH "/oem/hw_rockchip_qt/images/"
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("wipe");
    type->ss_name = ("wipe");

    third = new MenuThirdKeyPatternWipePattern(tr("Pattern"),"Pattern",0,0,15,1,DATATYPE_ICON);
    third->list_text << tr(PATTERN_PATH"icon0_normal.png") << tr(PATTERN_PATH"icon1_normal.png")
                     << tr(PATTERN_PATH"icon2_normal.png") << tr(PATTERN_PATH"icon3_normal.png")
                     << tr(PATTERN_PATH"icon4_normal.png") << tr(PATTERN_PATH"icon5_normal.png")
                     << tr(PATTERN_PATH"icon6_normal.png") << tr(PATTERN_PATH"icon7_normal.png")
                     << tr(PATTERN_PATH"icon8_normal.png") << tr(PATTERN_PATH"icon9_normal.png")
                     << tr(PATTERN_PATH"icon10_normal.png") << tr(PATTERN_PATH"icon11_normal.png")
                     << tr(PATTERN_PATH"icon12_normal.png") << tr(PATTERN_PATH"icon13_normal.png")
                     << tr(PATTERN_PATH"icon14_normal.png") << tr(PATTERN_PATH"icon15_normal.png")
                     << tr(PATTERN_PATH"icon16_normal.png") << tr(PATTERN_PATH"icon17_normal.png");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdKeyPatternWipeSize(tr("Size"),"Size",1,100,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdKeyPatternWipeXPosition(tr("X Position"),"X Position",-16.0,0,16.0,0.2,DATATYPE_FLOAT);
    list_third.append(third);

    third = new MenuThirdKeyPatternWipeYPosition(tr("Y Position"),"Y Position",-9.0,0,9.0,0.2,DATATYPE_FLOAT);
    list_third.append(third);

    third = new MenuThirdKeyPatternWipeSymmetry(tr("Symmetry"),"Symmetry",0,50,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdKeyPatternWipeSoftness(tr("Softness"),"Softness",0,0,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_KeyPattern_Mask()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("mask");
    type->ss_name = ("mask");

    third = new MenuThirdKeyPatternMaskEnable(tr("Enable"),"Enable",0,0,1,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdKeyPatternMaskHStart(tr("H Start"),"H Start",0,0,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdKeyPatternMaskVStart(tr("V Start"),"V Start",0,0,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdKeyPatternMaskHEnd(tr("H End"),"H End",0,100,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdKeyPatternMaskVEnd(tr("V End"),"V End",0,100,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_KeyPattern_Resize()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("resize");
    type->ss_name = ("resize");

    third = new MenuThirdKeyPatternResize(tr("Resize"),"Resize",0,0,1,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdKeyPatternSize(tr("Size"),"Size",0,1,0,1,DATATYPE_ENUM);
    third->list_text << tr("0.25") << tr("0.33") << tr("0.50");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdKeyPatternXPosition(tr("X Position"),"X Position",-16.0,10.6,16.0,0.2,DATATYPE_FLOAT);
    list_third.append(third);

    third = new MenuThirdKeyPatternYPosition(tr("Y Position"),"Y Position",-9.0,6,9.0,0.2,DATATYPE_FLOAT);
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_ChromaKey_Source()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("source");
    type->ss_name = ("source");

    third = new MenuThirdChromaKeySource(tr("Fill"),"Fill",0,3,3,1,DATATYPE_ENUM);
    third->list_text << tr("In 1") << tr("In 2") << tr("In 3") << tr("In 4") << tr("Aux") << tr("Still 1") << tr("Still 1 Key") << tr("Still 2") << tr("Still 2 Key") << tr("Color 1") << tr("Color 2") << tr("Color Bar") << tr("Black");
    third->max = third->list_text.size() - 1;
    third->current = third->list_text.indexOf("Still 2");
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_ChromaKey_Mask()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("mask");
    type->ss_name = ("mask");

    third = new MenuThirdChromaKeyMaskEnable(tr("Enable"),"Enable",0,0,1,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdChromaKeyMaskHStart(tr("H Start"),"H Start",0,0,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdChromaKeyMaskVStart(tr("V Start"),"V Start",0,0,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdChromaKeyMaskHEnd(tr("H End"),"H End",0,100,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdChromaKeyMaskVEnd(tr("V End"),"V End",0,100,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_ChromaKey_Resize()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("resize");
    type->ss_name = ("resize");

    third = new MenuThirdChromaKeyResize(tr("Resize"),"Resize",0,0,1,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdChromaKeySize(tr("Size"),"Size",0,1,0,1,DATATYPE_ENUM);
    third->list_text << tr("0.25") << tr("0.33") << tr("0.50");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdChromaKeyXPosition(tr("X Position"),"X Position",-16.0,10.6,16.0,0.2,DATATYPE_FLOAT);
    list_third.append(third);

    third = new MenuThirdChromaKeyYPosition(tr("Y Position"),"Y Position",-9.0,6,9.0,0.2,DATATYPE_FLOAT);
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_ChromaKey_Control()
{
    MenuSecond *control = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    control->name = tr("control");
    control->ss_name = ("control");

    third = new MenuThirdChromaKeySMPXPosition(tr("SMP X Position"),"SMP X Position",-16.0,-16.0,16.0,0.2,DATATYPE_FLOAT);
    list_third.append(third);

    third = new MenuThirdChromaKeySMPYPosition(tr("SMP Y Position"),"SMP Y Position",-9.0,-9.0,9.0,0.2,DATATYPE_FLOAT);
    list_third.append(third);

    third = new MenuThirdChromaKeySample(tr("Sample"),"Sample",0,0,1,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdChromaKeyForeground(tr("Foreground"),"Foreground",0,0,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdChromaKeyBackground(tr("Background"),"Background",0,0,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdChromaKeyKeyEdge(tr("KeyEdge"),"KeyEdge",0,50,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    control->third = list_third;
    return control;
}

MenuSecond *Settings::new_PIP_Source()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("source");
    type->ss_name = ("source");

    third = new MenuThirdPipSource(tr("PIP"),"PIP",0,0,7,1,DATATYPE_ENUM);
    third->list_text << tr("In 1") << tr("In 2") << tr("In 3") << tr("In 4") << tr("Aux") << tr("Still 1") << tr("Still 1 Key")
                     << tr("Still 2") << tr("Still 2 Key") << tr("Color 1") << tr("Color 2") << tr("Color Bar") << tr("Black");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_PIP_Size_Position()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("size/position");
    type->ss_name = ("size/position");

    third = new MenuThirdPipSize(tr("Pip size"),"Pip size",0,1,0,1,DATATYPE_ENUM);
    third->list_text << tr("0.25") << tr("0.33") << tr("0.50");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdPipXPosition(tr("X Position"),"X Position",-16.0,16.0,16.0,0.2,DATATYPE_FLOAT);
    list_third.append(third);

    third = new MenuThirdPipYPosition(tr("Y Position"),"Y Position",-9.0,9.0,9.0,0.2,DATATYPE_FLOAT);
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_PIP_Mask()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("mask");
    type->ss_name = ("mask");

    third = new MenuThirdPipMaskEnable(tr("Enable"),"Enable",0,0,1,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdPipMaskHStart(tr("H Start"),"H Start",0,0,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdPipMaskVStart(tr("V Start"),"V Start",0,0,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdPipMaskHEnd(tr("H End"),"H End",0,100,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdPipMaskVEnd(tr("V End"),"V End",0,100,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_PIP_Board()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("border");
    type->ss_name = ("border");

    third = new MenuThirdPipBorderEnable(tr("Enable"),"Enable",0,0,1,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdPipBorderWidth(tr("Width"),"Width",0,0,31,1,DATATYPE_NUMBER);
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_PIP_Color()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("color");
    type->ss_name = ("color");

    third = new MenuThirdPipBorderColorHue(tr("Hue"),"Hue",0,0,359,1,DATATYPE_NUMBER);
    list_third.append(third);

    third = new MenuThirdPipBorderColorSaturation(tr("Saturation"),"Saturation",0,100,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdPipBorderColorBrightness(tr("Brightness"),"Brightness",0,100,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_DSK_Source()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("source");
    type->ss_name = ("source");

    third = new MenuThirdDSKSourceFill(tr("Fill"),"Fill",0,7,7,1,DATATYPE_ENUM);
    third->list_text << tr("In 1") << tr("In 2") << tr("In 3") << tr("In 4") << tr("Aux") << tr("Still 1") << tr("Still 1 Key")
                     << tr("Still 2") << tr("Still 2 Key") << tr("Color 1") << tr("Color 2") << tr("Color Bar") << tr("Black");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdDSKSourceKey(tr("Key"),"Key",0,8,8,1,DATATYPE_ENUM);
    third->list_text << tr("In 1") << tr("In 2") << tr("In 3") << tr("In 4") << tr("Aux") << tr("Still 1") << tr("Still 1 Key")
                     << tr("Still 2") << tr("Still 2 Key")/* << tr("Color 1") << tr("Color 2")*/ << tr("Color Bar") << tr("Black");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_DSK_Mask()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("mask");
    type->ss_name = ("mask");

    third = new MenuThirdDSKMaskEnable(tr("Enable"),"Enable",0,0,1,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdDSKMaskHStart(tr("H Start"),"H Start",0,0,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdDSKMaskVStart(tr("V Start"),"V Start",0,0,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdDSKMaskHEnd(tr("H End"),"H End",0,100,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdDSKMaskVEnd(tr("V End"),"V End",0,100,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_DSK_Control()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("control");
    type->ss_name = ("control");

    third = new MenuThirdDSKShapedKey(tr("Shaped Key"),"Shaped Key",0,0,1,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdDSKClip(tr("Clip"),"Clip",0,15,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdDSKMaskGain(tr("Gain"),"Gain",0,50,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdDSKInvert(tr("Invert"),"Invert",0,0,1,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_DSK_Rate()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("rate");
    type->ss_name = ("rate");

    third = new MenuThirdDSKRate(tr("Rate"),"Rate",0.5,1.0,8.0,0.5,DATATYPE_FLOAT);
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_FTB_Rate()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("rate");
    type->ss_name = ("rate");

    third = new MenuThirdFTBRate(tr("Rate"),"Rate",0.5,1.0,8.0,0.5,DATATYPE_FLOAT);
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_FTB_Audio()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("audio");
    type->ss_name = ("audio");

    third = new MenuThirdFTBAfv(tr("AFV"),"AFV",0,1,1,1,DATATYPE_ENUM);
    third->list_text << tr("Off") << tr("On");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_Transition_Mix()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("mix");
    type->ss_name = ("mix");

    third = new MenuThirdTransitionMixRate(tr("Rate"),"Rate",0.5,1.0,8.0,0.5,DATATYPE_FLOAT);
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_Transition_Dip()
{
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("dip");
    type->ss_name = ("dip");

    third = new MenuThirdTransitionDipSource(tr("Source"),"Source",0,8,9,1,DATATYPE_ENUM);
    third->list_text << tr("In 1") << tr("In 2") << tr("In 3") << tr("In 4") << tr("Aux") << tr("Black") << tr("Still 1")
                     << tr("Still 2") << tr("Color 1") << tr("Color 2") << tr("Color Bar");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdTransitionDipRate(tr("Rate"),"Rate",0.5,1.0,8.0,0.5,DATATYPE_FLOAT);
    list_third.append(third);

    type->third = list_third;
    return type;
}

MenuSecond *Settings::new_Transition_Wipe()
{
#define PATTERN_PATH "/oem/hw_rockchip_qt/images/"
    MenuSecond *type = new MenuSecond;
    QList<MenuThird*>list_third;
    MenuThird *third;
    type->name = tr("wipe");
    type->ss_name = ("wipe");

    third = new MenuThirdTransitionWipePattern(tr("Pattern"),"Pattern",0,0,15,1,DATATYPE_ICON);
    third->list_text << tr(PATTERN_PATH"icon0_normal.png") << tr(PATTERN_PATH"icon1_normal.png")
                     << tr(PATTERN_PATH"icon2_normal.png") << tr(PATTERN_PATH"icon3_normal.png")
                     << tr(PATTERN_PATH"icon4_normal.png") << tr(PATTERN_PATH"icon5_normal.png")
                     << tr(PATTERN_PATH"icon6_normal.png") << tr(PATTERN_PATH"icon7_normal.png")
                     << tr(PATTERN_PATH"icon8_normal.png") << tr(PATTERN_PATH"icon9_normal.png")
                     << tr(PATTERN_PATH"icon10_normal.png") << tr(PATTERN_PATH"icon11_normal.png")
                     << tr(PATTERN_PATH"icon12_normal.png") << tr(PATTERN_PATH"icon13_normal.png")
                     << tr(PATTERN_PATH"icon14_normal.png") << tr(PATTERN_PATH"icon15_normal.png")
                     << tr(PATTERN_PATH"icon16_normal.png") << tr(PATTERN_PATH"icon17_normal.png");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdTransitionWipeRate(tr("Rate"),"Rate",0.5,1.0,8.0,0.5,DATATYPE_FLOAT);
    list_third.append(third);

    third = new MenuThirdTransitionWipeXPosition(tr("X Position"),"X Position",-16.0,0,16.0,0.2,DATATYPE_FLOAT);
    list_third.append(third);

    third = new MenuThirdTransitionWipeYPosition(tr("Y Position"),"Y Position",-9.0,0,9.0,0.2,DATATYPE_FLOAT);
    list_third.append(third);

    third = new MenuThirdTransitionWipeDirection(tr("Direction"),"Direction",0,0,1,1,DATATYPE_ENUM);
    third->list_text << tr("Normal") << tr("Inverse");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    third = new MenuThirdTransitionWipeSymmetry(tr("Symmetry"),"Symmetry",0,50,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

//    third = new MenuThirdTransitionWipePosition(tr("Symmetry Of Circle"),0,81,100,1,DATATYPE_PERCENTAGE);
//    list_third.append(third);

    third = new MenuThirdTransitionWipeSoftness(tr("Softness"),"Softness",0,0,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    third = new MenuThirdTransitionWipeBorder(tr("Border"),"Border",0,0,100,1,DATATYPE_PERCENTAGE);
    list_third.append(third);

    //名字后面必须加空格（不能为“Fill Source”）否则崩溃，暂时无解
    third = new MenuThirdTransitionWipeFillSource(tr("Fill Source "),"Fill Source ",0,8,1,1,DATATYPE_ENUM);
    third->list_text << tr("In 1") << tr("In 2") << tr("In 3") << tr("In 4") << tr("Aux") << tr("Black") << tr("Still 1")
                     << tr("Still 2") << tr("Color 1") << tr("Color 2") << tr("Color Bar");
    third->max = third->list_text.size() - 1;
    list_third.append(third);

    type->third = list_third;
    return type;
}

void Settings::init_menuData()
{
    //必须先new再初始化数据
    new_menuData();

//    for(int i = 0;i < m_listFirst.size();++i)
//    {
//        for(int j = 0;j < m_listFirst[i]->second.size();++j)
//        {
//            for(int k = 0;k < m_listFirst[i]->second[j]->third.size();++k)
//            {
//                auto var = getIniValue(m_listFirst[i]->ss_name,m_listFirst[i]->second[j]->ss_name,m_listFirst[i]->second[j]->third[k]->ss_name);
//                if(!var.isNull())
//                    m_listFirst[i]->second[j]->third[k]->current = var;
//            }
//        }
//    }
}

void Settings::init_keyData()
{
    for(int i = KEY_LED_AUDIO_MIC1;i <=KEY_LED_AUDIO_PGM;++i){
        m_listKey[i] = new KeyAudioInput(i);
    }
    m_listKey[KEY_LED_AUDIO_AFV] = new KeyAudioAfv(KEY_LED_AUDIO_AFV);
    m_listKey[KEY_LED_AUDIO_ON] = new KeyAudioOn(KEY_LED_AUDIO_ON);
    for(int i = KEY_LED_MEM1;i <=KEY_LED_MEM8;++i){
        m_listKey[i] = new KeyMem(i);
    }
    m_listKey[KEY_LED_RECORDER_REC] = new KeyRecordStart(KEY_LED_RECORDER_REC);
    m_listKey[KEY_LED_RECORDER_STOP] = new KeyRecordStop(KEY_LED_RECORDER_STOP);
    m_listKey[KEY_LED_PLAYER_PLAY] = new KeyPlayStart(KEY_LED_PLAYER_PLAY);
    m_listKey[KEY_LED_PLAYER_PAUSE] = new KeyPlayStop(KEY_LED_PLAYER_PAUSE);
    m_listKey[KEY_LED_PLAYER_PREV] = new KeyPlayPrevious(KEY_LED_PLAYER_PREV);
    m_listKey[KEY_LED_PLAYER_NEXT] = new KeyPlayNext(KEY_LED_PLAYER_NEXT);
    m_listKey[KEY_LED_LIVE] = new KeyLiveStream(KEY_LED_LIVE);

    m_listKey[KEY_LED_MEMU] = new KeyLedMenu(KEY_LED_MEMU);
    m_listKey[KEY_MENU_ENTER] = new KeyMenuEnter(KEY_MENU_ENTER);


    for(int i = KEY_LED_PGM_1;i <=KEY_LED_PGM_STLL2;++i){
        m_listKey[i] = new KeyPgm(i);
    }

    for(int i = KEY_LED_PVW_1;i <=KEY_LED_PVW_STLL2;++i){
        m_listKey[i] = new KeyPvw(i);
    }

    m_listKey[KEY_LED_TRANS_CUT] = new KeyCut(KEY_LED_TRANS_CUT);
    m_listKey[KEY_LED_TRANS_AUTO] = new KeyAuto(KEY_LED_TRANS_AUTO);
    m_listKey[KEY_LED_TRANS_FTB] = new KeyFtb(KEY_LED_TRANS_FTB);
    m_listKey[KEY_LED_TRANS_PREVIEW] = new KeyPrev(KEY_LED_TRANS_PREVIEW);
    for(int i = KEY_LED_TRANS_MIX;i <=KEY_LED_TRANS_WIPE;++i){
        m_listKey[i] = new KeyTransition(i);
    }
    m_listKey[KEY_LED_KEY] = new KeyKey(KEY_LED_KEY);
    m_listKey[KEY_LED_DSK] = new KeyKey(KEY_LED_DSK);
    m_listKey[KEY_LED_BKGD] = new KeyKey(KEY_LED_BKGD);
    m_listKey[KEY_LED_KEY_ON_AIR] = new KeyKeyOnAir(KEY_LED_KEY_ON_AIR);
    m_listKey[KEY_LED_DSK_ON_AIR] = new KeyDskOnAir(KEY_LED_DSK_ON_AIR);

    m_listKey[KEY_AUDIO_ENTER] = new keyAudioEnter(KEY_AUDIO_ENTER);
}

void Settings::init_PgmTallyTimer()
{
#define PGMTALLY_FLUSH_FREQUENCY 100
    static QTimer *pgmTallyTimer = nullptr;
    if(!pgmTallyTimer)
    {
        pgmTallyTimer = new QTimer;
        connect(pgmTallyTimer,&QTimer::timeout,this,[=](){
            if(models != nullptr)
                setPgmTally(models->getPGMTally());
        });
        pgmTallyTimer->start(PGMTALLY_FLUSH_FREQUENCY);
    }
}

QVariant Settings::getIniValue(int first,int second,int third)
{
    QString filepath = DATA_PATH "/" + m_listFirst[first]->ss_name + ".ini";

    QString query = m_listFirst[first]->second[second]->ss_name + "/" + m_listFirst[first]->second[second]->third[third]->ss_name;

    QSettings set(filepath,QSettings::IniFormat);
    return set.value(query);
}

void Settings::setIniValue(int first,int second,int third)
{
    QString firstName = m_listFirst[first]->ss_name;
    firstName = firstName.replace('/','_');

    QString filepath = DATA_PATH "/" + firstName + ".ini";
    qDebug() << filepath;
    QString query = m_listFirst[first]->second[second]->ss_name + "/" + m_listFirst[first]->second[second]->third[third]->ss_name;
    QVariant var = m_listFirst[first]->second[second]->third[third]->current;

    QSettings set(filepath, QSettings::IniFormat);
    qDebug() << "set init value -------    query = " << query << "   var = " << var;
    set.setValue(query, var);
    set.sync();

    //断电无法保存，c语言方式再写一次
    QFile file(filepath);
    file.open(QIODevice::ReadOnly);
    QByteArray ba = file.readAll();
    qDebug() << ba;
    file.close();

    FILE *fp;
    fp = fopen(filepath.toUtf8().data(), "wt+");
    fwrite(ba, 1, ba.size(), fp);
    fflush(fp);
    fsync(fileno(fp));
    fclose(fp);
}

void Settings::setIniValue(QString filepath, QString key, QVariant value)
{
    QSettings set(filepath, QSettings::IniFormat);
    set.setValue(key, value);
    set.sync();

    //断电无法保存，c语言方式再写一次
    QFile file(filepath);
    file.open(QIODevice::ReadOnly);
    QByteArray ba = file.readAll();
    qDebug() << ba;
    file.close();

    FILE *fp;
    fp = fopen(filepath.toUtf8().data(), "wt+");
    fwrite(ba, 1, ba.size(), fp);
    fflush(fp);
    fsync(fileno(fp));
    fclose(fp);
}

QVariant Settings::getIniValue(QString fileName, QString first, QString second)
{
    QString filepath = DATA_PATH "/" + fileName + ".ini";
    qDebug() << filepath;

    QString query =first + "/" + second;
    qDebug() << query;

    QSettings set(filepath,QSettings::IniFormat);
    return set.value(query);
}

QVariant Settings::getIniValue(QString filepath, QString key)
{
    QSettings set(filepath,QSettings::IniFormat);
    return set.value(key);
}

void Settings::setMenuDHCPNetwork(QString ip, QString mask, QString gateway, QString primary_dns, QString seconday_dns)
{
    m_listFirst[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_IP_ADDRESS]->setValue(ip);
    m_listFirst[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_SUBNET_MASK]->setValue(mask);
    m_listFirst[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_GATEWAY]->setValue(gateway);
    m_listFirst[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_PRIMARY_DNS]->setValue(primary_dns);
    m_listFirst[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_SECONDAY_DNS]->setValue(seconday_dns);

    emit thirdMenuValueChanged(MENU_FIRST_SETTING,SETTING_NETWORK,NETWORK_IP_ADDRESS);
    emit thirdMenuValueChanged(MENU_FIRST_SETTING,SETTING_NETWORK,NETWORK_SUBNET_MASK);
    emit thirdMenuValueChanged(MENU_FIRST_SETTING,SETTING_NETWORK,NETWORK_GATEWAY);
    emit thirdMenuValueChanged(MENU_FIRST_SETTING,SETTING_NETWORK,NETWORK_PRIMARY_DNS);
    emit thirdMenuValueChanged(MENU_FIRST_SETTING,SETTING_NETWORK,NETWORK_SECONDAY_DNS);
}

void Settings::setMenuDHCPNetworkIpAddress(QString ip)
{
    m_listFirst[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_IP_ADDRESS]->setValue(ip);
    emit thirdMenuValueChanged(MENU_FIRST_SETTING,SETTING_NETWORK,NETWORK_IP_ADDRESS);
}

void Settings::setMenuDHCPNetworkMask(QString mask)
{
    m_listFirst[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_SUBNET_MASK]->setValue(mask);
    emit thirdMenuValueChanged(MENU_FIRST_SETTING,SETTING_NETWORK,NETWORK_SUBNET_MASK);
}

void Settings::setMenuDHCPNetworkGateway(QString gateway)
{
    m_listFirst[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_GATEWAY]->setValue(gateway);
    emit thirdMenuValueChanged(MENU_FIRST_SETTING,SETTING_NETWORK,NETWORK_GATEWAY);
}

void Settings::setMenuDHCPNetworkPrimaryDNS(QString dns)
{
    m_listFirst[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_PRIMARY_DNS]->setValue(dns);
    emit thirdMenuValueChanged(MENU_FIRST_SETTING,SETTING_NETWORK,NETWORK_PRIMARY_DNS);
}

void Settings::setMenuDHCPNetworksecondayDNS(QString dns)
{
    m_listFirst[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_SECONDAY_DNS]->setValue(dns);
    emit thirdMenuValueChanged(MENU_FIRST_SETTING,SETTING_NETWORK,NETWORK_SECONDAY_DNS);
}

void Settings::setMenuValue(int first, int second, int third, QVariant value)
{
    if(!m_listFirst[first]->second[second]->third[third]->setValue(value))
    {
        emit thirdMenuValueChanged(first,second,third);
    }
}

QVariant Settings::getMenuValue(int first, int second, int third)
{
    return m_listFirst[first]->second[second]->third[third]->current;
}

void Settings::addMenuValue()
{
    if(!m_listFirst[lastFirstUnfold()]->second[lastSecondUnfold()]->third[lastThirdUnfold()]->addValue())
    {
        setIniValue(lastFirstUnfold(),lastSecondUnfold(),lastThirdUnfold());
        emit thirdMenuValueChanged(lastFirstUnfold(),lastSecondUnfold(),lastThirdUnfold());
    }
}

void Settings::subMenuValue()
{
    if(!m_listFirst[lastFirstUnfold()]->second[lastSecondUnfold()]->third[lastThirdUnfold()]->subValue())
    {
        setIniValue(lastFirstUnfold(),lastSecondUnfold(),lastThirdUnfold());
        emit thirdMenuValueChanged(lastFirstUnfold(),lastSecondUnfold(),lastThirdUnfold());
    }
}

void Settings::addMenuValue(int first, int second, int third)
{
    if(!m_listFirst[first]->second[second]->third[third]->addValue())
    {
        setIniValue(first,second,third);
        emit thirdMenuValueChanged(first,second,third);
    }
}

void Settings::subMenuValue(int first, int second, int third)
{
    if(!m_listFirst[first]->second[second]->third[third]->subValue())
    {
        setIniValue(first,second,third);
        emit thirdMenuValueChanged(first,second,third);
    }
}

void Settings::sdImagesChanged(QList<QString> list)
{
    auto item = m_listFirst[MENU_FIRST_STILL_GENERATOR]->second[STILL_GENERATE_UPLOAD]->third[STILL_UPLOAD_LOAD_PICTURE];
    int size = list.size();
    if(size == 0){
        item->current = 0;
        item->max = 0;
        item->list_text.clear();
    }else{
        for(int i = 0;i < size;++i){
            item->list_text.append(list[i]);
            item->max = item->list_text.size() - 1;
            item->current = 0;
        }
    }
    thirdMenuValueChanged(MENU_FIRST_STILL_GENERATOR,STILL_GENERATE_UPLOAD,STILL_UPLOAD_LOAD_PICTURE);
}

void Settings::streamKeyListChanged(QList<QString> list)
{
    for(int i = STREAM_STREAM1;i < STREAM_MAX;++i)
    {
        auto item = m_listFirst[MENU_FIRST_STREAM]->second[i]->third[MENU_THIRD_STREAM_UPLOAD_KEY];
        int size = list.size();
        if(size == 0){
            item->current = 0;
            item->max = 0;
            item->list_text.clear();
        }else{
            for(int i = 0;i < size;++i){
                item->list_text.append(list[i]);
                item->max = item->list_text.size() - 1;
                item->current = 0;
            }
        }
        thirdMenuValueChanged(MENU_FIRST_STREAM,i,MENU_THIRD_STREAM_UPLOAD_KEY);
    }
}

void Settings::macroListChanged(QList<QString> list)
{
    auto item = m_listFirst[MENU_FIRST_MACRO]->second[MACRO_MACRO]->third[MENU_THIRD_MACRO_IMPORT];
    int size = list.size();
    item->list_text.clear();
    if(size == 0){
        item->current = 0;
        item->max = 0;
    }else{
        for(int i = 0;i < size;++i){
            item->list_text.append(list[i]);
            item->max = item->list_text.size() - 1;
            item->current = 0;
        }
    }
    thirdMenuValueChanged(MENU_FIRST_MACRO,MACRO_MACRO,MENU_THIRD_MACRO_IMPORT);
}
