#ifndef MIXEFFECTBLOCKS_H
#define MIXEFFECTBLOCKS_H

#include <QObject>

class MixEffectBlock;

class MixEffectBlocks : public QObject
{
    Q_OBJECT
    Q_PROPERTY(MixEffectBlock* mixEffectBlock READ mixEffectBlock)
public:
    explicit MixEffectBlocks(QObject *parent = nullptr);

    MixEffectBlock* mixEffectBlock() const
    {
        return m_mixEffectBlock;
    }

private:

    MixEffectBlock* m_mixEffectBlock;

};

#endif // MIXEFFECTBLOCKS_H
