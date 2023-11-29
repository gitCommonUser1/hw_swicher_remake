#ifndef KEYIMPLEMENT_H
#define KEYIMPLEMENT_H

#include "settings.h"
#include "MenuEnums.h"
#include "models.h"
#include "QSwitcher.h"
#include "leftmenumodel.h"
#include "media_sd.h"
#include "rv_switch_api.h"
#include "messagedialogcontrol.h"
#include "regexp.h"
#include "ndi.h"
#include "xmlprocessor.h"

#include <QTimer>

extern Settings*settings;
extern Models*models;
extern LeftMenuModel*leftMenuModel;
extern Media_sd *media_sd;
extern MessageDialogControl *messageDialogControl;
extern Ndi *ndi;

class KeyAbstractClass{
public:
    KeyAbstractClass(){

    }

    KeyAbstractClass(int index){
        key_index = index;
    }

    virtual void doWork(int status = 1) = 0;

protected:
    int key_index;
};


class KeyAudioInput:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;

        if(settings->listDialogVisible() || settings->keyboardVisible() || messageDialogControl->messageDialogVisible())
            return ;

        if(settings->lastFirstUnfold() == MENU_FIRST_AUDIO_MIXER && settings->lastSecondUnfold() == key_index - KEY_LED_AUDIO_MIC1)
            models->setMenuVisible(0);
        else
            models->setMenu(MENU_FIRST_AUDIO_MIXER,key_index - KEY_LED_AUDIO_MIC1);//AUDIO_MIXER_MIC1
    }
};

class keyAudioEnter:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        return;
    }
};

class KeyAudioAfv:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
//        if(status != 1)
//            return ;
//        if(settings->lastFirstUnfold() == MENU_FIRST_AUDIO_MIXER && settings->lastSecondUnfold() >= AUDIO_MIXER_IN1 && settings->lastSecondUnfold() <= AUDIO_MIXER_AUX){
//            int value = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[settings->lastSecondUnfold()]->third[IN_ENABLE]->current.toInt();
//            if(value == AUDIO_OFF || value == AUDIO_ON)
//                settings->setMenuValue(settings->lastFirstUnfold(),settings->lastSecondUnfold(),IN_ENABLE,AUDIO_AFV);
//            else if(value == AUDIO_AFV)
//                settings->setMenuValue(settings->lastFirstUnfold(),settings->lastSecondUnfold(),IN_ENABLE,AUDIO_OFF);
//        }
    }
};

class KeyAudioOn:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
//        if(status != 1)
//            return ;
//        if(settings->lastFirstUnfold() == MENU_FIRST_AUDIO_MIXER){
//            if(settings->lastSecondUnfold() >= AUDIO_MIXER_MIC1 && settings->lastSecondUnfold() <= AUDIO_MIXER_MIC2){
//                int value = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[settings->lastSecondUnfold()]->third[MIC_ENABLE]->current.toInt();
//                if(value == AUDIO_OFF){
//                    settings->setMenuValue(settings->lastFirstUnfold(),settings->lastSecondUnfold(),MIC_ENABLE,AUDIO_ON);
//                }else if(value == AUDIO_ON){
//                    settings->setMenuValue(settings->lastFirstUnfold(),settings->lastSecondUnfold(),MIC_ENABLE,AUDIO_OFF);
//                }
//            }else if(settings->lastSecondUnfold() >= AUDIO_MIXER_IN1 && settings->lastSecondUnfold() <= AUDIO_MIXER_AUX){
//                int value = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[settings->lastSecondUnfold()]->third[IN_ENABLE]->current.toInt();
//                if(value == AUDIO_OFF || value == AUDIO_AFV){
//                    settings->setMenuValue(settings->lastFirstUnfold(),settings->lastSecondUnfold(),IN_ENABLE,AUDIO_ON);
//                }else if(value == AUDIO_ON){
//                    settings->setMenuValue(settings->lastFirstUnfold(),settings->lastSecondUnfold(),IN_ENABLE,AUDIO_OFF);
//                }
//            }
//        }
    }
};


class KeyRecordStart:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;
//        models->sendKeySignalHasOneParameter(&Models::record,1);
    }
};

