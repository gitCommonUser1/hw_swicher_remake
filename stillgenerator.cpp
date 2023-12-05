#include "stillgenerator.h"
#include "stillselection.h"
#include "stills.h"

StillGenerator::StillGenerator(QObject *parent) : QObject(parent)
{
    m_stills = new Stills(this);
    m_stills->setObjectName("stills");
    m_stillSelection = new StillSelection(this);
    m_stillSelection->setObjectName("stillSelection");
}
