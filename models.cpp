﻿#include <QDebug>
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
#include "KeyImplement.h"
#include "playbackgroupmanager.h"

extern Settings *settings;
extern LeftMenuModel *leftMenuModel;
extern RightMenuModel *rightMenuModel;
extern QSwitcher *switcher;
extern struct fpga_t g_fpga;
extern MyProvider *myProvider;
extern Media_sd *media_sd;
extern bool init_settings_is_ok;
extern Profile *profile;
extern PlaybackGroupManager* playbackGroupManager;

Models::Models(QObject *parent) : QObject(parent)
{
    init_connect();
    init_signals();
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
    connect(this,&Models::keyOnAir,this,&Models::setKeyOnAir);

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
    connect(this,&Models::chromaKeySampleXPosition,this,&Models::setChromaKeySampleXPosition);
    connect(this,&Models::chromaKeySampleYPosition,this,&Models::setChromaKeySampleYPosition);
    connect(this,&Models::chromaKeySample,this,&Models::setChromaKeySample);
    connect(this,&Models::chromaKeyForeground,this,&Models::setChromaKeyForeground);
    connect(this,&Models::chromaKeyBackground,this,&Models::setChromaKeyBackground);
    connect(this,&Models::chromaKeyKeyEdge,this,&Models::setChromaKeyKeyEdge);

    //key pattern
    connect(this,&Models::keyPatternWipePattern,this,&Models::setKeyPatternWipePattern);
    connect(this,&Models::keyPatternWipeSize,this,&Models::setKeyPatternWipeSize);
    connect(this,&Models::keyPatternWipeXPosition,this,&Models::setKeyPatternWipeXPosition);
    connect(this,&Models::keyPatternWipeYPosition,this,&Models::setKeyPatternWipeYPosition);
    connect(this,&Models::keyPatternWipeSymmetry,this,&Models::setKeyPatternWipeSymmetry);
    connect(this,&Models::keyPatternWipeSoftness,this,&Models::setKeyPatternWipeSoftness);

    //pip
    connect(this,&Models::pipBorderEnable,this,&Models::setPipBorderEnable);
    connect(this,&Models::pipBorderWidth,this,&Models::setPipBorderWidth);
    connect(this,&Models::pipBorderColorHue,this,&Models::setPipBorderColorHue);
    connect(this,&Models::pipBorderColorSaturation,this,&Models::setPipBorderColorSaturation);
    connect(this,&Models::pipBorderColorBrightness,this,&Models::setPipBorderColorBrightness);


    //transition
    connect(this,&Models::transitionRate,this,&Models::setTransitionRate);
    connect(this,&Models::transitionDipSource,this,&Models::setTransitionDipSource);
    connect(this,&Models::transitionWipePattern,this,&Models::setTransitionWipePattern);
    connect(this,&Models::transitionWipeXPosition,this,&Models::setTransitionWipeXPosition);
    connect(this,&Models::transitionWipeYPosition,this,&Models::setTransitionWipeYPosition);
    connect(this,&Models::transitionWipeDirection,this,&Models::setTransitionWipeDirection);
    connect(this,&Models::transitionWipeSymmetry,this,&Models::setTransitionWipeSymmetry);
    connect(this,&Models::transitionWipeSoftness,this,&Models::setTransitionWipeSoftness);
    connect(this,&Models::transitionWipeBorder,this,&Models::setTransitionWipeBorder);
    connect(this,&Models::transitionWipeFillSource,this,&Models::setTransitionWipeFillSource);

    //dsk
    connect(this,&Models::dskOnAir,this,&Models::setDskOnAir);
    connect(this,&Models::dskSourceFill,this,&Models::setDskSourceFill);
    connect(this,&Models::dskSourceKey,this,&Models::setDskSourceKey);
    connect(this,&Models::dskMaskEnable,this,&Models::setDskMaskEnable);
    connect(this,&Models::dskMaskHStart,this,&Models::setDskMaskHStart);
    connect(this,&Models::dskMaskVStart,this,&Models::setDskMaskVStart);
    connect(this,&Models::dskMaskHEnd,this,&Models::setDskMaskHEnd);
    connect(this,&Models::dskMaskVEnd,this,&Models::setDskMaskVEnd);
    connect(this,&Models::dskShapedKey,this,&Models::setDskShapedKey);
    connect(this,&Models::dskClip,this,&Models::setDskClip);
    connect(this,&Models::dskGain,this,&Models::setDskGain);
    connect(this,&Models::dskInvert,this,&Models::setDskInvert);
    connect(this,&Models::dskRate,this,&Models::setDskRate);

    //ftb
    connect(this,&Models::ftbEnable,this,&Models::setFtbEnable);
    connect(this,&Models::ftbRate,this,&Models::setFtbRate);
    connect(this,&Models::ftbAfv,this,&Models::setFtbAfv);

    //audio mixer
    connect(this,&Models::audioEnable,this,&Models::setAudioEnable);
    connect(this,&Models::audioFader,this,&Models::setAudioFader);
    connect(this,&Models::audioBalance,this,&Models::setAudioBalance);
    connect(this,&Models::audioInput,this,&Models::setAudioInput);
    connect(this,&Models::audioDelay,this,&Models::setAudioDelay);
    connect(this,&Models::monitorLevel,this,&Models::setMonitorLevel);
    connect(this,&Models::monitorSource,this,&Models::setMonitorSource);


    //still generator
    connect(this,&Models::stillSelection,this,&Models::setStillSelection);
    connect(this,&Models::stillLocation,this,&Models::setStillLocation);

    //macro


    //streams
    connect(this,&Models::streamPlatform,this,&Models::setStreamPlatform);
    connect(this,&Models::streamEnable,this,&Models::setStreamEnable);
    connect(this,&Models::streamServer,this,&Models::setStreamServer);
    connect(this,&Models::streamUrl,this,&Models::setStreamUrl);
    connect(this,&Models::streamOutput,this,&Models::setStreamOutput);

    //playback
    connect(this,&Models::playbackList,this,&Models::setPlaybackList);
    connect(this,&Models::playbackSequential,this,&Models::setPlaybackSequential);
    connect(this,&Models::playbackProgressBar,this,&Models::setPlaybackProgressBar);

    //setting
    connect(this,&Models::srcName,this,&Models::setSrcName);
    connect(this,&Models::mvMeter,this,&Models::setMvMeter);
    connect(this,&Models::mvLayout,this,&Models::setMvLayout);
    connect(this,&Models::marker,this,&Models::setMarker);
    connect(this,&Models::micInput,this,&Models::setMicInput);
    connect(this,&Models::recordFileName,this,&Models::setRecordFileName);
    connect(this,&Models::srcSelection,this,&Models::setSrcSelection);
    connect(this,&Models::outFormat,this,&Models::setOutFormat);
    connect(this,&Models::outputColorSpace,this,&Models::setOutputColorSpace);
    connect(this,&Models::outSource,this,&Models::setOutSource);
    connect(this,&Models::quality,this,&Models::setQuality);
    connect(this,&Models::buttonBrightness,this,&Models::setButtonBrightness);
    connect(this,&Models::protocol,this,&Models::setProtocol);
    connect(this,&Models::iPAddress,this,&Models::setIPAddress);
    connect(this,&Models::subnetMask,this,&Models::setSubnetMask);
    connect(this,&Models::gateway,this,&Models::setGateway);
    connect(this,&Models::primaryDNS,this,&Models::setPrimaryDNS);
    connect(this,&Models::seconddaryDNS,this,&Models::setSeconddaryDNS);
    connect(this,&Models::language,this,&Models::setLanguage);

    //button status
    connect(this,&Models::pgmIndex,this,&Models::setPgmIndex);
    connect(this,&Models::pvwIndex,this,&Models::setPvwIndex);
    connect(this,&Models::nextTransition,this,&Models::setNextTransition);
    connect(this,&Models::transitionStyle,this,&Models::setTransitionStyle);
    connect(this,&Models::previewTransition,this,&Models::setPreviewTransition);
    connect(this,&Models::cutTransition,this,&Models::setCutTransition);
    connect(this,&Models::autoTransition,this,&Models::setAutoTransition);

}

void Models::init_signals()
{
    auto metaObj = this->metaObject();
    for(int i = 0;i < metaObj->methodCount();++i)
    {
        if(metaObj->method(i).methodType() == QMetaMethod::Signal)
        {
            m_signals[metaObj->method(i).name()] = metaObj->method(i);
        }
    }
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
    bool pgm = profile->setting()->mvMeters()->pgm()->enable();
    bool in1 = profile->setting()->mvMeters()->in1()->enable();
    bool in2 = profile->setting()->mvMeters()->in2()->enable();
    bool in3 = profile->setting()->mvMeters()->in3()->enable();
    bool in4 = profile->setting()->mvMeters()->in4()->enable();
    bool aux = profile->setting()->mvMeters()->aux()->enable();

    int value = 0;
    in1?value += 1:value += 0;
    in2?value += 2:value += 0;
    in3?value += 4:value += 0;
//    in4 == MENU_ON?value += 8:value += 0;
    aux?value += 16:value += 0;
    pgm?value += 32:value += 0;
    if(visible != 0 && in4){
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
    for(int i = KEY_LED_AUDIO_MIC1;i <= KEY_LED_AUDIO_PGM;++i)
    {
        if(QSwitcher::get_led(i))
        {
            QSwitcher::set_led(KEY_LED_AUDIO_MIC1 + i,SWITCHER_LED_OFF);
            break;
        }
    }
    QSwitcher::set_led(KEY_LED_AUDIO_AFV,SWITCHER_LED_OFF);
    QSwitcher::set_led(KEY_LED_AUDIO_ON,SWITCHER_LED_OFF);
    if(index != -1)
    {
        QSwitcher::set_led(KEY_LED_AUDIO_MIC1 + index,SWITCHER_LED_ON);

        int value = AUDIO_OFF;
        if(index >= AUDIO_MIXER_MIC1 && index <= AUDIO_MIXER_MIC2){
            value = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[index]->third[MIC1_ENABLE]->current.toInt();
            if(value == AUDIO_ON)
                QSwitcher::set_led(KEY_LED_AUDIO_ON,SWITCHER_LED_ON);
        }
        else if(index >= AUDIO_MIXER_IN1 && index <= AUDIO_MIXER_AUX){
            value = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[index]->third[IN1_ENABLE]->current.toInt();
            if(value == AUDIO_ON)
                QSwitcher::set_led(KEY_LED_AUDIO_ON,SWITCHER_LED_ON);
            else if(value == AUDIO_AFV)
                QSwitcher::set_led(KEY_LED_AUDIO_AFV,SWITCHER_LED_ON);
        }
    }
}

void Models::menuAudioEnableChnaged(int second,int third)
{
    int value = AudioSource::OFF;
    if(second >= AUDIO_MIXER_MIC1 && second <= AUDIO_MIXER_MIC2 && (third == MIC1_ENABLE)){
        value = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[second]->third[third]->current.toInt();
        if(value == AudioSource::ON)
            QSwitcher::set_led(KEY_LED_AUDIO_ON,SWITCHER_LED_ON);
        else
            QSwitcher::set_led(KEY_LED_AUDIO_ON,SWITCHER_LED_OFF);
    }else if(second >= AUDIO_MIXER_IN1 && second <= AUDIO_MIXER_AUX && third == IN1_ENABLE){
        value = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[second]->third[third]->current.toInt();
        if(value == AUDIO_ON){
            QSwitcher::set_led(KEY_LED_AUDIO_ON,SWITCHER_LED_ON);
            QSwitcher::set_led(KEY_LED_AUDIO_AFV,SWITCHER_LED_OFF);
        }else if(value == AUDIO_AFV){
            QSwitcher::set_led(KEY_LED_AUDIO_ON,SWITCHER_LED_OFF);
            QSwitcher::set_led(KEY_LED_AUDIO_AFV,SWITCHER_LED_ON);
        }else{
            QSwitcher::set_led(KEY_LED_AUDIO_ON,SWITCHER_LED_OFF);
            QSwitcher::set_led(KEY_LED_AUDIO_AFV,SWITCHER_LED_OFF);
        }
    }
}

void Models::setMonitorSource(QString source)
{
    int value = AudioSource::sourceNameStringToIndex(source);
    if(profile->audioMixer()->audioOutput()->monitor()->source() != value)
    {
        profile->audioMixer()->audioOutput()->monitor()->setSource(value);
        return ;
    }
    int index = AudioSource::sourceNameStringToIndex(source);
    fpga_write(&g_fpga,MON_SOURCE,index);
}

void Models::setAudioVolumn(int index)
{
    //这里有菜单的每一级index来查找需要修改的音频，后续PC发起时，需要将菜单的选项先设置好，否则出错
    int fpga_left = -1;
    int fpga_right = -1;
    double input,fader,balance,left,right;
    int left_value,right_value;
    AudioSource *source = nullptr;
    switch (index) {
    case AudioSource::MIC1:
        //mic 1
        fpga_left = FPGA_MIC1_L_LEV;
        fpga_right = FPGA_MIC1_R_LEV;
        source = profile->audioMixer()->audioInput()->mic1();
        break;
    case AudioSource::MIC2:
        //mic 2
        fpga_left = FPGA_MIC2_L_LEV;
        fpga_right = FPGA_MIC2_R_LEV;
        source = profile->audioMixer()->audioInput()->mic2();
        break;
    case AudioSource::IN1:
        //in 1
        fpga_left = FPGA_IN1_L_LEV;
        fpga_right = FPGA_IN1_R_LEV;
        source = profile->audioMixer()->audioInput()->in1();
        break;
    case AudioSource::IN2:
        //in 2
        fpga_left = FPGA_IN2_L_LEV;
        fpga_right = FPGA_IN2_R_LEV;
        source = profile->audioMixer()->audioInput()->in2();
        break;
    case AudioSource::IN3:
        //in 3
        fpga_left = FPGA_IN3_L_LEV;
        fpga_right = FPGA_IN3_R_LEV;
        source = profile->audioMixer()->audioInput()->in2();
        break;
    case AudioSource::IN4:
        //in 4
        fpga_left = FPGA_IN4_L_LEV;
        fpga_right = FPGA_IN4_R_LEV;
        source = profile->audioMixer()->audioInput()->in3();
        break;
    case AudioSource::AUX:
        //aux
        fpga_left = FPGA_AUX_L_LEV;
        fpga_right = FPGA_AUX_R_LEV;
        source = profile->audioMixer()->audioInput()->in4();
        break;
    case AudioSource::PGM:
        //pgm
        fpga_left = FPGA_MASTER_LEV;
        source = profile->audioMixer()->audioOutput()->pgm();
        break;
    }

    fader = source->fader();
    balance = source->balance();
    input = source->input();

    if(index == AudioSource::MIC1 || index == AudioSource::MIC2)
        input = 0;

    left = input + fader;
    right = input + fader;
    balance < 0?left += balance:right -= balance;
    if(index == AudioSource::PGM)
        left = fader;

    left_value = getAudioRegValue(left);
    right_value = getAudioRegValue(right);

    if(fpga_left != -1)
        fpga_write(&g_fpga,fpga_left,left_value);
    if(fpga_right != -1)
        fpga_write(&g_fpga,fpga_right,right_value);

    QString sFader;
    if(fader >= 0){
        sFader += '+';
        if(fader < 10)
            sFader += '0';
    }
    sFader += QString::number(dround(fader,3),'f',2);
    if(sFader.indexOf('-') != -1){
        sFader.replace('-',"\u2212");
        if(fader > -10)
            sFader.insert(1,'0');
    }
    settings->setAudioNumbers(index,sFader);
}

void Models::setAudioOn()
{
    int flag = 0;

    int mic1 = profile->audioMixer()->audioInput()->mic1()->enable();
    int mic2 = profile->audioMixer()->audioInput()->mic2()->enable();
    int in1 = profile->audioMixer()->audioInput()->in1()->enable();
    int in2 = profile->audioMixer()->audioInput()->in2()->enable();
    int in3 = profile->audioMixer()->audioInput()->in3()->enable();
    int in4 = profile->audioMixer()->audioInput()->in4()->enable();
    int aux = profile->audioMixer()->audioInput()->aux()->enable();

    if(in1 == AUDIO_ON)
       flag += 1;
    if(in2 == AUDIO_ON)
       flag += 2;
    if(in3 == AUDIO_ON)
       flag += 4;
    if(in4 == AUDIO_ON)
       flag += 8;
    if(aux == AUDIO_ON)
       flag += 16;
    if(mic1 == AUDIO_ON)
       flag += 32;
    if(mic2 == AUDIO_ON)
       flag += 64;

    settings->setAudioOnFlag(flag);

    fpga_write(&g_fpga,FPGA_AUDIO_ON,flag);
}

void Models::setAudioAfv()
{
    int flag = 0;

    int in1 = profile->audioMixer()->audioInput()->in1()->enable();
    int in2 = profile->audioMixer()->audioInput()->in2()->enable();
    int in3 = profile->audioMixer()->audioInput()->in3()->enable();
    int in4 = profile->audioMixer()->audioInput()->in4()->enable();
    int aux = profile->audioMixer()->audioInput()->aux()->enable();

    if(in1 == AUDIO_AFV)
       flag += 1;
    if(in2 == AUDIO_AFV)
       flag += 2;
    if(in3 == AUDIO_AFV)
       flag += 4;
    if(in4 == AUDIO_AFV)
       flag += 8;
    if(aux == AUDIO_AFV)
       flag += 16;

    settings->setAudioAfvFlag(flag);

    fpga_write(&g_fpga,FPGA_AUDIO_AFV,flag);
}

void Models::addAudioFaderByAudioKnob(QString source, int value)
{
    double fader,val,step;
    int index = AudioSource::sourceNameStringToIndex(source);
    switch (index) {
    case AudioSource::MIC1:
        fader = profile->audioMixer()->audioInput()->mic1()->fader();
        step = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_MIC1]->third[MIC1_FADER]->step.toDouble();
        break;
    case AudioSource::MIC2:
        fader = profile->audioMixer()->audioInput()->mic2()->fader();
        step = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_MIC2]->third[MIC2_FADER]->step.toDouble();
        break;
    case AudioSource::IN1:
        fader = profile->audioMixer()->audioInput()->in1()->fader();
        step = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_IN1]->third[IN1_FADER]->step.toDouble();
        break;
    case AudioSource::IN2:
        fader = profile->audioMixer()->audioInput()->in2()->fader();
        step = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_IN2]->third[IN2_FADER]->step.toDouble();
        break;
    case AudioSource::IN3:
        fader = profile->audioMixer()->audioInput()->in3()->fader();
        step = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_IN3]->third[IN3_FADER]->step.toDouble();
        break;
    case AudioSource::IN4:
        fader = profile->audioMixer()->audioInput()->in4()->fader();
        step = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_IN4]->third[IN4_FADER]->step.toDouble();
        break;
    case AudioSource::AUX:
        fader = profile->audioMixer()->audioInput()->aux()->fader();
        step = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_AUX]->third[AUX_FADER]->step.toDouble();
        break;
    case AudioSource::PGM:
        fader = profile->audioMixer()->audioOutput()->pgm()->fader();
        step = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_PGM]->third[PGM_FADER]->step.toDouble();
        break;
    }
    val = fader + value * step;
    setAudioFader(source,val);
}

