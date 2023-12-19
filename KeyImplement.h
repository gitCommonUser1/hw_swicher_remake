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
#include "gostreamsystem.h"
#include "profile_include.h"
#include <QTimer>
#include "playbackgroupmanager.h"

extern Settings*settings;
extern Models*models;
extern LeftMenuModel*leftMenuModel;
extern Media_sd *media_sd;
extern MessageDialogControl *messageDialogControl;
extern Ndi *ndi;
extern Profile *profile;
extern PlaybackGroupManager* playbackGroupManager;


//点击时记住最后一次值，如果没有确定修改返回则恢复
static int lastValue = 0;

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
        if(status != 1)
            return;
        int second = settings->lastSecondUnfold();
        if(settings->lastFirstUnfold() == MENU_FIRST_AUDIO_MIXER)
        {
            switch (second) {
            case AUDIO_MIXER_MIC1:
                profile->audioMixer()->audioInput()->mic1()->setFader(0);
                break;
            case AUDIO_MIXER_MIC2:
                profile->audioMixer()->audioInput()->mic2()->setFader(0);
                break;
            case AUDIO_MIXER_IN1:
                profile->audioMixer()->audioInput()->in1()->setFader(0);
                break;
            case AUDIO_MIXER_IN2:
                profile->audioMixer()->audioInput()->in2()->setFader(0);
                break;
            case AUDIO_MIXER_IN3:
                profile->audioMixer()->audioInput()->in3()->setFader(0);
                break;
            case AUDIO_MIXER_IN4:
                profile->audioMixer()->audioInput()->in4()->setFader(0);
                break;
            case AUDIO_MIXER_AUX:
                profile->audioMixer()->audioInput()->aux()->setFader(0);
                break;
            case AUDIO_MIXER_PGM:
                profile->audioMixer()->audioOutput()->pgm()->setFader(0);
                break;
            }
        }

    }
};

class KeyAudioAfv:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;
        if(settings->lastFirstUnfold() == MENU_FIRST_AUDIO_MIXER && settings->lastSecondUnfold() >= AUDIO_MIXER_IN1 && settings->lastSecondUnfold() <= AUDIO_MIXER_AUX){
            int value = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[settings->lastSecondUnfold()]->third[IN1_ENABLE]->current.toInt();
            if(value == AudioSource::ON || value == AudioSource::OFF)
                settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[settings->lastSecondUnfold()]->third[IN1_ENABLE]->doWork(AudioSource::AFV);
            else if(value == AudioSource::AFV)
                settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[settings->lastSecondUnfold()]->third[IN1_ENABLE]->doWork(AudioSource::OFF);
        }
    }
};

class KeyAudioOn:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;
        if(settings->lastFirstUnfold() == MENU_FIRST_AUDIO_MIXER){
            if(settings->lastSecondUnfold() >= AUDIO_MIXER_MIC1 && settings->lastSecondUnfold() <= AUDIO_MIXER_MIC2){
                int value = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[settings->lastSecondUnfold()]->third[MIC1_ENABLE]->current.toInt();
                if(value == AudioSource::OFF){
                    settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[settings->lastSecondUnfold()]->third[MIC1_ENABLE]->doWork(AudioSource::ON);
                }else if(value == AudioSource::ON){
                    settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[settings->lastSecondUnfold()]->third[MIC1_ENABLE]->doWork(AudioSource::OFF);
                }
            }else if(settings->lastSecondUnfold() >= AUDIO_MIXER_IN1 && settings->lastSecondUnfold() <= AUDIO_MIXER_AUX){
                int value = settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[settings->lastSecondUnfold()]->third[IN1_ENABLE]->current.toInt();
                if(value == AudioSource::OFF || value == AudioSource::AFV){
                    settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[settings->lastSecondUnfold()]->third[IN1_ENABLE]->doWork(AudioSource::ON);
                }else if(value == AudioSource::ON){
                    settings->listFirst()[MENU_FIRST_AUDIO_MIXER]->second[settings->lastSecondUnfold()]->third[IN1_ENABLE]->doWork(AudioSource::OFF);
                }
            }
        }
    }
};


class KeyRecordStart:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;
        models->recordStart();
    }
};

class KeyRecordStop:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;
        models->recordStop();
    }
};

class KeyPlayStart:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;
        models->playPause(0);
    }
};

class KeyPlayStop:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;
//        models->sendKeySignalHasOneParameter(&Models::play,1);
        models->playPause(1);
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
        models->setPlayNext();
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
        models->setPlayPrevious();
    }
};

