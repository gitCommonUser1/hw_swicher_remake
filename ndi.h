#ifndef NDI_H
#define NDI_H

#include <QObject>
#include "settings.h"
#include "hw_config.h"

extern Settings *settings;

#include "ndilistmodelitem.h"

class Ndi : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<NdiItemClass> ndiList READ ndiList WRITE setNdiList NOTIFY ndiListChanged)
    Q_PROPERTY(NdiItemClass lastConnectItem READ lastConnectItem WRITE setLastConnectItem NOTIFY lastConnectItemChanged)
    Q_PROPERTY(int ndiStatus READ ndiStatus WRITE setNdiStatus NOTIFY ndiStatusChanged)

public:
    explicit Ndi(QObject *parent = nullptr);
    NdilistModelItem *ndiListModelItem(){return m_ndilistModelItem;}

    Q_INVOKABLE void searchNdiList(int searchTime);
    Q_INVOKABLE void connectNdi(QString name,QString ip,QString port);
    Q_INVOKABLE void connectNdi(QString name,QString url);
    Q_INVOKABLE void connectNdi(NdiItemClass item);
    Q_INVOKABLE void disconnectNdi();

    void setNdiData(QList<NdiItemClass> ndiList);
    void appendOther(QList<NdiItemClass> &ndiList);
    void swapCurrentToTop(QList<NdiItemClass> &ndiList);
    bool isItemExists(NdiItemClass item);

    QList<NdiItemClass> ndiList() const
    {
        return m_ndiList;
    }

    NdiItemClass lastConnectItem() const
    {
        return m_lastConnectItem;
    }

    int ndiStatus() const
    {
        return m_ndiStatus;
    }

public slots:
    void parseNdiStatus(QString json);
    void parseNdiList(QString json);
    void onNdiStatusChanged(int status);


    void setNdiList(QList<NdiItemClass> ndiList)
    {
        m_ndiList = ndiList;
        emit ndiListChanged(m_ndiList);
    }

    void setLastConnectItem(NdiItemClass lastConnectItem)
    {
        m_lastConnectItem = lastConnectItem;
        settings->setIniValue(NDI_PATH,"url",lastConnectItem.url());
        settings->setIniValue(NDI_PATH,"name",lastConnectItem.name());
        emit lastConnectItemChanged(m_lastConnectItem);
    }

    void setNdiStatus(int ndiStatus)
    {
        m_ndiStatus = ndiStatus;
        emit ndiStatusChanged(m_ndiStatus);
    }
private:
    NdilistModelItem *m_ndilistModelItem;

    QList<NdiItemClass> m_ndiList;

    NdiItemClass m_lastConnectItem;

    int m_ndiStatus;

signals:

void ndiListChanged(QList<NdiItemClass> ndiList);
void lastConnectItemChanged(NdiItemClass lastConnectItem);
void ndiStatusChanged(int ndiStatus);
};

#endif // NDI_H
