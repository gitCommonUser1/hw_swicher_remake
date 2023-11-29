#include "colorbacks.h"
#include "colorback.h"

ColorBacks::ColorBacks(QObject *parent) : QObject(parent)
{
    m_colorBack1 = new ColorBack(COLOR1,this);
    m_colorBack2 = new ColorBack(COLOR2,this);
    m_colorBack1->setObjectName("colorBack1");
    m_colorBack2->setObjectName("colorBack2");
    colorBack1()->setHue(0);
    colorBack1()->setSaturation(0);
    colorBack1()->setBrightness(100);
    colorBack2()->setHue(45);
    colorBack2()->setSaturation(100);
    colorBack2()->setBrightness(100);
}
