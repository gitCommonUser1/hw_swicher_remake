#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QVariant>
#include "MenuDataType.h"
#include "include/hw_config.h"
#include <QSettings>
#include "QSwitcher.h"
#include <QTextCodec>
#include <QFile>
#include <QQmlApplicationEngine>
#include "keyboardevent.h"
#include "regexp.h"

class KeyAbstractClass;

struct STREAM_PROFILE{
    QString resolution;
    QString fps;
    int bitrate;
    int audio_bitrate;
};

struct STREAM_SERVER{
    std::map<QString,QString>name_url;
    std::map<QString,QList<STREAM_PROFILE>>name_config;
};

typedef std::map<QString,STREAM_SERVER> StreamData ;

class Settings : public QObject
{
    Q_OBJECT
    Q_ENUMS(MENU_SHOW_STATUS)
public:
    enum MENU_SHOW_STATUS{
        MENU_HIDE = 0,
        MENU_SHOW_BIG,
        MENU_SHOW_SMALL
    };

    Q_INVOKABLE QString toLocal8bitString(QString text){
        return text.toLocal8Bit();
    }

    Q_INVOKABLE QList<int> ipAddressToList(QString ipAddress){
#define IP_SEGMENT 4
        QList<int> list;
        if(!RegExp::exactMatch(REG_IPADDRESS,ipAddress)){
            return list;
        }
        auto lt = ipAddress.split(".");
        for(int i = 0;i < lt.size();++i){
            list << lt[i].toInt();
        }
        return list;
    };

    Q_INVOKABLE QString listToIpAddress(QList<int>list){
        QString ipAddress = "";
        if(list.size() != 4){
            return ipAddress;
        }
        for(int i = 0;i < list.size();++i){
            ipAddress += QString::number(list[i]);
            if(i != list.size() - 1)
                ipAddress += ".";
        }
        if(!RegExp::exactMatch(REG_IPADDRESS,ipAddress)){
            return "";
        }
        return ipAddress;
    }

    Q_INVOKABLE bool showColorBack1Flag();
    Q_INVOKABLE bool showColorBack2Flag();
    Q_INVOKABLE bool showSuperSourceBorder1ColorFlag();
    Q_INVOKABLE bool showSuperSourceBorder2ColorFlag();
    Q_INVOKABLE bool showChromaKeyControlColorFlag();
    Q_INVOKABLE bool showPIPBorderColorFLag();
    Q_INVOKABLE bool isTransitionSelect();
    Q_INVOKABLE bool isKeyPatternSelect();

    explicit Settings(QObject *parent = nullptr);
    void init_settings();
    void init_stillImages();
    void init_chromaColor();
    void reset_settings();
    void init_menuStatus();
    void init_menuData();
    void init_keyData();
    void init_PgmTallyTimer();
    void new_menuData();

    void new_ColorBack();
    void new_SuperSource();
    void new_PIP();
    void new_KeyType();
    void new_LumaKey();
    void new_ChromaKey();
    void new_KeyPattern();
    void new_Transition();
    void new_DSK();
    void new_FTB();
    void new_AudioMixer();
    void new_StillGenerator();
    void new_Macro();
    void new_Stream();
    void new_Playback();
    void new_Setting();

    MenuSecond* new_Color1();
    MenuSecond* new_Color2();

    MenuSecond* new_SuperSource_Enable();
    MenuSecond* new_SuperSource_Source();
    MenuSecond* new_SuperSource_Control();
    MenuSecond* new_SuperSource_Mask1();
    MenuSecond* new_SuperSource_Mask2();
    MenuSecond* new_SuperSource_Board1();
    MenuSecond* new_SuperSource_Board2();

    MenuSecond* new_type();

    MenuSecond* new_LumaKey_Source();
    MenuSecond* new_LumaKey_Mask();
    MenuSecond* new_LumaKey_Control();
    MenuSecond* new_LumaKey_Resize();

    MenuSecond* new_KeyPattern_Source();
    MenuSecond* new_KeyPattern_Wipe();
    MenuSecond* new_KeyPattern_Mask();
    MenuSecond* new_KeyPattern_Resize();

    MenuSecond* new_ChromaKey_Source();
    MenuSecond* new_ChromaKey_Mask();
    MenuSecond* new_ChromaKey_Resize();
    MenuSecond* new_ChromaKey_Control();

    MenuSecond* new_PIP_Source();
    MenuSecond* new_PIP_Size_Position();
    MenuSecond* new_PIP_Mask();
    MenuSecond* new_PIP_Board();
    MenuSecond* new_PIP_Color();

    MenuSecond* new_DSK_Source();
    MenuSecond* new_DSK_Mask();
    MenuSecond* new_DSK_Control();
    MenuSecond* new_DSK_Rate();


    MenuSecond* new_FTB_Rate();
    MenuSecond* new_FTB_Audio();


    MenuSecond* new_Transition_Mix();
    MenuSecond* new_Transition_Dip();
    MenuSecond* new_Transition_Wipe();

    MenuSecond* new_Mic1();
    MenuSecond* new_Mic2();
    MenuSecond* new_In1();
    MenuSecond* new_In2();
    MenuSecond* new_In3();
    MenuSecond* new_In4();
    MenuSecond* new_Aux();
    MenuSecond* new_PGM();
    MenuSecond* new_Monitor();


    MenuSecond* new_Still_Selection();
    MenuSecond* new_Still_Upload();

    MenuSecond* new_Macro_Sleep();

    MenuSecond* new_Stream1();
    MenuSecond* new_Stream2();
    MenuSecond* new_Stream3();

    MenuSecond* new_Playback_Set();

    MenuSecond* new_Setting_Src_Name();
    MenuSecond* new_Setting_Mv_Meter();
    MenuSecond* new_Setting_Mv_Layout();
    MenuSecond* new_Setting_Market();
    MenuSecond* new_Setting_Mic_Input();
    MenuSecond* new_Setting_Record_File();
    MenuSecond* new_Setting_Aux_Source();
    MenuSecond* new_Setting_Version();
    MenuSecond* new_Setting_Out_Format();
    MenuSecond* new_Setting_Out_Source();
    MenuSecond* new_Setting_Quality();
    MenuSecond* new_Setting_Network();
    MenuSecond* new_Setting_Panel();
    MenuSecond* new_Setting_Language();
    MenuSecond* new_Setting_Reset();

    Q_PROPERTY(int pgmCurrentIndex READ pgmCurrentIndex WRITE setpgmCurrentIndex NOTIFY pgmCurrentIndexChanged)
    //
    Q_PROPERTY(int pgmTally READ pgmTally WRITE setPgmTally NOTIFY pgmTallyChanged)