class KeyLiveStream:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;
        settings->setLiveStatus(!settings->liveStatus());
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
            }else{
                settings->setKeyboardVisible(0);
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
            auto menuType = settings->listFirst()[settings->lastFirstUnfold()]->second[settings->lastSecondUnfold()]->third[settings->lastThirdUnfold()]->menuType;
            if(menuType == ENTER_CALL_OR_RESET)
            {
                settings->setMenuValue(settings->lastFirstUnfold(),settings->lastSecondUnfold(),settings->lastThirdUnfold(),lastValue);
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
                if(profile->setting()->srcSelections()->aux()->selection() == SrcSelections::SD_CARD){
                    // sd card
                    settings->setPlayingIndex(playbackGroupManager->listCurrent());
                    models->playPause(1);
                    models->playStart();
                    settings->setListDialogVisible(0);
                }else if(profile->setting()->srcSelections()->aux()->selection() == SrcSelections::NDI){
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
            auto menuType = settings->listFirst()[first]->second[second]->third[settings->rightListViewCurrent()]->menuType;
            if(third == -1)
            {
                //进入三级菜单
                switch (menuType) {
                case CHANGED_CALL:
                    models->chooseRightMenu();
                    break;
                case ENTER_CALL_OR_RESET:
                    models->chooseRightMenu();
                    lastValue = settings->listFirst()[first]->second[second]->third[settings->rightListViewCurrent()]->current.toInt();
                    break;
                case ENTER_CALL:
                    models->chooseRightMenu();
                    lastValue = settings->listFirst()[first]->second[second]->third[settings->rightListViewCurrent()]->current.toInt();
                    break;
                case EVENT_CALL:
                    settings->listFirst()[first]->second[second]->third[settings->rightListViewCurrent()]->doEvent();
                    break;
                }

//                //super source 只有当Style选择不为Crop时，Y position才可以调整
//                if(first == MENU_FIRST_SUPER_SOURCE &&
//                   second == SUPER_SOURCE_CONTROL &&
//                   settings->rightListViewCurrent() == SUPER_CONTROL_Y_POSITION &&
//                   settings->listFirst()[MENU_FIRST_SUPER_SOURCE]->second[SUPER_SOURCE_CONTROL]->third[SUPER_CONTROL_STYLE]->current.toInt() == CROP)
//                {
//                    return ;
//                }
            }
            else
            {
                //退出三级菜单
//                if(first == MENU_FIRST_MACRO &&
//                   second == MACRO_MACRO &&
//                   third == MENU_THIRD_MACRO_SLEEP)
//                {
//                    //macro send sleep
//                    int ms = settings->listFirst()[MENU_FIRST_MACRO]->second[MACRO_MACRO]->third[MENU_THIRD_MACRO_SLEEP]->current.toInt();
////                    models->sendKeySignalHasOneParameter(&Models::mSleep,ms,false);
//                }
//                if(first == MENU_FIRST_MACRO &&
//                   second == MACRO_MACRO &&
//                   third == MENU_THIRD_MACRO_IMPORT)
//                {
//                    models->macroImport();
//                }
//                if(first == MENU_FIRST_MACRO &&
//                   second == MACRO_MACRO &&
//                   third == MENU_THIRD_MACRO_EXPORT)
//                {
//                    models->macroExport();
//                }
                switch (menuType) {
                case CHANGED_CALL:
                    break;
                case ENTER_CALL_OR_RESET:
                    settings->listFirst()[first]->second[second]->third[third]->doEvent();
                    break;
                case ENTER_CALL:
                    settings->listFirst()[first]->second[second]->third[third]->doEvent();
                    break;
                case EVENT_CALL:
                    break;
                }
                models->cancelRightMenu();
            }
        }
    }
};


class KeyPgm:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 2){
        if(status != 2)
            return ;
//        models->sendKeySignalHasOneParameter(&Models::pgmIndex,key_index - KEY_LED_PGM_1,false);
        profile->mixEffectBlocks()->mixEffectBlock()->program()->setInput(key_index - KEY_LED_PGM_1);
    }
};

class KeyPvw:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;
//        models->sendKeySignalHasOneParameter(&Models::pvwIndex,key_index - KEY_LED_PVW_1,false);
        profile->mixEffectBlocks()->mixEffectBlock()->preview()->setInput(key_index - KEY_LED_PVW_1);
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
        if(SWITCHER_LED_R == QSwitcher::get_led(KEY_LED_TRANS_AUTO)){
            //red
            if(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->style() == TransitionStyle::styleIndexToString(TransitionStyle::DIP) &&
               profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->dipParameters()->input() == TransitionStyle::AUX){
                models->playPause(0);
            }
        }else if(SWITCHER_LED_W == QSwitcher::get_led(KEY_LED_TRANS_AUTO)){
            //white
//            if(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->style() == TransitionStyle::styleIndexToString(TransitionStyle::DIP) &&
//               profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->dipParameters()->input() == TransitionStyle::AUX){
//                models->playPause(1);
//            }
        }
    }
};

class KeyFtb:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->ftb()->setEnable(true);
        }
        else
        {
            profile->mixEffectBlocks()->mixEffectBlock()->ftb()->setEnable(false);
        }
    }
};

class KeyPrev:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->setPreviewTransition(true);
        }
        else
        {
            profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->setPreviewTransition(false);
        }
    }
};

class KeyTransition:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status != 1)
            return ;
        profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->setStyle(TransitionStyle::styleIndexToString(key_index - KEY_LED_TRANS_MIX));
    }
};

class KeyKey:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
//        int key_flag,dsk_flag,bkgd_flag,value;
        int value = 0;
        if(QSwitcher::get_led(KEY_LED_BKGD) != SWITCHER_LED_OFF)
            value += 0b100;
        if(QSwitcher::get_led(KEY_LED_DSK) != SWITCHER_LED_OFF)
            value += 0b010;
        if(QSwitcher::get_led(KEY_LED_KEY) != SWITCHER_LED_OFF)
            value += 0b001;
        qDebug() << "value:" <<value;
        profile->mixEffectBlocks()->mixEffectBlock()->nextTransition()->setSelection(NextTransition::selectionValueToString(value));
    }
};

class KeyKeyOnAir:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status == 1)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->key()->setOnAir(true);
        }
        else
        {
            profile->mixEffectBlocks()->mixEffectBlock()->keys()->key()->setOnAir(false);
        }
    }
};


class KeyDskOnAir:public KeyAbstractClass{
public:
    using KeyAbstractClass::KeyAbstractClass;
    void doWork(int status = 1){
        if(status == 1)
        {
            profile->downstreamKeys()->downstreamKey()->setOnAir(true);
        }
        else
        {
            profile->downstreamKeys()->downstreamKey()->setOnAir(false);
        }
    }
};


#endif // KEYIMPLEMENT_H
