#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QObject>
#include <QFont>
#include <QFontDatabase>
#include "settings.h"
#include "control.h"
#include "src/QSwitcher.h"
#include "models.h"
#include "leftmenumodel.h"
#include "rightmenumodel.h"
#include "qthread_uevent.h"
#include "qthread_route.h"
#include "media_sd.h"
#include "sdimages.h"
#include "myprovider.h"
#include "MenuEnums.h"
#include <QtConcurrent>
#include "keyboardevent.h"
#include <QTimer>
#include "fpga.h"
#include "testqml.h"
#include <QQuickItem>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <execinfo.h>
#include <pthread.h>
#include <QFile>
#include "playbackgroupmanager.h"
#include "osee_syslog.h"

#include "qthread_connect.h"
#include "messagedialogcontrol.h"
#include "regexp.h"
#include "include/general.h"

#include "rv_switch_api.h"
#include "hdmi_out.h"
#include "ndi.h"
#include "device_info.h"
#include "ndi_license.h"
#include "gostreamsystem.h"
#include "profile_include.h"
#include "playbackgroupmanager.h"
#include "macrorunner.h"
#include "macrorecorder.h"

Models *models;
Settings *settings;
LeftMenuModel *leftMenuModel;
RightMenuModel *rightMenuModel;
QSwitcher *switcher;
SDImages *sdImages;
MyProvider *myProvider;
KeyboardEvent *keyboardEvent;
MessageDialogControl *messageDialogControl;
Ndi *ndi;
Profile *profile;
PlaybackGroupManager *playbackGroupManager;
MacroRunner *macroRunner;
MacroRecorder *macroRecorder;

bool g_running = true;

bool init_settings_is_ok = false;

static void print_backtrace(void)
{
#define BCACKTRACE_DEEP 100
    void* array[BCACKTRACE_DEEP];
    int size, i;
    char** strings;

    size = backtrace(array, BCACKTRACE_DEEP);

    //    syslog(LOG_WARNING, "\nBacktrace (%d deep):\n", size);
    fprintf(stderr, "\nBacktrace (%d deep):\n", size);

    strings = backtrace_symbols(array, size);
    if (strings == NULL)
    {
        SYS_ALERT("%s: malloc error\n", __func__);
        return;
    }

    for (i = 0; i < size; i++)
    {
        //        syslog(LOG_WARNING, "%d: %s\n", i, strings[i]);
        fprintf(stderr, "%d: %s\n", i, strings[i]);
    }
    free(strings);
}

void handler(int sig)
{
    SYS_ALERT("ALERT: CATCH SIG(%d) !\n", sig);

    print_backtrace();

    g_running = 0;

    exit(sig);
}

void usage(void)
{
    printf("%s v%d.%d.%d (%s %s)\n\n", OSEE_HW_NAME,
           OSEE_HW_VERSION_MAJOR, OSEE_HW_VERSION_MINOR, OSEE_HW_VERSION_PATCH,
           __DATE__, __TIME__);

    printf("Usage: %s [-v] [-l level]\n", OSEE_HW_NAME);
    printf("\ninit %s\n\n", OSEE_HW_NAME);
    printf("\t-l level       Set debug level\n");
    printf("\t-v             Print version\n");
}

void software_info(void)
{
    QString _dateTime;
    _dateTime += __DATE__;
    _dateTime += __TIME__;

    QDateTime dateTime = QLocale(QLocale::English).toDateTime(_dateTime, "MMM dd yyyyhh:mm:ss");

    printf("%s v%d.%d.%d (%s %s)\n\n", OSEE_HW_NAME,
           OSEE_HW_VERSION_MAJOR, OSEE_HW_VERSION_MINOR, OSEE_HW_VERSION_PATCH,
           __DATE__, __TIME__);
    printf("Copyright (c) 2001-%d Osee Technology Co. Ltd. <http://www.osee-dig.com.cn>.\n", dateTime.date().year());
}