    Q_PROPERTY(int audioOnFlag READ audioOnFlag WRITE setAudioOnFlag NOTIFY audioOnFlagChanged)
    Q_PROPERTY(int audioAfvFlag READ audioAfvFlag WRITE setAudioAfvFlag NOTIFY audioAfvFlagChanged)
    Q_PROPERTY(QList<QString> audioNumbers READ audioNumbers WRITE setAudioNumbers NOTIFY audioNumbersChanged)

    //really aux source
    Q_PROPERTY(int reallyAuxSourceIndex READ reallyAuxSourceIndex WRITE setReallyAuxSourceIndex NOTIFY reallyAuxSourceIndexChanged)
    Q_PROPERTY(int reallySrcSelectionIn1 READ reallySrcSelectionIn1 WRITE setReallySrcSelectionIn1 NOTIFY reallySrcSelectionIn1Changed)
    Q_PROPERTY(int reallySrcSelectionIn2 READ reallySrcSelectionIn2 WRITE setReallySrcSelectionIn2 NOTIFY reallySrcSelectionIn2Changed)
    Q_PROPERTY(int reallySrcSelectionIn3 READ reallySrcSelectionIn3 WRITE setReallySrcSelectionIn3 NOTIFY reallySrcSelectionIn3Changed)
    Q_PROPERTY(int reallySrcSelectionIn4 READ reallySrcSelectionIn4 WRITE setReallySrcSelectionIn4 NOTIFY reallySrcSelectionIn4Changed)

    //really out format
    Q_PROPERTY(int reallyOutFormat READ reallyOutFormat WRITE setReallyOutFormat NOTIFY reallyOutFormatChanged)

    //really out source hdmi1
    Q_PROPERTY(int reallyOutSourceHDMI1 READ reallyOutSourceHDMI1 WRITE setReallyOutSourceHDMI1 NOTIFY reallyOutSourceHDMI1Changed)
    Q_PROPERTY(int reallyOutSourceHDMI2 READ reallyOutSourceHDMI2 WRITE setReallyOutSourceHDMI2 NOTIFY reallyOutSourceHDMI2Changed)
    Q_PROPERTY(int reallyOutSourceUVC READ reallyOutSourceUVC WRITE setReallyOutSourceUVC NOTIFY reallyOutSourceUVCChanged)

    Q_PROPERTY(int reallyOutput1ColorSpace READ reallyOutput1ColorSpace WRITE setReallyOutput1ColorSpace NOTIFY reallyOutput1ColorSpaceChanged)
    Q_PROPERTY(int reallyOutput2ColorSpace READ reallyOutput2ColorSpace WRITE setReallyOutput2ColorSpace NOTIFY reallyOutput2ColorSpaceChanged)

    //color
    //color 1
    Q_PROPERTY(QList<int> color1Data READ color1Data WRITE setColor1Data NOTIFY color1DataChanged)
    //color2
    Q_PROPERTY(QList<int> color2Data READ color2Data WRITE setColor2Data NOTIFY color2DataChanged)
    //super source mask1
    Q_PROPERTY(QList<int> color3Data READ color3Data WRITE setColor3Data NOTIFY color3DataChanged)
    //pip border color
    Q_PROPERTY(QList<int> color4Data READ color4Data WRITE setColor4Data NOTIFY color4DataChanged)
    //chroma key color
    Q_PROPERTY(QList<int> color5Data READ color5Data WRITE setColor5Data NOTIFY color5DataChanged)
    Q_PROPERTY(QList<int> chromakeySMPColorYcbcr READ chromakeySMPColorYcbcr WRITE setChromakeySMPColorYcbcr NOTIFY chromakeySMPColorYcbcrChanged)
    //super source mask2
    Q_PROPERTY(QList<int> color6Data READ color6Data WRITE setColor6Data NOTIFY color6DataChanged)


    //
    Q_PROPERTY(int loadStillProgress READ loadStillProgress WRITE setLoadStillProgress NOTIFY loadStillProgressChanged)

    //record file name
    Q_PROPERTY(QString recordFileName READ recordFileName WRITE setRecordFileName NOTIFY recordFileNameChanged)

    //key event
    Q_PROPERTY(int keyIsPressed READ keyIsPressed WRITE setKeyIsPressed NOTIFY keyIsPressedChanged)


    //menu set
    Q_PROPERTY(int lastFirstUnfold READ lastFirstUnfold WRITE setLastFirstUnfold NOTIFY lastFirstUnfoldChanged)
    Q_PROPERTY(int lastSecondUnfold READ lastSecondUnfold WRITE setLastSecondUnfold NOTIFY lastSecondUnfoldChanged)
    Q_PROPERTY(int lastThirdUnfold READ lastThirdUnfold WRITE setLastThirdUnfold NOTIFY lastThirdUnfoldChanged)

    Q_PROPERTY(int menuVisible READ menuVisible WRITE setMenuVisible NOTIFY menuVisibleChanged)
    Q_PROPERTY(int menuSize READ menuSize WRITE setMenuSize NOTIFY menuSizeChanged)

    Q_PROPERTY(int audioVisible READ audioVisible WRITE setAudioVisible NOTIFY audioVisibleChanged)


    Q_PROPERTY(int leftListViewCurrent READ leftListViewCurrent WRITE setLeftListViewCurrent NOTIFY leftListViewCurrentChanged)
    Q_PROPERTY(int rightListViewCurrent READ rightListViewCurrent WRITE setRightListViewCurrent NOTIFY rightListViewCurrentChanged)

    Q_PROPERTY(int audioMixerSelectedIndex READ audioMixerSelectedIndex WRITE setAudioMixerSelectedIndex NOTIFY audioMixerSelectedIndexChanged)

    //still images
    Q_PROPERTY(QList<QString> stillImages READ stillImages WRITE setStillImages NOTIFY stillImagesChanged)


    Q_PROPERTY(int transitionWipePatternIndex READ transitionWipePatternIndex WRITE setTransitionWipePatternIndex NOTIFY transitionWipePatternIndexChanged)
    Q_PROPERTY(int keyPatternWipePatternIndex READ keyPatternWipePatternIndex WRITE setKeyPatternWipePatternIndex NOTIFY keyPatternWipePatternIndexChanged)

