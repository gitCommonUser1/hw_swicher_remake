#ifndef REGEXP_H
#define REGEXP_H

#include <QObject>

enum REGULAR{
    REG_IPADDRESS = 0,
    REG_FILE_NAME,
    REG_SRC_NAME,
    REG_MAX
};

class RegExp : public QObject
{
    Q_OBJECT
public:
    explicit RegExp(QObject *parent = nullptr);

    Q_INVOKABLE static bool exactMatch(int regIndex,QString text);

signals:

};

#endif // REGEXP_H
