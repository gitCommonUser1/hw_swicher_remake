#include "testqml.h"
#include <QSettings>
#include <QDebug>
#include "QSwitcher.h"
#include <QTimer>
#include "include/net_opr.h"
#include "include/uvc_ports.h"
#include "fpga.h"
#include "rv_switch_api.h"

#define TEST_FILE "/mnt/sdcard/Factory Test.txt"

TestQml::TestQml(QObject *parent) : QObject(parent)
{
    //Factory Test.txt     >>>      model=1
    QSettings settings(TEST_FILE,QSettings::IniFormat);
    setValue(settings.value("model").toInt());

    if(m_value == 1){
        //
        init1();
    }else if(m_value == 2){
        //
        init1();
        init2();
    }
}

TestQml::~TestQml()
{
    if(m_value == 2){
        qDebug() << "~22222";
        QSwitcher::set_leds_test(0);
    }
}

void TestQml::init1()
{
    //

    rv_switch_init(RV_1080P60);
    rv_switch_init_video_enc();

    set_uvc_out_mode(UVC_OUT_HOST);

    setEthernetFlag(false);
    setUsbOutFlag(false);
    setUsbInFlag(false);
    setRtcFlag(false);

    //set audio
    fpga_write(&g_fpga,FPGA_IN1_L_LEV,56625);
    fpga_write(&g_fpga,FPGA_IN1_R_LEV,56625);
    fpga_write(&g_fpga,FPGA_IN2_L_LEV,56625);
    fpga_write(&g_fpga,FPGA_IN2_R_LEV,56625);
    fpga_write(&g_fpga,FPGA_IN3_L_LEV,56625);
    fpga_write(&g_fpga,FPGA_IN3_R_LEV,56625);
    fpga_write(&g_fpga,FPGA_IN4_L_LEV,56625);
    fpga_write(&g_fpga,FPGA_IN4_R_LEV,56625);
    fpga_write(&g_fpga,FPGA_AUX_L_LEV,56625);
    fpga_write(&g_fpga,FPGA_AUX_R_LEV,56625);
    fpga_write(&g_fpga,FPGA_MIC1_L_LEV,56625);
    fpga_write(&g_fpga,FPGA_MIC1_R_LEV,56625);
    fpga_write(&g_fpga,FPGA_MIC2_L_LEV,56625);
    fpga_write(&g_fpga,FPGA_MIC2_R_LEV,56625);
    fpga_write(&g_fpga,FPGA_MASTER_LEV,56625);
    fpga_write(&g_fpga,FPGA_AUDIO_ON,127);

    fpga_write(&g_fpga,MON_LEVEL,1843);
    fpga_write(&g_fpga,MON_SOURCE,5);



    QTimer *timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,[=](){
        //
        if(!m_ethernetFlag){
            if(get_netspeedmode(ETHERNET_NETIF) == 1000)
                setEthernetFlag(true);
        }
        if(!m_usbOutFlag){
            char cmd[] = "usb=`cat /sys/kernel/debug/usb/devices | grep -w \"Bus=03 Lev=01\"`;test -n \"$usb\"";
            if(!system(cmd)){
                setUsbOutFlag(true);
            }
        }
        if(!m_usbInFlag){
            char cmd[] = "usb=`cat /sys/kernel/debug/usb/devices | grep -w \"Bus=01 Lev=01\"`;test -n \"$usb\"";
            if(!system(cmd)){
                setUsbInFlag(true);
            }
        }
        if(!m_rtcFlag){
            char cmd[] = "hwclock -r";
            if(!system(cmd)){
                setRtcFlag(true);
            }
        }
    });
    timer->start(500);
}

void TestQml::init2()
{
    //
    audio_min = false;
    audio_max = false;
    audio_pressed = false;
    menu_min = false;
    menu_max = false;
    menu_pressed = false;

    QSwitcher::set_leds_brightness(1);
    QSwitcher::set_leds_test(1);
}

