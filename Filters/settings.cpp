#include "settings.h"

#include <QVBoxLayout>

#include "constants.h"

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    name(new QLabel(this)),
    area(new QScrollArea(this))
{
    QFont font = name->font();
    font.setBold(true);
    font.setPixelSize(Constants::filterNameSize);
    name->setFont(font);

    buttons = new QDialogButtonBox(
                QDialogButtonBox::Ok|QDialogButtonBox::Cancel,
                Qt::Horizontal,
                this);

    connect(buttons, &QDialogButtonBox::accepted, [this](){
        emit buttonPressed(QDialogButtonBox::Ok);
    });
    connect(buttons, &QDialogButtonBox::rejected, [this](){
        emit buttonPressed(QDialogButtonBox::Cancel);
    });

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(name, 0);
    layout->addWidget(area, 1);
    layout->addWidget(buttons, 0);

    area->setMinimumHeight(Constants::settingsMinimumHeight);
    area->setFrameStyle(0);
}

Settings::~Settings()
{
    area->takeWidget();
}

void Settings::setParametersWidget(QWidget *w)
{
    area->takeWidget();
    area->setWidget(w);
}

void Settings::setFilterName(QString n)
{
    name->setText(n);
}

void Settings::setButtonsEnable(bool enable)
{
    buttons->setEnabled(enable);
}