    //keyboard
    Q_PROPERTY(int keyboardReg READ keyboardReg WRITE setKeyboardReg NOTIFY keyboardRegChanged)
    Q_PROPERTY(int keyboardVisible READ keyboardVisible WRITE setKeyboardVisible NOTIFY keyboardVisibleChanged)
    Q_PROPERTY(int keyboardCurrentIndex READ keyboardCurrentIndex WRITE setKeyboardCurrentIndex NOTIFY keyboardCurrentIndexChanged)
    Q_PROPERTY(int keyboardKeyNumberMax READ keyboardKeyNumberMax WRITE setKeyboardKeyNumberMax NOTIFY keyboardKeyNumberMaxChanged)
    Q_PROPERTY(int keyboardKetMoveFlag READ keyboardKetMoveFlag WRITE setKeyboardKetMoveFlag NOTIFY keyboardKetMoveFlagChanged)
    Q_PROPERTY(int keyboardType READ keyboardType WRITE setKeyboardType NOTIFY keyboardTypeChanged)

    //player led
    Q_PROPERTY(int playLedStatus READ playLedStatus WRITE setPlayLedStatus NOTIFY playLedStatusChanged)
    Q_PROPERTY(int recordLedStatus READ recordLedStatus WRITE setRecordLedStatus NOTIFY recordLedStatusChanged)
    Q_PROPERTY(int liveLedStatus READ liveLedStatus WRITE setLiveLedStatus NOTIFY liveLedStatusChanged)
    Q_PROPERTY(int liveStreamStatus1 READ liveStreamStatus1 WRITE setLiveStreamStatus1 NOTIFY liveStreamStatus1Changed)
    Q_PROPERTY(int liveStreamStatus2 READ liveStreamStatus2 WRITE setLiveStreamStatus2 NOTIFY liveStreamStatus2Changed)
    Q_PROPERTY(int liveStreamStatus3 READ liveStreamStatus3 WRITE setLiveStreamStatus3 NOTIFY liveStreamStatus3Changed)
    Q_PROPERTY(int playListCurrent READ playListCurrent WRITE setPlayListCurrent NOTIFY playListCurrentChanged)
    Q_PROPERTY(QList<QString> playList READ playList WRITE setPlayList NOTIFY playListChanged)

    Q_PROPERTY(int playAutoNextFlag READ playAutoNextFlag WRITE setPlayAutoNextFlag NOTIFY playAutoNextFlagChanged)


    //record time  live time
    Q_PROPERTY(int recordSecond READ recordSecond WRITE setRecordSecond NOTIFY recordSecondChanged)
    Q_PROPERTY(QString recordTimeStr READ recordTimeStr WRITE setRecordTimeStr NOTIFY recordTimeStrChanged)
    Q_PROPERTY(int liveSecond READ liveSecond WRITE setLiveSecond NOTIFY liveSecondChanged)
    Q_PROPERTY(QString liveTimeStr READ liveTimeStr WRITE setLiveTimeStr NOTIFY liveTimeStrChanged)


    Q_PROPERTY(int liveStatus READ liveStatus WRITE setLiveStatus NOTIFY liveStatusChanged)


    //stream enable list  ->  audio show
    Q_PROPERTY(QList<bool> streamOutputList READ streamOutputList WRITE setStreamOutputList NOTIFY streamOutputListChanged)

    //playlist dialog visible
    Q_PROPERTY(int playListDialogVisible READ playListDialogVisible WRITE setPlayListDialogVisible NOTIFY playListDialogVisibleChanged)
    Q_PROPERTY(int playListDialogCurrent READ playListDialogCurrent WRITE setPlayListDialogCurrent NOTIFY playListDialogCurrentChanged)


    Q_PROPERTY(int listDialogVisible READ listDialogVisible WRITE setListDialogVisible NOTIFY listDialogVisibleChanged)
    Q_PROPERTY(int ndiListDialogVisible READ ndiListDialogVisible WRITE setNdiListDialogVisible NOTIFY ndiListDialogVisibleChanged)
    Q_PROPERTY(int ndiListDialogCurrent READ ndiListDialogCurrent WRITE setNdiListDialogCurrent NOTIFY ndiListDialogCurrentChanged)


    StreamData streamData;
    std::map<QString,QList<STREAM_PROFILE>> recordData;
    QString getStreamUrlIndex(int index);
    STREAM_PROFILE getStreamBitrateIndex(QString quality);
    STREAM_PROFILE getRecordBitrate(QString quality);
    QString platformChangedText(int index,int changeNumber);
    QString serverChangedText(int index,int changeNumber);

    void initStreamData();
    void initRecordData();


    int lastFirstUnfold() const
    {
        return m_lastFirstUnfold;
    }

    int lastSecondUnfold() const
    {
        return m_lastSecondUnfold;
    }

    int menuVisible() const
    {
        return m_menuVisible;
    }

    int menuSize() const
    {
        return m_menuSize;
    }

    int lastThirdUnfold() const
    {
        return m_lastThirdUnfold;
    }

    int leftListViewCurrent() const
    {
        return m_leftListViewCurrent;
    }

    int rightListViewCurrent() const
    {
        return m_rightListViewCurrent;
    }

public slots:
    void setLastFirstUnfold(int lastFirstUnfold)
    {
        if (m_lastFirstUnfold == lastFirstUnfold)
            return;

        m_lastFirstUnfold = lastFirstUnfold;
        emit lastFirstUnfoldChanged(m_lastFirstUnfold);
    }

    void setLastSecondUnfold(int lastSecondUnfold)
    {
        if (m_lastSecondUnfold == lastSecondUnfold)
            return;

        m_lastSecondUnfold = lastSecondUnfold;
        emit lastSecondUnfoldChanged(m_lastSecondUnfold);
    }

    void setMenuVisible(int menuVisible)
    {
//        if (m_menuVisible == menuVisible)
//            return;

        m_menuVisible = menuVisible;
        emit menuVisibleChanged(m_menuVisible);
    }

    void setMenuSize(int menuSize)
    {
//        if (m_menuSize == menuSize)
//            return;

        m_menuSize = menuSize;
        emit menuSizeChanged(m_menuSize);
    }

    void setLastThirdUnfold(int lastThirdUnfold)
    {
        if (m_lastThirdUnfold == lastThirdUnfold)
            return;

        m_lastThirdUnfold = lastThirdUnfold;
        emit lastThirdUnfoldChanged(m_lastThirdUnfold);
    }

    void setLeftListViewCurrent(int leftListViewCurrent)
    {
        if (m_leftListViewCurrent == leftListViewCurrent)
            return;

        m_leftListViewCurrent = leftListViewCurrent;
        emit leftListViewCurrentChanged(m_leftListViewCurrent);
    }

