#ifndef MIXEFFECTBLOCK_H
#define MIXEFFECTBLOCK_H

#include <QObject>

class Program;
class Preview;
class NextTransition;
class TransitionStyle;
class Keys;
class FadeToBlack;

class MixEffectBlock : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int index READ index WRITE setIndex FINAL )
    Q_PROPERTY(Program* program READ program)
    Q_PROPERTY(Preview* preview READ preview)
    Q_PROPERTY(NextTransition* nextTransition READ nextTransition)
    Q_PROPERTY(TransitionStyle* transitionStyle READ transitionStyle)
    Q_PROPERTY(Keys* keys READ keys)
    Q_PROPERTY(FadeToBlack* ftb READ ftb)
public:
    explicit MixEffectBlock(QObject *parent = nullptr);

    static int inputStringToIndex(QString str);
    static QString inputIndexToString(int index);

    enum INPUT{
        IN1 = 0,
        IN2,
        IN3,
        IN4,
        AUX,
        BLACK,
        STILL1,
        STILL2
    };


    int index() const
    {
        return m_index;
    }

    Program* program() const
    {
        return m_program;
    }

    Preview* preview() const
    {
        return m_preview;
    }

    NextTransition* nextTransition() const
    {
        return m_nextTransition;
    }

    Keys* keys() const
    {
        return m_keys;
    }

    TransitionStyle* transitionStyle() const
    {
        return m_transitionStyle;
    }

    FadeToBlack* ftb() const
    {
        return m_ftb;
    }

public slots:
    void setIndex(int index)
    {
        if (m_index == index)
            return;

        m_index = index;
    }

private:

    int m_index;

    Program* m_program;

    Preview* m_preview;

    NextTransition* m_nextTransition;

    Keys* m_keys;

    TransitionStyle* m_transitionStyle;

    FadeToBlack* m_ftb;

signals:
};

#endif // MIXEFFECTBLOCK_H
