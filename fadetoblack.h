#ifndef FADETOBLACK_H
#define FADETOBLACK_H

#include <QObject>
#include "osee_math.h"

class FadeToBlack : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double rate READ rate WRITE setRate NOTIFY rateChanged)
    Q_PROPERTY(bool afv READ afv WRITE setAfv NOTIFY afvChanged)
public:
    explicit FadeToBlack(QObject *parent = nullptr);

    double rate() const
    {
        return m_rate;
    }

    bool afv() const
    {
        return m_afv;
    }

public slots:
    void setRate(double rate)
    {
        auto dst = dround(rate,2);
        if(dst < m_rate_min)
            dst = m_rate_min;
        if(dst > m_rate_max)
            dst = m_rate_max;
        if(dst == m_rate)
            return ;

        m_rate = dst;
        emit rateChanged(m_rate);
    }

    void setAfv(bool afv)
    {
        if (m_afv == afv)
            return;

        m_afv = afv;
        emit afvChanged(m_afv);
    }

private:

    double m_rate;
    double m_rate_min;
    double m_rate_max;

    bool m_afv;

signals:

void rateChanged(double rate);
void afvChanged(bool afv);
};

#endif // FADETOBLACK_H
