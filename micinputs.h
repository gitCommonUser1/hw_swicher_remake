#ifndef MICINPUTS_H
#define MICINPUTS_H

#include <QObject>
#include <QWidget>

class MicInput;

class MicInputs : public QObject
{
    Q_OBJECT
    Q_PROPERTY(MicInput* mic1 READ mic1)
    Q_PROPERTY(MicInput* mic2 READ mic2)
public:
    explicit MicInputs(QObject *parent = nullptr);
    enum INPUT{
        MIC_POWER = 0,
        MIC,
        LINE
    };

    enum SOURCE{
        MIC1 = 0,
        MIC2
    };

    static int micInputStringToIndex(QString input);
    static QString micInputIndexToString(int index);
    static int srcNameToIndex(QString srcName);

    MicInput* mic1() const
    {
        return m_mic1;
    }

    MicInput* mic2() const
    {
        return m_mic2;
    }

private:


    MicInput* m_mic1;

    MicInput* m_mic2;

signals:

};

#endif // MICINPUTS_H
