#include "stills.h"
#include "still.h"

#define STILL_MAX 32

Stills::Stills(QObject *parent) : QObject(parent)
{
    connect(this,&Stills::stillsChanged,this,[=](QList<QObject*> stills){
        std::sort(m_stills.begin(),m_stills.end(),Stills::listCompare);
    });
}

void Stills::append(QObject *still)
{
    auto item = qobject_cast<Still*>(still);
    if(item == nullptr)
        return ;
    if(item->index() < 0 || item->index() >= STILL_MAX)
        return ;

    auto exists = stillIndex(item->index());
    if(stillIndex(item->index()) != nullptr)
    {
        //已经存在
        if(m_stills.removeOne(exists))
            delete exists;
        m_stills.append(still);
    }
    else
    {
        //不存在
        m_stills.append(still);
    }
    stillsChanged(m_stills);
}

QObject *Stills::stillIndex(int index)
{
    for(int i = 0;i < m_stills.size();++i)
    {
        auto still = qobject_cast<Still*>(m_stills[i]);
        if(still->index() == index)
            return still;
    }

    return nullptr;
}

bool Stills::listCompare(QObject *src, QObject *dst)
{
    auto macroSrc = qobject_cast<Still*>(src);
    auto macroDst = qobject_cast<Still*>(dst);
    if(macroSrc->index() < macroDst->index())
        return true;
    else
        return false;
}

QObject *Stills::getDynamicChildrenClassName()
{
    Still *still = new Still(this);
    m_stills.push_back(still);
    return still;
}
