#ifndef KEYBOARDEVENT_H
#define KEYBOARDEVENT_H

#include <QObject>

class KeyboardEvent : public QObject
{
    Q_OBJECT
    Q_ENUMS(KEYBOARD_TYPE)
public:
    enum KEYBOARD_TYPE{
        KEYBOARD_NORMAL = 0,
        KEYBOARD_IPADDR,
        KEYBOARD_MAX
    };
    explicit KeyboardEvent(QObject *parent = nullptr);

    Q_INVOKABLE void sendKeyEvent(QObject *obj,QString key);

signals:

};

#endif // KEYBOARDEVENT_H
