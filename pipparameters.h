#ifndef PIPPARAMETERS_H
#define PIPPARAMETERS_H

#include <QObject>

class PIPParameters : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int fillSource READ fillSource WRITE setFillSource NOTIFY fillSourceChanged)
    Q_PROPERTY(QString size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(float xPosition READ xPosition WRITE setXPosition NOTIFY xPositionChanged)
    Q_PROPERTY(float yPosition READ yPosition WRITE setYPosition NOTIFY yPositionChanged)
    Q_PROPERTY(bool maskEnable READ maskEnable WRITE setMaskEnable NOTIFY maskEnableChanged)
    Q_PROPERTY(int maskHStart READ maskHStart WRITE setMaskHStart NOTIFY maskHStartChanged)
    Q_PROPERTY(int maskVStart READ maskVStart WRITE setMaskVStart NOTIFY maskVStartChanged)
    Q_PROPERTY(int maskHEnd READ maskHEnd WRITE setMaskHEnd NOTIFY maskHEndChanged)
    Q_PROPERTY(int maskVEnd READ maskVEnd WRITE setMaskVEnd NOTIFY maskVEndChanged)
    Q_PROPERTY(bool borderEnable READ borderEnable WRITE setBorderEnable NOTIFY borderEnableChanged)
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged)
    Q_PROPERTY(int borderColorHue READ borderColorHue WRITE setBorderColorHue NOTIFY borderColorHueChanged)
    Q_PROPERTY(int borderColorSaturation READ borderColorSaturation WRITE setBorderColorSaturation NOTIFY borderColorSaturationChanged)
    Q_PROPERTY(int borderColorBrightness READ borderColorBrightness WRITE setBorderColorBrightness NOTIFY borderColorBrightnessChanged)

public:
    explicit PIPParameters(QObject *parent = nullptr);

    int fillSource() const
    {
        return m_fillSource;
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

    bool borderEnable() const
    {
        return m_borderEnable;
    }

    int borderWidth() const
    {
        return m_borderWidth;
    }

    int borderColorHue() const
    {
        return m_borderColorHue;
    }

    int borderColorSaturation() const
    {
        return m_borderColorSaturation;
    }

    int borderColorBrightness() const
    {
        return m_borderColorBrightness;
    }

public slots:
    void setFillSource(int fillSource)
    {
        if (m_fillSource == fillSource)
            return;

        m_fillSource = fillSource;
        emit fillSourceChanged(m_fillSource);
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

    void setBorderEnable(bool borderEnable)
    {
        if (m_borderEnable == borderEnable)
            return;

        m_borderEnable = borderEnable;
        emit borderEnableChanged(m_borderEnable);
    }

    void setBorderWidth(int borderWidth)
    {
        if (m_borderWidth == borderWidth)
            return;

        m_borderWidth = borderWidth;
        emit borderWidthChanged(m_borderWidth);
    }

    void setBorderColorHue(int borderColorHue)
    {
        if (m_borderColorHue == borderColorHue)
            return;

        m_borderColorHue = borderColorHue;
        emit borderColorHueChanged(m_borderColorHue);
    }

    void setBorderColorSaturation(int borderColorSaturation)
    {
        if (m_borderColorSaturation == borderColorSaturation)
            return;

        m_borderColorSaturation = borderColorSaturation;
        emit borderColorSaturationChanged(m_borderColorSaturation);
    }

    void setBorderColorBrightness(int borderColorBrightness)
    {
        if (m_borderColorBrightness == borderColorBrightness)
            return;

        m_borderColorBrightness = borderColorBrightness;
        emit borderColorBrightnessChanged(m_borderColorBrightness);
    }

private:

    int m_fillSource;

    QString m_size;

    float m_xPosition;

    float m_yPosition;

    bool m_maskEnable;

    int m_maskHStart;

    int m_maskVStart;

    int m_maskHEnd;

    int m_maskVEnd;

    bool m_borderEnable;

    int m_borderWidth;

    int m_borderColorHue;

    int m_borderColorSaturation;

    int m_borderColorBrightness;

signals:

void fillSourceChanged(int fillSource);
void sizeChanged(QString size);
void xPositionChanged(float xPosition);
void yPositionChanged(float yPosition);
void maskEnableChanged(bool maskEnable);
void maskHStartChanged(int maskHStart);
void maskVStartChanged(int maskVStart);
void maskHEndChanged(int maskHEnd);
void maskVEndChanged(int maskVEnd);
void borderEnableChanged(bool borderEnable);
void borderWidthChanged(int borderWidth);
void borderColorHueChanged(int borderColorHue);
void borderColorSaturationChanged(int borderColorSaturation);
void borderColorBrightnessChanged(int borderColorBrightness);
};

#endif // PIPPARAMETERS_H
