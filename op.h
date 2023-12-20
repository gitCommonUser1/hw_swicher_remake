#ifndef OP_H
#define OP_H

#include <QObject>
#include <QVariantMap>

class Op : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap method READ method WRITE setMethod)
public:
    explicit Op(QObject *parent = nullptr);

    QVariantMap method() const
    {
        return m_method;
    }

public slots:


    void setMethod(QVariantMap method)
    {
        m_method = method;
    }

private:

    QVariantMap m_method;

signals:


};

#endif // OP_H
