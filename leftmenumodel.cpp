#include "leftmenumodel.h"
#include <QDebug>
#include "settings.h"
#include "models.h"

extern Settings*settings;
extern Models *models;

LeftMenuModel::LeftMenuModel(QObject *parent): QAbstractListModel(parent)
{
    init();
}

void LeftMenuModel::init()
{
    m_menu.clear();
    auto list = settings->listFirst();
    for(int i = 0;i < list.size();++i)
    {
        QList<QString> s;
        for(int j = 0;j < list[i]->second.size();++j)
        {
            s.append(models->languages[list[i]->second[j]->name]);
        }
        m_menu.push_back(std::pair<QString,QList<QString>>(models->languages[list[i]->name],s));
    }

    for(int i = 0;i < m_menu.size();++i)
    {
        MenuItem item(m_menu[i].first,MENU_LEVEL_FIRST,i);
        if(hasIndex(i,0)){
            changeItem(item,i);
        }else{
            appendItem(item);
        }
    }

    if(m_items.size() > m_menu.size())
    {
        int index = settings->lastFirstUnfold();
        for(int i = 0;i < m_menu[index].second.size();++i)
        {
            MenuItem item(m_menu[index].second[i],MENU_LEVEL_SECOND,i);
            changeItem(item,index+1+i);
        }
    }
}

void LeftMenuModel::appendItem(const MenuItem &item)
{
    beginInsertRows(QModelIndex(),rowCount(),rowCount());
    m_items << item;
    endInsertRows();
}

void LeftMenuModel::insertItem(const MenuItem &item, int index)
{
    beginInsertRows(QModelIndex(),index,index);
    m_items.insert(index,item);
    endInsertRows();
}

void LeftMenuModel::removeItem(int index)
{
    beginRemoveRows(QModelIndex(),index,index);
    m_items.removeAt(index);
    endRemoveRows();
}

void LeftMenuModel::changeItem(const MenuItem &item, int ind)
{
    m_items[ind] = item;
    dataChanged(index(ind,0),index(ind,0));
}

int LeftMenuModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_items.count();
}

QVariant LeftMenuModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_items.count())
           return QVariant();

       MenuItem item = m_items[index.row()];
       if (role == LeftMenuModel::TypeName)
           return item.name();
       else if (role == LeftMenuModel::TypeLevel)
           return item.level();
       else if(role == LeftMenuModel::TypeUnfold)
           return item.unfold();
       else if(role == LeftMenuModel::TypeIndex)
           return item.index();
       return QVariant();
}

int LeftMenuModel::length()
{
    return m_items.size();
}

void LeftMenuModel::setUnfold(int index, int value)
{
    m_items[index].setUnfold(value);
    emit dataChanged(this->index(index),this->index(index));
}

QHash<int, QByteArray> LeftMenuModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[LeftMenuModel::TypeName] = "name";
    roles[LeftMenuModel::TypeLevel] = "level";
    roles[LeftMenuModel::TypeUnfold] = "unfold";
    roles[LeftMenuModel::TypeIndex] = "index";
    return roles;
}
