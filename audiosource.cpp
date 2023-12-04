#include "audiosource.h"

static QList<QString> nameList = {
    "mic1",
    "mic2",
    "in1",
    "in2",
    "in3",
    "in4",
    "aux",
    "pgm"
};

AudioSource::AudioSource(QObject *parent) : QObject(parent)
{
    m_fader_max = 10.0;
    m_fader_min = -75.0;
    m_fader = 0;
    m_balance_min = -40.0;
    m_balance_max = 40.0;
    m_balance = 0;
    m_input_min = -75.0;
    m_input_max = 6.0;
    m_input = 0;
    m_enable = false;
    m_delay_min = 0;
    m_delay_max = 170;
    m_delay = 0;
    m_level_min = -31;
    m_level_max = 0;
    m_level = -6;
    m_enable_min = 0;
    m_enable_max = AUDIO_ENABLE_MAX;
    m_enable = OFF;
    m_source_min = 0;
    m_source_max = MAX;
    m_source = PGM;
}

int AudioSource::sourceNameStringToIndex(QString name)
{
    for(int i = 0;i < nameList.size();++i)
    {
        if(name == nameList[i])
            return i;
    }

    return 0;
}

QString AudioSource::sourceNameIndexToString(int index)
{
    if(index >= nameList.size())
        index = nameList.size() -1;
    if(index < 0)
        index = 0;
    return nameList[index];
}
