#include "settings.h"

#include <QColorDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <limits>

Settings::Settings(QPointF *functionFieldFrom, QPointF *functionFieldTo, QSize *gridSize, QColor *lineColor, QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QGroupBox *fieldSizeGroup = new QGroupBox(tr("Function field"), this);
    QGridLayout *fieldSizeLayout = new QGridLayout(fieldSizeGroup);

    QGroupBox *gridSizeGroup = new QGroupBox(tr("Grid size"), this);
    QGridLayout *gridSizeLayout = new QGridLayout(gridSizeGroup);

    QGroupBox *colorGroup = new QGroupBox(tr("Set function field"), this);
    QGridLayout *colorLayout = new QGridLayout(colorGroup);

    mainLayout->addWidget(fieldSizeGroup);
    mainLayout->addWidget(gridSizeGroup);
    mainLayout->addWidget(colorGroup);

    QLabel *label1 = new QLabel(tr("From:"), this);
    QLabel *label2 = new QLabel(tr("To:"), this);
    QLabel *label3 = new QLabel(tr("x"), this);
    QLabel *label4 = new QLabel(tr("y"), this);

    QDoubleSpinBox *spinX1 = new QDoubleSpinBox(this);
    QDoubleSpinBox *spinX2 = new QDoubleSpinBox(this);
    QDoubleSpinBox *spinY1 = new QDoubleSpinBox(this);
    QDoubleSpinBox *spinY2 = new QDoubleSpinBox(this);

    spinX1->setRange(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
    spinX2->setRange(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
    spinY1->setRange(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
    spinY2->setRange(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max());

    spinX1->setValue(functionFieldFrom->x());
    spinY1->setValue(functionFieldFrom->y());
    spinX2->setValue(functionFieldTo->x());
    spinY2->setValue(functionFieldTo->y());

    fieldSizeLayout->addWidget(label3, 0, 1);
    fieldSizeLayout->addWidget(label4, 0, 2);
    fieldSizeLayout->addWidget(label1, 1, 0);
    fieldSizeLayout->addWidget(spinX1, 1, 1);
    fieldSizeLayout->addWidget(spinY1, 1, 2);
    fieldSizeLayout->addWidget(spinX2, 2, 1);
    fieldSizeLayout->addWidget(spinY2, 2, 2);
    fieldSizeLayout->addWidget(label2, 2, 0);

    QLabel *label5 = new QLabel(tr("width"), this);
    QLabel *label6 = new QLabel(tr("height"), this);

    QSpinBox *spinW = new QSpinBox(this);
    QSpinBox *spinH = new QSpinBox(this);
    spinW->setMinimum(2);
    spinH->setMinimum(2);
    spinW->setMaximum(500);
    spinH->setMaximum(500);
    spinW->setValue(gridSize->width());
    spinH->setValue(gridSize->height());

    gridSizeLayout->addWidget(label5, 0, 0);
    gridSizeLayout->addWidget(label6, 0, 1);
    gridSizeLayout->addWidget(spinW, 1, 0);
    gridSizeLayout->addWidget(spinH, 1, 1);

    QPushButton *colorButton = new QPushButton(this);
    colorButton->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Button, *lineColor);
    colorButton->setPalette(palette);
    colorButton->setFlat(true);
    colorButton->setMinimumHeight(50);

    connect(colorButton, &QPushButton::clicked, this, [this, colorButton](){
        QColor initial = colorButton->palette().color(QPalette::Button);
        QColor newColor = QColorDialog::getColor(initial, this, tr("Choose line color"));

        if (!newColor.isValid())
            return;

        QPalette palette;
        palette.setColor(QPalette::Button, newColor);
        colorButton->setPalette(palette);
    });

    colorLayout->addWidget(colorButton);

    QDialogButtonBox *buttons = new QDialogButtonBox(this);
    buttons->setOrientation(Qt::Horizontal);
    buttons->setStandardButtons(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);

    mainLayout->addWidget(buttons);


    connect(buttons, &QDialogButtonBox::accepted, this,
            [this, spinW, spinH, spinX1, spinX2, spinY1, spinY2,
            functionFieldFrom, functionFieldTo, lineColor, gridSize, colorButton](){

                if (spinX1->value() >= spinX2->value()
                        || spinY1->value() >= spinY2->value()){
                    QMessageBox::warning(this, "Incorrect settings", "'From' should be less than 'to'");
                    return;
                }

            functionFieldFrom->setX(spinX1->value());
            functionFieldFrom->setY(spinY1->value());
            functionFieldTo->setX(spinX2->value());
            functionFieldTo->setY(spinY2->value());

            gridSize->setWidth(spinW->value());
            gridSize->setHeight(spinH->value());

            *lineColor = colorButton->palette().color(QPalette::Button);

        accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, this, &Settings::reject);

}
