#include "rightmenumodel.h"
#include "settings.h"
#include <QDebug>
#include "osee_math.h"
#include "models.h"
#include "MenuEnums.h"

extern Settings*settings;
extern Models *models;

RightMenuModel::RightMenuModel(QObject *parent):QAbstractListModel(parent)
{

}

void RightMenuModel::appendItem(const RightMenuData &item)
{
    beginInsertRows(QModelIndex(),rowCount(),rowCount());
    m_items << item;
    endInsertRows();
}

void RightMenuModel::insertItem(const RightMenuData &item, int index)
{
    beginInsertRows(QModelIndex(),index,index);
    m_items.insert(index,item);
    endInsertRows();
}

void RightMenuModel::removeItem(int index)
{
    beginRemoveRows(QModelIndex(),index,index);
    m_items.removeAt(index);
    endRemoveRows();
}

void RightMenuModel::clearItem()
{
    while(1)
    {
        if(length() != 0)
        {
            removeItem(0);
        }
        else
        {
            break;
        }
    }
}

int RightMenuModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_items.count();
}

QVariant RightMenuModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_items.count())
           return QVariant();

       RightMenuData item = m_items[index.row()];
       if (role == RightMenuModel::TypeKey)
           return item.key();
       else if (role == RightMenuModel::TypeValue)
           return item.value();
       else if(role == RightMenuModel::TypeIndex)
           return item.index();
       else if(role == RightMenuModel::TypeIsMin)
           return item.isMin();
       else if(role == RightMenuModel::TypeIsMax)
           return item.isMax();
       else if(role == RightMenuModel::TypeSpecial)
           return item.special();
       return QVariant();
}

int RightMenuModel::length()
{
    return m_items.size();
}

QHash<int, QByteArray> RightMenuModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[RightMenuModel::TypeKey] = "key";
    roles[RightMenuModel::TypeValue] = "value";
    roles[RightMenuModel::TypeIndex] = "index";
    roles[RightMenuModel::TypeIsMin] = "isMin";
    roles[RightMenuModel::TypeIsMax] = "isMax";
    roles[RightMenuModel::TypeSpecial] = "special";
    return roles;
}

