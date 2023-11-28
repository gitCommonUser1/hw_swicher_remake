#ifndef AUXCALLBACK_H
#define AUXCALLBACK_H

#include <QDebug>
#include "settings.h"
#include "rv_switch_api.h"

extern Settings *settings;

//E_STATUE_SUCCESS
static void recordStartCallback(long handle,int status,int error){
    settings->setRecordLedStatus(status);
    if(status == E_STATUS_FAILED){
        qDebug() << __FUNCTION__ << error;
    }
}
static void recordStopCallback(long handle,int status,int error){
    if(status == E_STATUE_SUCCESS){
        settings->setRecordLedStatus(E_STATUS_FAILED);
    }
    if(status == E_STATUS_FAILED){
        qDebug() << __FUNCTION__ << error;
    }
}

static void pushCallBack1(long handle,int status,int error){
    qDebug() << "<<<<<pushCallBack1:" << status;
    settings->setLiveStreamStatus1(status);
    settings->setLiveLedStatus(status);
    if(status == E_STATUS_FAILED){
        qDebug() << __FUNCTION__ << error;
    }
}

static void pushCallBack2(long handle,int status,int error){
    qDebug() << "<<<<<pushCallBack2:" << status;
    settings->setLiveStreamStatus2(status);
    settings->setLiveLedStatus(status);
    if(status == E_STATUS_FAILED){
        qDebug() << __FUNCTION__ << error;
    }
}

static void pushCallBack3(long handle,int status,int error){
    qDebug() << "<<<<<pushCallBack3:" << status;
    settings->setLiveStreamStatus3(status);
    settings->setLiveLedStatus(status);
    if(status == E_STATUS_FAILED){
        qDebug() << __FUNCTION__ << error;
    }
}

static void playCallback(int handle,int status,int error){
    qDebug() << "_________________-playCallback:" << status;
    if(status == E_STATUS_MP4_PLAY){
        qDebug() << "set_led ret:" << QSwitcher::set_led(KEY_LED_PLAYER_PLAY,SWITCHER_LED_ON);
    }else {
        qDebug() << "set_led ret:" << QSwitcher::set_led(KEY_LED_PLAYER_PLAY,SWITCHER_LED_OFF);
    }
    if(status == E_STATUS_FAILED){
        qDebug() << __FUNCTION__ << error;
    }
    settings->setPlayLedStatus(status);
}

static void playStartCallback(long handle,int status,int error){
    settings->setPlayLedStatus(status);
    if(status == E_STATUS_FAILED){
        qDebug() << __FUNCTION__ << error;
    }
}

static void playStopCallback(long handle,int status,int error){
    if(status == E_STATUE_SUCCESS){
        settings->setPlayLedStatus(E_STATUS_FAILED);
    }
    if(status == E_STATUS_FAILED){
        qDebug() << __FUNCTION__ << error;
    }
}

static void switchUsbCameraStartCallback(long handle,int status,int error){

}
static void switchUsbCameraStopCallback(long handle,int status,int error){

}

static void ndiReceiveStartCallback(long handle,int status,int error){

}
static void ndiReceiveStopCallback(long handle,int status,int error){

}

//typedef void (*rk_switch_cb)(long handle, int status, int error);
#endif // AUXCALLBACK_H
