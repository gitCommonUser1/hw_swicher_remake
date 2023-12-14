#ifndef IPADDRESS_H
#define IPADDRESS_H

#include <QObject>
#include "regexp.h"

class IPAddress : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString value READ value WRITE setValue NOTIFY valueChanged)
    QString m_value;

public:
    explicit IPAddress(QObject *parent = nullptr);

QString value() const
{
    return m_value;
}

public slots:
void setValue(QString value)
{
    if(!RegExp::exactMatch(REG_IPADDRESS,value))
        return ;
    if (m_value == value)
        return;

    m_value = value;
    emit valueChanged(m_value);
}

signals:

void valueChanged(QString value);
};

#endif // IPADDRESS_H
