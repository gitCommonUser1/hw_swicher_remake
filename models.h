#ifndef MODELS_H
#define MODELS_H

#include <QObject>
#include <QMetaMethod>
#include <QDebug>
#include "settings.h"
#include <QQmlApplicationEngine>

extern Settings*settings;

class Models : public QObject
{
    Q_OBJECT
private:
    QQmlApplicationEngine *m_engine;
public:
    explicit Models(QObject *parent = nullptr);
    void init_connect();

    void setEngine(QQmlApplicationEngine *engine){
        m_engine = engine;
    }
    void changeLanguage(int language = -1);
    std::map<QString,QString>languages;

    //key press and release
    void keyClick(int keyIndex);

    //菜单初始化
    void initMenu();
    //菜单索引
    void setMenu(int first = -1,int second = -1,int third = -1);
    //菜单展开
    void menuUnfold(int index);
    //菜单收起
    void menuFold();
    //设置菜单尺寸
    void setMenuSize(int size);//0:1/2   1:2/2
    //设置umd显示、隐藏（主要针对HDMI4）
    void setUMDVisivle(int visible); //1 show    0 hide
    //设置mvMeter显示、隐藏（主要针对HDMI4）
    void setMvMeterVisivle(int visible); //1 show    0 hide
    //菜单显示、隐藏
    void setMenuVisible(int visible);
    //菜单滚动
    void menuKnob(int value);

    //刷新右侧菜单
    void flushRightMenu();
    //修改右侧菜单
    void changeRightMenu(int first,int second,int third);

    //进入右侧菜单
    void enterRightMenu();
    //退出右侧菜单
    void quitRightMenu();
    //右侧选中
    void chooseRightMenu();
    //
    void cancelRightMenu();

    //reset factory
    void resetFactory();


    //音频菜单选择切换
    void menuAudioSelectChanged(int index);
    void menuAudioEnableChnaged(int second,int third);
    void closeAllAudioLed();
    //音量控制
//    void setMonitorLevel(int value);
//    void setAudioFader(int value);

    void initDHCPNetworkData();


    //fgm tally
    int getPGMTally();

    //still generate
    void setStillSelection();
    void setStillUpload();
    void initStillUpload();
    void initStillUpLoadRvSwitch();

    Q_INVOKABLE void loadStill1();
    Q_INVOKABLE void loadStill2();
    Q_INVOKABLE void loadStill3();
    Q_INVOKABLE void loadStill4();

    //player
    int playStart();
    void playPause(int index);
    void playStop();
    void setPlay(int status);
    void recordStart();
    void recordStop();
    void setRecord(int status);
    void setPlayNext();
    void setPlayPrevious();
    void autoPlayNext();

    //aux source
    void setAuxSource();
    void setSrcSelection(int third);

    void setMvMeter();
    void setMvLayout();
    void setMarket();

    void setMicInput(int third);

    int getOutFormatIndexForEnum(int index);
    int getOutFormat(int index);
    void setOutFormat();
    void setOutSource();
    void setColorSpace(int third);

    void setQuality(int third);

    void setProtocol();
    void setNetwork(int third);


    //sleep
    void setMSleep(int value);
    void setUserWait();

    void setPlatform(int second);
    void setStreamKey(int second);
    void setLoadStreamKey(int second);
    void setLiveStatus(int status);
    void openAllStream();
    void closeAllStream();
    void setStreamOutput();

    void setSrcName(int third);

    //playback
    void setPlaybackMode();

    void setButtonBrightness();



    //sd
    int sd_remaintime_calc();
    void sd_format();

    //macro import export
    void macroImport();
    void macroExport();

signals:
    void loadStillImage(int flag,QString name);
    void stillImageFileNameChanged(QString name);
    void sendKey(int key);



    //color
    /**/
    void colorBackHue(int colorIndex,int hue);
    void colorBackSaturation(int colorIndex,int saturation);
    void colorBackBrightness(int colorIndex,int brightness);
    /**/

