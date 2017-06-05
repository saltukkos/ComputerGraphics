#include "filtersharp.h"

#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>

#include "filterfactory.h"
AUTOREGISTER(FilterSharp)

static std::vector<float> kernels[]{
    {
        0,  -1, 0,
        -1, 5,  -1,
        0,  -1, 0
    },
    {
        1/-256.0, 4/-256.0, 6/-256.0, 4/-256.0, 1/-256.0,
        4/-256.0, 16/-256.0, 24/-256.0, 16/-256.0, 4/-256.0,
        6/-256.0, 24/-256.0, 476/256.0, 24/-256.0, 6/-256.0,
        4/-256.0, 16/-256.0, 24/-256.0, 16/-256.0, 4/-256.0,
        1/-256.0, 4/-256.0, 6/-256.0, 4/-256.0, 1/-256.0
    }
};


FilterSharp::FilterSharp() :
    currentType(0)
{
    QWidget *settingsWidget = getSettingsWidget();
    qDeleteAll(settingsWidget->children());

    QVBoxLayout *layout = new QVBoxLayout(settingsWidget);

    QComboBox *type = new QComboBox(settingsWidget);
    type->addItems({"Standard 3x3", "Gaussian-based 5x5"});

    connect(type, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, [this](int i){
        currentType = i;
        emit settingsChanged();
    });

    layout->addWidget(new QLabel("Type:", settingsWidget));
    layout->addWidget(type);

}

QKeySequence FilterSharp::getShortcut()
{
    return QKeySequence("Ctrl+H");
}

QString FilterSharp::getText()
{
    return "Sharpen";
}

QString FilterSharp::getDescription()
{
    return "Sharpen";
}

std::vector<float> FilterSharp::getMatrix()
{
    return kernels[currentType];
}
