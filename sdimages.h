#ifndef SDIMAGES_H
#define SDIMAGES_H

#include <QObject>
#include <QDebug>

//SD卡插拔重新获取图片列表

class QTimer;
class QFileSystemWatcher;
class QDir;

class SDImages : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QString> imageList READ imageList WRITE setImageList NOTIFY imageListChanged)
    Q_PROPERTY(QList<QString> videoList READ videoList WRITE setVideoList NOTIFY videoListChanged)
    Q_PROPERTY(QList<QString> streamKeyList READ streamKeyList WRITE setStreamKeyList NOTIFY streamKeyListChanged)

public:
    explicit SDImages(QObject *parent = nullptr);

    QList<QString> imageList() const
    {
        return m_imageList;
    }

    QList<QString> videoList() const
    {
        return m_videoList;
    }

public slots:
    void flushImageList();
    void flushVideoList();
    void flushStreamKeyList();

    void setImageList(QList<QString> imageList)
    {
        if (m_imageList == imageList)
            return;

        m_imageList = imageList;
        emit imageListChanged(m_imageList);
    }


    void setVideoList(QList<QString> videoList)
    {
        if (m_videoList == videoList)
            return;

        m_videoList = videoList;
        emit videoListChanged(m_videoList);
    }

    void setStreamKeyList(QList<QString> streamKeyList)
    {
        if (m_streamKeyList == streamKeyList)
            return;

        m_streamKeyList = streamKeyList;
        emit streamKeyListChanged(m_streamKeyList);
    }

public:
    QList<QString>m_imageList;
    QTimer *timer;
    QFileSystemWatcher *watcher;


    QList<QString> m_videoList;
    QList<QString> m_streamKeyList;

    QList<QString> streamKeyList() const
    {
        return m_streamKeyList;
    }

signals:
void imageListChanged(QList<QString>);

void videoListChanged(QList<QString>);
void streamKeyListChanged(QList<QString> streamKeyList);
};

#endif // SDIMAGES_H
