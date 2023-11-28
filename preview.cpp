#include "preview.h"
#include "mixeffectblock.h"

Preview::Preview(QObject *parent) : QObject(parent)
{
    setInput(MixEffectBlock::IN1);
}
