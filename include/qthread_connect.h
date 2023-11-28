#ifndef _QTHREAD_CONNETC_H
#define _QTHREAD_CONNETC_H

#include <QThread>

#define TSL50_BUF_SIZE (65535 + 2)//16bit PBC

class QThreadConnect : public QThread
{
    Q_OBJECT

public:
    QThreadConnect(QObject *parent = 0);
    ~QThreadConnect();

    void run();
};


extern int g_connect_epollfd;

#endif // _QTHREAD_CONNETC_H
