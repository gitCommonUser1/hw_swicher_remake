#include "colorbacks.h"
#include "colorback.h"

ColorBacks::ColorBacks(QObject *parent) : QObject(parent)
{
    colorBack1()->setColorIndex(COLOR1);
    colorBack1()->setHue(0);
    colorBack1()->setSaturation(0);
    colorBack1()->setBrightness(100);

    colorBack2()->setColorIndex(COLOR2);
    colorBack2()->setHue(45);
    colorBack2()->setSaturation(100);
    colorBack2()->setBrightness(100);
}
