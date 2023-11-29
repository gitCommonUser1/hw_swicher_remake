#include "dipparameters.h"
#include "transitionstyle.h"

DipParameters::DipParameters(QObject *parent) : QObject(parent)
{
    m_rate_max = 8.0;
    m_rate_min = 0.5;
    setRate(1.0);
    m_input_min = 0;
    m_input_max = TransitionStyle::MAX;

    setInput(TransitionStyle::COLOR1);
}
