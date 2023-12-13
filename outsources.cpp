#include "outsources.h"
#include "outsource.h"

static QList<QString> srcList = {
    "HDMI 1",
    "HDMI 2",
    "UVC"
};

static QList<QString> sourceList = {
    "In 1",
    "In 2",
    "In 3",
    "In 4",
    "Aux",
    "PGM",
    "PVW",
    "MultiView"
};

OutSources::OutSources(QObject *parent) : QObject(parent)
{
    m_hdmi1 = new OutSource("HDMI 1",this);
    m_hdmi1->setObjectName("hdmi1");
    m_hdmi1->setSource(PGM);

    m_hdmi2 = new OutSource("HDMI 2",this);
    m_hdmi2->setObjectName("hdmi2");
    m_hdmi2->setSource(MULTIVIEW);

    m_uvc = new OutSource("UVC",this);
    m_uvc->setObjectName("uvc");
    m_uvc->setSource(PGM);
}

int OutSources::srcStringToIndex(QString src)
{
    for(int i = 0;i < srcList.size();++i)
    {
        if(src.compare(srcList[i],Qt::CaseInsensitive) == 0)
            return i;
    }
    return 0;
}

QString OutSources::srcIndexToString(int index)
{
    if(index < 0)
        index = 0;
    if(index >= srcList.size())
        index = srcList.size() - 1;
    return srcList[index];
}

int OutSources::sourceStringToIndex(QString source)
{
    for(int i = 0;i < sourceList.size();++i)
    {
        if(source.compare(sourceList[i],Qt::CaseInsensitive) == 0)
            return i;
    }
    return 0;
}

QString OutSources::sourceIndexToString(int index)
{
    if(index < 0)
        index = 0;
    if(index >= sourceList.size())
        index = sourceList.size() - 1;
    return sourceList[index];
}
