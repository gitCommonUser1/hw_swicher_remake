#include "mvlayout.h"

static QList<QString> layoutList = {
    "PVW|PGM",
    "PGM|PVW"
};

MvLayout::MvLayout(QObject *parent) : QObject(parent)
{
    m_layout = PVW_PGM;
}

int MvLayout::mvLayoutStringToIndex(QString layout)
{
    for(int i = 0;i < layoutList.size();++i)
    {
        if(layout.compare(layoutList[i],Qt::CaseInsensitive) == 0)
        {
            return i;
        }
    }
    return 0;
}

QString MvLayout::mvLayoutIndexToString(int index)
{
    if(index < 0)
        index = 0;
    if(index >= layoutList.size())
        index = layoutList.size() - 1;

    return layoutList[index];
}
