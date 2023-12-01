#ifndef SUPERSOURCEMASK_H
#define SUPERSOURCEMASK_H

#include <QObject>

class SuperSourceMask : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int index READ index WRITE setIndex FINAL)
    Q_PROPERTY(bool enable READ enable WRITE setEnable NOTIFY enableChanged)
    Q_PROPERTY(int hStart READ hStart WRITE setHStart NOTIFY hStartChanged)
    Q_PROPERTY(int vStart READ vStart WRITE setVStart NOTIFY vStartChanged)
    Q_PROPERTY(int hEnd READ hEnd WRITE setHEnd NOTIFY hEndChanged)
    Q_PROPERTY(int vEnd READ vEnd WRITE setVEnd NOTIFY vEndChanged)
public:
    explicit SuperSourceMask(int index,QObject *parent = nullptr);

    int index() const
    {
        return m_index;
    }

    int hStart() const
    {
        return m_hStart;
    }

    int vStart() const
    {
        return m_vStart;
    }

    int hEnd() const
    {
        return m_hEnd;
    }

    int vEnd() const
    {
        return m_vEnd;
    }

    bool enable() const
    {
        return m_enable;
    }

public slots:
    void setIndex(int index)
    {
        m_index = index;
    }

    void setHStart(int hStart)
    {
        if(hStart > m_hStart_max)
            hStart = m_hStart_max;
        if(hStart < m_hStart_min)
            hStart = m_hStart_min;
        if (m_hStart == hStart)
            return;

        m_hStart = hStart;
        emit hStartChanged(m_hStart);
    }

    void setVStart(int vStart)
    {
        if(vStart > m_vStart_max)
            vStart = m_vStart_max;
        if(vStart < m_vStart_min)
            vStart = m_vStart_min;
        if (m_vStart == vStart)
            return;

        m_vStart = vStart;
        emit vStartChanged(m_vStart);
    }

    void setHEnd(int hEnd)
    {
        if(hEnd > m_hEnd_max)
            hEnd = m_hEnd_max;
        if(hEnd < m_hEnd_min)
            hEnd = m_hEnd_min;
        if (m_hEnd == hEnd)
            return;

        m_hEnd = hEnd;
        emit hEndChanged(m_hEnd);
    }

    void setVEnd(int vEnd)
    {
        if(vEnd > m_vEnd_max)
            vEnd = m_vEnd_max;
        if(vEnd < m_vEnd_min)
            vEnd = m_vEnd_min;
        if (m_vEnd == vEnd)
            return;

        m_vEnd = vEnd;
        emit vEndChanged(m_vEnd);
    }

    void setEnable(bool enable)
    {
        if (m_enable == enable)
            return;

        m_enable = enable;
        emit enableChanged(m_enable);
    }

private:
    int m_index;

    int m_hStart;
    int m_hStart_min;
    int m_hStart_max;

    int m_vStart;
    int m_vStart_min;
    int m_vStart_max;

    int m_hEnd;
    int m_hEnd_min;
    int m_hEnd_max;

    int m_vEnd;
    int m_vEnd_min;
    int m_vEnd_max;

    bool m_enable;

signals:

void hStartChanged(int hStart);
void vStartChanged(int vStart);
void hEndChanged(int hEnd);
void vEndChanged(int vEnd);
void enableChanged(bool enable);
};

#endif // SUPERSOURCEMASK_H