bool checkQtKmsIsOk()
{
    bool read_error = false;

    //判断qtkms与Setting中out format是否一致
    QFile qtkms(QTKMS_FILE_PATH);
    if(!qtkms.open(QIODevice::ReadOnly)){
        SYS_ERR("qtmks open fail!!!!!\n",__FUNCTION__,__LINE__);
        read_error = true;
    }
    auto byteArray = qtkms.readAll();
    qtkms.close();

    QString out_format = "";
    QJsonParseError json_error;
    QJsonDocument document=QJsonDocument::fromJson(byteArray, &json_error);
    if(json_error.error==QJsonParseError::NoError){
        if(document.isObject()){
            const QJsonObject obj=document.object();
            if(!(obj.contains("device") && obj.contains("hwcursor") && obj.contains("pbuffers") && obj.contains("outputs") && obj.size() == 4))
                read_error = true;
            if(obj["device"] != "/dev/dri/card0" || obj["hwcursor"].toBool() != false || obj["pbuffers"].toBool() != true){
                read_error = true;
            }
            QJsonArray array = obj["outputs"].toArray();
            if(array.size() != 1){
                read_error = true;
            }
            else{
                QJsonObject outObj = array[0].toObject();
                if(outObj["name"] != "HDMI1" || outObj["format"] != "ARGB8888"){
                     read_error = true;
                }
                QString outformat = outObj["mode"].toString();
                if(outformat == "1920x1080@30"){
                    out_format = "1080p30";
                }else if(outformat == "1920x1080@24"){
                    out_format = "1080p24";
                }else if(outformat == "1920x1080@25"){
                    out_format = "1080p25";
                }else if(outformat == "1920x1080@50"){
                    out_format = "1080p50";
                }else if(outformat == "1920x1080@60"){
                    out_format = "1080p60";
                }
            }
        }else{
            read_error = true;
        }
    }else{
       qDebug()<<"json error:"<<json_error.errorString();
       read_error = true;
    }

    if(out_format == ""){
        read_error = true;
    }else{
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_OUT_FORMAT,SETTING_OUT_FORMAT_FORMAT,out_format);
    }

    if(read_error){
        // if error set default 1080p60  0x4
        rv_switch_resolutionset(0x4);
        settings->setMenuValue(MENU_FIRST_SETTING,SETTING_OUT_FORMAT,SETTING_OUT_FORMAT_FORMAT,"1080p60");
        return false;
    }else{
        return true;
    }
}

void set_s9022_output()
{
    int out_format = settings->listFirst()[MENU_FIRST_SETTING]->second[SETTING_OUT_FORMAT]->third[SETTING_OUT_FORMAT_FORMAT]->current.toInt();

    for(int i = 0; i < HDMI_OUT_NUM; i++)
        set_hdmi_out_format(i, out_format);
}