void Models::addMonitorLevelByAudioKnob(int value)
{
    int step = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[AUDIO_MIXER_MONITOR]->third[MONITOR_LEVEL]->step.toDouble();
    int level = profile->audioMixer()->audioOutput()->monitor()->level();
    int val = level + value * step;
    setMonitorLevel(val);
}

void Models::setStillSelection(int still, int selection)
{
    if(still == StillGenerator::STILL1)
    {
        if(profile->stillGenerator()->stillSelection()->still1() != selection)
        {
            profile->stillGenerator()->stillSelection()->setStill1(selection);
            return ;
        }
    }
    else if(still == StillGenerator::STILL2)
    {
        if(profile->stillGenerator()->stillSelection()->still2() != selection)
        {
            profile->stillGenerator()->stillSelection()->setStill2(selection);
            return ;
        }
    }
    setStillSelectionCtrl();
}

void Models::setStillLocation(int index)
{
    if(profile->stillGenerator()->stillSelection()->location() != index)
    {
        profile->stillGenerator()->stillSelection()->setLocation(index);
        return ;
    }
    setStillSelectionCtrl();
}

void Models::setStillSelectionCtrl()
{
    int value;
    int still1 = profile->stillGenerator()->stillSelection()->still1();
    int still2 = profile->stillGenerator()->stillSelection()->still2();
    int location_index = profile->stillGenerator()->stillSelection()->location();
    location_index = location_index << 10;
    still2 = still2 << 5;
    value = still1 + still2 + location_index;
    fpga_write(&g_fpga,STILL_SEL,value);
}

void Models::setStreamEnable(bool enable)
{
    if(profile->streams()->enable() != enable)
    {
        profile->streams()->setEnable(enable);
        return ;
    }
    if(enable)
        openAllStream();
    else
        closeAllStream();
}

void Models::setStreamPlatform(int streamIndex, QString platform)
{
    switch (streamIndex) {
    case Streams::STREAM1:
        if(settings->liveStreamStatus1() != E_STATUS_CLOSE)
            return ;
        if(profile->streams()->stream1()->platfrom() != platform)
        {
            profile->streams()->stream1()->setPlatfrom(platform);
            return ;
        }
        updateServerArray(streamIndex);
        settings->setMenuValue(MENU_FIRST_STREAM,STREAM_STREAM1,MENU_THIRD_STREAM_PLATFORM,platform);
        setStreamServer(streamIndex,profile->streams()->stream1()->server());
        break;
    case Streams::STREAM2:
        if(settings->liveStreamStatus2() != E_STATUS_CLOSE)
            return ;
        if(profile->streams()->stream2()->platfrom() != platform)
        {
            profile->streams()->stream2()->setPlatfrom(platform);
            return ;
        }
        updateServerArray(streamIndex);
        settings->setMenuValue(MENU_FIRST_STREAM,STREAM_STREAM2,MENU_THIRD_STREAM_PLATFORM,platform);
        setStreamServer(streamIndex,profile->streams()->stream2()->server());
        break;
    case Streams::STREAM3:
        if(settings->liveStreamStatus3() != E_STATUS_CLOSE)
            return ;
        if(profile->streams()->stream3()->platfrom() != platform)
        {
            profile->streams()->stream3()->setPlatfrom(platform);
            return ;
        }
        updateServerArray(streamIndex);
        settings->setMenuValue(MENU_FIRST_STREAM,STREAM_STREAM3,MENU_THIRD_STREAM_PLATFORM,platform);
        setStreamServer(streamIndex,profile->streams()->stream3()->server());
        break;
    }
}

void Models::setStreamServer(int streamIndex, QString server)
{
    qDebug() << "streamIndex :" << streamIndex;
    int index = -1;
    switch (streamIndex) {
    case Streams::STREAM1:
        if(settings->liveStreamStatus1() != E_STATUS_CLOSE)
            return ;
        index = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_SERVER]->list_text.indexOf(server);
        if(index == -1)
            server = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_SERVER]->list_text[0];
        else
            server = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_SERVER]->list_text[index];
        if(profile->streams()->stream1()->server() != server)
        {
            profile->streams()->stream1()->setServer(server);
            return ;
        }
        settings->setMenuValue(MENU_FIRST_STREAM,STREAM_STREAM1,MENU_THIRD_STREAM_SERVER,server);
        break;
    case Streams::STREAM2:
        if(settings->liveStreamStatus2() != E_STATUS_CLOSE)
            return ;
        index = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM2]->third[MENU_THIRD_STREAM_SERVER]->list_text.indexOf(server);
        if(index == -1)
            server = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM2]->third[MENU_THIRD_STREAM_SERVER]->list_text[0];
        else
            server = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM2]->third[MENU_THIRD_STREAM_SERVER]->list_text[index];
        if(profile->streams()->stream2()->server() != server)
        {
            profile->streams()->stream2()->setServer(server);
            return ;
        }
        settings->setMenuValue(MENU_FIRST_STREAM,STREAM_STREAM2,MENU_THIRD_STREAM_SERVER,server);
        break;
    case Streams::STREAM3:
        if(settings->liveStreamStatus3() != E_STATUS_CLOSE)
            return ;
        index = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM3]->third[MENU_THIRD_STREAM_SERVER]->list_text.indexOf(server);
        if(index == -1)
            server = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM3]->third[MENU_THIRD_STREAM_SERVER]->list_text[0];
        else
            server = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM3]->third[MENU_THIRD_STREAM_SERVER]->list_text[index];
        if(profile->streams()->stream3()->server() != server)
        {
            profile->streams()->stream3()->setServer(server);
            return ;
        }
        settings->setMenuValue(MENU_FIRST_STREAM,STREAM_STREAM3,MENU_THIRD_STREAM_SERVER,server);
        break;
    }
}

void Models::setStreamUrl(int streamIndex, QString url)
{
    switch (streamIndex) {
    case Streams::STREAM1:
        if(settings->liveStreamStatus1() != E_STATUS_CLOSE)
            return ;
        if(profile->streams()->stream1()->url() != url)
        {
            profile->streams()->stream1()->setUrl(url);
            return ;
        }
        break;
    case Streams::STREAM2:
        if(settings->liveStreamStatus2() != E_STATUS_CLOSE)
            return ;
        if(profile->streams()->stream2()->url() != url)
        {
            profile->streams()->stream2()->setUrl(url);
            return ;
        }
        break;
    case Streams::STREAM3:
        if(settings->liveStreamStatus3() != E_STATUS_CLOSE)
            return ;
        if(profile->streams()->stream3()->url() != url)
        {
            profile->streams()->stream3()->setUrl(url);
            return ;
        }
        break;
    }
}

void Models::setStreamOutput(int streamIndex, bool output)
{
    switch (streamIndex) {
    case Streams::STREAM1:
        if(profile->streams()->stream1()->output() != output)
        {
            profile->streams()->stream1()->setOutput(output);
            return ;
        }
        break;
    case Streams::STREAM2:
        if(profile->streams()->stream2()->output() != output)
        {
            profile->streams()->stream2()->setOutput(output);
            return ;
        }
        break;
    case Streams::STREAM3:
        if(profile->streams()->stream3()->output() != output)
        {
            profile->streams()->stream3()->setOutput(output);
            return ;
        }
        break;
    }

    if(profile->streams()->enable())
    {
        if(output)
            openOneStream(streamIndex);
        else
            closeOneStream(streamIndex);
    }

    bool output1 = profile->streams()->stream1()->output();
    bool output2 = profile->streams()->stream2()->output();
    bool output3 = profile->streams()->stream3()->output();
    settings->setStreamOutputList({output1,output2,output3});
}

void Models::updateServerArray(int streamIndex)
{
    QString platform ,server;
    MenuThird *third = nullptr;
    Stream *stream = nullptr;
    if(streamIndex == Streams::STREAM1)
    {
        third = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_SERVER];
        platform = profile->streams()->stream1()->platfrom();
        server = profile->streams()->stream1()->server();
        stream = profile->streams()->stream1();
    }
    else if(streamIndex == Streams::STREAM2)
    {
        third = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM2]->third[MENU_THIRD_STREAM_SERVER];
        platform = profile->streams()->stream2()->platfrom();
        server = profile->streams()->stream2()->server();
        stream = profile->streams()->stream2();
    }
    else if(streamIndex == Streams::STREAM3)
    {
        third = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM3]->third[MENU_THIRD_STREAM_SERVER];
        platform = profile->streams()->stream3()->platfrom();
        server = profile->streams()->stream3()->server();
        stream = profile->streams()->stream3();
    }

    if(third == nullptr)
        return ;

    auto servers = settings->streamData[platform].name_url;
    (third)->list_text.clear();
    for(auto it = servers.begin();it != servers.end(); it++)
    {
        (third)->list_text << it->first;
    }
    (third)->max = (third)->list_text.size() - 1;
    //    stream->setServer(server);
}

void Models::setPlaybackList(QString list)
{
    int index = Playback::playbackListStringToIndex(list);
    if(profile->playback()->playbackList() != index)
    {
        profile->playback()->setPlaybackList(index);
        return ;
    }
}

void Models::setPlaybackSequential(bool sequential)
{
    if(profile->playback()->sequential() != sequential)
    {
        profile->playback()->setSequential(sequential);
        return ;
    }
    rv_switch_mp4_replay(sequential);
}

void Models::setPlaybackProgressBar(bool progressBar)
{
    if(profile->playback()->progressBar() != progressBar)
    {
        profile->playback()->setProgressBar(progressBar);
        return ;
    }
}

void Models::setSrcName(QString srcName, QString name)
{   
    int index = SrcNames::srcNameStringToIndex(srcName);
    std::string s_text = name.toUtf8().toStdString();
    auto s_name = s_text.data();
    struct umd_config_t cfg = {
        .font = FONT_PATH,
        .font_size = 20,

        .italic = 0,
        .bold = 0,
        .align = ALIGN_CENTER,

        .width = 160,
        .height = 40,
    };

    if(index > SrcNames::PVW){
        cfg.width = 128;
        cfg.height = 30;
        cfg.font_size = 16;
    }

    int fpga_add = 0;
    switch (index) {
    case SrcNames::PGM:
        if(profile->setting()->srcNames()->pgm()->name() != name)
        {
            profile->setting()->srcNames()->pgm()->setName(name);
            return ;
        }
        fpga_add = UMD_PGM;
        break;
    case SrcNames::PVW:
        if(profile->setting()->srcNames()->pvw()->name() != name)
        {
            profile->setting()->srcNames()->pvw()->setName(name);
            return ;
        }
        fpga_add = UMD_PVW;
        break;
    case SrcNames::IN1:
        if(profile->setting()->srcNames()->in1()->name() != name)
        {
            profile->setting()->srcNames()->in1()->setName(name);
            return ;
        }
        fpga_add = UMD_IN1;
        break;
    case SrcNames::IN2:
        if(profile->setting()->srcNames()->in2()->name() != name)
        {
            profile->setting()->srcNames()->in2()->setName(name);
            return ;
        }
        fpga_add = UMD_IN2;
        break;
    case SrcNames::IN3:
        if(profile->setting()->srcNames()->in3()->name() != name)
        {
            profile->setting()->srcNames()->in3()->setName(name);
            return ;
        }
        fpga_add = UMD_IN3;
        break;
    case SrcNames::IN4:
        if(profile->setting()->srcNames()->in4()->name() != name)
        {
            profile->setting()->srcNames()->in4()->setName(name);
            return ;
        }
        fpga_add = UMD_IN4;
        break;
    case SrcNames::AUX:
        if(profile->setting()->srcNames()->aux()->name() != name)
        {
            profile->setting()->srcNames()->aux()->setName(name);
            return ;
        }
        fpga_add = UMD_AUX;
        break;
    case SrcNames::STILL1:
        if(profile->setting()->srcNames()->still1()->name() != name)
        {
            profile->setting()->srcNames()->still1()->setName(name);
            return ;
        }
        fpga_add = UMD_STILL1;
        break;
    case SrcNames::STILL2:
        if(profile->setting()->srcNames()->still2()->name() != name)
        {
            profile->setting()->srcNames()->still2()->setName(name);
            return ;
        }
        fpga_add = UMD_STILL2;
        break;
    default:
        return ;
    }
    str2umd(s_name,strlen(s_name),&cfg);
    fpga_write_buffer(&g_fpga,fpga_add,(uint8_t*)cfg.buffer,cfg.bufsize);
    free(cfg.buffer);
}

