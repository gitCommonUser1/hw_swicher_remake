#ifndef SRCNAMES_H
#define SRCNAMES_H

#include <QObject>

class SrcName;

class SrcNames : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SrcName* pgm READ pgm)
    Q_PROPERTY(SrcName* pvw READ pvw)
    Q_PROPERTY(SrcName* in1 READ in1)
    Q_PROPERTY(SrcName* in2 READ in2)
    Q_PROPERTY(SrcName* in3 READ in3)
    Q_PROPERTY(SrcName* in4 READ in4)
    Q_PROPERTY(SrcName* aux READ aux)
    Q_PROPERTY(SrcName* still1 READ still1)
    Q_PROPERTY(SrcName* still2 READ still2)
public:
    explicit SrcNames(QObject *parent = nullptr);

    enum SRCNAMES{
        PGM = 0,
        PVW,
        IN1,
        IN2,
        IN3,
        IN4,
        AUX,
        STILL1,
        STILL2
    };

    static int srcNameStringToIndex(QString srcName);
    static QString srcNameIndexToString(int index);

    SrcName* pgm() const
    {
        return m_pgm;
    }

    SrcName* pvw() const
    {
        return m_pvw;
    }

    SrcName* in1() const
    {
        return m_in1;
    }

    SrcName* in2() const
    {
        return m_in2;
    }

    SrcName* in3() const
    {
        return m_in3;
    }

    SrcName* in4() const
    {
        return m_in4;
    }

    SrcName* aux() const
    {
        return m_aux;
    }

    SrcName* still1() const
    {
        return m_still1;
    }

    SrcName* still2() const
    {
        return m_still2;
    }

private:


    SrcName* m_pgm;

    SrcName* m_pvw;

    SrcName* m_in1;

    SrcName* m_in2;

    SrcName* m_in3;

    SrcName* m_in4;

    SrcName* m_aux;

    SrcName* m_still1;

    SrcName* m_still2;

signals:

};

#endif // SRCNAMES_H
