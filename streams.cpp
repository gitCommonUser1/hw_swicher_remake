#include "streams.h"
#include "stream.h"
#include <QFile>
#include <QDebug>
#include <QXmlStreamReader>

Streams::Streams(QObject *parent) : QObject(parent)
{
    m_stream1 = new Stream(0,this);
    m_stream1->setObjectName("stream1");
    m_stream1->setOutput(true);
    m_stream2 = new Stream(1,this);
    m_stream2->setObjectName("stream2");
    m_stream2->setOutput(false);
    m_stream3 = new Stream(2,this);
    m_stream3->setObjectName("stream3");
    m_stream3->setOutput(false);
}
