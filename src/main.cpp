#include <getopt.h>

#include <cmath>
#include <iostream>
#include <iomanip>

#include "librsim/astro/orbitcalculator.h"
#include "librsim/astro/astroutils.h"
#include "librsim/nequick/nequick.h"
#include "librsim/astro/radarstation.h"
#include "librsim/ImitationDriver.h"

/*static const char *opt_string = "h";

static const struct option long_opts[] =
    {
        {"start-latitude",  required_argument, NULL, 1},
        {"start-longitude", required_argument, NULL, 2},
        {"end-latitude",    required_argument, NULL, 3},
        {"end-longitude",   required_argument, NULL, 4}
    };*/
int main(int argc, char **argv)
{
    //setlocale(LC_ALL, "Russian");
    ImitationDriver imitationDriver = ImitationDriver();
    imitationDriver.RunImitation();
}