class KeyRecordStop:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;
//        models->sendKeySignalHasOneParameter(&Models::record,0);
    }
};

class KeyPlayStart:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;
//        models->sendKeySignalHasOneParameter(&Models::play,0);
    }
};

class KeyPlayStop:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;
//        models->sendKeySignalHasOneParameter(&Models::play,1);
    }
};

class KeyPlayNext:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;

        if(settings->pgmTally() & 0x10)
            return ;

//        models->sendKeySignal(&Models::playNext);
    }
};

class KeyPlayPrevious:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;

        if(settings->pgmTally() & 0x10)
            return ;

//        models->sendKeySignal(&Models::playPrevious);
    }
};

class KeyLiveStream:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;
        int enable = settings->liveStatus();
        if(enable == 0)
        {
            // open
            int stream1 = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_OUTPUT]->current.toInt();
            int stream2 = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM2]->third[MENU_THIRD_STREAM_OUTPUT]->current.toInt();
            int stream3 = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM3]->third[MENU_THIRD_STREAM_OUTPUT]->current.toInt();
            if(!(stream1 == OUTPUT_DISABLE && stream2 == OUTPUT_DISABLE && stream3 == OUTPUT_DISABLE))
            {
//                if(settings->liveStreamStatus1() == E_STATUS_CLOSE && settings->liveStreamStatus2() == E_STATUS_CLOSE && settings->liveStreamStatus3() == E_STATUS_CLOSE)
//                    models->sendKeySignalHasOneParameter(&Models::liveStatus,1);
            }
        }
        else
        {
            // close
//            models->sendKeySignalHasOneParameter(&Models::liveStatus,0);
        }
    }
};


class KeyMem:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        qDebug() << "_____keyMem index:" << key_index << "clicked!";
    }
};


class KeyLedMenu:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;
        if(messageDialogControl->messageDialogVisible()){
            if(!messageDialogControl->options().isEmpty())
                messageDialogControl->cancel();
            return ;
        }
        if(settings->listDialogVisible()){
            settings->setListDialogVisible(0);
            return ;
        }
        if(settings->keyboardVisible()){
            if(settings->keyboardKetMoveFlag()){
                models->sendKey(settings->keyboardCurrentIndex());
                return ;
            }
        }
        if(settings->menuVisible() == settings->MENU_HIDE)
        {
            if(settings->lastFirstUnfold() == MENU_FIRST_AUDIO_MIXER && settings->lastSecondUnfold() != -1){
                models->setMenuVisible(settings->MENU_SHOW_SMALL);
            }else{
                models->setMenuVisible(settings->MENU_SHOW_BIG);
            }
            return ;
        }
        if(settings->lastThirdUnfold() != -1)
        {
            //如果aux source没有确定选中，恢复之前的选项
            if(settings->lastFirstUnfold() == MENU_FIRST_SETTING && settings->lastSecondUnfold() == SETTING_AUX_SOURCE)
            {
                int index = settings->lastThirdUnfold();
                if(index == SETTING_AUX_SOURCE_SOURCE){
                    if(settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_AUX_SOURCE]->third[SETTING_AUX_SOURCE_SOURCE]->current.toInt() != settings->reallyAuxSourceIndex())
                        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_AUX_SOURCE,SETTING_AUX_SOURCE_SOURCE,settings->reallyAuxSourceIndex());
                }
                if(index == SETTING_AUX_SOURCE_IN1){
                    if(settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_AUX_SOURCE]->third[SETTING_AUX_SOURCE_IN1]->current.toInt() != settings->reallySrcSelectionIn1())
                        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_AUX_SOURCE,SETTING_AUX_SOURCE_IN1,settings->reallySrcSelectionIn1());
                }
                if(index == SETTING_AUX_SOURCE_IN2){
                    if(settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_AUX_SOURCE]->third[SETTING_AUX_SOURCE_IN2]->current.toInt() != settings->reallySrcSelectionIn2())
                        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_AUX_SOURCE,SETTING_AUX_SOURCE_IN2,settings->reallySrcSelectionIn2());
                }
                if(index == SETTING_AUX_SOURCE_IN3){
                    if(settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_AUX_SOURCE]->third[SETTING_AUX_SOURCE_IN3]->current.toInt() != settings->reallySrcSelectionIn3())
                        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_AUX_SOURCE,SETTING_AUX_SOURCE_IN3,settings->reallySrcSelectionIn3());
                }
                if(index == SETTING_AUX_SOURCE_IN4){
                    if(settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_AUX_SOURCE]->third[SETTING_AUX_SOURCE_IN4]->current.toInt() != settings->reallySrcSelectionIn4())
                        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_AUX_SOURCE,SETTING_AUX_SOURCE_IN4,settings->reallySrcSelectionIn4());
                }
            }
            //如果out source没有确定选中，恢复之前的选项
            if(settings->lastFirstUnfold() == MENU_FIRST_SETTING && settings->lastSecondUnfold() == SETTING_OUT_SOURCE)
            {
                int index = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_SOURCE]->third[settings->lastThirdUnfold()]->current.toInt();
                if(settings->lastThirdUnfold() == SETTING_OUT_SOURCE_HDMI1)
                {
                    if(index != settings->reallyOutSourceHDMI1())
                        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_OUT_SOURCE,SETTING_OUT_SOURCE_HDMI1,settings->reallyOutSourceHDMI1());
                }
