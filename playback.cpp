#include "playback.h"

static QList<QString> playbackGroupList = {
    "Single Group",
    "All Group"
};

Playback::Playback(QObject *parent) : QObject(parent)
{

    setPlaybackList(SIGNAL_GROUP);
    setSequential(false);
    setProgressBar(true);
}

int Playback::playbackListStringToIndex(QString playback)
{
    for(int i = 0;i < playbackGroupList.size();++i)
    {
        if(playback.compare(playbackGroupList[i],Qt::CaseInsensitive) == 0)
        {
            return i;
        }
    }
    return 0;
}

QString Playback::playbackListIndexToString(int index)
{
    if(index < 0)
        index = 0;
    if(index >= playbackGroupList.size())
        index = playbackGroupList.size() - 1;

    return playbackGroupList[index];
}
