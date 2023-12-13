#ifndef OUTSOURCES_H
#define OUTSOURCES_H

#include <QObject>

class OutSource;

class OutSources : public QObject
{
    Q_OBJECT
    Q_PROPERTY(OutSource* hdmi1 READ hdmi1)
    Q_PROPERTY(OutSource* hdmi2 READ hdmi2)
    Q_PROPERTY(OutSource* uvc READ uvc)
public:
    explicit OutSources(QObject *parent = nullptr);

    static int srcStringToIndex(QString src);
    static QString srcIndexToString(int index);
    static int sourceStringToIndex(QString source);
    static QString sourceIndexToString(int index);

    enum SRCS{
        HDMI1 = 0,
        HDMI2,
        UVC
    };

    enum SOURCES{
        IN1 = 0,
        IN2,
        IN3,
        IN4,
        AUX,
        PGM,
        PVW,
        MULTIVIEW,
        MAX
    };

    OutSource* hdmi1() const
    {
        return m_hdmi1;
    }

    OutSource* hdmi2() const
    {
        return m_hdmi2;
    }

    OutSource* uvc() const
    {
        return m_uvc;
    }

private:


    OutSource* m_hdmi1;

    OutSource* m_hdmi2;

    OutSource* m_uvc;

signals:

};

#endif // OUTSOURCES_H
