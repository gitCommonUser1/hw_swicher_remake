#include "program.h"
#include "mixeffectblock.h"

Program::Program(QObject *parent) : QObject(parent)
{
    setInput(MixEffectBlock::IN1);
}
