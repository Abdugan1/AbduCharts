#ifndef UPPERTOOLBAR_H
#define UPPERTOOLBAR_H

#include <QToolBar>

class QFontComboBox;
class QComboBox;
class QTextCharFormat;

class UpperToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit UpperToolBar(QWidget* parent = nullptr);

signals:
    void fontFormatChanged(const QTextCharFormat& format);

public slots:
    void updateTextFormattingResponsiblePart(const QTextCharFormat& format);

private slots:
    void mergeFontFamily(const QString& fontFamily);
    void mergeFontSize(const QString& fontSize);
    void mergeBold();
    void mergeItalic();
    void mergeUnderline();

private:
    void addTextFormattingResponsiblePart();
    void initTextFormattingResponsiblePart();

private:
    QFontComboBox* fontFamilyComboBox_  = nullptr;
    QComboBox*     fontSizeComboBox_    = nullptr;

    QToolButton* textBoldButton_      = nullptr;
    QToolButton* textItalicButton_    = nullptr;
    QToolButton* textUnderlineButton_ = nullptr;
};

#endif // UPPERTOOLBAR_H