//                else if(settings->lastThirdUnfold() == SETTING_OUT_SOURCE_HDMI2)
//                {
//                    if(index != settings->reallyOutSourceHDMI2())
//                        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_OUT_SOURCE,SETTING_OUT_SOURCE_HDMI2,settings->reallyOutSourceHDMI2());
//                }
                else if(settings->lastThirdUnfold() == SETTING_OUT_SOURCE_AUX)
                {
                    if(index != settings->reallyOutSourceUVC())
                        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_OUT_SOURCE,SETTING_OUT_SOURCE_AUX,settings->reallyOutSourceUVC());
                }
            }
            //如果out format没有确定选中，恢复之前的选项
            if(settings->lastFirstUnfold() == MENU_FIRST_SETTING && settings->lastSecondUnfold() == SETTING_OUT_FORMAT && settings->lastThirdUnfold() == SETTING_OUT_FORMAT_FORMAT)
            {
                int index = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_FORMAT]->third[SETTING_OUT_FORMAT_FORMAT]->current.toInt();
                    if(index != settings->reallyOutFormat())
                        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_OUT_FORMAT,SETTING_OUT_FORMAT_FORMAT,settings->reallyOutFormat());
            }
            //如果out format color space没有确定选中，恢复之前的选项
            if(settings->lastFirstUnfold() == MENU_FIRST_SETTING && settings->lastSecondUnfold() == SETTING_OUT_FORMAT && settings->lastThirdUnfold() != SETTING_OUT_FORMAT_FORMAT)
            {
                int index = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_FORMAT]->third[settings->lastThirdUnfold()]->current.toInt();
                if(settings->lastThirdUnfold() == SETTING_OUT_FORMAT_OUTPUT1_COLOR_SPACE)
                {
                    if(index != settings->reallyOutput1ColorSpace())
                        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_OUT_FORMAT,SETTING_OUT_FORMAT_OUTPUT1_COLOR_SPACE,settings->reallyOutput1ColorSpace());
                }
                else if(settings->lastThirdUnfold() == SETTING_OUT_FORMAT_OUTPUT2_COLOR_SPACE)
                {
                    if(index != settings->reallyOutput2ColorSpace())
                        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_OUT_FORMAT,SETTING_OUT_FORMAT_OUTPUT2_COLOR_SPACE,settings->reallyOutput2ColorSpace());
                }
            }
            models->cancelRightMenu();
            return ;
        }
        if(settings->lastSecondUnfold() != -1)
        {
            models->quitRightMenu();
            return ;
        }
        if(settings->lastFirstUnfold() != -1)
        {
//                models->menuFold();
            settings->setLastFirstUnfold(-1);
            return ;
        }
        models->setMenuVisible(settings->MENU_HIDE);
    }
};


