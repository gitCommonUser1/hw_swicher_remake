#ifndef SRCNAME_H
#define SRCNAME_H

#include <QObject>
#include "regexp.h"

class SrcName : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString src READ src FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
public:
    explicit SrcName(QString src,QObject *parent = nullptr);

    QString src() const
    {
        return m_src;
    }

    QString name() const
    {
        return m_name;
    }

public slots:
    void setName(QString name)
    {
        if(!RegExp::exactMatch(REG_SRC_NAME,name))
            return ;

        if (m_name == name)
            return;

        m_name = name;
        emit nameChanged(m_name);
    }

private:


    QString m_src;

    QString m_name;

signals:
void nameChanged(QString name);
};

#endif // SRCNAME_H
