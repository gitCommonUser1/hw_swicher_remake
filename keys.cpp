#include "keys.h"

static QList<QString> sizeList = {
    "0.25",
    "0.33",
    "0.50"
};

Keys::Keys(QObject *parent) : QObject(parent)
{

}

QString Keys::sizeIndexToString(int index)
{
    if(index >= 0 && index < sizeList.size())
        return sizeList[index];
    else
        return sizeList[0];
}

int Keys::sizeStringToIndex(QString size)
{
    for(int i = 0;i < sizeList.size();++i)
    {
        if(sizeList[i] == size)
        {
            return i;
        }
    }
    return 0;
}
