#include "lumaparameters.h"
#include "keys.h"

LumaParameters::LumaParameters(QObject *parent) : QObject(parent)
{
    setFillSource(Keys::STILL2);
    setKeySource(Keys::STILL2KEY);
    setMaskEnable(false);
    setMaskHStart(0);
    setMaskVStart(0);
    setMaskHEnd(100);
    setMaskVEnd(100);
    setShapedKey(false);
    setClip(15);
    setGain(50);
    setInvert(false);
    setResize(false);
    setSize(Keys::sizeIndexToString(1));
    setXPosition(10.60);
    setYPosition(6.00);
}
