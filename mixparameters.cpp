#include "mixparameters.h"

MixParameters::MixParameters(QObject *parent) : QObject(parent)
{
    m_rate_max = 8.0;
    m_rate_min = 0.5;
    setRate(1.0);
}
