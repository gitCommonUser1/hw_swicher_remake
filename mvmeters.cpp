#include "mvmeters.h"
#include "mvmeter.h"

static QList<QString > mvMeterList{
    "PGM",
    "In 1",
    "In 2",
    "In 3",
    "In 4",
    "Aux"
};

MvMeters::MvMeters(QObject *parent) : QObject(parent)
{
    m_pgm = new MvMeter(mvMeterList[PGM],this);
    m_pgm->setObjectName("pgm");
    m_in1 = new MvMeter(mvMeterList[IN1],this);
    m_in1->setObjectName("in1");
    m_in2 = new MvMeter(mvMeterList[IN2],this);
    m_in2->setObjectName("in2");
    m_in3 = new MvMeter(mvMeterList[IN3],this);
    m_in3->setObjectName("in3");
    m_in4 = new MvMeter(mvMeterList[IN4],this);
    m_in4->setObjectName("in4");
    m_aux = new MvMeter(mvMeterList[AUX],this);
    m_aux->setObjectName("aux");
}

int MvMeters::mvMeterStringToIndex(QString src)
{
    for(int i = 0;i < mvMeterList.size();++i)
    {
        if(src.compare(mvMeterList[i],Qt::CaseInsensitive) == 0)
            return i;
    }
    return 0;
}

QString MvMeters::mvMeterIndexToString(int index)
{
    if(index < 0)
        index = 0;
    if(index >= mvMeterList.size())
        index = mvMeterList.size() - 1;

    return mvMeterList[index];
}
