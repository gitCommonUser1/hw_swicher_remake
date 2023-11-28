#include "dipparameters.h"
#include "transitionstyle.h"

DipParameters::DipParameters(QObject *parent) : QObject(parent)
{
    setRate(1.0);

    setInput(TransitionStyle::COLOR1);
}
