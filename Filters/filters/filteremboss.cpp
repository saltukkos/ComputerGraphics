#include "filteremboss.h"

#include "utils.h"

#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>

#include "filterfactory.h"
AUTOREGISTER(FilterEmboss)

static std::vector<float> kernels[]{
    {
        0,  -1,  0,
        1,  0,  -1,
        0,  1,  0
    },
    {
        0,  1,  0,
        1,  0,  -1,
        0,  -1,  0
    },
    {
        0,  1,  0,
        -1,  0,  1,
        0,  -1,  0
    },
    {
        0,  -1,  0,
        -1,  0,  1,
        0,  1,  0
    }
};

FilterEmboss::FilterEmboss() :
    direction(0)
{
    QWidget *settingsWidget = getSettingsWidget();
    qDeleteAll(settingsWidget->children());

    QVBoxLayout *layout = new QVBoxLayout(settingsWidget);

    QComboBox *dirBox = new QComboBox(settingsWidget);
    dirBox->addItems({"Top-right", "Bottom-right", "Bottom-left", "Top-left"});

    connect(dirBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, [this](int i){
        direction = i;
        emit settingsChanged();
    });

    layout->addWidget(new QLabel("Emboss direction:", settingsWidget));
    layout->addWidget(dirBox);
}

QKeySequence FilterEmboss::getShortcut()
{
    return QKeySequence("Ctrl+E");
}

QString FilterEmboss::getText()
{
    return "Emboss";
}

QString FilterEmboss::getDescription()
{
    return "Emboss";
}

void FilterEmboss::doFilter(const QImage *input, QImage *output)
{
    const std::vector<float> &matrix = kernels[direction];

    perPixelLoop(input, output, [&matrix](const QImage *image, int x, int y){
        Utils::Color color = Utils::convolvePixel(image, x, y, matrix, 3);

        int ri = Utils::trimColor(qRound(color.red + 128));
        int gi = Utils::trimColor(qRound(color.green + 128));
        int bi = Utils::trimColor(qRound(color.blue + 128));

        return QColor(ri, gi, bi);

    });
}

