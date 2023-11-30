#include "key.h"

static QList<QString> keyList = {
    "Luma",
    "Chroma",
    "Pattern",
    "PIP"
};

Key::Key(QObject *parent) : QObject(parent)
{
    setIndex(0);
    setType(typeIndexToString(0));
    setOnAir(false);
}

QString Key::typeIndexToString(int index)
{
    if(index < 0)
        index = 0;
    if(index >= keyList.size())
        index = keyList.size() - 1;

    return keyList[index];
}

int Key::typeStringToIndex(QString str)
{
    for(int i = 0;i < keyList.size();++i)
    {
        if(str == keyList[i])
            return i;
    }

    return 0;
}
