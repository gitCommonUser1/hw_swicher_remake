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
#include "playbackgroupmanager.h"
#include "macrorunner.h"
#include "macrorecorder.h"
#include "macro.h"
#include <QVariant>

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
extern PlaybackGroupManager* playbackGroupManager;
extern MacroRunner *macroRunner;
extern MacroRecorder *macroRecorder;


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
            models->recordStop();
            return ;
        }
#define RECORD_MAX_SECOND 3600 * 5
        if(settings->recordSecond() >= RECORD_MAX_SECOND)
        {
            models->recordStop();
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
            recordTimer->stop();
            settings->setRecordSecond(0);
            if(playbackGroupManager->list().size() == 1 && settings->playLedStatus() == E_STATUS_MP4_CLOSE){
                    settings->setPlayingIndex(0);
                    models->playPause(1);
                    models->playStart();
            }
        }
    });

    connect(settings,&Settings::liveLedStatusChanged,this,[=](int status){
//        qDebug() << "liveLedStatusChanged:" << status;

        bool enable1,enable2,enable3;
        enable1 = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_OUTPUT]->current.toInt();// == OUTPUT_ENABLE;
        enable2 = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM2]->third[MENU_THIRD_STREAM_OUTPUT]->current.toInt();// == OUTPUT_ENABLE;
        enable3 = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM3]->third[MENU_THIRD_STREAM_OUTPUT]->current.toInt();// == OUTPUT_ENABLE;

        bool liveStatus = profile->streams()->enable();
        if(liveStatus)
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

    connect(settings,&Settings::playLedStatusChanged,this,[=](int status){
        if(status == E_STATUS_MP4_CLOSE){
            if(SWITCHER_LED_R == QSwitcher::get_led(KEY_LED_TRANS_AUTO)){
                models->setCutTransition();
            }
            else if(SWITCHER_LED_R == QSwitcher::get_led(KEY_LED_PGM_AUX)){
                models->setCutTransition();
            }
            if(profile->playback()->playbackList() == Playback::ALL_GROUP){
                if(settings->playingIndex() < playbackGroupManager->list().size() - 1 && playbackGroupManager->list().size() != 0){
                    settings->setPlayingIndex(settings->playingIndex() + 1);
                    models->playStart();
                }else{
                    models->playPause(1);
                    models->playStart();
                }
            }else if(profile->playback()->playbackList() == Playback::SIGNAL_GROUP){
                if(!playbackGroupManager->isGroupLast(playbackGroupManager->list()[settings->playingIndex()])){
                    settings->setPlayingIndex(settings->playingIndex() + 1);
                    models->playStart();
                }else{
                    models->playPause(1);
                    models->playStart();
                }
            }
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

            if(profile->setting()->srcSelections()->aux()->selection() == SrcSelections::SD_CARD){
                settings->setPlayListDialogVisible(1);
            }else if(profile->setting()->srcSelections()->aux()->selection() == SrcSelections::NDI){
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
                    bool dhcp = profile->setting()->network()->protocol();
                    if(dhcp)
                    {
                        settings->setMenuDHCPNetwork("","","","","");
                    }
                }
            }
    });
    connect(&qthread_route,&QThreadRoute::emit_netaddr_change,this,[=](bool is_new, QString netif, QString ipaddr, int prefixlen){
        if (netif == ETHERNET_NETIF)
        {
            bool dhcp = profile->setting()->network()->protocol();
            if(dhcp)
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
            bool dhcp = profile->setting()->network()->protocol();
            if(dhcp)
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
            bool dhcp = profile->setting()->network()->protocol();
            if(dhcp)
            {
                char c_dns1[16], c_dns2[16];
                get_dns(netif.toStdString().data(), c_dns1, 16, c_dns2, 16);
                settings->setMenuDHCPNetworkPrimaryDNS(c_dns1);
                settings->setMenuDHCPNetworksecondayDNS(c_dns2);
            }
        }
    });

    //key board input ok
    connect(settings,&Settings::keyboardOk,[=](int first,int second,int third,QString value){
        if(first == -1)
            return ;
        if(second == -1)
            return ;
        if(third == -1)
            return ;

        settings->listFirst()[first]->second[second]->third[third]->doWork(value);
    });


    //macro灯光熄灭
    for(int i = KEY_LED_MEM1;i <= KEY_LED_MEM8;++i)
    {
        QSwitcher::set_led(i,SWITCHER_LED_OFF);
    }

    qRegisterMetaType<Macro*>("Macro*");
    // macro manager
    connect(macroRunner,&MacroRunner::workStatusChanged,this,[=](int index,bool isWorking){
        index = KEY_LED_MEM1 + index - 1;
        if(index < KEY_LED_MEM1 || index > KEY_LED_MEM8)
            return ;
        if(isWorking)
            QSwitcher::set_led(index,SWITCHER_LED_ON,LEDS_BLINK_2Hz);
        else
            QSwitcher::set_led(index,SWITCHER_LED_ON);
    });
    connect(macroRecorder,&MacroRecorder::workStatusChanged,this,[=](int index,bool isWorking){
        index = KEY_LED_MEM1 + index - 1;
        if(index < KEY_LED_MEM1 || index > KEY_LED_MEM8)
            return ;
        if(isWorking){
            QSwitcher::set_led(index,SWITCHER_LED_ON,LEDS_BLINK_0_5Hz);
        }
        else{
            QSwitcher::set_led(index,SWITCHER_LED_OFF);
            profile->macroPool()->append(macroRecorder->data());
        }
    });
    connect(macroRunner,&MacroRunner::runMethod,this,[=](QVariantMap map){
        auto modelsSignals = models->getSignals();
        QString methodName = map["id"].toString();
        if(modelsSignals.find(methodName) == modelsSignals.end())
            return ;
        QList<QGenericArgument>args;
        QMetaMethod method = modelsSignals[methodName];
        for(int i = 0;i < method.parameterCount();++i){
            if(map.contains(method.parameterNames()[i])){
                QGenericArgument arg;
                int type = map[method.parameterNames()[i]].type();
                switch (type) {
                case QVariant::String:
                    args.push_back(QGenericArgument("QString",map[method.parameterNames()[i]].data()));
                    break;
                case QVariant::Int:
                    args.push_back(QGenericArgument("int",map[method.parameterNames()[i]].data()));
                    break;
                case QVariant::Bool:
                    args.push_back(QGenericArgument("bool",map[method.parameterNames()[i]].data()));
                    break;
                case QVariant::Double:
                    args.push_back(QGenericArgument("double",map[method.parameterNames()[i]].data()));
                    break;
                }
            }
        }
        switch (args.size()) {
        case 0:
            models->macroInvoke(method);
            break;
        case 1:
            models->macroInvoke(method,args[0]);
            break;
        case 2:
            models->macroInvoke(method,args[0],args[1]);
            break;
        case 3:
            models->macroInvoke(method,args[0],args[1],args[2]);
            break;
        case 4:
            models->macroInvoke(method,args[0],args[1],args[2],args[3]);
            break;
        }
    });
}

