#ifndef AUDIOMIXER_H
#define AUDIOMIXER_H

#include <QObject>

class AudioMixer : public QObject
{
    Q_OBJECT
public:
    explicit AudioMixer(QObject *parent = nullptr);

signals:

};

#endif // AUDIOMIXER_H
