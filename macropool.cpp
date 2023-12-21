#include "macropool.h"
#include "macro.h"

MacroPool::MacroPool(QObject *parent) : QObject(parent)
{
    connect(this,&MacroPool::macrosChanged,this,[=](QList<QObject*> macros){
        std::sort(m_macros.begin(),m_macros.end(),MacroPool::listCompare);
    });
}

void MacroPool::checkMacro()
{
    for(int i = 0;i < m_macros.size();++i)
    {
        auto macro = qobject_cast<Macro*>(m_macros[i]);
        if(macro->ops().size() != 0){
            emit newMacro(macro->index());
        }
    }
}

bool MacroPool::listCompare(QObject *src, QObject *dst)
{
    auto macroSrc = qobject_cast<Macro*>(src);
    auto macroDst = qobject_cast<Macro*>(dst);
    if(macroSrc->index() < macroDst->index())
        return true;
    else
        return false;
}

bool MacroPool::isMacroIndexExists(int index)
{
    for(int i = 0;i < m_macros.size();++i)
    {
        if(qobject_cast<Macro*>(m_macros[i])->index() == index)
            return true;
    }
    return false;
}

Macro *MacroPool::macroIndex(int index)
{
    for(int i = 0;i < m_macros.size();++i)
    {
        if(qobject_cast<Macro*>(m_macros[i])->index() == index)
            return qobject_cast<Macro*>(m_macros[i]);
    }
    return nullptr;
}

void MacroPool::append(Macro *macro)
{
    if(macro == nullptr)
        return ;
    if(macro->ops().size() == 0)
    {
        deleteMacro(qobject_cast<Macro*>(macro));
        return ;
    }

    m_macros.append(macro);
    emit macrosChanged(m_macros);
    emit newMacro(macro->index());
}

void MacroPool::remove(int index)
{
    for(int i = 0;i < m_macros.size();++i)
    {
        if(qobject_cast<Macro*>(m_macros[i])->index() == index)
        {
            deleteMacro(qobject_cast<Macro*>(m_macros[i]));
            m_macros.removeAt(i);
            emit removeMacro(index);
            break;
        }
    }
    emit macrosChanged(m_macros);
}

void MacroPool::move(int src, int dst)
{
    bool src_exists = false;
    bool dst_exists = false;
    Macro *srcMacro = nullptr;
    for(int i = 0;i < m_macros.size();++i)
    {
        if(src_exists && dst_exists)
            break;
        if(qobject_cast<Macro*>(m_macros[i])->index() == src)
            src_exists = true,srcMacro = qobject_cast<Macro*>(m_macros[i]);
        if(qobject_cast<Macro*>(m_macros[i])->index() == dst)
            dst_exists = true;
    }

    if(src_exists && dst_exists)
    {
        swap(src,dst);
    }
    if(src_exists && !dst_exists)
    {
        srcMacro->setIndex(dst);
        emit moveMacro(src,dst);
    }
    emit macrosChanged(m_macros);
}

void MacroPool::swap(int src, int dst)
{
    bool src_exists = false;
    bool dst_exists = false;
    Macro *srcMacro = nullptr;
    Macro *dstMacro = nullptr;
    for(int i = 0;i < m_macros.size();++i)
    {
        if(src_exists && dst_exists)
            break;
        if(qobject_cast<Macro*>(m_macros[i])->index() == src)
            src_exists = true,srcMacro = qobject_cast<Macro*>(m_macros[i]);
        if(qobject_cast<Macro*>(m_macros[i])->index() == dst)
            dst_exists = true,dstMacro = qobject_cast<Macro*>(m_macros[i]);
    }
    if(src_exists && dst_exists)
    {
        srcMacro->setIndex(dst);
        dstMacro->setIndex(src);
    }
    emit macrosChanged(m_macros);
}

void MacroPool::deleteMacro(Macro *macro)
{
    if(macro == nullptr)
        return ;

    auto list = macro->ops();
    for(int i = 0;i < list.size(); ++i)
    {
        delete list[i];
    }
    delete macro;
}

QObject *MacroPool::getDynamicChildrenClassName()
{
    Macro *macro = new Macro(this);
    m_macros.push_back(macro);
    return macro;
}
