#ifndef SRCSELECTION_H
#define SRCSELECTION_H

#include <QObject>
#include "srcselections.h"

class SrcSelection : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName FINAL)
    Q_PROPERTY(int selection READ selection WRITE setSelection NOTIFY selectionChanged)
public:
    explicit SrcSelection(QString name,QObject *parent = nullptr);


    int selection() const
    {
        return m_selection;
    }

    QString name() const
    {
        return m_name;
    }

public slots:


    void setSelection(int selection)
    {
        if(SrcSelections::sourceStringToIndex(m_name) == SrcSelections::AUX)
        {
            if(selection >= SrcSelections::AUX_MAX)
                selection = SrcSelections::AUX_MAX - 1;
        }
        else
        {
            if(selection >= SrcSelections::INPUT_MAX)
                selection = SrcSelections::INPUT_MAX - 1;
        }
        if(selection <= 0)
            selection = 0;

        if (m_selection == selection)
            return;

        m_selection = selection;
        emit selectionChanged(m_selection);
    }

    void setName(QString name)
    {
        m_name = name;
    }

private:


    int m_selection;

    QString m_name;

signals:

void selectionChanged(int selection);
};

#endif // SRCSELECTION_H