int main(int argc, char *argv[])
{
/* ISO C99 signals.  */
    signal(SIGINT, handler);    /* Interactive attention signal.  */
    signal(SIGILL, handler);    /* Illegal instruction.  */
    signal(SIGABRT, handler);   /* Abnormal termination.  */
    signal(SIGFPE, handler);    /* Erroneous arithmetic operation.  */
    signal(SIGSEGV, handler);   /* Invalid access to storage.  */
    signal(SIGTERM, handler);   /* Termination request.  */

    /* Historical signals specified by POSIX. */
    signal(SIGBUS, handler);    /* Bus error.  */
    signal(SIGSYS, handler);    /* Bad system call.  */
    signal(SIGPIPE, handler);   /* Broken pipe.  */
    signal(SIGALRM, handler);   /* Alarm clock.  */

    /* New(er) POSIX signals (1003.1-2008, 1003.1-2013).  */
    signal(SIGXFSZ, handler);   /* File size limit exceeded.  */
    int ch;

    while ((ch = getopt(argc, argv,
                "vh?")) != -1)
            switch (ch) {
                case 'v':
                    software_info();
                    return 0;
                case 'l':
                    sscanf(optarg, "%i", (int *)&g_syslog_priority);
                    break;
                case 'h':
                case '?':
                default:
                    usage();
                    return 0;
            }
        argc -= optind;
        argv += optind;

    openlog(OSEE_HW_NAME, LOG_PID | LOG_CONS, 0);
    setlogmask(g_syslog_priority);

    //恢复出厂设置
    QFile reset_file(FACTORY_RESET_FILE_PATH);
    if(reset_file.exists())
    {
//        system("rm " DATA_PATH "/*.ini");
        system("rm " STILLPATH "*");
//        system("rm " MACRO_PATH "*");
//        system("cp " MACRO_DEFAULT_FILE_PATH " " MACRO_PATH);
        system("rm " FACTORY_RESET_FILE_PATH);
        system("rm " DATA_PATH "/*.xml");
        //这里copy一个默认的profile.xml过来
        //
        //
        system("sync");
    }

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication *app = new QGuiApplication(argc, argv);
    int id = QFontDatabase::addApplicationFont(FONT_PATH);
    /* 指定开发板使用字库的路径 */
    if (id == -1) {
        qDebug("Cannot open font roboto-regular");
    } else {
        qDebug("font loaded!!");
        QString family = QFontDatabase::applicationFontFamilies(id).at(0);
        QFont roboto(family);
        app->setFont(roboto);
    }

    QQmlApplicationEngine engine;


    //profile
    profile = new Profile;

    messageDialogControl = new MessageDialogControl;
    engine.rootContext()->setContextProperty("messageDialogControl",messageDialogControl);
    qmlRegisterType<MessageDialogControl>("MessageDialogControl", 1, 0, "MessageDialogControl");

    qthread_uevent.start();
    qthread_route.start();
    switcher = new QSwitcher();
    switcher->start();
    fpga_init(&g_fpga);

    //增加测试界面
    TestQml *testQml = new TestQml();
    if(testQml->value() != 0){
        //打开音频表
        fpga_write(&g_fpga,FPGA_OSD_CTRL,2);
        engine.rootContext()->setContextProperty("testQml",testQml);
        engine.rootContext()->setContextProperty("switcher",switcher);
        engine.rootContext()->setContextProperty("qthread_uevent",&qthread_uevent);
        QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:/test.qml")));
        QObject* object = component.create();
        testQml->setQmlObject(object);
        if(testQml->value() == 2){
            testQml->initMap();
            QObject::connect(switcher,&QSwitcher::keyChanged,testQml,&TestQml::keyChanged);
            QObject::connect(switcher,&QSwitcher::knobChanged,testQml,&TestQml::knobChanged);
            QObject::connect(switcher,&QSwitcher::pushChanged,testQml,&TestQml::pushChanged);
        }
        QObject::connect(&qthread_uevent,&QThreadUevent::emit_sd_change,app,[=](bool flag){
            if(!flag){
                delete testQml;
                app->exit(0);
            }
        });
        return app->exec();
    }

    myProvider = new MyProvider();
    engine.addImageProvider(QLatin1String("still"), myProvider);

    qRegisterMetaType<std::map<QString, QString>>("std::map<QString, QString>");
    qmlRegisterType<RightMenuModel>("RightMenuData", 1, 0, "RightMenuData");

    models = new Models();
    models->setEngine(&engine);
    settings = new Settings();
    settings->init_menuStatus();

    models->initStillUpLoadRvSwitch();

    leftMenuModel = new LeftMenuModel();
    rightMenuModel = new RightMenuModel();

    media_sd = new Media_sd();
    media_sd->current_usage();
    int second = models->sd_remaintime_calc();
    media_sd->checkGbFree(second);

    sdImages = new SDImages();
    QObject::connect(sdImages,&SDImages::imageListChanged,settings,&Settings::sdImagesChanged);
    QObject::connect(sdImages,&SDImages::videoListChanged,app,[=](QList<QString>list){

    });
    QObject::connect(sdImages,&SDImages::streamKeyListChanged,settings,&Settings::streamKeyListChanged);
    QObject::connect(sdImages,&SDImages::macroListChanged,settings,&Settings::macroListChanged);
    sdImages->flushImageList();
    sdImages->flushVideoList();
    sdImages->flushStreamKeyList();
    sdImages->flushMacroList();

    playbackGroupManager = new PlaybackGroupManager();
    engine.rootContext()->setContextProperty("playbackGroupManager",playbackGroupManager);

    QObject::connect(&qthread_uevent,&QThreadUevent::emit_sd_change,app,[=](bool flag){
        if(!flag){
            //player control
            models->playStop();
            models->recordStop();

            //sd dir check
            sdImages->m_imageList.clear();
            sdImages->imageListChanged(sdImages->m_imageList);
            sdImages->m_videoList.clear();
            sdImages->videoListChanged(sdImages->m_videoList);
            sdImages->m_streamKeyList.clear();
            sdImages->streamKeyListChanged(sdImages->m_streamKeyList);
            sdImages->m_macroList.clear();
            sdImages->macroListChanged(sdImages->m_macroList);
            sdImages->watcher->removePaths(sdImages->watcher->directories());

            //sd capacity
            media_sd->current_usage();
            int second = models->sd_remaintime_calc();
            media_sd->checkGbFree(second);

            playbackGroupManager->clearGroups();
        }else{
            //刚接到信号时，文件系统未初始化完毕
            QTimer::singleShot(500,[=](){
                //sd dir check
                //if mount fail return and show dialog !
                if(!media_sd->is_online())
                    return ;
                if(is_mounted(SD_MOUNTED_PATH) != 0){
                    messageDialogControl->dialogShow(QObject::tr("Your SD card failed to mount, is it formatted?"),{QObject::tr("Cancel"),QObject::tr("Confirm")},MessageDialogControl::MESSAGE_SD_FORMAT);
                    qthread_uevent.emit_sd_change(false);
                    return ;
                }

                QDir dir (SD_IMAGE_PATH);
                if(!dir.exists())
                {
                    dir.mkdir(SD_IMAGE_PATH);
                    system("sync -f " SD_IMAGE_PATH);
                }
                QDir dir2 (SD_VIDEO_PATH);
                if(!dir2.exists())
                {
                    dir2.mkdir(SD_VIDEO_PATH);
                    system("sync -f " SD_VIDEO_PATH);
                }
                QDir dir3 (SD_STREAM_KEY_PATH);
                if(!dir3.exists())
                {
                    dir3.mkdir(SD_STREAM_KEY_PATH);
                    system("sync -f " SD_STREAM_KEY_PATH);
                }
                QDir dir4 (SD_MACRO_PATH);
                if(!dir4.exists())
                {
                    dir4.mkdir(SD_MACRO_PATH);
                    system("sync -f " SD_MACRO_PATH);
                }
                if(!QFile::exists(GROUP_FILE))
                {
                    QFile file(GROUP_FILE);
                    file.open(QIODevice::WriteOnly);
                    file.flush();
                    fsync(file.handle());
                    file.close();
                }

                //

                sdImages->watcher->addPath(SD_IMAGE_PATH);
                sdImages->watcher->addPath(SD_VIDEO_PATH);
                sdImages->watcher->addPath(SD_STREAM_KEY_PATH);
                sdImages->watcher->addPath(SD_MACRO_PATH);
                sdImages->flushImageList();
                sdImages->flushVideoList();
                sdImages->flushStreamKeyList();
                sdImages->flushMacroList();

                //sd capacity
                media_sd->current_usage();
                int second = models->sd_remaintime_calc();
                media_sd->checkGbFree(second);

                playbackGroupManager->readGroups();
                if(playbackGroupManager->list().size() != 0 && settings->playLedStatus() == E_STATUS_MP4_CLOSE){
                    settings->setPlayingIndex(0);
                    models->playPause(1);
                    models->playStart();
                }
                //generate ndi license
                if(get_ndi_license_state()){
                    qDebug() << "_____get_ndi_license";
                    qDebug() << get_ndi_license();
                }
            });
        }
    });

    if(media_sd->is_online())
    {
        qthread_uevent.emit_sd_change(true);
    }
    else
    {
        qthread_uevent.emit_sd_change(false);
    }

    keyboardEvent = new KeyboardEvent();
    qmlRegisterType<KeyboardEvent>("KeyboardEvent", 1, 0, "KeyboardEvent");

    checkQtKmsIsOk();
    set_s9022_output();

    qRegisterMetaType<NdiItemClass>("NdiItemClass");
    qRegisterMetaType<QList<NdiItemClass>>("QList<NdiItemClass>");
    //ndi
    ndi = new Ndi();
    engine.rootContext()->setContextProperty("ndi",ndi);
    engine.rootContext()->setContextProperty("ndiListModelItem",ndi->ndiListModelItem());

    //macro
    macroRunner = new MacroRunner();
    macroRecorder = new MacroRecorder();

    //控制
    Control control;
    engine.rootContext()->setContextProperty("control",&control);

    profile->emitSignals();
    init_settings_is_ok = true;

    //init color
    //这里必须调用一次，初始化后color应该是绿色，否则颜色不对
    //初始化 chroma  smp color
    int r = profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->red();
    int g = profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->green();
    int b = profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->blue();
    int y = profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->y();
    int cb = profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->cb();
    int cr = profile->mixEffectBlocks()->mixEffectBlock()->keys()->chromaParameters()->cr();
    settings->setColor5Data({r,g,b});
    settings->setChromakeySMPColorYcbcr({y,cb,cr});


    models->initDHCPNetworkData();

    RegExp *regExp = new RegExp();

    engine.rootContext()->setContextProperty("settings",settings);
    engine.rootContext()->setContextProperty("leftMenuModel",leftMenuModel);
    engine.rootContext()->setContextProperty("rightMenuModel",rightMenuModel);
    engine.rootContext()->setContextProperty("models",models);
    engine.rootContext()->setContextProperty("keyboardEvent",keyboardEvent);
    engine.rootContext()->setContextProperty("media_sd",media_sd);
    engine.rootContext()->setContextProperty("regExp",regExp);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    QThreadConnect *connect = new QThreadConnect();
    connect->start();

    return app->exec();
}
