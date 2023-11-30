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
    Q_PROPERTY(Key* key READ key WRITE setKey NOTIFY keyChanged)
    Q_PROPERTY(LumaParameters* lumaParameters READ lumaParameters WRITE setLumaParameters NOTIFY lumaParametersChanged)
    Q_PROPERTY(ChromaParameters* chromaParameters READ chromaParameters WRITE setChromaParameters NOTIFY chromaParametersChanged)
    Q_PROPERTY(PatternParameters* patternParameters READ patternParameters WRITE setPatternParameters NOTIFY patternParametersChanged)
    Q_PROPERTY(PIPParameters* pIPParameters READ pIPParameters WRITE setPIPParameters NOTIFY pIPParametersChanged)

public:
    explicit Keys(QObject *parent = nullptr);

    static QString sizeIndexToString(int index);
    static int sizeStringToIndex(QString size);

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

public slots:
    void setKey(Key* key)
    {
        if (m_key == key)
            return;

        m_key = key;
        emit keyChanged(m_key);
    }

    void setLumaParameters(LumaParameters* lumaParameters)
    {
        if (m_lumaParameters == lumaParameters)
            return;

        m_lumaParameters = lumaParameters;
        emit lumaParametersChanged(m_lumaParameters);
    }

    void setChromaParameters(ChromaParameters* chromaParameters)
    {
        if (m_chromaParameters == chromaParameters)
            return;

        m_chromaParameters = chromaParameters;
        emit chromaParametersChanged(m_chromaParameters);
    }

    void setPatternParameters(PatternParameters* patternParameters)
    {
        if (m_patternParameters == patternParameters)
            return;

        m_patternParameters = patternParameters;
        emit patternParametersChanged(m_patternParameters);
    }

    void setPIPParameters(PIPParameters* pIPParameters)
    {
        if (m_pIPParameters == pIPParameters)
            return;

        m_pIPParameters = pIPParameters;
        emit pIPParametersChanged(m_pIPParameters);
    }

private:

    Key* m_key;

    LumaParameters* m_lumaParameters;

    ChromaParameters* m_chromaParameters;

    PatternParameters* m_patternParameters;

    PIPParameters* m_pIPParameters;

signals:

void keyChanged(Key* keyType);
void lumaParametersChanged(LumaParameters* lumaParameters);
void chromaParametersChanged(ChromaParameters* chromaParameters);
void patternParametersChanged(PatternParameters* patternParameters);
void pIPParametersChanged(PIPParameters* pIPParameters);
};

#endif // KEYS_H
