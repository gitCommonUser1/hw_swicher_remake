#ifndef TESTQML_H
#define TESTQML_H

#include <QObject>

class TestQml : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(bool ethernetFlag READ ethernetFlag WRITE setEthernetFlag NOTIFY ethernetFlagChanged)
    Q_PROPERTY(bool usbOutFlag READ usbOutFlag WRITE setUsbOutFlag NOTIFY usbOutFlagChanged)
    Q_PROPERTY(bool usbInFlag READ usbInFlag WRITE setUsbInFlag NOTIFY usbInFlagChanged)
    Q_PROPERTY(bool rtcFlag READ rtcFlag WRITE setRtcFlag NOTIFY rtcFlagChanged)
    int m_value;
    QObject *qml_object;
    std::map<int,QObject*>m_objects;


    bool audio_min;
    bool audio_max;
    bool audio_pressed;
    bool menu_min;
    bool menu_max;
    bool menu_pressed;

    bool m_ethernetFlag;

    bool m_usbOutFlag;

    bool m_usbInFlag;

    bool m_rtcFlag;

public:
    explicit TestQml(QObject *parent = nullptr);
    ~TestQml();
    void init1();
    void init2();

int value() const
{
    return m_value;
}

void setQmlObject(QObject *obj){
    qml_object = obj;
}

void initMap();

bool ethernetFlag() const
{
    return m_ethernetFlag;
}

bool usbOutFlag() const
{
    return m_usbOutFlag;
}

bool usbInFlag() const
{
    return m_usbInFlag;
}

bool rtcFlag() const
{
    return m_rtcFlag;
}

public slots:
void setValue(int value)
{
    if (m_value == value)
        return;

    m_value = value;
    emit valueChanged(m_value);
}

void keyChanged(int key,int value);
void knobChanged(int key,int value);
void pushChanged(int key,int value);

void setEthernetFlag(bool ethernetFlag)
{
    if (m_ethernetFlag == ethernetFlag)
        return;

    m_ethernetFlag = ethernetFlag;
    emit ethernetFlagChanged(m_ethernetFlag);
}

void setUsbOutFlag(bool usbOutFlag)
{
    if (m_usbOutFlag == usbOutFlag)
        return;

    m_usbOutFlag = usbOutFlag;
    emit usbOutFlagChanged(m_usbOutFlag);
}

void setUsbInFlag(bool usbInFlag)
{
    if (m_usbInFlag == usbInFlag)
        return;

    m_usbInFlag = usbInFlag;
    emit usbInFlagChanged(m_usbInFlag);
}

void setRtcFlag(bool rtcFlag)
{
    if (m_rtcFlag == rtcFlag)
        return;

    m_rtcFlag = rtcFlag;
    emit rtcFlagChanged(m_rtcFlag);
}

signals:

void valueChanged(int value);
void ethernetFlagChanged(bool ethernetFlag);
void usbOutFlagChanged(bool usbOutFlag);
void usbInFlagChanged(bool usbInFlag);
void rtcFlagChanged(bool rtcFlag);
};

#endif // TESTQML_H
