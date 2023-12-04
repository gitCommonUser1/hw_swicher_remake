#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QObject>

class AudioSource;
class AudioInput : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AudioSource* mic1 READ mic1)
    Q_PROPERTY(AudioSource* mic2 READ mic2)
    Q_PROPERTY(AudioSource* in1 READ in1)
    Q_PROPERTY(AudioSource* in2 READ in2)
    Q_PROPERTY(AudioSource* in3 READ in3)
    Q_PROPERTY(AudioSource* in4 READ in4)
    Q_PROPERTY(AudioSource* aux READ aux)
public:
    explicit AudioInput(QObject *parent = nullptr);

    AudioSource* mic1() const
    {
        return m_mic1;
    }

    AudioSource* mic2() const
    {
        return m_mic2;
    }

    AudioSource* in1() const
    {
        return m_in1;
    }

    AudioSource* in2() const
    {
        return m_in2;
    }

    AudioSource* in3() const
    {
        return m_in3;
    }

    AudioSource* in4() const
    {
        return m_in4;
    }

    AudioSource* aux() const
    {
        return m_aux;
    }

private:


    AudioSource* m_mic1;

    AudioSource* m_mic2;

    AudioSource* m_in1;

    AudioSource* m_in2;

    AudioSource* m_in3;

    AudioSource* m_in4;

    AudioSource* m_aux;

signals:

};

#endif // AUDIOINPUT_H
