#ifndef RECORDER_H
#define RECORDER_H

#include "macroworker.h"

class MacroRecorder :public MacroWorker{
    Q_OBJECT
public:
    using MacroWorker::MacroWorker;
    void startRecord(int index);
    void stopRecord();

    void append(QVariantMap item);
};



#endif // RECORDER_H
