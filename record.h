#ifndef RECORD_H
#define RECORD_H

#include <QObject>
#include "regexp.h"

class Record : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(int quality READ quality WRITE setQuality NOTIFY qualityChanged)
public:
    explicit Record(QObject *parent = nullptr);

    enum QUALITY {
        HIGH = 0,
        MID,
        LOW
    };

    QString fileName() const
    {
        return m_fileName;
    }

    int quality() const
    {
        return m_quality;
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

    void setQuality(int quality)
    {
        if(quality >= LOW)
            quality = LOW;
        if(quality <= HIGH)
            quality = HIGH;

        if (m_quality == quality)
            return;

        m_quality = quality;
        emit qualityChanged(m_quality);
    }

private:


    QString m_fileName;

    int m_quality;

signals:

void fileNameChanged(QString fileName);
void qualityChanged(int quality);
};

#endif // RECORD_H
