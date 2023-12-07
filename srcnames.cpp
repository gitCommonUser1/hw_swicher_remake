#include "srcnames.h"
#include "srcname.h"

static QList<QString> srcNameList = {
    "PGM",
    "PVW",
    "IN 1",
    "IN 2",
    "IN 3",
    "IN 4",
    "Aux",
    "Still 1",
    "Still 2"
};

SrcNames::SrcNames(QObject *parent) : QObject(parent)
{
    m_pgm = new SrcName("pgm",this);
    m_pgm->setObjectName("pgm");
    m_pgm->setName("Program");
    m_pvw = new SrcName("pvw",this);
    m_pvw->setObjectName("pvw");
    m_pvw->setName("Preview");
    m_in1 = new SrcName("in1",this);
    m_in1->setObjectName("in1");
    m_in1->setName("HDMI 1");
    m_in2 = new SrcName("in2",this);
    m_in2->setObjectName("in2");
    m_in2->setName("HDMI 2");
    m_in3 = new SrcName("in3",this);
    m_in3->setObjectName("in3");
    m_in3->setName("HDMI 3");
    m_in4 = new SrcName("in4",this);
    m_in4->setObjectName("in4");
    m_in4->setName("HDMI 4");
    m_aux = new SrcName("aux",this);
    m_aux->setObjectName("aux");
    m_aux->setName("Aux");
    m_still1 = new SrcName("still1",this);
    m_still1->setObjectName("still1");
    m_still1->setName("Still 1");
    m_still2 = new SrcName("still2",this);
    m_still2->setObjectName("still2");
    m_still2->setName("Still 2");

}

int SrcNames::srcNameStringToIndex(QString srcName)
{
    for(int i = 0;i < srcNameList.size();++i)
    {
        if(srcName.compare(srcNameList[i],Qt::CaseInsensitive) == 0)
            return i;
    }
    return 0;
}

QString SrcNames::srcNameIndexToString(int index)
{
    if(index < 0)
        index = 0;
    if(index >= srcNameList.size())
        index = srcNameList.size() - 1;

    return srcNameList[index];
}
