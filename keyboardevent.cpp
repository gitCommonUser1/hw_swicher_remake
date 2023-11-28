#include "keyboardevent.h"
#include <QApplication>
#include <QKeyEvent>

KeyboardEvent::KeyboardEvent(QObject *parent) : QObject(parent)
{

}

void KeyboardEvent::sendKeyEvent(QObject *obj,QString key)
{
    if(key == "space"){
        QCoreApplication::sendEvent(obj, new QKeyEvent(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier," "));
        QCoreApplication::sendEvent(obj, new QKeyEvent(QEvent::KeyRelease, Qt::Key_Space, Qt::NoModifier," "));
    }else if(key == "backspace"){
        QCoreApplication::sendEvent(obj, new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier));
        QCoreApplication::sendEvent(obj, new QKeyEvent(QEvent::KeyRelease, Qt::Key_Backspace, Qt::NoModifier));
    }else if(key == "left"){
        QCoreApplication::sendEvent(obj, new QKeyEvent(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier));
        QCoreApplication::sendEvent(obj, new QKeyEvent(QEvent::KeyRelease, Qt::Key_Left, Qt::NoModifier));
    }else if(key == "right"){
        QCoreApplication::sendEvent(obj, new QKeyEvent(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier));
        QCoreApplication::sendEvent(obj, new QKeyEvent(QEvent::KeyRelease, Qt::Key_Right, Qt::NoModifier));
    }
    else{
        QCoreApplication::sendEvent(obj, new QKeyEvent(QEvent::KeyPress, 0, Qt::NoModifier, key));
        QCoreApplication::sendEvent(obj, new QKeyEvent(QEvent::KeyRelease,0, Qt::NoModifier, key));
    }
}
