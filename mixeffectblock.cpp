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

    m_program = new Program(this);
    m_program->setObjectName("program");
    m_preview = new Preview(this);
    m_preview->setObjectName("preview");
    m_nextTransition = new NextTransition(this);
    m_nextTransition->setObjectName("nextTransition");
    m_transitionStyle = new TransitionStyle(this);
    m_transitionStyle->setObjectName("transitionStyle");
    m_keys = new Keys(this);
    m_keys->setObjectName("keys");
    m_ftb = new FadeToBlack(this);
    m_ftb->setObjectName("ftb");
}
