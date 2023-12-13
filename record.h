#ifndef RECORD_H
#define RECORD_H

#include <QObject>
#include "regexp.h"

class Record : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
public:
    explicit Record(QObject *parent = nullptr);

    QString fileName() const
    {
        return m_fileName;
    }

public slots:
    void setFileName(QString fileName)
    {
        if(!RegExp::exactMatch(REG_FILE_NAME,fileName))
            return ;

        if (m_fileName == fileName)
            return;

        m_fileName = fileName;
        emit fileNameChanged(m_fileName);
    }

private:


    QString m_fileName;

signals:

void fileNameChanged(QString fileName);
};

#endif // RECORD_H
