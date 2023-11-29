#ifndef NEXTTRANSITION_H
#define NEXTTRANSITION_H

#include <QObject>

class NextTransition : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString selection READ selection WRITE setSelection NOTIFY selectionChanged)
    Q_PROPERTY(QString nextSelection READ nextSelection WRITE setNextSelection NOTIFY nextSelectionChanged)

public:
    explicit NextTransition(QObject *parent = nullptr);

    static QString selectionValueToString(int value);
    static int selectionStringToValue(QString selection);

    QString selection() const
    {
        return m_selection;
    }

    QString nextSelection() const
    {
        return m_nextSelection;
    }

public slots:
    void setSelection(QString selection)
    {
        if (m_selection == selection)
            return;

        m_selection = selection;
        emit selectionChanged(m_selection);
    }

    void setNextSelection(QString nextSelection)
    {
        if (m_nextSelection == nextSelection)
            return;

        m_nextSelection = nextSelection;
        emit nextSelectionChanged(m_nextSelection);
    }

private:

    QString m_selection;

    QString m_nextSelection;

signals:

void selectionChanged(QString selection);
void nextSelectionChanged(QString nextSelection);
};

#endif // NEXTTRANSITION_H
