#include "sdimages.h"
#include "include/hw_config.h"
#include <QDir>
#include <QDebug>
#include <QFileSystemWatcher>
#include <QTimer>
#include <QFileSystemWatcher>
#include <QDir>
#include "media_sd.h"

extern Media_sd *media_sd;

SDImages::SDImages(QObject *parent) : QObject(parent)
{
    watcher = new QFileSystemWatcher();
    watcher->addPath(SD_IMAGE_PATH);
    watcher->addPath(SD_VIDEO_PATH);
    watcher->addPath(SD_STREAM_KEY_PATH);
    watcher->addPath(SD_MACRO_PATH);
    connect(watcher,&QFileSystemWatcher::fileChanged,this,[=](){
        qDebug() << "___fileChanged";
    });
    connect(watcher,&QFileSystemWatcher::directoryChanged,this,[=](){
        flushImageList();
        flushVideoList();
        flushStreamKeyList();
        flushMacroList();
    });
}

void SDImages::flushImageList()
{
    QDir dir(SD_IMAGE_PATH);
    QStringList nameList;
    nameList << "*.png";
    dir.setNameFilters(nameList);
    dir.setFilter(QDir::NoDotAndDotDot | QDir::Files );
    setImageList(dir.entryList());
}

void SDImages::flushVideoList()
{
    QDir dir(SD_VIDEO_PATH);
    QStringList nameList;
    nameList << "*.mp4";
    dir.setNameFilters(nameList);
    dir.setFilter(QDir::NoDotAndDotDot | QDir::Files );
    setVideoList(dir.entryList());
}

void SDImages::flushStreamKeyList()
{
    QDir dir(SD_STREAM_KEY_PATH);
    QStringList nameList;
    nameList << "*.txt";
    dir.setNameFilters(nameList);
    dir.setFilter(QDir::NoDotAndDotDot | QDir::Files );
    setStreamKeyList(dir.entryList());
}

void SDImages::flushMacroList()
{
    QDir dir(SD_MACRO_PATH);
    QStringList nameList;
    nameList << "*.xml";
    dir.setNameFilters(nameList);
    dir.setFilter(QDir::NoDotAndDotDot | QDir::Files );
    setMacroList(dir.entryList());
}
