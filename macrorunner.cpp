#include "macrorunner.h"
#include <QThread>
#include <QDebug>
#include "macro.h"
#include "op.h"
void MacroRunner::startRun(Macro *item)
{
    if(!item || item->ops().size() == 0)
        return ;

    macro = item;
    m_isWorking = true;
    m_index = macro->index();
    getThread()->start();
    emit workStatusChanged(m_index,m_isWorking);
}

void MacroRunner::stopRun()
{
    auto thread = getThread();
//    thread->requestInterruption();
    thread->terminate();
    m_isWorking = false;
    emit workStatusChanged(m_index,m_isWorking);
}

QThread *MacroRunner::getThread()
{
    static QThread *thread = nullptr;
    if(!thread){
        thread = new QThread;
        moveToThread(thread);
        connect(thread,&QThread::started,this,&MacroRunner::doWork);
    }
    return thread;
}

void MacroRunner::doWork()
{
    //
    qDebug() << "runner start!!" << "   id:" << m_index;
    auto list = macro->ops();
    int i = 0;
    qDebug() << "________macro size :" << list.size();
    while(1){
        if(/*getThread()->currentThread()->isInterruptionRequested() || */i >= list.size()){    //   ||   execute over!
            m_isWorking = false;
            getThread()->quit();
            qDebug("runner over!!");
            emit workStatusChanged(m_index,m_isWorking);
            return;
        }
        auto op = qobject_cast<Op*>(list[i]);
        qDebug() << "macro  " << i << "  id:" << op->method()["id"];
        emit runMethod(op->method());
        i++;
        //space
        QThread::msleep(1);
    }
}