    void setRightListViewCurrent(int rightListViewCurrent)
    {
        if (m_rightListViewCurrent == rightListViewCurrent)
            return;

        m_rightListViewCurrent = rightListViewCurrent;
        emit rightListViewCurrentChanged(m_rightListViewCurrent);
    }

    void setAudioMixerSelectedIndex(int audioMixerSelectedIndex)
    {
        if (m_audioMixerSelectedIndex == audioMixerSelectedIndex)
            return;

        m_audioMixerSelectedIndex = audioMixerSelectedIndex;
        emit audioMixerSelectedIndexChanged(m_audioMixerSelectedIndex);
    }

    void deviceid_change(QString sn);

signals:
    void lastFirstUnfoldChanged(int lastFirstUnfold);

    void lastSecondUnfoldChanged(int lastSecondUnfold);

    void nameChanged(int lastThirdUnfold);

    void menuVisibleChanged(int menuVisible);

    void menuSizeChanged(int menuSize);

    void lastThirdUnfoldChanged(int lastThirdUnfold);

    void leftListViewCurrentChanged(int leftListViewCurrent);

    void rightListViewCurrentChanged(int rightListViewCurrent);

private:
    int m_lastFirstUnfold;
    int m_lastSecondUnfold;
    int m_lastThirdUnfold;
    int m_menuVisible;
    int m_menuSize;
    int m_leftListViewCurrent;
    int m_rightListViewCurrent;

    QList<MenuFirst *>m_listFirst;
    std::map<int,KeyAbstractClass*>m_listKey;

    int m_audioMixerSelectedIndex;

    //still images
    QList<QString>m_stillImages;

    int m_transitionWipePatternIndex;

    int m_keyPatternWipePatternIndex;

    int m_audioVisible;

    int m_keyboardVisible;

    int m_keyboardCurrentIndex;

    int m_keyboardKeyNumberMax;

    int m_keyboardKetMoveFlag;

    int m_keyIsPressed;

    int m_playLedStatus;

    int m_recordLedStatus;

int m_liveLedStatus;

int m_playListCurrent;

QList<QString> m_playList;

int m_playListChanged;

QString m_recordTimeStr;

int m_liveSecond;

QString m_liveTimeStr;

int m_recordSecond;

int m_loadStillProgress;

QString m_recordFileName;

QList<int> m_color1Data;

QList<int> m_color2Data;

int m_reallyAuxSourceIndex;

int m_audioOnFlag;

int m_audioAfvFlag;

QList<QString> m_audioNumbers;

QList<int> m_color3Data;

QList<int> m_color4Data;

int m_reallyOutSourceHDMI1;

int m_reallyOutSourceHDMI2;

int m_reallyOutSourceUVC;

int m_keyboardReg;

int m_liveStreamStatus1;

int m_liveStreamStatus2;

int m_liveStreamStatus3;

int m_pgmCurrentIndex;

int m_liveStatus;

QList<bool> m_streamOutputList;

QList<int> m_color5Data;

int m_playListDialogVisible;

int m_playListDialogCurrent;

int m_playAutoNextFlag;

int m_reallyOutFormat;

QList<int> m_color6Data;

QList<int> m_chromakeySMPColorYcbcr;

int m_pgmTally;

int m_reallyOutput1ColorSpace;

int m_reallyOutput2ColorSpace;

int m_reallySrcSelectionIn1;

int m_reallySrcSelectionIn2;

int m_reallySrcSelectionIn3;

int m_reallySrcSelectionIn4;

int m_listDialogVisible;

int m_ndiListDialogVisible;

int m_ndiListDialogCurrent;

int m_keyboardType;

public:
    //qsettings
    QVariant getIniValue(int first,int second,int third);
    void setIniValue(int first,int second,int third);
    void setIniValue(QString filepath,QString key,QVariant value);
    QVariant getIniValue(QString fileName,QString first,QString second);
    QVariant getIniValue(QString filepath,QString key);
public:
    Q_INVOKABLE QList<MenuFirst *> listFirst(){
        return  m_listFirst;
    }
    Q_INVOKABLE std::map<int,KeyAbstractClass *> listKey(){
        return  m_listKey;
    }

    Q_INVOKABLE void setMenuDHCPNetwork(QString ip,QString mask,QString gateway,QString primary_dns,QString seconday_dns);
    Q_INVOKABLE void setMenuDHCPNetworkIpAddress(QString ip);
    Q_INVOKABLE void setMenuDHCPNetworkMask(QString mask);
    Q_INVOKABLE void setMenuDHCPNetworkGateway(QString gateway);
    Q_INVOKABLE void setMenuDHCPNetworkPrimaryDNS(QString dns);
    Q_INVOKABLE void setMenuDHCPNetworksecondayDNS(QString dns);

    Q_INVOKABLE void setMenuValue(int first,int second,int third,QVariant value);
    Q_INVOKABLE QVariant getMenuValue(int first,int second,int third);
    Q_INVOKABLE void addMenuValue();
    Q_INVOKABLE void subMenuValue();
    Q_INVOKABLE void addMenuValue(int first,int second,int third);
    Q_INVOKABLE void subMenuValue(int first,int second,int third);

    int audioMixerSelectedIndex() const
    {
        return m_audioMixerSelectedIndex;
    }

    QList<QString> stillImages() const
    {
        return m_stillImages;
    }

    int transitionWipePatternIndex() const
    {
        return m_transitionWipePatternIndex;
    }

    int keyPatternWipePatternIndex() const
    {
        return m_keyPatternWipePatternIndex;
    }

    int audioVisible() const
    {
        return m_audioVisible;
    }

    int keyboardVisible() const
    {
        return m_keyboardVisible;
    }

    int keyboardCurrentIndex() const
    {
        return m_keyboardCurrentIndex;
    }

    int keyboardKeyNumberMax() const
    {
        return m_keyboardKeyNumberMax;
    }

    int keyboardKetMoveFlag() const
    {
        return m_keyboardKetMoveFlag;
    }

    int keyIsPressed() const
    {
        return m_keyIsPressed;
    }

    int playLedStatus() const
    {
        return m_playLedStatus;
    }

    int recordLedStatus() const
    {
        return m_recordLedStatus;
    }

    int liveLedStatus() const
    {
        return m_liveLedStatus;
    }

    int playListCurrent() const
    {
        return m_playListCurrent;
    }

    QList<QString> playList() const
    {
        return m_playList;
    }

    int recordSecond() const
    {
        return m_recordSecond;
    }

    QString recordTimeStr() const
    {
        return m_recordTimeStr;
    }

