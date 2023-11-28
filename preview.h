#ifndef PREVIEW_H
#define PREVIEW_H

#include <QObject>

class Preview : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int input READ input WRITE setInput NOTIFY inputChanged)

public:
    explicit Preview(QObject *parent = nullptr);

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

#endif // PREVIEW_H
