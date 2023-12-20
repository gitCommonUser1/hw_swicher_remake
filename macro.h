#ifndef MACRO_H
#define MACRO_H

#include <QObject>
class Op;

class Macro : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QList<QObject*> ops READ ops WRITE setOps NOTIFY opsChanged)
public:
    explicit Macro(QObject *parent = nullptr);

    void append(Op *op);
    void remove(int index);

    int index() const
    {
        return m_index;
    }

    QString name() const
    {
        return m_name;
    }

    QString description() const
    {
        return m_description;
    }

    QList<QObject*> ops() const
    {
        return m_ops;
    }

public slots:
    void setIndex(int index)
    {
        if (m_index == index)
            return;

        m_index = index;
        emit indexChanged(m_index);
    }

    void setName(QString name)
    {
        if (m_name == name)
            return;

        m_name = name;
        emit nameChanged(m_name);
    }

    void setDescription(QString description)
    {
        if (m_description == description)
            return;

        m_description = description;
        emit descriptionChanged(m_description);
    }

    void setOps(QList<QObject*> ops)
    {
        if (m_ops == ops)
            return;

        m_ops = ops;
        emit opsChanged(m_ops);
    }

private:


    int m_index;

    QString m_name;

    QString m_description;

    QList<QObject*> m_ops;

signals:

void indexChanged(int index);
void nameChanged(QString name);
void descriptionChanged(QString description);
void opsChanged(QList<QObject*> ops);
};

#endif // MACRO_H
