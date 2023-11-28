#ifndef DOWNSTREAMKEYS_H
#define DOWNSTREAMKEYS_H

#include <QObject>

class DownstreamKey;

class DownstreamKeys : public QObject
{
    Q_OBJECT
    Q_PROPERTY(DownstreamKey* downstreamKey READ downstreamKey WRITE setDownstreamKey NOTIFY downstreamKeyChanged)
public:
    explicit DownstreamKeys(QObject *parent = nullptr);


    DownstreamKey* downstreamKey() const
    {
        return m_downstreamKey;
    }

public slots:
    void setDownstreamKey(DownstreamKey* downstreamKey)
    {
        if (m_downstreamKey == downstreamKey)
            return;

        m_downstreamKey = downstreamKey;
        emit downstreamKeyChanged(m_downstreamKey);
    }

private:

    DownstreamKey* m_downstreamKey;

signals:

void downstreamKeyChanged(DownstreamKey* downstreamKey);
};

#endif // DOWNSTREAMKEYS_H
