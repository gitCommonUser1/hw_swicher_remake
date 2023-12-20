#ifndef GOSTREAMSYSTEM_H
#define GOSTREAMSYSTEM_H

#include <QObject>

class QXmlStreamWriter;

class MixEffectBlocks;
class DownstreamKeys;
class ColorBacks;
class SuperSources;
class AudioMixer;
class StillGenerator;
class Streams;
class Playback;
class Setting;
class MacroPool;


class Profile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int majorVersion READ majorVersion WRITE setMajorVersion FINAL)
    Q_PROPERTY(int minorVersion READ minorVersion WRITE setMinorVersion FINAL)
    Q_PROPERTY(int patchVersion READ patchVersion WRITE setPatchVersion FINAL)
    Q_PROPERTY(QString product READ product WRITE setProduct NOTIFY productChanged)
    Q_PROPERTY(MixEffectBlocks* mixEffectBlocks READ mixEffectBlocks)
    Q_PROPERTY(DownstreamKeys* downstreamKeys READ downstreamKeys)
    Q_PROPERTY(ColorBacks* colorBacks READ colorBacks)
    Q_PROPERTY(SuperSources* superSources READ superSources)
    Q_PROPERTY(AudioMixer* audioMixer READ audioMixer)
    Q_PROPERTY(StillGenerator* stillGenerator READ stillGenerator)
    Q_PROPERTY(MacroPool* macroPool READ macroPool)
    Q_PROPERTY(Streams* streams READ streams)
    Q_PROPERTY(Playback* playback READ playback)
    Q_PROPERTY(Setting* setting READ setting)
public:
    explicit Profile(QObject *parent = nullptr);

    void emitSignals();
    void emitSignal(QObject *object);

    void write(QObject *object);
    void writeRecursion(QObject *object,QXmlStreamWriter &stream);
    int read(QObject *object);

    bool isHiddenProperty(QObject *object,QString name);



    MixEffectBlocks* mixEffectBlocks() const
    {
        return m_mixEffectBlocks;
    }

    DownstreamKeys* downstreamKeys() const
    {
        return m_downstreamKeys;
    }

    ColorBacks* colorBacks() const
    {
        return m_colorBacks;
    }

    int majorVersion() const
    {
        return m_majorVersion;
    }

    int minorVersion() const
    {
        return m_minorVersion;
    }

    int patchVersion() const
    {
        return m_patchVersion;
    }

    QString product() const
    {
        return m_product;
    }

    SuperSources* superSources() const
    {
        return m_superSources;
    }

    AudioMixer* audioMixer() const
    {
        return m_audioMixer;
    }

    StillGenerator* stillGenerator() const
    {
        return m_stillGenerator;
    }

    Streams* streams() const
    {
        return m_streams;
    }

    Setting* setting() const
    {
        return m_setting;
    }

    Playback* playback() const
    {
        return m_playback;
    }

    MacroPool* macroPool() const
    {
        return m_macroPool;
    }

public slots:
    void setMajorVersion(int majorVersion)
    {
        if (m_majorVersion == majorVersion)
            return;

        m_majorVersion = majorVersion;
    }

    void setMinorVersion(int minorVersion)
    {
        if (m_minorVersion == minorVersion)
            return;

        m_minorVersion = minorVersion;
    }

    void setPatchVersion(int patchVersion)
    {
        if (m_patchVersion == patchVersion)
            return;

        m_patchVersion = patchVersion;
    }

    void setProduct(QString product)
    {
        if (m_product == product)
            return;

        m_product = product;
        emit productChanged(m_product);
    }


private:

    MixEffectBlocks* m_mixEffectBlocks;

    DownstreamKeys* m_downstreamKeys;

    ColorBacks* m_colorBacks;

    int m_majorVersion;

    int m_minorVersion;

    int m_patchVersion;

    QString m_product;

    SuperSources* m_superSources;

    AudioMixer* m_audioMixer;

    StillGenerator* m_stillGenerator;

    Streams* m_streams;

    Setting* m_setting;

    Playback* m_playback;

    MacroPool* m_macroPool;

signals:
void productChanged(QString product);
};

#endif // GOSTREAMSYSTEM_H
