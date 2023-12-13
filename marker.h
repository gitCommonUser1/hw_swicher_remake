#ifndef MARKER_H
#define MARKER_H

#include <QObject>

class Marker : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool pvwMarker READ pvwMarker WRITE setPvwMarker NOTIFY pvwMarkerChanged)
public:
    explicit Marker(QObject *parent = nullptr);

    bool pvwMarker() const
    {
        return m_pvwMarker;
    }

public slots:
    void setPvwMarker(bool pvwMarker)
    {
        if (m_pvwMarker == pvwMarker)
            return;

        m_pvwMarker = pvwMarker;
        emit pvwMarkerChanged(m_pvwMarker);
    }

private:

    bool m_pvwMarker;

signals:

void pvwMarkerChanged(bool pvwMarker);
};

#endif // MARKER_H
