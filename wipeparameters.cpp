#include "wipeparameters.h"
#include "transitionstyle.h"

WipeParameters::WipeParameters(QObject *parent) : QObject(parent)
{
    m_rate_max = 8.0;
    m_rate_min = 0.5;
    setRate(1.0);
    m_pattern_min = 0;
    m_pattern_max = 17;
    setPattern(0);
    m_xPosition_min = dround(-16.0,2);
    m_xPosition_max = dround(16.0,2);
    setXPosition(0);
    m_yPosition_min = dround(-9.0,2);
    m_yPosition_max = dround(9.0,2);
    setYPosition(0);
    setReverseDirection(false);
    m_symmetry_min = 0;
    m_symmetry_max = 100;
    setSymmetry(50);
    m_softness_min = 0;
    m_softness_max = 100;
    setSoftness(0);
    m_border_min = 0;
    m_border_max = 100;
    setBorder(0);
    m_fillSource_min = 0;
    m_fillSource_max = TransitionStyle::MAX;
    setFillSource(TransitionStyle::COLOR1);
}
