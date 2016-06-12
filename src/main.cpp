#include <getopt.h>
#include <iostream>
#include <cstring>


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
//int calculate

int main(int argc, char **argv)
{
    try {
        std::map<int, Orbit> orbits =
            TLEReader::ReadSatellitesFromFile("data/visual.tle");
        ImitationDriver imitation_driver;
        imitation_driver.addSatellite(orbits[25544]);

        RadarStation radar("Experemntal Radar", 8.95, 50.67, 500,
                           astroutils::DegToRad(45), astroutils::DegToRad(45),
                           astroutils::DegToRad(70), astroutils::DegToRad(135),
                           70000000, 178
        );
        imitation_driver.addRadarStation(radar);

        auto simulation_report =
                imitation_driver.RunImitation(1465416819, 1465576329);

        std::list<SightObject> result_objects;
        for(auto it = simulation_report.begin(); it != simulation_report.end();
                ++it)
        {
            auto observation_report = it->second;
            for(auto observation_iterator = observation_report.begin();
                    observation_iterator != observation_report.end();
                    ++observation_iterator) {
                if(observation_iterator->GetObservedObjects().size() < 1)
                    continue;
                auto observe = *observation_iterator;
                result_objects.push_back(observe.GetObservedObjects()[0]);
            }
        }

        if(result_objects.size() < 1)
        {
            std::cout << "No observation" << std::endl;
            exit(EXIT_FAILURE);
        }

        fit_f(result_objects, orbits[25544], radar);
        fit(result_objects, orbits[25544], radar);

    } catch (TLEFormatException &e) {
        std::cout << e.what();
    }

    exit(EXIT_SUCCESS);
}
