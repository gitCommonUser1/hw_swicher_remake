#include "playbackgroupmanager.h"
#include <QFile>
#include <QDir>
#include <QSettings>
#include "sdimages.h"
#include <QSet>
#include <unistd.h>

extern SDImages *sdImages;

PlaybackGroupManager::PlaybackGroupManager(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<std::map<QString,QList<QString>>>("std::map<QString,QList<QString>>");

    connect(this,&PlaybackGroupManager::groupsChanged,this,[=](std::map<QString,QList<QString>> gourps){
        QList<QString> list;
        for(auto it = gourps.begin();it != gourps.end();++it)
        {
            for(int i = 0;i < it->second.size();++i)
            {
                list << it->second[i];
            }
        }
        setList(list);
        if(list.size() == 0)
        {
            setListCurrent(0);
        }
        else if(listCurrent() >= list.size())
        {
            setListCurrent(list.size() - 1);
        }
    });
}

void PlaybackGroupManager::readGroups()
{
    //
    QSet<QString> set;
    for(int i = 0;i < sdImages->videoList().size();++i)
    {
        set << sdImages->videoList()[i];
    }
    QSettings settings(GROUP_FILE,QSettings::IniFormat);
    auto groups = settings.childGroups();
    foreach (QString group, groups) {
        settings.beginGroup(group);  // 开始遍历组
        QStringList keys = settings.allKeys();  // 获取组中的所有键
        foreach (QString key, keys) {
            QVariant value = settings.value(key);  // 获取键对应的值
            qDebug() << group << "/" << key << ": " << value.toString();
            if(set.contains(value.toString())){
                set.erase(set.find(value.toString()));
                m_groups[group] << value.toString();
            }
        }
        settings.endGroup();  // 结束遍历组
    }

    for(auto it = set.begin();it != set.end();++it)
    {
        m_groups[DEFAULT_GROUP_NAME] << *it;
    }

    emit groupsChanged(m_groups);
    debugGroups();
    setListCurrent(0);
}

void PlaybackGroupManager::writeGroups()
{
    //
    QSettings settings(GROUP_FILE,QSettings::IniFormat);
    for(auto it = m_groups.begin();it != m_groups.end();++it)
    {
        if(it->first == DEFAULT_GROUP_NAME)
        {
            continue;
        }
        settings.beginGroup(it->first);
        for(int i = 0;i < it->second.size();++i)
        {
            settings.setValue(QString::number(i),it->second[i]);
        }
        settings.endGroup();
    }
    settings.sync();
    writeFileAgainByC(GROUP_FILE);
}

void PlaybackGroupManager::joinGroup(QString name, QString group)
{
    m_groups[group] << name;
    QSettings settings(GROUP_FILE,QSettings::IniFormat);
    settings.beginGroup(group);
    int size = settings.allKeys().size();
    settings.setValue(group + "/" + size,name);

    settings.sync();
    writeFileAgainByC(GROUP_FILE);

    emit groupsChanged(m_groups);
    debugGroups();
}

void PlaybackGroupManager::clearGroups()
{
    m_groups.clear();
    emit groupsChanged(m_groups);
}

void PlaybackGroupManager::writeFileAgainByC(const char *fileName)
{
    //断电无法保存，c语言方式再写一次
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QByteArray ba = file.readAll();
    file.close();

    FILE *fp;
    fp = fopen(fileName, "wt+");
    fwrite(ba, 1, ba.size(), fp);
    fflush(fp);
    fsync(fileno(fp));
    fclose(fp);
}

void PlaybackGroupManager::debugGroups()
{
    for(auto it = m_groups.begin();it != m_groups.end();++it)
    {
        qDebug() << "_________________ " << it->first;
        for(int i = 0;i < it->second.size();++i)
        {
            qDebug() << it->second[i];
        }
        qDebug() << "   ";
        qDebug() << "   ";
    }
}

QList<QString> PlaybackGroupManager::groupsName()
{
    QList<QString >list;
    for(auto it = m_groups.begin();it != m_groups.end();++it)
    {
        list << it->first;
    }
    return list;
}

QList<QString> PlaybackGroupManager::groupByName(QString name)
{
    return m_groups[name];
}

void PlaybackGroupManager::setListCurrentByName(QString name)
{
    for(int i = 0;i < m_list.size();++i)
    {
        if(m_list[i] == name)
        {
            setListCurrent(i);
            break;
        }
    }
}

bool PlaybackGroupManager::isGroupLast(QString name)
{
    for(auto it = m_groups.begin();it != m_groups.end();++it)
    {
        for(int i = 0;i < it->second.size();++i)
        {
            if(name == it->second[i] && i != it->second.size() - 1)
                return false;
        }
    }
    return true;
}

QString PlaybackGroupManager::group(int index)
{
    if(index < 0)
        return "";
    if(index >= list().size())
        return "";

    QString name = list()[index];
    for(auto it = m_groups.begin();it != m_groups.end();++it)
    {
        for(int i = 0;i < it->second.size();++i)
        {
            if(name == it->second[i])
                return it->first;
        }
    }
    return "";
}
