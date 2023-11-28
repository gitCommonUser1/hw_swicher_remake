#include "colorback.h"

ColorBack::ColorBack(QObject *parent) : QObject(parent)
{
    m_hue_max = 359;
    m_hue_min = 0;
    m_saturation_max = 100;
    m_saturation_min = 0;
    m_brightness_max = 100;
    m_brightness_min = 0;
}