void Models::setMvMeter(QString src, bool enable)
{
    int index = MvMeters::mvMeterStringToIndex(src);
    switch (index) {
    case MvMeters::PGM:
        if(profile->setting()->mvMeters()->pgm()->enable() != enable)
        {
            profile->setting()->mvMeters()->pgm()->setEnable(enable);
            return ;
        }
        break;
    case MvMeters::IN1:
        if(profile->setting()->mvMeters()->in1()->enable() != enable)
        {
            profile->setting()->mvMeters()->in1()->setEnable(enable);
            return ;
        }
        break;
    case MvMeters::IN2:
        if(profile->setting()->mvMeters()->in2()->enable() != enable)
        {
            profile->setting()->mvMeters()->in2()->setEnable(enable);
            return ;
        }
        break;
    case MvMeters::IN3:
        if(profile->setting()->mvMeters()->in3()->enable() != enable)
        {
            profile->setting()->mvMeters()->in3()->setEnable(enable);
            return ;
        }
        break;
    case MvMeters::IN4:
        if(profile->setting()->mvMeters()->in4()->enable() != enable)
        {
            profile->setting()->mvMeters()->in4()->setEnable(enable);
            return ;
        }
        break;
    case MvMeters::AUX:
        if(profile->setting()->mvMeters()->aux()->enable() != enable)
        {
            profile->setting()->mvMeters()->aux()->setEnable(enable);
            return ;
        }
        break;
    }
    bool pgm = profile->setting()->mvMeters()->pgm()->enable();
    bool in1 = profile->setting()->mvMeters()->in1()->enable();
    bool in2 = profile->setting()->mvMeters()->in2()->enable();
    bool in3 = profile->setting()->mvMeters()->in3()->enable();
    bool in4 = profile->setting()->mvMeters()->in4()->enable();
    bool aux = profile->setting()->mvMeters()->aux()->enable();

    int value = 0;
    in1?value += 1:value += 0;
    in2?value += 2:value += 0;
    in3?value += 4:value += 0;
    in4?value += 8:value += 0;
    aux?value += 16:value += 0;
    pgm?value += 32:value += 0;
    fpga_write(&g_fpga,AUDIO_MET_DISP,value);
}

void Models::setMvLayout(QString layout)
{
    int index = MvLayout::mvLayoutStringToIndex(layout);
    if(profile->setting()->mvLayout()->layout() != index)
    {
        profile->setting()->mvLayout()->setLayout(index);
        return ;
    }
    setPVWCtrl();
}

void Models::setMarker(bool pvwMarker)
{
    if(profile->setting()->marker()->pvwMarker() != pvwMarker)
    {
        profile->setting()->marker()->setPvwMarker(pvwMarker);
        return ;
    }

    setPVWCtrl();
}

void Models::setPVWCtrl()
{
    int pvw_pos = profile->setting()->mvLayout()->layout();
    bool market = profile->setting()->marker()->pvwMarker();

    int value = 0;
    pvw_pos == MvLayout::PGM_PVW?value += 2:value += 0;
    market ?value += 1:value += 0;

    fpga_write(&g_fpga,PVW_CTRL,value);
}

void Models::setMicInput(QString src, QString input)
{
    int index = MicInputs::srcNameToIndex(src);
    int value = MicInputs::micInputStringToIndex(input);
    if(index == MicInputs::MIC1)
    {
        if(profile->setting()->micInputs()->mic1()->input() != input)
        {
            profile->setting()->micInputs()->mic1()->setInput(value);
            return ;
        }
        rv_switch_set_mic0(value);
    }
    else if(index == MicInputs::MIC2)
    {
        if(profile->setting()->micInputs()->mic2()->input() != input)
        {
            profile->setting()->micInputs()->mic2()->setInput(value);
            return ;
        }
        rv_switch_set_mic1(value);
    }
}

void Models::setRecordFileName(QString fileName)
{
    if(profile->setting()->record()->fileName() != fileName)
    {
        profile->setting()->record()->setFileName(fileName);
        return ;
    }

}

void Models::setSrcSelection(QString src, QString selection)
{
    int index = SrcSelections::sourceStringToIndex(src);
    int int_selection = SrcSelections::selectionStringToIndex(selection);
    switch (index) {
    case SrcSelections::IN1:
        if(profile->setting()->srcSelections()->in1()->selection() != int_selection)
        {
            profile->setting()->srcSelections()->in1()->setSelection(int_selection);
            return ;
        }
        break;
    case SrcSelections::IN2:
        if(profile->setting()->srcSelections()->in2()->selection() != int_selection)
        {
            profile->setting()->srcSelections()->in2()->setSelection(int_selection);
            return ;
        }
        break;
    case SrcSelections::IN3:
        if(profile->setting()->srcSelections()->in3()->selection() != int_selection)
        {
            profile->setting()->srcSelections()->in3()->setSelection(int_selection);
            return ;
        }
        break;
    case SrcSelections::IN4:
        if(profile->setting()->srcSelections()->in4()->selection() != int_selection)
        {
            profile->setting()->srcSelections()->in4()->setSelection(int_selection);
            return ;
        }
        break;
    case SrcSelections::AUX:
        if(profile->setting()->srcSelections()->aux()->selection() != int_selection)
        {
            profile->setting()->srcSelections()->aux()->setSelection(int_selection);
            return ;
        }
        break;
    }

    if(index < SrcSelections::AUX)
    {
        set_hdmi_in_colorspace(index,(hdmi_in_colorspace_t)int_selection);
    }
    else if(index == SrcSelections::AUX)
    {
        if(int_selection == SrcSelections::NDI && get_ndi_license_state())
        {
            messageDialogControl->dialogShow(QObject::tr("No NDI license, please purchase license at www.osee-tech.com."),{QObject::tr("Cancel")},MessageDialogControl::MESSAGE_SD_FORMAT);
            profile->setting()->srcSelections()->aux()->setSelection(SrcSelections::SD_CARD);
        }
        else
        {
            switch (int_selection) {
            case SrcSelections::SD_CARD:
                rv_switch_usb_camera_stop0(0,0);
                osee_ndi_deinit();
                if(playbackGroupManager->list().size() != 0 && settings->playLedStatus() == E_STATUS_MP4_CLOSE){
                    settings->setPlayingIndex(0);
                    playPause(1);
                    playStart();
                }
                break;
            case SrcSelections::USB:
                playStop();
                osee_ndi_deinit();
                rv_switch_usb_camera_start0(0,0);
                break;
            case SrcSelections::NDI:
                playStop();
                rv_switch_usb_camera_stop0(0,0);
                if(ndi->lastConnectItem().url() != "")
                    ndi->connectNdi(ndi->lastConnectItem());
                break;
            }
        }
    }
}

void Models::setOutFormat(QString outFormat)
{
    int value = OutFormat::outFormatStringToIndex(outFormat);
    if(profile->setting()->outFormat()->format()->outFormat() != value)
    {
        profile->setting()->outFormat()->format()->setOutFormat(value);
        return ;
    }

    int fpga_value = getOutFormatIndexForEnum(value);
    //开机初始化不执行
    if(!init_settings_is_ok)
    {
        return ;
    }

    //set qt resolution
    rv_switch_resolutionset(fpga_value);
}

void Models::setOutputColorSpace(QString src, QString colorSpace)
{
    int index = OutFormat::outputStringToIndex(src);
    int color_space = SrcSelections::selectionStringToIndex(colorSpace);
    //output 1
    if(index == OutFormat::OUTPUT1)
    {
        if(profile->setting()->outFormat()->out1ColorSpace()->colorSpace() != color_space)
        {
            profile->setting()->outFormat()->out1ColorSpace()->setColorSpace(color_space);
            return ;
        }
        set_hdmi_out_colorspace(OutFormat::OUTPUT1,(hdmi_out_colorspace_t)color_space);
    }
    //output 2
    else if(index == OutFormat::OUTPUT2)
    {
        if(profile->setting()->outFormat()->out2ColorSpace()->colorSpace() != color_space)
        {
            profile->setting()->outFormat()->out2ColorSpace()->setColorSpace(color_space);
            return ;
        }
        set_hdmi_out_colorspace(OutFormat::OUTPUT2,(hdmi_out_colorspace_t)color_space);
    }
}

void Models::setOutSource(QString src, QString source)
{
    int src_index = OutSources::srcStringToIndex(src);
    int source_index = OutSources::sourceStringToIndex(source);
    switch (src_index) {
    case OutSources::HDMI1:
        if(profile->setting()->outSources()->hdmi1()->source() != source_index)
        {
            profile->setting()->outSources()->hdmi1()->setSource(source_index);
            return ;
        }
        break;
    case OutSources::HDMI2:
        break;
    case OutSources::UVC:
        if(profile->setting()->outSources()->uvc()->source() != source_index)
        {
            profile->setting()->outSources()->uvc()->setSource(source_index);
            return ;
        }
        break;
    }

    int hdmi1 = profile->setting()->outSources()->hdmi1()->source();
    //hdmi2 = mutilView
    int hdmi2 = OutSources::MULTIVIEW;
    int uvc = profile->setting()->outSources()->uvc()->source();

    Outsource ss;
    ss.hdmi1 = hdmi1;
    ss.hdmi2 = hdmi2;
    ss.uvc = uvc;
    qDebug() << "hdmi1:" << hdmi1 << "hdmi2:" << hdmi2 << "uvc:" << uvc;
    fpga_write(&g_fpga,OUT_SEL,*(uint16_t*)&ss);
}

void Models::setQuality(QString src, QString quality)
{
    int srcIndex = Streams::srcStringToIndex(src);
    int qualityIndex = Streams::qualityStringToIndex(quality);
    STREAM_PROFILE profiles;
    if(srcIndex == Streams::RECORDING)
    {
        if(profile->setting()->record()->quality() != qualityIndex)
        {
            profile->setting()->record()->setQuality(qualityIndex);
            return  ;
        }
        profiles = settings->getRecordBitrate(quality);
        rv_switch_set_record_bitrate(profiles.bitrate);
        int second = sd_remaintime_calc();
        media_sd->checkGbFree(second);
    }
    else if(srcIndex == Streams::STREAMING)
    {
        if(profile->streams()->quality() != qualityIndex)
        {
            profile->streams()->setQuality(qualityIndex);
            return  ;
        }

        profiles = settings->getStreamBitrateIndex(quality);
        rv_switch_set_push_bitrate(profiles.bitrate);
    }
    rv_switch_set_aac_bitrate(profiles.audio_bitrate);
}

void Models::setProtocol(bool dhcp)
{
    if(profile->setting()->network()->protocol() != dhcp)
    {
        profile->setting()->network()->setProtocol(dhcp);
        return ;
    }

    set_ipaddr(ETHERNET_NETIF, "0.0.0.0");
    if(dhcp)
    {
        set_static_interface(ETHERNET_NETIF,
                             NULL,
                             NULL,
                             NULL,
                             NULL,
                             NULL);
        settings->setMenuDHCPNetwork("","","","","");
    }
    else
    {
        QString ipAddress = profile->setting()->network()->iPAddress()->value();
        std::string s_ipAddress = ipAddress.toStdString();

        QString mask = profile->setting()->network()->subnetMask()->value();
        std::string s_mask = mask.toStdString();

        QString gateway = profile->setting()->network()->gateway()->value();
        std::string s_gateway = gateway.toStdString();

        QString dns1 = profile->setting()->network()->primaryDNS()->value();
        std::string s_dns1 = dns1.toStdString();

        QString dns2 = profile->setting()->network()->secondaryDNS()->value();
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

void Models::setIPAddress(QString value)
{
    if(profile->setting()->network()->iPAddress()->value() != value)
    {
        profile->setting()->network()->iPAddress()->setValue(value);
        return ;
    }
    setNetwork();
}

void Models::setSubnetMask(QString value)
{
    if(profile->setting()->network()->subnetMask()->value() != value)
    {
        profile->setting()->network()->subnetMask()->setValue(value);
        return ;
    }
    setNetwork();
}

void Models::setGateway(QString value)
{
    if(profile->setting()->network()->gateway()->value() != value)
    {
        profile->setting()->network()->gateway()->setValue(value);
        return ;
    }
    setNetwork();
}

void Models::setPrimaryDNS(QString value)
{
    if(profile->setting()->network()->primaryDNS()->value() != value)
    {
        profile->setting()->network()->primaryDNS()->setValue(value);
        return ;
    }
    setNetwork();
    system("/etc/init.d/S41dhcpcd restart > /dev/null 2>&1 &");
}

void Models::setSeconddaryDNS(QString value)
{
    if(profile->setting()->network()->secondaryDNS()->value() != value)
    {
        profile->setting()->network()->secondaryDNS()->setValue(value);
        return ;
    }
    setNetwork();
    system("/etc/init.d/S41dhcpcd restart > /dev/null 2>&1 &");
}

void Models::setNetwork()
{
    bool dhcp = profile->setting()->network()->protocol();
    if(dhcp)
        return ;

    QString dns1 = profile->setting()->network()->primaryDNS()->value();
    QString dns2 = profile->setting()->network()->secondaryDNS()->value();

    QString ipAddress = profile->setting()->network()->iPAddress()->value();
    std::string s_ipAddress = ipAddress.toStdString();

    QString mask = profile->setting()->network()->subnetMask()->value();
    std::string s_mask = mask.toStdString();

    QString gateway = profile->setting()->network()->gateway()->value();
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
}

void Models::setButtonBrightness(int value)
{
    if(profile->setting()->panel()->ButtonBrightness() != value)
    {
        profile->setting()->panel()->setButtonBrightness(value);
        return ;
    }
    QSwitcher::set_leds_brightness(value);
}

void Models::setLanguage(int language)
{
    static QTranslator * translator = nullptr;
    if(!translator){
        translator = new QTranslator();
    }
    qApp->removeTranslator(translator);

    QString languageFile;
    switch (language) {
    case Language::ENGLISH:
        languageFile = "./english.qm";
        break;
    case Language::CHINESE:
        languageFile = "./chinese.qm";
        break;
    }

    qDebug() << translator->load(languageFile);
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

        languages[settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_PLATFORM]->name] = tr("Platform");
        languages[settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_SERVER]->name] = tr("Server");
        languages[settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_KEY]->name] = tr("Key / RTMP URL");
        languages[settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_UPLOAD_KEY]->name] = tr("Upload Key / RTMP URL");
        languages[settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_OUTPUT]->name] = tr("Output");

        languages[settings->listFirst()[MENU_FIRST_PLAYBACK]->second[PLAYBACK_PLAYBACK]->third[MENU_THIRD_PLAYBACK_LIST]->name] = tr("Playback List");
        languages[settings->listFirst()[MENU_FIRST_PLAYBACK]->second[PLAYBACK_PLAYBACK]->third[MENU_THIRD_PLAYBACK_SEQUENTIAL]->name] = tr("Sequential");
        languages[settings->listFirst()[MENU_FIRST_PLAYBACK]->second[PLAYBACK_PLAYBACK]->third[MENU_THIRD_PLAYBACK_PROGRESS_BAR]->name] = tr("Progress Bar");

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
        languages["Single Group"] = tr("Single Group");
        languages["All Groups"] = tr("All Groups");
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

