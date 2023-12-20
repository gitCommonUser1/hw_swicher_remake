#include "macropool.h"
#include "macro.h"

MacroPool::MacroPool(QObject *parent) : QObject(parent)
{

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
