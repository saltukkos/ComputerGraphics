#include "filterconvolving.h"

#include <cassert>
#include <cmath>

#include "utils.h"

void FilterConvolving::doFilter(const QImage *input, QImage *output)
{
    std::vector<float> matrix = getMatrix();

    unsigned n = sqrt(matrix.size());
    assert(n*n == matrix.size() && n > 0);

    perPixelLoop(input, output, [&matrix, n](const QImage *image, int x, int y){
        Utils::Color color = Utils::convolvePixel(image, x, y, matrix, n);

        int ri = Utils::trimColor(qRound(color.red));
        int gi = Utils::trimColor(qRound(color.green));
        int bi = Utils::trimColor(qRound(color.blue));

        return QColor(ri, gi, bi);

    });
}
