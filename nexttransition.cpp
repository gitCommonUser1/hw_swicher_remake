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
            if(selectionList[i] == selection)
            {
                if(i == 0)
                {
                    //100
                    return 0b100;
                }
                else if(i == 1)
                {
                    //010
                    return 0b010;
                }
                else if(i == 2)
                {
                    //001
                    return 0b001;
                }
            }
        }
    }
    else if(list.size() == 2)
    {
        // 110 || 101 || 011
        if((list[0] == selectionList[0] && list[1] == selectionList[1]) || (list[1] == selectionList[0] && list[0] == selectionList[1]))
        {
            //110
            return 0b110;
        }
        else if((list[0] == selectionList[0] && list[1] == selectionList[2]) || (list[1] == selectionList[0] && list[0] == selectionList[2]))
        {
            //101
            return 0b101;
        }
        else if((list[0] == selectionList[1] && list[1] == selectionList[2]) || (list[1] == selectionList[1] && list[0] == selectionList[2]))
        {
            //011
            return 0b011;
        }

    }
    else if(list.size() == 3)
    {
        // 111
        int key = list.indexOf(selectionList[0]);
        int dsk = list.indexOf(selectionList[1]);
        int bkgd = list.indexOf(selectionList[2]);

        if((key != -1 && dsk != -1 && bkgd != -1) &&
            (key != dsk && key != bkgd && dsk != bkgd))
        {
            return 0b111;
        }
    }
    else
    {
        //BKGD
        return 0b001;
    }
}
