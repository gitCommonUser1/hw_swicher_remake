#ifndef TRANSITIONSTYLE_H
#define TRANSITIONSTYLE_H

#include <QObject>

class MixParameters;
class DipParameters;
class WipeParameters;

class TransitionStyle : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString style READ style WRITE setStyle NOTIFY styleChanged)
    Q_PROPERTY(QString nextStyle READ nextStyle WRITE setNextStyle NOTIFY nextStyleChanged)
    Q_PROPERTY(bool previewTransition READ previewTransition WRITE setPreviewTransition NOTIFY previewTransitionChanged)
    Q_PROPERTY(MixParameters* mixParameters READ mixParameters WRITE setMixParameters NOTIFY mixParametersChanged)
    Q_PROPERTY(DipParameters* dipParameters READ dipParameters WRITE setDipParameters NOTIFY dipParametersChanged)
    Q_PROPERTY(WipeParameters* wipeParameters READ wipeParameters WRITE setWipeParameters NOTIFY wipeParametersChanged)

public:
    explicit TransitionStyle(QObject *parent = nullptr);

    QString styleIndexToString(int index);
    int styleStringToIndex(QString style);

    enum TRANSITIONSOURCE{
        IN1 = 1001,
        IN2,
        IN3,
        IN4,
        AUX,
        BLACK,
        STILL1,
        STILL2,
        COLOR1,
        COLOR2,
        COLORBAR
    };

    QString style() const
    {
        return m_style;
    }

    QString nextStyle() const
    {
        return m_nextStyle;
    }

    bool previewTransition() const
    {
        return m_previewTransition;
    }

    MixParameters* mixParameters() const
    {
        return m_mixParameters;
    }

    DipParameters* dipParameters() const
    {
        return m_dipParameters;
    }

    WipeParameters* wipeParameters() const
    {
        return m_wipeParameters;
    }

public slots:
    void setStyle(QString style)
    {
        if (m_style == style)
            return;

        m_style = style;
        emit styleChanged(m_style);
    }

    void setNextStyle(QString nextStyle)
    {
        if (m_nextStyle == nextStyle)
            return;

        m_nextStyle = nextStyle;
        emit nextStyleChanged(m_nextStyle);
    }

    void setPreviewTransition(bool previewTransition)
    {
        if (m_previewTransition == previewTransition)
            return;

        m_previewTransition = previewTransition;
        emit previewTransitionChanged(m_previewTransition);
    }

    void setMixParameters(MixParameters* mixParameters)
    {
        if (m_mixParameters == mixParameters)
            return;

        m_mixParameters = mixParameters;
        emit mixParametersChanged(m_mixParameters);
    }

    void setDipParameters(DipParameters* dipParameters)
    {
        if (m_dipParameters == dipParameters)
            return;

        m_dipParameters = dipParameters;
        emit dipParametersChanged(m_dipParameters);
    }

    void setWipeParameters(WipeParameters* wipeParameters)
    {
        if (m_wipeParameters == wipeParameters)
            return;

        m_wipeParameters = wipeParameters;
        emit wipeParametersChanged(m_wipeParameters);
    }

private:

    QString m_style;

    QString m_nextStyle;

    bool m_previewTransition;

    MixParameters* m_mixParameters;

    DipParameters* m_dipParameters;

    WipeParameters* m_wipeParameters;

signals:

void styleChanged(QString style);
void nextStyleChanged(QString nextStyle);
void previewTransitionChanged(bool previewTransition);
void mixParametersChanged(MixParameters* mixParameters);
void dipParametersChanged(DipParameters* dipParameters);
void wipeParametersChanged(WipeParameters* wipeParameters);
};

#endif // TRANSITIONSTYLE_H
