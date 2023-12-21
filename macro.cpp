#include "macro.h"
#include "op.h"

Macro::Macro(QObject *parent) : QObject(parent)
{

}

void Macro::append(Op *op)
{
    m_ops.append(op);
    emit opsChanged(m_ops);
}

void Macro::remove(int index)
{
    if(index < 0 || index >= m_ops.size())
        return ;

    delete m_ops[index];
    m_ops.removeAt(index);
}

QObject *Macro::getDynamicChildrenClassName()
{
    Op *op = new Op(this);
    m_ops.append(op);
    return op;
}
