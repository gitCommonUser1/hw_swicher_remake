#include "supersourcemask.h"

SuperSourceMask::SuperSourceMask(int index,QObject *parent) : QObject(parent)
{
    setIndex(index);

    m_hStart_max = 100;
    m_hStart_min = 0;
    m_vStart_max = 100;
    m_vStart_min = 0;
    m_hEnd_max = 100;
    m_hEnd_min = 0;
    m_vEnd_max = 100;
    m_vEnd_min = 0;
}
