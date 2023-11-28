#ifndef MEDIA_SD_H
#define MEDIA_SD_H

#include <QObject>
#include <QString>


class Media_sd: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString gbFree READ gbFree WRITE setGbFree NOTIFY gbFreeChanged)
public:
    Media_sd();
    ~Media_sd();

    static QString secondToHMS(int second);

    void _is_online();
    char *_get_label();

    int is_online();

    char *name();

    uint64_t capacity();
    char *capacity_huma();
    int current_usage();

    uint64_t bused;
    uint64_t bfree;
    uint64_t btotal;

    int format_exfat(int index);
    int format_fat32(int index);

    QString gbFree() const
    {
        return m_gbFree;
    }
    void checkGbFree();
    void checkGbFree(int second);

private:
    bool online;
    char *label;
    char *partition[4];

    int mount_try;

    void _get_partition();

    QString m_gbFree;

public slots:
void slot_sd_change(bool is_online);
void setGbFree(QString gbFree)
{
    if (m_gbFree == gbFree)
        return;

    m_gbFree = gbFree;
    emit gbFreeChanged(m_gbFree);
}
signals:
void gbFreeChanged(QString gbFree);
};

extern Media_sd *media_sd;

#endif // MEDIA_SD_H
