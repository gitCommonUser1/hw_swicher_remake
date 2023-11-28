#ifndef PROGRAM_H
#define PROGRAM_H

#include <QObject>

class Program : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int input READ input WRITE setInput NOTIFY inputChanged)
public:
    explicit Program(QObject *parent = nullptr);

    int input() const
    {
        return m_input;
    }

public slots:
    void setInput(int input)
    {
        if (m_input == input)
            return;

        m_input = input;
        emit inputChanged(m_input);
    }

private:

    int m_input;

signals:

void inputChanged(int input);
};

#endif // PROGRAM_H
