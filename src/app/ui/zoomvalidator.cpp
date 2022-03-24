#include "ui/zoomvalidator.h"

#include <QDebug>

ZoomValidator::ZoomValidator(QObject *parent)
    : QValidator(parent)
{

}

QValidator::State ZoomValidator::validate(QString &input, int &) const
{
    if (input.isEmpty())
        return Intermediate;

    bool ok = false;
    int num = input.toInt(&ok);
    if (ok)
        return ((num >= Bottom && num <= Top) ? Acceptable : Intermediate);

    return Invalid;
}

void ZoomValidator::fixup(QString &input) const
{
    int num = input.toInt();
    num = qMin(num, int(Top));
    num = qMax(num, int(Bottom));
    input = QString::number(num);
}