//void Models::setAudioFader(int value)
//{
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
//}

void Models::initDHCPNetworkData()
{
    int protocol = profile->setting()->network()->protocol();
    if(protocol)
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

void Models::setChromaKeyProfile()
{
    float foreground = profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->foreground();
    float background = profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->background();
    float keyedge = profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->keyEdge();

    float bgclear = 2.1;
    float blkgloss = 1;
    float green_suppression_ratio = 1.5;
    float background_clean = foreground * 1.5 + 1;
    float key_edge = 60;

    int R = settings->color5Data()[0];
    int G = settings->color5Data()[1];
    int B = settings->color5Data()[2];
    int Y = settings->chromakeySMPColorYcbcr()[0];
    int cb = settings->chromakeySMPColorYcbcr()[1];
    int cr = settings->chromakeySMPColorYcbcr()[2];

    int resize = profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->resize();
    int mask = profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->maskEnable();
    int smp = profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->sample();
    int value = 0;
    resize > 0?value += 16:value +=0;
    mask > 0?value += 2:value += 0;
    smp > 0?value += 4:value +=0;
    G > B ?value += 1:value +=0;
//    fpga_write(&g_fpga,CHROMA_CTRL,value);

    qDebug() << "bgclear:" << bgclear;
    qDebug() << "blkgloss:" << blkgloss;
    qDebug() << "green_suppression_ratio:" << green_suppression_ratio;
    qDebug() << "background_clean:" << background_clean;
    qDebug() << "key_edge:" << key_edge;
    qDebug() << "foreground:" << foreground;
    qDebug() << "background:" << background;
    qDebug() << "keyedge:" << keyedge;

    auto chromaKeyData = getChromaKeyData(Y,cb,cr,bgclear,blkgloss,green_suppression_ratio,background_clean,key_edge,background,keyedge);
    chromaKeyData.print();

    fpga_write(&g_fpga,CHROMA_YS,chromaKeyData.ck_ys);
    fpga_write(&g_fpga,CHROMA_K1,chromaKeyData.k1);
    fpga_write(&g_fpga,CHROMA_K2,chromaKeyData.k2);
    fpga_write(&g_fpga,CHROMA_K3,chromaKeyData.k3);
    fpga_write(&g_fpga,CHROMA_K4,chromaKeyData.k4);
    fpga_write(&g_fpga,CHROMA_K5,chromaKeyData.k5);
    fpga_write(&g_fpga,CHROMA_K6,chromaKeyData.k6);
    fpga_write(&g_fpga,CHROMA_K7,chromaKeyData.k7);
    fpga_write(&g_fpga,CHROMA_K10,chromaKeyData.k10);
    fpga_write(&g_fpga,CHROMA_K11,chromaKeyData.k11);
    fpga_write(&g_fpga,CHROMA_K12,chromaKeyData.k12);
    fpga_write(&g_fpga,ADDR_MATTE_GAIN,chromaKeyData.gain);
    fpga_write(&g_fpga,ADDR_MATTE_CLIP,chromaKeyData.clip);

    value += 8;
    fpga_write(&g_fpga,CHROMA_CTRL,value);
//    usleep(50000);
//    QTimer::singleShot(50,this,[=](){
//        fpga_write(&g_fpga,CHROMA_CTRL,value - 8);
//    });
    fpga_write(&g_fpga,CHROMA_CTRL,value - 8);
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

void Models::setKeyPatternWipePattern(int patternIndex)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->pattern() != patternIndex)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setPattern(patternIndex);
        return ;
    }
    //    //send settings signal
    settings->setKeyPatternWipePatternIndex(patternIndex);
    setKeyPatternCtrl();
    setKeyPatternPosition();
}

void Models::setKeyPatternWipeSize(int size)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->wipeSize() != size)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setWipeSize(size);
        return ;
    }

#define PATTERN_WIPE_SIZE_PERCENT 2.55
    size = size * PATTERN_WIPE_SIZE_PERCENT;
    fpga_write(&g_fpga,PATTERN_WIPE_SIZE,size);
}

void Models::setKeyPatternWipeXPosition(double xPosition)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->wipeXPosition() != xPosition)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setWipeXPosition(xPosition);
        return ;
    }
    setKeyPatternPosition();
}

void Models::setKeyPatternWipeYPosition(double yPosition)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->wipeYPosition() != yPosition)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setWipeYPosition(yPosition);
        return ;
    }
    setKeyPatternPosition();
}

void Models::setKeyPatternWipeSymmetry(int symmetry)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->wipeSymmetry() != symmetry)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setWipeSymmetry(symmetry);
        return ;
    }
    setKeyPatternPosition();
}

void Models::setKeyPatternWipeSoftness(int softness)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->wipeSoftness() != softness)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setWipeSoftness(softness);
        return ;
    }

    float softnessCur ;
    float softnessMax = profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->softnessMax();
    float softnessMin = profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->softnessMin();

    softnessCur = (softness - softnessMin) / (softnessMax - softnessMin);
    int value = getWipeSoftness(softnessCur);
    fpga_write(&g_fpga,PATTERN_SOFTNESS,value);
}

void Models::setKeyPatternCtrl()
{
    int pattern_index = profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->pattern();
    int pattern_resize = profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->resize();
    int pattern_mask = profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->maskEnable();

    int value = 0;
    pattern_resize != 0?value += 0x40:value += 0;
    pattern_mask != 0?value += 0x20:value += 0;
    value += pattern_index;
    fpga_write(&g_fpga,PATTERN_CTRL,value);
}

void Models::setKeyPatternPosition()
{
    int pattern_index = profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->pattern();
    float posX = profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->wipeXPosition();
    float posXMax = profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->wipeXPositionMax();
    float posXMin = profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->wipeXPositionMin();
    posX = (posX - posXMin) / (posXMax - posXMin);

    float posY = profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->wipeYPosition();
    float posYMax = profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->wipeYPositionMax();
    float posYMin = profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->wipeYPositionMin();
    posY = (posY - posYMin) / (posYMax - posYMin);

    float symmertry = profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->wipeSymmetry();
    float symmertryMax = profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->SymmetryMax();
    float symmertryMin = profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->SymmetryMin();

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

void Models::setPipBorderEnable(bool enable)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->borderEnable() != enable)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setBorderEnable(enable);
        return ;
    }
    setKeyCtrl(Keys::PIP);
}

void Models::setPipBorderWidth(int width)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->borderWidth() != width)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setBorderWidth(width);
        return ;
    }
    fpga_write(&g_fpga,FPGA_PIP_BOARD,width);
}

void Models::setPipBorderColorHue(int hue)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->borderColorHue() != hue)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setBorderColorHue(hue);
        return ;
    }
    setPipBorderColor();
}

void Models::setPipBorderColorSaturation(int saturation)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->borderColorSaturation() != saturation)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setBorderColorSaturation(saturation);
        return ;
    }
    setPipBorderColor();
}

void Models::setPipBorderColorBrightness(int brightness)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->borderColorBrightness() != brightness)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setBorderColorBrightness(brightness);
        return ;
    }
    setPipBorderColor();
}

void Models::setPipBorderColor()
{
    int Hue = profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->borderColorHue();
    int Saturation = profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->borderColorSaturation();
    int Brightness = profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->borderColorBrightness();

    settings->setColor6Data({Hue,Saturation,Brightness});

    auto color = getColorYC(Hue,Saturation,Brightness);

    fpga_write(&g_fpga,PIP_COLOR_Y,color.Y);
    fpga_write(&g_fpga,PIP_COLOR_C,*(uint16_t *)&(color.Cr));
}

void Models::initStillUpLoadRvSwitch()
{
    const char *png_path[STILLMAX] = {0};
    std::string list[STILLMAX];
    for(int i = 0;i < STILLMAX;++i)
    {
        auto still = qobject_cast<Still*>(profile->stillGenerator()->stills()->stillIndex(i));
        if(still == nullptr)
            continue;
        QString path = still->path();
        path = path.toLocal8Bit();
        list[i] = path.toStdString();
        png_path[i] = list[i].c_str();
    }
    rv_switch_still_upload(png_path,STILLMAX);
}

void Models::stillLoadPictureIndexChanged(int value)
{
    int size = settings->listFirst()[MENU_FIRST_STILL_GENERATOR]->second[STILL_GENERATE_UPLOAD]->third[STILL_UPLOAD_LOAD_PICTURE]->list_text.size();
    if(size == 0)
        return ;
    if(value < 0)
        value = 0;
    if(value >= size)
        value = size - 1;
    settings->setMenuValue(MENU_FIRST_STILL_GENERATOR,STILL_GENERATE_UPLOAD,STILL_UPLOAD_LOAD_PICTURE,value);
}

