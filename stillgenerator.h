#ifndef STILLGENERATOR_H
#define STILLGENERATOR_H

#include <QObject>

class StillSelection;
class Stills;

class StillGenerator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(StillSelection* stillSelection READ stillSelection)
    Q_PROPERTY(Stills* stills READ stills)
public:
    explicit StillGenerator(QObject *parent = nullptr);

    StillSelection* stillSelection() const
    {
        return m_stillSelection;
    }

    Stills* stills() const
    {
        return m_stills;
    }

private:

    StillSelection* m_stillSelection;

    Stills* m_stills;

signals:

};

#endif // STILLGENERATOR_H
