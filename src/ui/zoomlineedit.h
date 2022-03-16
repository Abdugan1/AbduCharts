#ifndef ZOOMLINEEDIT_H
#define ZOOMLINEEDIT_H

#include <QLineEdit>

class QLabel;

class ZoomLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit ZoomLineEdit(QWidget *parent = nullptr);

private slots:
    void updatePercentLabel(const QString& text);

private:
    QLabel* percentLabel_ = nullptr;
};

#endif // ZOOMLINEEDIT_H
