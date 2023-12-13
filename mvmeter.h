#ifndef MVMETER_H
#define MVMETER_H

#include <QObject>

class MvMeter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enable READ enable WRITE setEnable NOTIFY enableChanged)
    Q_PROPERTY(QString name READ name WRITE setName FINAL)
public:
    explicit MvMeter(QString name,QObject *parent = nullptr);

    bool enable() const
    {
        return m_enable;
    }

    QString name() const
    {
        return m_name;
    }

public slots:
    void setEnable(bool enable)
    {
        if (m_enable == enable)
            return;

        m_enable = enable;
        emit enableChanged(m_enable);
    }

    void setName(QString name)
    {
        m_name = name;
    }

private:


    bool m_enable;

    QString m_name;

signals:

void enableChanged(bool enable);
};

#endif // MVMETER_H
