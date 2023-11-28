#include "pipparameters.h"
#include "keys.h"

PIPParameters::PIPParameters(QObject *parent) : QObject(parent)
{
    setFillSource(Keys::IN1);
    setSize(Keys::sizeIndexToString(1));
    setXPosition(10.60);
    setYPosition(6.00);
    setMaskEnable(false);
    setMaskHStart(0);
    setMaskVStart(0);
    setMaskHEnd(100);
    setMaskVEnd(100);
    setBorderEnable(false);
    setBorderWidth(0);
    setBorderColorHue(0);
    setBorderColorBrightness(100);
    setBorderColorSaturation(100);
}
