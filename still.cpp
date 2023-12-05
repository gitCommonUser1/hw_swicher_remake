#include "still.h"

Still::Still(QObject *parent) : QObject(parent)
{

}

Still::Still(int index, QString name, QString path, QObject *parent): QObject(parent)
{
    setIndex(index);
    setName(name);
    setPath(path);
}
