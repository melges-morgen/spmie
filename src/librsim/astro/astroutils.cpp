#include "astroutils.h"

#include <assert.h>

double astroutils::RandomizeValue(double expected, double sigma)
{
    assert(sigma < 1);
    double normalized_sigma = fabs(expected * sigma);
    std::normal_distribution<double> distribution(expected, normalized_sigma);

    return distribution(astroutils::generator);
}



