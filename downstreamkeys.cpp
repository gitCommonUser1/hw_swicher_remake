#include "downstreamkeys.h"
#include "downstreamkey.h"

DownstreamKeys::DownstreamKeys(QObject *parent) : QObject(parent)
{
    m_downstreamKey = new DownstreamKey(this);
    m_downstreamKey->setObjectName("downstreamKey");
}
