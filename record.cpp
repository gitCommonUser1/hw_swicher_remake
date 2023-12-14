#include "record.h"

Record::Record(QObject *parent) : QObject(parent)
{
    m_quality = LOW;
}