class KeyMenuEnter:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status == 1){
            if(messageDialogControl->messageDialogVisible()){
                messageDialogControl->buttonClicked();
            }else if(settings->keyboardVisible()){
                settings->setKeyIsPressed(1);
                models->sendKey(settings->keyboardCurrentIndex());
            }else if(settings->listDialogVisible()){
                if(settings->reallyAuxSourceIndex() == AUX_SOURCE_SD_CARD){
                    // sd card
                    if(settings->playList().size() == 0 || settings->playListDialogCurrent() >= settings->playList().size())
                        return ;
                    if(settings->recordFileName() == SD_VIDEO_PATH + settings->playList()[settings->playListDialogCurrent()])
                        return ;
                    settings->setListDialogVisible(0);
                    settings->setPlayListCurrent(settings->playListDialogCurrent());
                    QTimer::singleShot(50,nullptr,[=](){
                        int playback_mode = settings->listFirst()[MENU_FIRST_PLAYBACK]->second[PLAYBACK_PLAYBACK]->third[MENU_THIRD_PLAYBACK_MODE]->current.toInt();
                        if(playback_mode == SEQUENTIAL && settings->playLedStatus() != E_STATUS_MP4_CLOSE){
                                settings->setPlayAutoNextFlag(1);
                                models->playStop();
                        }else{
                            models->playStart();
                        }
                        models->playPause(0);
                    });
                }else if(settings->reallyAuxSourceIndex() == AUX_SOURCE_NDI){
                    // ndi
                    if(ndi->ndiList().size() > settings->ndiListDialogCurrent())
                        ndi->connectNdi(ndi->ndiList()[settings->ndiListDialogCurrent()]);
                    settings->setListDialogVisible(0);
                }
            }else{
                enterMenu(settings->leftListViewCurrent());
            }
        }else if(status == 0){
            settings->setKeyIsPressed(0);
        }
    }
    void enterMenu(int index){
        MenuItem item = leftMenuModel->m_items[index];
        int first = settings->lastFirstUnfold();
        int second = settings->lastSecondUnfold();
        int third = settings->lastThirdUnfold();
        if(second == -1)
        {
            if(item.level() == MENU_LEVEL_FIRST)
            {
                //一级菜单操作
                if(item.unfold() == 0)
                {
                    if(first != -1)
                    {
                        //先收起
    //                    models->menuFold();
                        settings->setLastFirstUnfold(-1);
                    }
                    //展开
    //                models->menuUnfold(item.index());
                    settings->setLastFirstUnfold(item.index());
                }
                else
                {
                    //收起
    //                models->menuFold();
                    settings->setLastFirstUnfold(-1);
                }
            }
            else if(item.level() == MENU_LEVEL_SECOND)
            {
                //二级菜单操作
                models->enterRightMenu();
            }
        }
        else
        {
            if(third == -1)
            {
                //进入三级菜单

                //不可点
                if(settings->liveStatus() != 0 && first == MENU_FIRST_STREAM)
                {
                    //推流开启，设置Stream
                    return ;
                }

                if(first == MENU_FIRST_SETTING &&
                   second == SETTING_RESET &&
                   settings->rightListViewCurrent() == SETTING_RESET_FACTORY)
                {
                    messageDialogControl->dialogShow(QObject::tr("This will restart the device."),{QObject::tr("Cancel"),QObject::tr("Confirm")},MessageDialogControl::MESSAGE_FACTORY_RESET);
                    return ;
                }

                if(first == MENU_FIRST_SETTING &&
                   second == SETTING_RESET &&
                   settings->rightListViewCurrent() == SETTING_RESET_SD_FORMAT)
                {
                    messageDialogControl->dialogShow(QObject::tr("Your SD card will be formatted."),{QObject::tr("Cancel"),QObject::tr("Confirm")},MessageDialogControl::MESSAGE_SD_FORMAT);
                    return ;
                }

                //super source 只有当Style选择不为Crop时，Y position才可以调整
                if(first == MENU_FIRST_SUPER_SOURCE &&
                   second == SUPER_SOURCE_CONTROL &&
                   settings->rightListViewCurrent() == SUPER_CONTROL_Y_POSITION &&
                   settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_CONTROL]->third[SUPER_CONTROL_STYLE]->current.toInt() == CROP)
                {
                    return ;
                }

                //可点
                models->chooseRightMenu();
                third = settings->lastThirdUnfold();
                if(first == MENU_FIRST_SETTING &&
                        second == SETTING_RECORD_FILE &&
                        third == SETTING_RECORD_FILE_NAME)
                {
                    //record file
                    settings->setKeyboardReg(REG_FILE_NAME);
                    settings->setKeyboardVisible(1);
                    auto item = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_RECORD_FILE]->third[SETTING_RECORD_FILE_NAME];
                    settings->setKeyboardInputText(models->languages[item->name],item->current.toString());
                }
                else if(first == MENU_FIRST_SETTING &&
                         second == SETTING_NETWORK &&
                         third != NETWORK_PROTOCOL)
                {
                    int protocol = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_PROTOCOL]->current.toInt();
                    if(protocol == NETWORK_PROTOCOL_STATIC_IP)
                    {
                        settings->setKeyboardVisible(1,1);
                        auto item = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[third];
                        settings->setKeyboardInputText(models->languages[item->name],item->current.toString());
                    }
                }
                else if(first == MENU_FIRST_STREAM &&
                        third == MENU_THIRD_STREAM_KEY)
                {
                    settings->setKeyboardVisible(1);
                    auto item = settings->listFirst()[MENU_FIRST_STREAM]->second[second]->third[MENU_THIRD_STREAM_KEY];
                    settings->setKeyboardInputText(models->languages[item->name],item->current.toString());
                }
                else if(first == MENU_FIRST_SETTING &&
                        second == SETTING_SRC_NAME){
                    settings->setKeyboardReg(REG_SRC_NAME);
                    settings->setKeyboardVisible(1);
                    auto item = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_SRC_NAME]->third[third];
                    settings->setKeyboardInputText(models->languages[item->name],item->current.toString());
                }
            }
            else
            {
                //退出三级菜单

                if(first == MENU_FIRST_STILL_GENERATOR &&
                   second == STILL_GENERATE_UPLOAD &&
                   third == STILL_UPLOAD_LOAD_PICTURE)
                {
                    //still  load  picture
//                    models->setStillUpload();
                    models->loadStill1();
                }
                if(first == MENU_FIRST_MACRO &&
                   second == MACRO_MACRO &&
                   third == MENU_THIRD_MACRO_SLEEP)
                {
                    //macro send sleep
                    int ms = settings->listFirst()[MENU_FIRST_MACRO]->second[MACRO_MACRO]->third[MENU_THIRD_MACRO_SLEEP]->current.toInt();
//                    models->sendKeySignalHasOneParameter(&Models::mSleep,ms,false);
                }
                if(first == MENU_FIRST_SETTING &&
                   second == SETTING_AUX_SOURCE &&
                   third == SETTING_AUX_SOURCE_SOURCE)
                {
//                    models->sendMenuSignal(&Models::auxSource,first,second,third);
                }
                if(first == MENU_FIRST_SETTING &&
                   second == SETTING_AUX_SOURCE &&
                   third != SETTING_AUX_SOURCE_SOURCE)
                {
//                    models->sendMenuSignalHasOneParameter(&Models::srcSelection,first,second,third);
                }
                if(first == MENU_FIRST_SETTING &&
                   second == SETTING_OUT_FORMAT &&
                    third == SETTING_OUT_FORMAT_FORMAT)
                {
                    messageDialogControl->dialogShow(QObject::tr("This will restart the device."),{QObject::tr("Cancel"),QObject::tr("Confirm")},MessageDialogControl::MESSAGE_OUT_FORMAT);
                }
                if(first == MENU_FIRST_SETTING &&
                   second == SETTING_OUT_FORMAT &&
                   third != SETTING_OUT_FORMAT_FORMAT)
                {
//                    models->sendMenuSignalHasOneParameter(&Models::colorSpace,first,second,third);
                }
                if(first == MENU_FIRST_SETTING &&
                   second == SETTING_OUT_SOURCE)
                {
//                    models->sendMenuSignal(&Models::outSource,first,second,third);
                }
                if(first == MENU_FIRST_STREAM &&
                   third == MENU_THIRD_STREAM_UPLOAD_KEY)
                {
                    models->setLoadStreamKey(second);
//                    models->sendMenuSignalHasOneParameter(&Models::loadStreamKey,first,second,third);
                }
                if(first == MENU_FIRST_MACRO &&
                   second == MACRO_MACRO &&
                   third == MENU_THIRD_MACRO_IMPORT)
                {
                    models->macroImport();
                }
                if(first == MENU_FIRST_MACRO &&
                   second == MACRO_MACRO &&
                   third == MENU_THIRD_MACRO_EXPORT)
                {
                    models->macroExport();
                }
                models->cancelRightMenu();
            }
        }
    }
};


