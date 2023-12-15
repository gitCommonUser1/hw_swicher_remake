#include "nexttransition.h"
#include <QDebug>
#include <QThread>

static QList<QString> selectionList = {
    "BKGD",
    "DSK",
    "KEY"
};

NextTransition::NextTransition(QObject *parent) : QObject(parent)
{
    setSelection({selectionValueToString(2)});
    setNextSelection({selectionValueToString(2)});
}

QString NextTransition::selectionValueToString(int value)
{
    QString ret = "";
    if(value == 0b111){
        // 111
        ret += selectionList[0];
        ret += ",";
        ret += selectionList[1];
        ret += ",";
        ret += selectionList[2];
    }else if(value == 0b110){
        // 110
        ret += selectionList[0];
        ret += ",";
        ret += selectionList[1];
    }else if(value == 0b101){
        // 101
        ret += selectionList[0];
        ret += ",";
        ret += selectionList[2];
    }else if(value == 0b100){
        // 100
        ret += selectionList[0];
    }else if(value == 0b011){
        // 011
        ret += selectionList[1];
        ret += ",";
        ret += selectionList[2];
    }else if(value == 0b010){
        // 010
        ret += selectionList[1];
    }else if(value == 0b001){
        // 001
        ret += selectionList[2];
    }
    return ret;
}

int NextTransition::selectionStringToValue(QString selection)
{
    auto list = selection.split(",");
    if(list.size() == 0 || list.size() == 1)
    {
        // 100 || 010 || 001
        for(int i = 0;i < selectionList.size();++i)
        {
            if(selection.compare(selectionList[i],Qt::CaseInsensitive) == 0)
            {
                if(i == 0)
                {
                    //100
                    qDebug() << "0b100";
                    return 0b100;
                }
                else if(i == 1)
                {
                    //010
                    qDebug() << "0b010";
                    return 0b010;
                }
                else if(i == 2)
                {
                    //001
                    qDebug() << "0b001";
                    return 0b001;
                }
            }
        }
        return 0b100;
    }
    else if(list.size() == 2)
    {
        // 110 || 101 || 011 list[1] == selectionList[1]
        if((list[0].compare(selectionList[0],Qt::CaseInsensitive) == 0 && list[1].compare(selectionList[1],Qt::CaseInsensitive) == 0)
                || (list[1].compare(selectionList[0],Qt::CaseInsensitive) == 0 && list[0].compare(selectionList[1],Qt::CaseInsensitive) == 0))
        {
            //110
            qDebug() << "0b110";
            return 0b110;
        }
        else if((list[0].compare(selectionList[0],Qt::CaseInsensitive) == 0 && list[1].compare(selectionList[2],Qt::CaseInsensitive) == 0)
                || (list[1].compare(selectionList[0],Qt::CaseInsensitive) == 0 && list[0].compare(selectionList[2],Qt::CaseInsensitive) == 0))
        {
            //101
            qDebug() << "0b101";
            return 0b101;
        }
        else if((list[0].compare(selectionList[1],Qt::CaseInsensitive) == 0 && list[1].compare(selectionList[2],Qt::CaseInsensitive) == 0)
                || (list[1].compare(selectionList[1],Qt::CaseInsensitive) == 0 && list[0].compare(selectionList[2],Qt::CaseInsensitive) == 0))
        {
            //011
            qDebug() << "0b011";
            return 0b011;
        }

    }
    else if(list.size() == 3)
    {
        // 111
        int key = list.indexOf(selectionList[0],Qt::CaseInsensitive);
        int dsk = list.indexOf(selectionList[1],Qt::CaseInsensitive);
        int bkgd = list.indexOf(selectionList[2],Qt::CaseInsensitive);

        if((key != -1 && dsk != -1 && bkgd != -1) /*&&*/
            /*(key != dsk && key != bkgd && dsk != bkgd)*/)
        {
            qDebug() << "0b111";
            return 0b111;
        }
    }
    else
    {
        //BKGD
        qDebug() << "0b100";
        return 0b100;
    }
}
