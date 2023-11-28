#ifndef STREAMSERVICE_H
#define STREAMSERVICE_H

#include <QObject>

class StreamService : public QObject
{
    Q_OBJECT
public:
    explicit StreamService(QObject *parent = nullptr);

signals:

};

#endif // STREAMSERVICE_H