void Control::connect_profile()
{
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
//        models->macroInvoke(&Models::pgmIndex,MixEffectBlock::inputIndexToString(input));
        if(input == MixEffectBlock::AUX){
            models->playPause(0);
        }
    });
    //pvw
    connect(profile->mixEffectBlocks()->mixEffectBlock()->preview(),&Preview::inputChanged,this,[=](int input){
//        models->macroInvoke(&Models::pvwIndex,MixEffectBlock::inputIndexToString(input));

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
        models->macroInvoke(&Models::transitionRate,TransitionStyle::styleIndexToString(TransitionStyle::MIX),rate);
        settings->setMenuValue(MENU_FIRST_TRANSITION,TRANSITION_MIX,TRANSITION_MIX_RATE,rate);
    });
    //DipParameters
    connect(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->dipParameters(),&DipParameters::rateChanged,this,[=](double rate){
        models->macroInvoke(&Models::transitionRate,TransitionStyle::styleIndexToString(TransitionStyle::DIP),rate);
        settings->setMenuValue(MENU_FIRST_TRANSITION,TRANSITION_DIP,TRANSITION_DIP_RATE,rate);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->dipParameters(),&DipParameters::inputChanged,this,[=](int input){
        models->macroInvoke(&Models::transitionDipSource,input);
        settings->setMenuValue(MENU_FIRST_TRANSITION,TRANSITION_DIP,TRANSITION_DIP_SOURCE,input);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->dipParameters(),&DipParameters::stingerChanged,this,[=](bool stinger){
        models->macroInvoke(&Models::transitionDipStinger,stinger);
        settings->setMenuValue(MENU_FIRST_TRANSITION,TRANSITION_DIP,TRANSITION_DIP_STINGER,stinger);
        if(stinger)
        {
            profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->dipParameters()->setInput(TransitionStyle::AUX);
        }
    });
    //WipeParameters
    connect(profile->mixEffectBlocks()->mixEffectBlock()->transitionStyle()->wipeParameters(),&WipeParameters::rateChanged,this,[=](double rate){
        models->macroInvoke(&Models::transitionRate,TransitionStyle::styleIndexToString(TransitionStyle::WIPE),rate);
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
    //key
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->key(),&Key::typeChanged,this,[=](QString type){
        models->macroInvoke(&Models::keyType,type);
        settings->setMenuValue(MENU_FIRST_KEY_TYPE,KEY_TYPE_TYPE,TYPE_TYPE,Key::typeStringToIndex(type));
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->key(),&Key::onAirChanged,this,[=](bool onAir){
        models->macroInvoke(&Models::keyOnAir,onAir);
        //key on air时，判断key source是否是aux，如果是aux，自动开始播放
        if(onAir == false)
            return ;
        int keyType = Keys::keyStringToIndex(profile->mixEffectBlocks()->mixEffectBlock()->keys()->key()->type());
        switch (keyType) {
        case Keys::LUMA:
            if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->keySource() == Keys::AUX
            || profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters()->fillSource() == Keys::AUX){
                models->playPause(0);
            }
            break;
        case Keys::CHROMA:
            if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->fillSource() == Keys::AUX){
                models->playPause(0);
            }
            break;
        case Keys::PATTERN:
            if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters()->fillSource() == Keys::AUX){
                models->playPause(0);
            }
            break;
        case Keys::PIP:
            if(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters()->fillSource() == Keys::AUX){
                models->playPause(0);
            }
            break;
        }
    });
    //LumaParameters
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters(),&LumaParameters::fillSourceChanged,this,[=](int fill){
        models->macroInvoke(&Models::keySourceFill,Keys::keyIndexToString(Keys::LUMA),fill);
        settings->setMenuValue(MENU_FIRST_LUMA_KEY,LUMA_KEY_SOURCE,LUMA_KEY_SOURCE_FILL,fill);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters(),&LumaParameters::keySourceChanged,this,[=](int key){
        models->macroInvoke(&Models::keySourceKey,Keys::keyIndexToString(Keys::LUMA),key);
        settings->setMenuValue(MENU_FIRST_LUMA_KEY,LUMA_KEY_SOURCE,LUMA_KEY_SOURCE_KEY,key);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters(),&LumaParameters::maskEnableChanged,this,[=](bool maskEnable){
        models->macroInvoke(&Models::keyMaskEnable,Keys::keyIndexToString(Keys::LUMA),maskEnable);
        settings->setMenuValue(MENU_FIRST_LUMA_KEY,LUMA_KEY_MASK,LUMA_KEY_MASK_ENABLE,maskEnable?1:0);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters(),&LumaParameters::maskHStartChanged,this,[=](int maskHStart){
        models->macroInvoke(&Models::keyMaskHStart,Keys::keyIndexToString(Keys::LUMA),maskHStart);
        settings->setMenuValue(MENU_FIRST_LUMA_KEY,LUMA_KEY_MASK,LUMA_KEY_MASK_H_START,maskHStart);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters(),&LumaParameters::maskVStartChanged,this,[=](int maskVStart){
        models->macroInvoke(&Models::keyMaskVStart,Keys::keyIndexToString(Keys::LUMA),maskVStart);
        settings->setMenuValue(MENU_FIRST_LUMA_KEY,LUMA_KEY_MASK,LUMA_KEY_MASK_V_START,maskVStart);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters(),&LumaParameters::maskHEndChanged,this,[=](int maskHEnd){
        models->macroInvoke(&Models::keyMaskHEnd,Keys::keyIndexToString(Keys::LUMA),maskHEnd);
        settings->setMenuValue(MENU_FIRST_LUMA_KEY,LUMA_KEY_MASK,LUMA_KEY_MASK_H_END,maskHEnd);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters(),&LumaParameters::maskVEndChanged,this,[=](int maskVEnd){
        models->macroInvoke(&Models::keyMaskVEnd,Keys::keyIndexToString(Keys::LUMA),maskVEnd);
        settings->setMenuValue(MENU_FIRST_LUMA_KEY,LUMA_KEY_MASK,LUMA_KEY_MASK_V_END,maskVEnd);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters(),&LumaParameters::shapedKeyChanged,this,[=](bool shapedKey){
        models->macroInvoke(&Models::keyShapedKey,Keys::keyIndexToString(Keys::LUMA),shapedKey);
        settings->setMenuValue(MENU_FIRST_LUMA_KEY,LUMA_KEY_CONTROL,LUMA_KEY_CONTROL_SHAPED_KEY,shapedKey);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters(),&LumaParameters::clipChanged,this,[=](int clip){
        models->macroInvoke(&Models::keyClip,Keys::keyIndexToString(Keys::LUMA),clip);
        settings->setMenuValue(MENU_FIRST_LUMA_KEY,LUMA_KEY_CONTROL,LUMA_KEY_CONTROL_CLIP,clip);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters(),&LumaParameters::gainChanged,this,[=](int gain){
        models->macroInvoke(&Models::keyGain,Keys::keyIndexToString(Keys::LUMA),gain);
        settings->setMenuValue(MENU_FIRST_LUMA_KEY,LUMA_KEY_CONTROL,LUMA_KEY_CONTROL_GAIN,gain);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters(),&LumaParameters::invertChanged,this,[=](bool invert){
        models->macroInvoke(&Models::keyInvert,Keys::keyIndexToString(Keys::LUMA),invert);
        settings->setMenuValue(MENU_FIRST_LUMA_KEY,LUMA_KEY_CONTROL,LUMA_KEY_CONTROL_INVERT,invert);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters(),&LumaParameters::resizeChanged,this,[=](bool resize){
        models->macroInvoke(&Models::keyResize,Keys::keyIndexToString(Keys::LUMA),resize);
        settings->setMenuValue(MENU_FIRST_LUMA_KEY,LUMA_KEY_RESIZE,LUMA_KEY_RESIZE_RESIZE,resize);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters(),&LumaParameters::sizeChanged,this,[=](QString size){
        models->macroInvoke(&Models::keySize,Keys::keyIndexToString(Keys::LUMA),size);
        settings->setMenuValue(MENU_FIRST_LUMA_KEY,LUMA_KEY_RESIZE,LUMA_KEY_RESIZE_SIZE,size);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters(),&LumaParameters::xPositionChanged,this,[=](double xPosition){
        models->macroInvoke(&Models::keyXPosition,Keys::keyIndexToString(Keys::LUMA),xPosition);
        settings->setMenuValue(MENU_FIRST_LUMA_KEY,LUMA_KEY_RESIZE,LUMA_KEY_RESIZE_X_POSITION,xPosition);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->lumaParameters(),&LumaParameters::yPositionChanged,this,[=](double yPosition){
        models->macroInvoke(&Models::keyYPosition,Keys::keyIndexToString(Keys::LUMA),yPosition);
        settings->setMenuValue(MENU_FIRST_LUMA_KEY,LUMA_KEY_RESIZE,LUMA_KEY_RESIZE_Y_POSITION,yPosition);
    });
    //chromaParameters
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters(),&ChromaParameters::fillSourceChanged,this,[=](int fillSource){
        models->macroInvoke(&Models::keySourceFill,Keys::keyIndexToString(Keys::CHROMA),fillSource);
        settings->setMenuValue(MENU_FIRST_CHROMA_KEY,CHROMA_KEY_SOURCE,CHROMA_KEY_SOURCE_FILL,fillSource);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters(),&ChromaParameters::maskEnableChanged,this,[=](bool maskEnable){
        models->macroInvoke(&Models::keyMaskEnable,Keys::keyIndexToString(Keys::CHROMA),maskEnable);
        settings->setMenuValue(MENU_FIRST_CHROMA_KEY,CHROMA_KEY_MASK,CHROMA_KEY_MASK_ENABLE,maskEnable?1:0);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters(),&ChromaParameters::maskHStartChanged,this,[=](int maskHStart){
        models->macroInvoke(&Models::keyMaskHStart,Keys::keyIndexToString(Keys::CHROMA),maskHStart);
        settings->setMenuValue(MENU_FIRST_CHROMA_KEY,CHROMA_KEY_MASK,CHROMA_KEY_MASK_H_START,maskHStart);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters(),&ChromaParameters::maskVStartChanged,this,[=](int maskVStart){
        models->macroInvoke(&Models::keyMaskVStart,Keys::keyIndexToString(Keys::CHROMA),maskVStart);
        settings->setMenuValue(MENU_FIRST_CHROMA_KEY,CHROMA_KEY_MASK,CHROMA_KEY_MASK_V_START,maskVStart);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters(),&ChromaParameters::maskHEndChanged,this,[=](int maskHEnd){
        models->macroInvoke(&Models::keyMaskHEnd,Keys::keyIndexToString(Keys::CHROMA),maskHEnd);
        settings->setMenuValue(MENU_FIRST_CHROMA_KEY,CHROMA_KEY_MASK,CHROMA_KEY_MASK_H_END,maskHEnd);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters(),&ChromaParameters::maskVEndChanged,this,[=](int maskVEnd){
        models->macroInvoke(&Models::keyMaskVEnd,Keys::keyIndexToString(Keys::CHROMA),maskVEnd);
        settings->setMenuValue(MENU_FIRST_CHROMA_KEY,CHROMA_KEY_MASK,CHROMA_KEY_MASK_V_END,maskVEnd);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters(),&ChromaParameters::resizeChanged,this,[=](bool resize){
        models->macroInvoke(&Models::keyResize,Keys::keyIndexToString(Keys::CHROMA),resize);
        settings->setMenuValue(MENU_FIRST_CHROMA_KEY,CHROMA_KEY_RESIZE,CHROMA_KEY_RESIZE_RESIZE,resize);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters(),&ChromaParameters::sizeChanged,this,[=](QString size){
        models->macroInvoke(&Models::keySize,Keys::keyIndexToString(Keys::CHROMA),size);
        settings->setMenuValue(MENU_FIRST_CHROMA_KEY,CHROMA_KEY_RESIZE,CHROMA_KEY_RESIZE_SIZE,size);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters(),&ChromaParameters::xPositionChanged,this,[=](double xPosition){
        models->macroInvoke(&Models::keyXPosition,Keys::keyIndexToString(Keys::CHROMA),xPosition);
        settings->setMenuValue(MENU_FIRST_CHROMA_KEY,CHROMA_KEY_RESIZE,CHROMA_KEY_RESIZE_X_POSITION,xPosition);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters(),&ChromaParameters::yPositionChanged,this,[=](double yPosition){
        models->macroInvoke(&Models::keyYPosition,Keys::keyIndexToString(Keys::CHROMA),yPosition);
        settings->setMenuValue(MENU_FIRST_CHROMA_KEY,CHROMA_KEY_RESIZE,CHROMA_KEY_RESIZE_Y_POSITION,yPosition);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters(),&ChromaParameters::smpXPositionChanged,this,[=](double smpXPosition){
        models->macroInvoke(&Models::chromaKeySampleXPosition,smpXPosition);
        settings->setMenuValue(MENU_FIRST_CHROMA_KEY,CHROMA_KEY_CONTROL,CHROMA_KEY_CONTROL_SMP_X_POSITION,smpXPosition);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters(),&ChromaParameters::smpYPositionChanged,this,[=](double smpYPosition){
        models->macroInvoke(&Models::chromaKeySampleYPosition,smpYPosition);
        settings->setMenuValue(MENU_FIRST_CHROMA_KEY,CHROMA_KEY_CONTROL,CHROMA_KEY_CONTROL_SMP_Y_POSITION,smpYPosition);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters(),&ChromaParameters::sampleChanged,this,[=](bool sample){
        models->macroInvoke(&Models::chromaKeySample,sample);
        settings->setMenuValue(MENU_FIRST_CHROMA_KEY,CHROMA_KEY_CONTROL,CHROMA_KEY_CONTROL_SMP_ENABLE,sample);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters(),&ChromaParameters::foregroundChanged,this,[=](int foreground){
        models->macroInvoke(&Models::chromaKeyForeground,foreground);
        settings->setMenuValue(MENU_FIRST_CHROMA_KEY,CHROMA_KEY_CONTROL,CHROMA_KEY_CONTROL_Foreground,foreground);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters(),&ChromaParameters::backgroundChanged,this,[=](int background){
        models->macroInvoke(&Models::chromaKeyBackground,background);
        settings->setMenuValue(MENU_FIRST_CHROMA_KEY,CHROMA_KEY_CONTROL,CHROMA_KEY_CONTROL_Background,background);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters(),&ChromaParameters::keyEdgeChanged,this,[=](int keyEdge){
        models->macroInvoke(&Models::chromaKeyKeyEdge,keyEdge);
        settings->setMenuValue(MENU_FIRST_CHROMA_KEY,CHROMA_KEY_CONTROL,CHROMA_KEY_CONTROL_KeyEdge,keyEdge);
    });
    //patternParameters
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters(),&PatternParameters::fillSourceChanged,this,[=](int fillSource){
        models->macroInvoke(&Models::keySourceFill,Keys::keyIndexToString(Keys::PATTERN),fillSource);
        settings->setMenuValue(MENU_FIRST_KEY_PATTERN,PATTERN_SOURCE,KEY_PATTERN_SOURCE_FILL,fillSource);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters(),&PatternParameters::patternChanged,this,[=](int pattern){
        models->macroInvoke(&Models::keyPatternWipePattern,pattern);
        settings->setMenuValue(MENU_FIRST_KEY_PATTERN,PATTERN_WIPE,KEY_PATTERN_WIPE_PATTERN,pattern);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters(),&PatternParameters::wipeSizeChanged,this,[=](int wipeSize){
        models->macroInvoke(&Models::keyPatternWipeSize,wipeSize);
        settings->setMenuValue(MENU_FIRST_KEY_PATTERN,PATTERN_WIPE,KEY_PATTERN_WIPE_SIZE,wipeSize);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters(),&PatternParameters::wipeXPositionChanged,this,[=](double wipeXPosition){
        models->macroInvoke(&Models::keyPatternWipeXPosition,wipeXPosition);
        settings->setMenuValue(MENU_FIRST_KEY_PATTERN,PATTERN_WIPE,KEY_PATTERN_WIPE_POS_X,wipeXPosition);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters(),&PatternParameters::wipeYPositionChanged,this,[=](double wipeYPosition){
        models->macroInvoke(&Models::keyPatternWipeYPosition,wipeYPosition);
        settings->setMenuValue(MENU_FIRST_KEY_PATTERN,PATTERN_WIPE,KEY_PATTERN_WIPE_POS_Y,wipeYPosition);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters(),&PatternParameters::wipeSymmetryChanged,this,[=](int wipeSymmetry){
        models->macroInvoke(&Models::keyPatternWipeSymmetry,wipeSymmetry);
        settings->setMenuValue(MENU_FIRST_KEY_PATTERN,PATTERN_WIPE,KEY_PATTERN_WIPE_SYMMERTRY,wipeSymmetry);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters(),&PatternParameters::wipeSoftnessChanged,this,[=](int wipeSoftness){
        models->macroInvoke(&Models::keyPatternWipeSoftness,wipeSoftness);
        settings->setMenuValue(MENU_FIRST_KEY_PATTERN,PATTERN_WIPE,KEY_PATTERN_WIPE_SOFTNESS,wipeSoftness);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters(),&PatternParameters::maskEnableChanged,this,[=](bool maskEnable){
        models->macroInvoke(&Models::keyMaskEnable,Keys::keyIndexToString(Keys::PATTERN),maskEnable);
        settings->setMenuValue(MENU_FIRST_KEY_PATTERN,PATTERN_MASK,KEY_PATTERN_MASK_ENABLE,maskEnable?1:0);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters(),&PatternParameters::maskHStartChanged,this,[=](int maskHStart){
        models->macroInvoke(&Models::keyMaskHStart,Keys::keyIndexToString(Keys::PATTERN),maskHStart);
        settings->setMenuValue(MENU_FIRST_KEY_PATTERN,PATTERN_MASK,KEY_PATTERN_MASK_H_START,maskHStart);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters(),&PatternParameters::maskVStartChanged,this,[=](int maskVStart){
        models->macroInvoke(&Models::keyMaskVStart,Keys::keyIndexToString(Keys::PATTERN),maskVStart);
        settings->setMenuValue(MENU_FIRST_KEY_PATTERN,PATTERN_MASK,KEY_PATTERN_MASK_V_START,maskVStart);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters(),&PatternParameters::maskHEndChanged,this,[=](int maskHEnd){
        models->macroInvoke(&Models::keyMaskHEnd,Keys::keyIndexToString(Keys::PATTERN),maskHEnd);
        settings->setMenuValue(MENU_FIRST_KEY_PATTERN,PATTERN_MASK,KEY_PATTERN_MASK_H_END,maskHEnd);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters(),&PatternParameters::maskVEndChanged,this,[=](int maskVEnd){
        models->macroInvoke(&Models::keyMaskVEnd,Keys::keyIndexToString(Keys::PATTERN),maskVEnd);
        settings->setMenuValue(MENU_FIRST_KEY_PATTERN,PATTERN_MASK,KEY_PATTERN_MASK_V_END,maskVEnd);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters(),&PatternParameters::resizeChanged,this,[=](bool resize){
        models->macroInvoke(&Models::keyResize,Keys::keyIndexToString(Keys::PATTERN),resize);
        settings->setMenuValue(MENU_FIRST_KEY_PATTERN,PATTERN_RESIZE,KEY_PATTERN_RESIZE_RESIZE,resize);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters(),&PatternParameters::sizeChanged,this,[=](QString size){
        models->macroInvoke(&Models::keySize,Keys::keyIndexToString(Keys::PATTERN),size);
        settings->setMenuValue(MENU_FIRST_KEY_PATTERN,PATTERN_RESIZE,KEY_PATTERN_RESIZE_SIZE,size);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters(),&PatternParameters::xPositionChanged,this,[=](double xPosition){
        models->macroInvoke(&Models::keyXPosition,Keys::keyIndexToString(Keys::PATTERN),xPosition);
        settings->setMenuValue(MENU_FIRST_KEY_PATTERN,PATTERN_RESIZE,KEY_PATTERN_RESIZE_X_POSITION,xPosition);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->patternParameters(),&PatternParameters::yPositionChanged,this,[=](double yPosition){
        models->macroInvoke(&Models::keyYPosition,Keys::keyIndexToString(Keys::PATTERN),yPosition);
        settings->setMenuValue(MENU_FIRST_KEY_PATTERN,PATTERN_RESIZE,KEY_PATTERN_RESIZE_Y_POSITION,yPosition);
    });
    //pIPParameters
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters(),&PIPParameters::fillSourceChanged,this,[=](int fillSource){
        models->macroInvoke(&Models::keySourceFill,Keys::keyIndexToString(Keys::PIP),fillSource);
        settings->setMenuValue(MENU_FIRST_PIP,PIP_SOURCE,PIP_SOURCE_FILL,fillSource);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters(),&PIPParameters::sizeChanged,this,[=](QString size){
        models->macroInvoke(&Models::keySize,Keys::keyIndexToString(Keys::PIP),size);
        settings->setMenuValue(MENU_FIRST_PIP,PIP_SIZE_POSITION,PIP_SIZE_SIZE,size);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters(),&PIPParameters::xPositionChanged,this,[=](double xPosition){
        models->macroInvoke(&Models::keyXPosition,Keys::keyIndexToString(Keys::PIP),xPosition);
        settings->setMenuValue(MENU_FIRST_PIP,PIP_SIZE_POSITION,PIP_SIZE_X_POSITION,xPosition);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters(),&PIPParameters::yPositionChanged,this,[=](double yPosition){
        models->macroInvoke(&Models::keyYPosition,Keys::keyIndexToString(Keys::PIP),yPosition);
        settings->setMenuValue(MENU_FIRST_PIP,PIP_SIZE_POSITION,PIP_SIZE_Y_POSITION,yPosition);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters(),&PIPParameters::maskEnableChanged,this,[=](bool maskEnable){
        models->macroInvoke(&Models::keyMaskEnable,Keys::keyIndexToString(Keys::PIP),maskEnable);
        settings->setMenuValue(MENU_FIRST_PIP,PIP_MASK,PIP_MASK_ENABLE,maskEnable?1:0);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters(),&PIPParameters::maskHStartChanged,this,[=](int maskHStart){
        models->macroInvoke(&Models::keyMaskHStart,Keys::keyIndexToString(Keys::PIP),maskHStart);
        settings->setMenuValue(MENU_FIRST_PIP,PIP_MASK,PIP_MASK_H_START,maskHStart);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters(),&PIPParameters::maskVStartChanged,this,[=](int maskVStart){
        models->macroInvoke(&Models::keyMaskVStart,Keys::keyIndexToString(Keys::PIP),maskVStart);
        settings->setMenuValue(MENU_FIRST_PIP,PIP_MASK,PIP_MASK_V_START,maskVStart);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters(),&PIPParameters::maskHEndChanged,this,[=](int maskHEnd){
        models->macroInvoke(&Models::keyMaskHEnd,Keys::keyIndexToString(Keys::PIP),maskHEnd);
        settings->setMenuValue(MENU_FIRST_PIP,PIP_MASK,PIP_MASK_H_END,maskHEnd);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters(),&PIPParameters::maskVEndChanged,this,[=](int maskVEnd){
        models->macroInvoke(&Models::keyMaskVEnd,Keys::keyIndexToString(Keys::PIP),maskVEnd);
        settings->setMenuValue(MENU_FIRST_PIP,PIP_MASK,PIP_MASK_V_END,maskVEnd);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters(),&PIPParameters::borderEnableChanged,this,[=](bool borderEnable){
        models->macroInvoke(&Models::pipBorderEnable,borderEnable);
        settings->setMenuValue(MENU_FIRST_PIP,PIP_BOARD,PIP_BORDER_ENABLE,borderEnable);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters(),&PIPParameters::borderWidthChanged,this,[=](int borderWidth){
        models->macroInvoke(&Models::pipBorderWidth,borderWidth);
        settings->setMenuValue(MENU_FIRST_PIP,PIP_BOARD,PIP_BORDER_WIDTH,borderWidth);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters(),&PIPParameters::borderColorHueChanged,this,[=](int borderColorHue){
        models->macroInvoke(&Models::pipBorderColorHue,borderColorHue);
        settings->setMenuValue(MENU_FIRST_PIP,PIP_COLOR,PIP_COLOR_HUE,borderColorHue);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters(),&PIPParameters::borderColorSaturationChanged,this,[=](int borderColorSaturation){
        models->macroInvoke(&Models::pipBorderColorSaturation,borderColorSaturation);
        settings->setMenuValue(MENU_FIRST_PIP,PIP_COLOR,PIP_COLOR_SATURATION,borderColorSaturation);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->keys()->pIPParameters(),&PIPParameters::borderColorBrightnessChanged,this,[=](int borderColorBrightness){
        models->macroInvoke(&Models::pipBorderColorBrightness,borderColorBrightness);
        settings->setMenuValue(MENU_FIRST_PIP,PIP_COLOR,PIP_COLOR_BRIGHTNESS,borderColorBrightness);
    });
    //FadeToBlack
    connect(profile->mixEffectBlocks()->mixEffectBlock()->ftb(),&FadeToBlack::enableChanged,this,[=](bool enable){
        models->macroInvoke(&Models::ftbEnable,enable);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->ftb(),&FadeToBlack::rateChanged,this,[=](double rate){
        models->macroInvoke(&Models::ftbRate,rate);
        settings->setMenuValue(MENU_FIRST_FTB,MENU_SECOND_FTB_RATE,FTB_RATE_RATE,rate);
    });
    connect(profile->mixEffectBlocks()->mixEffectBlock()->ftb(),&FadeToBlack::afvChanged,this,[=](bool afv){
        models->macroInvoke(&Models::ftbAfv,afv);
        settings->setMenuValue(MENU_FIRST_FTB,MENU_SECOND_FTB_AUDIO,FTB_AUDIO_AFV,afv);
    });
    //DownstreamKeys
    connect(profile->downstreamKeys()->downstreamKey(),&DownstreamKey::onAirChanged,this,[=](bool onAir){
        models->macroInvoke(&Models::dskOnAir,onAir);
        if(onAir == false)
            return ;
        if(profile->downstreamKeys()->downstreamKey()->keySource() == Keys::AUX ||
           profile->downstreamKeys()->downstreamKey()->fillSource() == Keys::AUX){
            models->playPause(0);
        }
    });
    connect(profile->downstreamKeys()->downstreamKey(),&DownstreamKey::fillSourceChanged,this,[=](int fillSource){
        models->macroInvoke(&Models::dskSourceFill,fillSource);
        settings->setMenuValue(MENU_FIRST_DSK,DSK_SOURCE,DSK_SOURCE_FILL,fillSource);
    });
    connect(profile->downstreamKeys()->downstreamKey(),&DownstreamKey::keySourceChanged,this,[=](int keySource){
        models->macroInvoke(&Models::dskSourceKey,keySource);
        settings->setMenuValue(MENU_FIRST_DSK,DSK_SOURCE,DSK_SOURCE_KEY,keySource);
    });
    connect(profile->downstreamKeys()->downstreamKey(),&DownstreamKey::maskEnableChanged,this,[=](bool maskEnable){
        models->macroInvoke(&Models::dskMaskEnable,maskEnable);
        settings->setMenuValue(MENU_FIRST_DSK,DSK_MASK,DSK_MASK_ENABLE,maskEnable);
    });
    connect(profile->downstreamKeys()->downstreamKey(),&DownstreamKey::maskHStartChanged,this,[=](int maskHStart){
        models->macroInvoke(&Models::dskMaskHStart,maskHStart);
        settings->setMenuValue(MENU_FIRST_DSK,DSK_MASK,DSK_MASK_H_START,maskHStart);
    });
    connect(profile->downstreamKeys()->downstreamKey(),&DownstreamKey::maskVStartChanged,this,[=](int maskVStart){
        models->macroInvoke(&Models::dskMaskVStart,maskVStart);
        settings->setMenuValue(MENU_FIRST_DSK,DSK_MASK,DSK_MASK_V_START,maskVStart);
    });
    connect(profile->downstreamKeys()->downstreamKey(),&DownstreamKey::maskHEndChanged,this,[=](int maskHEnd){
        models->macroInvoke(&Models::dskMaskHEnd,maskHEnd);
        settings->setMenuValue(MENU_FIRST_DSK,DSK_MASK,DSK_MASK_H_END,maskHEnd);
    });
    connect(profile->downstreamKeys()->downstreamKey(),&DownstreamKey::maskVEndChanged,this,[=](int maskVEnd){
        models->macroInvoke(&Models::dskMaskVEnd,maskVEnd);
        settings->setMenuValue(MENU_FIRST_DSK,DSK_MASK,DSK_MASK_V_END,maskVEnd);
    });
    connect(profile->downstreamKeys()->downstreamKey(),&DownstreamKey::clipChanged,this,[=](int clip){
        models->macroInvoke(&Models::dskClip,clip);
        settings->setMenuValue(MENU_FIRST_DSK,DSK_CONTROL,DSK_CONTROL_CLIP,clip);
    });
    connect(profile->downstreamKeys()->downstreamKey(),&DownstreamKey::gainChanged,this,[=](int gain){
        models->macroInvoke(&Models::dskGain,gain);
        settings->setMenuValue(MENU_FIRST_DSK,DSK_CONTROL,DSK_CONTROL_GAIN,gain);
    });
    connect(profile->downstreamKeys()->downstreamKey(),&DownstreamKey::invertChanged,this,[=](bool invert){
        models->macroInvoke(&Models::dskInvert,invert);
        settings->setMenuValue(MENU_FIRST_DSK,DSK_CONTROL,DSK_CONTROL_INVERT,invert);
    });
    connect(profile->downstreamKeys()->downstreamKey(),&DownstreamKey::shapedKeyChanged,this,[=](bool shapedKey){
        models->macroInvoke(&Models::dskShapedKey,shapedKey);
        settings->setMenuValue(MENU_FIRST_DSK,DSK_CONTROL,DSK_CONTROL_SHAPED_KEY,shapedKey);
    });
    connect(profile->downstreamKeys()->downstreamKey(),&DownstreamKey::rateChanged,this,[=](double rate){
        models->macroInvoke(&Models::dskRate,rate);
        settings->setMenuValue(MENU_FIRST_DSK,DSK_RATE,DSK_RATE_RATE,rate);
    });
    //superSource
    connect(profile->superSources()->superSource(),&SuperSource::enableChanged,this,[=](bool enable){
        models->macroInvoke(&Models::superSourceEnable,enable);
        settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_ENABLE,SUPER_ENABLE,enable);
    });
    connect(profile->superSources()->superSource(),&SuperSource::source1Changed,this,[=](int source1){
        models->macroInvoke(&Models::superSourceSource1,source1);
        settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_SOURCE,SUPER_SOURCE_SOURCE_SOURCE1,source1);
    });
    connect(profile->superSources()->superSource(),&SuperSource::source2Changed,this,[=](int source2){
        models->macroInvoke(&Models::superSourceSource2,source2);
        settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_SOURCE,SUPER_SOURCE_SOURCE_SOURCE2,source2);
    });
    connect(profile->superSources()->superSource(),&SuperSource::backgroundChanged,this,[=](int background){
        models->macroInvoke(&Models::superSourceBackground,background);
        settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_SOURCE,SUPER_SOURCE_SOURCE_BACKGROUND,background);
    });
    connect(profile->superSources()->superSource(),&SuperSource::styleChanged,this,[=](QString style){
        models->macroInvoke(&Models::superSourceControlStyle,style);
        settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_CONTROL,SUPER_CONTROL_STYLE,style);
    });
    connect(profile->superSources()->superSource(),&SuperSource::yPositionChanged,this,[=](int yPosition){
        models->macroInvoke(&Models::superSourceControlYPosition,yPosition);
        settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_CONTROL,SUPER_CONTROL_Y_POSITION,yPosition);
    });
    connect(profile->superSources()->superSource()->mask1(),&SuperSourceMask::enableChanged,this,[=](bool enable){
        models->macroInvoke(&Models::superSourceMaskEnable,SuperSource::MASK1,enable);
        settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_MASK1,SUPER_MASK1_ENABLE,enable);
    });
    connect(profile->superSources()->superSource()->mask1(),&SuperSourceMask::hStartChanged,this,[=](int hStart){
        models->macroInvoke(&Models::superSourceMaskHStart,SuperSource::MASK1,hStart);
        settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_MASK1,SUPER_MASK1_H_START,hStart);
    });
    connect(profile->superSources()->superSource()->mask1(),&SuperSourceMask::vStartChanged,this,[=](int vStart){
        models->macroInvoke(&Models::superSourceMaskVStart,SuperSource::MASK1,vStart);
        settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_MASK1,SUPER_MASK1_V_START,vStart);
    });
    connect(profile->superSources()->superSource()->mask1(),&SuperSourceMask::hEndChanged,this,[=](int hEnd){
        models->macroInvoke(&Models::superSourceMaskHEnd,SuperSource::MASK1,hEnd);
        settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_MASK1,SUPER_MASK1_H_END,hEnd);
    });
    connect(profile->superSources()->superSource()->mask1(),&SuperSourceMask::vEndChanged,this,[=](int vEnd){
        models->macroInvoke(&Models::superSourceMaskVEnd,SuperSource::MASK1,vEnd);
        settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_MASK1,SUPER_MASK1_V_END,vEnd);
    });
    connect(profile->superSources()->superSource()->mask2(),&SuperSourceMask::enableChanged,this,[=](bool enable){
        models->macroInvoke(&Models::superSourceMaskEnable,SuperSource::MASK2,enable);
        settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_MASK2,SUPER_MASK2_ENABLE,enable);
    });
    connect(profile->superSources()->superSource()->mask2(),&SuperSourceMask::hStartChanged,this,[=](int hStart){
        models->macroInvoke(&Models::superSourceMaskHStart,SuperSource::MASK2,hStart);
        settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_MASK2,SUPER_MASK2_H_START,hStart);
    });
    connect(profile->superSources()->superSource()->mask2(),&SuperSourceMask::vStartChanged,this,[=](int vStart){
        models->macroInvoke(&Models::superSourceMaskVStart,SuperSource::MASK2,vStart);
        settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_MASK2,SUPER_MASK2_V_START,vStart);
    });
    connect(profile->superSources()->superSource()->mask2(),&SuperSourceMask::hEndChanged,this,[=](int hEnd){
        models->macroInvoke(&Models::superSourceMaskHEnd,SuperSource::MASK2,hEnd);
        settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_MASK2,SUPER_MASK2_H_END,hEnd);
    });
    connect(profile->superSources()->superSource()->mask2(),&SuperSourceMask::vEndChanged,this,[=](int vEnd){
        models->macroInvoke(&Models::superSourceMaskVEnd,SuperSource::MASK2,vEnd);
        settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_MASK2,SUPER_MASK2_V_END,vEnd);
    });
    connect(profile->superSources()->superSource()->border1(),&SuperSourceBorder::widthChanged,this,[=](int width){
        models->macroInvoke(&Models::superSourceBorderWidth,SuperSource::BORDER1,width);
        settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_BORDER1,SUPER_BORDER1_WIDTH,width);
    });
    connect(profile->superSources()->superSource()->border1(),&SuperSourceBorder::colorHueChanged,this,[=](int colorHue){
        models->macroInvoke(&Models::superSourceBorderColorHue,SuperSource::BORDER1,colorHue);
        settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_BORDER1,SUPER_BORDER1_HUE,colorHue);
    });
    connect(profile->superSources()->superSource()->border1(),&SuperSourceBorder::colorSaturationChanged,this,[=](int colorSaturation){
        models->macroInvoke(&Models::superSourceBorderColorSaturation,SuperSource::BORDER1,colorSaturation);
        settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_BORDER1,SUPER_BORDER1_SATURATION,colorSaturation);
    });
    connect(profile->superSources()->superSource()->border1(),&SuperSourceBorder::colorBrightnessChanged,this,[=](int colorBrightness){
        models->macroInvoke(&Models::superSourceBorderColorBrightness,SuperSource::BORDER1,colorBrightness);
        settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_BORDER1,SUPER_BORDER1_BRIGHTNESS,colorBrightness);
    });
    connect(profile->superSources()->superSource()->border2(),&SuperSourceBorder::widthChanged,this,[=](int width){
        models->macroInvoke(&Models::superSourceBorderWidth,SuperSource::BORDER2,width);
        settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_BORDER2,SUPER_BORDER2_WIDTH,width);
    });
    connect(profile->superSources()->superSource()->border2(),&SuperSourceBorder::colorHueChanged,this,[=](int colorHue){
        models->macroInvoke(&Models::superSourceBorderColorHue,SuperSource::BORDER2,colorHue);
        settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_BORDER2,SUPER_BORDER2_HUE,colorHue);
    });
    connect(profile->superSources()->superSource()->border2(),&SuperSourceBorder::colorSaturationChanged,this,[=](int colorSaturation){
        models->macroInvoke(&Models::superSourceBorderColorSaturation,SuperSource::BORDER2,colorSaturation);
        settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_BORDER2,SUPER_BORDER2_SATURATION,colorSaturation);
    });
    connect(profile->superSources()->superSource()->border2(),&SuperSourceBorder::colorBrightnessChanged,this,[=](int colorBrightness){
        models->macroInvoke(&Models::superSourceBorderColorBrightness,SuperSource::BORDER2,colorBrightness);
        settings->setMenuValue(MENU_FIRST_SUPER_SOURCE,SUPER_SOURCE_BORDER2,SUPER_BORDER2_BRIGHTNESS,colorBrightness);
    });
    //audio mixer
    connect(profile->audioMixer()->audioInput()->mic1(),&AudioSource::faderChanged,this,[=](double fader){
        models->macroInvoke(&Models::audioFader,AudioSource::sourceNameIndexToString(AudioSource::MIC1),fader);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_MIC1,MIC1_FADER,fader);
    });
    connect(profile->audioMixer()->audioInput()->mic1(),&AudioSource::balanceChanged,this,[=](double balance){
        models->macroInvoke(&Models::audioBalance,AudioSource::sourceNameIndexToString(AudioSource::MIC1),balance);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_MIC1,MIC1_BALANCE,balance);
    });
    connect(profile->audioMixer()->audioInput()->mic1(),&AudioSource::inputChanged,this,[=](double input){
        models->macroInvoke(&Models::audioInput,AudioSource::sourceNameIndexToString(AudioSource::MIC1),input);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_MIC1,MIC1_INPUT,input);
    });
    connect(profile->audioMixer()->audioInput()->mic1(),&AudioSource::delayChanged,this,[=](int delay){
        models->macroInvoke(&Models::audioDelay,AudioSource::sourceNameIndexToString(AudioSource::MIC1),delay);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_MIC1,MIC1_DELAY,delay);
    });
    connect(profile->audioMixer()->audioInput()->mic1(),&AudioSource::enableChanged,this,[=](int enable){
        models->macroInvoke(&Models::audioEnable,AudioSource::sourceNameIndexToString(AudioSource::MIC1),enable);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_MIC1,MIC1_ENABLE,enable);
    });
    connect(profile->audioMixer()->audioInput()->mic2(),&AudioSource::faderChanged,this,[=](double fader){
        models->macroInvoke(&Models::audioFader,AudioSource::sourceNameIndexToString(AudioSource::MIC2),fader);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_MIC2,MIC2_FADER,fader);
    });
    connect(profile->audioMixer()->audioInput()->mic2(),&AudioSource::balanceChanged,this,[=](double balance){
        models->macroInvoke(&Models::audioBalance,AudioSource::sourceNameIndexToString(AudioSource::MIC2),balance);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_MIC2,MIC2_BALANCE,balance);
    });
    connect(profile->audioMixer()->audioInput()->mic2(),&AudioSource::inputChanged,this,[=](double input){
        models->macroInvoke(&Models::audioInput,AudioSource::sourceNameIndexToString(AudioSource::MIC2),input);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_MIC2,MIC2_INPUT,input);
    });
    connect(profile->audioMixer()->audioInput()->mic2(),&AudioSource::delayChanged,this,[=](int delay){
        models->macroInvoke(&Models::audioDelay,AudioSource::sourceNameIndexToString(AudioSource::MIC2),delay);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_MIC2,MIC2_DELAY,delay);
    });
    connect(profile->audioMixer()->audioInput()->mic2(),&AudioSource::enableChanged,this,[=](int enable){
        models->macroInvoke(&Models::audioEnable,AudioSource::sourceNameIndexToString(AudioSource::MIC2),enable);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_MIC2,MIC2_ENABLE,enable);
    });
    connect(profile->audioMixer()->audioInput()->in1(),&AudioSource::faderChanged,this,[=](double fader){
        models->macroInvoke(&Models::audioFader,AudioSource::sourceNameIndexToString(AudioSource::IN1),fader);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_IN1,IN1_FADER,fader);
    });
    connect(profile->audioMixer()->audioInput()->in1(),&AudioSource::balanceChanged,this,[=](double balance){
        models->macroInvoke(&Models::audioBalance,AudioSource::sourceNameIndexToString(AudioSource::IN1),balance);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_IN1,IN1_BALANCE,balance);
    });
    connect(profile->audioMixer()->audioInput()->in1(),&AudioSource::inputChanged,this,[=](double input){
        models->macroInvoke(&Models::audioInput,AudioSource::sourceNameIndexToString(AudioSource::IN1),input);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_IN1,IN1_INPUT,input);
    });
    connect(profile->audioMixer()->audioInput()->in1(),&AudioSource::enableChanged,this,[=](int enable){
        models->macroInvoke(&Models::audioEnable,AudioSource::sourceNameIndexToString(AudioSource::IN1),enable);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_IN1,IN1_ENABLE,enable);
    });
    connect(profile->audioMixer()->audioInput()->in2(),&AudioSource::faderChanged,this,[=](double fader){
        models->macroInvoke(&Models::audioFader,AudioSource::sourceNameIndexToString(AudioSource::IN2),fader);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_IN2,IN2_FADER,fader);
    });
    connect(profile->audioMixer()->audioInput()->in2(),&AudioSource::balanceChanged,this,[=](double balance){
        models->macroInvoke(&Models::audioBalance,AudioSource::sourceNameIndexToString(AudioSource::IN2),balance);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_IN2,IN2_BALANCE,balance);
    });
    connect(profile->audioMixer()->audioInput()->in2(),&AudioSource::inputChanged,this,[=](double input){
        models->macroInvoke(&Models::audioInput,AudioSource::sourceNameIndexToString(AudioSource::IN2),input);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_IN2,IN2_INPUT,input);
    });
    connect(profile->audioMixer()->audioInput()->in2(),&AudioSource::enableChanged,this,[=](int enable){
        models->macroInvoke(&Models::audioEnable,AudioSource::sourceNameIndexToString(AudioSource::IN2),enable);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_IN2,IN2_ENABLE,enable);
    });
    connect(profile->audioMixer()->audioInput()->in3(),&AudioSource::faderChanged,this,[=](double fader){
        models->macroInvoke(&Models::audioFader,AudioSource::sourceNameIndexToString(AudioSource::IN3),fader);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_IN3,IN3_FADER,fader);
    });
    connect(profile->audioMixer()->audioInput()->in3(),&AudioSource::balanceChanged,this,[=](double balance){
        models->macroInvoke(&Models::audioBalance,AudioSource::sourceNameIndexToString(AudioSource::IN3),balance);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_IN3,IN3_BALANCE,balance);
    });
    connect(profile->audioMixer()->audioInput()->in3(),&AudioSource::inputChanged,this,[=](double input){
        models->macroInvoke(&Models::audioInput,AudioSource::sourceNameIndexToString(AudioSource::IN3),input);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_IN3,IN3_INPUT,input);
    });
    connect(profile->audioMixer()->audioInput()->in3(),&AudioSource::enableChanged,this,[=](int enable){
        models->macroInvoke(&Models::audioEnable,AudioSource::sourceNameIndexToString(AudioSource::IN3),enable);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_IN3,IN3_ENABLE,enable);
    });
    connect(profile->audioMixer()->audioInput()->in4(),&AudioSource::faderChanged,this,[=](double fader){
        models->macroInvoke(&Models::audioFader,AudioSource::sourceNameIndexToString(AudioSource::IN4),fader);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_IN4,IN4_FADER,fader);
    });
    connect(profile->audioMixer()->audioInput()->in4(),&AudioSource::balanceChanged,this,[=](double balance){
        models->macroInvoke(&Models::audioBalance,AudioSource::sourceNameIndexToString(AudioSource::IN4),balance);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_IN4,IN4_BALANCE,balance);
    });
    connect(profile->audioMixer()->audioInput()->in4(),&AudioSource::inputChanged,this,[=](double input){
        models->macroInvoke(&Models::audioInput,AudioSource::sourceNameIndexToString(AudioSource::IN4),input);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_IN4,IN4_INPUT,input);
    });
    connect(profile->audioMixer()->audioInput()->in4(),&AudioSource::enableChanged,this,[=](int enable){
        models->macroInvoke(&Models::audioEnable,AudioSource::sourceNameIndexToString(AudioSource::IN4),enable);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_IN4,IN4_ENABLE,enable);
    });
    connect(profile->audioMixer()->audioInput()->aux(),&AudioSource::faderChanged,this,[=](double fader){
        models->macroInvoke(&Models::audioFader,AudioSource::sourceNameIndexToString(AudioSource::AUX),fader);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_AUX,AUX_FADER,fader);
    });
    connect(profile->audioMixer()->audioInput()->aux(),&AudioSource::balanceChanged,this,[=](double balance){
        models->macroInvoke(&Models::audioBalance,AudioSource::sourceNameIndexToString(AudioSource::AUX),balance);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_AUX,AUX_BALANCE,balance);
    });
    connect(profile->audioMixer()->audioInput()->aux(),&AudioSource::inputChanged,this,[=](double input){
        models->macroInvoke(&Models::audioInput,AudioSource::sourceNameIndexToString(AudioSource::AUX),input);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_AUX,AUX_INPUT,input);
    });
    connect(profile->audioMixer()->audioInput()->aux(),&AudioSource::enableChanged,this,[=](int enable){
        models->macroInvoke(&Models::audioEnable,AudioSource::sourceNameIndexToString(AudioSource::AUX),enable);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_AUX,AUX_ENABLE,enable);
    });
    connect(profile->audioMixer()->audioOutput()->pgm(),&AudioSource::faderChanged,this,[=](double fader){
        models->macroInvoke(&Models::audioFader,AudioSource::sourceNameIndexToString(AudioSource::PGM),fader);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_PGM,PGM_FADER,fader);
    });
    connect(profile->audioMixer()->audioOutput()->monitor(),&AudioSource::levelChanged,this,[=](int level){
        models->macroInvoke(&Models::monitorLevel,level);
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_MONITOR,MONITOR_LEVEL,level);
    });
    connect(profile->audioMixer()->audioOutput()->monitor(),&AudioSource::sourceChanged,this,[=](int source){
        models->macroInvoke(&Models::monitorSource,AudioSource::sourceNameIndexToString(source));
        settings->setMenuValue(MENU_FIRST_AUDIO_MIXER,AUDIO_MIXER_MONITOR,MONITOR_SOURCE,source);
    });
    //still selection
    connect(profile->stillGenerator()->stillSelection(),&StillSelection::still1Changed,this,[=](int still1){
        models->macroInvoke(&Models::stillSelection,StillGenerator::STILL1,still1);
        settings->setMenuValue(MENU_FIRST_STILL_GENERATOR,STILL_GENERATE_SELECTION,STILL_SELECTION_STILL1,still1);
    });
    connect(profile->stillGenerator()->stillSelection(),&StillSelection::still2Changed,this,[=](int still2){
        models->macroInvoke(&Models::stillSelection,StillGenerator::STILL2,still2);
        settings->setMenuValue(MENU_FIRST_STILL_GENERATOR,STILL_GENERATE_SELECTION,STILL_SELECTION_STILL2,still2);
    });
    connect(profile->stillGenerator()->stillSelection(),&StillSelection::locationChanged,this,[=](int location){
        models->macroInvoke(&Models::stillLocation,location);
        settings->setMenuValue(MENU_FIRST_STILL_GENERATOR,STILL_GENERATE_UPLOAD,STILL_UPLOAD_LOCATION,location);
    });
    //macro
    connect(profile->macroPool(),&MacroPool::newMacro,this,[=](int index){
        int key = KEY_LED_MEM1 + index - 1;
        if(key > KEY_LED_MEM8 || key < KEY_LED_MEM1)
            return ;
        QSwitcher::set_led(KEY_LED_MEM1 + index - 1,SWITCHER_LED_ON);
    });
    connect(profile->macroPool(),&MacroPool::removeMacro,this,[=](int index){
        int key = KEY_LED_MEM1 + index - 1;
        if(key > KEY_LED_MEM8 || key < KEY_LED_MEM1)
            return ;
        QSwitcher::set_led(KEY_LED_MEM1 + index - 1,SWITCHER_LED_OFF);
    });
    connect(profile->macroPool(),&MacroPool::moveMacro,this,[=](int src,int dst){
        int key_dst = KEY_LED_MEM1 + dst - 1;
        int key_src = KEY_LED_MEM1 + src - 1;

        if(key_src >= KEY_LED_MEM1 && key_src <= KEY_LED_MEM8)
            QSwitcher::set_led(key_src,SWITCHER_LED_OFF);
        if(key_dst >= KEY_LED_MEM1 && key_dst <= KEY_LED_MEM8)
            QSwitcher::set_led(key_dst,SWITCHER_LED_ON);
    });
    connect(profile->macroPool(),&MacroPool::swapMacro,this,[=](int src,int dst){

    });
    //stream            stream比较特殊，setMenu也在models中调用
    connect(profile->streams(),&Streams::enableChanged,this,[=](bool enable){
        models->macroInvoke(&Models::streamEnable,enable);
    });
    connect(profile->streams()->stream1(),&Stream::platfromChanged,this,[=](QString platfrom){
        models->macroInvoke(&Models::streamPlatform,Streams::STREAM1,platfrom);
    });
    connect(profile->streams()->stream1(),&Stream::serverChanged,this,[=](QString server){
        models->macroInvoke(&Models::streamServer,Streams::STREAM1,server);
        settings->setMenuValue(MENU_FIRST_STREAM,STREAM_STREAM1,MENU_THIRD_STREAM_SERVER,server);
    });
    connect(profile->streams()->stream1(),&Stream::urlChanged,this,[=](QString url){
        settings->setMenuValue(MENU_FIRST_STREAM,STREAM_STREAM1,MENU_THIRD_STREAM_KEY,url);
    });
    connect(profile->streams()->stream1(),&Stream::outputChanged,this,[=](bool output){
        models->macroInvoke(&Models::streamOutput,Streams::STREAM1,output);
        settings->setMenuValue(MENU_FIRST_STREAM,STREAM_STREAM1,MENU_THIRD_STREAM_OUTPUT,output);
    });
    auto third = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM1]->third[MENU_THIRD_STREAM_PLATFORM];
    profile->streams()->stream1()->setPlatfrom(third->list_text[third->current.toInt()]);
    connect(profile->streams()->stream2(),&Stream::platfromChanged,this,[=](QString platfrom){
        models->macroInvoke(&Models::streamPlatform,Streams::STREAM2,platfrom);
    });
    connect(profile->streams()->stream2(),&Stream::serverChanged,this,[=](QString server){
        models->macroInvoke(&Models::streamServer,Streams::STREAM2,server);
        settings->setMenuValue(MENU_FIRST_STREAM,STREAM_STREAM2,MENU_THIRD_STREAM_SERVER,server);
    });
    connect(profile->streams()->stream2(),&Stream::urlChanged,this,[=](QString url){
        settings->setMenuValue(MENU_FIRST_STREAM,STREAM_STREAM2,MENU_THIRD_STREAM_KEY,url);
    });
    connect(profile->streams()->stream2(),&Stream::outputChanged,this,[=](bool output){
        models->macroInvoke(&Models::streamOutput,Streams::STREAM2,output);
        settings->setMenuValue(MENU_FIRST_STREAM,STREAM_STREAM2,MENU_THIRD_STREAM_OUTPUT,output);
    });
    //不调崩溃
    third = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM2]->third[MENU_THIRD_STREAM_PLATFORM];
    profile->streams()->stream2()->setPlatfrom(third->list_text[third->current.toInt()]);
    connect(profile->streams()->stream3(),&Stream::platfromChanged,this,[=](QString platfrom){
        models->macroInvoke(&Models::streamPlatform,Streams::STREAM3,platfrom);
    });
    connect(profile->streams()->stream3(),&Stream::serverChanged,this,[=](QString server){
        models->macroInvoke(&Models::streamServer,Streams::STREAM3,server);
        settings->setMenuValue(MENU_FIRST_STREAM,STREAM_STREAM3,MENU_THIRD_STREAM_SERVER,server);
    });
    connect(profile->streams()->stream3(),&Stream::urlChanged,this,[=](QString url){
        settings->setMenuValue(MENU_FIRST_STREAM,STREAM_STREAM3,MENU_THIRD_STREAM_KEY,url);
    });
    connect(profile->streams()->stream3(),&Stream::outputChanged,this,[=](bool output){
        models->macroInvoke(&Models::streamOutput,Streams::STREAM3,output);
        settings->setMenuValue(MENU_FIRST_STREAM,STREAM_STREAM3,MENU_THIRD_STREAM_OUTPUT,output);
    });
    //不调崩溃
    third = settings->listFirst()[MENU_FIRST_STREAM]->second[STREAM_STREAM3]->third[MENU_THIRD_STREAM_PLATFORM];
    profile->streams()->stream3()->setPlatfrom(third->list_text[third->current.toInt()]);

    connect(profile->playback(),&Playback::playbackListChanged,this,[=](int playbackList){
        models->macroInvoke(&Models::playbackList,Playback::playbackListIndexToString(playbackList));
        settings->setMenuValue(MENU_FIRST_PLAYBACK,PLAYBACK_PLAYBACK,MENU_THIRD_PLAYBACK_LIST,playbackList);
    });
    connect(profile->playback(),&Playback::sequentialChanged,this,[=](bool sequential){
        models->macroInvoke(&Models::playbackSequential,sequential);
        settings->setMenuValue(MENU_FIRST_PLAYBACK,PLAYBACK_PLAYBACK,MENU_THIRD_PLAYBACK_SEQUENTIAL,sequential);
    });
    connect(profile->playback(),&Playback::progressBarChanged,this,[=](bool progressBar){
        models->macroInvoke(&Models::playbackProgressBar,progressBar);
        settings->setMenuValue(MENU_FIRST_PLAYBACK,PLAYBACK_PLAYBACK,MENU_THIRD_PLAYBACK_PROGRESS_BAR,progressBar);
    });

