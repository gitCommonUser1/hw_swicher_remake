#include "ndilistmodelitem.h"

NdilistModelItem::NdilistModelItem(QAbstractListModel *parent):QAbstractListModel(parent)
{

}

int NdilistModelItem::rowCount(const QModelIndex &parent) const
{
    return m_data.size();
}

QVariant NdilistModelItem::data(const QModelIndex &index, int role) const
{
    QVariant ret;
    int row = index.row();

    if(row >= m_data.size()||(!index.isValid()))
    {
        return QVariant();
    }

    NdiItemClass ndiData = m_data.at(row);

    // 下面的role要和setData中的role一一对应；
    switch(role) {
    case NdilistModelItem::NdiName:
        ret = ndiData.name();
        break;
    case NdilistModelItem::NdiUrl:
        ret = ndiData.url();
        break;
    case NdilistModelItem::NdiType:
        ret = ndiData.type();
        break;
    case NdilistModelItem::NdiCurrent:
        ret = ndiData.current();
        break;
    case NdilistModelItem::NdiItem:
        ret = QVariant::fromValue(ndiData);
        break;
    default :
        break;
    }
    return ret;
}

bool NdilistModelItem::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool ret = false;
    int row = index.row();

    if(row >= m_data.size()||(!index.isValid()))
    {
        return false;
    }
    NdiItemClass tmpData = m_data.at(row);

    switch(role) {
    case NdilistModelItem::NdiName:
        tmpData.setName(value.toString());
        ret = true;
        break;
    case NdilistModelItem::NdiUrl:
        tmpData.setUrl(value.toString());
        ret = true;
        break;
    case NdilistModelItem::NdiType:
        tmpData.setType(value.toInt());
        ret = true;
        break;
    case NdilistModelItem::NdiCurrent:
        tmpData.setCurrent(value.toInt());
        ret = true;
        break;
    case NdilistModelItem::NdiItem:
        tmpData = value.value<NdiItemClass>();
        ret = true;
        break;
    default :
        break;
    }
    m_data[row] = tmpData;
    return ret;
}

QHash<int, QByteArray> NdilistModelItem::roleNames() const
{
    QHash<int,QByteArray>ret;
    ret.insert(NdilistModelItem::NdiName,"name");
    ret.insert(NdilistModelItem::NdiUrl,"url");
    ret.insert(NdilistModelItem::NdiType,"type");
    ret.insert(NdilistModelItem::NdiCurrent,"current");
    ret.insert(NdilistModelItem::NdiItem,"item");
    return ret;
}


void NdilistModelItem::insertData(int row, NdiItemClass data)
{
    beginInsertRows(QModelIndex(),row,row);
    m_data.insert(row,data);
    endInsertRows();
}

void NdilistModelItem::removeData(int row)
{
    beginRemoveRows(QModelIndex(),row,row);
    m_data.removeAt(row);
    endRemoveRows();
}

void NdilistModelItem::removeAll()
{
    beginRemoveRows(QModelIndex(),0,rowCount() - 1);
    m_data.clear();
    endRemoveRows();
}

void NdilistModelItem::setData(int row, NdiItemClass data)
{
    if(m_data.size() <= row)
    {
        insertData(row,data);
    }
    else
    {
        m_data[row] = data;
        dataChanged(index(row),index(row));
    }
}

