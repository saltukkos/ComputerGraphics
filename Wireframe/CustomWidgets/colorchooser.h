#ifndef COLORCHOOSER_H
#define COLORCHOOSER_H

#include <QPushButton>
#include <QColor>

class ColorChooser : public QPushButton
{
    Q_OBJECT

public:
    ColorChooser(QColor initial = Qt::white, QString text = QString(), QWidget *parent = nullptr);

    void setColor(QColor color);

signals:
    void colorSelected(QColor color);

private:
    QColor selected;
};

#endif // COLORCHOOSER_H
