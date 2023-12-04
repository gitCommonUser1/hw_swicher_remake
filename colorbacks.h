#ifndef COLORBACKS_H
#define COLORBACKS_H

#include <QObject>

class ColorBack;

class ColorBacks : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ColorBack* colorBack1 READ colorBack1)
    Q_PROPERTY(ColorBack* colorBack2 READ colorBack2)
public:
    explicit ColorBacks(QObject *parent = nullptr);

    enum COLORBACKS{
        COLOR1 = 0,
        COLOR2
    };

    ColorBack* colorBack1() const
    {
        return m_colorBack1;
    }

    ColorBack* colorBack2() const
    {
        return m_colorBack2;
    }


private:

    ColorBack* m_colorBack1;

    ColorBack* m_colorBack2;

};

#endif // COLORBACKS_H
