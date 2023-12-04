#ifndef DOWNSTREAMKEYS_H
#define DOWNSTREAMKEYS_H

#include <QObject>

class DownstreamKey;

class DownstreamKeys : public QObject
{
    Q_OBJECT
    Q_PROPERTY(DownstreamKey* downstreamKey READ downstreamKey)
public:
    explicit DownstreamKeys(QObject *parent = nullptr);


    DownstreamKey* downstreamKey() const
    {
        return m_downstreamKey;
    }

private:

    DownstreamKey* m_downstreamKey;
};

#endif // DOWNSTREAMKEYS_H
