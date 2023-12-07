#ifndef SETTING_H
#define SETTING_H

#include <QObject>

class SrcNames;

class Setting : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SrcNames* srcNmaes READ srcNmaes)
public:
    explicit Setting(QObject *parent = nullptr);

    SrcNames* srcNmaes() const
    {
        return m_srcNmaes;
    }

private:
    SrcNames* m_srcNmaes;

signals:

};

#endif // SETTING_H
