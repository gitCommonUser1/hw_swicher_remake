#ifndef MODELS_H
#define MODELS_H

#include <QObject>
#include <QMetaMethod>
#include <QDebug>
#include "settings.h"
#include <QQmlApplicationEngine>
#include "macrorecorder.h"

extern MacroRecorder *macroRecorder;

extern Settings*settings;

class Models : public QObject
{
    Q_OBJECT
private:
    QQmlApplicationEngine *m_engine;
    std::map<QString,QMetaMethod> m_signals;
public:
    explicit Models(QObject *parent = nullptr);
    void init_connect();
    //为了兼容MACRO，models的信号一定不能有重名（重载）, (好像信号本身无法重载)
    void init_signals();
    const std::map<QString,QMetaMethod>& getSignals(){return m_signals;}

    void setEngine(QQmlApplicationEngine *engine){
        m_engine = engine;
    }
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
    void initStillUpLoadRvSwitch();
    void stillLoadPictureIndexChanged(int value);

    Q_INVOKABLE void loadStill1();
    Q_INVOKABLE void loadStill2();
    Q_INVOKABLE void loadStill3();
    Q_INVOKABLE void loadStill4();

    //player
    int playStart();
    void playPause(int index);
    void playStop();
    void recordStart();
    void recordStop();
    void setPlayNext();
    void setPlayPrevious();

    int getOutFormatIndexForEnum(int index);
    int getOutFormat(int index);

    void setLoadStreamKey(int second);
    void openAllStream();
    void openOneStream(int index);
    void closeAllStream();
    void closeOneStream(int index);
    void streamUploadKeyIndexChanged(int second,int value);

    //playback

    //sd
    int sd_remaintime_calc();
    void sd_format();

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
    void keyOnAir(bool onAir);
    /**/

    //keys
    /**/
    void keyMaskEnable(QString key,int enable);
    void keyMaskHStart(QString key,int hStart);
    void keyMaskVStart(QString key,int vStart);
    void keyMaskHEnd(QString key,int hEnd);
    void keyMaskVEnd(QString key,int vEnd);
    void keyResize(QString key,bool resize);
    void keySize(QString key,QString size);
    void keyXPosition(QString key,double xPosition);
    void keyYPosition(QString key,double yPosition);
    void keySourceFill(QString key,int fill);
    void keySourceKey(QString key,int sourceKey);
    void keyClip(QString key,int clip);
    void keyGain(QString key,int gain);
    void keyShapedKey(QString key,bool enable);
    void keyInvert(QString key,bool enable);
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
    void transitionRate(QString index,double rate);
    /**/
    /**/
    void transitionDipSource(int source);
    void transitionDipStinger(bool stinger);
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
    void dskEnable(bool enable);
    void dskOnAir(bool onAir);
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
    void ftbEnable(bool enable);
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
    void stillLocation(int location);
    /**/

    //macro
    /**/
    //这个地方的参数”value“如果修改了，需要将标记的这几个地方全部修改。
    //标记为：2023.12.20.21.32   直接全局查找
    void mSleep(int value);
    void userWait();
    /**/



    //stream

    /**/
    void streamEnable(bool enable);
    void streamPlatform(int streamIndex,QString platform);
    void streamServer(int streamIndex,QString server);
    void streamUrl(int streamIndex,QString url);
    void streamOutput(int streamIndex,bool output);

    /**/



    //player
    void record(int status);
    void play(int status);
    void playNext();
    void playPrevious();

    void network(int third);

    //playback
    void playbackList(QString list);
    void playbackSequential(bool sequential);
    void playbackProgressBar(bool progressBar);

    /**/



    //setting
    /**/
    void srcName(QString srcName,QString name);
    void mvMeter(QString src,bool enable);
    void mvLayout(QString layout);
    void marker(bool pvwMarker);
    void micInput(QString src,QString input);
    void recordFileName(QString fileName);
    void srcSelection(QString src,QString selection);
    void outFormat(QString outFormat);
    void outputColorSpace(QString src,QString colorSpace);
    void outSource(QString src,QString source);
    void quality(QString src,QString quality);
    void protocol(bool dhcp);
    void iPAddress(QString value);
    void subnetMask(QString value);
    void gateway(QString value);
    void primaryDNS(QString value);
    void seconddaryDNS(QString value);
    void network();
    void buttonBrightness(int value);
    void language(int language);
    /**/



    //button status
    void pgmIndex(QString input);
    void pvwIndex(QString input);
    void nextTransition(QString source);
    void transitionStyle(QString style);
    void previewTransition(bool preview);
    void cutTransition();
    void autoTransition();
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
    void setKeyOnAir(bool onAir);

