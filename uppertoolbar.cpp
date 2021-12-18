#include "uppertoolbar.h"

#include <QFontComboBox>
#include <QTextCharFormat>
#include <QLayout>
#include <QToolButton>
#include <QDebug>

UpperToolBar::UpperToolBar(QWidget *parent)
    : QToolBar(tr("UpperToolBar"), parent)
    , fontFamilyComboBox_(new QFontComboBox)
    , fontSizeComboBox_(new QComboBox)
    , textBoldButton_(new QToolButton)
    , textItalicButton_(new QToolButton)
    , textUnderlineButton_(new QToolButton)
{
    setMovable(false);

    addTextFormattingResponsiblePart();
}

void UpperToolBar::updateTextFormattingResponsiblePart(const QTextCharFormat &format)
{
    QFont font = format.font();
    fontFamilyComboBox_->setCurrentIndex(fontFamilyComboBox_->findText(font.family()));
    fontSizeComboBox_->setCurrentIndex(fontSizeComboBox_->findText(QString::number(font.pointSize())));
    textBoldButton_->setChecked(font.bold());
    textItalicButton_->setChecked(font.italic());
    textUnderlineButton_->setChecked(font.underline());
}

void UpperToolBar::mergeFontFamily(const QString &fontFamily)
{
    QTextCharFormat format;
    format.setFontFamily(fontFamily);
    emit fontFormatChanged(format);
}

void UpperToolBar::mergeFontSize(const QString &fontSize)
{
    qreal pointSize = fontSize.toFloat();
    if (pointSize > 0) {
        QTextCharFormat format;
        format.setFontPointSize(pointSize);
        emit fontFormatChanged(format);
    }
}

void UpperToolBar::mergeBold()
{
    QTextCharFormat format;
    format.setFontWeight(textBoldButton_->isChecked() ? QFont::Bold : QFont::Normal);
    emit fontFormatChanged(format);
}

void UpperToolBar::mergeItalic()
{
    QTextCharFormat format;
    format.setFontItalic(textItalicButton_->isChecked());
    emit fontFormatChanged(format);
}

void UpperToolBar::mergeUnderline()
{
    QTextCharFormat format;
    format.setFontUnderline(textUnderlineButton_->isChecked());
    emit fontFormatChanged(format);
}

void UpperToolBar::addTextFormattingResponsiblePart()
{
    initTextFormattingResponsiblePart();

    auto createHorizontalSpacer = [=]() {
        return new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    };

    QHBoxLayout* hLayout1 = new QHBoxLayout;
    hLayout1->addWidget(fontFamilyComboBox_);
    hLayout1->addWidget(fontSizeComboBox_);

    QHBoxLayout* hLayout2 = new QHBoxLayout;
    hLayout2->addWidget(textBoldButton_);
    hLayout2->addWidget(textItalicButton_);
    hLayout2->addWidget(textUnderlineButton_);

    // temporary
    QToolButton* button = new QToolButton;
    button->setText("tmp spacing");
    button->setEnabled(false);
    button->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
    hLayout2->addWidget(button);
//    hLayout2->addItem(new QSpacerItem(60, 20, QSizePolicy::Minimum, QSizePolicy::Minimum));

    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);

    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addLayout(vLayout);
    mainLayout->addItem(createHorizontalSpacer());


    QFrame* widget = new QFrame;
    widget->setLayout(mainLayout);
    addWidget(widget);
}

void UpperToolBar::initTextFormattingResponsiblePart()
{
    fontFamilyComboBox_->setInsertPolicy(QComboBox::NoInsert);
    fontFamilyComboBox_->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed));

    fontSizeComboBox_->setInsertPolicy(QComboBox::NoInsert);
    fontSizeComboBox_->setEditable(true);
    fontSizeComboBox_->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed));

    connect(fontFamilyComboBox_, &QComboBox::textActivated,
            this,                &UpperToolBar::mergeFontFamily);

    connect(fontSizeComboBox_, &QComboBox::textActivated,
            this,              &UpperToolBar::mergeFontSize);

    const QList<int> standardSizes = QFontDatabase::standardSizes();
    for (int size : standardSizes)
        fontSizeComboBox_->addItem(QString::number(size));

    textBoldButton_->setText("B");
    textBoldButton_->setCheckable(true);
    textBoldButton_->setShortcut(QKeySequence::Bold);
    textBoldButton_->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed));
    connect(textBoldButton_, &QToolButton::clicked, this, &UpperToolBar::mergeBold);

    textItalicButton_->setText("I");
    textItalicButton_->setCheckable(true);
    textItalicButton_->setShortcut(QKeySequence::Italic);
    textItalicButton_->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    connect(textItalicButton_, &QToolButton::clicked, this, &UpperToolBar::mergeItalic);

    textUnderlineButton_->setText("U");
    textUnderlineButton_->setCheckable(true);
    textUnderlineButton_->setShortcut(QKeySequence::Underline);
    textUnderlineButton_->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    connect(textUnderlineButton_, &QToolButton::clicked, this, &UpperToolBar::mergeUnderline);
}
