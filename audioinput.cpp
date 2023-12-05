#include "audioinput.h"
#include "audiosource.h"

AudioInput::AudioInput(QObject *parent) : QObject(parent)
{
    QObject *obj = nullptr;
    m_mic1 = new AudioSource(this);
    m_mic1->setObjectName("mic1");
    m_mic1->setName(AudioSource::sourceNameIndexToString(AudioSource::MIC1));
    obj = new QObject(m_mic1);
    obj->setObjectName("level");
    obj = new QObject(m_mic1);
    obj->setObjectName("source");
    m_mic2 = new AudioSource(this);
    m_mic2->setObjectName("mic2");
    m_mic2->setName(AudioSource::sourceNameIndexToString(AudioSource::MIC2));
    obj = new QObject(m_mic2);
    obj->setObjectName("level");
    obj = new QObject(m_mic2);
    obj->setObjectName("source");
    m_in1 = new AudioSource(this);
    m_in1->setObjectName("in1");
    m_in1->setName(AudioSource::sourceNameIndexToString(AudioSource::IN1));
    obj = new QObject(m_in1);
    obj->setObjectName("delay");
    obj = new QObject(m_in1);
    obj->setObjectName("level");
    obj = new QObject(m_in1);
    obj->setObjectName("source");
    m_in2 = new AudioSource(this);
    m_in2->setObjectName("in2");
    m_in2->setName(AudioSource::sourceNameIndexToString(AudioSource::IN2));
    obj = new QObject(m_in2);
    obj->setObjectName("delay");
    obj = new QObject(m_in2);
    obj->setObjectName("level");
    obj = new QObject(m_in2);
    obj->setObjectName("source");
    m_in3 = new AudioSource(this);
    m_in3->setObjectName("in3");
    m_in3->setName(AudioSource::sourceNameIndexToString(AudioSource::IN3));
    obj = new QObject(m_in3);
    obj->setObjectName("delay");
    obj = new QObject(m_in3);
    obj->setObjectName("level");
    obj = new QObject(m_in3);
    obj->setObjectName("source");
    m_in4 = new AudioSource(this);
    m_in4->setObjectName("in4");
    m_in4->setName(AudioSource::sourceNameIndexToString(AudioSource::IN4));
    obj = new QObject(m_in4);
    obj->setObjectName("delay");
    obj = new QObject(m_in4);
    obj->setObjectName("level");
    obj = new QObject(m_in4);
    obj->setObjectName("source");
    m_aux = new AudioSource(this);
    m_aux->setObjectName("aux");
    m_aux->setName(AudioSource::sourceNameIndexToString(AudioSource::AUX));
    obj = new QObject(m_aux);
    obj->setObjectName("delay");
    obj = new QObject(m_aux);
    obj->setObjectName("level");
    obj = new QObject(m_aux);
    obj->setObjectName("source");
}
