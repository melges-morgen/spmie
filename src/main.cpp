#include <getopt.h>

#include <cmath>
#include <iostream>
#include <iomanip>

#include "orbitcalculator.h"
#include "astroutils.h"

static const char *opt_string = "h";

static const struct option long_opts[] =
    {
        {"start-latitude",  required_argument, NULL, 1},
        {"start-longitude", required_argument, NULL, 2},
        {"end-latitude",    required_argument, NULL, 3},
        {"end-longitude",   required_argument, NULL, 4}
    };

int main(int argc, char **argv)
{
    double alat1 = 55.930825, along1 = 37.522267, h1 = 0.5,
        alat2 = 55.926840, along2 = 37.530721, h2 = 300.0, UT = 16.5,
        flux = 100.0;
    int y = 2015, month = 3;

    double deg2rad = M_PI / 180, xpdotp = 1440.0 / M_PI * 2;

    int sputnik_number = 25544;
    double epoch_day = 264.51, epoch_year = 2008;
    double epoch =
        (epoch_year - 1950) * 365 + epoch_day, xbstar = 0.0, xecco = 0.0006703,
        xargpo = 130.5306 * deg2rad, xinclo = 51.6416 * deg2rad,
        xmo = 325.0288 * deg2rad,
        xno = 15.721 / xpdotp, xnodeo = 247.4627 * deg2rad;


    Orbit zarya_orbit(sputnik_number, astroutils::ConvertAstroTimeToUnix(epoch),
                      xbstar, xinclo, xnodeo, xecco, xargpo, xmo, xno);


//  elsetrec orbit_param;
//  sgp4init(wgs84, sputnik_number, epoch, xbstar, xecco, xargpo, 
//      xinclo, xmo, xno, xnodeo,
//      orbit_param);

//  double r[3], gr[3], v[3];
    std::cout << "Минута после эпохи, "
    << std::setw(8) << "Широта,\t"
    << std::setw(8) << "Долгота,\t"
    << std::setw(8) << "Высота,\t"
    << std::setw(8) << "X,\t"
    << std::setw(8) << "Y,\t"
    << std::setw(8) << "Z" << std::endl;
//  double int_part;
    for (double minute = 140; minute < 720; minute += 1) {
        // sgp4(wgs84, orbit_param, minute, r, v);
        // ConvertToGeo(r, gr, modf(epoch_day, &int_part) + minute / 1440);
        OrbitPoint point = zarya_orbit.GetTrajectoryPoint(minute * 60);
        std::cout << minute << " минута после эпохи, "
        << std::setw(8) << point.GetLatitude() << ",\t"
        << std::setw(8) << point.GetLongitude() << ",\t"
        << std::setw(8) << point.GetAltitude() << ",\t"
        << std::setw(8) << point.GetX() << ",\t"
        << std::setw(8) << point.GetY() << ",\t"
        << std::setw(8) << point.GetZ() << std::endl;
//  std::cout << minute << " after epoch, " << asin(r[1]) * 180 / M_PI << ", "
//    << asin(r[1] / sqrt((1 - pow(r[0], 2)))) * 180 / M_PI << std::endl;
    }

    GeoPoint point1(55.930506, 37.522374, 500);
    GeoPoint point2(55.930188, 37.518115, 500);

    std::cout << point1.DestanceTo(point2) << std::endl;
//  std::cout << nequickintegral_(&alat1, &along1, &h1,
//                                &alat2, &along2, &h2,
//                                &y, &month, &UT, &flux) 
//            << std::endl;

    return 0;
}
