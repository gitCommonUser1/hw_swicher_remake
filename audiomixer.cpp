#include "audiomixer.h"
#include "audioinput.h"
#include "audiooutput.h"

AudioMixer::AudioMixer(QObject *parent) : QObject(parent)
{
    m_audioInput = new AudioInput(this);
    m_audioInput->setObjectName("audioInput");
    m_audioOutput = new AudioOutput(this);
    m_audioOutput->setObjectName("audioOutput");
}
