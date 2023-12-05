#ifndef STILLSELECTION_H
#define STILLSELECTION_H

#include <QObject>

class StillSelection : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int still1 READ still1 WRITE setStill1 NOTIFY still1Changed)
    Q_PROPERTY(int still2 READ still2 WRITE setStill2 NOTIFY still2Changed)
    Q_PROPERTY(int location READ location WRITE setLocation NOTIFY locationChanged)
public:
    explicit StillSelection(QObject *parent = nullptr);

    int still1() const
    {
        return m_still1;
    }

    int still2() const
    {
        return m_still2;
    }

    int location() const
    {
        return m_location;
    }

public slots:
    void setStill1(int still1)
    {
        if(still1 < m_still1_min)
            still1 = m_still1_min;
        if(still1 > m_still1_max)
            still1 = m_still1_max;
        if (m_still1 == still1)
            return;

        m_still1 = still1;
        emit still1Changed(m_still1);
    }

    void setStill2(int still2)
    {
        if(still2 < m_still2_min)
            still2 = m_still2_min;
        if(still2 > m_still2_max)
            still2 = m_still2_max;
        if (m_still2 == still2)
            return;

        m_still2 = still2;
        emit still2Changed(m_still2);
    }

    void setLocation(int location)
    {
        if(location < m_location_min)
            location = m_location_min;
        if(location > m_location_max)
            location = m_location_max;
        if (m_location == location)
            return;

        m_location = location;
        emit locationChanged(m_location);
    }

private:

    int m_still1;
    int m_still1_max;
    int m_still1_min;


    int m_still2;
    int m_still2_max;
    int m_still2_min;

    int m_location;
    int m_location_max;
    int m_location_min;

signals:

void still1Changed(int still1);
void still2Changed(int still2);
void locationChanged(int location);
};

#endif // STILLSELECTION_H
