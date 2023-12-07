#include "setting.h"
#include "srcnames.h"

Setting::Setting(QObject *parent) : QObject(parent)
{
    m_srcNmaes = new SrcNames(this);
    m_srcNmaes->setObjectName("srcNames");
}
