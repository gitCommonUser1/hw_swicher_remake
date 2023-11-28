#include "regexp.h"
#include <QRegExp>

RegExp::RegExp(QObject *parent) : QObject(parent)
{

}

bool RegExp::exactMatch(int regIndex,QString text)
{
    QRegExp reg;
    switch (regIndex) {
    case REG_IPADDRESS:
        reg.setPattern("((?:(?:25[0-5]|2[0-4]\\d|[01]?\\d?\\d)\\.){3}(?:25[0-5]|2[0-4]\\d|[01]?\\d?\\d))");
        break;
    case REG_FILE_NAME:
        reg.setPattern("^[^\\\\/:*?\"<>|]*$");
        break;
    case REG_SRC_NAME:
        if(text.length() <= 8)
            return true;
        else
            return false;
        break;
    default:
        return true;
    }
    return reg.exactMatch(text);
}
