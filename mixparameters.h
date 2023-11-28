#ifndef MIXPARAMETERS_H
#define MIXPARAMETERS_H

#include <QObject>

class MixParameters : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float rate READ rate WRITE setRate NOTIFY rateChanged)
public:
    explicit MixParameters(QObject *parent = nullptr);

    float rate() const
    {
        return m_rate;
    }

public slots:
    void setRate(float rate)
    {
        if (m_rate == rate)
            return;

        m_rate = rate;
        emit rateChanged(m_rate);
    }

private:

    float m_rate;

signals:

void rateChanged(float rate);
};

#endif // MIXPARAMETERS_H
