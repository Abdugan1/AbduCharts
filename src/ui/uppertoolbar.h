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
    void gridColorChanged(const QColor& color);
    void gridEnableStateChanged();

    void viewBackgroundEnableStateChanged(bool enabled);
    void viewBackgroundColorChanged(const QColor& color);

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

    void setupToolButton(QToolButton* button,
                         const QIcon& icon,
                         const QKeySequence& shortcut);

    QWidget* textFormattingPartLayoutedWidget();

    QWidget* viewPartLayoutedWidget();

private:
    // font options
    QFontComboBox* fontFamilyComboBox_  = nullptr;
    QComboBox*     fontSizeComboBox_    = nullptr;

    QToolButton* textBoldButton_      = nullptr;
    QToolButton* textItalicButton_    = nullptr;
    QToolButton* textUnderlineButton_ = nullptr;

    QToolButton* textAlignLeft_     = nullptr;
    QToolButton* textAlignCenter_   = nullptr;
    QToolButton* textAlignRight_    = nullptr;

    // commands
    QToolButton* undoButton_ = nullptr;
    QToolButton* redoButton_ = nullptr;

    // view options
    QCheckBox*          gridCheckBox_    = nullptr;
    QSpinBox*           gridSizeSpinBox_ = nullptr;
    ColorPickerButton*  gridColorButton_ = nullptr;

    QCheckBox*          backgroundCheckBox_ = nullptr;
    ColorPickerButton*  backgroundColorButton_ = nullptr;
};

#endif // UPPERTOOLBAR_H
