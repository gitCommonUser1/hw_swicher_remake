#ifndef PLAYBACKGROUPMANAGER_H
#define PLAYBACKGROUPMANAGER_H

#define GROUP_FILE "/mnt/sdcard/videos/groups.ini"
#define DEFAULT_GROUP_NAME "Default List"
#define RECORD_GROUP_NAME "Record Videos"

#include <QObject>
#include <map>

class PlaybackGroupManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QString> list READ list WRITE setList NOTIFY listChanged)
    Q_PROPERTY(int listCurrent READ listCurrent WRITE setListCurrent NOTIFY listCurrentChanged)
public:
    explicit PlaybackGroupManager(QObject *parent = nullptr);
    void readGroups();
    void writeGroups();
    void joinGroup(QString name,QString group);
    void clearGroups();

    void writeFileAgainByC(const char* fileName);
    void debugGroups();

    Q_INVOKABLE QList<QString> groupsName();
    Q_INVOKABLE QList<QString> groupByName(QString name);
    void setListCurrentByName(QString name);
    bool isGroupLast(QString name);
    QString group(int index);

    QList<QString> list() const
    {
        return m_list;
    }

    int listCurrent() const
    {
        return m_listCurrent;
    }

public slots:
    void setList(QList<QString> list)
    {
        if (m_list == list)
            return;

        m_list = list;
        emit listChanged(m_list);
    }

    void setListCurrent(int listCurrent)
    {
        if (m_listCurrent == listCurrent)
            return;

        m_listCurrent = listCurrent;
        emit listCurrentChanged(m_listCurrent);
    }

private:
    std::map<QString,QList<QString>>m_groups;

    QList<QString> m_list;

    int m_listCurrent;

signals:
    void groupsChanged(std::map<QString,QList<QString>> gourps);
    void listChanged(QList<QString> list);
    void listCurrentChanged(int listCurrent);
};

#endif // PLAYBACKGROUPMANAGER_H
