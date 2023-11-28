#ifndef FADETOBLACK_H
#define FADETOBLACK_H

#include <QObject>

class FadeToBlack : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float rate READ rate WRITE setRate NOTIFY rateChanged)
    Q_PROPERTY(bool afv READ afv WRITE setAfv NOTIFY afvChanged)
public:
    explicit FadeToBlack(QObject *parent = nullptr);

    float rate() const
    {
        return m_rate;
    }

    bool afv() const
    {
        return m_afv;
    }

public slots:
    void setRate(float rate)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_rate, rate))
            return;

        m_rate = rate;
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

    float m_rate;

    bool m_afv;

signals:

void rateChanged(float rate);
void afvChanged(bool afv);
};

#endif // FADETOBLACK_H