//    Setting
    connect(profile->setting()->srcNames()->pgm(),&SrcName::nameChanged,this,[=](QString name){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_SRC_NAME,SRC_NAME_PGM,name);
    });
    connect(profile->setting()->srcNames()->pvw(),&SrcName::nameChanged,this,[=](QString name){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_SRC_NAME,SRC_NAME_PVW,name);
    });
    connect(profile->setting()->srcNames()->in1(),&SrcName::nameChanged,this,[=](QString name){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_SRC_NAME,SRC_NAME_IN1,name);
    });
    connect(profile->setting()->srcNames()->in2(),&SrcName::nameChanged,this,[=](QString name){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_SRC_NAME,SRC_NAME_IN2,name);
    });
    connect(profile->setting()->srcNames()->in3(),&SrcName::nameChanged,this,[=](QString name){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_SRC_NAME,SRC_NAME_IN3,name);
    });
    connect(profile->setting()->srcNames()->in4(),&SrcName::nameChanged,this,[=](QString name){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_SRC_NAME,SRC_NAME_IN4,name);
    });
    connect(profile->setting()->srcNames()->aux(),&SrcName::nameChanged,this,[=](QString name){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_SRC_NAME,SRC_NAME_AUX,name);
    });
    connect(profile->setting()->srcNames()->still1(),&SrcName::nameChanged,this,[=](QString name){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_SRC_NAME,SRC_NAME_STILL1,name);
    });
    connect(profile->setting()->srcNames()->still2(),&SrcName::nameChanged,this,[=](QString name){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_SRC_NAME,SRC_NAME_STILL2,name);
    });
    connect(profile->setting()->mvMeters()->pgm(),&MvMeter::enableChanged,this,[=](bool enable){
        models->macroInvoke(&Models::mvMeter,MvMeters::mvMeterIndexToString(MvMeters::PGM),enable);
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_MV_METER,MV_METER_PGM,enable);
    });
    connect(profile->setting()->mvMeters()->in1(),&MvMeter::enableChanged,this,[=](bool enable){
        models->macroInvoke(&Models::mvMeter,MvMeters::mvMeterIndexToString(MvMeters::IN1),enable);
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_MV_METER,MV_METER_IN1,enable);
    });
    connect(profile->setting()->mvMeters()->in2(),&MvMeter::enableChanged,this,[=](bool enable){
        models->macroInvoke(&Models::mvMeter,MvMeters::mvMeterIndexToString(MvMeters::IN2),enable);
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_MV_METER,MV_METER_IN2,enable);
    });
    connect(profile->setting()->mvMeters()->in3(),&MvMeter::enableChanged,this,[=](bool enable){
        models->macroInvoke(&Models::mvMeter,MvMeters::mvMeterIndexToString(MvMeters::IN3),enable);
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_MV_METER,MV_METER_IN3,enable);
    });
    connect(profile->setting()->mvMeters()->in4(),&MvMeter::enableChanged,this,[=](bool enable){
        models->macroInvoke(&Models::mvMeter,MvMeters::mvMeterIndexToString(MvMeters::IN4),enable);
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_MV_METER,MV_METER_IN4,enable);
    });
    connect(profile->setting()->mvMeters()->aux(),&MvMeter::enableChanged,this,[=](bool enable){
        models->macroInvoke(&Models::mvMeter,MvMeters::mvMeterIndexToString(MvMeters::AUX),enable);
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_MV_METER,MV_METER_AUX,enable);
    });
    connect(profile->setting()->mvLayout(),&MvLayout::layoutChanged,this,[=](int layout){
        models->macroInvoke(&Models::mvLayout,MvLayout::mvLayoutIndexToString(layout));
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_MV_LAYOUT,MV_LAYOUT_SWAP,layout);
    });
    connect(profile->setting()->marker(),&Marker::pvwMarkerChanged,this,[=](bool pvwMarker){
        models->macroInvoke(&Models::marker,pvwMarker);
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_MARKER,MARKER_PVW,pvwMarker);
    });
    connect(profile->setting()->micInputs()->mic1(),&MicInput::inputChanged,this,[=](int input){
        models->macroInvoke(&Models::micInput,"mic1",MicInputs::micInputIndexToString(input));
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_MIC_INPUT,MENU_THIRD_MIC_INPUT_MIC_1_INPUT,input);
    });
    connect(profile->setting()->micInputs()->mic2(),&MicInput::inputChanged,this,[=](int input){
        models->macroInvoke(&Models::micInput,"mic2",MicInputs::micInputIndexToString(input));
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_MIC_INPUT,MENU_THIRD_MIC_INPUT_MIC_2_INPUT,input);
    });
    connect(profile->setting()->record(),&Record::fileNameChanged,this,[=](QString fileName){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_RECORD_FILE,SETTING_RECORD_FILE_NAME,fileName);
    });
    connect(profile->setting()->srcSelections()->in1(),&SrcSelection::selectionChanged,this,[=](int selection){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_AUX_SOURCE,SETTING_AUX_SOURCE_IN1,selection);
        models->macroInvoke(&Models::srcSelection,SrcSelections::sourceIndexToString(SrcSelections::IN1),SrcSelections::selectionIndexToString(selection));
    });
    connect(profile->setting()->srcSelections()->in2(),&SrcSelection::selectionChanged,this,[=](int selection){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_AUX_SOURCE,SETTING_AUX_SOURCE_IN2,selection);
        models->macroInvoke(&Models::srcSelection,SrcSelections::sourceIndexToString(SrcSelections::IN2),SrcSelections::selectionIndexToString(selection));
    });
    connect(profile->setting()->srcSelections()->in3(),&SrcSelection::selectionChanged,this,[=](int selection){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_AUX_SOURCE,SETTING_AUX_SOURCE_IN3,selection);
        models->macroInvoke(&Models::srcSelection,SrcSelections::sourceIndexToString(SrcSelections::IN3),SrcSelections::selectionIndexToString(selection));
    });
    connect(profile->setting()->srcSelections()->in4(),&SrcSelection::selectionChanged,this,[=](int selection){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_AUX_SOURCE,SETTING_AUX_SOURCE_IN4,selection);
        models->macroInvoke(&Models::srcSelection,SrcSelections::sourceIndexToString(SrcSelections::IN4),SrcSelections::selectionIndexToString(selection));
    });
    connect(profile->setting()->srcSelections()->aux(),&SrcSelection::selectionChanged,this,[=](int selection){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_AUX_SOURCE,SETTING_AUX_SOURCE_SOURCE,selection);
        models->macroInvoke(&Models::srcSelection,SrcSelections::sourceIndexToString(SrcSelections::AUX),SrcSelections::auxIndexToString(selection));
    });
    connect(profile->setting()->outFormat()->format(),&Format::outFormatChanged,this,[=](int outFormat){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_OUT_FORMAT,SETTING_OUT_FORMAT_FORMAT,outFormat);
        models->macroInvoke(&Models::outFormat,OutFormat::outFormatIndexToString(outFormat));
    });
    connect(profile->setting()->outFormat()->out1ColorSpace(),&OutputColorSpace::colorSpaceChanged,this,[=](int colorSpace){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_OUT_FORMAT,SETTING_OUT_FORMAT_OUTPUT1_COLOR_SPACE,colorSpace);
        models->macroInvoke(&Models::outputColorSpace,OutFormat::outputIndexToString(OutFormat::OUTPUT1),SrcSelections::selectionIndexToString(colorSpace));
    });
    connect(profile->setting()->outFormat()->out2ColorSpace(),&OutputColorSpace::colorSpaceChanged,this,[=](int colorSpace){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_OUT_FORMAT,SETTING_OUT_FORMAT_OUTPUT2_COLOR_SPACE,colorSpace);
        models->macroInvoke(&Models::outputColorSpace,OutFormat::outputIndexToString(OutFormat::OUTPUT2),SrcSelections::selectionIndexToString(colorSpace));
    });
    connect(profile->setting()->outSources()->hdmi1(),&OutSource::sourceChanged,this,[=](int source){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_OUT_SOURCE,SETTING_OUT_SOURCE_HDMI1,source);
        models->macroInvoke(&Models::outSource,OutSources::srcIndexToString(OutSources::HDMI1),OutSources::sourceIndexToString(source));
    });
    connect(profile->setting()->outSources()->uvc(),&OutSource::sourceChanged,this,[=](int source){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_OUT_SOURCE,SETTING_OUT_SOURCE_AUX,source);
        models->macroInvoke(&Models::outSource,OutSources::srcIndexToString(OutSources::UVC),OutSources::sourceIndexToString(source));
    });
    connect(profile->setting()->record(),&Record::qualityChanged,this,[=](int quality){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_QUALITY,QUALITY_RECORDING,quality);
        models->macroInvoke(&Models::quality,Streams::srcIndexToString(Streams::RECORDING),Streams::qualityIndexToString(quality));
    });
    connect(profile->streams(),&Streams::qualityChanged,this,[=](int quality){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_QUALITY,QUALITY_STREAMING,quality);
        models->macroInvoke(&Models::quality,Streams::srcIndexToString(Streams::STREAMING),Streams::qualityIndexToString(quality));
    });
    connect(profile->setting()->network(),&Network::protocolChanged,this,[=](bool protocol){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_NETWORK,NETWORK_PROTOCOL,protocol);
        models->macroInvoke(&Models::protocol,protocol);
    });
    connect(profile->setting()->network()->iPAddress(),&IPAddress::valueChanged,this,[=](QString value){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_NETWORK,NETWORK_IP_ADDRESS,value);
        models->macroInvoke(&Models::iPAddress,value);
    });
    connect(profile->setting()->network()->subnetMask(),&SubnetMask::valueChanged,this,[=](QString value){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_NETWORK,NETWORK_SUBNET_MASK,value);
        models->macroInvoke(&Models::subnetMask,value);
    });
    connect(profile->setting()->network()->gateway(),&Gateway::valueChanged,this,[=](QString value){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_NETWORK,NETWORK_GATEWAY,value);
        models->macroInvoke(&Models::gateway,value);
    });
    connect(profile->setting()->network()->primaryDNS(),&PrimaryDNS::valueChanged,this,[=](QString value){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_NETWORK,NETWORK_PRIMARY_DNS,value);
        models->macroInvoke(&Models::primaryDNS,value);
    });
    connect(profile->setting()->network()->secondaryDNS(),&SecondaryDNS::valueChanged,this,[=](QString value){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_NETWORK,NETWORK_SECONDAY_DNS,value);
        models->macroInvoke(&Models::seconddaryDNS,value);
    });
    connect(profile->setting()->panel(),&Panel::ButtonBrightnessChanged,this,[=](int ButtonBrightness){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_PANEL,MENU_THIRD_PANEL_BUTTON_BRIGHTNESS,ButtonBrightness);
        models->macroInvoke(&Models::buttonBrightness,ButtonBrightness);
    });
    connect(profile->setting()->language(),&Language::languageChanged,this,[=](int language){
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_LANGUAGE,SETTING_LANGUAGE_LANGUAGE,language);
        models->language(language);
    });
}

