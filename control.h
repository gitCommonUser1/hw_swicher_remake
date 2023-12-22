#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>

class Control : public QObject
{
    Q_OBJECT
public:
    explicit Control(QObject *parent = nullptr);

    void init_connect();
    void connect_profile();
    void init_led();
    void init_rv_switch();
signals:

public slots:
    void slotKnobChanged(const int knob, const int value); //旋转编码器

    void slotPushChanged(const int push, const int value); //推杆

    void slotKeyChanged(const int key, const int value); //按键

    void slotKeyStatusChanged(const int key, const int status); //按键灯状态

    //业务

};

#endif // CONTROL_H
