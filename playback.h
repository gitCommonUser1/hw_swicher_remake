#ifndef PLAYBACK_H
#define PLAYBACK_H

#include <QObject>

class Playback : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int playbackList READ playbackList WRITE setPlaybackList NOTIFY playbackListChanged)
    Q_PROPERTY(bool sequential READ sequential WRITE setSequential NOTIFY sequentialChanged)
    Q_PROPERTY(bool progressBar READ progressBar WRITE setProgressBar NOTIFY progressBarChanged)
    int m_playbackList;

    bool m_sequential;

    bool m_progressBar;

public:
    explicit Playback(QObject *parent = nullptr);

    static int playbackListStringToIndex(QString playback);

    static QString playbackListIndexToString(int index);

    enum PLAYBACKLIST{
        SIGNAL_GROUP = 0,
        ALL_GROUP,
        MAX
    };


int playbackList() const
{
    return m_playbackList;
}

bool sequential() const
{
    return m_sequential;
}

bool progressBar() const
{
    return m_progressBar;
}

public slots:
void setPlaybackList(int playbackList)
{
    if(playbackList < 0)
        playbackList = 0;
    if(playbackList >= MAX)
        playbackList = MAX - 1;


    if (m_playbackList == playbackList)
        return;

    m_playbackList = playbackList;
    emit playbackListChanged(m_playbackList);
}

void setSequential(bool sequential)
{
    if (m_sequential == sequential)
        return;

    m_sequential = sequential;
    emit sequentialChanged(m_sequential);
}

void setProgressBar(bool progressBar)
{
    if (m_progressBar == progressBar)
        return;

    m_progressBar = progressBar;
    emit progressBarChanged(m_progressBar);
}

signals:

void playbackListChanged(int playbackList);
void sequentialChanged(bool sequential);
void progressBarChanged(bool progressBar);
};

#endif // PLAYBACK_H
