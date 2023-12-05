#ifndef STILLS_H
#define STILLS_H

#include <QObject>

class Still;

class Stills : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> stills READ stills WRITE setStills NOTIFY stillsChanged)
public:
    explicit Stills(QObject *parent = nullptr);


    QList<QObject*> stills() const
    {
        return m_stills;
    }

public slots:

    void setStills(QList<QObject*> stills)
    {
        if (m_stills == stills)
            return;

        m_stills = stills;
        emit stillsChanged(m_stills);
    }

private:

    QList<QObject*> m_stills;

signals:

void stillsChanged(QList<QObject*> stills);
};

#endif // STILLS_H