    //super source
    /**/
    void superSourceEnable(bool enable);
    void superSourceSource1(int source1);
    void superSourceSource2(int source2);
    void superSourceBackground(int background);
    void superSourceControlStyle(QString style);
    void superSourceControlYPosition(int YPosition);
    void superSourceMaskEnable(int maskIndex,bool enable);
    void superSourceMaskHStart(int maskIndex,int HStart);
    void superSourceMaskVStart(int maskIndex,int VStart);
    void superSourceMaskHEnd(int maskIndex,int HEnd);
    void superSourceMaskVEnd(int maskIndex,int VEnd);
    void superSourceBorderWidth(int borderIndex,int width);
    void superSourceBorderColorHue(int borderIndex,int hue);
    void superSourceBorderColorSaturation(int borderIndex,int saturation);
    void superSourceBorderColorBrightness(int borderIndex,int brightness);
    /**/

    //key type
    /**/
    void keyType(QString type);
    /**/

    //keys
    /**/
    void keyMaskEnable(int key,int enable);
    void keyMaskHStart(int key,int hStart);
    void keyMaskVStart(int key,int vStart);
    void keyMaskHEnd(int key,int hEnd);
    void keyMaskVEnd(int key,int vEnd);
    void keyResize(int key,bool resize);
    void keySize(int key,QString size);
    void keyXPosition(int key,double xPosition);
    void keyYPosition(int key,double yPosition);
    void keySourceFill(int key,int fill);
    void keySourceKey(int key,int sourceKey);
    void keyClip(int key,int clip);
    void keyGain(int key,int gain);
    void keyShapedKey(int key,bool enable);
    void keyInvert(int key,bool enable);
    /**/
    //luma key
    /**/
    /**/
    //chroma key
    /**/
    void chromaKeySampleXPosition(double xPosition);
    void chromaKeySampleYPosition(double yPosition);
    void chromaKeySample(bool enable);
    void chromaKeySampleColorHue(int hue);
    void chromaKeySampleColorSaturation(int saturation);
    void chromaKeySampleColorBrightness(int brightness);
    void chromaKeyForeground(int foreground);
    void chromaKeyBackground(int background);
    void chromaKeyKeyEdge(int keyEdge);
    /**/
    //key pattern
    /**/
    void keyPatternWipePattern(int patternIndex);
    void keyPatternWipeSize(int size);
    void keyPatternWipeXPosition(double xPosition);
    void keyPatternWipeYPosition(double yPosition);
    void keyPatternWipeSymmetry(int symmetry);
    void keyPatternWipeSoftness(int softness);
    /**/
    //pip
    /**/
    void pipBorderEnable(bool enable);
    void pipBorderWidth(int width);
    void pipBorderColorHue(int hue);
    void pipBorderColorSaturation(int saturation);
    void pipBorderColorBrightness(int brightness);
    /**/

    //transition
    /**/
    void transitionRate(int index,double rate);
    /**/
    /**/
    void transitionDipSource(int source);
    void transitionWipePattern(int pattern);
    void transitionWipeXPosition(double xPosition);
    void transitionWipeYPosition(double yPosition);
    void transitionWipeDirection(bool direction);
    void transitionWipeSymmetry(int symmetry);
    void transitionWipeSoftness(int softness);
    void transitionWipeBorder(int border);
    void transitionWipeFillSource(int fillSource);
    /**/

    //dsk
    /**/
    void dskSourceFill(int fill);
    void dskSourceKey(int key);
    void dskMaskEnable(bool enable);
    void dskMaskHStart(int hStart);
    void dskMaskVStart(int vStart);
    void dskMaskHEnd(int hEnd);
    void dskMaskVEnd(int vEnd);
    void dskShapedKey(bool enable);
    void dskClip(int clip);
    void dskGain(int gain);
    void dskInvert(bool invert);
    void dskRate(double rate);
    /**/

    //ftb
    /**/
    void ftbRate(double rate);
    void ftbAfv(bool afv);
    /**/