void Control::slotKnobChanged(const int knob, int value)
{
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
                if(profile->setting()->srcSelections()->aux()->selection() == SrcSelections::SD_CARD){
                    size = playbackGroupManager->list().size();
                    current = playbackGroupManager->listCurrent();
                }
                else if(profile->setting()->srcSelections()->aux()->selection() == SrcSelections::NDI){
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
                if(profile->setting()->srcSelections()->aux()->selection() == SrcSelections::SD_CARD){
                    playbackGroupManager->setListCurrent(finalValue);
                }else if(profile->setting()->srcSelections()->aux()->selection() == SrcSelections::NDI){
                    settings->setNdiListDialogCurrent(finalValue);
                }
            }else{
                models->menuKnob(value);
            }
            break;
        case KNOB_AUDIO:
            if(settings->lastFirstUnfold() == MENU_FIRST_AUDIO_MIXER && settings->lastSecondUnfold() != -1){
                QString source ;
                switch (settings->lastSecondUnfold()) {
                case AUDIO_MIXER_MIC1:
                    source = AudioSource::sourceNameIndexToString(AudioSource::MIC1);
                    break;
                case AUDIO_MIXER_MIC2:
                    source = AudioSource::sourceNameIndexToString(AudioSource::MIC2);
                    break;
                case AUDIO_MIXER_IN1:
                    source = AudioSource::sourceNameIndexToString(AudioSource::IN1);
                    break;
                case AUDIO_MIXER_IN2:
                    source = AudioSource::sourceNameIndexToString(AudioSource::IN2);
                    break;
                case AUDIO_MIXER_IN3:
                    source = AudioSource::sourceNameIndexToString(AudioSource::IN3);
                    break;
                case AUDIO_MIXER_IN4:
                    source = AudioSource::sourceNameIndexToString(AudioSource::IN4);
                    break;
                case AUDIO_MIXER_AUX:
                    source = AudioSource::sourceNameIndexToString(AudioSource::AUX);
                    break;
                case AUDIO_MIXER_PGM:
                    source = AudioSource::sourceNameIndexToString(AudioSource::PGM);
                    break;
                }
                models->addAudioFaderByAudioKnob(source,value);
            }
            else
                models->addMonitorLevelByAudioKnob(value);
            break;
        }
}

