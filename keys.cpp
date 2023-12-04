#include "keys.h"
#include "lumaparameters.h"
#include "chromaparameters.h"
#include "patternparameters.h"
#include "pipparameters.h"
#include "key.h"

static QList<QString> sizeList = {
    "0.25",
    "0.33",
    "0.50"
};

static QList<QString> keyList = {
    "LUMA",
    "CHROMA",
    "PATTERN",
    "PIP"
};

Keys::Keys(QObject *parent) : QObject(parent)
{
    m_key = new Key(this);
    m_key->setObjectName("key");
    m_lumaParameters = new LumaParameters(this);
    m_lumaParameters->setObjectName("lumaParameters");
    m_chromaParameters = new ChromaParameters(this);
    m_chromaParameters->setObjectName("chromaParameters");
    m_patternParameters = new PatternParameters(this);
    m_patternParameters->setObjectName("patternParameters");
    m_pIPParameters = new PIPParameters(this);
    m_pIPParameters->setObjectName("pIPParameters");
}

QString Keys::sizeIndexToString(int index)
{
    if(index < 0)
        index = 0;
    if(index >= sizeList.size())
        index = sizeList.size() - 1;

    return sizeList[index];
}

int Keys::sizeStringToIndex(QString size)
{
    for(int i = 0;i < sizeList.size();++i)
    {
        if(size.compare(sizeList[i],Qt::CaseInsensitive) == 0)
        {
            return i;
        }
    }
    return 0;
}

QString Keys::keyIndexToString(int index)
{
    if(index < 0)
        index = 0;
    if(index >= keyList.size())
        index = keyList.size() - 1;

    return keyList[index];
}

int Keys::keyStringToIndex(QString key)
{
    for(int i = 0;i < keyList.size();++i)
    {
        if(key.compare(keyList[i],Qt::CaseInsensitive) == 0)
        {
            return i;
        }
    }
    return 0;
}