    //audio

    /**/
    void audioEnable(QString source,int enable);
    void audioFader(QString source,double fader);
    void audioBalance(QString source,double balance);
    void audioInput(QString source,double input);
    void audioDelay(QString source,int delay);
    void monitorLevel(int level);
    void monitorSource(QString source);
    /**/


    //still generate

    /**/
    void stillSelection(int stillIndex,int index);
//    void stillUpload();
    void stillLocation(int location);
    /**/

    //player
    void record(int status);
    void play(int status);
    void playNext();
    void playPrevious();

    void recordFile();
    void srcSelection(int third);
    void auxSource();

    void mvMeter();
    void mvLayout();
    void market();

    void micInput(int third);

    void outFormat();
    void outSource();
    void colorSpace(int third);

    void quality(int third);

    void protocol();
    void network(int third);

    //sleep
    void mSleep(int value);
    void userWait();

    //stream

    /**/
    void platform(int streamIndex,int platform);
    void streamServer(int streamIndex,int server);
    void streamKey(int streamIndex,QString key);
    void streamOutput(int streamIndex,int output);
    /**/


    void srcName(int third);

    //playback

    /**/
    void playbackMode(int mode);

    void buttonBrightness();




    //button status
    void pgmIndex(int input);
    void pvwIndex(int input);
    void nextTransition(QString source);
    void transitionStyle(QString style);
    void previewTransition(bool preview);
    void ftb();
    void cutTransition();
    void autoTransition();
    void keyOnAir(bool status);
    void dskOnAir(int status);
    void transitionPosition(int value);
    void transitionSource(int source);

    void sendMacroData(std::map<QString,QString>);



public slots:
    // signals  ---  slots

    //color back
    void setColorBackHue(int colorIndex,int hue);
    void setColorBackSaturation(int colorIndex,int saturation);
    void setColorBackBrightness(int colorIndex,int brightness);
    void setColorBack(int colorIndex,int hue,int saturation,int brightness);

    //super source
    void setSuperSourceEnable(bool enable);
    void setSuperSourceSource1(int source1);
    void setSuperSourceSource2(int source2);
    void setSuperSourceBackground(int background);
    void setSuperSourceControlStyle(QString style);
    void setSuperSourceControlYPosition(int YPosition);
    void setSuperSourceMaskEnable(int maskIndex,bool enable);
    void setSuperSourceMaskHStart(int maskIndex,int HStart);
    void setSuperSourceMaskVStart(int maskIndex,int VStart);
    void setSuperSourceMaskHEnd(int maskIndex,int HEnd);
    void setSuperSourceMaskVEnd(int maskIndex,int VEnd);
    void setSuperSourceBorderWidth(int borderIndex,int width);
    void setSuperSourceBorderColorHue(int borderIndex,int hue);
    void setSuperSourceBorderColorSaturation(int borderIndex,int saturation);
    void setSuperSourceBorderColorBrightness(int borderIndex,int brightness);
    void setSuperSourceCtrl();//
    void setSuperSource();//
    void setSuperSourceMask(int maskIndex,int PositionIndex);//
    void setSuperSourceColor(int colorIndex);//

    //key type
    void setKeyType(QString type);

    //keys
    void setKeyMaskEnable(int key,int enable);
    void setKeyMaskHStart(int key,int hStart);
    void setKeyMaskVStart(int key,int vStart);
    void setKeyMaskHEnd(int key,int hEnd);
    void setKeyMaskVEnd(int key,int vEnd);
    void setKeyResize(int key,bool resize);
    void setKeySize(int key,QString size);
    void setKeyXPosition(int key,double xPosition);
    void setKeyYPosition(int key,double yPosition);
    void setKeySourceFill(int key,int fill);
    void setKeySourceKey(int key,int sourceKey);
    void setKeyClip(int key,int clip);
    void setKeyGain(int key,int gain);
    void setKeyShapedKey(int key,bool enable);
    void setKeyInvert(int key,bool enable);
    void setKeyCtrl(int key);//