void Control::slotPushChanged(const int push, const int value)
{
//    qDebug() << "push:" << push << "  value:" << value;
//    models->macroInvoke((&Models::transitionPosition),value);
}
void Control::slotKeyChanged(const int key, const int value)
{
    if(key >= KEY_LED_PGM_1 && key <= KEY_LED_PGM_STLL2 && value == 1){
        models->macroRecord(&Models::pgmIndex,MixEffectBlock::inputIndexToString(key - KEY_LED_PGM_1));
        return ;
    }
    if(key >= KEY_LED_PVW_1 && key <= KEY_LED_PVW_STLL2 && value == 1){
        models->macroRecord(&Models::pvwIndex,MixEffectBlock::inputIndexToString(key - KEY_LED_PVW_1));
        return ;
    }
    if(key == KEY_LED_TRANS_CUT && value == 1){
        models->macroRecord(&Models::cutTransition);
        return ;
    }
    if(key == KEY_LED_TRANS_AUTO && value == 1){
        models->macroRecord(&Models::autoTransition);
        return ;
    }
    if(key >= KEY_LED_TRANS_MIX && key <= KEY_LED_TRANS_WIPE)
        return ;
    if(key == KEY_LED_TRANS_PREVIEW)
        return ;
    if(key == KEY_LED_TRANS_FTB)
        return ;
    if(key == KEY_LED_KEY_ON_AIR)
        return ;
    if(key == KEY_LED_DSK_ON_AIR)
        return ;
    if(key >= KEY_LED_KEY && key <= KEY_LED_BKGD)
        return ;

    //key long pressed and short pressed
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

                if(key_pressed_index == KEY_LED_PLAYER_PLAY)
                {
                    //打开播放列表
                    if(!messageDialogControl->messageDialogVisible() && !settings->keyboardVisible())
                    {
                        if(profile->setting()->srcSelections()->aux()->selection() != SrcSelections::USB)
                            settings->setListDialogVisible(1);
                    }
                    return ;
                }
                else if(key_pressed_index == KEY_AUDIO_ENTER)
                {
                    models->resetFactory();
                    return ;
                }
                else if(key_pressed_index == KEY_LED_MEMU)
                {
                    if(messageDialogControl->messageDialogVisible() && messageDialogControl->type() == MessageDialogControl::MESSAGE_UPDATE)
                    {
                        messageDialogControl->cancel();
                        system("mount -o remount /dev/mmcblk0p7 /oem/");
                        settings->setIniValue(UPDATE_MESSAGE_FILE,"update",1);
                        system("mount -ro remount /dev/mmcblk0p7 /oem/");
                        return ;
                    }
                }
                else if(key_pressed_index >= KEY_LED_MEM1 && key_pressed_index <= KEY_LED_MEM8)
                {
                    //record macro
                    macroRecorder->startRecord(key_pressed_index - KEY_LED_MEM1 + 1);
                }
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
            settings->listKey()[key]->doWork(1);
        }
    }

