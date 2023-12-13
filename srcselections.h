#ifndef SRCSELECTIONS_H
#define SRCSELECTIONS_H

#include <QObject>

class SrcSelection;

class SrcSelections : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SrcSelection* in1 READ in1)
    Q_PROPERTY(SrcSelection* in2 READ in2)
    Q_PROPERTY(SrcSelection* in3 READ in3)
    Q_PROPERTY(SrcSelection* in4 READ in4)
    Q_PROPERTY(SrcSelection* aux READ aux)

public:
    explicit SrcSelections(QObject *parent = nullptr);

    static int sourceStringToIndex(QString source);
    static QString sourceIndexToString(int index);

    static int selectionStringToIndex(QString selection);
    static QString selectionIndexToString(int index);
    static QString auxIndexToString(int index);

    enum SOURCE{
        IN1 = 0,
        IN2,
        IN3,
        IN4,
        AUX
    };

    enum INPUTSELECTION{
        AUTO = 0,
        RGB_FULL,
        RGB_LIMIT,
        YCBCR422_FULL,
        YCBCR422_LIMIT,
        YCBCR444_FULL,
        YCBCR444_LIMIT,
        INPUT_MAX
    };

    enum AUXSELECTION{
        SD_CARD = 0,
        USB,
        NDI,
        AUX_MAX
    };

    SrcSelection* in1() const
    {
        return m_in1;
    }

    SrcSelection* in2() const
    {
        return m_in2;
    }

    SrcSelection* in3() const
    {
        return m_in3;
    }

    SrcSelection* in4() const
    {
        return m_in4;
    }

    SrcSelection* aux() const
    {
        return m_aux;
    }

private:
    SrcSelection* m_in1;

    SrcSelection* m_in2;

    SrcSelection* m_in3;

    SrcSelection* m_in4;

    SrcSelection* m_aux;

signals:

};

#endif // SRCSELECTIONS_H