    int liveSecond() const
    {
        return m_liveSecond;
    }

    QString liveTimeStr() const
    {
        return m_liveTimeStr;
    }

    int loadStillProgress() const
    {
        return m_loadStillProgress;
    }

    QString recordFileName() const
    {
        return m_recordFileName;
    }

    QList<int> color1Data() const
    {
        return m_color1Data;
    }

    QList<int> color2Data() const
    {
        return m_color2Data;
    }

    int reallyAuxSourceIndex() const
    {
        return m_reallyAuxSourceIndex;
    }

    int audioOnFlag() const
    {
        return m_audioOnFlag;
    }

    int audioAfvFlag() const
    {
        return m_audioAfvFlag;
    }

    QList<QString> audioNumbers() const
    {
        return m_audioNumbers;
    }

    QList<int> color3Data() const
    {
        return m_color3Data;
    }

    QList<int> color4Data() const
    {
        return m_color4Data;
    }

    int reallyOutSourceHDMI1() const
    {
        return m_reallyOutSourceHDMI1;
    }

    int reallyOutSourceHDMI2() const
    {
        return m_reallyOutSourceHDMI2;
    }

    int reallyOutSourceUVC() const
    {
        return m_reallyOutSourceUVC;
    }

    int keyboardReg() const
    {
        return m_keyboardReg;
    }

    int liveStreamStatus1() const
    {
        return m_liveStreamStatus1;
    }

    int liveStreamStatus2() const
    {
        return m_liveStreamStatus2;
    }

    int liveStreamStatus3() const
    {
        return m_liveStreamStatus3;
    }

    int pgmCurrentIndex() const
    {
        return m_pgmCurrentIndex;
    }

    int liveStatus() const
    {
        return m_liveStatus;
    }

    QList<bool> streamOutputList() const
    {
        return m_streamOutputList;
    }

    QList<int> color5Data() const
    {
        return m_color5Data;
    }

    int playListDialogVisible() const
    {
        return m_playListDialogVisible;
    }

    int playListDialogCurrent() const
    {
        return m_playListDialogCurrent;
    }

    int playAutoNextFlag() const
    {
        return m_playAutoNextFlag;
    }

    int reallyOutFormat() const
    {
        return m_reallyOutFormat;
    }

    QList<int> color6Data() const
    {
        return m_color6Data;
    }

    QList<int> chromakeySMPColorYcbcr() const
    {
        return m_chromakeySMPColorYcbcr;
    }

    int pgmTally() const
    {
        return m_pgmTally;
    }

    int reallyOutput1ColorSpace() const
    {
        return m_reallyOutput1ColorSpace;
    }

    int reallyOutput2ColorSpace() const
    {
        return m_reallyOutput2ColorSpace;
    }

    int reallySrcSelectionIn1() const
    {
        return m_reallySrcSelectionIn1;
    }

    int reallySrcSelectionIn2() const
    {
        return m_reallySrcSelectionIn2;
    }

    int reallySrcSelectionIn3() const
    {
        return m_reallySrcSelectionIn3;
    }

    int reallySrcSelectionIn4() const
    {
        return m_reallySrcSelectionIn4;
    }

    int listDialogVisible() const
    {
        return m_listDialogVisible;
    }

    int ndiListDialogVisible() const
    {
        return m_ndiListDialogVisible;
    }

    int ndiListDialogCurrent() const
    {
        return m_ndiListDialogCurrent;
    }

    int keyboardType() const
    {
        return m_keyboardType;
    }

signals:
//    void thirdMenuValueChanged();
    void thirdMenuValueChanged(int,int,int);
    void audioMixerSelectedIndexChanged(int audioMixerSelectedIndex);

    void stillImagesChanged(QList<QString> stillImages);

    void transitionWipePatternIndexChanged(int transitionWipePatternIndex);

    void keyPatternWipePatternIndexChanged(int keyPatternWipePatternIndex);

    void audioVisibleChanged(int audioVisible);

    void keyboardVisibleChanged(int keyboardVisible);

    void keyboardCurrentIndexChanged(int keyboardCurrentIndex);

    void keyboardKeyNumberMaxChanged(int keyboardKeyNumberMax);

    void keyboardKetMoveFlagChanged(int keyboardKetMoveFlag);


    //keyboard input text
    void setKeyboardInputText(QString title,QString content);

    void keyIsPressedChanged(int keyIsPressed);

    void playLedStatusChanged(int playLedStatus);

    void recordLedStatusChanged(int recordLedStatus);

    void liveLedStatusChanged(int liveLedStatus);

    void playListCurrentChanged(int playListCurrent);

    void playListChanged(QList<QString> playList);

    void recordSecondChanged(int recordSecond);

    void recordTimeStrChanged(QString recordTimeStr);

    void liveSecondChanged(int liveSecond);

    void liveTimeStrChanged(QString liveTimeStr);

    void loadStillProgressChanged(int loadStillProgress);

    void recordFileNameChanged(QString recordFileName);

    void color1DataChanged(QList<int> color1Data);

    void color2DataChanged(QList<int> color2Data);

    void reallyAuxSourceIndexChanged(int reallyAuxSourceIndex);

    void audioOnFlagChanged(int audioOnFlag);

    void audioAfvFlagChanged(int audioAfvFlag);

    void audioNumbersChanged(QList<QString> audioNumbers);

    void color3DataChanged(QList<int> color3Data);

    void color4DataChanged(QList<int> color4Data);

    void reallyOutSourceHDMI1Changed(int reallyOutSourceHDMI1);

    void reallyOutSourceHDMI2Changed(int reallyOutSourceHDMI2);

    void reallyOutSourceUVCChanged(int reallyOutSourceUVC);

    void keyboardRegChanged(int keyboardReg);

    void liveStreamStatus1Changed(int liveStreamStatus1);

    void liveStreamStatus2Changed(int liveStreamStatus2);

    void liveStreamStatus3Changed(int liveStreamStatus3);

    void pgmCurrentIndexChanged(int pgmCurrentIndex);

    void liveStatusChanged(int liveStatus);

    void streamOutputListChanged(QList<bool> streamOutputList);


    void color5DataChanged(QList<int> color5Data);

    void playListDialogVisibleChanged(int playListDialogVisible);

    void playListDialogCurrentChanged(int playListDialogCurrent);

    void playAutoNextFlagChanged(int playAutoNextFlag);

    void reallyOutFormatChanged(int reallyOutFormat);

    void color6DataChanged(QList<int> color6Data);

