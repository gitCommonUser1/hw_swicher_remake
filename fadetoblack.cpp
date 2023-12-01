#include "fadetoblack.h"

FadeToBlack::FadeToBlack(QObject *parent) : QObject(parent)
{
    m_rate_min = 0.5;
    m_rate_max = 8.0;
    setRate(1.00);
    setAfv(true);
}
