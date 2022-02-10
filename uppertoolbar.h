#ifndef UPPERTOOLBAR_H
#define UPPERTOOLBAR_H

#include <QToolBar>

class QFontComboBox;
class QComboBox;
class QTextCharFormat;
class QTextBlockFormat;
class QToolButton;
class QCheckBox;
class QSpinBox;

class ColorPickerButton;

class UpperToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit UpperToolBar(QWidget* parent = nullptr);

signals:
    void charFormatChanged(const QTextCharFormat& format);
    void blockFormatChanged(const QTextBlockFormat& format);

    void gridSizeValueChanged();
    void gridColorChanged(QColor color);
    void gridEnabledChanged();

public slots:
    void updateCharFormattingResponsiblePart(const QTextCharFormat& format);
    void updateBlockFormattingResponsiblePart(const QTextBlockFormat& format);

private slots:
    void mergeFontFamily(const QString& fontFamily);
    void mergeFontSize(const QString& fontSize);
    void mergeBold();
    void mergeItalic();
    void mergeUnderline();
    void mergeAlignLeft();
    void mergeAlignCenter();
    void mergeAlignRight();

private:
    void setupTextFormattingPart();
    void setupViewPart();

    QWidget* textFormattingPartLayoutedWidget();

    QWidget* viewPartLayoutedWidget();

private:
    QFontComboBox* fontFamilyComboBox_  = nullptr;
    QComboBox*     fontSizeComboBox_    = nullptr;

    QToolButton* textBoldButton_      = nullptr;
    QToolButton* textItalicButton_    = nullptr;
    QToolButton* textUnderlineButton_ = nullptr;

    QToolButton* textAlignLeft_     = nullptr;
    QToolButton* textAlignCenter_   = nullptr;
    QToolButton* textAlignRight_    = nullptr;

    //
    QCheckBox*          gridCheckBox_    = nullptr;
    QSpinBox*           gridSizeSpinBox_ = nullptr;
    ColorPickerButton*  gridColorButton_ = nullptr;
};

#endif // UPPERTOOLBAR_H
