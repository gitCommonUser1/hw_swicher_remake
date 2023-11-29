#ifndef WIPEPARAMETERS_H
#define WIPEPARAMETERS_H

#include <QObject>

//extern double dround(double x,int bit);
#include "osee_math.h"

class WipeParameters : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double rate READ rate WRITE setRate NOTIFY rateChanged)
    Q_PROPERTY(int pattern READ pattern WRITE setPattern NOTIFY patternChanged)
    Q_PROPERTY(int symmetry READ symmetry WRITE setSymmetry NOTIFY symmetryChanged)
    Q_PROPERTY(double xPosition READ xPosition WRITE setXPosition NOTIFY xPositionChanged)
    Q_PROPERTY(double yPosition READ yPosition WRITE setYPosition NOTIFY yPositionChanged)
    Q_PROPERTY(bool reverseDirection READ reverseDirection WRITE setReverseDirection NOTIFY reverseDirectionChanged)
    Q_PROPERTY(int softness READ softness WRITE setSoftness NOTIFY softnessChanged)
    Q_PROPERTY(int border READ border WRITE setBorder NOTIFY borderChanged)
    Q_PROPERTY(int fillSource READ fillSource WRITE setFillSource NOTIFY fillSourceChanged)

public:
    explicit WipeParameters(QObject *parent = nullptr);

    double rate() const
    {
        return m_rate;
    }

    int pattern() const
    {
        return m_pattern;
    }

    int symmetry() const
    {
        return m_symmetry;
    }

    double xPosition() const
    {
        return m_xPosition;
    }

    double yPosition() const
    {
        return m_yPosition;
    }

    bool reverseDirection() const
    {
        return m_reverseDirection;
    }

    int softness() const
    {
        return m_softness;
    }

    int border() const
    {
        return m_border;
    }

    int fillSource() const
    {
        return m_fillSource;
    }

public slots:
    void setRate(double rate)
    {
        if(rate > m_rate_max)
            rate = m_rate_max;
        if(rate < m_rate_min)
            rate = m_rate_min;
        if (m_rate == rate)
            return;

        m_rate = rate;
        emit rateChanged(m_rate);
    }

    void setPattern(int pattern)
    {
        if(pattern < m_pattern_min)
            pattern = m_pattern_min;
        if(pattern > m_pattern_max)
            pattern = m_pattern_max;
        if (m_pattern == pattern)
            return;

        m_pattern = pattern;
        emit patternChanged(m_pattern);
    }

    void setSymmetry(int symmetry)
    {
        if(symmetry < m_symmetry_min)
            symmetry = m_symmetry_min;
        if(symmetry > m_symmetry_max)
            symmetry = m_symmetry_max;
        if (m_symmetry == symmetry)
            return;

        m_symmetry = symmetry;
        emit symmetryChanged(m_symmetry);
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

    void setReverseDirection(bool reverseDirection)
    {
        if (m_reverseDirection == reverseDirection)
            return;

        m_reverseDirection = reverseDirection;
        emit reverseDirectionChanged(m_reverseDirection);
    }

    void setSoftness(int softness)
    {
        if(softness < m_softness_min)
            softness = m_softness_min;
        if(softness > m_softness_max)
            softness = m_softness_max;
        if (m_softness == softness)
            return;

        m_softness = softness;
        emit softnessChanged(m_softness);
    }

    void setBorder(int border)
    {
        if(border < m_border_min)
            border = m_border_min;
        if(border > m_border_max)
            border = m_border_max;
        if (m_border == border)
            return;

        m_border = border;
        emit borderChanged(m_border);
    }

    void setFillSource(int fillSource)
    {
        if(fillSource < m_fillSource_min)
            fillSource = m_fillSource_min;
        if(fillSource >= m_fillSource_max)
            fillSource = m_fillSource_max - 1;
        if (m_fillSource == fillSource)
            return;

        m_fillSource = fillSource;
        emit fillSourceChanged(m_fillSource);
    }

    double xPositionMax(){return m_xPosition_max;}
    double xPositionMin(){return m_xPosition_min;}
    double yPositionMax(){return m_yPosition_max;}
    double yPositionMin(){return m_yPosition_min;}
    int symmetryMax(){return m_symmetry_max;}
    int symmetryMin(){return m_symmetry_min;}
    int softnessMax(){return m_softness_max;}
    int softnessMin(){return m_softness_min;}
    int borderMax(){return m_border_max;}
    int borderMin(){return m_border_min;}

private:

    double m_rate;
    double m_rate_min;
    double m_rate_max;

    int m_pattern;
    int m_pattern_min;
    int m_pattern_max;

    int m_symmetry;
    int m_symmetry_min;
    int m_symmetry_max;

    double m_xPosition;
    double m_xPosition_min;
    double m_xPosition_max;

    double m_yPosition;
    double m_yPosition_min;
    double m_yPosition_max;

    bool m_reverseDirection;
    bool m_reverseDirection_min;
    bool m_reverseDirection_max;

    int m_softness;
    int m_softness_min;
    int m_softness_max;

    int m_border;
    int m_border_min;
    int m_border_max;

    int m_fillSource;
    int m_fillSource_min;
    int m_fillSource_max;

signals:

void rateChanged(double rate);
void patternChanged(int pattern);
void symmetryChanged(int symmetry);
void xPositionChanged(double xPosition);
void yPositionChanged(double yPosition);
void reverseDirectionChanged(bool reverseDirection);
void softnessChanged(int softness);
void borderChanged(int border);
void fillSourceChanged(int fillSource);
};

#endif // WIPEPARAMETERS_H