void TestQml::initMap()
{
    if(!qml_object)
        return ;

    m_objects[KEY_LED_LIVE] = qml_object->findChild<QObject*>("live");

    m_objects[KEY_LED_AUDIO_MIC1] = qml_object->findChild<QObject*>("mic1");
    m_objects[KEY_LED_AUDIO_MIC2] = qml_object->findChild<QObject*>("mic2");
    m_objects[KEY_LED_AUDIO_IN1] = qml_object->findChild<QObject*>("in1");
    m_objects[KEY_LED_AUDIO_IN2] = qml_object->findChild<QObject*>("in2");
    m_objects[KEY_LED_AUDIO_IN3] = qml_object->findChild<QObject*>("in3");
    m_objects[KEY_LED_AUDIO_IN4] = qml_object->findChild<QObject*>("in4");
    m_objects[KEY_LED_AUDIO_AUX] = qml_object->findChild<QObject*>("aux");
    m_objects[KEY_LED_AUDIO_PGM] = qml_object->findChild<QObject*>("pgm");
    m_objects[KEY_LED_AUDIO_AFV] = qml_object->findChild<QObject*>("afv");
    m_objects[KEY_LED_AUDIO_ON] = qml_object->findChild<QObject*>("on");

    m_objects[KEY_LED_MEM1] = qml_object->findChild<QObject*>("mem1");
    m_objects[KEY_LED_MEM2] = qml_object->findChild<QObject*>("mem2");
    m_objects[KEY_LED_MEM3] = qml_object->findChild<QObject*>("mem3");
    m_objects[KEY_LED_MEM4] = qml_object->findChild<QObject*>("mem4");
    m_objects[KEY_LED_MEM5] = qml_object->findChild<QObject*>("mem5");
    m_objects[KEY_LED_MEM6] = qml_object->findChild<QObject*>("mem6");
    m_objects[KEY_LED_MEM7] = qml_object->findChild<QObject*>("mem7");
    m_objects[KEY_LED_MEM8] = qml_object->findChild<QObject*>("mem8");

    m_objects[KEY_LED_MEMU] = qml_object->findChild<QObject*>("menu");

    m_objects[KEY_LED_RECORDER_REC] = qml_object->findChild<QObject*>("record");
    m_objects[KEY_LED_RECORDER_STOP] = qml_object->findChild<QObject*>("record_stop");
    m_objects[KEY_LED_PLAYER_PREV] = qml_object->findChild<QObject*>("previous");
    m_objects[KEY_LED_PLAYER_NEXT] = qml_object->findChild<QObject*>("next");
    m_objects[KEY_LED_PLAYER_PLAY] = qml_object->findChild<QObject*>("play");
    m_objects[KEY_LED_PLAYER_PAUSE] = qml_object->findChild<QObject*>("play_stop");

    m_objects[KEY_LED_KEY_ON_AIR] = qml_object->findChild<QObject*>("key_on_air");
    m_objects[KEY_LED_DSK_ON_AIR] = qml_object->findChild<QObject*>("dsk_on_air");
    m_objects[KEY_LED_KEY] = qml_object->findChild<QObject*>("key");
    m_objects[KEY_LED_DSK] = qml_object->findChild<QObject*>("dsk");
    m_objects[KEY_LED_BKGD] = qml_object->findChild<QObject*>("bkgd");

    m_objects[KEY_LED_PVW_1] = qml_object->findChild<QObject*>("pvw1");
    m_objects[KEY_LED_PVW_2] = qml_object->findChild<QObject*>("pvw2");
    m_objects[KEY_LED_PVW_3] = qml_object->findChild<QObject*>("pvw3");
    m_objects[KEY_LED_PVW_4] = qml_object->findChild<QObject*>("pvw4");
    m_objects[KEY_LED_PVW_AUX] = qml_object->findChild<QObject*>("pvw_aux");
    m_objects[KEY_LED_PVW_BLACK] = qml_object->findChild<QObject*>("pvw_black");
    m_objects[KEY_LED_PVW_STLL1] = qml_object->findChild<QObject*>("pvw_still1");
    m_objects[KEY_LED_PVW_STLL2] = qml_object->findChild<QObject*>("pvw_still2");

    m_objects[KEY_LED_PGM_1] = qml_object->findChild<QObject*>("pgm1");
    m_objects[KEY_LED_PGM_2] = qml_object->findChild<QObject*>("pgm2");
    m_objects[KEY_LED_PGM_3] = qml_object->findChild<QObject*>("pgm3");
    m_objects[KEY_LED_PGM_4] = qml_object->findChild<QObject*>("pgm4");
    m_objects[KEY_LED_PGM_AUX] = qml_object->findChild<QObject*>("pgm_aux");
    m_objects[KEY_LED_PGM_BLACK] = qml_object->findChild<QObject*>("pgm_black");
    m_objects[KEY_LED_PGM_STLL1] = qml_object->findChild<QObject*>("pgm_still1");
    m_objects[KEY_LED_PGM_STLL2] = qml_object->findChild<QObject*>("pgm_still2");

    m_objects[KEY_LED_TRANS_MIX] = qml_object->findChild<QObject*>("mix");
    m_objects[KEY_LED_TRANS_DIP] = qml_object->findChild<QObject*>("dip");
    m_objects[KEY_LED_TRANS_WIPE] = qml_object->findChild<QObject*>("wipe");
    m_objects[KEY_LED_TRANS_PREVIEW] = qml_object->findChild<QObject*>("prev");
    m_objects[KEY_LED_TRANS_FTB] = qml_object->findChild<QObject*>("ftb");
    m_objects[KEY_LED_TRANS_AUTO] = qml_object->findChild<QObject*>("auto");
    m_objects[KEY_LED_TRANS_CUT] = qml_object->findChild<QObject*>("cut");

    m_objects[PUSH_ROD] = qml_object->findChild<QObject*>("push");

    m_objects[KNOB_AUDIO] = qml_object->findChild<QObject*>("round_audio");

    m_objects[KNOB_MENU] = qml_object->findChild<QObject*>("round_menu");
}

