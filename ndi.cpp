#include "ndi.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "rv_switch_api.h"

extern Ndi *ndi;

void ndiSearchCallback(long handle, const char *buf, int buf_size){
    ndi->parseNdiList(buf);
}

void ndiStatusCallback(long handle, int status, int error){
    qDebug() << "______ndi status:" << status;
    ndi->setNdiStatus(status);
}

Ndi::Ndi(QObject *parent) : QObject(parent)
{
    m_ndilistModelItem = new NdilistModelItem();

    NdiItemClass item;
    item.setUrl(settings->getIniValue(NDI_PATH,"url").toString());
    item.setName(settings->getIniValue(NDI_PATH,"name").toString());
    setLastConnectItem(item);

    connect(this,&Ndi::ndiListChanged,this,&Ndi::setNdiData);
    connect(this,&Ndi::ndiStatusChanged,this,&Ndi::onNdiStatusChanged);
}

void Ndi::searchNdiList(int searchTime)
{
    rkav_ndi_search(searchTime,ndiSearchCallback,0);
}

void Ndi::connectNdi(QString name, QString ip, QString port)
{
    NdiItemClass item;
    item.setName(name);
    item.setUrl(ip + ":" + port);
    connectNdi(item);
}

void Ndi::connectNdi(QString name, QString url)
{
    NdiItemClass item;
    item.setName(name);
    item.setUrl(url);
    connectNdi(item);
}

void Ndi::connectNdi(NdiItemClass item)
{
    QJsonObject obj;
    obj["name"] = item.name();
    obj["url"] = item.url();
    qDebug() << "obj:" << obj;
    QJsonDocument json(obj);
    auto str = json.toJson();

    setLastConnectItem(item);
    rkav_ndi_connect(str.data(),str.size(),ndiStatusCallback,0);
}

void Ndi::disconnectNdi()
{
    rkav_ndi_disconnect();
}

void Ndi::parseNdiList(QString json)
{
    QJsonParseError parseError;
    QList<NdiItemClass>list;
    auto doc = QJsonDocument::fromJson(json.toUtf8(),&parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "JSON parse error:" << parseError.errorString();
    }
    if (doc.isObject()) {
        auto obj = doc.object();
        auto arr = obj.value("arr");
        if(arr.isArray()){
            auto array = arr.toArray();
            for(auto it = array.begin();it != array.end();++it){
                NdiItemClass itemClass;
                itemClass.setName((*it).toObject().value("name").toString());
                itemClass.setUrl((*it).toObject().value("url").toString());
                if(itemClass == m_lastConnectItem && m_ndiStatus == 1){
                    itemClass.setCurrent(1);
                }
                qDebug() << "itemClass: " << itemClass.name();
                list.append(itemClass);
            }
        }
    }
    std::sort(list.begin(),list.end(),NdiItemClass::compareBarData);
    swapCurrentToTop(list);
    setNdiList(list);
}

void Ndi::onNdiStatusChanged(int status)
{
    swapCurrentToTop(m_ndiList);
    ndiListChanged(m_ndiList);
}

void Ndi::parseNdiStatus(QString json)
{
    qDebug() << "___ndi status:" << json;
    QJsonParseError parseError;
    auto doc = QJsonDocument::fromJson(json.toLatin1(),&parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "JSON parse error:" << parseError.errorString();
    }
    if(doc.isObject()){
        setNdiStatus(doc.object().value("state").toInt());
    }
}

void Ndi::setNdiData(QList<NdiItemClass> ndiList)
{
    while(m_ndilistModelItem->rowCount() > ndiList.size()){
        m_ndilistModelItem->removeData(m_ndilistModelItem->rowCount() - 1);
    }
    for(int i = 0;i < ndiList.size();++i)
    {
        if(ndiList[i] == m_lastConnectItem && m_ndiStatus == 1)
            ndiList[i].setCurrent(1);
        else
            ndiList[i].setCurrent(0);
        m_ndilistModelItem->setData(i,ndiList[i]);
    }
}

void Ndi::appendOther(QList<NdiItemClass> &ndiList)
{
    NdiItemClass item;
    item.setName(tr("Other..."));
    item.setType(1);
    ndiList.append(item);
}

void Ndi::swapCurrentToTop(QList<NdiItemClass> &ndiList)
{
    for(int i = 0;i < ndiList.size();++i)
    {
        if(ndiList[i] == lastConnectItem())
        {
            ndiList.swapItemsAt(0,i);
            break;
        }
    }
}

bool Ndi::isItemExists(NdiItemClass item)
{
    for(int i = 0;i < m_ndiList.size();++i)
    {
        if(m_ndiList[i].name() == item.name() && m_ndiList[i].url() == item.url())
            return true;
    }
    return false;
}
