#include "mixeffectblocks.h"
#include "mixeffectblock.h"

MixEffectBlocks::MixEffectBlocks(QObject *parent) : QObject(parent)
{
    m_mixEffectBlock = new MixEffectBlock;
}
