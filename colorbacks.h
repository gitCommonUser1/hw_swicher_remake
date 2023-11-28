#ifndef COLORBACKS_H
#define COLORBACKS_H

#include <QObject>

class ColorBack;

class ColorBacks : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ColorBack* colorBack1 READ colorBack1 WRITE setColorBack1 NOTIFY colorBack1Changed)
    Q_PROPERTY(ColorBack* colorBack2 READ colorBack2 WRITE setColorBack2 NOTIFY colorBack2Changed)
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

public slots:
    void setColorBack1(ColorBack* colorBack1)
    {
        if (m_colorBack1 == colorBack1)
            return;

        m_colorBack1 = colorBack1;
        emit colorBack1Changed(m_colorBack1);
    }

    void setColorBack2(ColorBack* colorBack2)
    {
        if (m_colorBack2 == colorBack2)
            return;

        m_colorBack2 = colorBack2;
        emit colorBack2Changed(m_colorBack2);
    }

private:

    ColorBack* m_colorBack1;

    ColorBack* m_colorBack2;

signals:

void colorBack1Changed(ColorBack* colorBack1);
void colorBack2Changed(ColorBack* colorBack2);
};

#endif // COLORBACKS_H
