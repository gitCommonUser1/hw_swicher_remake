#include "stream.h"

Stream::Stream(int index,QObject *parent) : QObject(parent)
{
    setIndex(index);
}
