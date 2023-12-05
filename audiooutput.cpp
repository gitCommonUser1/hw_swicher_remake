#include "audiooutput.h"
#include "audiosource.h"

AudioOutput::AudioOutput(QObject *parent) : QObject(parent)
{
    QObject *obj = nullptr;
    m_pgm = new AudioSource(this);
    m_pgm->setObjectName("pgm");
    m_pgm->setName(AudioSource::sourceNameIndexToString(AudioSource::PGM));
    obj = new QObject(m_pgm);
    obj->setObjectName("balance");
    obj = new QObject(m_pgm);
    obj->setObjectName("input");
    obj = new QObject(m_pgm);
    obj->setObjectName("enable");
    obj = new QObject(m_pgm);
    obj->setObjectName("delay");
    obj = new QObject(m_pgm);
    obj->setObjectName("level");
    obj = new QObject(m_pgm);
    obj->setObjectName("source");
    m_monitor = new AudioSource(this);
    m_monitor->setObjectName("monitor");
    m_monitor->setName("monitor");
    obj = new QObject(m_monitor);
    obj->setObjectName("fader");
    obj = new QObject(m_monitor);
    obj->setObjectName("balance");
    obj = new QObject(m_monitor);
    obj->setObjectName("input");
    obj = new QObject(m_monitor);
    obj->setObjectName("enable");
    obj = new QObject(m_monitor);
    obj->setObjectName("delay");
}
