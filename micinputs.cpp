#include "micinputs.h"
#include "micinput.h"

static QList<QString> micInputList = {
    "Mic+power",
    "Mic",
    "Line",
};

MicInputs::MicInputs(QObject *parent) : QObject(parent)
{
    m_mic1 = new MicInput(0,this);
    m_mic1->setObjectName("mic1");
    m_mic1->setInput(LINE);

    m_mic2 = new MicInput(1,this);
    m_mic2->setObjectName("mic2");
    m_mic2->setInput(LINE);
}

int MicInputs::micInputStringToIndex(QString input)
{
    for(int i = 0;i < micInputList.size();++i)
    {
        if(input.compare(micInputList[i],Qt::CaseInsensitive) == 0)
            return i;
    }
    return 0;
}

QString MicInputs::micInputIndexToString(int index)
{
    if(index < 0)
        index = 0 ;
    if(index >= micInputList.size())
        index = micInputList.size() - 1;

    return micInputList[index];
}

int MicInputs::srcNameToIndex(QString srcName)
{
    if(srcName.compare("mic1",Qt::CaseInsensitive) == 0)
        return MIC1;
    else if(srcName.compare("mic2",Qt::CaseInsensitive) == 0)
        return MIC2;
    else
        return MIC1;
}
