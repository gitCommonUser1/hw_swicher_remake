#ifndef WORKER_H
#define WORKER_H

#include <QObject>

class Macro;

class MacroWorker:public QObject{
    Q_OBJECT
public:
    explicit MacroWorker();
    bool isWorking(){
        return m_isWorking;
    }
    int index(){
        return m_index;
    }
    Macro* data(){
        return macro;
    }
protected:
    bool m_isWorking;
    int m_index;
    Macro *macro;

signals:
    void workStatusChanged(int index,bool isWorking);
};

#endif // WORKER_H
