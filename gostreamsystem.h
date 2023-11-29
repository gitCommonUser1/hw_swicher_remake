#ifndef GOSTREAMSYSTEM_H
#define GOSTREAMSYSTEM_H

#include <QObject>

class QXmlStreamWriter;

class MixEffectBlocks;
class DownstreamKeys;
class ColorBacks;

class Profile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int majorVersion READ majorVersion WRITE setMajorVersion NOTIFY majorVersionChanged)
    Q_PROPERTY(int minorVersion READ minorVersion WRITE setMinorVersion NOTIFY minorVersionChanged)
    Q_PROPERTY(int patchVersion READ patchVersion WRITE setPatchVersion NOTIFY patchVersionChanged)
    Q_PROPERTY(QString product READ product WRITE setProduct NOTIFY productChanged)
    Q_PROPERTY(MixEffectBlocks* mixEffectBlocks READ mixEffectBlocks WRITE setMixEffectBlocks NOTIFY mixEffectBlocksChanged)
    Q_PROPERTY(DownstreamKeys* downstreamKeys READ downstreamKeys WRITE setDownstreamKeys NOTIFY downstreamKeysChanged)
    Q_PROPERTY(ColorBacks* colorBacks READ colorBacks WRITE setColorBacks NOTIFY colorBacksChanged)
public:
    explicit Profile(QObject *parent = nullptr);

    void write(QObject *object);
    void writeRecursion(QObject *object,QXmlStreamWriter &stream);
    int read(QObject *object);



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

public slots:
    void setMixEffectBlocks(MixEffectBlocks* mixEffectBlocks)
    {
        if (m_mixEffectBlocks == mixEffectBlocks)
            return;

        m_mixEffectBlocks = mixEffectBlocks;
        emit mixEffectBlocksChanged(m_mixEffectBlocks);
    }

    void setDownstreamKeys(DownstreamKeys* downstreamKeys)
    {
        if (m_downstreamKeys == downstreamKeys)
            return;

        m_downstreamKeys = downstreamKeys;
        emit downstreamKeysChanged(m_downstreamKeys);
    }

    void setColorBacks(ColorBacks* colorBacks)
    {
        if (m_colorBacks == colorBacks)
            return;

        m_colorBacks = colorBacks;
        emit colorBacksChanged(m_colorBacks);
    }

    void setMajorVersion(int majorVersion)
    {
        if (m_majorVersion == majorVersion)
            return;

        m_majorVersion = majorVersion;
        emit majorVersionChanged(m_majorVersion);
    }

    void setMinorVersion(int minorVersion)
    {
        if (m_minorVersion == minorVersion)
            return;

        m_minorVersion = minorVersion;
        emit minorVersionChanged(m_minorVersion);
    }

    void setPatchVersion(int patchVersion)
    {
        if (m_patchVersion == patchVersion)
            return;

        m_patchVersion = patchVersion;
        emit patchVersionChanged(m_patchVersion);
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

signals:

void mixEffectBlocksChanged(MixEffectBlocks* mixEffectBlocks);
void downstreamKeysChanged(DownstreamKeys* downstreamKeys);
void colorBacksChanged(ColorBacks* colorBacks);
void majorVersionChanged(int majorVersion);
void minorVersionChanged(int minorVersion);
void patchVersionChanged(int patchVersion);
void productChanged(QString product);
};

#endif // GOSTREAMSYSTEM_H
