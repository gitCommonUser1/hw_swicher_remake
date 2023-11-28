#include "transitionstyle.h"

static QList<QString>styleList = {
    "Mix",
    "Dip",
    "Wipe"
};


TransitionStyle::TransitionStyle(QObject *parent) : QObject(parent)
{
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
