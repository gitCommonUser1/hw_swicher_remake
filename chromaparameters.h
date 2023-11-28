#ifndef CHROMAPARAMETERS_H
#define CHROMAPARAMETERS_H

#include <QObject>

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
    Q_PROPERTY(float xPosition READ xPosition WRITE setXPosition NOTIFY xPositionChanged)
    Q_PROPERTY(float yPosition READ yPosition WRITE setYPosition NOTIFY yPositionChanged)
    Q_PROPERTY(float smpXPosition READ smpXPosition WRITE setSmpXPosition NOTIFY smpXPositionChanged)
    Q_PROPERTY(float smpYPosition READ smpYPosition WRITE setSmpYPosition NOTIFY smpYPositionChanged)
    Q_PROPERTY(bool sample READ sample WRITE setSample NOTIFY sampleChanged)
    Q_PROPERTY(int red READ red WRITE setRed NOTIFY redChanged)
    Q_PROPERTY(int green READ green WRITE setGreen NOTIFY greenChanged)
    Q_PROPERTY(int blue READ blue WRITE setBlue NOTIFY blueChanged)
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

    float xPosition() const
    {
        return m_xPosition;
    }

    float yPosition() const
    {
        return m_yPosition;
    }

    float smpXPosition() const
    {
        return m_smpXPosition;
    }

    float smpYPosition() const
    {
        return m_smpYPosition;
    }

    bool sample() const
    {
        return m_sample;
    }

    int red() const
    {
        return m_red;
    }

    int green() const
    {
        return m_green;
    }

    int blue() const
    {
        return m_blue;
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

public slots:
    void setFillSource(int fillSource)
    {
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

    void setSmpXPosition(float smpXPosition)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_smpXPosition, smpXPosition))
            return;

        m_smpXPosition = smpXPosition;
        emit smpXPositionChanged(m_smpXPosition);
    }

    void setSmpYPosition(float smpYPosition)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_smpYPosition, smpYPosition))
            return;

        m_smpYPosition = smpYPosition;
        emit smpYPositionChanged(m_smpYPosition);
    }

    void setSample(bool sample)
    {
        if (m_sample == sample)
            return;

        m_sample = sample;
        emit sampleChanged(m_sample);
    }

    void setRed(int red)
    {
        if (m_red == red)
            return;

        m_red = red;
        emit redChanged(m_red);
    }

    void setGreen(int green)
    {
        if (m_green == green)
            return;

        m_green = green;
        emit greenChanged(m_green);
    }

    void setBlue(int blue)
    {
        if (m_blue == blue)
            return;

        m_blue = blue;
        emit blueChanged(m_blue);
    }

    void setForeground(int foreground)
    {
        if (m_foreground == foreground)
            return;

        m_foreground = foreground;
        emit foregroundChanged(m_foreground);
    }

    void setBackground(int background)
    {
        if (m_background == background)
            return;

        m_background = background;
        emit backgroundChanged(m_background);
    }

    void setKeyEdge(int keyEdge)
    {
        if (m_keyEdge == keyEdge)
            return;

        m_keyEdge = keyEdge;
        emit keyEdgeChanged(m_keyEdge);
    }

private:


    int m_fillSource;

    bool m_maskEnable;

    int m_maskHStart;

    int m_maskVStart;

    int m_maskHEnd;

    int m_maskVEnd;

    bool m_resize;

    QString m_size;

    float m_xPosition;

    float m_yPosition;

    float m_smpXPosition;

    float m_smpYPosition;

    bool m_sample;

    int m_red;

    int m_green;

    int m_blue;

    int m_foreground;

    int m_background;

    int m_keyEdge;

signals:

void fillSourceChanged(int fillSource);
void maskEnableChanged(bool maskEnable);
void maskHStartChanged(int maskHStart);
void maskVStartChanged(int maskVStart);
void maskHEndChanged(int maskHEnd);
void maskVEndChanged(int maskVEnd);
void resizeChanged(bool resize);
void sizeChanged(QString size);
void xPositionChanged(float xPosition);
void yPositionChanged(float yPosition);
void smpXPositionChanged(float smpXPosition);
void smpYPositionChanged(float smpYPosition);
void sampleChanged(bool sample);
void redChanged(int red);
void greenChanged(int green);
void blueChanged(int blue);
void foregroundChanged(int foreground);
void backgroundChanged(int background);
void keyEdgeChanged(int keyEdge);
};

#endif // CHROMAPARAMETERS_H
