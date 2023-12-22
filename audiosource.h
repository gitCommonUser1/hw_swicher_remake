#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include <QObject>
#include "osee_math.h"

class AudioSource : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName FINAL)
    Q_PROPERTY(double fader READ fader WRITE setFader NOTIFY faderChanged)
    Q_PROPERTY(double balance READ balance WRITE setBalance NOTIFY balanceChanged)
    Q_PROPERTY(double input READ input WRITE setInput NOTIFY inputChanged)
    Q_PROPERTY(int delay READ delay WRITE setDelay NOTIFY delayChanged)
    Q_PROPERTY(int enable READ enable WRITE setEnable NOTIFY enableChanged)
    Q_PROPERTY(int level READ level WRITE setLevel NOTIFY levelChanged)
    Q_PROPERTY(int source READ source WRITE setSource NOTIFY sourceChanged)
public:
    explicit AudioSource(QObject *parent = nullptr);

    enum AUDIOSOURCE{
        MIC1 = 0,
        MIC2,
        IN1,
        IN2,
        IN3,
        IN4,
        AUX,
        PGM,
        MAX
    };
    enum AUDIOENABLE{
        OFF = 0,
        ON,
        AFV,
        AUDIO_ENABLE_MAX
    };

    static int sourceNameStringToIndex(QString name);
    static QString sourceNameIndexToString(int index);

    QString name() const
    {
        return m_name;
    }

    double fader() const
    {
        return m_fader;
    }

    double balance() const
    {
        return m_balance;
    }

    double input() const
    {
        return m_input;
    }

    int delay() const
    {
        return m_delay;
    }

    int enable() const
    {
        return m_enable;
    }

    int level() const
    {
        return m_level;
    }

    int source() const
    {
        return m_source;
    }

public slots:
    void setName(QString name)
    {
        if (m_name == name)
            return;

        m_name = name;
    }

    void setFader(double fader)
    {
        auto dst = dround(fader,2);
        if(dst < m_fader_min)
            dst = m_fader_min;
        if(dst > m_fader_max)
            dst = m_fader_max;
        if(dst == m_fader)
            return ;

        m_fader = dst;
        emit faderChanged(m_fader);
    }

    void setBalance(double balance)
    {
        auto dst = dround(balance,2);
        if(dst < m_balance_min)
            dst = m_balance_min;
        if(dst > m_balance_max)
            dst = m_balance_max;
        if(dst == m_balance)
            return ;

        m_balance = dst;
        emit balanceChanged(m_balance);
    }

    void setInput(double input)
    {
        auto dst = dround(input,2);
        if(dst < m_input_min)
            dst = m_input_min;
        if(dst > m_input_max)
            dst = m_input_max;
        if(dst == m_input)
            return ;

        m_input = dst;
        emit inputChanged(m_input);
    }

    void setDelay(int delay)
    {
        if(delay < m_delay_min)
            delay = m_delay_min;
        if(delay > m_delay_max)
            delay = m_delay_max;
        if (m_delay == delay)
            return;

        m_delay = delay;
        emit delayChanged(m_delay);
    }

    void setEnable(int enable)
    {
        if(enable < m_enable_min)
            enable = m_enable_min;
        if(enable >= m_enable_max)
            enable = m_enable_max-1;
        if (m_enable == enable)
            return;

        m_enable = enable;
        emit enableChanged(m_enable);
    }

    void setLevel(int level)
    {
        if(level < m_level_min)
            level = m_level_min;
        if(level > m_level_max)
            level = m_level_max;
        if (m_level == level)
            return;

        m_level = level;
        emit levelChanged(m_level);
    }

    void setSource(int source)
    {
        if(source >= m_source_max)
            source = m_source_max - 1;
        if(source < m_source_min)
            source = m_source_min;
        if (m_source == source)
            return;

        m_source = source;
        emit sourceChanged(m_source);
    }

private:

    QString m_name;

    double m_fader;
    double m_fader_max;
    double m_fader_min;

    double m_balance;
    double m_balance_max;
    double m_balance_min;

    double m_input;
    double m_input_max;
    double m_input_min;

    int m_delay;
    int m_delay_max;
    int m_delay_min;

    int m_enable;
    int m_enable_min;
    int m_enable_max;

    int m_level;
    int m_level_max;
    int m_level_min;

    int m_source;
    int m_source_max;
    int m_source_min;

signals:

void faderChanged(double fader);
void balanceChanged(double balance);
void inputChanged(double input);
void delayChanged(int delay);
void enableChanged(int enable);
void levelChanged(int level);
void sourceChanged(int source);
};

#endif // AUDIOSOURCE_H
