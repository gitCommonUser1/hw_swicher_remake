#ifndef LUMAPARAMETERS_H
#define LUMAPARAMETERS_H

#include <QObject>

class LumaParameters : public QObject
{
    Q_OBJECT
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
    Q_PROPERTY(bool resize READ resize WRITE setResize NOTIFY resizeChanged)
    Q_PROPERTY(QString size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(float xPosition READ xPosition WRITE setXPosition NOTIFY xPositionChanged)
    Q_PROPERTY(float yPosition READ yPosition WRITE setYPosition NOTIFY yPositionChanged)

public:
    explicit LumaParameters(QObject *parent = nullptr);

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

    bool resize() const
    {
        return m_resize;
    }

    QString size() const
    {
        return m_size;
    }

    float xPosition() const
    {
        return m_xPosition;
    }

    float yPosition() const
    {
        return m_yPosition;
    }

    bool shapedKey() const
    {
        return m_shapedKey;
    }

public slots:
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
        if (m_maskHStart == maskHStart)
            return;

        m_maskHStart = maskHStart;
        emit maskHStartChanged(m_maskHStart);
    }

    void setMaskVStart(int maskVStart)
    {
        if (m_maskVStart == maskVStart)
            return;

        m_maskVStart = maskVStart;
        emit maskVStartChanged(m_maskVStart);
    }

    void setMaskHEnd(int maskHEnd)
    {
        if (m_maskHEnd == maskHEnd)
            return;

        m_maskHEnd = maskHEnd;
        emit maskHEndChanged(m_maskHEnd);
    }

    void setMaskVEnd(int maskVEnd)
    {
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

    void setResize(bool resize)
    {
        if (m_resize == resize)
            return;

        m_resize = resize;
        emit resizeChanged(m_resize);
    }

    void setSize(QString size)
    {
        if (m_size == size)
            return;

        m_size = size;
        emit sizeChanged(m_size);
    }

    void setXPosition(float xPosition)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_xPosition, xPosition))
            return;

        m_xPosition = xPosition;
        emit xPositionChanged(m_xPosition);
    }

    void setYPosition(float yPosition)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_yPosition, yPosition))
            return;

        m_yPosition = yPosition;
        emit yPositionChanged(m_yPosition);
    }

    void setShapedKey(bool shapedKey)
    {
        if (m_shapedKey == shapedKey)
            return;

        m_shapedKey = shapedKey;
        emit shapedKeyChanged(m_shapedKey);
    }

private:

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

    bool m_resize;

    QString m_size;

    float m_xPosition;

    float m_yPosition;

    bool m_shapedKey;

signals:

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
void resizeChanged(bool resize);
void sizeChanged(QString size);
void xPositionChanged(float xPosition);
void yPositionChanged(float yPosition);
void shapedKeyChanged(bool shapedKey);
};

#endif // LUMAPARAMETERS_H