#define LOADSLEEP 100
void Models::loadStill1()
{
    qDebug() << "loadStill1";
    settings->setLoadStillProgress(2);

    QString name = settings->listFirst()[MENU_FIRST_STILL_GENERATOR]->second[STILL_GENERATE_UPLOAD]->third[STILL_UPLOAD_LOAD_PICTURE]->getText();

    if(name == "")
        return ;

    int location_index = profile->stillGenerator()->stillSelection()->location();

    QImage image(SD_IMAGE_PATH + name);
    if(image.width() == 1920 && image.height() == 1080)
    {
        //2.copy SD卡文件到userdata
        QString cmd = "cp \'" SD_IMAGE_PATH + name + "\'  \'" STILLPATH + QString::number(location_index) + ".png" + "\'";
        qDebug() << "_____-cmd:" << cmd;
        std::string s_cmd = cmd.toStdString();
        system(s_cmd.data());
        system("sync");

        //设置加载文件名
        myProvider->setStillImageFileName(STILLPATH + QString::number(location_index) + ".png");
        //1.设置配置文件中location 位置对应文件名
        Still *still = new Still(location_index,name,STILLPATH + QString::number(location_index) + ".png");
        profile->stillGenerator()->stills()->append(still);
        fpga_write(&g_fpga,STILL_UPLOAD,(1 << 2) + (1 << 1));//110
        emit loadStillImage(1,"fill");
    }
    else
    {
        QTimer::singleShot(10,this,[=](){
            QImage image1 = image.scaled(1920,1080);
            image1.save(STILLPATH + QString::number(location_index) + ".png");
            system("sync");
            //设置加载文件名
            myProvider->setStillImageFileName(STILLPATH + QString::number(location_index) + ".png");
            //1.设置配置文件中location 位置对应文件名
            Still *still = new Still(location_index,name,STILLPATH + QString::number(location_index) + ".png");
            profile->stillGenerator()->stills()->append(still);
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
    if(profile->setting()->srcSelections()->aux()->selection() != SrcSelections::SD_CARD)
        return -1;
    if(!media_sd->is_online())
        return -1;
    if(!(settings->playingIndex() < playbackGroupManager->list().size() && settings->playingIndex() >= 0))
        return -1;

    QString play = SD_VIDEO_PATH + playbackGroupManager->list()[settings->playingIndex()];
    qDebug() << play;
    //
    play = play.toLocal8Bit();

    std::string s_play = play.toStdString();
    rv_switch_mp4_open((char*)(s_play.data()),(rk_switch_cb)(playCallback),0);
    return 0;
}

void Models::playPause(int index)
{
    if(profile->setting()->srcSelections()->aux()->selection() != SrcSelections::SD_CARD)
        return ;
    if(!media_sd->is_online())
        return ;
    if(!(settings->playingIndex() < playbackGroupManager->list().size() && settings->playingIndex() >= 0))
        return ;

    rv_switch_mp4_pause(index,(rk_switch_cb)(playCallback),0);
}

void Models::playStop()
{
    qDebug() << "<<<<<<<<<<<playStop";
    rv_switch_mp4_close((rk_switch_cb)(playCallback),0);
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

    QString fileName = profile->setting()->record()->fileName();
    fileName = fileName + "_" + QString::number(QDateTime::currentMSecsSinceEpoch()) + ".mp4";
    settings->setCurrentRecordFileName(fileName);
    fileName = SD_VIDEO_PATH + fileName ;
    std::string s_fileName = fileName.toStdString();
    rv_switch_record_start0((char*)(s_fileName.data()),(rk_switch_cb)(recordStartCallback),0);
}

void Models::recordStop()
{
    if(settings->recordLedStatus() == E_STATUS_FAILED)
        return ;

    static bool recordMinFlag = false;
    if(!recordMinFlag && settings->recordSecond() < 2){
        recordMinFlag = true;
        QtConcurrent::run([=](){
            while(1){
                if(settings->recordSecond() >= 2 || !media_sd->is_online()){
                    recordMinFlag = false;
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
    playbackGroupManager->joinGroup(settings->currentRecordFileName(),RECORD_GROUP_NAME);
    settings->setCurrentRecordFileName("");
    rv_switch_record_stop0((rk_switch_cb)(recordStopCallback),0);
}

void Models::setPlayNext()
{
    if(profile->setting()->srcSelections()->aux()->selection() != SrcSelections::SD_CARD)
        return ;
    if(!media_sd->is_online())
        return ;
    if(settings->playingIndex() + 1 >= playbackGroupManager->list().size())
        return ;

    settings->setPlayingIndex(settings->playingIndex() + 1);
    playStart();
}

void Models::setPlayPrevious()
{
    if(profile->setting()->srcSelections()->aux()->selection() != SrcSelections::SD_CARD)
        return ;
    if(!media_sd->is_online())
        return ;
    if(settings->playingIndex() - 1 < 0 || playbackGroupManager->list().size() <= 1)
        return ;

    settings->setPlayingIndex(settings->playingIndex() - 1);
    playStart();
}

void Models::openAllStream()
{
    QString rtmp1,rtmp2,rtmp3;
    std::string s_rtmp1,s_rtmp2,s_rtmp3;
    bool enable1,enable2,enable3;
    rtmp1 = settings->getStreamUrlIndex(STREAM_STREAM1);
    s_rtmp1 = rtmp1.toStdString();
    rtmp2 = settings->getStreamUrlIndex(STREAM_STREAM2);
    s_rtmp2 = rtmp2.toStdString();
    rtmp3 = settings->getStreamUrlIndex(STREAM_STREAM3);
    s_rtmp3 = rtmp3.toStdString();
    enable1 = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_OUTPUT]->current.toInt();// == OUTPUT_ENABLE;
    enable2 = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM2]->third[MENU_THIRD_STREAM_OUTPUT]->current.toInt();// == OUTPUT_ENABLE;
    enable3 = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM3]->third[MENU_THIRD_STREAM_OUTPUT]->current.toInt();// == OUTPUT_ENABLE;
    if(enable1)
    {
        rv_switch_push_start0((char*)(s_rtmp1.data()),(rk_switch_cb)(pushCallBack1),0);
    }
    if(enable2)
    {
        rv_switch_push_start1((char*)(s_rtmp2.data()),(rk_switch_cb)(pushCallBack2),0);
    }
    if(enable3)
    {
        rv_switch_push_start2((char*)(s_rtmp3.data()),(rk_switch_cb)(pushCallBack3),0);
    }
}

void Models::openOneStream(int index)
{
    QString rtmp;
    std::string s_rtmp;
    rtmp = settings->getStreamUrlIndex(index);;
    s_rtmp = rtmp.toStdString();
    if(index == Streams::STREAM1)
    {
        qDebug() << "start one:" << 0;
        rv_switch_push_start0((char*)(s_rtmp.data()),(rk_switch_cb)(pushCallBack1),0);
    }
    else if(index == Streams::STREAM2)
    {
        qDebug() << "start one:" << 1;
        rv_switch_push_start1((char*)(s_rtmp.data()),(rk_switch_cb)(pushCallBack2),0);
    }
    else if(index == Streams::STREAM3)
    {
        qDebug() << "start one:" << 2;
        rv_switch_push_start2((char*)(s_rtmp.data()),(rk_switch_cb)(pushCallBack3),0);
    }
}

void Models::closeAllStream()
{
    bool enable1,enable2,enable3;
    enable1 = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_OUTPUT]->current.toInt();// == OUTPUT_ENABLE;
    enable2 = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM2]->third[MENU_THIRD_STREAM_OUTPUT]->current.toInt();// == OUTPUT_ENABLE;
    enable3 = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM3]->third[MENU_THIRD_STREAM_OUTPUT]->current.toInt();// == OUTPUT_ENABLE;
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

void Models::closeOneStream(int index)
{
    if(index == Streams::STREAM1)
        rv_switch_push_stop0(pushCallBack1,0);
    else if(index == Streams::STREAM2)
        rv_switch_push_stop1(pushCallBack2,0);
    else if(index == Streams::STREAM3)
        rv_switch_push_stop2(pushCallBack3,0);
}

void Models::streamUploadKeyIndexChanged(int second, int value)
{
    int size = settings->listFirst()[MENU_FIRST_STREAM]->second[second]->third[MENU_THIRD_STREAM_UPLOAD_KEY]->list_text.size();
    if(size == 0)
        return ;
    if(value < 0)
        value = 0;
    if(value >= size)
        value = size - 1;
    settings->setMenuValue(MENU_FIRST_STREAM,second,MENU_THIRD_STREAM_UPLOAD_KEY,value);
}

int Models::getOutFormatIndexForEnum(int index)
{
    int value = 0;
    switch (index) {
    case OutFormat::OUT_1080p24:
        value = 0x0;
        break;
    case OutFormat::OUT_1080p25:
        value = 0x1;
        break;
    case OutFormat::OUT_1080p30:
        value = 0x2;
        break;
    case OutFormat::OUT_1080p50:
        value = 0x3;
        break;
    case OutFormat::OUT_1080p60:
        value = 0x4;
        break;
    }
    if(index < OutFormat::OUT_1080p24)
        value = 0x0;
    if(index > OutFormat::OUT_1080p60)
        value = 0x4;

    return value;
}

int Models::getOutFormat(int index)
{
    int value = 30;
    switch (index) {
    case OutFormat::OUT_1080p24:
        value = 24;
        break;
    case OutFormat::OUT_1080p25:
        value = 25;
        break;
    case OutFormat::OUT_1080p30:
        value = 30;
        break;
    case OutFormat::OUT_1080p50:
        value = 50;
        break;
    case OutFormat::OUT_1080p60:
        value = 60;
        break;
    }
    if(index < OutFormat::OUT_1080p24)
        value = 24;
    if(index > OutFormat::OUT_1080p60)
        value = 60;

    return value;
}

void Models::setMSleep(int value)
{
    qDebug() << "setSleep:" <<  value;
//    QThread::msleep(value);
}

void Models::setUserWait()
{

}

void Models::setLoadStreamKey(int second)
{
    if(second == STREAM_STREAM1 && settings->liveStreamStatus1() != E_STATUS_CLOSE)
        return ;
    if(second == STREAM_STREAM2 && settings->liveStreamStatus2() != E_STATUS_CLOSE)
        return ;
    if(second == STREAM_STREAM3 && settings->liveStreamStatus3() != E_STATUS_CLOSE)
        return ;
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

void Models::setPgmIndex(QString str)
{
    int index = MixEffectBlock::inputStringToIndex(str);
    if(profile->mixEffectBlocks()->mixEffectBlock()->program()->input() != index)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->program()->setInput(index);
        return ;
    }
    if(QSwitcher::get_led(KEY_LED_PGM_1 + index) == LED_STATUS_OFF)
        keyClick(KEY_LED_PGM_1 + index);
}

void Models::setPvwIndex(QString str)
{
    int index = MixEffectBlock::inputStringToIndex(str);
    if(profile->mixEffectBlocks()->mixEffectBlock()->preview()->input() != index)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->preview()->setInput(index);
        return ;
    }
    if(QSwitcher::get_led(KEY_LED_PVW_1 + index) == LED_STATUS_OFF)
        keyClick(KEY_LED_PVW_1 + index);
}

void Models::setNextTransition(QString source)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->nextTransition()->selection() != source)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->nextTransition()->setSelection(source);
        return ;
    }

    int index = NextTransition::selectionStringToValue(source);
    qDebug() << source;
    qDebug() << index;
    if(index & 0b100){
        qDebug() << "111";
        if(QSwitcher::get_led(KEY_LED_BKGD) == SWITCHER_LED_OFF){
            qDebug() << "222";
            keyClick(KEY_LED_BKGD);
        }
    }else{
        //bkgd close
        if(QSwitcher::get_led(KEY_LED_BKGD) != SWITCHER_LED_OFF){
            keyClick(KEY_LED_BKGD);
        }
    }
    if(index & 0b010){
        //dsk open
        if(QSwitcher::get_led(KEY_LED_DSK) == SWITCHER_LED_OFF){
            keyClick(KEY_LED_DSK);
        }
    }else{
        //dsk close
        if(QSwitcher::get_led(KEY_LED_DSK) != SWITCHER_LED_OFF){
            keyClick(KEY_LED_DSK);
        }
    }
    if(index & 0b001){
        //key open
        if(QSwitcher::get_led(KEY_LED_KEY) == SWITCHER_LED_OFF){
            keyClick(KEY_LED_KEY);
        }
    }else{
        //key close
        if(QSwitcher::get_led(KEY_LED_KEY) != SWITCHER_LED_OFF){
            keyClick(KEY_LED_KEY);
        }
    }
}

void Models::setTransitionStyle(QString style)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->style() != style)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->setStyle(style);
        return ;
    }

    int value = TransitionStyle::styleStringToIndex(style);
    if(value == TransitionStyle::MIX)
    {
        if(QSwitcher::get_led(KEY_LED_TRANS_MIX) == SWITCHER_LED_OFF){
            keyClick(KEY_LED_TRANS_MIX);
        }
    }
    if(value == TransitionStyle::DIP)
    {
        if(QSwitcher::get_led(KEY_LED_TRANS_DIP) == SWITCHER_LED_OFF){
            keyClick(KEY_LED_TRANS_DIP);
        }
    }
    if(value == TransitionStyle::WIPE)
    {
        if(QSwitcher::get_led(KEY_LED_TRANS_WIPE) == SWITCHER_LED_OFF){
            keyClick(KEY_LED_TRANS_WIPE);
        }
    }

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

void Models::setPreviewTransition(bool status)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->previewTransition() != status)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->setPreviewTransition(status);
        return ;
    }

    if(status && QSwitcher::get_led(KEY_LED_TRANS_PREVIEW) == SWITCHER_LED_OFF)
    {
        keyClick(KEY_LED_TRANS_PREVIEW);
    }
    else if(!status && QSwitcher::get_led(KEY_LED_TRANS_PREVIEW) != SWITCHER_LED_OFF)
    {
        keyClick(KEY_LED_TRANS_PREVIEW);
    }
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

void Models::setKeyOnAir(bool onAir)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->key()->onAir() != onAir)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->key()->setOnAir(onAir);
        return ;
    }
    if(!onAir && QSwitcher::get_led(KEY_LED_KEY_ON_AIR) != SWITCHER_LED_OFF){
        keyClick(KEY_LED_KEY_ON_AIR);
    }else if(onAir && QSwitcher::get_led(KEY_LED_KEY_ON_AIR) == SWITCHER_LED_OFF){
        keyClick(KEY_LED_KEY_ON_AIR);
    }
}

int Models::sd_remaintime_calc()
{
    media_sd->current_usage();
    auto really_free = _getfree_recordreally(media_sd->bfree,settings->recordSecond());
    STREAM_PROFILE record = settings->getRecordBitrate(Streams::qualityIndexToString(profile->setting()->record()->quality()));
    int rate = record.bitrate / 8;
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
        setColorBack(colorIndex,m_colorBack->hue(),m_colorBack->saturation(),m_colorBack->brightness());
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
        setColorBack(colorIndex,m_colorBack->hue(),m_colorBack->saturation(),m_colorBack->brightness());
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
        setColorBack(colorIndex,m_colorBack->hue(),m_colorBack->saturation(),m_colorBack->brightness());
    }
}

