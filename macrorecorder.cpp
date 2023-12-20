#include "macrorecorder.h"
#include <QDebug>
#include "macro.h"
#include "op.h"
#include "macropool.h"
#include "gostreamsystem.h"
#include "profile_include.h"

extern Profile *profile;

void MacroRecorder::startRecord(int index)
{
    if(m_isWorking)
        return ;

    if(profile->macroPool()->isMacroIndexExists(index))
        profile->macroPool()->remove(index);

    macro = new Macro;
    m_index = index;
    macro->setIndex(m_index);
    m_isWorking = true;
    emit workStatusChanged(m_index,m_isWorking);
}

void MacroRecorder::stopRecord()
{
    if(!m_isWorking)
        return ;

    m_isWorking = false;
    emit workStatusChanged(m_index,m_isWorking);
}

void MacroRecorder::append(QVariantMap item)
{
    if(!m_isWorking)
        return ;

    auto list = macro->ops();
    for(int i = list.size() - 1;i >= 0; --i)
    {
        qDebug() << " this name: "<<item["id"];
        qDebug() << " op " << i << " name: "<<item["id"];
        if(item["id"].toString() == qobject_cast<Op*>(macro->ops()[i])->method()["id"].toString())
        {
            qDebug() << "true!!!!";
            macro->remove(i);
            break;
        }
        if(item["id"].toString().indexOf("sleep",Qt::CaseInsensitive) != -1)
        {
            break;
        }
    }
    Op* op = new Op();
    op->setMethod(item);
    macro->append(op);
}
