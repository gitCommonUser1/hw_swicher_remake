#include "setting.h"
#include "srcnames.h"
#include "mvmeters.h"

Setting::Setting(QObject *parent) : QObject(parent)
{
    m_srcNames = new SrcNames(this);
    m_srcNames->setObjectName("srcNames");

    m_mvMeters = new MvMeters(this);
    m_mvMeters->setObjectName("mvMeters");
}
