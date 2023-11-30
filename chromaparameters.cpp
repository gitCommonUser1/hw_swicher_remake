#include "chromaparameters.h"
#include "keys.h"

ChromaParameters::ChromaParameters(QObject *parent) : QObject(parent)
{
    m_fillSource_min = 0;
    m_fillSource_max = Keys::MAX;
    setFillSource(Keys::STILL2);
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
    m_smpXPosition_min = -16.0;
    m_smpXPosition_max = 16.0;
    setSmpXPosition(-16.00);
    m_smpYPosition_min = -9.0;
    m_smpYPosition_max = 9.0;
    setSmpYPosition(-9.00);
    setSample(false);
    m_foreground_min = 0;
    m_foreground_max = 100;
    setForeground(0);
    m_background_min = 0;
    m_background_max = 100;
    setBackground(0);
    m_keyEdge_min = 0;
    m_keyEdge_max = 100;
    setKeyEdge(0);
    setRed(0);
    setGreen(0);
    setBlue(0);
    setY(0);
    setCb(0);
    setCr(0);
}
