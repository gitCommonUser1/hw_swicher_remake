#ifndef MVMETERS_H
#define MVMETERS_H

#include <QObject>

class MvMeter;

class MvMeters : public QObject
{
    Q_OBJECT
    Q_PROPERTY(MvMeter* pgm READ pgm)
    Q_PROPERTY(MvMeter* in1 READ in1)
    Q_PROPERTY(MvMeter* in2 READ in2)
    Q_PROPERTY(MvMeter* in3 READ in3)
    Q_PROPERTY(MvMeter* in4 READ in4)
    Q_PROPERTY(MvMeter* aux READ aux)
public:
    explicit MvMeters(QObject *parent = nullptr);

    enum MVMETERS{
        PGM = 0,
        IN1,
        IN2,
        IN3,
        IN4,
        AUX
    };

    static int mvMeterStringToIndex(QString src);
    static QString mvMeterIndexToString(int index);

    MvMeter* pgm() const
    {
        return m_pgm;
    }

    MvMeter* in1() const
    {
        return m_in1;
    }

    MvMeter* in2() const
    {
        return m_in2;
    }

    MvMeter* in3() const
    {
        return m_in3;
    }

    MvMeter* in4() const
    {
        return m_in4;
    }

    MvMeter* aux() const
    {
        return m_aux;
    }

private:

    MvMeter* m_pgm;

    MvMeter* m_in1;

    MvMeter* m_in2;

    MvMeter* m_in3;

    MvMeter* m_in4;

    MvMeter* m_aux;

signals:

};

#endif // MVMETERS_H
