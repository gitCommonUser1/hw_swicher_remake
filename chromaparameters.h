#ifndef CHROMAPARAMETERS_H
#define CHROMAPARAMETERS_H

#include <QObject>
#include "osee_math.h"

class ChromaParameters : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int fillSource READ fillSource WRITE setFillSource NOTIFY fillSourceChanged)
    Q_PROPERTY(bool maskEnable READ maskEnable WRITE setMaskEnable NOTIFY maskEnableChanged)
    Q_PROPERTY(int maskHStart READ maskHStart WRITE setMaskHStart NOTIFY maskHStartChanged)
    Q_PROPERTY(int maskVStart READ maskVStart WRITE setMaskVStart NOTIFY maskVStartChanged)
    Q_PROPERTY(int maskHEnd READ maskHEnd WRITE setMaskHEnd NOTIFY maskHEndChanged)
    Q_PROPERTY(int maskVEnd READ maskVEnd WRITE setMaskVEnd NOTIFY maskVEndChanged)
    Q_PROPERTY(bool resize READ resize WRITE setResize NOTIFY resizeChanged)
    Q_PROPERTY(QString size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(double xPosition READ xPosition WRITE setXPosition NOTIFY xPositionChanged)
    Q_PROPERTY(double yPosition READ yPosition WRITE setYPosition NOTIFY yPositionChanged)
    Q_PROPERTY(double smpXPosition READ smpXPosition WRITE setSmpXPosition NOTIFY smpXPositionChanged)
    Q_PROPERTY(double smpYPosition READ smpYPosition WRITE setSmpYPosition NOTIFY smpYPositionChanged)
    Q_PROPERTY(bool sample READ sample WRITE setSample NOTIFY sampleChanged)
    Q_PROPERTY(int y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(int cb READ cb WRITE setCb NOTIFY cbChanged)
    Q_PROPERTY(int cr READ cr WRITE setCr NOTIFY crChanged)
    Q_PROPERTY(int foreground READ foreground WRITE setForeground NOTIFY foregroundChanged)
    Q_PROPERTY(int background READ background WRITE setBackground NOTIFY backgroundChanged)
    Q_PROPERTY(int keyEdge READ keyEdge WRITE setKeyEdge NOTIFY keyEdgeChanged)

public:
    explicit ChromaParameters(QObject *parent = nullptr);

    int fillSource() const
    {
        return m_fillSource;
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

    double smpXPosition() const
    {
        return m_smpXPosition;
    }

    double smpYPosition() const
    {
        return m_smpYPosition;
    }

    bool sample() const
    {
        return m_sample;
    }

    int foreground() const
    {
        return m_foreground;
    }

    int background() const
    {
        return m_background;
    }

    int keyEdge() const
    {
        return m_keyEdge;
    }

    int y() const
    {
        return m_y;
    }

    int cb() const
    {
        return m_cb;
    }

    int cr() const
    {
        return m_cr;
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

    void setMaskEnable(bool maskEnable)
    {
        if (m_maskEnable == maskEnable)
            return;

        m_maskEnable = maskEnable;
        emit maskEnableChanged(m_maskEnable);
    }

    void setMaskHStart(int maskHStart)
    {
        if(maskHStart > m_maskHStart_max)
            maskHStart = m_maskHStart_max;
        if(maskHStart < m_maskHStart_min)
            maskHStart = m_maskHStart_min;
        if (m_maskHStart == maskHStart)
            return;

        m_maskHStart = maskHStart;
        emit maskHStartChanged(m_maskHStart);
    }

    void setMaskVStart(int maskVStart)
    {
        if(maskVStart > m_maskVStart_max)
            maskVStart = m_maskVStart_max;
        if(maskVStart < m_maskVStart_min)
            maskVStart = m_maskVStart_min;
        if (m_maskVStart == maskVStart)
            return;

        m_maskVStart = maskVStart;
        emit maskVStartChanged(m_maskVStart);
    }

    void setMaskHEnd(int maskHEnd)
    {
        if(maskHEnd > m_maskHEnd_max)
            maskHEnd = m_maskHEnd_max;
        if(maskHEnd < m_maskHEnd_min)
            maskHEnd = m_maskHEnd_min;
        if (m_maskHEnd == maskHEnd)
            return;

        m_maskHEnd = maskHEnd;
        emit maskHEndChanged(m_maskHEnd);
    }

    void setMaskVEnd(int maskVEnd)
    {
        if(maskVEnd > m_maskVEnd_max)
            maskVEnd = m_maskVEnd_max;
        if(maskVEnd < m_maskVEnd_min)
            maskVEnd = m_maskVEnd_min;
        if (m_maskVEnd == maskVEnd)
            return;

        m_maskVEnd = maskVEnd;
        emit maskVEndChanged(m_maskVEnd);
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

    void setSmpXPosition(double smpXPosition)
    {
        auto dst = dround(smpXPosition,2);
        if(dst < m_smpXPosition_min)
            dst = m_smpXPosition_min;
        if(dst > m_smpXPosition_max)
            dst = m_smpXPosition_max;
        if(dst == m_smpXPosition)
            return ;

        m_smpXPosition = dst;
        emit smpXPositionChanged(m_smpXPosition);
    }

    void setSmpYPosition(double smpYPosition)
    {
        auto dst = dround(smpYPosition,2);
        if(dst < m_smpYPosition_min)
            dst = m_smpYPosition_min;
        if(dst > m_smpYPosition_max)
            dst = m_smpYPosition_max;
        if(dst == m_smpYPosition)
            return ;

        m_smpYPosition = dst;
        emit smpYPositionChanged(m_smpYPosition);
    }

    void setSample(bool sample)
    {
        if (m_sample == sample)
            return;

        m_sample = sample;
        emit sampleChanged(m_sample);
    }

    void setForeground(int foreground)
    {
        if(foreground > m_foreground_max)
            foreground = m_foreground_max;
        if(foreground < m_foreground_min)
            foreground = m_foreground_min;
        if (foreground == m_foreground)
            return;

        m_foreground = foreground;
        emit foregroundChanged(m_foreground);
    }

    void setBackground(int background)
    {
        if(background > m_background_max)
            background = m_background_max;
        if(background < m_background_min)
            background = m_background_min;
        if (background == m_background)
            return;

        m_background = background;
        emit backgroundChanged(m_background);
    }

    void setKeyEdge(int keyEdge)
    {
        if(keyEdge > m_keyEdge_max)
            keyEdge = m_keyEdge_max;
        if(keyEdge < m_keyEdge_min)
            keyEdge = m_keyEdge_min;
        if (keyEdge == m_keyEdge)
            return;
        m_keyEdge = keyEdge;
        emit keyEdgeChanged(m_keyEdge);
    }

    void setY(int y)
    {
        if (m_y == y)
            return;

        m_y = y;
        emit yChanged(m_y);
    }

    void setCb(int cb)
    {
        if (m_cb == cb)
            return;

        m_cb = cb;
        emit cbChanged(m_cb);
    }

    void setCr(int cr)
    {
        if (m_cr == cr)
            return;

        m_cr = cr;
        emit crChanged(m_cr);
    }

private:


    int m_fillSource;
    int m_fillSource_min;
    int m_fillSource_max;

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

    bool m_resize;

    QString m_size;

    double m_xPosition;
    double m_xPosition_min;
    double m_xPosition_max;

    double m_yPosition;
    double m_yPosition_min;
    double m_yPosition_max;

    double m_smpXPosition;
    double m_smpXPosition_min;
    double m_smpXPosition_max;

    double m_smpYPosition;
    double m_smpYPosition_min;
    double m_smpYPosition_max;

    bool m_sample;

    int m_foreground;
    int m_foreground_min;
    int m_foreground_max;

    int m_background;
    int m_background_min;
    int m_background_max;

    int m_keyEdge;
    int m_keyEdge_min;
    int m_keyEdge_max;

    int m_y;

    int m_cb;

    int m_cr;

signals:

void fillSourceChanged(int fillSource);
void maskEnableChanged(bool maskEnable);
void maskHStartChanged(int maskHStart);
void maskVStartChanged(int maskVStart);
void maskHEndChanged(int maskHEnd);
void maskVEndChanged(int maskVEnd);
void resizeChanged(bool resize);
void sizeChanged(QString size);
void xPositionChanged(double xPosition);
void yPositionChanged(double yPosition);
void smpXPositionChanged(double smpXPosition);
void smpYPositionChanged(double smpYPosition);
void sampleChanged(bool sample);
void foregroundChanged(int foreground);
void backgroundChanged(int background);
void keyEdgeChanged(int keyEdge);
void yChanged(int y);
void cbChanged(int cb);
void crChanged(int cr);
};

#endif // CHROMAPARAMETERS_H
