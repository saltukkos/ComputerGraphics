#include "filterchess.h"

#include <QVBoxLayout>

#include "customWidgets/intselector.h"
#include "filterfactory.h"

AUTOREGISTER(FilterChess)

FilterChess::FilterChess() :
    size(1)
{
    QWidget *settingsWidget = getSettingsWidget();
    qDeleteAll(settingsWidget->children());

    QVBoxLayout *layout = new QVBoxLayout(settingsWidget);

    IntSelector *selector = new IntSelector(1, 200, "Cell size:");
    layout->addWidget(selector);

    connect(selector, &IntSelector::valueChanged, this, [this](int v){
        size = v;
        emit settingsChanged();
    });

    selector->setValue(size);
}

QString FilterChess::getText()
{
    return "Chess board";
}

QString FilterChess::getDescription()
{
    return "Generate chess board";
}

void FilterChess::doFilter(const QImage *input, QImage *output)
{
    int _size = size;
    perPixelLoop(input, output, [_size](const QImage *, int x, int y){
        bool b = (x / _size) % 2 == (y / _size) % 2;
        int color = b ? 0 : 255;
        return QColor(color, color, color);
    });
}

