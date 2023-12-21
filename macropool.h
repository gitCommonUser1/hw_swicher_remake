#ifndef MACROPOOL_H
#define MACROPOOL_H

#include <QObject>

class Macro;

class MacroPool : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> macros READ macros WRITE setMacros NOTIFY macrosChanged)
public:
    explicit MacroPool(QObject *parent = nullptr);

    static bool listCompare(QObject* src,QObject* dst);

    bool isMacroIndexExists(int index);
    Macro *macroIndex(int index);

    void append(Macro *macro);
    void remove(int index);
    void move(int src,int dst);
    void swap(int src,int dst);

    static void deleteMacro(Macro *macro);

    QList<QObject*> macros() const
    {
        return m_macros;
    }

public slots:
    void setMacros(QList<QObject*> macros)
    {
        if (m_macros == macros)
            return;

        m_macros = macros;
        emit macrosChanged(m_macros);
    }

private:
    QList<QObject*> m_macros;

signals:

void macrosChanged(QList<QObject*> macros);

void newMacro(int index);
void removeMacro(int index);
void moveMacro(int src,int dst);
void swapMacro(int src,int dst);

};

#endif // MACROPOOL_H
