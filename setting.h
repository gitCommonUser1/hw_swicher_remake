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
class OutFormat;
class OutSources;
class Panel;
class Language;
class Network;

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
    Q_PROPERTY(OutFormat* outFormat READ outFormat)
    Q_PROPERTY(OutSources* outSources READ outSources)
    Q_PROPERTY(Panel* panel READ panel)
    Q_PROPERTY(Language* language READ language)
    Q_PROPERTY(Network* network READ network)

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

    OutFormat* outFormat() const
    {
        return m_outFormat;
    }

    OutSources* outSources() const
    {
        return m_outSources;
    }

    Panel* panel() const
    {
        return m_panel;
    }

    Language* language() const
    {
        return m_language;
    }

    Network* network() const
    {
        return m_network;
    }

private:
    SrcNames* m_srcNames;

    MvMeters* m_mvMeters;

    MvLayout* m_mvLayout;

    Marker* m_marker;

    MicInputs* m_micInputs;

    Record* m_record;

    SrcSelections* m_srcSelections;

    OutFormat* m_outFormat;

    OutSources* m_outSources;

    Panel* m_panel;

    Language* m_language;

    Network* m_network;

signals:

};

#endif // SETTING_H