    //keys
    void setKeyMaskEnable(QString key,int enable);
    void setKeyMaskHStart(QString key,int hStart);
    void setKeyMaskVStart(QString key,int vStart);
    void setKeyMaskHEnd(QString key,int hEnd);
    void setKeyMaskVEnd(QString key,int vEnd);
    void setKeyResize(QString key,bool resize);
    void setKeySize(QString key,QString size);
    void setKeyXPosition(QString key,double xPosition);
    void setKeyYPosition(QString key,double yPosition);
    void setKeySourceFill(QString key,int fill);
    void setKeySourceKey(QString key,int sourceKey);
    void setKeyClip(QString key,int clip);
    void setKeyGain(QString key,int gain);
    void setKeyShapedKey(QString key,bool enable);
    void setKeyInvert(QString key,bool enable);
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
    void setTransitionRate(QString index,double rate);
    /**/

    /**/
    void setTransitionDipSource(int source);
    void setTransitionDipStinger(int stinger);
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
    void setDskOnAir(bool onAir);
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
    void setFtbEnable(bool enable);
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
    void addAudioFaderByAudioKnob(QString source,int value);
    void addMonitorLevelByAudioKnob(int value);
    /**/


    //still generator
    /**/
    void setStillSelection(int still,int selection);
    void setStillLocation(int index);
    void setStillSelectionCtrl();
    /**/


    //macro
    /**/
    void setMSleep(int value);
    void setUserWait();
    /**/


    //stream
    /**/
    void setStreamEnable(bool enable);
    void setStreamPlatform(int streamIndex,QString platform);
    void setStreamServer(int streamIndex,QString server);
    void setStreamUrl(int streamIndex,QString url);
    void setStreamOutput(int streamIndex,bool output);
    void updateServerArray(int streamIndex);
    /**/


    //playback
    /**/
    void setPlaybackList(QString list);
    void setPlaybackSequential(bool sequential);
    void setPlaybackProgressBar(bool progressBar);
    /**/


    //setting
    /**/
    void setSrcName(QString srcName,QString name);
    void setMvMeter(QString src,bool enable);
    void setMvLayout(QString layout);
    void setMarker(bool pvwMarker);
    void setPVWCtrl();//
    void setMicInput(QString src,QString input);
    void setRecordFileName(QString fileName);
    void setSrcSelection(QString src,QString selection);
    void setOutFormat(QString outFormat);
    void setOutputColorSpace(QString src,QString colorSpace);
    void setOutSource(QString src,QString source);
    void setQuality(QString src,QString quality);
    void setProtocol(bool dhcp);
    void setIPAddress(QString value);
    void setSubnetMask(QString value);
    void setGateway(QString value);
    void setPrimaryDNS(QString value);
    void setSeconddaryDNS(QString value);
    void setNetwork();
    void setButtonBrightness(int value);
    void setLanguage(int language);
    /**/


    //button status
    void setPgmIndex(QString index);
    void setPvwIndex(QString index);
    void setNextTransition(QString source);
    void setTransitionStyle(QString style);
    void setCutTransition();
    void setAutoTransition();
    void setPreviewTransition(bool preview);
    void setTransitionPosition(int value);
    void setTransitionSource(int source);


public:
    int initStillMutex;

    //获取所有参数
    void pushArgs(){}
    QVariantList m_args;
    template<typename T,typename... ARGS>
    void pushArgs(T t,ARGS... args)
    {
        m_args.push_back(t);
        pushArgs(args...);
    }
    //新macro调用方法
    template<typename myFunction,typename... ARGS>
    void macroInvoke(myFunction function,ARGS... tail)
    {
        m_args.clear();
        pushArgs(tail...);
        //调用
        (this->*function)(tail...);
        //
        QMetaMethod method = QMetaMethod::fromSignal(function);
        qDebug() << "method name: "<< method.name();
        qDebug() << "pars:" << method.parameterNames();
        QVariantMap item;
        item["id"] = method.name();
        for(int i = 0;i < method.parameterNames().size();++i)
        {
            //
            //item[method.parameterNames()[i]] = tail...
            item[method.parameterNames()[i]] = m_args[i];
        }
        macroRecorder->append(item);
    }
    template<typename myFunction,typename... ARGS>
    void macroRecord(myFunction function,ARGS... tail)
    {
        m_args.clear();
        pushArgs(tail...);

        QMetaMethod method = QMetaMethod::fromSignal(function);
        qDebug() << "method name: "<< method.name();
        qDebug() << "pars:" << method.parameterNames();
        QVariantMap item;
        item["id"] = method.name();
        for(int i = 0;i < method.parameterNames().size();++i)
        {
            //
            //item[method.parameterNames()[i]] = tail...
            item[method.parameterNames()[i]] = m_args[i];
        }
        macroRecorder->append(item);
        //不调用
//        (this->*function)(tail...);
        //
//        QMetaMethod method = QMetaMethod::fromSignal(function);
    }


    template<typename... ARGS>
    void macroInvoke(QMetaMethod method,ARGS... tail)
    {
        method.invoke(this,tail...);
        QVariantMap item;
        item["id"] = method.name();
        for(int i = 0;i < method.parameterNames().size();++i)
        {
            item[method.parameterNames()[i]] = m_args[i];
        }
        macroRecorder->append(item);
    }
};

#endif // MODELS_H
