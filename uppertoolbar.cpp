#include "uppertoolbar.h"
#include "colorpickerbutton.h"
#include "grid.h"

#include <QFontComboBox>
#include <QTextCharFormat>
#include <QTextBlockFormat>
#include <QLayout>
#include <QToolButton>
#include <QButtonGroup>
#include <QCheckBox>
#include <QSpinBox>
#include <QColorDialog>
#include <QDebug>

const QColor DefaultGridColor = Qt::lightGray;

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
    , gridCheckBox_(new QCheckBox("&Grid"))
    , gridSizeSpinBox_(new QSpinBox)
    , gridColorButton_(new ColorPickerButton(DefaultGridColor))
{
    setMovable(false);

    setupTextFormattingPart();
    setupViewPart();

    addWidget(textFormattingPartLayoutedWidget());
    addSeparator();
    addWidget(viewPartLayoutedWidget());
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

void UpperToolBar::setupTextFormattingPart()
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

void UpperToolBar::setupViewPart()
{
    gridCheckBox_->setChecked(Grid::enabled());
    connect(gridCheckBox_, &QCheckBox::stateChanged, this, [this](int state) {
        Grid::setEnabled(state == Qt::Checked);
        emit gridEnabledChanged();
    });

    gridSizeSpinBox_->setSuffix(" pt");
    gridSizeSpinBox_->setMinimum(Grid::minGridSize());
    gridSizeSpinBox_->setMaximum(Grid::maxGridSize());
    gridSizeSpinBox_->setValue(Grid::gridSize());
    connect(gridSizeSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
        Grid::setGridSize(value);
        emit gridSizeValueChanged();
    });

    connect(gridColorButton_, &ColorPickerButton::colorPicked,
            this,             &UpperToolBar::gridColorChanged);
}

QWidget *UpperToolBar::textFormattingPartLayoutedWidget()
{
    QHBoxLayout* comboBoxesLayout = new QHBoxLayout;
    comboBoxesLayout->addWidget(fontFamilyComboBox_);
    comboBoxesLayout->addWidget(fontSizeComboBox_);

    QHBoxLayout* textFormatButtonsLayout = new QHBoxLayout;
    textFormatButtonsLayout->addWidget(textBoldButton_);
    textFormatButtonsLayout->addWidget(textItalicButton_);
    textFormatButtonsLayout->addWidget(textUnderlineButton_);
    textFormatButtonsLayout->addWidget(textAlignLeft_);
    textFormatButtonsLayout->addWidget(textAlignCenter_);
    textFormatButtonsLayout->addWidget(textAlignRight_);

    // temporary
    QToolButton* button = new QToolButton;
    button->setText("tmp spacing");
    button->setEnabled(false);
    button->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
    textFormatButtonsLayout->addWidget(button);
//    textFormatButtonsLayout->addItem(new QSpacerItem(60, 20, QSizePolicy::Minimum, QSizePolicy::Minimum));

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(comboBoxesLayout);
    mainLayout->addLayout(textFormatButtonsLayout);

    QWidget* widget = new QWidget;
    widget->setLayout(mainLayout);
    return widget;
}

QWidget *UpperToolBar::viewPartLayoutedWidget()
{
    QHBoxLayout* gridPartLayout = new QHBoxLayout;
    gridPartLayout->addWidget(gridCheckBox_);
    gridPartLayout->addWidget(gridSizeSpinBox_);
    gridPartLayout->addWidget(gridColorButton_);

    QWidget* widget = new QWidget;
    widget->setLayout(gridPartLayout);

    return widget;
}
