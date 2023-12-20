#include "streams.h"
#include "stream.h"
#include <QFile>
#include <QDebug>
#include <QXmlStreamReader>

static QList<QString> qualityList = {
    "High",
    "Medium",
    "Low"
};

static QList<QString> srcList = {
    "Recording",
    "Streaming"
};

Streams::Streams(QObject *parent) : QObject(parent)
{
    m_enable = false;
    m_stream1 = new Stream(0,this);
    m_stream1->setObjectName("stream1");
    m_stream1->setOutput(true);
    m_stream2 = new Stream(1,this);
    m_stream2->setObjectName("stream2");
    m_stream2->setOutput(false);
    m_stream3 = new Stream(2,this);
    m_stream3->setObjectName("stream3");
    m_stream3->setOutput(false);

    m_quality = LOW;
}

int Streams::srcStringToIndex(QString src)
{
    for(int i = 0;i < srcList.size();++i)
    {
        if(src.compare(srcList[i],Qt::CaseInsensitive) == 0)
            return i;
    }
    return 0;
}

QString Streams::srcIndexToString(int index)
{
    if(index < 0)
        index = 0;
    if(index >= srcList.size() )
        index = srcList.size() - 1;

    return srcList[index];
}

int Streams::qualityStringToIndex(QString quality)
{
    for(int i = 0;i < qualityList.size();++i)
    {
        if(quality.compare(qualityList[i],Qt::CaseInsensitive) == 0)
            return i;
    }
    return 0;
}

QString Streams::qualityIndexToString(int index)
{
    if(index < 0)
        index = 0;
    if(index >= qualityList.size() )
        index = qualityList.size() - 1;

    return qualityList[index];
}