//    //key keyOnAir dsk dskOnAir bkgd prev等键转交灯状态信号处理        mem key另外处理
//    if(key != KEY_LED_TRANS_PREVIEW
//        && key != KEY_LED_KEY_ON_AIR
//        && key != KEY_LED_DSK_ON_AIR
//        && key != KEY_LED_KEY
//        && key != KEY_LED_DSK
//        && key != KEY_LED_BKGD
//        && key != KEY_LED_PLAYER_PLAY
//        && key != KEY_AUDIO_ENTER
//        && key != KEY_LED_MEMU
//        && !(key >= KEY_LED_MEM1 && key <= KEY_LED_MEM8)){
//        //
//        settings->listKey()[key]->doWork(value);

//    }else if(key >= KEY_LED_MEM1 && key <= KEY_LED_MEM8){
//        //mem key  long pressed  and  short pressed
//        static int key_pressed_index = 0;
//        static int t_pressed = 0;
//        static QTimer *timer = nullptr;
//        if(timer == nullptr){
//            timer = new QTimer;
//            connect(timer,&QTimer::timeout,this,[=](){
//                t_pressed++;
//                if(t_pressed >= 2){
//                    qDebug() << "_______" << key_pressed_index << "  pressed:long" ;
//                    timer->stop();
//                    t_pressed = 0;
//                    settings->listKey()[key_pressed_index]->doWork(1);
//                }
//            });
//        }
//        if(value == 1){
//            //pressed
//            key_pressed_index = key;
//            timer->start(1000);
//        }else if(value == 0){
//            //released
//            if(timer->isActive()){
//                qDebug() << "_______" << key_pressed_index << "  pressed:short" ;
//                timer->stop();
//                t_pressed = 0;
//                settings->listKey()[key]->doWork(0);
//            }
//        }
//    }else if(key == KEY_LED_PLAYER_PLAY){
//        static QTimer *timer = nullptr;
//        static int t_pressed = 0;
//        if(timer == nullptr){
//            timer = new QTimer;
//            connect(timer,&QTimer::timeout,this,[=](){
//                t_pressed++;
//                if(t_pressed >= 2){
//                    qDebug() << "_________ play  pressed:long" ;
//                    timer->stop();
//                    t_pressed = 0;
//                    if(!messageDialogControl->messageDialogVisible() && !settings->keyboardVisible()){
//                        int auxSource = settings->reallyAuxSourceIndex() ;
//                        if(auxSource == AUX_SOURCE_SD_CARD || auxSource == AUX_SOURCE_NDI){
//                            settings->setListDialogVisible(1);
//                        }
//                    }

