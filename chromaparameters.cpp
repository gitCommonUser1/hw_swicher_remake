#include "chromaparameters.h"
#include "keys.h"

ChromaParameters::ChromaParameters(QObject *parent) : QObject(parent)
{
    setFillSource(Keys::STILL2);
    setMaskEnable(false);
    setMaskHStart(0);
    setMaskVStart(0);
    setMaskHEnd(100);
    setMaskVEnd(100);
    setResize(false);
    setSize(Keys::sizeIndexToString(1));
    setXPosition(10.60);
    setYPosition(6.00);
    setSmpXPosition(-16.00);
    setSmpYPosition(-9.00);
    setSample(false);
    setForeground(0);
    setBackground(0);
    setKeyEdge(0);
    setRed(0);
    setGreen(0);
    setBlue(0);
}
