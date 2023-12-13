#ifndef MVLAYOUT_H
#define MVLAYOUT_H

#include <QObject>

class MvLayout : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int layout READ layout WRITE setLayout NOTIFY layoutChanged)
public:
    explicit MvLayout(QObject *parent = nullptr);

    enum MVLAYOUT{
        PVW_PGM = 0,
        PGM_PVW
    };

    static int mvLayoutStringToIndex(QString layout);
    static QString mvLayoutIndexToString(int index);

    int layout() const
    {
        return m_layout;
    }

public slots:
    void setLayout(int layout)
    {
        if(layout <= PVW_PGM)
            layout = PVW_PGM;
        if(layout >= PGM_PVW)
            layout = PGM_PVW;

        if (m_layout == layout)
            return;

        m_layout = layout;
        emit layoutChanged(m_layout);
    }

private:

    int m_layout;

signals:

void layoutChanged(int layout);
};

#endif // MVLAYOUT_H
