#ifndef OUTSOURCE_H
#define OUTSOURCE_H

#include <QObject>

class OutSource : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString src READ src WRITE setSrc FINAL)
    Q_PROPERTY(int source READ source WRITE setSource NOTIFY sourceChanged)
public:
    explicit OutSource(QString src,QObject *parent = nullptr);

    QString src() const
    {
        return m_src;
    }

    int source() const
    {
        return m_source;
    }

public slots:
    void setSrc(QString src)
    {
        m_src = src;
    }

    void setSource(int source)
    {
        if (m_source == source)
            return;

        m_source = source;
        emit sourceChanged(m_source);
    }

private:


    QString m_src;

    int m_source;

signals:

void sourceChanged(int source);
};

#endif // OUTSOURCE_H
