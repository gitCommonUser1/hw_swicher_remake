#include "patternparameters.h"
#include "keys.h"

PatternParameters::PatternParameters(QObject *parent) : QObject(parent)
{
    setFillSource(Keys::STILL2);
    setPattern(0);
    setWipeSize(100);
    setXPosition(0);
    setYPosition(0);
    setWipeSymmetry(50);
    setWipeSoftness(0);
    setMaskEnable(false);
    setMaskHStart(0);
    setMaskVStart(0);
    setMaskHEnd(100);
    setMaskVEnd(100);
    setResize(false);
    setSize(Keys::sizeIndexToString(1));
    setXPosition(10.60);
    setYPosition(6.00);
}
