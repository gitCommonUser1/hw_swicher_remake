#include "srcselections.h"
#include "srcselection.h"

static QList<QString> srcSelectionList = {
    "Auto",
    "RGB Full",
    "RGB Limit",
    "YCbCr422 Full",
    "YCbCr422 Limit",
    "YCbCr444 Full",
    "YCbCr444 Limit"
};

static QList<QString> auxSelectionList = {
    "SD Card",
    "USB Camera",
    "NDI"
};

static QList<QString> sourceList = {
    "In 1",
    "In 2",
    "In 3",
    "In 4",
    "Aux"
};

SrcSelections::SrcSelections(QObject *parent) : QObject(parent)
{
    m_in1 = new SrcSelection(sourceList[IN1],this);
    m_in1->setObjectName("in1");
    m_in2 = new SrcSelection(sourceList[IN2],this);
    m_in2->setObjectName("in2");
    m_in3 = new SrcSelection(sourceList[IN3],this);
    m_in3->setObjectName("in3");
    m_in4 = new SrcSelection(sourceList[IN4],this);
    m_in4->setObjectName("in4");
    m_aux = new SrcSelection(sourceList[AUX],this);
    m_aux->setObjectName("aux");
}

int SrcSelections::sourceStringToIndex(QString source)
{
    for(int i = 0;i < sourceList.size();++i)
    {
        if(source.compare(sourceList[i],Qt::CaseInsensitive) == 0)
            return i;
    }
    return 0;
}

QString SrcSelections::sourceIndexToString(int index)
{
    if(index < 0)
        index = 0;
    if(index >= sourceList.size())
        index = sourceList.size() - 1;

    return sourceList[index];
}

int SrcSelections::selectionStringToIndex(QString selection)
{
    for(int i = 0;i < srcSelectionList.size();++i)
    {
        if(selection.compare(srcSelectionList[i],Qt::CaseInsensitive) == 0)
            return i;
    }
    for(int i = 0;i < auxSelectionList.size();++i)
    {
        if(selection.compare(auxSelectionList[i],Qt::CaseInsensitive) == 0)
            return i;
    }
    return 0;
}

QString SrcSelections::selectionIndexToString(int index)
{
    if(index < 0)
        index = 0;
    if(index >= srcSelectionList.size())
        index = srcSelectionList.size() - 1;

    return srcSelectionList[index];
}

QString SrcSelections::auxIndexToString(int index)
{
    if(index < 0)
        index = 0;
    if(index >= auxSelectionList.size())
        index = auxSelectionList.size() - 1;

    return auxSelectionList[index];
}