    void chromakeySMPColorYcbcrChanged(QList<int> chromakeySMPColorYcbcr);

    void pgmTallyChanged(int pgmTally);

    void reallyOutput1ColorSpaceChanged(int reallyOutput1ColorSpace);

    void reallyOutput2ColorSpaceChanged(int reallyOutput2ColorSpace);

    void reallySrcSelectionIn1Changed(int reallySrcSelectionIn1);

    void reallySrcSelectionIn2Changed(int reallySrcSelectionIn2);

    void reallySrcSelectionIn3Changed(int reallySrcSelectionIn3);

    void reallySrcSelectionIn4Changed(int reallySrcSelectionIn4);

    void listDialogVisibleChanged(int listDialogVisible);

    void ndiListDialogVisibleChanged(int ndiListDialogVisible);

    void ndiListDialogCurrentChanged(int ndiListDialogCurrent);

    void keyboardTypeChanged(int keyboardType);

    void keyboardOk(int first,int second,int third,QString value);

public slots:
void sdImagesChanged(QList<QString> list);
void streamKeyListChanged(QList<QString> list);
void macroListChanged(QList<QString> list);
//弃用
void setStillImages(QList<QString> stillImages)
{
    if (m_stillImages == stillImages)
        return;

    m_stillImages = stillImages;
    emit stillImagesChanged(m_stillImages);
}

void setStillImages(int index,QString text)
{
    if(index >= STILLMAX)
        return ;
    if(m_stillImages[index] == text)
        return ;

    m_stillImages[index] = text;

    setIniValue(DATA_STILL_PATH,"still/"+QString::number(index),text);

    emit stillImagesChanged(m_stillImages);
}
void appendStillImages(QString text)
{
    if(m_stillImages.size() >= STILLMAX)
        return;
    m_stillImages.append(text);
    emit stillImagesChanged(m_stillImages);
}

void setTransitionWipePatternIndex(int transitionWipePatternIndex)
{
    if (m_transitionWipePatternIndex == transitionWipePatternIndex)
        return;

    m_transitionWipePatternIndex = transitionWipePatternIndex;
    emit transitionWipePatternIndexChanged(m_transitionWipePatternIndex);
}
void setKeyPatternWipePatternIndex(int keyPatternWipePatternIndex)
{
    if (m_keyPatternWipePatternIndex == keyPatternWipePatternIndex)
        return;

    m_keyPatternWipePatternIndex = keyPatternWipePatternIndex;
    emit keyPatternWipePatternIndexChanged(m_keyPatternWipePatternIndex);
}
void setAudioVisible(int audioVisible)
{
    if (m_audioVisible == audioVisible)
        return;

    m_audioVisible = audioVisible;
    emit audioVisibleChanged(m_audioVisible);
}
void setKeyboardVisible(int keyboardVisible,int type = KeyboardEvent::KEYBOARD_NORMAL)
{
    if (m_keyboardVisible == keyboardVisible)
        return;
    //打开keyboard前，设置type
    setKeyboardType(type);
    //
    m_keyboardVisible = keyboardVisible;
    emit keyboardVisibleChanged(m_keyboardVisible);
}
void setKeyboardCurrentIndex(int keyboardCurrentIndex)
{
    if (m_keyboardCurrentIndex == keyboardCurrentIndex)
        return;

    if(keyboardCurrentIndex < 0){
        keyboardCurrentIndex = m_keyboardKeyNumberMax - 1;
    }else if(keyboardCurrentIndex >= m_keyboardKeyNumberMax){
        keyboardCurrentIndex = 0;
    }

    m_keyboardCurrentIndex = keyboardCurrentIndex;
    emit keyboardCurrentIndexChanged(m_keyboardCurrentIndex);
}
void setKeyboardKeyNumberMax(int keyboardKeyNumberMax)
{
    if (m_keyboardKeyNumberMax == keyboardKeyNumberMax)
        return;

    m_keyboardKeyNumberMax = keyboardKeyNumberMax;
    emit keyboardKeyNumberMaxChanged(m_keyboardKeyNumberMax);
}
void setKeyboardKetMoveFlag(int keyboardKetMoveFlag)
{
    if (m_keyboardKetMoveFlag == keyboardKetMoveFlag)
        return;

    m_keyboardKetMoveFlag = keyboardKetMoveFlag;
    emit keyboardKetMoveFlagChanged(m_keyboardKetMoveFlag);
}
void setKeyIsPressed(int keyIsPressed)
{
    if (m_keyIsPressed == keyIsPressed)
        return;

    m_keyIsPressed = keyIsPressed;
    emit keyIsPressedChanged(m_keyIsPressed);
}
void setPlayLedStatus(int playLedStatus)
{
//    if (m_playLedStatus == playLedStatus)
//        return;

    m_playLedStatus = playLedStatus;
    emit playLedStatusChanged(m_playLedStatus);
}
void setRecordLedStatus(int recordLedStatus)
{
    if (m_recordLedStatus == recordLedStatus)
        return;

    m_recordLedStatus = recordLedStatus;
    emit recordLedStatusChanged(m_recordLedStatus);
}
void setLiveLedStatus(int liveLedStatus)
{
//    if (m_liveLedStatus == liveLedStatus)
//        return;

    m_liveLedStatus = liveLedStatus;
    emit liveLedStatusChanged(m_liveLedStatus);
}
void setPlayListCurrent(int playListCurrent)
{
    if (m_playListCurrent == playListCurrent)
        return;
    if(playListCurrent >= m_playList.size() && m_playList.size() != 0)
        return ;
//    if(playListCurrent < 0)
//        return ;

    m_playListCurrent = playListCurrent;
    emit playListCurrentChanged(m_playListCurrent);
}
void setPlayList(QList<QString> playList)
{
    if (m_playList == playList)
        return;

    if(playList.size() == 0){
        setPlayListCurrent(0);
    }

    m_playList = playList;
    emit playListChanged(m_playList);
}

bool isPlayListLast(){
    if(playListCurrent() < 0 || playListCurrent() >= playList().size())
        return false;
    if(playList().size() == 0)
        return false;
    if(playListCurrent() == playList().size() - 1)
        return true;
    else
        return false;
}

void setRecordSecond(int recordSecond)
{
    if (m_recordSecond == recordSecond)
        return;

    m_recordSecond = recordSecond;
    emit recordSecondChanged(m_recordSecond);
}
void setRecordTimeStr(QString recordTimeStr)
{
    if (m_recordTimeStr == recordTimeStr)
        return;

    m_recordTimeStr = recordTimeStr;
    emit recordTimeStrChanged(m_recordTimeStr);
}
void setLiveSecond(int liveSecond)
{
    if (m_liveSecond == liveSecond)
        return;

    m_liveSecond = liveSecond;
    emit liveSecondChanged(m_liveSecond);
}
void setLiveTimeStr(QString liveTimeStr)
{
    if (m_liveTimeStr == liveTimeStr)
        return;

    m_liveTimeStr = liveTimeStr;
    emit liveTimeStrChanged(m_liveTimeStr);
}
void setLoadStillProgress(int loadStillProgress)
{
    if (m_loadStillProgress == loadStillProgress)
        return;

    m_loadStillProgress = loadStillProgress;
    emit loadStillProgressChanged(m_loadStillProgress);
}
void setRecordFileName(QString recordFileName)
{
    if (m_recordFileName == recordFileName)
        return;

    m_recordFileName = recordFileName;
    emit recordFileNameChanged(m_recordFileName);
}
void setColor1Data(QList<int> color1Data)
{
    if (m_color1Data == color1Data)
        return;

    m_color1Data = color1Data;
    emit color1DataChanged(m_color1Data);
}
void setColor2Data(QList<int> color2Data)
{
    if (m_color2Data == color2Data)
        return;

    m_color2Data = color2Data;
    emit color2DataChanged(m_color2Data);
}
void setReallyAuxSourceIndex(int reallyAuxSourceIndex)
{
    if (m_reallyAuxSourceIndex == reallyAuxSourceIndex)
        return;

    m_reallyAuxSourceIndex = reallyAuxSourceIndex;
    emit reallyAuxSourceIndexChanged(m_reallyAuxSourceIndex);
}
void setAudioOnFlag(int audioOnFlag)
{
    if (m_audioOnFlag == audioOnFlag)
        return;

    m_audioOnFlag = audioOnFlag;
    emit audioOnFlagChanged(m_audioOnFlag);
}
void setAudioAfvFlag(int audioAfvFlag)
{
    if (m_audioAfvFlag == audioAfvFlag)
        return;

    m_audioAfvFlag = audioAfvFlag;
    emit audioAfvFlagChanged(m_audioAfvFlag);
}
void setAudioNumbers(QList<QString> audioNumbers)
{
    if (m_audioNumbers == audioNumbers)
        return;

    m_audioNumbers = audioNumbers;
    emit audioNumbersChanged(m_audioNumbers);
}

void setAudioNumbers(int index,QString audioNumber)
{
    if(m_audioNumbers.size() == 0)
    {
        QList<QString>list;
        list << "+00.00" << "+00.00" << "+00.00" << "+00.00" << "+00.00" << "+00.00" << "+00.00" << "+00.00";
        setAudioNumbers(list);
    }
    if (m_audioNumbers[index] == audioNumber)
        return;

    m_audioNumbers[index] = audioNumber;
    emit audioNumbersChanged(m_audioNumbers);
}
void setColor3Data(QList<int> color3Data)
{
    if (m_color3Data == color3Data)
        return;

    m_color3Data = color3Data;
    emit color3DataChanged(m_color3Data);
}
void setColor4Data(QList<int> color4Data)
{
    if (m_color4Data == color4Data)
        return;

    m_color4Data = color4Data;
    emit color4DataChanged(m_color4Data);
}
void setReallyOutSourceHDMI1(int reallyOutSourceHDMI1)
{
    if (m_reallyOutSourceHDMI1 == reallyOutSourceHDMI1)
        return;

    m_reallyOutSourceHDMI1 = reallyOutSourceHDMI1;
    emit reallyOutSourceHDMI1Changed(m_reallyOutSourceHDMI1);
}
void setReallyOutSourceHDMI2(int reallyOutSourceHDMI2)
{
    if (m_reallyOutSourceHDMI2 == reallyOutSourceHDMI2)
        return;

    m_reallyOutSourceHDMI2 = reallyOutSourceHDMI2;
    emit reallyOutSourceHDMI2Changed(m_reallyOutSourceHDMI2);
}
void setReallyOutSourceUVC(int reallyOutSourceUVC)
{
    if (m_reallyOutSourceUVC == reallyOutSourceUVC)
        return;

    m_reallyOutSourceUVC = reallyOutSourceUVC;
    emit reallyOutSourceUVCChanged(m_reallyOutSourceUVC);
}
void setKeyboardReg(int keyboardReg)
{
    if (m_keyboardReg == keyboardReg)
        return;

    m_keyboardReg = keyboardReg;
    emit keyboardRegChanged(m_keyboardReg);
}
void setLiveStreamStatus1(int liveStreamStatus1)
{
    if (m_liveStreamStatus1 == liveStreamStatus1)
        return;

    m_liveStreamStatus1 = liveStreamStatus1;
    emit liveStreamStatus1Changed(m_liveStreamStatus1);
}
void setLiveStreamStatus2(int liveStreamStatus2)
{
    if (m_liveStreamStatus2 == liveStreamStatus2)
        return;

    m_liveStreamStatus2 = liveStreamStatus2;
    emit liveStreamStatus2Changed(m_liveStreamStatus2);
}
void setLiveStreamStatus3(int liveStreamStatus3)
{
    if (m_liveStreamStatus3 == liveStreamStatus3)
        return;

    m_liveStreamStatus3 = liveStreamStatus3;
    emit liveStreamStatus3Changed(m_liveStreamStatus3);
}
void setpgmCurrentIndex(int pgmCurrentIndex)
{
    if (m_pgmCurrentIndex == pgmCurrentIndex)
        return;

    printf("________________________m_pgmCurrentIndex:%d\n " ,m_pgmCurrentIndex);

    m_pgmCurrentIndex = pgmCurrentIndex;
    emit pgmCurrentIndexChanged(m_pgmCurrentIndex);
}
void setLiveStatus(int liveStatus)
{
    if (m_liveStatus == liveStatus)
        return;

    m_liveStatus = liveStatus;
    emit liveStatusChanged(m_liveStatus);
}
void setStreamOutputList(QList<bool> streamOutputList)
{
    if (m_streamOutputList == streamOutputList)
        return;

    m_streamOutputList = streamOutputList;
    emit streamOutputListChanged(m_streamOutputList);
}

void setColor5Data(QList<int> color5Data)
{
    if (m_color5Data == color5Data)
        return;
    if(color5Data.size() == 0)
        return ;

    m_color5Data = color5Data;
    emit color5DataChanged(m_color5Data);

//    setIniValue(CHROMAKEY_SMP_COLOR_PATH,"r",m_color5Data[0]);
//    setIniValue(CHROMAKEY_SMP_COLOR_PATH,"g",m_color5Data[1]);
//    setIniValue(CHROMAKEY_SMP_COLOR_PATH,"b",m_color5Data[2]);
}

void saveSMPColorAndColor5ToFile()
{
    setIniValue(CHROMAKEY_SMP_COLOR_PATH,"r",m_color5Data[0]);
    setIniValue(CHROMAKEY_SMP_COLOR_PATH,"g",m_color5Data[1]);
    setIniValue(CHROMAKEY_SMP_COLOR_PATH,"b",m_color5Data[2]);
    setIniValue(CHROMAKEY_SMP_COLOR_PATH,"y",m_chromakeySMPColorYcbcr[0]);
    setIniValue(CHROMAKEY_SMP_COLOR_PATH,"cb",m_chromakeySMPColorYcbcr[1]);
    setIniValue(CHROMAKEY_SMP_COLOR_PATH,"cr",m_chromakeySMPColorYcbcr[2]);
}


void setPlayListDialogVisible(int playListDialogVisible)
{
    if (m_playListDialogVisible == playListDialogVisible)
        return;

    m_playListDialogVisible = playListDialogVisible;
    emit playListDialogVisibleChanged(m_playListDialogVisible);
}
void setPlayListDialogCurrent(int playListDialogCurrent)
{
//    if (m_playListDialogCurrent == playListDialogCurrent)
//        return;

    m_playListDialogCurrent = playListDialogCurrent;
    emit playListDialogCurrentChanged(m_playListDialogCurrent);
}
void setPlayAutoNextFlag(int playAutoNextFlag)
{
    if (m_playAutoNextFlag == playAutoNextFlag)
        return;

    m_playAutoNextFlag = playAutoNextFlag;
    emit playAutoNextFlagChanged(m_playAutoNextFlag);
}
void setReallyOutFormat(int reallyOutFormat)
{
    if (m_reallyOutFormat == reallyOutFormat)
        return;

    m_reallyOutFormat = reallyOutFormat;
    emit reallyOutFormatChanged(m_reallyOutFormat);
}
void setColor6Data(QList<int> color6Data)
{
    if (m_color6Data == color6Data)
        return;

    m_color6Data = color6Data;
    emit color6DataChanged(m_color6Data);
}
void setChromakeySMPColorYcbcr(QList<int> chromakeySMPColorYcbcr)
{
    if (m_chromakeySMPColorYcbcr == chromakeySMPColorYcbcr)
        return;
    if(chromakeySMPColorYcbcr.size() == 0)
        return ;

    m_chromakeySMPColorYcbcr = chromakeySMPColorYcbcr;
    emit chromakeySMPColorYcbcrChanged(m_chromakeySMPColorYcbcr);

//    setIniValue(CHROMAKEY_SMP_COLOR_PATH,"y",m_chromakeySMPColorYcbcr[0]);
//    setIniValue(CHROMAKEY_SMP_COLOR_PATH,"cb",m_chromakeySMPColorYcbcr[1]);
//    setIniValue(CHROMAKEY_SMP_COLOR_PATH,"cr",m_chromakeySMPColorYcbcr[2]);
}
void setPgmTally(int pgmTally)
{
    if (m_pgmTally == pgmTally)
        return;

    m_pgmTally = pgmTally;
    emit pgmTallyChanged(m_pgmTally);
}
void setReallyOutput1ColorSpace(int reallyOutput1ColorSpace)
{
    if (m_reallyOutput1ColorSpace == reallyOutput1ColorSpace)
        return;

    m_reallyOutput1ColorSpace = reallyOutput1ColorSpace;
    emit reallyOutput1ColorSpaceChanged(m_reallyOutput1ColorSpace);
}
void setReallyOutput2ColorSpace(int reallyOutput2ColorSpace)
{
    if (m_reallyOutput2ColorSpace == reallyOutput2ColorSpace)
        return;

    m_reallyOutput2ColorSpace = reallyOutput2ColorSpace;
    emit reallyOutput2ColorSpaceChanged(m_reallyOutput2ColorSpace);
}
void setReallySrcSelectionIn1(int reallySrcSelectionIn1)
{
    if (m_reallySrcSelectionIn1 == reallySrcSelectionIn1)
        return;

    m_reallySrcSelectionIn1 = reallySrcSelectionIn1;
    emit reallySrcSelectionIn1Changed(m_reallySrcSelectionIn1);
}
void setReallySrcSelectionIn2(int reallySrcSelectionIn2)
{
    if (m_reallySrcSelectionIn2 == reallySrcSelectionIn2)
        return;

    m_reallySrcSelectionIn2 = reallySrcSelectionIn2;
    emit reallySrcSelectionIn2Changed(m_reallySrcSelectionIn2);
}
void setReallySrcSelectionIn3(int reallySrcSelectionIn3)
{
    if (m_reallySrcSelectionIn3 == reallySrcSelectionIn3)
        return;

    m_reallySrcSelectionIn3 = reallySrcSelectionIn3;
    emit reallySrcSelectionIn3Changed(m_reallySrcSelectionIn3);
}
void setReallySrcSelectionIn4(int reallySrcSelectionIn4)
{
    if (m_reallySrcSelectionIn4 == reallySrcSelectionIn4)
        return;

    m_reallySrcSelectionIn4 = reallySrcSelectionIn4;
    emit reallySrcSelectionIn4Changed(m_reallySrcSelectionIn4);
}
void setListDialogVisible(int listDialogVisible)
{
    if (m_listDialogVisible == listDialogVisible)
        return;

    m_listDialogVisible = listDialogVisible;
    emit listDialogVisibleChanged(m_listDialogVisible);
}
void setNdiListDialogVisible(int ndiListDialogVisible)
{
    if (m_ndiListDialogVisible == ndiListDialogVisible)
        return;

    m_ndiListDialogVisible = ndiListDialogVisible;
    emit ndiListDialogVisibleChanged(m_ndiListDialogVisible);
}
void setNdiListDialogCurrent(int ndiListDialogCurrent)
{
    if (m_ndiListDialogCurrent == ndiListDialogCurrent)
        return;

    m_ndiListDialogCurrent = ndiListDialogCurrent;
    emit ndiListDialogCurrentChanged(m_ndiListDialogCurrent);
}
void setKeyboardType(int keyboardType)
{
    if (m_keyboardType == keyboardType)
        return;

    m_keyboardType = keyboardType;
    emit keyboardTypeChanged(m_keyboardType);
}



};

#endif // SETTINGS_H
