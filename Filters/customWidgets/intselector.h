#ifndef INTSELECTOR_H
#define INTSELECTOR_H

#include <QSpinBox>
#include <QWidget>

class IntSelector : public QWidget
{
    Q_OBJECT
public:
    explicit IntSelector(int min, int max, const QString &text, QWidget *parent = 0);

    int getValue();

signals:
    void valueChanged(int);

public slots:
    void setValue(int value);

private:
    QSpinBox *spinBox;
};

#endif // INTSELECTOR_H
