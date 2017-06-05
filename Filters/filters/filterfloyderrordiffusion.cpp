#include "filterfloyderrordiffusion.h"

#include <QHBoxLayout>

#include "customWidgets/intselector.h"
#include "filterfactory.h"
#include "utils.h"

AUTOREGISTER(FilterFloydErrorDiffusion)

FilterFloydErrorDiffusion::FilterFloydErrorDiffusion():
    rCount(3),
    gCount(3),
    bCount(2)
{
    QWidget *settingsWidget = getSettingsWidget();
    qDeleteAll(settingsWidget->children());

    QHBoxLayout *layout = new QHBoxLayout(settingsWidget);

    IntSelector *selectorR = new IntSelector(2, 256, "Number of shadows of red");
    layout->addWidget(selectorR);

    IntSelector *selectorG = new IntSelector(2, 256, "Number of shadows of green");
    layout->addWidget(selectorG);

    IntSelector *selectorB = new IntSelector(2, 256, "Number of shadows of blue");
    layout->addWidget(selectorB);

    connect(selectorR, &IntSelector::valueChanged, this, [this](int v){
        rCount = v;
        emit settingsChanged();
    });
    connect(selectorG, &IntSelector::valueChanged, this, [this](int v){
        gCount = v;
        emit settingsChanged();
    });
    connect(selectorB, &IntSelector::valueChanged, this, [this](int v){
        bCount = v;
        emit settingsChanged();
    });

    selectorR->setValue(rCount);
    selectorG->setValue(gCount);
    selectorB->setValue(bCount);
}

QKeySequence FilterFloydErrorDiffusion::getShortcut()
{
    return QKeySequence("Ctrl+F");
}

QString FilterFloydErrorDiffusion::getText()
{
    return "Floyd–Steinberg dithering";
}

QString FilterFloydErrorDiffusion::getDescription()
{
    return "Error-diffusion sithering algorithm";
}

static const QPoint shifts[]{
    {1, 0},
    {-1, 1},
    {0, 1},
    {1, 1}
};

static const float diffusion[]{
    7./16, 3./16, 5./16, 1.16
};

static int findClosest(float actual, float shadows){
    float space = 256/(shadows - 1);
    int n = qRound(actual/(space));
    return Utils::trimColor(space*n);
}

void FilterFloydErrorDiffusion::doFilter(const QImage *input, QImage *output)
{
    std::vector<Utils::Color> errors;
    errors.resize(input->width()*input->height());
    int rn = rCount;
    int gn = gCount;
    int bn = gCount;

    perPixelLoop(input, output, [&errors, rn, gn, bn](const QImage *image, int x, int y){
        QColor color = QColor(*Utils::getImagePixel(image, x, y));
        float red = color.red() + errors[x + y*image->width()].red;
        float green = color.green() + errors[x + y*image->width()].green;
        float blue = color.blue() + errors[x + y*image->width()].blue;

        int newRed = findClosest(red, rn);
        int newGreen = findClosest(green, gn);
        int newBlue = findClosest(blue, bn);

        for (int i = 0; i < 4; ++i){
            int xp = x + shifts[i].x();
            int yp = image->width()*(y + shifts[i].y());

            if (xp < 0 || yp < 0 || xp >= image->width() || yp >= image->height())
                continue;

            Utils::Color &err = errors[xp + yp];

            err.red += diffusion[i]*(newRed - red);
            err.green += diffusion[i]*(newGreen - green);
            err.blue += diffusion[i]*(newBlue - blue);
        }

        return QColor(newRed, newGreen, newBlue);
    });
}

