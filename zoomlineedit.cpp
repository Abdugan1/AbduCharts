#include "zoomlineedit.h"
#include "zoomvalidator.h"

#include <QLabel>

ZoomLineEdit::ZoomLineEdit(QWidget *parent)
    : QLineEdit(parent)
    , percentLabel_(new QLabel("%", this))
{
    setValidator(new ZoomValidator(this));
    setMaxLength(4);
    connect(this, &QLineEdit::textChanged, this, &ZoomLineEdit::updatePercentLabel);
    updatePercentLabel("");

    connect(this, &ZoomLineEdit::editingFinished, this, &ZoomLineEdit::selectAll);
}

void ZoomLineEdit::updatePercentLabel(const QString &text)
{
    QFontMetrics fm(font());
    int w = fm.horizontalAdvance(" " + text);
    int h = 4;
    percentLabel_->move(w, h);
}
