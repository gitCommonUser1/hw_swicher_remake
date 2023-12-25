#ifndef STILL_H
#define STILL_H

#include <QObject>

class Still : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int index READ index WRITE setIndex)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
public:
    explicit Still(QObject *parent = nullptr);
    explicit Still(int index,QString name,QString path,QObject *parent = nullptr);

    int index() const
    {
        return m_index;
    }

    QString name() const
    {
        return m_name;
    }

    QString path() const
    {
        return m_path;
    }

public slots:
    void setIndex(int index)
    {
        if (m_index == index)
            return;

        m_index = index;
    }

    void setName(QString name)
    {
        if (m_name == name)
            return;

        m_name = name;
        emit nameChanged(m_name);
    }

    void setPath(QString path)
    {
        if (m_path == path)
            return;

        m_path = path;
        emit pathChanged(m_path);
    }

private:


    int m_index;

    QString m_name;

    QString m_path;

signals:
void nameChanged(QString name);
void pathChanged(QString path);
};

#endif // STILL_H
