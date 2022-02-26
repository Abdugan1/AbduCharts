#ifndef ZOOMVALIDATOR_H
#define ZOOMVALIDATOR_H

#include <QValidator>

class ZoomValidator : public QValidator
{
public:
    explicit ZoomValidator(QObject* parent = nullptr);

    State validate(QString &input, int &pos) const override;

    void fixup(QString &input) const override;

private:
    enum {
        Bottom = 10,
        Top = 1000
    };
};

#endif // ZOOMVALIDATOR_H
