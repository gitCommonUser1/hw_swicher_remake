#ifndef RUNNER_H
#define RUNNER_H
#include "macroworker.h"
#include <QVariantMap>

class MacroRunner :public MacroWorker{
    Q_OBJECT
public:
    using MacroWorker::MacroWorker;
    void startRun(Macro *macro);
    void stopRun();

    QThread *getThread();

public slots:
    void doWork();

signals:
    void runMethod(QVariantMap method);
};

#endif // RUNNER_H
