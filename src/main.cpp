#include <getopt.h>
#include <iostream>


#include "librsim/ImitationDriver.h"
#include "librsim/astro/TLEReader.h"

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
    imitationDriver.RunImitation(10, 1100, 0);
    try {
        std::map<int, Orbit> orbits =
            TLEReader::ReadSatellitesFromFile("tle_test.txt");

        for(std::map<int, Orbit>::iterator it = orbits.begin();
            it != orbits.end(); ++it)
        {
            std::cout << it->first << " " << it->second.GetSatelliteNumber();
        }
    } catch (TLEFormatException &e) {
        std::cout << e.what();
    }
    exit(EXIT_SUCCESS);
}
