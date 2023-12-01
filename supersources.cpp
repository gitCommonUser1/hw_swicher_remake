#include "supersources.h"
#include "supersource.h"

SuperSources::SuperSources(QObject *parent) : QObject(parent)
{
    m_superSource = new SuperSource(this);
    m_superSource->setObjectName("superSource");
}
