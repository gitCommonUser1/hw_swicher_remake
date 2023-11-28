#include "gostreamsystem.h"
#include "mixeffectblocks.h"
#include "colorbacks.h"
#include "downstreamkeys.h"
#include "device_info.h"
#include <QDebug>

#define PRODUCT_NAME "GoStream Deck"

Profile::Profile(QObject *parent) : QObject(parent)
{
    setProduct(PRODUCT_NAME);
    auto str = get_version();
    QString version = QString(str);
    QStringList versions = version.split(".");
    if(versions.size() == 3)
    {
        setMajorVersion(versions[0].toInt());
        setMinorVersion(versions[1].toInt());
        setPatchVersion(versions[2].toInt());
    }
    if(str)
        free(str);


    m_mixEffectBlocks = new MixEffectBlocks(this);
    m_mixEffectBlocks->setObjectName("mixEffectBlocks");
    m_colorBacks = new ColorBacks(this);
    m_colorBacks->setObjectName("colorBacks");
    m_downstreamKeys = new DownstreamKeys(this);
    m_downstreamKeys->setObjectName("downstreamKeys");
}