//                }
//            });
//        }
//        if(value == 1){
//            //pressed
//            timer->start(1000);
//        }else if(value == 0){
//            //released
//            if(timer->isActive()){
//                qDebug() << "_________ play  pressed:short" ;
//                timer->stop();
//                t_pressed = 0;
//                settings->listKey()[key]->doWork();
//            }
//        }
//    }else if(key == KEY_AUDIO_ENTER){
//        static QTimer *timer = nullptr;
//        static int t_pressed = 0;
//        if(timer == nullptr){
//            timer = new QTimer;
//            connect(timer,&QTimer::timeout,this,[=](){
//                t_pressed++;
//                if(t_pressed >= 3){
//                    qDebug() << "_________ audio enter  pressed:long" ;
//                    timer->stop();
//                    t_pressed = 0;
//                    models->resetFactory();
//                }
//            });
//        }
//        if(value == 1){
//            //pressed
//            timer->start(1000);
//        }else if(value == 0){
//            //released
//            if(timer->isActive()){
//                qDebug() << "_________ audio enter  pressed:short" ;
//                profile->write(profile);
//                timer->stop();
//                t_pressed = 0;
//            }
//        }
//    }else if(key == KEY_LED_MEMU){
//        settings->listKey()[key]->doWork(value);
//        if(!QFile::exists(UPDATE_MESSAGE_FILE) && !messageDialogControl->messageDialogVisible()){
//            messageDialogControl->dialogShow("",{},MessageDialogControl::MESSAGE_UPDATE);
//            return ;
//        }
//        static QTimer *timer = nullptr;
//        static int t_pressed = 0;
//        if(timer == nullptr){
//            timer = new QTimer;
//            connect(timer,&QTimer::timeout,this,[=](){
//                t_pressed++;
//                if(t_pressed >= 2){
//                    qDebug() << "_________ audio enter  pressed:long" ;
//                    timer->stop();
//                    t_pressed = 0;
//                    if(messageDialogControl->messageDialogVisible() && messageDialogControl->type() == MessageDialogControl::MESSAGE_UPDATE){
//                        messageDialogControl->cancel();
//                        system("mount -o remount /dev/mmcblk0p7 /oem/");
//                        settings->setIniValue(UPDATE_MESSAGE_FILE,"update",1);
//                        system("mount -ro remount /dev/mmcblk0p7 /oem/");
//                    }
//                }
//            });
//        }
//        if(value == 1){
//            //pressed
//            timer->start(1000);
//        }else if(value == 0){
//            //released
//            if(timer->isActive()){
//                qDebug() << "_________ audio enter  pressed:short" ;
//                timer->stop();
//                t_pressed = 0;
//            }
//        }
//    }
}

void Control::slotKeyStatusChanged(const int key, const int status)
{
    if(key >= KEY_LED_RECORDER_REC && key <= KEY_LED_PLAYER_PAUSE)
        return ;

    settings->listKey()[key]->doWork(status);
    //key keyOnAir dsk dskOnAir bkgd prev等键转交灯状态信号处理
//    if(key == KEY_LED_TRANS_PREVIEW
//        || key == KEY_LED_KEY_ON_AIR
//        || key == KEY_LED_DSK_ON_AIR
//        || key == KEY_LED_KEY
//        || key == KEY_LED_DSK
//        || key == KEY_LED_BKGD){
//        //
//        settings->listKey()[key]->doWork(1);
//    }

    //set play when pgm is aux and cut when the video is over
//    if(key == KEY_LED_PGM_AUX  && status == LED_STATUS_RED){
//        // pgm is aux
//        models->setPlay(0);
//    }
}