void Models::setColorBack(int colorIndex,int hue,int saturation,int brightness)
{
    int fpga_value_y = -1;
    int fpga_value_c = -1;
    if(colorIndex == ColorBacks::COLOR1){
        settings->setColor1Data({hue,saturation,brightness});
        fpga_value_y = COLOR1_Y;
        fpga_value_c = COLOR1_C;
    }else if(colorIndex == ColorBacks::COLOR2){
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

void Models::setSuperSourceEnable(bool enable)
{
    if(profile->superSources()->superSource()->enable() != enable)
    {
        profile->superSources()->superSource()->setEnable(enable);
        return ;
    }

    setSuperSourceCtrl();
}

void Models::setSuperSourceSource1(int source1)
{
    if(profile->superSources()->superSource()->source1() != source1)
    {
        profile->superSources()->superSource()->setSource1(source1);
        return ;
    }
    setSuperSource();
}

void Models::setSuperSourceSource2(int source2)
{
    if(profile->superSources()->superSource()->source2() != source2)
    {
        profile->superSources()->superSource()->setSource2(source2);
        return ;
    }
    setSuperSource();
}

void Models::setSuperSourceBackground(int background)
{
    if(profile->superSources()->superSource()->background() != background)
    {
        profile->superSources()->superSource()->setBackground(background);
        return ;
    }
    setSuperSource();
}

void Models::setSuperSourceControlStyle(QString style)
{
    if(profile->superSources()->superSource()->style() != style)
    {
        profile->superSources()->superSource()->setStyle(style);
        return ;
    }

    setSuperSourceCtrl();
}

void Models::setSuperSourceControlYPosition(int YPosition)
{
    if(profile->superSources()->superSource()->yPosition() != YPosition)
    {
        profile->superSources()->superSource()->setYPosition(YPosition);
        return ;
    }

    float p;
    p = YPosition / 100.0;
    p = (MASK_V_MAX - MASK_V_MIN) * p - MASK_V_MAX;
    int value = getMaskPositionV(p);
    fpga_write(&g_fpga,SUPER_SRC_POS,value);
}

void Models::setSuperSourceMaskEnable(int maskIndex, bool enable)
{
    if(maskIndex == SuperSource::MASK1)
    {
        if(profile->superSources()->superSource()->mask1()->enable() != enable)
        {
            profile->superSources()->superSource()->mask2()->setEnable(enable);
            return ;
        }
    }
    else if(maskIndex == SuperSource::MASK2)
    {
        if(profile->superSources()->superSource()->mask2()->enable() != enable)
        {
            profile->superSources()->superSource()->mask2()->setEnable(enable);
            return ;
        }
    }

    setSuperSourceCtrl();
}

void Models::setSuperSourceMaskHStart(int maskIndex, int HStart)
{
    if(maskIndex == SuperSource::MASK1)
    {
        if(profile->superSources()->superSource()->mask1()->hStart() != HStart)
        {
            profile->superSources()->superSource()->mask2()->setHStart(HStart);
            return ;
        }
    }
    else if(maskIndex == SuperSource::MASK2)
    {
        if(profile->superSources()->superSource()->mask2()->hStart() != HStart)
        {
            profile->superSources()->superSource()->mask2()->setHStart(HStart);
            return ;
        }
    }

    setSuperSourceMask(maskIndex,SUPER_MASK1_H_START);
}

void Models::setSuperSourceMaskVStart(int maskIndex, int VStart)
{
    if(maskIndex == SuperSource::MASK1)
    {
        if(profile->superSources()->superSource()->mask1()->vStart() != VStart)
        {
            profile->superSources()->superSource()->mask2()->setVStart(VStart);
            return ;
        }
    }
    else if(maskIndex == SuperSource::MASK2)
    {
        if(profile->superSources()->superSource()->mask2()->vStart() != VStart)
        {
            profile->superSources()->superSource()->mask2()->setVStart(VStart);
            return ;
        }
    }

    setSuperSourceMask(maskIndex,SUPER_MASK1_V_START);
}

void Models::setSuperSourceMaskHEnd(int maskIndex, int HEnd)
{
    if(maskIndex == SuperSource::MASK1)
    {
        if(profile->superSources()->superSource()->mask1()->hEnd() != HEnd)
        {
            profile->superSources()->superSource()->mask2()->setHEnd(HEnd);
            return ;
        }
    }
    else if(maskIndex == SuperSource::MASK2)
    {
        if(profile->superSources()->superSource()->mask2()->hEnd() != HEnd)
        {
            profile->superSources()->superSource()->mask2()->setHEnd(HEnd);
            return ;
        }
    }

    setSuperSourceMask(maskIndex,SUPER_MASK1_H_END);
}

void Models::setSuperSourceMaskVEnd(int maskIndex, int VEnd)
{
    if(maskIndex == SuperSource::MASK1)
    {
        if(profile->superSources()->superSource()->mask1()->vEnd() != VEnd)
        {
            profile->superSources()->superSource()->mask2()->setVEnd(VEnd);
            return ;
        }
    }
    else if(maskIndex == SuperSource::MASK2)
    {
        if(profile->superSources()->superSource()->mask2()->vEnd() != VEnd)
        {
            profile->superSources()->superSource()->mask2()->setVEnd(VEnd);
            return ;
        }
    }

    setSuperSourceMask(maskIndex,SUPER_MASK1_V_END);
}

void Models::setSuperSourceBorderWidth(int borderIndex, int width)
{
    int fpga_value = -1;
    if(borderIndex == SuperSource::BORDER1)
    {
        if(profile->superSources()->superSource()->border1()->width() != width)
        {
            profile->superSources()->superSource()->border1()->setWidth(width);
            return ;
        }
        fpga_value = SUPER_SRC1_BORDER;
    }
    else if(borderIndex == SuperSource::BORDER2)
    {
        if(profile->superSources()->superSource()->border2()->width() != width)
        {
            profile->superSources()->superSource()->border2()->setWidth(width);
            return ;
        }
       fpga_value = SUPER_SRC2_BORDER;
    }

    if(fpga_value != -1)
        fpga_write(&g_fpga,fpga_value,width);
}

void Models::setSuperSourceBorderColorHue(int borderIndex, int hue)
{
    if(borderIndex == SuperSource::MASK1)
    {
        if(profile->superSources()->superSource()->border1()->colorHue() != hue)
        {
            profile->superSources()->superSource()->border1()->setColorHue(hue);
            return ;
        }
    }
    else if(borderIndex == SuperSource::MASK2)
    {
        if(profile->superSources()->superSource()->border2()->colorHue() != hue)
        {
            profile->superSources()->superSource()->border2()->setColorHue(hue);
            return ;
        }
    }
    setSuperSourceColor(borderIndex);
}

void Models::setSuperSourceBorderColorSaturation(int borderIndex, int saturation)
{
    if(borderIndex == SuperSource::MASK1)
    {
        if(profile->superSources()->superSource()->border1()->colorSaturation() != saturation)
        {
            profile->superSources()->superSource()->border1()->setColorSaturation(saturation);
            return ;
        }
    }
    else if(borderIndex == SuperSource::MASK2)
    {
        if(profile->superSources()->superSource()->border2()->colorSaturation() != saturation)
        {
            profile->superSources()->superSource()->border2()->setColorSaturation(saturation);
            return ;
        }
    }
    setSuperSourceColor(borderIndex);
}

void Models::setSuperSourceBorderColorBrightness(int borderIndex, int brightness)
{
    if(borderIndex == SuperSource::MASK1)
    {
        if(profile->superSources()->superSource()->border1()->colorBrightness() != brightness)
        {
            profile->superSources()->superSource()->border1()->setColorBrightness(brightness);
            return ;
        }
    }
    else if(borderIndex == SuperSource::MASK2)
    {
        if(profile->superSources()->superSource()->border2()->colorBrightness() != brightness)
        {
            profile->superSources()->superSource()->border2()->setColorBrightness(brightness);
            return ;
        }
    }
    setSuperSourceColor(borderIndex);
}

void Models::setSuperSourceCtrl()
{
    int value = 0;
    int enable = profile->superSources()->superSource()->enable();
    int mask1 = profile->superSources()->superSource()->mask1()->enable();
    int mask2 = profile->superSources()->superSource()->mask2()->enable();
    int style = SuperSource::styleStringToIndex(profile->superSources()->superSource()->style());

    if(style == SuperSource::CROP_ZOOM_IN)
        value += 8;
    else if(style == SuperSource::ZOOM_IN_CROP)
        value += 16;
    else if(style == SuperSource::CROP)
        value += 24;

    enable > 0?value += 2: value += 0;
    mask1 > 0?value += 4: value += 0;
    mask2 > 0?value += 1: value += 0;

    fpga_write(&g_fpga,SUPER_SRC_CTRL,value);
}

void Models::setSuperSource()
{
    int source1 = profile->superSources()->superSource()->source1();
    int source2 = profile->superSources()->superSource()->source2();
    int background = profile->superSources()->superSource()->background();

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

    if(maskIndex == SuperSource::MASK1)
    {
        switch (PositionIndex) {
        case SUPER_MASK1_H_START:
            //定义域取负
            fpga_value = SUPER_SRC1_MASK_HSTART;
            per = profile->superSources()->superSource()->mask1()->hStart();
            p = per / 100.0;
            p = (MASK_H_MAX - MASK_H_MIN) * p - MASK_H_MAX;
            value = getMaskPositionH(p);
            break;
        case SUPER_MASK1_V_START:
            fpga_value = SUPER_SRC1_MASK_VSTART;
            per = profile->superSources()->superSource()->mask1()->vStart();
            p = per / 100.0;
            p = (MASK_V_MAX - MASK_V_MIN) * p - MASK_V_MAX;
            value = getMaskPositionV(p);
            break;
        case SUPER_MASK1_H_END:
            //定义域取负
            fpga_value = SUPER_SRC1_MASK_HEND;
            per = profile->superSources()->superSource()->mask1()->hEnd();
            p = per / 100.0;
            p = (MASK_H_MAX - MASK_H_MIN) * p - MASK_H_MAX;
            value = getMaskPositionH(p);
            break;
        case SUPER_MASK1_V_END:
            fpga_value = SUPER_SRC1_MASK_VEND;
            per = profile->superSources()->superSource()->mask1()->vEnd();
            p = per / 100.0;
            p = (MASK_V_MAX - MASK_V_MIN) * p - MASK_V_MAX;
            value = getMaskPositionV(p);
            break;
        }
    }else if(maskIndex == SuperSource::MASK2)
    {
        switch (PositionIndex) {
        case SUPER_MASK2_H_START:
            //定义域取负
            fpga_value = SUPER_SRC2_MASK_HSTART;
            per = profile->superSources()->superSource()->mask2()->hStart();
            p = per / 100.0;
            p = (MASK_H_MAX - MASK_H_MIN) * p - MASK_H_MAX;
            value = getMaskPositionH(p);
            break;
        case SUPER_MASK2_V_START:
            fpga_value = SUPER_SRC2_MASK_VSTART;
            per = profile->superSources()->superSource()->mask2()->vStart();
            p = per / 100.0;
            p = (MASK_V_MAX - MASK_V_MIN) * p - MASK_V_MAX;
            value = getMaskPositionV(p);
            break;
        case SUPER_MASK2_H_END:
            //定义域取负
            fpga_value = SUPER_SRC2_MASK_HEND;
            per = profile->superSources()->superSource()->mask2()->hEnd();
            p = per / 100.0;
            p = (MASK_H_MAX - MASK_H_MIN) * p - MASK_H_MAX;
            value = getMaskPositionH(p);
            break;
        case SUPER_MASK2_V_END:
            fpga_value = SUPER_SRC2_MASK_VEND;
            per = profile->superSources()->superSource()->mask2()->vEnd();
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
    if(colorIndex == SuperSource::BORDER1){
        Hue = profile->superSources()->superSource()->border1()->colorHue();
        Saturation = profile->superSources()->superSource()->border1()->colorSaturation();
        Brightness = profile->superSources()->superSource()->border1()->colorBrightness();
        settings->setColor3Data({Hue,Saturation,Brightness});
        fpga_value_y = SUPER_SRC1_BORDER_Y;
        fpga_value_c = SUPER_SRC1_BORDER_C;

    }else if(colorIndex == SuperSource::BORDER2){
        Hue = profile->superSources()->superSource()->border2()->colorHue();
        Saturation = profile->superSources()->superSource()->border2()->colorSaturation();
        Brightness = profile->superSources()->superSource()->border2()->colorBrightness();
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

void Models::setKeyType(QString type)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->key()->type() != type)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->key()->setType(type);
        return ;
    }
    fpga_write(&g_fpga,UPSTREAM_KEY_TYPE,Key::typeStringToIndex(type));
}

void Models::setKeyMaskEnable(QString keystr, int enable)
{
    int key = Keys::keyStringToIndex(keystr);
    switch (key) {
    case Keys::LUMA:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->maskEnable() != enable)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setMaskEnable(enable);
            return ;
        }
        break;
    case Keys::CHROMA:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->maskEnable() != enable)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setMaskEnable(enable);
            return ;
        }
        break;
    case Keys::PATTERN:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->maskEnable() != enable)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setMaskEnable(enable);
            return ;
        }
        break;
    case Keys::PIP:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->maskEnable() != enable)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setMaskEnable(enable);
            return ;
        }
        break;
    default:
        return ;
    }

    setKeyCtrl(key);
}

void Models::setKeyMaskHStart(QString keystr, int hStart)
{
    int key = Keys::keyStringToIndex(keystr);
    int fpga_value = -1;
    float p;
    int value;
    int per;
    switch (key) {
    case Keys::LUMA:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->maskHStart() != hStart)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setMaskHStart(hStart);
            return ;
        }
        fpga_value = LUMA_KEY_MASK_HSTART;
        break;
    case Keys::CHROMA:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->maskHStart() != hStart)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setMaskHStart(hStart);
            return ;
        }
        fpga_value = CHROMA_MASK_HSTART;
        break;
    case Keys::PATTERN:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->maskHStart() != hStart)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setMaskHStart(hStart);
            return ;
        }
        fpga_value = PATTERN_MASK_HSTART;
        break;
    case Keys::PIP:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->maskHStart() != hStart)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setMaskHStart(hStart);
            return ;
        }
        fpga_value = PIP_MASK_HSTART;
        break;
    default:
        return ;
    }
    per = hStart;
    p = per / 100.0;
    p = (MASK_H_MAX - MASK_H_MIN) * p - MASK_H_MAX;
    value = getMaskPositionH(p);

    if(fpga_value != -1)
        fpga_write(&g_fpga,fpga_value,value);
}

void Models::setKeyMaskVStart(QString keystr, int vStart)
{
    int key = Keys::keyStringToIndex(keystr);
    int fpga_value = -1;
    float p;
    int value;
    int per;
    switch (key) {
    case Keys::LUMA:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->maskVStart() != vStart)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setMaskVStart(vStart);
            return ;
        }
        fpga_value = LUMA_KEY_MASK_VSTART;
        break;
    case Keys::CHROMA:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->maskVStart() != vStart)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setMaskVStart(vStart);
            return ;
        }
        fpga_value = CHROMA_MASK_VSTART;
        break;
    case Keys::PATTERN:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->maskVStart() != vStart)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setMaskVStart(vStart);
            return ;
        }
        fpga_value = PATTERN_MASK_VSTART;
        break;
    case Keys::PIP:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->maskVStart() != vStart)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setMaskVStart(vStart);
            return ;
        }
        fpga_value = PIP_MASK_VSTART;
        break;
    default:
        return ;
    }
    per = vStart;
    p = per / 100.0;
    p = (MASK_V_MAX - MASK_V_MIN) * p - MASK_V_MAX;
    value = getMaskPositionV(p);

    if(fpga_value != -1)
        fpga_write(&g_fpga,fpga_value,value);
}

void Models::setKeyMaskHEnd(QString keystr, int hEnd)
{
    int key = Keys::keyStringToIndex(keystr);
    int fpga_value = -1;
    float p;
    int value;
    int per;
    switch (key) {
    case Keys::LUMA:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->maskHEnd() != hEnd)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setMaskHEnd(hEnd);
            return ;
        }
        fpga_value = LUMA_KEY_MASK_HEND;
        break;
    case Keys::CHROMA:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->maskHEnd() != hEnd)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setMaskHEnd(hEnd);
            return ;
        }
        fpga_value = CHROMA_MASK_HEND;
        break;
    case Keys::PATTERN:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->maskHEnd() != hEnd)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setMaskHEnd(hEnd);
            return ;
        }
        fpga_value = PATTERN_MASK_HEND;
        break;
    case Keys::PIP:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->maskHEnd() != hEnd)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setMaskHEnd(hEnd);
            return ;
        }
        fpga_value = PIP_MASK_HEND;
        break;
    default:
        return ;
    }
    per = hEnd;
    p = per / 100.0;
    p = (MASK_H_MAX - MASK_H_MIN) * p - MASK_H_MAX;
    value = getMaskPositionH(p);

    if(fpga_value != -1)
        fpga_write(&g_fpga,fpga_value,value);
}

void Models::setKeyMaskVEnd(QString keystr, int vEnd)
{
    int key = Keys::keyStringToIndex(keystr);
    int fpga_value = -1;
    float p;
    int value;
    int per;
    switch (key) {
    case Keys::LUMA:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->maskVEnd() != vEnd)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setMaskVEnd(vEnd);
            return ;
        }
        fpga_value = LUMA_KEY_MASK_VEND;
        break;
    case Keys::CHROMA:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->maskVEnd() != vEnd)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setMaskVEnd(vEnd);
            return ;
        }
        fpga_value = CHROMA_MASK_VEND;
        break;
    case Keys::PATTERN:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->maskVEnd() != vEnd)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setMaskVEnd(vEnd);
            return ;
        }
        fpga_value = PATTERN_MASK_VEND;
        break;
    case Keys::PIP:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->maskVEnd() != vEnd)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setMaskVEnd(vEnd);
            return ;
        }
        fpga_value = PIP_MASK_VEND;
        break;
    default:
        return ;
    }
    per = vEnd;
    p = per / 100.0;
    p = (MASK_V_MAX - MASK_V_MIN) * p - MASK_V_MAX;
    value = getMaskPositionV(p);

    if(fpga_value != -1)
        fpga_write(&g_fpga,fpga_value,value);
}

void Models::setKeyResize(QString keystr, bool resize)
{
    int key = Keys::keyStringToIndex(keystr);
    switch (key) {
    case Keys::LUMA:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->resize() != resize)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setResize(resize);
            return ;
        }
        break;
    case Keys::CHROMA:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->resize() != resize)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setResize(resize);
            return ;
        }
        break;
    case Keys::PATTERN:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->resize() != resize)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setResize(resize);
            return ;
        }
        break;
    default:
        return ;
    }
    setKeyCtrl(key);
}

void Models::setKeySize(QString keystr, QString size)
{
    int key = Keys::keyStringToIndex(keystr);
    int fpga_par = -1;
    int fpga_value = -1;
    switch (key) {
    case Keys::LUMA:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->size() != size)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setSize(size);
            return ;
        }
        fpga_par = LUMA_KEY_SIZE;
        break;
    case Keys::CHROMA:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->size() != size)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setSize(size);
            return ;
        }
        fpga_par = CHROMA_SIZE;
        break;
    case Keys::PATTERN:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->size() != size)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setSize(size);
            return ;
        }
        fpga_par = PATTERN_SIZE;
        break;
    case Keys::PIP:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->size() != size)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setSize(size);
            return ;
        }
        fpga_par = PIP_SIZE;
        break;
    default:
        return ;
    }
    int value = Keys::sizeStringToIndex(size);
    switch (value) {
    case Keys::KEY_SIZE_0_25:
        fpga_value = 0;
        break;
    case Keys::KEY_SIZE_0_33:
        fpga_value = 1;
        break;
    case Keys::KEY_SIZE_0_50:
        fpga_value = 2;
        break;
    }
    if(fpga_par != -1 && fpga_value != -1)
        fpga_write(&g_fpga,fpga_par,fpga_value);
}

void Models::setKeyXPosition(QString keystr, double xPosition)
{
    int key = Keys::keyStringToIndex(keystr);
    int fpga_value = -1;
    switch (key) {
    case Keys::LUMA:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->xPosition() != xPosition)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setXPosition(xPosition);
            return ;
        }
        fpga_value = LUMA_KEY_H_POS;
        break;
    case Keys::CHROMA:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->xPosition() != xPosition)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setXPosition(xPosition);
            return ;
        }
        fpga_value = CHROMA_H_POS;
        break;
    case Keys::PATTERN:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->xPosition() != xPosition)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setXPosition(xPosition);
            return ;
        }
        fpga_value = PATTERN_H_POS;
        break;
    case Keys::PIP:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->xPosition() != xPosition)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setXPosition(xPosition);
            return ;
        }
        fpga_value = PIP_H_POS;
        break;
    default:
        return ;
    }
    int value;
    value = getMaskPositionH(xPosition);

    if(fpga_value != -1)
        fpga_write(&g_fpga,fpga_value,value);
}

