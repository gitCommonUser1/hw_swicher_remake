#ifndef SUPERSOURCES_H
#define SUPERSOURCES_H

#include <QObject>

class SuperSource;

class SuperSources : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SuperSource* superSource READ superSource)
public:
    explicit SuperSources(QObject *parent = nullptr);

    SuperSource* superSource() const
    {
        return m_superSource;
    }


private:
    SuperSource* m_superSource;

};

#endif // SUPERSOURCES_H
