#include <QObject>
#include "control.h"
#include "settings.h"
#include "leftmenumodel.h"
#include "models.h"
#include "QSwitcher.h"
#include "MenuEnums.h"
#include <QDebug>
#include <QDateTime>
#include "rv_switch_api.h"
#include "rightmenumodel.h"
#include "keyboardevent.h"
#include "AuxCallback.h"
#include "media_sd.h"
#include <QTimer>
#include "KeyImplement.h"
#include "messagedialogcontrol.h"
#include "qthread_route.h"
#include "include/net_opr.h"
#include "dhcp_event.h"
#include "qthread_uevent.h"
#include "timeZone.h"
#include <QtConcurrent>
#include "ndi.h"
#include "ndi_license.h"
#include "gostreamsystem.h"
#include "profile_include.h"

extern Settings *settings;
extern LeftMenuModel *leftMenuModel;
extern RightMenuModel *rightMenuModel;
extern QSwitcher *switcher;
extern Models *models;
extern KeyboardEvent *keyboardEvent;
extern Media_sd *media_sd;
extern MessageDialogControl *messageDialogControl;
extern Ndi *ndi;
extern Profile *profile;

//键盘按下时间 ms
#define KEY_PRESS_INTERVAL 2000
//自动按键频率 ms
#define KEY_AUTO_PRESS_INTERVAL 100

Control::Control(QObject *parent) : QObject(parent)
{
    init_connect();
    connect_profile();

    //init audio led
    models->closeAllAudioLed();
    int resolution = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_FORMAT]->third[SETTING_OUT_FORMAT_FORMAT]->current.toInt();
    rv_switch_init(models->getOutFormatIndexForEnum(resolution));
    //5.18版本固定发1080p60，之后的版本改回来
//    rv_switch_init(0x4);
    rv_switch_init_video_enc();
}

