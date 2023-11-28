#ifndef NEXTTRANSITION_H
#define NEXTTRANSITION_H

#include <QObject>

class NextTransition : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList selection READ selection WRITE setSelection NOTIFY selectionChanged)
    Q_PROPERTY(QStringList nextSelection READ nextSelection WRITE setNextSelection NOTIFY nextSelectionChanged)

public:
    explicit NextTransition(QObject *parent = nullptr);

    static QString selectionValueToString(int value);
    static int selectionStringToValue(QString selection);

    QStringList selection() const
    {
        return m_selection;
    }

    QStringList nextSelection() const
    {
        return m_nextSelection;
    }

public slots:
    void setSelection(QStringList selection)
    {
        if (m_selection == selection)
            return;

        m_selection = selection;
        emit selectionChanged(m_selection);
    }

    void setNextSelection(QStringList nextSelection)
    {
        if (m_nextSelection == nextSelection)
            return;

        m_nextSelection = nextSelection;
        emit nextSelectionChanged(m_nextSelection);
    }

private:

    QStringList m_selection;

    QStringList m_nextSelection;

signals:

void selectionChanged(QStringList selection);
void nextSelectionChanged(QStringList nextSelection);
};

#endif // NEXTTRANSITION_H