void Models::setKeyYPosition(QString keystr, double yPosition)
{
    int key = Keys::keyStringToIndex(keystr);
    int fpga_value = -1;
    switch (key) {
    case Keys::LUMA:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->yPosition() != yPosition)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setYPosition(yPosition);
            return ;
        }
        fpga_value = LUMA_KEY_V_POS;
        break;
    case Keys::CHROMA:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->yPosition() != yPosition)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setYPosition(yPosition);
            return ;
        }
        fpga_value = CHROMA_V_POS;
        break;
    case Keys::PATTERN:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->yPosition() != yPosition)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setYPosition(yPosition);
            return ;
        }
        fpga_value = PATTERN_V_POS;
        break;
    case Keys::PIP:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->yPosition() != yPosition)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setYPosition(yPosition);
            return ;
        }
        fpga_value = PIP_V_POS;
        break;
    default:
        return ;
    }
    int value;
    value = getMaskPositionV(yPosition);

    if(fpga_value != -1)
        fpga_write(&g_fpga,fpga_value,value);
}

void Models::setKeySourceFill(QString keystr, int fill)
{
    int key = Keys::keyStringToIndex(keystr);
    int fpga_value = -1;
    u_int16_t value = fill;;
    u_int8_t key_index = 0;
    LumaKeySource source = {};
    switch (key) {
    case Keys::LUMA:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->fillSource() != fill)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setFillSource(fill);
            return ;
        }
        fpga_value = LUMA_SRC_SEL;
        key_index = profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->keySource();
        //屏蔽color1、color2
        if(key_index > INPUT_SOURCE_STILL2_KEY)
            key_index +=2;
        source = {(u_int8_t)fill,key_index};
        value = *(u_int16_t*)&source;
        break;
    case Keys::CHROMA:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->fillSource() != fill)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setFillSource(fill);
            return ;
        }
        fpga_value = CHROMA_SRC_SEL;
        break;
    case Keys::PATTERN:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->fillSource() != fill)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->setFillSource(fill);
            return ;
        }
        fpga_value = PATTERN_SRC_SEL;
        break;
    case Keys::PIP:
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->fillSource() != fill)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->setFillSource(fill);
            return ;
        }
        fpga_value = PIP_SRC_SEL;
        break;
    }
    if(fpga_value != -1)
        fpga_write(&g_fpga,fpga_value,value);
}

void Models::setKeySourceKey(QString keystr, int sourceKey)
{
    int key = Keys::keyStringToIndex(keystr);
    if(key == Keys::LUMA)
    {
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->keySource() != sourceKey)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setKeySource(sourceKey);
            return ;
        }
        int fill = profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->fillSource();
        LumaKeySource source = {(u_int8_t)fill,(u_int8_t)sourceKey};
        fpga_write(&g_fpga,LUMA_SRC_SEL,*(u_int16_t*)&source);
    }
}

void Models::setKeyClip(QString keystr, int clip)
{
    int key = Keys::keyStringToIndex(keystr);
    if(key == Keys::LUMA)
    {
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->clip() != clip)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setClip(clip);
            return ;
        }
        int fpga_value = getKeyClip(clip);
        fpga_write(&g_fpga,LUMA_KEY_CLIP,fpga_value);
    }
}

void Models::setKeyGain(QString keystr, int gain)
{
    int key = Keys::keyStringToIndex(keystr);
    if(key == Keys::LUMA)
    {
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->gain() != gain)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setGain(gain);
            return ;
        }
        int fpga_value = getKeyGain(gain);
        fpga_write(&g_fpga,LUMA_KEY_GAIN,fpga_value);
    }
}

void Models::setKeyShapedKey(QString keystr, bool enable)
{
    int key = Keys::keyStringToIndex(keystr);
    if(key == Keys::LUMA)
    {
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->shapedKey() != enable)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setShapedKey(enable);
            return ;
        }
        setKeyCtrl(key);
    }
}

void Models::setKeyInvert(QString keystr, bool enable)
{
    int key = Keys::keyStringToIndex(keystr);
    if(key == Keys::LUMA)
    {
        if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->invert() != enable)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->setInvert(enable);
            return ;
        }
        setKeyCtrl(key);
    }
}

void Models::setKeyCtrl(int key)
{
    switch (key) {
    case Keys::LUMA:
    {
        LumaKeyCtrl ctl;
        int shapedKeyOn = profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->shapedKey()?1:0;
        shapedKeyOn == 0?ctl.shaped = 0:ctl.shaped = 1;
        int InvertOn = profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->invert()?1:0;
        InvertOn == 0?ctl.invert = 0:ctl.invert = 1;
        int maskOn = profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->maskEnable()?1:0;;
        maskOn == 0?ctl.mask = 0:ctl.mask = 1;
        int resizeOn = profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->resize()?1:0;
        resizeOn == 0?ctl.resize = 0:ctl.resize = 1;
        fpga_write(&g_fpga,LUMA_KEY_CTRL,*(u_int16_t*)&ctl);
    }
        break;
    case Keys::CHROMA:
    {
        int enable = profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->resize()?1:0;
        int mask = profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->maskEnable()?1:0;;
        int smp = profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->sample()?1:0;
        int value = 0;
        enable > 0?value += 16:value += 0;
        mask > 0?value += 2:value += 0;
        smp > 0?value += 4:value +=0;
        int G = settings->color5Data()[1];
        int B = settings->color5Data()[2];
        G > B ?value += 1:value +=0;
        fpga_write(&g_fpga,CHROMA_CTRL,value);
    }
        break;
    case Keys::PATTERN:
    {
        int pattern_index = profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->pattern()?1:0;
        int pattern_resize = profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->resize()?1:0;
        int pattern_mask = profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->maskEnable()?1:0;
        settings->setKeyPatternWipePatternIndex(pattern_index);
        int value = 0;
        pattern_resize != 0?value += 0x40:value += 0;
        pattern_mask != 0?value += 0x20:value += 0;
        value += pattern_index;
        fpga_write(&g_fpga,PATTERN_CTRL,value);
    }
        break;
    case Keys::PIP:
    {
        int value = 0;
        int mask = profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->maskEnable()?1:0;
        int board = profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->borderEnable()?1:0;
        mask > 0 ?value += 1:value += 0;
        board > 0 ?value += 2:value += 0;
        fpga_write(&g_fpga,PIP_CTRL,value);
    }
        break;
    }
}

void Models::setChromaKeySampleXPosition(double xPosition)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->smpXPosition() != xPosition)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setSmpXPosition(xPosition);
        return ;
    }
    double y = profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->smpYPosition();
    int x_value = getMaskPositionH(xPosition);
    int y_value = getMaskPositionV(y);
    fpga_write(&g_fpga,CHROMA_SMP_X_POS,x_value);
    fpga_write(&g_fpga,CHROMA_SMP_Y_POS,y_value);
}

void Models::setChromaKeySampleYPosition(double yPosition)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->smpYPosition() != yPosition)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setSmpYPosition(yPosition);
        return ;
    }
    double x = profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->smpXPosition();
    int x_value = getMaskPositionH(x);
    int y_value = getMaskPositionV(yPosition);
    fpga_write(&g_fpga,CHROMA_SMP_X_POS,x_value);
    fpga_write(&g_fpga,CHROMA_SMP_Y_POS,y_value);
}

void Models::setChromaKeySample(bool enable)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->sample() != enable)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setSample(enable);
        return ;
    }

    int resize = profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->resize();
    int mask = profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->maskEnable();
    int smp = enable;
    int value = 0;
    resize > 0?value += 16:value += 0;
    mask > 0?value += 2:value += 0;
    smp > 0?value += 4:value +=0;

    static QTimer *timer = nullptr;
    if(timer == nullptr)
    {
        timer = new QTimer();
        connect(timer,&QTimer::timeout,this,[=](){
            setChromaKeySampleColor();
        });
    }
    if(smp > 0)
    {
        timer->start(50);
    }
    else
    {
        timer->stop();
        //set data
        setChromaKeyProfile();
        //save color
//        settings->saveSMPColorAndColor5ToFile();
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setY(settings->chromakeySMPColorYcbcr()[0]);
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setCb(settings->chromakeySMPColorYcbcr()[1]);
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setCr(settings->chromakeySMPColorYcbcr()[2]);
    }
    int G = settings->color5Data()[1];
    int B = settings->color5Data()[2];
    G > B ?value += 1:value +=0;
    fpga_write(&g_fpga,CHROMA_CTRL,value);
}

void Models::setChromaKeySampleColorHue(int hue)
{

}

void Models::setChromaKeySampleColorSaturation(int saturation)
{

}

void Models::setChromaKeySampleColorBrightness(int brightness)
{

}

void Models::setChromaKeyForeground(int foreground)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->foreground() != foreground)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setForeground(foreground);
        return ;
    }
    setChromaKeyProfile();
}

void Models::setChromaKeyBackground(int background)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->background() != background)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setBackground(background);
        return ;
    }
    setChromaKeyProfile();
}

void Models::setChromaKeyKeyEdge(int keyEdge)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->keyEdge() != keyEdge)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->setKeyEdge(keyEdge);
        return ;
    }
    setChromaKeyProfile();
}

void Models::setTransitionRate(QString str, double rate)
{
    int index = TransitionStyle::styleStringToIndex(str);
    //////////////
    int outFormat = getOutFormat(profile->setting()->outFormat()->format()->outFormat());
    int fpga_value = getFTBRateValue(rate,outFormat);
    int fpga_par = -1;
    if(index == TransitionStyle::MIX)
    {
        fpga_par = MIX_RATE;
        if(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->mixParameters()->rate() != rate)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->mixParameters()->setRate(rate);
            return ;
        }
    }
    else if(index == TransitionStyle::DIP)
    {
        fpga_par = DIP_RATE;
        if(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->dipParameters()->rate() != rate)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->dipParameters()->setRate(rate);
            return ;
        }
    }
    else if(index == TransitionStyle::WIPE)
    {
        fpga_par = WIPE_RATE;
        if(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->rate() != rate)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->setRate(rate);
            return ;
        }
    }
    if(fpga_par != -1)
        fpga_write(&g_fpga,fpga_par,fpga_value);
}

void Models::setTransitionDipSource(int source)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->dipParameters()->input() != source)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->dipParameters()->setInput(source);
        return ;
    }
    fpga_write(&g_fpga,DIP_SRC_SEL,source);
}

void Models::setTransitionWipePattern(int pattern)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->pattern() != pattern)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->setPattern(pattern);
        return ;
    }

    bool derection = profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->reverseDirection();
//    //send settings signal
    settings->setTransitionWipePatternIndex(pattern);
    int value = 0;
    derection ?value += 0x20:value += 0;
    value += pattern;
    fpga_write(&g_fpga,WIPE_PATTERN,value);
}

void Models::setTransitionWipeXPosition(double xPosition)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->xPosition() != xPosition)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->setXPosition(xPosition);
        return ;
    }

    setTransitionWipePosition();
}

void Models::setTransitionWipeYPosition(double yPosition)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->yPosition() != yPosition)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->setYPosition(yPosition);
        return ;
    }

    setTransitionWipePosition();
}

void Models::setTransitionWipeDirection(bool direction)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->reverseDirection() != direction)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->setReverseDirection(direction);
        return ;
    }

    int pattern = profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->pattern();
//    //send settings signal
//    settings->setTransitionWipePatternIndex(pattern);
    int value = 0;
    direction ?value += 0x20:value += 0;
    value += pattern;
    fpga_write(&g_fpga,WIPE_PATTERN,value);
}

void Models::setTransitionWipeSymmetry(int symmetry)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->symmetry() != symmetry)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->setSymmetry(symmetry);
        return ;
    }

    setTransitionWipePosition();
}

void Models::setTransitionWipeSoftness(int softness)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->softness() != softness)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->setSoftness(softness);
        return ;
    }

    float f_softness = softness;
    float softnessMax = profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->softnessMax();
    float softnessMin = profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->softnessMin();

    f_softness = (f_softness - softnessMin) / (softnessMax - softnessMin);
    int value = getWipeSoftness(f_softness);
    fpga_write(&g_fpga,WIPE_SOFTNESS,value);
}

void Models::setTransitionWipeBorder(int border)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->border() != border)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->setBorder(border);
        return ;
    }

    float softness = profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->softness();
    float softnessMax = profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->softnessMax();
    float softnessMin = profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->softnessMin();

    softness = (softness - softnessMin) / (softnessMax - softnessMin);
    int value = getWipeSoftness(softness);

    float board = border;
    float boardMax = profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->borderMax();
    float boardMin = profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->borderMin();

    board = (board - boardMin) / (boardMax - boardMin);
    value = (int)(value * board + 0.5);
    fpga_write(&g_fpga,WIPE_BOARD,value);
}

void Models::setTransitionWipeFillSource(int fillSource)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->fillSource() != fillSource)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->setFillSource(fillSource);
        return ;
    }
    fpga_write(&g_fpga,WIPE_BOARD_SRC_SEL,fillSource);
}

