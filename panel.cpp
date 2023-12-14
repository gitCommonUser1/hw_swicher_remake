#include "panel.h"

Panel::Panel(QObject *parent) : QObject(parent)
{
    m_ButtonBrightness = 7;
    m_ButtonBrightness_min = 0;
    m_ButtonBrightness_max = 15;
}
