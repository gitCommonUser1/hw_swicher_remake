#ifndef LUMAPARAMETERS_H
#define LUMAPARAMETERS_H

#include <QObject>
#include "osee_math.h"

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
    Q_PROPERTY(double xPosition READ xPosition WRITE setXPosition NOTIFY xPositionChanged)
    Q_PROPERTY(double yPosition READ yPosition WRITE setYPosition NOTIFY yPositionChanged)

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

    double xPosition() const
    {
        return m_xPosition;
    }

    double yPosition() const
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
        if(fillSource >= m_fillSource_max)
            fillSource = m_fillSource_max - 1;
        if(fillSource < m_fillSource_min)
            fillSource = m_fillSource_min;
        if (m_fillSource == fillSource)
            return;

        m_fillSource = fillSource;
        emit fillSourceChanged(m_fillSource);
    }

    void setKeySource(int keySource)
    {
        if(keySource >= m_keySource_max)
            keySource = m_keySource_max - 1;
        if(keySource < m_keySource_min)
            keySource = m_keySource_min;
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
        if(maskHStart >= m_maskHStart_max)
            maskHStart = m_maskHStart_max ;
        if(maskHStart < m_maskHStart_min)
            maskHStart = m_maskHStart_min;
        if (m_maskHStart == maskHStart)
            return;

        m_maskHStart = maskHStart;
        emit maskHStartChanged(m_maskHStart);
    }

    void setMaskVStart(int maskVStart)
    {
        if(maskVStart >= m_maskVStart_max)
            maskVStart = m_maskVStart_max ;
        if(maskVStart < m_maskVStart_min)
            maskVStart = m_maskVStart_min;
        if (m_maskVStart == maskVStart)
            return;

        m_maskVStart = maskVStart;
        emit maskVStartChanged(m_maskVStart);
    }

    void setMaskHEnd(int maskHEnd)
    {
        if(maskHEnd >= m_maskHEnd_max)
            maskHEnd = m_maskHEnd_max ;
        if(maskHEnd < m_maskHEnd_min)
            maskHEnd = m_maskHEnd_min;
        if (m_maskHEnd == maskHEnd)
            return;

        m_maskHEnd = maskHEnd;
        emit maskHEndChanged(m_maskHEnd);
    }

    void setMaskVEnd(int maskVEnd)
    {
        if(maskVEnd >= m_maskVEnd_max)
            maskVEnd = m_maskVEnd_max ;
        if(maskVEnd < m_maskVEnd_min)
            maskVEnd = m_maskVEnd_min;
        if (m_maskVEnd == maskVEnd)
            return;

        m_maskVEnd = maskVEnd;
        emit maskVEndChanged(m_maskVEnd);
    }

    void setClip(int clip)
    {
        if(clip >= m_clip_max)
            clip = m_clip_max ;
        if(clip < m_clip_min)
            clip = m_clip_min;
        if (m_clip == clip)
            return;

        m_clip = clip;
        emit clipChanged(m_clip);
    }

    void setGain(int gain)
    {
        if(gain >= m_gain_max)
            gain = m_gain_max ;
        if(gain < m_gain_min)
            gain = m_gain_min;
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

    void setXPosition(double xPosition)
    {
        auto dst = dround(xPosition,2);
        if(dst < m_xPosition_min)
            dst = m_xPosition_min;
        if(dst > m_xPosition_max)
            dst = m_xPosition_max;
        if(dst == m_xPosition)
            return ;

        m_xPosition = dst;
        emit xPositionChanged(m_xPosition);
    }

    void setYPosition(double yPosition)
    {
        auto dst = dround(yPosition,2);
        if(dst < m_yPosition_min)
            dst = m_yPosition_min;
        if(dst > m_yPosition_max)
            dst = m_yPosition_max;
        if(dst == m_yPosition)
            return ;

        m_yPosition = dst;
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
    int m_fillSource_min;
    int m_fillSource_max;

    int m_keySource;
    int m_keySource_min;
    int m_keySource_max;

    bool m_maskEnable;

    int m_maskHStart;
    int m_maskHStart_min;
    int m_maskHStart_max;

    int m_maskVStart;
    int m_maskVStart_min;
    int m_maskVStart_max;

    int m_maskHEnd;
    int m_maskHEnd_min;
    int m_maskHEnd_max;

    int m_maskVEnd;
    int m_maskVEnd_min;
    int m_maskVEnd_max;

    int m_clip;
    int m_clip_min;
    int m_clip_max;

    int m_gain;
    int m_gain_min;
    int m_gain_max;

    bool m_invert;

    bool m_resize;

    QString m_size;

    double m_xPosition;
    double m_xPosition_min;
    double m_xPosition_max;


    double m_yPosition;
    double m_yPosition_min;
    double m_yPosition_max;

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
void xPositionChanged(double xPosition);
void yPositionChanged(double yPosition);
void shapedKeyChanged(bool shapedKey);
};

#endif // LUMAPARAMETERS_H
