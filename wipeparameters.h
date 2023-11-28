#ifndef WIPEPARAMETERS_H
#define WIPEPARAMETERS_H

#include <QObject>

class WipeParameters : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float rate READ rate WRITE setRate NOTIFY rateChanged)
    Q_PROPERTY(int pattern READ pattern WRITE setPattern NOTIFY patternChanged)
    Q_PROPERTY(int symmetry READ symmetry WRITE setSymmetry NOTIFY symmetryChanged)
    Q_PROPERTY(float xPosition READ xPosition WRITE setXPosition NOTIFY xPositionChanged)
    Q_PROPERTY(float yPosition READ yPosition WRITE setYPosition NOTIFY yPositionChanged)
    Q_PROPERTY(bool reverseDirection READ reverseDirection WRITE setReverseDirection NOTIFY reverseDirectionChanged)
    Q_PROPERTY(int softness READ softness WRITE setSoftness NOTIFY softnessChanged)
    Q_PROPERTY(int border READ border WRITE setBorder NOTIFY borderChanged)
    Q_PROPERTY(int fillSource READ fillSource WRITE setFillSource NOTIFY fillSourceChanged)

public:
    explicit WipeParameters(QObject *parent = nullptr);

    float rate() const
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

    float xPosition() const
    {
        return m_xPosition;
    }

    float yPosition() const
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
    void setRate(float rate)
    {
        if (m_rate == rate)
            return;

        m_rate = rate;
        emit rateChanged(m_rate);
    }

    void setPattern(int pattern)
    {
        if (m_pattern == pattern)
            return;

        m_pattern = pattern;
        emit patternChanged(m_pattern);
    }

    void setSymmetry(int symmetry)
    {
        if (m_symmetry == symmetry)
            return;

        m_symmetry = symmetry;
        emit symmetryChanged(m_symmetry);
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

    void setReverseDirection(bool reverseDirection)
    {
        if (m_reverseDirection == reverseDirection)
            return;

        m_reverseDirection = reverseDirection;
        emit reverseDirectionChanged(m_reverseDirection);
    }

    void setSoftness(int softness)
    {
        if (m_softness == softness)
            return;

        m_softness = softness;
        emit softnessChanged(m_softness);
    }

    void setBorder(int border)
    {
        if (m_border == border)
            return;

        m_border = border;
        emit borderChanged(m_border);
    }

    void setFillSource(int fillSource)
    {
        if (m_fillSource == fillSource)
            return;

        m_fillSource = fillSource;
        emit fillSourceChanged(m_fillSource);
    }

private:

    float m_rate;

    int m_pattern;

    int m_symmetry;

    float m_xPosition;

    float m_yPosition;

    bool m_reverseDirection;

    int m_softness;

    int m_border;

    int m_fillSource;

signals:

void rateChanged(float rate);
void patternChanged(int pattern);
void symmetryChanged(int symmetry);
void xPositionChanged(float xPosition);
void yPositionChanged(float yPosition);
void reverseDirectionChanged(bool reverseDirection);
void softnessChanged(int softness);
void borderChanged(int border);
void fillSourceChanged(int fillSource);
};

#endif // WIPEPARAMETERS_H
