#include "pipparameters.h"
#include "keys.h"

PIPParameters::PIPParameters(QObject *parent) : QObject(parent)
{
    m_fillSource_min = 0;
    m_fillSource_max = Keys::MAX;
    setFillSource(Keys::IN1);
    setSize(Keys::sizeIndexToString(1));
    m_xPosition_min = -16.0;
    m_xPosition_max = 16.0;
    setXPosition(10.60);
    m_yPosition_min = -9.0;
    m_yPosition_max = 9.0;
    setYPosition(6.00);
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
    setBorderEnable(false);
    m_borderWidth_min = 0;
    m_borderWidth_max = 31;
    setBorderWidth(0);
    m_borderColorHue_min = 0;
    m_borderColorHue_max = 359;
    setBorderColorHue(0);
    m_borderColorBrightness_min = 0;
    m_borderColorBrightness_max = 100;
    setBorderColorBrightness(100);
    m_borderColorSaturation_min = 0;
    m_borderColorSaturation_max = 100;
    setBorderColorSaturation(100);
}
