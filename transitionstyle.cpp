#include "transitionstyle.h"
#include "mixparameters.h"
#include "dipparameters.h"
#include "wipeparameters.h"

static QList<QString>styleList = {
    "Mix",
    "Dip",
    "Wipe"
};


TransitionStyle::TransitionStyle(QObject *parent) : QObject(parent)
{
    m_mixParameters = new MixParameters(this);
    m_mixParameters->setObjectName("mixParameters");
    m_dipParameters = new DipParameters(this);
    m_dipParameters->setObjectName("dipParameters");
    m_wipeParameters = new WipeParameters(this);
    m_wipeParameters->setObjectName("wipeParameters");


    setStyle(styleIndexToString(0));
    setNextStyle(styleIndexToString(0));
    setPreviewTransition(false);
}

QString TransitionStyle::styleIndexToString(int index)
{
    if(index > 0 && index < styleList.size())
        return styleList[index];
    else
        return styleList[0];
}

int TransitionStyle::styleStringToIndex(QString style)
{
    for(int i = 0;i < styleList.size();++i)
    {
        if(styleList[i] == style)
        {
            return i;
        }
    }
    return 0;
}
