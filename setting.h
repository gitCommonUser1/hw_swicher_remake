#ifndef SETTING_H
#define SETTING_H

#include <QObject>

class SrcNames;
class MvMeters;

class Setting : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SrcNames* srcNames READ srcNames)
    Q_PROPERTY(MvMeters* mvMeters READ mvMeters)

public:
    explicit Setting(QObject *parent = nullptr);

    SrcNames* srcNames() const
    {
        return m_srcNames;
    }

    MvMeters* mvMeters() const
    {
        return m_mvMeters;
    }

private:
    SrcNames* m_srcNames;

    MvMeters* m_mvMeters;

signals:

};

#endif // SETTING_H
