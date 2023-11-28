#ifndef DOWNSTREAMKEY_H
#define DOWNSTREAMKEY_H

#include <QObject>

class DownstreamKey : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int  index READ index WRITE setIndex NOTIFY indexChanged)
    Q_PROPERTY(int fillSource READ fillSource WRITE setFillSource NOTIFY fillSourceChanged)
    Q_PROPERTY(int keySource READ keySource WRITE setKeySource NOTIFY keySourceChanged)
    Q_PROPERTY(bool maskEnable READ maskEnable WRITE setMaskEnable NOTIFY maskEnableChanged)
    Q_PROPERTY(int maskHStart READ maskHStart WRITE setMaskHStart NOTIFY maskHStartChanged)
    Q_PROPERTY(int maskVStart READ maskVStart WRITE setMaskVStart NOTIFY maskVStartChanged)
    Q_PROPERTY(int maskHEnd READ maskHEnd WRITE setMaskHEnd NOTIFY maskHEndChanged)
    Q_PROPERTY(int maskVEnd READ maskVEnd WRITE setMaskVEnd NOTIFY maskVEndChanged)
    Q_PROPERTY(bool shapedKey READ shapedKey WRITE setShapedKey NOTIFY shapedKeyChanged)
    Q_PROPERTY(int clip READ clip WRITE setClip NOTIFY clipChanged)
    Q_PROPERTY(int gain READ gain WRITE setGain NOTIFY gainChanged)
    Q_PROPERTY(bool invert READ invert WRITE setInvert NOTIFY invertChanged)
    Q_PROPERTY(float rate READ rate WRITE setRate NOTIFY rateChanged)

public:
    explicit DownstreamKey(QObject *parent = nullptr);

    int index() const
    {
        return m_index;
    }

    int fillSource() const
    {
        return m_fillSource;
    }

    int keySource() const
    {
        return m_keySource;
    }

    bool maskEnable() const
    {
        return m_maskEnable;
    }

    int maskHStart() const
    {
        return m_maskHStart;
    }

    int maskVStart() const
    {
        return m_maskVStart;
    }

    int maskHEnd() const
    {
        return m_maskHEnd;
    }

    int maskVEnd() const
    {
        return m_maskVEnd;
    }

    int clip() const
    {
        return m_clip;
    }

    int gain() const
    {
        return m_gain;
    }

    bool invert() const
    {
        return m_invert;
    }

    bool shapedKey() const
    {
        return m_shapedKey;
    }

    float rate() const
    {
        return m_rate;
    }

public slots:
    void setIndex(int index)
    {
        if (m_index == index)
            return;

        m_index = index;
        emit indexChanged(m_index);
    }

    void setFillSource(int fillSource)
    {
        if (m_fillSource == fillSource)
            return;

        m_fillSource = fillSource;
        emit fillSourceChanged(m_fillSource);
    }

    void setKeySource(int keySource)
    {
        if (m_keySource == keySource)
            return;

        m_keySource = keySource;
        emit keySourceChanged(m_keySource);
    }

    void setMaskEnable(bool maskEnable)
    {
        if (m_maskEnable == maskEnable)
            return;

        m_maskEnable = maskEnable;
        emit maskEnableChanged(m_maskEnable);
    }

    void setMaskHStart(int maskHStart)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (m_maskHStart == maskHStart)
            return;

        m_maskHStart = maskHStart;
        emit maskHStartChanged(m_maskHStart);
    }

    void setMaskVStart(int maskVStart)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (m_maskVStart == maskVStart)
            return;

        m_maskVStart = maskVStart;
        emit maskVStartChanged(m_maskVStart);
    }

    void setMaskHEnd(int maskHEnd)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (m_maskHEnd == maskHEnd)
            return;

        m_maskHEnd = maskHEnd;
        emit maskHEndChanged(m_maskHEnd);
    }

    void setMaskVEnd(int maskVEnd)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (m_maskVEnd == maskVEnd)
            return;

        m_maskVEnd = maskVEnd;
        emit maskVEndChanged(m_maskVEnd);
    }

    void setClip(int clip)
    {
        if (m_clip == clip)
            return;

        m_clip = clip;
        emit clipChanged(m_clip);
    }

    void setGain(int gain)
    {
        if (m_gain == gain)
            return;

        m_gain = gain;
        emit gainChanged(m_gain);
    }

    void setInvert(bool invert)
    {
        if (m_invert == invert)
            return;

        m_invert = invert;
        emit invertChanged(m_invert);
    }

    void setShapedKey(bool shapedKey)
    {
        if (m_shapedKey == shapedKey)
            return;

        m_shapedKey = shapedKey;
        emit shapedKeyChanged(m_shapedKey);
    }

    void setRate(float rate)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_rate, rate))
            return;

        m_rate = rate;
        emit rateChanged(m_rate);
    }

private:
    int m_index;

    int m_fillSource;

    int m_keySource;

    bool m_maskEnable;

    int m_maskHStart;

    int m_maskVStart;

    int m_maskHEnd;

    int m_maskVEnd;

    int m_clip;

    int m_gain;

    bool m_invert;

    bool m_shapedKey;

    float m_rate;

signals:

void indexChanged(int index);
void fillSourceChanged(int fillSource);
void keySourceChanged(int keySource);
void maskEnableChanged(bool maskEnable);
void maskHStartChanged(int maskHStart);
void maskVStartChanged(int maskVStart);
void maskHEndChanged(int maskHEnd);
void maskVEndChanged(int maskVEnd);
void clipChanged(int clip);
void gainChanged(int gain);
void invertChanged(bool invert);
void shapedKeyChanged(bool shapedKey);
void rateChanged(float rate);
};

#endif // DOWNSTREAMKEY_H
