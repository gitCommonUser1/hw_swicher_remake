#include "mixeffectblock.h"
#include "program.h"
#include "preview.h"
#include "nexttransition.h"
#include "transitionstyle.h"
#include "keys.h"
#include "fadetoblack.h"

static QList<QString> inputList = {
    "in 1",
    "in 2",
    "in 3",
    "in 4",
    "aux",
    "black",
    "still 1",
    "still 2"
};

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

int MixEffectBlock::inputStringToIndex(QString str)
{
    for(int i = 0;i < inputList.size();++i)
    {
        if(str.compare(inputList[i],Qt::CaseInsensitive) == 0)
            return i;
    }
    return 0;
}

QString MixEffectBlock::inputIndexToString(int index)
{
    if(index >= inputList.size())
        index = inputList.size() - 1;
    if(index < 0)
        index = 0;

    return inputList[index];
}
