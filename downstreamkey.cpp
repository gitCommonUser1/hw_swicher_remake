#include "downstreamkey.h"
#include "keys.h"

DownstreamKey::DownstreamKey(QObject *parent) : QObject(parent)
{
    setIndex(0);
    setOnAir(false);
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
    m_rate_min = 0.5;
    m_rate_max = 8.0;
    setRate(1.00);
}
