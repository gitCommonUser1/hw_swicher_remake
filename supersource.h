#ifndef SUPERSOURCE_H
#define SUPERSOURCE_H

#include <QObject>

class SuperSourceMask;
class SuperSourceBorder;

class SuperSource : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enable READ enable WRITE setEnable NOTIFY enableChanged)
    Q_PROPERTY(int source1 READ source1 WRITE setSource1 NOTIFY source1Changed)
    Q_PROPERTY(int source2 READ source2 WRITE setSource2 NOTIFY source2Changed)
    Q_PROPERTY(int background READ background WRITE setBackground NOTIFY backgroundChanged)
    Q_PROPERTY(QString style READ style WRITE setStyle NOTIFY styleChanged)
    Q_PROPERTY(int yPosition READ yPosition WRITE setYPosition NOTIFY yPositionChanged)
    Q_PROPERTY(SuperSourceMask* mask1 READ mask1)
    Q_PROPERTY(SuperSourceMask* mask2 READ mask2)
    Q_PROPERTY(SuperSourceBorder* border1 READ border1)
    Q_PROPERTY(SuperSourceBorder* border2 READ border2)
public:
    explicit SuperSource(QObject *parent = nullptr);

    static int styleStringToIndex(QString str);
    static QString styleIndexToString(int index);

    enum MASK{
        MASK1 = 0,
        MASK2
    };

    enum BORDER{
        BORDER1 = 0,
        BORDER2
    };

    enum STYLE{
        ZOOM_IN = 0,
        CROP_ZOOM_IN,
        ZOOM_IN_CROP,
        CROP
    };

    bool enable() const
    {
        return m_enable;
    }

    int source1() const
    {
        return m_source1;
    }

    int source2() const
    {
        return m_source2;
    }

    int background() const
    {
        return m_background;
    }

    QString style() const
    {
        return m_style;
    }

    int yPosition() const
    {
        return m_yPosition;
    }

    SuperSourceMask* mask1() const
    {
        return m_mask1;
    }

    SuperSourceMask* mask2() const
    {
        return m_mask2;
    }

    SuperSourceBorder* border1() const
    {
        return m_border1;
    }

    SuperSourceBorder* border2() const
    {
        return m_border2;
    }

public slots:
    void setEnable(bool enable)
    {
        if (m_enable == enable)
            return;

        m_enable = enable;
        emit enableChanged(m_enable);
    }

    void setSource1(int source1)
    {
        if(source1 >= m_source1_max)
            source1 = m_source1_max - 1;
        if(source1 < m_source1_min)
            source1 = m_source1_min;
        if (m_source1 == source1)
            return;

        m_source1 = source1;
        emit source1Changed(m_source1);
    }

    void setSource2(int source2)
    {
        if(source2 >= m_source2_max)
            source2 = m_source2_max - 1;
        if(source2 < m_source2_min)
            source2 = m_source2_min;
        if (m_source2 == source2)
            return;

        m_source2 = source2;
        emit source2Changed(m_source2);
    }

    void setBackground(int background)
    {
        if(background >= m_background_max)
            background = m_background_max - 1;
        if(background < m_background_min)
            background = m_background_min;
        if (m_background == background)
            return;

        m_background = background;
        emit backgroundChanged(m_background);
    }

    void setStyle(QString style)
    {
        if (m_style == style)
            return;

        m_style = style;
        emit styleChanged(m_style);
    }

    void setYPosition(int yPosition)
    {
        if(yPosition > m_yPosition_max)
            yPosition = m_yPosition_max;
        if(yPosition < m_yPosition_min)
            yPosition = m_yPosition_min;
        if (m_yPosition == yPosition)
            return;

        m_yPosition = yPosition;
        emit yPositionChanged(m_yPosition);
    }

private:

    bool m_enable;

    int m_source1;
    int m_source1_min;
    int m_source1_max;

    int m_source2;
    int m_source2_min;
    int m_source2_max;

    int m_background;
    int m_background_min;
    int m_background_max;

    QString m_style;

    int m_yPosition;
    int m_yPosition_min;
    int m_yPosition_max;

    SuperSourceMask* m_mask1;

    SuperSourceMask* m_mask2;

    SuperSourceBorder* m_border1;

    SuperSourceBorder* m_border2;

signals:

void enableChanged(bool enable);
void source1Changed(int source1);
void source2Changed(int source2);
void backgroundChanged(int background);
void styleChanged(QString style);
void yPositionChanged(int yPosition);
};

#endif // SUPERSOURCE_H
