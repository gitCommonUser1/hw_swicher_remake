#ifndef DIPPARAMETERS_H
#define DIPPARAMETERS_H

#include <QObject>

class DipParameters : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float rate READ rate WRITE setRate NOTIFY rateChanged)
    Q_PROPERTY(int input READ input WRITE setInput NOTIFY inputChanged)

public:
    explicit DipParameters(QObject *parent = nullptr);

    float rate() const
    {
        return m_rate;
    }

    int input() const
    {
        return m_input;
    }

public slots:
    void setRate(float rate)
    {
        if (m_rate == rate)
            return;

        m_rate = rate;
        emit rateChanged(m_rate);
    }

    void setInput(int input)
    {
        if (m_input == input)
            return;

        m_input = input;
        emit inputChanged(m_input);
    }

private:

    float m_rate;

    int m_input;

signals:

void rateChanged(float rate);
void inputChanged(int input);
};

#endif // DIPPARAMETERS_H