void TestQml::keyChanged(int key, int value)
{
    if(!qml_object)
        return ;

    if(value == 1){
        if(key == KEY_AUDIO_ENTER){
            if(!audio_pressed)
                    audio_pressed = true;
        }
        else if(key == KEY_MENU_ENTER){
            if(!menu_pressed)
                menu_pressed = true;
        }
        else
            m_objects[key]->setProperty("isOk",true);
    }

    if(audio_max && audio_min && audio_pressed)
        m_objects[KNOB_AUDIO]->setProperty("isOk",true);

    if(menu_max && menu_min && menu_pressed)
        m_objects[KNOB_MENU]->setProperty("isOk",true);
}

void TestQml::knobChanged(int key, int value)
{
    if(key == KNOB_AUDIO){
        if(value > 0 && !audio_max)
            audio_max = true;
        if(value < 0 && !audio_min)
            audio_min = true;
    }else if(key == KNOB_MENU){
        if(value > 0 && !menu_max)
            menu_max = true;
        if(value < 0 && !menu_min)
            menu_min = true;
    }

    if(audio_max && audio_min && audio_pressed)
        m_objects[KNOB_AUDIO]->setProperty("isOk",true);

    if(menu_max && menu_min && menu_pressed)
        m_objects[KNOB_MENU]->setProperty("isOk",true);
}

void TestQml::pushChanged(int key, int value)
{
    static bool min = false;
    static bool max = false;

    if(value == 0)
        min = true;
    if(value == 256)
        max = true;

    if(min && max)
        m_objects[PUSH_ROD]->setProperty("isOk",true);
}