void Control::init_connect()
{
    connect(switcher,&QSwitcher::knobChanged,this,&Control::slotKnobChanged,Qt::BlockingQueuedConnection);
    connect(switcher,&QSwitcher::keyChanged,this,&Control::slotKeyChanged,Qt::BlockingQueuedConnection);
    connect(switcher,&QSwitcher::pushChanged,this,&Control::slotPushChanged,Qt::BlockingQueuedConnection);
    connect(switcher,&QSwitcher::keyStatusChanged,this,&Control::slotKeyStatusChanged,Qt::BlockingQueuedConnection);

    //record time  live time
    QTimer *recordTimer = new QTimer;
    QTimer *liveTimer = new QTimer;
    connect(recordTimer,&QTimer::timeout,this,[=](){
        int second = models->sd_remaintime_calc();
        settings->setRecordSecond(settings->recordSecond()+1);
        media_sd->checkGbFree(second);
        if(second < 1)
        {
            models->setRecord(0);
            return ;
        }
#define RECORD_MAX_SECOND 3600 * 5
        if(settings->recordSecond() >= RECORD_MAX_SECOND)
        {
            models->setRecord(0);
            return ;
        }
    });

    connect(liveTimer,&QTimer::timeout,this,[=](){
        settings->setLiveSecond(settings->liveSecond()+1);
    });

    connect(settings,&Settings::recordSecondChanged,this,[=](){
    settings->setRecordTimeStr(Media_sd::secondToHMS(settings->recordSecond()));
//        settings->setRecordTimeStr(QDateTime::fromSecsSinceEpoch(settings->recordSecond(),Qt::UTC).toString("hh:mm:ss"));
        qDebug() << settings->recordTimeStr();
    });

    connect(settings,&Settings::liveSecondChanged,this,[=](){
        settings->setLiveTimeStr(Media_sd::secondToHMS(settings->liveSecond()));
//        settings->setLiveTimeStr(QDateTime::fromSecsSinceEpoch(settings->liveSecond(),Qt::UTC).toString("hh:mm:ss"));
        qDebug() << settings->liveTimeStr();
    });

    //player led controls
    connect(settings,&Settings::recordLedStatusChanged,this,[=](int status){
        if(status == E_STATUE_SUCCESS){
            QSwitcher::set_led(KEY_LED_RECORDER_REC,SWITCHER_LED_ON);
            recordTimer->start(1000);
        }else if(status == E_STATUS_FAILED){
            QSwitcher::set_led(KEY_LED_RECORDER_REC,SWITCHER_LED_OFF);
            settings->setRecordSecond(0);
            if(settings->playList().size() == 1 && settings->playLedStatus() == E_STATUS_MP4_CLOSE){
                    models->playPause(1);
                    models->playStart();
            }
        }else if(status == E_STATUS_PROCESS){
//            QSwitcher::set_led(KEY_LED_RECORDER_REC,SWITCHER_LED_OFF,LEDS_BLINK_2Hz);
        }
    });

    connect(settings,&Settings::recordFileNameChanged,this,[=](QString name){
        if(name == ""){
            recordTimer->stop();
        }
    });

    connect(settings,&Settings::liveLedStatusChanged,this,[=](int status){
//        qDebug() << "liveLedStatusChanged:" << status;

        bool enable1,enable2,enable3;
        enable1 = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_OUTPUT]->current.toInt() == OUTPUT_ENABLE;
        enable2 = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM2]->third[MENU_THIRD_STREAM_OUTPUT]->current.toInt() == OUTPUT_ENABLE;
        enable3 = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM3]->third[MENU_THIRD_STREAM_OUTPUT]->current.toInt() == OUTPUT_ENABLE;

        int liveStatus = settings->liveStatus();
        if(liveStatus != 0)
        {
            //start status
            int led_status = E_STATUE_SUCCESS;
            if(enable1 && settings->liveStreamStatus1() != E_STATUE_SUCCESS)
            {
                led_status = settings->liveStreamStatus1();
            }
            if(enable2 && settings->liveStreamStatus2() != E_STATUE_SUCCESS)
            {
                led_status = settings->liveStreamStatus2();
            }
            if(enable3 && settings->liveStreamStatus3() != E_STATUE_SUCCESS)
            {
                led_status = settings->liveStreamStatus3();
            }

            if(led_status != E_STATUE_SUCCESS)
            {
                QSwitcher::set_led(KEY_LED_LIVE,SWITCHER_LED_ON,LEDS_BLINK_0_5Hz);
            }
            else
            {
                QSwitcher::set_led(KEY_LED_LIVE,SWITCHER_LED_ON);
            }
        }
        else
        {
            //close status
            int led_status = E_STATUS_CLOSE;
            if(enable1 && settings->liveStreamStatus1() != E_STATUS_CLOSE)
            {
                led_status = settings->liveStreamStatus1();
            }
            if(enable2 && settings->liveStreamStatus2() != E_STATUS_CLOSE)
            {
                led_status = settings->liveStreamStatus2();
            }
            if(enable3 && settings->liveStreamStatus3() != E_STATUS_CLOSE)
            {
                led_status = settings->liveStreamStatus3();
            }
            if(led_status != E_STATUS_CLOSE)
            {
                QSwitcher::set_led(KEY_LED_LIVE,SWITCHER_LED_ON,LEDS_BLINK_0_5Hz);
            }
            else
            {
                QSwitcher::set_led(KEY_LED_LIVE,SWITCHER_LED_OFF);
            }
        }
        //
    });

    connect(settings,&Settings::liveStatusChanged,this,[=](int status){
        if(status == 0){
            //close
            models->closeAllStream();
        }else{
            //open
            models->openAllStream();
        }
    });

    connect(settings,&Settings::playLedStatusChanged,this,[=](int status){
        qDebug() << "<<<<<<<<<playLedStatusChanged:" << status;
        qDebug() << "<<<<<<<<<<<QSwitcher::get_led(KEY_LED_PGM_AUX):" << QSwitcher::get_led(KEY_LED_PGM_AUX);
        int playback_mode = settings->listFirst()[MENU_FIRST_PLAYBACK]->second[PLAYBACK_PLAYBACK]->third[MENU_THIRD_PLAYBACK_MODE]->current.toInt();
        int auxSource = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_AUX_SOURCE]->third[SETTING_AUX_SOURCE_SOURCE]->current.toInt();
        if(status == E_STATUS_MP4_CLOSE && SWITCHER_LED_R == QSwitcher::get_led(KEY_LED_PGM_AUX)){
            if(!media_sd->is_online() || auxSource != AUX_SOURCE_SD_CARD || playback_mode != SEQUENTIAL || (playback_mode == SEQUENTIAL && settings->isPlayListLast()))
                models->setCutTransition();
        }

        // auto next if playback mode is Sequential
        if(status == E_STATUS_MP4_CLOSE && playback_mode == SEQUENTIAL ){//&& rv_swithc_mp4_get_end_state() != 0
//            models->setPlayNext();
            if(settings->playAutoNextFlag())
                models->playStart();
            else
                models->autoPlayNext();
            settings->setPlayAutoNextFlag(0);
        }
    });

    QTimer *timer1 = new QTimer;
    timer1->setSingleShot(true);
    timer1->setInterval(KEY_PRESS_INTERVAL);
    QTimer *timer2 = new QTimer;
    connect(timer1,&QTimer::timeout,this,[=](){
        timer2->start(KEY_AUTO_PRESS_INTERVAL);
    });
    connect(timer2,&QTimer::timeout,this,[=](){
        models->sendKey(settings->keyboardCurrentIndex());
    });
    //键盘长按
    connect(settings,&Settings::keyIsPressedChanged,this,[=](int status){
        //1 pressed    0 released
        if(status == 1){
            timer1->start();
        }else if(status == 0){
            timer1->stop();
            timer2->stop();
        }
    });


    //菜单大小
    connect(settings,&Settings::menuVisibleChanged,this,[=](int visible){
        if(visible == settings->MENU_HIDE){
            settings->setAudioVisible(1);
            settings->setMenuSize(0);
            models->setUMDVisivle(1);
            models->setMvMeterVisivle(1);
        }else if(visible == settings->MENU_SHOW_BIG){
            settings->setAudioVisible(0);
            settings->setMenuSize(1);
            models->setUMDVisivle(0);
            models->setMvMeterVisivle(0);
        }else if(visible == settings->MENU_SHOW_SMALL){
            settings->setAudioVisible(1);
            settings->setMenuSize(1);
            models->setUMDVisivle(0);
            models->setMvMeterVisivle(0);
        }
    });

    //messageDialog visible changed
    connect(messageDialogControl,&MessageDialogControl::messageDialogVisibleChanged,this,[=](bool flag){
        if(flag){
            //show
            settings->setAudioVisible(0);
            settings->setMenuSize(1);
            models->setUMDVisivle(0);
            models->setMvMeterVisivle(0);
        }else{
            settings->menuVisibleChanged(settings->menuVisible());
        }
    });

    //playListDialog visible changed
    connect(settings,&Settings::listDialogVisibleChanged,this,[=](bool flag){
        if(flag){
            //show
            settings->setAudioVisible(0);
            settings->setMenuSize(1);
            models->setUMDVisivle(0);
            models->setMvMeterVisivle(0);

            if(settings->reallyAuxSourceIndex() == AUX_SOURCE_SD_CARD){
                settings->setPlayListDialogVisible(1);
            }else if(settings->reallyAuxSourceIndex() == AUX_SOURCE_NDI){
                settings->setNdiListDialogVisible(1);
            }
        }else{
            settings->menuVisibleChanged(settings->menuVisible());
            settings->setPlayListDialogVisible(0);
            settings->setNdiListDialogVisible(0);
        }
    });

    connect(settings,&Settings::ndiListDialogVisibleChanged,this,[=](bool flag){
#define NDI_SEARCH_SPACE_TIME 5000
#define NDI_SEARCH_RESULT_WATI_TIME 1000
        static QTimer *timer = nullptr;
        std::function<void()> func = [=](){
            ndi->searchNdiList(NDI_SEARCH_RESULT_WATI_TIME);
        };
        if(flag){
            if(!timer){
                timer = new QTimer();
                connect(timer,&QTimer::timeout,this,func);
            }
            func();
            timer->start(NDI_SEARCH_SPACE_TIME);
        }else{
            if(timer){
                timer->stop();
            }
        }
    });

    connect(settings,&Settings::menuSizeChanged,this,[=](int menuSize){
        qDebug() << "menuSize: " <<menuSize;
        models->setMenuSize(menuSize);
    });
    //

    connect(settings,&Settings::lastFirstUnfoldChanged,this,[=](int index){
        if(index != -1){
            models->menuUnfold(index);
        }else{
            models->menuFold();
        }
    });

    connect(settings,&Settings::lastSecondUnfoldChanged,this,[=](int index){
        if(settings->lastFirstUnfold() != MENU_FIRST_AUDIO_MIXER){

        }
        else{
            if(index <= AUDIO_MIXER_PGM){
                models->menuAudioSelectChanged(index);

                if(index != -1){
                    //如果是大菜单，缩小
                    if(settings->menuVisible() == settings->MENU_SHOW_BIG){
                        settings->setMenuVisible(settings->MENU_SHOW_SMALL);
                    }
                }else{
                    //如果是小菜单，放大
                    if(settings->menuVisible() == settings->MENU_SHOW_SMALL){
                        settings->setMenuVisible(settings->MENU_SHOW_BIG);
                    }
                }
            }
        }
    });

    connect(settings,&Settings::leftListViewCurrentChanged,this,[=](){
        if(leftMenuModel->m_items[settings->leftListViewCurrent()].level() == MENU_LEVEL_SECOND){
            //停留至二级菜单
            models->flushRightMenu();
        }
    });

    //三级菜单修改信号          //int first,int second,int third   这里只更新ui    2023.11.22
    connect(settings,&Settings::thirdMenuValueChanged,this,[=](int first,int second,int third){
//        settings->listFirst()[first]->second[second]->third[third]->doWork(first,second,third);
        if(settings->lastFirstUnfold() == first && settings->leftListViewCurrent() == first + second + 1)
            models->changeRightMenu(first,second,third);
        //特殊情况，音频修改
        if(settings->lastFirstUnfold() == MENU_FIRST_AUDIO_MIXER){
            models->menuAudioEnableChnaged(second,third);
        }
    });

    //三级菜单状态改变
    connect(settings,&Settings::lastThirdUnfoldChanged,this,[=](){
        //关闭键盘
        if(settings->keyboardVisible()){
            settings->setKeyboardVisible(0);
        }
    });

    //网络
    connect(&qthread_route,&QThreadRoute::emit_netlink_change,this,[=](uint32_t flags, QString netif){
        if (netif == ETHERNET_NETIF)
            {
                if (_is_net_link(flags))
                {
                    //
                }
                else
                {
                    int index = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_PROTOCOL]->current.toInt();
                    if(index == NETWORK_PROTOCOL_DHCP)
                    {
                        settings->setMenuDHCPNetwork("","","","","");
                    }
                }
            }
    });
    connect(&qthread_route,&QThreadRoute::emit_netaddr_change,this,[=](bool is_new, QString netif, QString ipaddr, int prefixlen){
        if (netif == ETHERNET_NETIF)
        {
            int index = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_PROTOCOL]->current.toInt();
            if(index == NETWORK_PROTOCOL_DHCP)
            {
                if (is_new)
                {
                    char _mask[16];
                    prefixlen2mask(prefixlen, _mask, 16);
                    settings->setMenuDHCPNetworkIpAddress(ipaddr);
                    settings->setMenuDHCPNetworkMask(_mask);
                }
                else
                {
                    settings->setMenuDHCPNetworkIpAddress("");
                    settings->setMenuDHCPNetworkMask("");
                }
            }
            if(is_new)
            {
                //network connect
                QtConcurrent::run([=](){
                    ntp_date_reset_thread();
                    setTimeZone();
                });
            }
        }
    });
    connect(&qthread_route,&QThreadRoute::emit_netroute_change,this,[=](bool is_add, QString netif, QString gateway){
        if (netif == ETHERNET_NETIF)
        {
            int index = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_PROTOCOL]->current.toInt();
            if(index == NETWORK_PROTOCOL_DHCP)
            {
                if (is_add)
                {
                    settings->setMenuDHCPNetworkGateway(gateway);
                }
                else
                {
                    settings->setMenuDHCPNetworkGateway("");
                }
            }
        }
    });
    connect(&dhcp_event,&DhcpEvent::emit_dhcp_change,this,[=](bool is_add, QString netif){
        if (netif == ETHERNET_NETIF)
        {
            int index = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_NETWORK]->third[NETWORK_PROTOCOL]->current.toInt();
            if(index == NETWORK_PROTOCOL_DHCP)
            {
                char c_dns1[16], c_dns2[16];
                get_dns(netif.toStdString().data(), c_dns1, 16, c_dns2, 16);
                settings->setMenuDHCPNetworkPrimaryDNS(c_dns1);
                settings->setMenuDHCPNetworksecondayDNS(c_dns2);
            }
        }
    });
}

