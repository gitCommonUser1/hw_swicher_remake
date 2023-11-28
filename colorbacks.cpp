#include "colorbacks.h"
#include "colorback.h"

ColorBacks::ColorBacks(QObject *parent) : QObject(parent)
{
    m_colorBack1 = new ColorBack(this);
    m_colorBack2 = new ColorBack(this);
    m_colorBack1->setObjectName("colorBack1");
    m_colorBack2->setObjectName("colorBack2");
    colorBack1()->setColorIndex(COLOR1);
    colorBack1()->setHue(0);
    colorBack1()->setSaturation(0);
    colorBack1()->setBrightness(100);

    colorBack2()->setColorIndex(COLOR2);
    colorBack2()->setHue(45);
    colorBack2()->setSaturation(100);
    colorBack2()->setBrightness(100);
}
