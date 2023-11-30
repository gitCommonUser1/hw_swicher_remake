#ifndef KEY_H
#define KEY_H

#include <QObject>

class Key : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int  index READ index WRITE setIndex FINAL)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(bool onAir READ onAir WRITE setOnAir NOTIFY onAirChanged)

public:
    explicit Key(QObject *parent = nullptr);
    static QString typeIndexToString(int index);
    static int typeStringToIndex(QString str);

    int index() const
    {
        return m_index;
    }

    QString type() const
    {
        return m_type;
    }

    bool onAir() const
    {
        return m_onAir;
    }

public slots:
    void setIndex(int index)
    {
        if (m_index == index)
            return;

        m_index = index;
        emit indexChanged(m_index);
    }

    void setType(QString type)
    {
        if (m_type == type)
            return;

        m_type = type;
        emit typeChanged(m_type);
    }

    void setOnAir(bool onAir)
    {
        if (m_onAir == onAir)
            return;

        m_onAir = onAir;
        emit onAirChanged(m_onAir);
    }

private:

    int m_index;

    QString m_type;

    bool m_onAir;

signals:

void indexChanged(int index);
void typeChanged(QString type);
void onAirChanged(bool onAir);
};

#endif // KEY_H
