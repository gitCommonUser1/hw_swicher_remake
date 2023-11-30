#include "lumaparameters.h"
#include "keys.h"

LumaParameters::LumaParameters(QObject *parent) : QObject(parent)
{
    m_fillSource_min = 0;
    m_fillSource_max = Keys::MAX;
    setFillSource(Keys::STILL2);
    m_keySource_min = 0;
    m_keySource_max = Keys::MAX;
    setKeySource(Keys::STILL2KEY);
    setMaskEnable(false);
    m_maskHStart_min = 0;
    m_maskHStart_max = 100;
    setMaskHStart(0);
    m_maskVStart_min = 0;
    m_maskVStart_max = 100;
    setMaskVStart(0);
    m_maskHEnd_min = 0;
    m_maskHEnd_max = 100;
    setMaskHEnd(100);
    m_maskVEnd_min = 0;
    m_maskVEnd_max = 100;
    setMaskVEnd(100);
    setShapedKey(false);
    m_clip_min = 0;
    m_clip_max = 100;
    setClip(15);
    m_gain_min = 0;
    m_gain_max = 100;
    setGain(50);
    setInvert(false);
    setResize(false);
    setSize(Keys::sizeIndexToString(1));
    m_xPosition_min = -16.0;
    m_xPosition_max = 16.0;
    setXPosition(10.60);
    m_yPosition_min = -9.0;
    m_yPosition_max = 9.0;
    setYPosition(6.00);
}
