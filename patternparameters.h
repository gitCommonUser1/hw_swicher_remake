#ifndef PATTERNPARAMETERS_H
#define PATTERNPARAMETERS_H

#include <QObject>

class PatternParameters : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int fillSource READ fillSource WRITE setFillSource NOTIFY fillSourceChanged)
    Q_PROPERTY(int pattern READ pattern WRITE setPattern NOTIFY patternChanged)
    Q_PROPERTY(int wipeSize READ wipeSize WRITE setWipeSize NOTIFY wipeSizeChanged)
    Q_PROPERTY(float wipeXPosition READ wipeXPosition WRITE setWipeXPosition NOTIFY wipeXPositionChanged)
    Q_PROPERTY(float wipeYPosition READ wipeYPosition WRITE setWipeYPosition NOTIFY wipeYPositionChanged)
    Q_PROPERTY(int wipeSymmetry READ wipeSymmetry WRITE setWipeSymmetry NOTIFY wipeSymmetryChanged)
    Q_PROPERTY(int wipeSoftness READ wipeSoftness WRITE setWipeSoftness NOTIFY wipeSoftnessChanged)
    Q_PROPERTY(bool maskEnable READ maskEnable WRITE setMaskEnable NOTIFY maskEnableChanged)
    Q_PROPERTY(int maskHStart READ maskHStart WRITE setMaskHStart NOTIFY maskHStartChanged)
    Q_PROPERTY(int maskVStart READ maskVStart WRITE setMaskVStart NOTIFY maskVStartChanged)
    Q_PROPERTY(int maskHEnd READ maskHEnd WRITE setMaskHEnd NOTIFY maskHEndChanged)
    Q_PROPERTY(int maskVEnd READ maskVEnd WRITE setMaskVEnd NOTIFY maskVEndChanged)
    Q_PROPERTY(bool resize READ resize WRITE setResize NOTIFY resizeChanged)
    Q_PROPERTY(QString size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(float xPosition READ xPosition WRITE setXPosition NOTIFY xPositionChanged)
    Q_PROPERTY(float yPosition READ yPosition WRITE setYPosition NOTIFY yPositionChanged)
public:
    explicit PatternParameters(QObject *parent = nullptr);

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

    float wipeXPosition() const
    {
        return m_wipeXPosition;
    }

    float wipeYPosition() const
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

    float xPosition() const
    {
        return m_xPosition;
    }

    float yPosition() const
    {
        return m_yPosition;
    }

public slots:
    void setFillSource(int fillSource)
    {
        if (m_fillSource == fillSource)
            return;

        m_fillSource = fillSource;
        emit fillSourceChanged(m_fillSource);
    }

    void setPattern(int pattern)
    {
        if (m_pattern == pattern)
            return;

        m_pattern = pattern;
        emit patternChanged(m_pattern);
    }

    void setWipeSize(int wipeSize)
    {
        if (m_wipeSize == wipeSize)
            return;

        m_wipeSize = wipeSize;
        emit wipeSizeChanged(m_wipeSize);
    }

    void setWipeXPosition(float wipeXPosition)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_wipeXPosition, wipeXPosition))
            return;

        m_wipeXPosition = wipeXPosition;
        emit wipeXPositionChanged(m_wipeXPosition);
    }

    void setWipeYPosition(float wipeYPosition)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_wipeYPosition, wipeYPosition))
            return;

        m_wipeYPosition = wipeYPosition;
        emit wipeYPositionChanged(m_wipeYPosition);
    }

    void setWipeSymmetry(int wipeSymmetry)
    {
        if (m_wipeSymmetry == wipeSymmetry)
            return;

        m_wipeSymmetry = wipeSymmetry;
        emit wipeSymmetryChanged(m_wipeSymmetry);
    }

    void setWipeSoftness(int wipeSoftness)
    {
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

private:

    int m_fillSource;

    int m_pattern;

    int m_wipeSize;

    float m_wipeXPosition;

    float m_wipeYPosition;

    int m_wipeSymmetry;

    int m_wipeSoftness;

    bool m_maskEnable;

    int m_maskHStart;

    int m_maskVStart;

    int m_maskHEnd;

    int m_maskVEnd;

    bool m_resize;

    QString m_size;

    float m_xPosition;

    float m_yPosition;

signals:

void fillSourceChanged(int fillSource);
void patternChanged(int pattern);
void wipeSizeChanged(int wipeSize);
void wipeXPositionChanged(float wipeXPosition);
void wipeYPositionChanged(float wipeYPosition);
void wipeSymmetryChanged(int wipeSymmetry);
void wipeSoftnessChanged(int wipeSoftness);
void maskEnableChanged(bool maskEnable);
void maskHStartChanged(int maskHStart);
void maskVStartChanged(int maskVStart);
void maskHEndChanged(int maskHEnd);
void maskVEndChanged(int maskVEnd);
void resizeChanged(bool resize);
void sizeChanged(QString size);
void xPositionChanged(float xPosition);
void yPositionChanged(float yPosition);
};

#endif // PATTERNPARAMETERS_H
