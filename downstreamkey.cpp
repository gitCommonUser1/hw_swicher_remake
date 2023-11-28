#include "downstreamkey.h"
#include "keys.h"

DownstreamKey::DownstreamKey(QObject *parent) : QObject(parent)
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
    setRate(1.00);
}
