#include "stills.h"
#include "still.h"

Stills::Stills(QObject *parent) : QObject(parent)
{
    QList<QObject*>list;
    Still *st1 = new Still(1,"aa","bb",this);
    Still *st2 = new Still(4,"cc","dd",this);
    Still *st3 = new Still(5,"ee","ff",this);
    list << st1 << st2 << st3;
    setStills(list);

}
