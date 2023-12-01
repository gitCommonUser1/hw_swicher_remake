#include "supersourceborder.h"

SuperSourceBorder::SuperSourceBorder(int index,QObject *parent) : QObject(parent)
{
    setIndex(index);

    m_width_max = 10;
    m_width_min = 0;
    m_colorHue_max = 359;
    m_colorHue_min = 0;
    m_colorSaturation_max = 100;
    m_colorSaturation_min = 0;
    m_colorBrightness_max = 100;
    m_colorBrightness_min = 0;
}
