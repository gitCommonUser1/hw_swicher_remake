#ifndef SUPERSOURCES_H
#define SUPERSOURCES_H

#include <QObject>

class SuperSource;

class SuperSources : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SuperSource* superSource READ superSource WRITE setSuperSource NOTIFY superSourceChanged)
public:
    explicit SuperSources(QObject *parent = nullptr);

    SuperSource* superSource() const
    {
        return m_superSource;
    }

public slots:
    void setSuperSource(SuperSource* superSource)
    {
        if (m_superSource == superSource)
            return;

        m_superSource = superSource;
        emit superSourceChanged(m_superSource);
    }

private:
    SuperSource* m_superSource;

signals:

void superSourceChanged(SuperSource* superSource);
};

#endif // SUPERSOURCES_H
