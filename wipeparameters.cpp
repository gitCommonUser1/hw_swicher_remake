#include "wipeparameters.h"
#include "transitionstyle.h"

WipeParameters::WipeParameters(QObject *parent) : QObject(parent)
{
    setRate(1.0);
    setPattern(0);
    setXPosition(0);
    setYPosition(0);
    setReverseDirection(false);
    setSymmetry(50);
    setSoftness(0);
    setBorder(0);
    setFillSource(TransitionStyle::COLOR1);
}
