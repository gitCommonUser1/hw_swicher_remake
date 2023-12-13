#ifndef MICINPUT_H
#define MICINPUT_H

#include <QObject>
#include "micinputs.h"

class MicInput : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int index READ index WRITE setIndex FINAL)
    Q_PROPERTY(int input READ input WRITE setInput NOTIFY inputChanged)
public:
    explicit MicInput(int index,QObject *parent = nullptr);

    int input() const
    {
        return m_input;
    }

    int index() const
    {
        return m_index;
    }

public slots:
    void setInput(int input)
    {
        if(input <= MicInputs::MIC_POWER)
            input = MicInputs::MIC_POWER;
        if(input >= MicInputs::LINE)
            input = MicInputs::LINE;

        if (m_input == input)
            return;

        m_input = input;
        emit inputChanged(m_input);
    }

    void setIndex(int index)
    {
        m_index = index;
    }

private:


    int m_input;

    int m_index;

signals:

void inputChanged(int input);
};

#endif // MICINPUT_H
