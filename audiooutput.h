#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include <QObject>
class AudioSource;

class AudioOutput : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AudioSource* pgm READ pgm)
    Q_PROPERTY(AudioSource* monitor READ monitor)
public:
    explicit AudioOutput(QObject *parent = nullptr);

    AudioSource* pgm() const
    {
        return m_pgm;
    }

    AudioSource* monitor() const
    {
        return m_monitor;
    }

private:

    AudioSource* m_pgm;

    AudioSource* m_monitor;

};

#endif // AUDIOOUTPUT_H
