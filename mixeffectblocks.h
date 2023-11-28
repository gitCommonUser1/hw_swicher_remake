#ifndef MIXEFFECTBLOCKS_H
#define MIXEFFECTBLOCKS_H

#include <QObject>

class MixEffectBlock;

class MixEffectBlocks : public QObject
{
    Q_OBJECT
    Q_PROPERTY(MixEffectBlock* mixEffectBlock READ mixEffectBlock WRITE setMixEffectBlock NOTIFY mixEffectBlockChanged)
public:
    explicit MixEffectBlocks(QObject *parent = nullptr);

    MixEffectBlock* mixEffectBlock() const
    {
        return m_mixEffectBlock;
    }

public slots:
    void setMixEffectBlock(MixEffectBlock* mixEffectBlock)
    {
        if (m_mixEffectBlock == mixEffectBlock)
            return;

        m_mixEffectBlock = mixEffectBlock;
        emit mixEffectBlockChanged(m_mixEffectBlock);
    }

private:

    MixEffectBlock* m_mixEffectBlock;

signals:

void mixEffectBlockChanged(MixEffectBlock* mixEffectBlock);
};

#endif // MIXEFFECTBLOCKS_H
