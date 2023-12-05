#include "stillselection.h"

StillSelection::StillSelection(QObject *parent) : QObject(parent)
{
    m_still1_min = 0;
    m_still1_max = 31;
    m_still1 = 0;

    m_still2_min = 0;
    m_still2_max = 31;
    m_still2 = 1;
}