    //chroma key
    void setChromaKeySampleXPosition(double xPosition);
    void setChromaKeySampleYPosition(double yPosition);
    void setChromaKeySample(bool enable);
    void setChromaKeySampleColorHue(int hue);
    void setChromaKeySampleColorSaturation(int saturation);
    void setChromaKeySampleColorBrightness(int brightness);
    void setChromaKeyForeground(int foreground);
    void setChromaKeyBackground(int background);
    void setChromaKeyKeyEdge(int keyEdge);
    void setChromaKeyProfile();//
    void setChromaKeySampleColor();//

    //key pattern
    void setKeyPatternWipePattern(int patternIndex);
    void setKeyPatternWipeSize(int size);
    void setKeyPatternWipeXPosition(double xPosition);
    void setKeyPatternWipeYPosition(double yPosition);
    void setKeyPatternWipeSymmetry(int symmetry);
    void setKeyPatternWipeSoftness(int softness);
    void setKeyPatternCtrl();
    void setKeyPatternPosition();

    //pip
    void setPipBorderEnable(bool enable);
    void setPipBorderWidth(int width);
    void setPipBorderColorHue(int hue);
    void setPipBorderColorSaturation(int saturation);
    void setPipBorderColorBrightness(int brightness);
    void setPipBorderColor();//


    //transition
    /**/
    void setTransitionRate(int index,double rate);
    /**/

    /**/
    void setTransitionDipSource(int source);
    void setTransitionWipePattern(int pattern);
    void setTransitionWipeXPosition(double xPosition);
    void setTransitionWipeYPosition(double yPosition);
    void setTransitionWipeDirection(bool direction);
    void setTransitionWipeSymmetry(int symmetry);
    void setTransitionWipeSoftness(int softness);
    void setTransitionWipeBorder(int border);
    void setTransitionWipeFillSource(int fillSource);
    void setTransitionWipePosition();//
    /**/

    //dsk
    /**/
    void setDskSourceFill(int fill);
    void setDskSourceKey(int key);
    void setDskMaskEnable(bool enable);
    void setDskMaskHStart(int hStart);
    void setDskMaskVStart(int vStart);
    void setDskMaskHEnd(int hEnd);
    void setDskMaskVEnd(int vEnd);
    void setDskShapedKey(bool enable);
    void setDskClip(int clip);
    void setDskGain(int gain);
    void setDskInvert(bool invert);
    void setDskRate(double rate);
    void setDskCtrl();//
    /**/

    //ftb
    /**/
    void setFtbRate(double rate);
    void setFtbAfv(bool afv);
    /**/

    //audio mixer
    /**/
    void setAudioEnable(QString source,int enable);
    void setAudioFader(QString source,double fader);
    void setAudioBalance(QString source,double balance);
    void setAudioInput(QString source,double input);
    void setAudioDelay(QString source,int delay);
    void setMonitorLevel(int level);
    void setMonitorSource(QString source);
    void setAudioVolumn(int index);//
    void setAudioOn();//
    void setAudioAfv();//
    /**/


    //button status
    void setPgmIndex(int index);
    void setPvwIndex(int index);
    void setNextTransition(QString source);
    void setTransitionStyle(QString style);
    void setFtb();
    void setCutTransition();
    void setAutoTransition();
    void setPreviewTransition(bool preview);
    void setTransitionPosition(int value);
    void setTransitionSource(int source);
    void setKeyOnAir(bool status);
    void setDskOnAir(int status);


public:
    int initStillMutex;

    QString parameterToString(QMetaMethod func,int parameter);
    int parameterToInt(QMetaMethod func,QString parameter);
    int isKeyMehotd(QMetaMethod func);

    //新macro调用方法
    template<typename myFunction,typename... ARGS>
    void macroInvoke(myFunction function,ARGS... tail)
    {
        (this->*function)(tail...);
        //
        QMetaMethod method = QMetaMethod::fromSignal(function);
    }
};

#endif // MODELS_H
