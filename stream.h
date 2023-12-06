#ifndef STREAM_H
#define STREAM_H

#include <QObject>

class Stream : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int index READ index WRITE setIndex FINAL)
    Q_PROPERTY(QString platfrom READ platfrom WRITE setPlatfrom NOTIFY platfromChanged)
    Q_PROPERTY(QString server READ server WRITE setServer NOTIFY serverChanged)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(bool output READ output WRITE setOutput NOTIFY outputChanged)
public:
    explicit Stream(int index,QObject *parent = nullptr);

    int index() const
    {
        return m_index;
    }

    QString platfrom() const
    {
        return m_platfrom;
    }

    QString server() const
    {
        return m_server;
    }

    QString url() const
    {
        return m_url;
    }

    bool output() const
    {
        return m_output;
    }

public slots:
    void setIndex(int index)
    {
        m_index = index;
    }

    void setPlatfrom(QString platfrom)
    {
        if (m_platfrom == platfrom)
            return;

        m_platfrom = platfrom;
        emit platfromChanged(m_platfrom);
    }

    void setServer(QString server)
    {
        if (m_server == server)
            return;

        m_server = server;
        emit serverChanged(m_server);
    }

    void setUrl(QString url)
    {
        if (m_url == url)
            return;

        m_url = url;
        emit urlChanged(m_url);
    }

    void setOutput(bool output)
    {
        if (m_output == output)
            return;

        m_output = output;
        emit outputChanged(m_output);
    }

private:
    int m_index;

    QString m_platfrom;

    QString m_server;

    QString m_url;

    bool m_output;

signals:

void platfromChanged(QString platfrom);
void serverChanged(QString server);
void urlChanged(QString url);
void outputChanged(bool output);
};

#endif // STREAM_H
