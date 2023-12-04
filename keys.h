#ifndef KEYS_H
#define KEYS_H

#include <QObject>

class Key;
class LumaParameters;
class ChromaParameters;
class PatternParameters;
class PIPParameters;

class Keys : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Key* key READ key)
    Q_PROPERTY(LumaParameters* lumaParameters READ lumaParameters)
    Q_PROPERTY(ChromaParameters* chromaParameters READ chromaParameters)
    Q_PROPERTY(PatternParameters* patternParameters READ patternParameters)
    Q_PROPERTY(PIPParameters* pIPParameters READ pIPParameters)

public:
    explicit Keys(QObject *parent = nullptr);

    static QString sizeIndexToString(int index);
    static int sizeStringToIndex(QString size);
    static QString keyIndexToString(int index);
    static int keyStringToIndex(QString key);

    enum KEYTYPE{
        LUMA = 0,
        CHROMA,
        PATTERN,
        PIP
    };

    enum KEYSIZE{
        KEY_SIZE_0_25 = 0,
        KEY_SIZE_0_33,
        KEY_SIZE_0_50
    };

    enum KEYSOURCE{
        IN1 ,
        IN2,
        IN3,
        IN4,
        AUX,
        STILL1,
        STILL1KEY,
        STILL2,
        STILL2KEY,
        COLOR1,
        COLOR2,
        COLORBAR,
        BLACK,
        MAX
    };

    Key* key() const
    {
        return m_key;
    }

    LumaParameters* lumaParameters() const
    {
        return m_lumaParameters;
    }

    ChromaParameters* chromaParameters() const
    {
        return m_chromaParameters;
    }

    PatternParameters* patternParameters() const
    {
        return m_patternParameters;
    }

    PIPParameters* pIPParameters() const
    {
        return m_pIPParameters;
    }


private:

    Key* m_key;

    LumaParameters* m_lumaParameters;

    ChromaParameters* m_chromaParameters;

    PatternParameters* m_patternParameters;

    PIPParameters* m_pIPParameters;
};

#endif // KEYS_H
