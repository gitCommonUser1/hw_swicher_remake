#include "fadetoblack.h"

FadeToBlack::FadeToBlack(QObject *parent) : QObject(parent)
{
    setRate(1.00);
    setAfv(true);
}
