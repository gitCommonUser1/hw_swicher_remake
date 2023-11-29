#ifndef DIPPARAMETERS_H
#define DIPPARAMETERS_H

#include <QObject>

class DipParameters : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double rate READ rate WRITE setRate NOTIFY rateChanged)
    Q_PROPERTY(int input READ input WRITE setInput NOTIFY inputChanged)

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
        if (m_input == input)
            return;

        m_input = input;
        emit inputChanged(m_input);
    }

private:

    double m_rate;
    double m_rate_min;
    double m_rate_max;

    int m_input;
    int m_input_min;
    int m_input_max;

signals:

void rateChanged(double rate);
void inputChanged(int input);
};

#endif // DIPPARAMETERS_H
