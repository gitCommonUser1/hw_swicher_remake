#ifndef DIPPARAMETERS_H
#define DIPPARAMETERS_H

#include <QObject>
#include "transitionstyle.h"

class DipParameters : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double rate READ rate WRITE setRate NOTIFY rateChanged)
    Q_PROPERTY(int input READ input WRITE setInput NOTIFY inputChanged)
    Q_PROPERTY(bool stinger READ stinger WRITE setStinger NOTIFY stingerChanged)

public:
    explicit DipParameters(QObject *parent = nullptr);

    double rate() const
    {
        return m_rate;
    }

    int input() const
    {
        return m_input;
    }

    bool stinger() const
    {
        return m_stinger;
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

    void setInput(int input)
    {
        if(m_input < m_input_min)
            input = m_input_min;
        if(m_input >= m_input_max)
            input = m_input_max -1;
        if(m_stinger)
        {
            input = TransitionStyle::AUX;
        }
        if (m_input == input)
            return;

        m_input = input;
        emit inputChanged(m_input);
    }

    void setStinger(bool stinger)
    {
        if (m_stinger == stinger)
            return;

        m_stinger = stinger;
        emit stingerChanged(m_stinger);
    }

private:

    double m_rate;
    double m_rate_min;
    double m_rate_max;

    int m_input;
    int m_input_min;
    int m_input_max;

    bool m_stinger;

signals:

void rateChanged(double rate);
void inputChanged(int input);
void stingerChanged(bool stinger);
};

#endif // DIPPARAMETERS_H
