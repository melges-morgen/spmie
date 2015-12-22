#include "astroutils.h"

double astroutils::RandomizeValue(double expected, double sigma)
{
    std::normal_distribution<double> distribution(expected, sigma);

    return distribution(astroutils::generator);
}



