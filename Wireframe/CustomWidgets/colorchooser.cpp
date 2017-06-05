#include "colorchooser.h"

#include <QColorDialog>

ColorChooser::ColorChooser(QColor initial, QString text, QWidget *parent) :
    QPushButton(parent)
{
    setText(text);
    setFlat(true);
    setAutoFillBackground(true);

    setColor(initial);

    connect(this, &QPushButton::clicked, this, [this, parent](){
        QColor color = QColorDialog::getColor
        (
            palette().color(QPalette::Button),
            parent
        );

        if (!color.isValid())
            return;

        setColor(color);
        emit colorSelected(selected);
    });
}

void ColorChooser::setColor(QColor color)
{
    selected = color;
    QPalette palette;

    palette.setColor(QPalette::Button, color);

    int gray = 0.2126f * color.red() + 0.7152f * color.green() + 0.0722f * color.blue();
    palette.setColor(QPalette::Text, gray > 150 ? Qt::black : Qt::white);

    setPalette(palette);
}
