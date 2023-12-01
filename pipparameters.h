#ifndef PIPPARAMETERS_H
#define PIPPARAMETERS_H

#include <QObject>
#include "osee_math.h"

class PIPParameters : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int fillSource READ fillSource WRITE setFillSource NOTIFY fillSourceChanged)
    Q_PROPERTY(QString size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(double xPosition READ xPosition WRITE setXPosition NOTIFY xPositionChanged)
    Q_PROPERTY(double yPosition READ yPosition WRITE setYPosition NOTIFY yPositionChanged)
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

    double xPosition() const
    {
        return m_xPosition;
    }

    double yPosition() const
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
        if(fillSource >= m_fillSource_max)
            fillSource = m_fillSource_max - 1;
        if(fillSource < m_fillSource_min)
            fillSource = m_fillSource_min;
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

    void setBorderEnable(bool borderEnable)
    {
        if (m_borderEnable == borderEnable)
            return;

        m_borderEnable = borderEnable;
        emit borderEnableChanged(m_borderEnable);
    }

    void setBorderWidth(int borderWidth)
    {
        if(borderWidth > m_borderWidth_max)
            borderWidth = m_borderWidth_max;
        if(borderWidth < m_borderWidth_min)
            borderWidth = m_borderWidth_min;
        if (m_borderWidth == borderWidth)
            return;

        m_borderWidth = borderWidth;
        emit borderWidthChanged(m_borderWidth);
    }

    void setBorderColorHue(int borderColorHue)
    {
        if(borderColorHue > m_borderColorHue_max)
            borderColorHue = m_borderColorHue_max;
        if(borderColorHue < m_borderColorHue_min)
            borderColorHue = m_borderColorHue_min;
        if (m_borderColorHue == borderColorHue)
            return;

        m_borderColorHue = borderColorHue;
        emit borderColorHueChanged(m_borderColorHue);
    }

    void setBorderColorSaturation(int borderColorSaturation)
    {
        if(borderColorSaturation > m_borderColorSaturation_max)
            borderColorSaturation = m_borderColorSaturation_max;
        if(borderColorSaturation < m_borderColorSaturation_min)
            borderColorSaturation = m_borderColorSaturation_min;
        if (m_borderColorSaturation == borderColorSaturation)
            return;

        m_borderColorSaturation = borderColorSaturation;
        emit borderColorSaturationChanged(m_borderColorSaturation);
    }

    void setBorderColorBrightness(int borderColorBrightness)
    {
        if(borderColorBrightness > m_borderColorBrightness_max)
            borderColorBrightness = m_borderColorBrightness_max;
        if(borderColorBrightness < m_borderColorBrightness_min)
            borderColorBrightness = m_borderColorBrightness_min;
        if (m_borderColorBrightness == borderColorBrightness)
            return;

        m_borderColorBrightness = borderColorBrightness;
        emit borderColorBrightnessChanged(m_borderColorBrightness);
    }

private:

    int m_fillSource;
    int m_fillSource_min;
    int m_fillSource_max;

    QString m_size;

    double m_xPosition;
    double m_xPosition_min;
    double m_xPosition_max;

    double m_yPosition;
    double m_yPosition_min;
    double m_yPosition_max;

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

    bool m_borderEnable;

    int m_borderWidth;
    int m_borderWidth_min;
    int m_borderWidth_max;

    int m_borderColorHue;
    int m_borderColorHue_min;
    int m_borderColorHue_max;

    int m_borderColorSaturation;
    int m_borderColorSaturation_min;
    int m_borderColorSaturation_max;

    int m_borderColorBrightness;
    int m_borderColorBrightness_min;
    int m_borderColorBrightness_max;

signals:

void fillSourceChanged(int fillSource);
void sizeChanged(QString size);
void xPositionChanged(double xPosition);
void yPositionChanged(double yPosition);
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
