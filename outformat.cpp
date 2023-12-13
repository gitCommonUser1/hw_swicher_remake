#include "outformat.h"
#include "format.h"
#include "outputcolorspace.h"
#include "srcselections.h"

static QList<QString> outFormatList = {
    "1080p24",
    "1080p25",
    "1080p30",
    "1080p50",
    "1080p60"
};

static QList<QString> outputList = {
    "output 1",
    "output 2"
};

OutFormat::OutFormat(QObject *parent) : QObject(parent)
{
    m_format = new Format(this);
    m_format->setOutFormat(OUT_1080p60);
    m_format->setObjectName("format");

    m_out1ColorSpace = new OutputColorSpace(OUTPUT1,this);
    m_out1ColorSpace->setObjectName("out1ColorSpace");
    m_out1ColorSpace->setOutputIndex(SrcSelections::AUTO);

    m_out2ColorSpace = new OutputColorSpace(OUTPUT2,this);
    m_out2ColorSpace->setObjectName("out2ColorSpace");
    m_out2ColorSpace->setOutputIndex(SrcSelections::AUTO);
}

int OutFormat::outFormatStringToIndex(QString outFormat)
{
    for(int i = 0;i < outFormatList.size();++i)
    {
        if(outFormat.compare(outFormatList[i],Qt::CaseInsensitive) == 0)
        {
            return i;
        }
    }
    return 0;
}

QString OutFormat::outFormatIndexToString(int index)
{
    if(index < 0)
        index = 0;
    if(index >= outFormatList.size())
        index = outFormatList.size() - 1;

    return outFormatList[index];
}

int OutFormat::outputStringToIndex(QString output)
{
    for(int i = 0;i < outputList.size();++i)
    {
        if(output.compare(outputList[i],Qt::CaseInsensitive) == 0)
            return i;
    }
    return 0;
}

QString OutFormat::outputIndexToString(int index)
{
    if(index < 0)
        index = 0;
    if(index >= outputList.size())
        index = outputList.size() - 1;

    return outputList[index];
}
