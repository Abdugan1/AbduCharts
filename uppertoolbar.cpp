#include "uppertoolbar.h"

#include <QFontComboBox>
#include <QTextCharFormat>
#include <QTextBlockFormat>
#include <QLayout>
#include <QToolButton>
#include <QButtonGroup>
#include <QDebug>

UpperToolBar::UpperToolBar(QWidget *parent)
    : QToolBar(tr("UpperToolBar"), parent)
    , fontFamilyComboBox_(new QFontComboBox)
    , fontSizeComboBox_(new QComboBox)
    , textBoldButton_(new QToolButton)
    , textItalicButton_(new QToolButton)
    , textUnderlineButton_(new QToolButton)
    , textAlignLeft_(new QToolButton)
    , textAlignCenter_(new QToolButton)
    , textAlignRight_(new QToolButton)
{
    setMovable(false);

    addTextFormattingResponsiblePart();
}

void UpperToolBar::updateCharFormattingResponsiblePart(const QTextCharFormat &format)
{
    QFont font = format.font();
    fontFamilyComboBox_->setCurrentIndex(fontFamilyComboBox_->findText(font.family()));
    fontSizeComboBox_->setCurrentIndex(fontSizeComboBox_->findText(QString::number(font.pointSize())));
    textBoldButton_->setChecked(font.bold());
    textItalicButton_->setChecked(font.italic());
    textUnderlineButton_->setChecked(font.underline());
}

void UpperToolBar::updateBlockFormattingResponsiblePart(const QTextBlockFormat &format)
{
    switch (format.alignment()) {
    case Qt::AlignLeft:
        textAlignLeft_->setChecked(true);
        break;
    case Qt::AlignCenter:
        textAlignCenter_->setChecked(true);
        break;
    case Qt::AlignRight:
        textAlignRight_->setChecked(true);
        break;
    }
}

void UpperToolBar::mergeFontFamily(const QString &fontFamily)
{
    QTextCharFormat format;
    format.setFontFamily(fontFamily);
    emit charFormatChanged(format);
}

void UpperToolBar::mergeFontSize(const QString &fontSize)
{
    qreal pointSize = fontSize.toFloat();
    if (pointSize > 0) {
        QTextCharFormat format;
        format.setFontPointSize(pointSize);
        emit charFormatChanged(format);
    }
}

void UpperToolBar::mergeBold()
{
    QTextCharFormat format;
    format.setFontWeight(textBoldButton_->isChecked() ? QFont::Bold : QFont::Normal);
    emit charFormatChanged(format);
}

void UpperToolBar::mergeItalic()
{
    QTextCharFormat format;
    format.setFontItalic(textItalicButton_->isChecked());
    emit charFormatChanged(format);
}

void UpperToolBar::mergeUnderline()
{
    QTextCharFormat format;
    format.setFontUnderline(textUnderlineButton_->isChecked());
    emit charFormatChanged(format);
}

void UpperToolBar::mergeAlignLeft()
{
    QTextBlockFormat format;
    format.setAlignment(Qt::AlignLeft);
    emit blockFormatChanged(format);
}

void UpperToolBar::mergeAlignCenter()
{
    QTextBlockFormat format;
    format.setAlignment(Qt::AlignCenter);
    qDebug() << "format:" << (format.alignment() == Qt::AlignCenter);
    emit blockFormatChanged(format);
}

void UpperToolBar::mergeAlignRight()
{
    QTextBlockFormat format;
    format.setAlignment(Qt::AlignRight);
    emit blockFormatChanged(format);
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
    hLayout2->addWidget(textAlignLeft_);
    hLayout2->addWidget(textAlignCenter_);
    hLayout2->addWidget(textAlignRight_);

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

    textAlignLeft_ ->setText("L");
    textAlignLeft_->setCheckable(true);
    textAlignLeft_->setShortcut(Qt::CTRL + Qt::Key_L);
    connect(textAlignLeft_, &QToolButton::clicked, this, &UpperToolBar::mergeAlignLeft);

    textAlignCenter_->setText("C");
    textAlignCenter_->setCheckable(true);
    textAlignCenter_->setShortcut(Qt::CTRL + Qt::Key_E);
    connect(textAlignCenter_, &QToolButton::clicked, this, &UpperToolBar::mergeAlignCenter);

    textAlignRight_->setText("R");
    textAlignRight_->setCheckable(true);
    textAlignRight_->setShortcut(Qt::CTRL + Qt::Key_R);
    connect(textAlignRight_, &QToolButton::clicked, this, &UpperToolBar::mergeAlignRight);

    textAlignLeft_->setChecked(true);

    QButtonGroup* buttonGroup = new QButtonGroup;
    buttonGroup->addButton(textAlignLeft_);
    buttonGroup->addButton(textAlignCenter_);
    buttonGroup->addButton(textAlignRight_);
}
