//
// Created by melge on 01.07.2015.
//

#include <iostream>
#include <iomanip>
#include "ImitationDriver.h"

void ImitationDriver::RunImitation()
{
    int month = 3;
    double flux = 100.0;
    double deg2rad = M_PI / 180, xpdotp = 1440.0 / M_PI * 2;

    std::cout << sizeof(flux) << std::endl;

    int sputnik_number = 25544;
    double epoch_day = 264.51, epoch_year = 2008; //epoch_month = 6;
    double epoch =
            (epoch_year - 1950) * 365 + epoch_day, xbstar = 0.0, xecco = 0.0006703,
            xargpo = 130.5306 * deg2rad, xinclo = 51.6416 * deg2rad,
            xmo = 325.0288 * deg2rad,
            xno = 15.721 / xpdotp, xnodeo = 247.4627 * deg2rad;


    Orbit zarya_orbit(sputnik_number, astroutils::ConvertAstroTimeToUnix(epoch),
                      xbstar, xinclo, xnodeo, xecco, xargpo, xmo, xno);

    //RadarStation radar_station(45.380216, 36.1791801, 40.2, 0, 90, 0, 90);



    std::cout << "Minutes after epoch, "
    << std::setw(8) << "Lat;\t"
    << std::setw(8) << "Long;\t"
    << std::setw(8) << "Alt;\t"
    << std::setw(8) << "Real dist;\t"
    << std::setw(8) << "Seen dist;\t"
    << std::setw(8) << "Delta D,\t"
    << std::setw(8) << "Zenit\t"
    << std::endl;

    RadarStation rls(55.930506, 37.522374, 0, astroutils::DegToRad(50),
                     astroutils::DegToRad(30), astroutils::DegToRad(180),
                     astroutils::DegToRad(90), 300000000);

    for (double minute = 0; minute < 720; minute += 1) {
        // sgp4(wgs84, orbit_param, minute, r, v);
        // ConvertToGeo(r, gr, modf(epoch_day, &int_part) + minute / 1440);
        OrbitPoint point = zarya_orbit.GetTrajectoryPoint(lrint(minute * 60));
        if(!rls.IsInSigh(point))
            continue;

        std::cout << minute << ";\t"
        << std::setw(8) << point.GetLatitude() << ";\t"
        << std::setw(8) << point.GetLongitude() << ";\t"
        << std::setw(8) << point.GetAltitude() << ";\t"
        << std::setw(8) << rls.DistanceTo(point) << ";\t"
        << std::setw(8) << rls.ObservedDistanceTo(point) << ";\t"
        << std::setw(8) << rls.ObservedDistanceTo(point) - rls.DistanceTo(point) << ";\t"
        << std::setw(8) << astroutils::RadToDeg(rls.ZenithAngleTo(point))
        << std::endl;
//  std::cout << minute << " after epoch, " << asin(r[1]) * 180 / M_PI << ", "
//    << asin(r[1] / sqrt((1 - pow(r[0], 2)))) * 180 / M_PI << std::endl;
    }

//    RadarStation rls1(55.930506, 37.522374, 0, 0, 30, 0, 30, 300000000);
//    RadarStation rls2(56.689763, 37.518270, 10000, 0, 30, 0, 30, 300000000);
//    std::cout << astroutils::RadToDeg(rls1.ZenithAngleTo(rls2)) << std::endl;
//    std::cout << astroutils::RadToDeg(rls1.AzimuthAngleTo(rls2)) << std::endl;

//    GeoPoint point2(55.930188, 37.518115, 500);
//    std::cout << astroutils::RadToDeg(point1.AzimuthAngleTo(point2));


//  std::cout << nequickintegral_(&alat1, &along1, &h1,
//                                &alat2, &along2, &h2,
//                                &y, &month, &UT, &flux)
//            << std::endl;
}