void Models::setTransitionWipePosition()
{
    int pattern_index = profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->pattern();
    float posX = profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->xPosition();
    float posXMax = profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->xPositionMax();
    float posXMin = profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->xPositionMin();
    posX = (posX - posXMin) / (posXMax - posXMin);

    float posY = profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->yPosition();
    float posYMax = profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->yPositionMax();
    float posYMin = profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->yPositionMin();
    posY = (posY - posYMin) / (posYMax - posYMin);

    float symmertry = profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->symmetry();
    float symmertryMax = profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->symmetryMax();
    float symmertryMin = profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters()->symmetryMin();

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

void Models::setDskOnAir(bool onAir)
{
    if(profile->downstreamKeys()->downstreamKey()->onAir() != onAir)
    {
        profile->downstreamKeys()->downstreamKey()->setOnAir(onAir);
        return ;
    }

    if(!onAir && QSwitcher::get_led(KEY_LED_DSK_ON_AIR) != SWITCHER_LED_OFF){
        keyClick(KEY_LED_DSK_ON_AIR);
    }else if(onAir && QSwitcher::get_led(KEY_LED_DSK_ON_AIR) == SWITCHER_LED_OFF){
        keyClick(KEY_LED_DSK_ON_AIR);
    }
}

void Models::setDskSourceFill(int fill)
{
    if(profile->downstreamKeys()->downstreamKey()->fillSource() != fill)
    {
        profile->downstreamKeys()->downstreamKey()->setFillSource(fill);
        return ;
    }

    DSKSource dsk;
    int key = profile->downstreamKeys()->downstreamKey()->keySource();

    //屏蔽color1、color2
    if(key > INPUT_SOURCE_STILL2_KEY)
        key +=2;

    dsk.fill = fill;
    dsk.key = key;

    fpga_write(&g_fpga,FPGA_DSK_SRC_SEL,*(uint16_t*)&dsk);
}

void Models::setDskSourceKey(int key)
{
    if(profile->downstreamKeys()->downstreamKey()->keySource() != key)
    {
        profile->downstreamKeys()->downstreamKey()->setKeySource(key);
        return ;
    }

    DSKSource dsk;
    int fill = profile->downstreamKeys()->downstreamKey()->fillSource();

    //屏蔽color1、color2
    if(key > INPUT_SOURCE_STILL2_KEY)
        key +=2;

    dsk.fill = fill;
    dsk.key = key;

    fpga_write(&g_fpga,FPGA_DSK_SRC_SEL,*(uint16_t*)&dsk);
}

void Models::setDskMaskEnable(bool enable)
{
    if(profile->downstreamKeys()->downstreamKey()->maskEnable() != enable)
    {
        profile->downstreamKeys()->downstreamKey()->setMaskEnable(enable);
        return ;
    }
    setDskCtrl();
}

void Models::setDskMaskHStart(int hStart)
{
    if(profile->downstreamKeys()->downstreamKey()->maskHStart() != hStart)
    {
        profile->downstreamKeys()->downstreamKey()->setMaskHStart(hStart);
        return ;
    }
    float p;
    int value;
    p = hStart / 100.0;
    p = (MASK_H_MAX - MASK_H_MIN) * p - MASK_H_MAX;
    value = getMaskPositionH(p);
    fpga_write(&g_fpga,FPGA_DSK_MASK_HSTART,value);

}

void Models::setDskMaskVStart(int vStart)
{
    if(profile->downstreamKeys()->downstreamKey()->maskVStart() != vStart)
    {
        profile->downstreamKeys()->downstreamKey()->setMaskVStart(vStart);
        return ;
    }
    float p;
    int value;
    p = vStart / 100.0;
    p = (MASK_V_MAX - MASK_V_MIN) * p - MASK_V_MAX;
    value = getMaskPositionV(p);
    fpga_write(&g_fpga,FPGA_DSK_MASK_VSTART,value);
}

void Models::setDskMaskHEnd(int hEnd)
{
    if(profile->downstreamKeys()->downstreamKey()->maskHEnd() != hEnd)
    {
        profile->downstreamKeys()->downstreamKey()->setMaskHEnd(hEnd);
        return ;
    }
    float p;
    int value;
    p = hEnd / 100.0;
    p = (MASK_H_MAX - MASK_H_MIN) * p - MASK_H_MAX;
    value = getMaskPositionH(p);
    fpga_write(&g_fpga,FPGA_DSK_MASK_HEND,value);
}

void Models::setDskMaskVEnd(int vEnd)
{
    if(profile->downstreamKeys()->downstreamKey()->maskVEnd() != vEnd)
    {
        profile->downstreamKeys()->downstreamKey()->setMaskVEnd(vEnd);
        return ;
    }
    float p;
    int value;
    p = vEnd / 100.0;
    p = (MASK_V_MAX - MASK_V_MIN) * p - MASK_V_MAX;
    value = getMaskPositionV(p);
    fpga_write(&g_fpga,FPGA_DSK_MASK_VEND,value);
}

void Models::setDskShapedKey(bool enable)
{
    if(profile->downstreamKeys()->downstreamKey()->shapedKey() != enable)
    {
        profile->downstreamKeys()->downstreamKey()->setShapedKey(enable);
        return ;
    }
    setDskCtrl();
}

void Models::setDskClip(int clip)
{
    if(profile->downstreamKeys()->downstreamKey()->clip() != clip)
    {
        profile->downstreamKeys()->downstreamKey()->setClip(clip);
        return ;
    }
    int fpga_value = getKeyClip(clip);
    fpga_write(&g_fpga,FPGA_DSK_CLIP,fpga_value);
}

void Models::setDskGain(int gain)
{
    if(profile->downstreamKeys()->downstreamKey()->gain() != gain)
    {
        profile->downstreamKeys()->downstreamKey()->setGain(gain);
        return ;
    }
    int fpga_value = getKeyGain(gain);
    fpga_write(&g_fpga,FPGA_DSK_GAIN,fpga_value);
}

void Models::setDskInvert(bool invert)
{
    if(profile->downstreamKeys()->downstreamKey()->invert() != invert)
    {
        profile->downstreamKeys()->downstreamKey()->setInvert(invert);
        return ;
    }
    setDskCtrl();
}

void Models::setDskRate(double rate)
{
    ////////////////
    if(profile->downstreamKeys()->downstreamKey()->rate() != rate)
    {
        profile->downstreamKeys()->downstreamKey()->setRate(rate);
        return ;
    }
    int fpga_value = getFTBRateValue(rate,getOutFormat(profile->setting()->outFormat()->format()->outFormat()));
    fpga_write(&g_fpga,FPGA_DSK_RATE,fpga_value);
}

void Models::setDskCtrl()
{
    int value = 0;
    int mask = profile->downstreamKeys()->downstreamKey()->maskEnable();
    int shaped = profile->downstreamKeys()->downstreamKey()->shapedKey();
    int invert = profile->downstreamKeys()->downstreamKey()->invert();

    if(mask != 0)
        value += 4;
    if(shaped != 0)
        value += 1;
    if(invert != 0)
        value += 2;

    fpga_write(&g_fpga,FPGA_DSK_CTRL,value);
}

void Models::setFtbEnable(bool enable)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->ftb()->enable() != enable)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->ftb()->setEnable(enable);
        return ;
    }

    if(enable && QSwitcher::get_led(KEY_LED_TRANS_FTB) == SWITCHER_LED_OFF)
    {
        keyClick(KEY_LED_TRANS_FTB);
    }
    else if(!enable && QSwitcher::get_led(KEY_LED_TRANS_FTB) != SWITCHER_LED_OFF)
    {
        keyClick(KEY_LED_TRANS_FTB);
    }
}

void Models::setFtbRate(double rate)
{
    if(profile->mixEffectBlocks()->mixEffectBlock()->ftb()->rate() != rate)
    {
        profile->mixEffectBlocks()->mixEffectBlock()->ftb()->setRate(rate);
        return ;
    }

    /////////////
    int fpga_value = getFTBRateValue(rate,getOutFormat(profile->setting()->outFormat()->format()->outFormat()));
    fpga_write(&g_fpga,FTB_RATE,fpga_value);
}

void Models::setFtbAfv(bool afv)
{
    fpga_write(&g_fpga,FTB_AFV,afv?1:0);
}

void Models::setAudioEnable(QString source, int enable)
{
    int index = AudioSource::sourceNameStringToIndex(source);
    switch (index) {
    case AudioSource::MIC1:
    {
        if(profile->audioMixer()->audioInput()->mic1()->enable() != enable)
        {
            profile->audioMixer()->audioInput()->mic1()->setEnable(enable);
            return ;
        }
    }
        break;
    case AudioSource::MIC2:
    {
        if(profile->audioMixer()->audioInput()->mic2()->enable() != enable)
        {
            profile->audioMixer()->audioInput()->mic2()->setEnable(enable);
            return ;
        }
    }
        break;
    case AudioSource::IN1:
    {
        if(profile->audioMixer()->audioInput()->in1()->enable() != enable)
        {
            profile->audioMixer()->audioInput()->in1()->setEnable(enable);
            return ;
        }
    }
        break;
    case AudioSource::IN2:
    {
        if(profile->audioMixer()->audioInput()->in2()->enable() != enable)
        {
            profile->audioMixer()->audioInput()->in2()->setEnable(enable);
            return ;
        }
    }
        break;
    case AudioSource::IN3:
    {
        if(profile->audioMixer()->audioInput()->in3()->enable() != enable)
        {
            profile->audioMixer()->audioInput()->in3()->setEnable(enable);
            return ;
        }
    }
        break;
    case AudioSource::IN4:
    {
        if(profile->audioMixer()->audioInput()->in4()->enable() != enable)
        {
            profile->audioMixer()->audioInput()->in4()->setEnable(enable);
            return ;
        }
    }
        break;
    case AudioSource::AUX:
    {
        if(profile->audioMixer()->audioInput()->aux()->enable() != enable)
        {
            profile->audioMixer()->audioInput()->aux()->setEnable(enable);
            return ;
        }
    }
        break;
    }
    if(index >= AudioSource::MIC1 && index <= AudioSource::MIC2)
    {
        setAudioOn();
    }
    else if(index >= AudioSource::IN1 && index <= AudioSource::AUX)
    {
        setAudioAfv();
        setAudioOn();
    }
}

void Models::setAudioFader(QString source, double fader)
{
    int index = AudioSource::sourceNameStringToIndex(source);
    switch (index) {
    case AudioSource::MIC1:
    {
        if(profile->audioMixer()->audioInput()->mic1()->fader() != fader)
        {
            profile->audioMixer()->audioInput()->mic1()->setFader(fader);
            return ;
        }
    }
        break;
    case AudioSource::MIC2:
    {
        if(profile->audioMixer()->audioInput()->mic2()->fader() != fader)
        {
            profile->audioMixer()->audioInput()->mic2()->setFader(fader);
            return ;
        }
    }
        break;
    case AudioSource::IN1:
    {
        if(profile->audioMixer()->audioInput()->in1()->fader() != fader)
        {
            profile->audioMixer()->audioInput()->in1()->setFader(fader);
            return ;
        }
    }
        break;
    case AudioSource::IN2:
    {
        if(profile->audioMixer()->audioInput()->in2()->fader() != fader)
        {
            profile->audioMixer()->audioInput()->in2()->setFader(fader);
            return ;
        }
    }
        break;
    case AudioSource::IN3:
    {
        if(profile->audioMixer()->audioInput()->in3()->fader() != fader)
        {
            profile->audioMixer()->audioInput()->in3()->setFader(fader);
            return ;
        }
    }
        break;
    case AudioSource::IN4:
    {
        if(profile->audioMixer()->audioInput()->in4()->fader() != fader)
        {
            profile->audioMixer()->audioInput()->in4()->setFader(fader);
            return ;
        }
    }
        break;
    case AudioSource::AUX:
    {
        if(profile->audioMixer()->audioInput()->aux()->fader() != fader)
        {
            profile->audioMixer()->audioInput()->aux()->setFader(fader);
            return ;
        }
    }
        break;
    case AudioSource::PGM:
        if(profile->audioMixer()->audioOutput()->pgm()->fader() != fader)
        {
            profile->audioMixer()->audioOutput()->pgm()->setFader(fader);
            return ;
        }
        break;
    }
    setAudioVolumn(index);
}

void Models::setAudioBalance(QString source, double balance)
{
    int index = AudioSource::sourceNameStringToIndex(source);
    switch (index) {
    case AudioSource::MIC1:
    {
        if(profile->audioMixer()->audioInput()->mic1()->balance() != balance)
        {
            profile->audioMixer()->audioInput()->mic1()->setBalance(balance);
            return ;
        }
    }
        break;
    case AudioSource::MIC2:
    {
        if(profile->audioMixer()->audioInput()->mic2()->balance() != balance)
        {
            profile->audioMixer()->audioInput()->mic2()->setBalance(balance);
            return ;
        }
    }
        break;
    case AudioSource::IN1:
    {
        if(profile->audioMixer()->audioInput()->in1()->balance() != balance)
        {
            profile->audioMixer()->audioInput()->in1()->setBalance(balance);
            return ;
        }
    }
        break;
    case AudioSource::IN2:
    {
        if(profile->audioMixer()->audioInput()->in2()->balance() != balance)
        {
            profile->audioMixer()->audioInput()->in2()->setBalance(balance);
            return ;
        }
    }
        break;
    case AudioSource::IN3:
    {
        if(profile->audioMixer()->audioInput()->in3()->balance() != balance)
        {
            profile->audioMixer()->audioInput()->in3()->setBalance(balance);
            return ;
        }
    }
        break;
    case AudioSource::IN4:
    {
        if(profile->audioMixer()->audioInput()->in4()->balance() != balance)
        {
            profile->audioMixer()->audioInput()->in4()->setBalance(balance);
            return ;
        }
    }
        break;
    case AudioSource::AUX:
    {
        if(profile->audioMixer()->audioInput()->aux()->balance() != balance)
        {
            profile->audioMixer()->audioInput()->aux()->setBalance(balance);
            return ;
        }
    }
        break;
    }
    setAudioVolumn(index);
}

void Models::setAudioInput(QString source, double input)
{
    int index = AudioSource::sourceNameStringToIndex(source);
    switch (index) {
    case AudioSource::MIC1:
    {
        if(profile->audioMixer()->audioInput()->mic1()->input() != input)
        {
            profile->audioMixer()->audioInput()->mic1()->setInput(input);
            return ;
        }
        rv_switch_set_mic0_volume(input,input);
    }
        break;
    case AudioSource::MIC2:
    {
        if(profile->audioMixer()->audioInput()->mic2()->input() != input)
        {
            profile->audioMixer()->audioInput()->mic2()->setInput(input);
            return ;
        }
        rv_switch_set_mic1_volume(input,input);
    }
        break;
    case AudioSource::IN1:
    {
        if(profile->audioMixer()->audioInput()->in1()->input() != input)
        {
            profile->audioMixer()->audioInput()->in1()->setInput(input);
            return ;
        }
    }
        break;
    case AudioSource::IN2:
    {
        if(profile->audioMixer()->audioInput()->in2()->input() != input)
        {
            profile->audioMixer()->audioInput()->in2()->setInput(input);
            return ;
        }
    }
        break;
    case AudioSource::IN3:
    {
        if(profile->audioMixer()->audioInput()->in3()->input() != input)
        {
            profile->audioMixer()->audioInput()->in3()->setInput(input);
            return ;
        }
    }
        break;
    case AudioSource::IN4:
    {
        if(profile->audioMixer()->audioInput()->in4()->input() != input)
        {
            profile->audioMixer()->audioInput()->in4()->setInput(input);
            return ;
        }
    }
        break;
    case AudioSource::AUX:
    {
        if(profile->audioMixer()->audioInput()->aux()->input() != input)
        {
            profile->audioMixer()->audioInput()->aux()->setInput(input);
            return ;
        }
    }
        break;
    }
    setAudioVolumn(index);
}

void Models::setAudioDelay(QString source, int delay)
{
    int index = AudioSource::sourceNameStringToIndex(source);
    int fpag_value = -1;
    switch (index) {
    case AudioSource::MIC1:
    {
        if(profile->audioMixer()->audioInput()->mic1()->delay() != delay)
        {
            profile->audioMixer()->audioInput()->mic1()->setDelay(delay);
            return ;
        }
        fpag_value = FPGA_MIC1_DELAY;
    }
        break;
    case AudioSource::MIC2:
    {
        if(profile->audioMixer()->audioInput()->mic2()->delay() != delay)
        {
            profile->audioMixer()->audioInput()->mic2()->setDelay(delay);
            return ;
        }
        fpag_value = FPGA_MIC1_DELAY;
    }
        break;
    }
    if(fpag_value != -1)
        fpga_write(&g_fpga,fpag_value,delay);
}

void Models::setMonitorLevel(int level)
{
    if(profile->audioMixer()->audioOutput()->monitor()->level() != level)
    {
        profile->audioMixer()->audioOutput()->monitor()->setLevel(level);
        return ;
    }
    int value = getMonitorRegValue(level);
    fpga_write(&g_fpga,MON_LEVEL,value);
}
