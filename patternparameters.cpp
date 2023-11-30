#include "patternparameters.h"
#include "keys.h"

PatternParameters::PatternParameters(QObject *parent) : QObject(parent)
{
    m_fillSource_min = 0;
    m_fillSource_max = Keys::MAX;
    setFillSource(Keys::STILL2);
    m_pattern_min = 0;
    m_pattern_max = PATTERN_MAX;
    setPattern(0);
    m_wipeSize_min = 0;
    m_wipeSize_max = 100;
    setWipeSize(100);
    m_wipeXPosition_min = -16.0;
    m_wipeXPosition_max = 16.0;
    setWipeXPosition(0);
    m_wipeYPosition_min = -9.0;
    m_wipeYPosition_max = 9.0;
    setWipeYPosition(0);
    m_wipeSymmetry_min = 0;
    m_wipeSymmetry_max = 100;
    setWipeSymmetry(50);
    m_wipeSoftness_min = 0;
    m_wipeSoftness_max = 100;
    setWipeSoftness(0);
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
    setResize(false);
    setSize(Keys::sizeIndexToString(1));
    m_xPosition_min = -16.0;
    m_xPosition_max = 16.0;
    setXPosition(10.60);
    m_yPosition_min = -9.0;
    m_yPosition_max = 9.0;
    setYPosition(6.00);
}
