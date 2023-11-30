#ifndef PATTERNPARAMETERS_H
#define PATTERNPARAMETERS_H

#include <QObject>
#include "osee_math.h"

class PatternParameters : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int fillSource READ fillSource WRITE setFillSource NOTIFY fillSourceChanged)
    Q_PROPERTY(int pattern READ pattern WRITE setPattern NOTIFY patternChanged)
    Q_PROPERTY(int wipeSize READ wipeSize WRITE setWipeSize NOTIFY wipeSizeChanged)
    Q_PROPERTY(double wipeXPosition READ wipeXPosition WRITE setWipeXPosition NOTIFY wipeXPositionChanged)
    Q_PROPERTY(double wipeYPosition READ wipeYPosition WRITE setWipeYPosition NOTIFY wipeYPositionChanged)
    Q_PROPERTY(int wipeSymmetry READ wipeSymmetry WRITE setWipeSymmetry NOTIFY wipeSymmetryChanged)
    Q_PROPERTY(int wipeSoftness READ wipeSoftness WRITE setWipeSoftness NOTIFY wipeSoftnessChanged)
    Q_PROPERTY(bool maskEnable READ maskEnable WRITE setMaskEnable NOTIFY maskEnableChanged)
    Q_PROPERTY(int maskHStart READ maskHStart WRITE setMaskHStart NOTIFY maskHStartChanged)
    Q_PROPERTY(int maskVStart READ maskVStart WRITE setMaskVStart NOTIFY maskVStartChanged)
    Q_PROPERTY(int maskHEnd READ maskHEnd WRITE setMaskHEnd NOTIFY maskHEndChanged)
    Q_PROPERTY(int maskVEnd READ maskVEnd WRITE setMaskVEnd NOTIFY maskVEndChanged)
    Q_PROPERTY(bool resize READ resize WRITE setResize NOTIFY resizeChanged)
    Q_PROPERTY(QString size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(double xPosition READ xPosition WRITE setXPosition NOTIFY xPositionChanged)
    Q_PROPERTY(double yPosition READ yPosition WRITE setYPosition NOTIFY yPositionChanged)
public:
    explicit PatternParameters(QObject *parent = nullptr);

    enum PATTERNTYPE{
        LEFT_TO_RIGHT = 0,
        TOP_TO_BOTTOM,
        VERTICAL_CENTER,
        HORIZONTAL_CENTER,
        CROSS_CENTER,
        SQUARE_CENTER,
        DIAMOND,
        CIRCLE,
        SQUARE_TOP_LEFT,
        SQUARE_TOP_RIGHT,
        SQUARE_BOTTOM_RIGHT,
        SQUARE_BOTTOM_LEFT,
        SQUARE_TOP_CENTER,
        SQUARE_LEFT_CENTER,
        SQUARE_BOTTOM_CENTER,
        SQUARE_RIGHT_CENTER,
        BOTTOM_RIGHT_ANGLE,
        BOTTOM_LEFT_ANGLE,
        PATTERN_MAX
    };

    int fillSource() const
    {
        return m_fillSource;
    }

    int pattern() const
    {
        return m_pattern;
    }

    int wipeSize() const
    {
        return m_wipeSize;
    }

    double wipeXPosition() const
    {
        return m_wipeXPosition;
    }

    double wipeYPosition() const
    {
        return m_wipeYPosition;
    }

    int wipeSymmetry() const
    {
        return m_wipeSymmetry;
    }

    int wipeSoftness() const
    {
        return m_wipeSoftness;
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

    double wipeXPositionMax(){return m_xPosition_max;}
    double wipeXPositionMin(){return m_xPosition_min;}
    double wipeYPositionMax(){return m_yPosition_max;}
    double wipeYPositionMin(){return m_yPosition_min;}
    double SymmetryMax(){return m_wipeSymmetry_max;}
    double SymmetryMin(){return m_wipeSymmetry_min;}
    double softnessMax(){return m_wipeSoftness_max;}
    double softnessMin(){return m_wipeSoftness_min;}

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

    void setPattern(int pattern)
    {
        if(pattern >= m_pattern_max)
            pattern = m_pattern_max - 1;
        if(pattern < m_pattern_min)
            pattern = m_pattern_min;
        if (m_pattern == pattern)
            return;

        m_pattern = pattern;
        emit patternChanged(m_pattern);
    }

    void setWipeSize(int wipeSize)
    {
        if(wipeSize > m_wipeSize_max)
            wipeSize = m_wipeSize_max;
        if(wipeSize < m_wipeSize_min)
            wipeSize = m_wipeSize_min;
        if (m_wipeSize == wipeSize)
            return;

        m_wipeSize = wipeSize;
        emit wipeSizeChanged(m_wipeSize);
    }

    void setWipeXPosition(double wipeXPosition)
    {
        auto dst = dround(wipeXPosition,2);
        if(dst < m_wipeXPosition_min)
            dst = m_wipeXPosition_min;
        if(dst > m_wipeXPosition_max)
            dst = m_wipeXPosition_max;
        if(dst == m_wipeXPosition)
            return ;

        m_wipeXPosition = dst;
        emit wipeXPositionChanged(m_wipeXPosition);
    }

    void setWipeYPosition(double wipeYPosition)
    {
        auto dst = dround(wipeYPosition,2);
        if(dst < m_wipeYPosition_min)
            dst = m_wipeYPosition_min;
        if(dst > m_wipeYPosition_max)
            dst = m_wipeYPosition_max;
        if(dst == m_wipeYPosition)
            return ;

        m_wipeYPosition = dst;
        emit wipeYPositionChanged(m_wipeYPosition);
    }

    void setWipeSymmetry(int wipeSymmetry)
    {
        if(wipeSymmetry > m_wipeSymmetry_max)
            wipeSymmetry = m_wipeSymmetry_max;
        if(wipeSymmetry < m_wipeSymmetry_min)
            wipeSymmetry = m_wipeSymmetry_min;
        if (m_wipeSymmetry == wipeSymmetry)
            return;

        m_wipeSymmetry = wipeSymmetry;
        emit wipeSymmetryChanged(m_wipeSymmetry);
    }

    void setWipeSoftness(int wipeSoftness)
    {
        if(wipeSoftness > m_wipeSoftness_max)
            wipeSoftness = m_wipeSoftness_max;
        if(wipeSoftness < m_wipeSoftness_min)
            wipeSoftness = m_wipeSoftness_min;
        if (m_wipeSoftness == wipeSoftness)
            return;

        m_wipeSoftness = wipeSoftness;
        emit wipeSoftnessChanged(m_wipeSoftness);
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

private:

    int m_fillSource;
    int m_fillSource_min;
    int m_fillSource_max;

    int m_pattern;
    int m_pattern_min;
    int m_pattern_max;

    int m_wipeSize;
    int m_wipeSize_min;
    int m_wipeSize_max;

    double m_wipeXPosition;
    double m_wipeXPosition_min;
    double m_wipeXPosition_max;

    double m_wipeYPosition;
    double m_wipeYPosition_min;
    double m_wipeYPosition_max;

    int m_wipeSymmetry;
    int m_wipeSymmetry_min;
    int m_wipeSymmetry_max;

    int m_wipeSoftness;
    int m_wipeSoftness_min;
    int m_wipeSoftness_max;

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

signals:

void fillSourceChanged(int fillSource);
void patternChanged(int pattern);
void wipeSizeChanged(int wipeSize);
void wipeXPositionChanged(double wipeXPosition);
void wipeYPositionChanged(double wipeYPosition);
void wipeSymmetryChanged(int wipeSymmetry);
void wipeSoftnessChanged(int wipeSoftness);
void maskEnableChanged(bool maskEnable);
void maskHStartChanged(int maskHStart);
void maskVStartChanged(int maskVStart);
void maskHEndChanged(int maskHEnd);
void maskVEndChanged(int maskVEnd);
void resizeChanged(bool resize);
void sizeChanged(QString size);
void xPositionChanged(double xPosition);
void yPositionChanged(double yPosition);
};

#endif // PATTERNPARAMETERS_H
