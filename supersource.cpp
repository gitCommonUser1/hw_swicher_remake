#include "supersource.h"
#include "supersourcemask.h"
#include "supersourceborder.h"
#include "keys.h"

//third->list_text << tr("Zoom in") << tr("Crop-Zoom in") << tr("Zoom in-Crop") << tr("Crop") ;
static QList<QString> styleList = {
    "Zoom in",
    "Crop-Zoom in",
    "Zoom in-Crop",
    "Crop"
};

SuperSource::SuperSource(QObject *parent) : QObject(parent)
{
    m_mask1 = new SuperSourceMask(MASK1,this);
    m_mask1->setObjectName("mask1");
    m_mask2 = new SuperSourceMask(MASK2,this);
    m_mask2->setObjectName("mask2");

    m_border1 = new SuperSourceBorder(BORDER1,this);
    m_border1->setObjectName("border1");
    m_border2 = new SuperSourceBorder(BORDER2,this);
    m_border2->setObjectName("border2");

    m_enable = false;
    m_source1_min = 0;
    m_source1_max = Keys::MAX;
    m_source1 = Keys::IN1;
    m_source2_min = 0;
    m_source2_max = Keys::MAX;
    m_source2 = Keys::IN2;
    m_background_min = 0;
    m_background_max = Keys::MAX;
    m_background = Keys::STILL1;
    m_yPosition_min = 0;
    m_yPosition_max = 100;
    m_yPosition = 50;

    m_style = styleIndexToString(0);
}

int SuperSource::styleStringToIndex(QString str)
{
    for(int i = 0;i < styleList.size();++i)
    {
        if(str.compare(styleList[i],Qt::CaseInsensitive) == 0)
            return i;
    }
    return 0;
}

QString SuperSource::styleIndexToString(int index)
{
    if(index < 0)
        index = 0;
    if(index >= styleList.size())
        index = styleList.size() - 1;

    return styleList[index];
}