class KeyPgm:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;
//        models->sendKeySignalHasOneParameter(&Models::pgmIndex,key_index - KEY_LED_PGM_1,false);
    }
};

class KeyPvw:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;
//        models->sendKeySignalHasOneParameter(&Models::pvwIndex,key_index - KEY_LED_PVW_1,false);
    }
};

class KeyCut:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;
//        models->sendKeySignal(&Models::cutTransition,false);
    }
};

class KeyAuto:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;
//        models->sendKeySignal(&Models::autoTransition,false);
    }
};

class KeyFtb:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;
//        models->sendKeySignal(&Models::ftb,false);
    }
};

class KeyPrev:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;
        int flag = QSwitcher::get_led(KEY_LED_TRANS_PREVIEW);
//        models->sendKeySignalHasOneParameter(&Models::prev,flag,false);
    }
};

class KeyTransition:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;
//        models->sendKeySignalHasOneParameter(&Models::transitionIndex,key_index - KEY_LED_TRANS_MIX,false);
    }
};

class KeyKey:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;
        int key_flag,dsk_flag,bkgd_flag,value;
        switch (key_index) {
        case KEY_LED_KEY:
            dsk_flag = QSwitcher::get_led(KEY_LED_DSK);
            bkgd_flag = QSwitcher::get_led(KEY_LED_BKGD);
            if(dsk_flag == SWITCHER_LED_OFF && bkgd_flag == SWITCHER_LED_OFF)
                key_flag = SWITCHER_LED_ON;
            else
                key_flag = QSwitcher::get_led(KEY_LED_KEY);
            break;
        case KEY_LED_DSK:
            key_flag = QSwitcher::get_led(KEY_LED_KEY);
            bkgd_flag = QSwitcher::get_led(KEY_LED_BKGD);
            if(key_flag == SWITCHER_LED_OFF && bkgd_flag == SWITCHER_LED_OFF)
                dsk_flag = SWITCHER_LED_ON;
            else
                dsk_flag = QSwitcher::get_led(KEY_LED_DSK);
            break;
        case KEY_LED_BKGD:
            key_flag = QSwitcher::get_led(KEY_LED_KEY);
            dsk_flag = QSwitcher::get_led(KEY_LED_DSK);
            if(key_flag == SWITCHER_LED_OFF && dsk_flag == SWITCHER_LED_OFF)
                bkgd_flag = SWITCHER_LED_ON;
            else
                bkgd_flag = QSwitcher::get_led(KEY_LED_BKGD);
            break;
        }
        value = 0;
        if(key_flag)
            value += 1;
        if(dsk_flag)
            value += 2;
        if(bkgd_flag)
            value += 4;
        qDebug() << "value: " << value;
//        models->sendKeySignalHasOneParameter(&Models::transitionSource,value,false);
    }
};

class KeyKeyOnAir:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;

        int flag = QSwitcher::get_led(KEY_LED_KEY_ON_AIR);
//        models->sendKeySignalHasOneParameter(&Models::keyOnAir,flag,false);
    }
};


class KeyDskOnAir:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;

        int flag = QSwitcher::get_led(KEY_LED_DSK_ON_AIR);
//        models->sendKeySignalHasOneParameter(&Models::dskOnAir,flag,false);
    }
};


#endif // KEYIMPLEMENT_H