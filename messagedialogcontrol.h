#ifndef MESSAGEDIALOGCONTROL_H
#define MESSAGEDIALOGCONTROL_H

#define UPDATE_MESSAGE_FILE "/oem/hw_rockchip_qt/update.message"

#include <QObject>

class MessageDialogControl : public QObject
{
    Q_OBJECT    
    Q_ENUMS(MESSAGE_TYPE)

    Q_PROPERTY(QList<QString> options READ options WRITE setOptions NOTIFY optionsChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(int optionCurrent READ optionCurrent WRITE setOptionCurrent NOTIFY optionCurrentChanged)
    Q_PROPERTY(int messageDialogVisible READ messageDialogVisible WRITE setMessageDialogVisible NOTIFY messageDialogVisibleChanged)
    Q_PROPERTY(int progressBarVisible READ progressBarVisible WRITE setProgressBarVisible NOTIFY progressBarVisibleChanged)
    Q_PROPERTY(int progressPercent READ progressPercent WRITE setProgressPercent NOTIFY progressPercentChanged)
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)
    QList<QString> m_options;

    QString m_title;

    int m_optionCurrent;

    int m_messageDialogVisible;

public:
    explicit MessageDialogControl(QObject *parent = nullptr);

    Q_INVOKABLE void dialogShow(QString p_title,QList<QString>p_options,int type = MESSAGE_NORMAL);
    void buttonClicked();
    void cancel();

    void setProcessPercent(int per);
    void setProcessOver();

    enum MESSAGE_TYPE{
        MESSAGE_NORMAL = 0,
        MESSAGE_FACTORY_RESET,
        MESSAGE_SD_FORMAT,
        MESSAGE_SD_NO_MOUNT_TIP,
        MESSAGE_OUT_FORMAT,
        MESSAGE_UPDATE,
        MESSAGE_TYPE_MAX
    };

QList<QString> options() const
{
    return m_options;
}

QString title() const
{
    return m_title;
}

int optionCurrent() const
{
    return m_optionCurrent;
}

int messageDialogVisible() const
{
    return m_messageDialogVisible;
}

int progressBarVisible() const
{
    return m_progressBarVisible;
}

int progressPercent() const
{
    return m_progressPercent;
}

int type() const
{
    return m_type;
}

public slots:
void setOptions(QList<QString> options)
{
    if (m_options == options)
        return;

    m_options = options;
    emit optionsChanged(m_options);
}

void setTitle(QString title)
{
    if (m_title == title)
        return;

    m_title = title;
    emit titleChanged(m_title);
}

void setOptionCurrent(int optionCurrent)
{
    if (m_optionCurrent == optionCurrent)
        return;

    m_optionCurrent = optionCurrent;
    emit optionCurrentChanged(m_optionCurrent);
}

void setMessageDialogVisible(int messageDialogVisible)
{
    if (m_messageDialogVisible == messageDialogVisible)
        return;

    m_messageDialogVisible = messageDialogVisible;
    emit messageDialogVisibleChanged(m_messageDialogVisible);
}

void setProgressBarVisible(int progressBarVisible)
{
    if (m_progressBarVisible == progressBarVisible)
        return;

    m_progressBarVisible = progressBarVisible;
    emit progressBarVisibleChanged(m_progressBarVisible);
}


void setProgressPercent(int progressPercent)
{
    if (m_progressPercent == progressPercent)
        return;

    m_progressPercent = progressPercent;
    emit progressPercentChanged(m_progressPercent);
}

void setType(int type)
{
    if (m_type == type)
        return;

    m_type = type;
    emit typeChanged(m_type);
}

signals:

void accept();
void reject();

void optionsChanged(QList<QString> options);
void titleChanged(QString title);
void optionCurrentChanged(int optionCurrent);
void messageDialogVisibleChanged(int messageDialogVisible);


void progressBarVisibleChanged(int progressBarVisible);

void progressPercentChanged(int progressPercent);

void typeChanged(int type);

private:
int m_type;
int m_progressBarVisible;
int m_progressPercent;
};

#endif // MESSAGEDIALOGCONTROL_H
