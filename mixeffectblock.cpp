#include "mixeffectblock.h"
#include "program.h"
#include "preview.h"
#include "nexttransition.h"
#include "transitionstyle.h"
#include "keys.h"
#include "fadetoblack.h"

MixEffectBlock::MixEffectBlock(QObject *parent) : QObject(parent)
{
    //当前仅一个MixEffectBlock
    setIndex(0);

    m_program = new Program;
    m_preview = new Preview;
    m_nextTransition = new NextTransition;
    m_transitionStyle = new TransitionStyle;
    m_keys = new Keys;
    m_ftb = new FadeToBlack;
}
