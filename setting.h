#ifndef SETTING_H
#define SETTING_H

#include <QObject>

class SrcNames;
class MvMeters;
class MvLayout;
class Marker;
class MicInputs;
class Record;
class SrcSelections;

class Setting : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SrcNames* srcNames READ srcNames)
    Q_PROPERTY(MvMeters* mvMeters READ mvMeters)
    Q_PROPERTY(MvLayout* mvLayout READ mvLayout)
    Q_PROPERTY(Marker* marker READ marker)
    Q_PROPERTY(MicInputs* micInputs READ micInputs)
    Q_PROPERTY(Record* record READ record)
    Q_PROPERTY(SrcSelections* srcSelections READ srcSelections)

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

    MvLayout* mvLayout() const
    {
        return m_mvLayout;
    }

    Marker* marker() const
    {
        return m_marker;
    }

    MicInputs* micInputs() const
    {
        return m_micInputs;
    }

    Record* record() const
    {
        return m_record;
    }

    SrcSelections* srcSelections() const
    {
        return m_srcSelections;
    }

private:
    SrcNames* m_srcNames;

    MvMeters* m_mvMeters;

    MvLayout* m_mvLayout;

    Marker* m_marker;

    MicInputs* m_micInputs;

    Record* m_record;

    SrcSelections* m_srcSelections;

signals:

};

#endif // SETTING_H
