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
    if(list.size() != 0)
    {
        //特殊情况，这一个和上一个op都是sleep
        auto last = qobject_cast<Op*>(list[list.size() - 1])->method();
        if(last["id"].toString().contains("sleep",Qt::CaseInsensitive) &&
           item["id"].toString().contains("sleep",Qt::CaseInsensitive) )
        {
            //2023.12.20.21.32
            int sleep = last["value"].toInt();
            macro->remove(list.size() - 1);
            item["value"] = item["value"].toInt() + sleep;
            Op* op = new Op();
            op->setMethod(item);
            macro->append(op);

            return ;
        }
    }

    for(int i = list.size() - 1;i >= 0; --i)
    {
        qDebug() << " this name: "<<item["id"];
        qDebug() << " op " << i << " name: "<<item["id"];
        if(item["id"].toString() == qobject_cast<Op*>(list[i])->method()["id"].toString())
        {
            qDebug() << "true!!!!";
            macro->remove(i);
            break;
        }
        if(item["id"].toString().contains("sleep",Qt::CaseInsensitive))
        {
            break;
        }
    }
    Op* op = new Op();
    op->setMethod(item);
    macro->append(op);
}
