#include <getopt.h>
#include <iostream>
#include <cstring>
#include <librsim/astro/orbitresolver.h>


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

double get_discrepancy(Orbit &real, Orbit &other)
{
    return sqrt(
            pow(real.GetDragCoefficient() - other.GetDragCoefficient(), 2)
            + pow(real.GetInclinationAngle() - other.GetInclinationAngle(), 2)
            + pow(real.GetAscendingNode() - other.GetAscendingNode(), 2)
            + pow(real.GetEccentricity() - other.GetEccentricity(), 2)
            + pow(real.GetApsisArgument() - other.GetApsisArgument(), 2)
            + pow(real.GetMeanAnomaly() - other.GetMeanAnomaly(), 2)
            + pow(real.GetMeanMotion() - other.GetMeanMotion(), 2)
    );
}

void print_difference(Orbit &real, Orbit &with_flux, Orbit without_flux,
                      double noise, double flux)
{
    double discrepancy_flux = get_discrepancy(real, with_flux);
    double discrepancy_no_flux = get_discrepancy(real, without_flux);

    printf("0,%f,%f,%f,flux\n", noise, flux, discrepancy_flux);
    printf("0,%f,%f,%f,no_flux\n", noise, flux, discrepancy_no_flux);
}

void run_imitation(Orbit &target_orbit, double target_sigma, double target_flux)
{
    ImitationDriver imitation_driver;
    imitation_driver.addSatellite(target_orbit);

    RadarStation radar("Experemntal Radar", 8.95, 50.67, 500,
                       astroutils::DegToRad(45), astroutils::DegToRad(45),
                       astroutils::DegToRad(70), astroutils::DegToRad(135),
                       70000000, target_flux, target_sigma
    );
    imitation_driver.addRadarStation(radar);

    auto simulation_report =
            imitation_driver.RunImitation(1465429869, 1465566894);

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
//    std::cout << "id,Time,Distance Delta,Azimuth Delta,Zenith Delta"
//    << std::endl;
//    for(SightObject &object : result_objects)
//    {
////        printf("%d,%lli,%f,%f,%f\n",
////               object.GetObjectId(), object.GetObservationTime(),
////               object.DistanceDelta(),
////               object.AzimuthDelta(), object.ZenithDelta());
//    }

    if(result_objects.size() < 1)
    {
        std::cout << "No observation" << std::endl;
        exit(EXIT_FAILURE);
    }

    Orbit restored_orbit = orbitresolver::RestoreOrbitConsideringFlux(
            target_orbit, result_objects, radar);
    Orbit restored_orbit_no_flux =
            orbitresolver::RestoreOrbitNoFlux(
                    target_orbit, result_objects, radar);

    print_difference(target_orbit, restored_orbit, restored_orbit_no_flux,
                     target_sigma, target_flux);
}

int main(int argc, char **argv)
{
    try {
        std::map<int, Orbit> orbits =
            TLEReader::ReadSatellitesFromFile("data/visual.tle");
        printf("d,noise,flux,discrepancy,type\n");
        for(double noise = 0.005; noise < 0.3; noise += 0.007)
            run_imitation(orbits[25544], noise, 178);

    } catch (TLEFormatException &e) {
        std::cout << e.what();
    }

    exit(EXIT_SUCCESS);
}
