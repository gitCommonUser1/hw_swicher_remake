#ifndef MENUDATATYPE_H
#define MENUDATATYPE_H

#include <QObject>
#include <QList>
#include <QVariant>
#include <QDebug>

#include "osee_math.h"

enum DATATYPE{
    DATATYPE_NUMBER = 0,//数字
    DATATYPE_PERCENTAGE,//百分比
    DATATYPE_FLOAT,//浮点数
    DATATYPE_ENUM,//枚举类型
    DATATYPE_DB,
    DATATYPE_DB_INT,
    DATATYPE_MS,
    DATATYPE_S,
    DATATYPE_ICON,
    DATATYPE_TEXT,// url  fileName  xxx
    DATATYPE_MAX
};

//菜单类型
//目前支持的菜单类型有：
//1.直接改变调用
//2.改变后确认调用，取消则恢复之前的值
//3.改变后确认调用，取消不恢复
//4.按钮类型，点击进入事件，事件结束后调用
enum MENUTYPE{
    CHANGED_CALL = 0,//1.直接改变调用
    ENTER_CALL_OR_RESET,//2.改变后确认调用，取消则恢复之前的值
    ENTER_CALL,//3.改变后确认调用，取消不恢复
    EVENT_CALL//4.按钮类型，点击进入事件，事件结束后调用
};

class MenuThird{

public:
    MenuThird(){
    }
    MenuThird(QString t_name,QString s_name,QVariant t_min,QVariant t_current,QVariant t_max,QVariant t_step,DATATYPE t_type){
        name = t_name;
        ss_name = s_name;
        min = t_min;
        current = t_current;
        max = t_max;
        step = t_step;
        type = t_type;
        menuType = CHANGED_CALL;
    }
    QString name;   //菜单显示
    QString ss_name; //别名，存储和读取用，不会修改
    QVariant current;
    QVariant min;
    QVariant max;
    QVariant step;
    DATATYPE type;
    MENUTYPE menuType;
    QString getText(){
        if(type == DATATYPE_NUMBER){
            return current.toString();
        }else if(type == DATATYPE_PERCENTAGE){
            return current.toString() + "%";
        }else if(type == DATATYPE_FLOAT){
            return QString::number(dround(current.toDouble(),3),'f',2);
        }else if(type == DATATYPE_ENUM){
            if(list_text.size() == 0)
                return "";
            int index = current.toInt();
            if(index < 0)
                index = 0;
            if(index >= list_text.size())
                index = list_text.size() - 1;
            return list_text[index];
        }else if(type == DATATYPE_DB){
            return QString::number(dround(current.toDouble(),2),'f',1) + "dB";
        }else if(type == DATATYPE_DB_INT){
            return QString::number(current.toInt()) + "dB";
        }else if(type == DATATYPE_MS){
            return current.toString() + "mS";
        }else if(type == DATATYPE_S){
            return current.toString() + "S";
        }else if(type == DATATYPE_ICON){
            return current <= list_text.size() - 1? list_text[current.toInt()]: "";
        }else if(type == DATATYPE_TEXT){
            return current.toString();
        }
    }
    int addValue(){
        if(type == DATATYPE_NUMBER || type == DATATYPE_PERCENTAGE || type == DATATYPE_MS || type == DATATYPE_S || type == DATATYPE_DB_INT){
            if(current.toInt() <= max.toInt() - step.toInt())
                current = current.toInt() + step.toInt();
            else
                return -1;
        }else if(type == DATATYPE_FLOAT || type == DATATYPE_DB){
            double t_current = dround(current.toDouble(),2);
            double t_max = dround(max.toDouble(),2);
            if(t_current < t_max){
                current = current.toDouble() + step.toDouble();
                if(current > max){
                    current = max;
                }
            }
            else
                return -1;
        }else if(type == DATATYPE_ENUM || type == DATATYPE_ICON){
            if(current.toInt() < list_text.size() - 1)
                current = current.toInt() + 1;
            else
                return -1;
        }else if(type == DATATYPE_TEXT){
            return -1;
        }
        return 0;
    }
    int subValue(){
        if(type == DATATYPE_NUMBER || type == DATATYPE_PERCENTAGE || type == DATATYPE_MS || type == DATATYPE_S || type == DATATYPE_DB_INT){
            if(current.toInt() >= min.toInt() + step.toInt())
                current = current.toInt() - step.toInt();
            else
                return -1;
        }else if(type == DATATYPE_FLOAT || type == DATATYPE_DB){
            double t_current = dround(current.toDouble(),2);
            double t_min = dround(min.toDouble(),2);
            if(t_current > t_min){
                current = current.toDouble() - step.toDouble();
                if(current < min){
                    current = min;
                }
            }
            else
                return -1;
        }else if(type == DATATYPE_ENUM || type == DATATYPE_ICON){
            if(current.toInt() > 0)
                current = current.toInt() - 1;
            else
                return -1;
        }else if(type == DATATYPE_TEXT){
            return -1;
        }
        return 0;
    }

    int setValue(QVariant var){
        if(type == DATATYPE_NUMBER || type == DATATYPE_PERCENTAGE || type == DATATYPE_MS || type == DATATYPE_S || type == DATATYPE_DB_INT){
            if(var.toInt() > max.toInt())
                return -1;//var = max;
            if(var.toInt() < min.toInt())
                return -1;//var = min;

                current = var;
        }else if(type == DATATYPE_FLOAT || type == DATATYPE_DB){
            double t_var = dround(var.toDouble(),2);
            double t_max = dround(max.toDouble(),2);
            double t_min = dround(min.toDouble(),2);
            if(t_var > t_max)
                return -1;//var = max;
            if(t_var < t_min)
                return -1;//var = min;

                current = var;
        }else if(type == DATATYPE_ICON){
            if(var.toInt() > list_text.size() - 1)
                return -1;//var = list_text.size() - 1;
            if(var.toInt() < 0)
                return -1;//var = 0;

                current = var;
        }else if(type == DATATYPE_ENUM){
            if(var.type() == QVariant::String)
            {
                int index = list_text.indexOf(var.toString());
                if(index != -1){
                    current = index;
                }else
                    return -1;
            }
            else
            {
                int index = var.toInt();
                if(index < 0)
                    return -1;//index = 0;
                if(index > list_text.size() - 1)
                    return -1;//index = list_text.size() - 1;
                current = index;
            }

        }
        else if(type == DATATYPE_TEXT){
            current = var;
        }
        return 0;
    }

    QList<QString>list_text;

    //值改变工作
    virtual void doWork(QVariant value) = 0;
    //点击工作
    virtual void doEvent(){ qDebug("doEvent !") ;}
};

struct MenuSecond{
    QString name;   //菜单显示
    QString ss_name; //别名，存储和读取用，不会修改
    QList<MenuThird *>third;
};

struct MenuFirst{
    QString name;   //菜单显示
    QString ss_name; //别名，存储和读取用，不会修改
    QList<MenuSecond *>second;//QList<Second>
};

#endif // MENUDATATYPE_H
