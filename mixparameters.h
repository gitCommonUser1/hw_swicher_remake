#ifndef MIXPARAMETERS_H
#define MIXPARAMETERS_H

#include <QObject>

class MixParameters : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double rate READ rate WRITE setRate NOTIFY rateChanged)
public:
    explicit MixParameters(QObject *parent = nullptr);

    double rate() const
    {
        return m_rate;
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

private:

    double m_rate;
    double m_rate_min;
    double m_rate_max;

signals:

void rateChanged(double rate);
};

#endif // MIXPARAMETERS_H