void Control::connect_profile()
{
    //先把所有值初始化
    profile->read(profile);

    //color back
    connect(profile->colorBacks()->colorBack1(),&ColorBack::hueChanged,this,[=](int hue){
        models->macroInvoke(&Models::colorBackHue,ColorBacks::COLOR1,hue);
        settings->setMenuValue(MENU_FIRST_COLOR_BACK,COLOR_BACK_COLOR1,COLORBACK1_HUE,hue);
    });
    connect(profile->colorBacks()->colorBack1(),&ColorBack::saturationChanged,this,[=](int saturation){
        models->macroInvoke(&Models::colorBackSaturation,ColorBacks::COLOR1,saturation);
        settings->setMenuValue(MENU_FIRST_COLOR_BACK,COLOR_BACK_COLOR1,COLORBACK1_SATURATION,saturation);
    });
    connect(profile->colorBacks()->colorBack1(),&ColorBack::brightnessChanged,this,[=](int brightness){
        models->macroInvoke(&Models::colorBackBrightness,ColorBacks::COLOR1,brightness);
        settings->setMenuValue(MENU_FIRST_COLOR_BACK,COLOR_BACK_COLOR1,COLORBACK1_BRIGHTNESS,brightness);
    });
    connect(profile->colorBacks()->colorBack2(),&ColorBack::hueChanged,this,[=](int hue){
        models->macroInvoke(&Models::colorBackHue,ColorBacks::COLOR2,hue);
        settings->setMenuValue(MENU_FIRST_COLOR_BACK,COLOR_BACK_COLOR2,COLORBACK2_HUE,hue);
    });
    connect(profile->colorBacks()->colorBack2(),&ColorBack::saturationChanged,this,[=](int saturation){
        models->macroInvoke(&Models::colorBackSaturation,ColorBacks::COLOR2,saturation);
        settings->setMenuValue(MENU_FIRST_COLOR_BACK,COLOR_BACK_COLOR2,COLORBACK2_SATURATION,saturation);
    });
    connect(profile->colorBacks()->colorBack2(),&ColorBack::brightnessChanged,this,[=](int brightness){
        models->macroInvoke(&Models::colorBackBrightness,ColorBacks::COLOR2,brightness);
        settings->setMenuValue(MENU_FIRST_COLOR_BACK,COLOR_BACK_COLOR2,COLORBACK2_BRIGHTNESS,brightness);
    });

    //mixEffectBlocks
    //pgm
    connect(profile->mixEffectBlocks()->mixEffectBlock()->program(),&Program::inputChanged,this,[=](int input){
        models->macroInvoke(&Models::pgmIndex,input);
    });
    //pvw
    connect(profile->mixEffectBlocks()->mixEffectBlock()->preview(),&Preview::inputChanged,this,[=](int input){
        models->macroInvoke(&Models::pvwIndex,input);
    });
    //nextTransition
    connect(profile->mixEffectBlocks()->mixEffectBlock()->nextTransition(),&NextTransition::selectionChanged,this,[=](QString selection){
        models->macroInvoke(&Models::nextTransition,selection);
    });
    //TransitionStyle
    connect(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle(),&TransitionStyle::styleChanged,this,[=](QString style){
        models->macroInvoke(&Models::transitionStyle,style);
    });
    //previewTransition
    connect(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle(),&TransitionStyle::previewTransitionChanged,this,[=](bool flag){
        models->macroInvoke(&Models::previewTransition,flag);
    });
    //MixParameters
    connect(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->mixParameters(),&MixParameters::rateChanged,this,[=](double rate){
        models->macroInvoke(&Models::transitionRate,TransitionStyle::MIX,rate);
        settings->setMenuValue(MENU_FIRST_TRANSITION,TRANSITION_MIX,TRANSITION_MIX_RATE,rate);
    });
    //DipParameters
    connect(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->dipParameters(),&DipParameters::rateChanged,this,[=](double rate){
        models->macroInvoke(&Models::transitionRate,TransitionStyle::DIP,rate);
        settings->setMenuValue(MENU_FIRST_TRANSITION,TRANSITION_DIP,TRANSITION_DIP_RATE,rate);
    });
    //DipParameters
    connect(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->dipParameters(),&DipParameters::inputChanged,this,[=](int input){
        models->macroInvoke(&Models::transitionDipSource,input);
        settings->setMenuValue(MENU_FIRST_TRANSITION,TRANSITION_DIP,TRANSITION_DIP_SOURCE,input);
    });
    //WipeParameters
    connect(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters(),&WipeParameters::rateChanged,this,[=](double rate){
        models->macroInvoke(&Models::transitionRate,TransitionStyle::WIPE,rate);
        settings->setMenuValue(MENU_FIRST_TRANSITION,TRANSITION_WIPE,TRANSITION_WIPE_RATE,rate);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters(),&WipeParameters::patternChanged,this,[=](int pattern){
        models->macroInvoke(&Models::transitionWipePattern,pattern);
        settings->setMenuValue(MENU_FIRST_TRANSITION,TRANSITION_WIPE,TRANSITION_WIPE_PATTERN,pattern);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters(),&WipeParameters::symmetryChanged,this,[=](int symmetry){
        models->macroInvoke(&Models::transitionWipeSymmetry,symmetry);
        settings->setMenuValue(MENU_FIRST_TRANSITION,TRANSITION_WIPE,TRANSITION_WIPE_SYMMERTRY,symmetry);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters(),&WipeParameters::xPositionChanged,this,[=](double xPosition){
        models->macroInvoke(&Models::transitionWipeXPosition,xPosition);
        settings->setMenuValue(MENU_FIRST_TRANSITION,TRANSITION_WIPE,TRANSITION_WIPE_POSITIONX,xPosition);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters(),&WipeParameters::yPositionChanged,this,[=](double yPosition){
        models->macroInvoke(&Models::transitionWipeYPosition,yPosition);
        settings->setMenuValue(MENU_FIRST_TRANSITION,TRANSITION_WIPE,TRANSITION_WIPE_POSITIONY,yPosition);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters(),&WipeParameters::reverseDirectionChanged,this,[=](bool direction){
        models->macroInvoke(&Models::transitionWipeDirection,direction);
        settings->setMenuValue(MENU_FIRST_TRANSITION,TRANSITION_WIPE,TRANSITION_WIPE_DIRECTION,direction?1:0);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters(),&WipeParameters::softnessChanged,this,[=](int softness){
        models->macroInvoke(&Models::transitionWipeSoftness,softness);
        settings->setMenuValue(MENU_FIRST_TRANSITION,TRANSITION_WIPE,TRANSITION_WIPE_SOFTNESS,softness);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters(),&WipeParameters::borderChanged,this,[=](int border){
        models->macroInvoke(&Models::transitionWipeBorder,border);
        settings->setMenuValue(MENU_FIRST_TRANSITION,TRANSITION_WIPE,TRANSITION_WIPE_BOARD,border);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters(),&WipeParameters::fillSourceChanged,this,[=](int source){
        models->macroInvoke(&Models::transitionWipeFillSource,source);
        settings->setMenuValue(MENU_FIRST_TRANSITION,TRANSITION_WIPE,TRANSITION_WIPE_FILL_SOURCE,source);
    });

    //第二遍读取，防止发多次信号
    profile->read(profile);
}

void Control::slotKnobChanged(const int knob, int value)
{
    //    qDebug() << "knob:" << knob << "  value:" << value;
//        if (value > 0)
//            value = 1;
//        else if (value < 0)
//            value = -1;
        switch (knob) {
        case KNOB_MENU:
            if(settings->keyIsPressed())
                return ;
            if(messageDialogControl->messageDialogVisible()){
                if(value < 0 && messageDialogControl->optionCurrent() != 0){
                    messageDialogControl->setOptionCurrent(messageDialogControl->optionCurrent() - 1);
                }else if(value > 0 && messageDialogControl->optionCurrent() < messageDialogControl->options().size() - 1){
                    messageDialogControl->setOptionCurrent(messageDialogControl->optionCurrent() + 1);
                }
            }
            else if(settings->keyboardVisible()){
                if(settings->keyboardKetMoveFlag()){
                    //-2 left   -1 right
//                    models->sendKey(value>0?-1:-2);
                    if(value > 0){
                        while(value--)
                            models->sendKey(-1);
                    }else if(value < 0){
                        while(value++)
                            models->sendKey(-2);
                    }
                }else{
                    settings->setKeyboardCurrentIndex(settings->keyboardCurrentIndex() + value);
                }
            }else if(settings->listDialogVisible()){
                int size = 0;
                int current = 0;
                if(settings->reallyAuxSourceIndex() == AUX_SOURCE_SD_CARD){
                    size = settings->playList().size();
                    current = settings->playListDialogCurrent();
                }
                else if(settings->reallyAuxSourceIndex() == AUX_SOURCE_NDI){
                    size = ndi->ndiList().size();
                    current = settings->ndiListDialogCurrent();
                }
                if(size < 1)
                    return ;
                int finalValue = 0;
                if(value > 0){
                    finalValue = current + value;
                    if(finalValue >= size)
                        finalValue = size - 1;
                }else if(value < 0){
                    finalValue = current + value;
                    if(finalValue < 0)
                        finalValue = 0;
                }
                if(settings->reallyAuxSourceIndex() == AUX_SOURCE_SD_CARD){
                    settings->setPlayListDialogCurrent(finalValue);
                }else if(settings->reallyAuxSourceIndex() == AUX_SOURCE_NDI){
                    settings->setNdiListDialogCurrent(finalValue);
                }
            }else{
                models->menuKnob(value);
            }
            break;
        case KNOB_AUDIO:
            if(settings->lastFirstUnfold() == MENU_FIRST_AUDIO_MIXER && settings->lastSecondUnfold() != -1)
                models->setAudioFader(value);
            else
                models->setMonitorLevel(value);
            break;
        }
}

void Control::slotPushChanged(const int push, const int value)
{
    qDebug() << "push:" << push << "  value:" << value;
//    models->sendKeySignalHasOneParameter(&Models::transitionPosition,value,false);
    models->macroInvoke((&Models::transitionPosition),value);
}
void Control::slotKeyChanged(const int key, const int value)
{
    //key keyOnAir dsk dskOnAir bkgd prev等键转交灯状态信号处理        mem key另外处理
    if(key != KEY_LED_TRANS_PREVIEW
        && key != KEY_LED_KEY_ON_AIR
        && key != KEY_LED_DSK_ON_AIR
        && key != KEY_LED_KEY
        && key != KEY_LED_DSK
        && key != KEY_LED_BKGD
        && key != KEY_LED_PLAYER_PLAY
        && key != KEY_AUDIO_ENTER
        && key != KEY_LED_MEMU
        && !(key >= KEY_LED_MEM1 && key <= KEY_LED_MEM8)){
        //
        settings->listKey()[key]->doWork(value);

    }else if(key >= KEY_LED_MEM1 && key <= KEY_LED_MEM8){
        //mem key  long pressed  and  short pressed
        static int key_pressed_index = 0;
        static int t_pressed = 0;
        static QTimer *timer = nullptr;
        if(timer == nullptr){
            timer = new QTimer;
            connect(timer,&QTimer::timeout,this,[=](){
                t_pressed++;
                if(t_pressed >= 2){
                    qDebug() << "_______" << key_pressed_index << "  pressed:long" ;
                    timer->stop();
                    t_pressed = 0;
                    settings->listKey()[key_pressed_index]->doWork(1);
                }
            });
        }
        if(value == 1){
            //pressed
            key_pressed_index = key;
            timer->start(1000);
        }else if(value == 0){
            //released
            if(timer->isActive()){
                qDebug() << "_______" << key_pressed_index << "  pressed:short" ;
                timer->stop();
                t_pressed = 0;
                settings->listKey()[key]->doWork(0);
            }
        }
    }else if(key == KEY_LED_PLAYER_PLAY){
        static QTimer *timer = nullptr;
        static int t_pressed = 0;
        if(timer == nullptr){
            timer = new QTimer;
            connect(timer,&QTimer::timeout,this,[=](){
                t_pressed++;
                if(t_pressed >= 2){
                    qDebug() << "_________ play  pressed:long" ;
                    timer->stop();
                    t_pressed = 0;
                    if(!messageDialogControl->messageDialogVisible() && !settings->keyboardVisible()){
                        int auxSource = settings->reallyAuxSourceIndex() ;
                        if(auxSource == AUX_SOURCE_SD_CARD || auxSource == AUX_SOURCE_NDI){
                            settings->setListDialogVisible(1);
                        }
                    }

                }
            });
        }
        if(value == 1){
            //pressed
            timer->start(1000);
        }else if(value == 0){
            //released
            if(timer->isActive()){
                qDebug() << "_________ play  pressed:short" ;
                timer->stop();
                t_pressed = 0;
                settings->listKey()[key]->doWork();
            }
        }
    }else if(key == KEY_AUDIO_ENTER){
        static QTimer *timer = nullptr;
        static int t_pressed = 0;
        if(timer == nullptr){
            timer = new QTimer;
            connect(timer,&QTimer::timeout,this,[=](){
                t_pressed++;
                if(t_pressed >= 3){
                    qDebug() << "_________ audio enter  pressed:long" ;
                    timer->stop();
                    t_pressed = 0;
                    models->resetFactory();
                }
            });
        }
        if(value == 1){
            //pressed
            timer->start(1000);
        }else if(value == 0){
            //released
            if(timer->isActive()){
                qDebug() << "_________ audio enter  pressed:short" ;
                profile->write(profile);
                timer->stop();
                t_pressed = 0;
            }
        }
    }else if(key == KEY_LED_MEMU){
        settings->listKey()[key]->doWork(value);
        if(!QFile::exists(UPDATE_MESSAGE_FILE) && !messageDialogControl->messageDialogVisible()){
            messageDialogControl->dialogShow("",{},MessageDialogControl::MESSAGE_UPDATE);
            return ;
        }
        static QTimer *timer = nullptr;
        static int t_pressed = 0;
        if(timer == nullptr){
            timer = new QTimer;
            connect(timer,&QTimer::timeout,this,[=](){
                t_pressed++;
                if(t_pressed >= 2){
                    qDebug() << "_________ audio enter  pressed:long" ;
                    timer->stop();
                    t_pressed = 0;
                    if(messageDialogControl->messageDialogVisible() && messageDialogControl->type() == MessageDialogControl::MESSAGE_UPDATE){
                        messageDialogControl->cancel();
                        system("mount -o remount /dev/mmcblk0p7 /oem/");
                        settings->setIniValue(UPDATE_MESSAGE_FILE,"update",1);
                        system("mount -ro remount /dev/mmcblk0p7 /oem/");
                    }
                }
            });
        }
        if(value == 1){
            //pressed
            timer->start(1000);
        }else if(value == 0){
            //released
            if(timer->isActive()){
                qDebug() << "_________ audio enter  pressed:short" ;
                timer->stop();
                t_pressed = 0;
            }
        }
    }
}

void Control::slotKeyStatusChanged(const int key, const int status)
{
    //key keyOnAir dsk dskOnAir bkgd prev等键转交灯状态信号处理
    if(key == KEY_LED_TRANS_PREVIEW
        || key == KEY_LED_KEY_ON_AIR
        || key == KEY_LED_DSK_ON_AIR
        || key == KEY_LED_KEY
        || key == KEY_LED_DSK
        || key == KEY_LED_BKGD){
        //
        settings->listKey()[key]->doWork(1);
    }else if(key >= KEY_LED_PGM_1 && key <= KEY_LED_PGM_STLL2 && status == LED_STATUS_RED){
        settings->setpgmCurrentIndex(key - KEY_LED_PGM_1);
    }

    //set play when pgm is aux and cut when the video is over
    if(key == KEY_LED_PGM_AUX  && status == LED_STATUS_RED){
        // pgm is aux
        models->setPlay(0);
    }
}