void RightMenuModel::init_items()
{
    clearItem();
    if(settings->lastFirstUnfold() == -1)
        return ;
    int real_second_index = settings->leftListViewCurrent() - settings->lastFirstUnfold() - 1;

    auto list = settings->listFirst();
    if(settings->lastFirstUnfold() >= list.size())
        return ;

    auto third_list = list[settings->lastFirstUnfold()]->second[real_second_index]->third;
    for(int i = 0;i < third_list.size();++i)
    {
        RightMenuData data;
        data.setKey(models->languages[third_list[i]->name]);//
        if(settings->lastFirstUnfold() == MENU_FIRST_STREAM && i != MENU_THIRD_STREAM_OUTPUT)
            data.setValue(third_list[i]->getText());
        else
            third_list[i]->type == DATATYPE_ENUM?data.setValue(models->languages[third_list[i]->getText()]):data.setValue(third_list[i]->getText());
        data.setIndex(i);
        data.setSpecial(0);
        if(settings->lastFirstUnfold() == MENU_FIRST_COLOR_BACK && (real_second_index == COLOR_BACK_COLOR1 || real_second_index == COLOR_BACK_COLOR2) && (i == COLORBACK1_BRIGHTNESS || i == COLORBACK2_BRIGHTNESS))
            data.setSpecial(RightMenuModel::COLOR);
        else if(settings->lastFirstUnfold() == MENU_FIRST_SUPER_SOURCE && (real_second_index == SUPER_SOURCE_BORDER1 || real_second_index == SUPER_SOURCE_BORDER2) && (i == SUPER_BORDER1_BRIGHTNESS || i == SUPER_BORDER2_BRIGHTNESS))
            data.setSpecial(RightMenuModel::COLOR);
        else if(settings->lastFirstUnfold() == MENU_FIRST_CHROMA_KEY && real_second_index == CHROMA_KEY_CONTROL && i == CHROMA_KEY_CONTROL_SMP_ENABLE)
            data.setSpecial(RightMenuModel::COLOR);
        else if(settings->lastFirstUnfold() == MENU_FIRST_PIP && real_second_index == PIP_COLOR && i == PIP_COLOR_BRIGHTNESS)
            data.setSpecial(RightMenuModel::COLOR);
        else if(settings->lastFirstUnfold() == MENU_FIRST_KEY_PATTERN && real_second_index == PATTERN_WIPE && i == KEY_PATTERN_WIPE_PATTERN)
            data.setSpecial(RightMenuModel::PATTERN);
        else if(settings->lastFirstUnfold() == MENU_FIRST_KEY_PATTERN && real_second_index == PATTERN_WIPE && i == KEY_PATTERN_WIPE_PATTERN)
            data.setSpecial(RightMenuModel::PATTERN);
        else if(settings->lastFirstUnfold() == MENU_FIRST_TRANSITION && real_second_index == TRANSITION_WIPE && i == TRANSITION_WIPE_PATTERN)
            data.setSpecial(RightMenuModel::PATTERN);
        else if(settings->lastFirstUnfold() == MENU_FIRST_STILL_GENERATOR && real_second_index == STILL_GENERATE_UPLOAD && i == STILL_UPLOAD_LOCATION)
            data.setSpecial(RightMenuModel::LOCATION);
        else if(settings->lastFirstUnfold() == MENU_FIRST_STILL_GENERATOR && real_second_index == STILL_GENERATE_UPLOAD && i == STILL_UPLOAD_LOAD_PICTURE)
            data.setSpecial(RightMenuModel::LOAD_PICTURE);
        else if(settings->lastFirstUnfold() == MENU_FIRST_MACRO && real_second_index == MACRO_MACRO && i == MENU_THIRD_MACRO_IMPORT)
            data.setSpecial(RightMenuModel::WSTRING);
        else if(settings->lastFirstUnfold() == MENU_FIRST_STREAM && i == MENU_THIRD_STREAM_UPLOAD_KEY)
            data.setSpecial(RightMenuModel::WSTRING);
        else if(settings->lastFirstUnfold() == MENU_FIRST_STREAM && i == MENU_THIRD_STREAM_UPLOAD_KEY)
            data.setSpecial(RightMenuModel::WSTRING);


        if(third_list[i]->type == DATATYPE_TEXT){
            data.setIsMax(1);
            data.setIsMin(1);
        }else{
            double t_current = dround(third_list[i]->current.toDouble(),2);
            double t_max = dround(third_list[i]->max.toDouble(),2);
            double t_min = dround(third_list[i]->min.toDouble(),2);

            if(t_current <= t_min)
                data.setIsMin(1);
            if(t_current >= t_max)
                data.setIsMax(1);
        }

        appendItem(data);
        emit dataChanged(this->index(i),this->index(i));
    }
}

void RightMenuModel::change_item(int first,int second,int third)
{
    if(m_items.size() < third + 1)
        return ;
    auto item = settings->listFirst()[first]->second[second]->third[third];
    m_items[third].setKey(models->languages[item->name]);
    if(settings->lastFirstUnfold() == MENU_FIRST_STREAM && third != MENU_THIRD_STREAM_OUTPUT)
        m_items[third].setValue(item->getText());
    else
        item->type == DATATYPE_ENUM?m_items[third].setValue(models->languages[item->getText()]):m_items[third].setValue(item->getText());

    double t_current = dround(item->current.toDouble(),2);
    double t_max = dround(item->max.toDouble(),2);
    double t_min = dround(item->min.toDouble(),2);

    if(t_current == t_min)
        m_items[third].setIsMin(1);
    else
        m_items[third].setIsMin(0);
    if(t_current == t_max)
        m_items[third].setIsMax(1);
    else
        m_items[third].setIsMax(0);

    emit dataChanged(this->index(third),this->index(third));
}

