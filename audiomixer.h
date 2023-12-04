#ifndef AUDIOMIXER_H
#define AUDIOMIXER_H

#include <QObject>
class AudioInput;
class AudioOutput;

class AudioMixer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AudioInput* audioInput READ audioInput)
    Q_PROPERTY(AudioOutput* audioOutput READ audioOutput)
public:
    explicit AudioMixer(QObject *parent = nullptr);

    AudioInput* audioInput() const
    {
        return m_audioInput;
    }

    AudioOutput* audioOutput() const
    {
        return m_audioOutput;
    }

private:

    AudioInput* m_audioInput;

    AudioOutput* m_audioOutput;

signals:

};

#endif // AUDIOMIXER_H
