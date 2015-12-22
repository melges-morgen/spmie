//
// Created by melge on 01.07.2015.
//

#include <iostream>
#include <iomanip>
#include "ImitationDriver.h"

std::map<std::string, std::vector<SightReport>>
ImitationDriver::RunImitation(time_t from, time_t to)
{
    int month = 3;
    double flux = 100.0;
    double deg2rad = M_PI / 180, xpdotp = 1440.0 / M_PI * 2;

    int sputnik_number = 25544;
    double epoch_day = 264.51, epoch_year = 2008; //epoch_month = 6;
    double epoch = (epoch_year - 1950) * 365 + epoch_day, xbstar = 0.0,
        xecco = 0.0006703,
        xargpo = 130.5306 * deg2rad, xinclo = 51.6416 * deg2rad,
        xmo = 325.0288 * deg2rad,
        xno = 15.721 / xpdotp, xnodeo = 247.4627 * deg2rad;


    Orbit zarya_orbit(sputnik_number, astroutils::ConvertAstroTimeToUnix(epoch),
                      xbstar, xinclo, xnodeo, xecco, xargpo, xmo, xno);

    //RadarStation radar_station(45.380216, 36.1791801, 40.2, 0, 90, 0, 90);



//    std::cout << "Minutes after epoch, "
//    << std::setw(10) << "Lat;\t"
//    << std::setw(10) << "Long;\t"
//    << std::setw(10) << "Alt;\t"
//    << std::setw(10) << "Real dist;\t"
//    << std::setw(10) << "Calculated dist;\t"
//    << std::setw(10) << "Delta D,\t"
//    << std::setw(10) << "Zenit\t"
//    << std::endl;

    RadarStation rls(55.930506, 37.522374, 0, astroutils::DegToRad(50),
                     astroutils::DegToRad(30), astroutils::DegToRad(180),
                     astroutils::DegToRad(90), 300000000);

    for (double minute = 0; minute < 720; minute += 1) {
        // sgp4(wgs84, orbit_param, minute, r, v);
        // ConvertToGeo(r, gr, modf(epoch_day, &int_part) + minute / 1440);
        OrbitPoint point = zarya_orbit.GetTrajectoryPoint(lrint(minute * 60));
        if(!rls.IsInSigh(point))
            continue;

//        std::cout.precision(5);
//        std::cout << minute << ";\t"
//        << std::setw(10) << point.GetLatitude() << ";\t"
//        << std::setw(10) << point.GetLongitude() << ";\t"
//        << std::setw(10) << point.GetAltitude() << ";\t"
//        << std::setw(10) << rls.DistanceTo(point) << ";\t"
//        << std::setw(10) << rls.ObservedDistanceTo(point) << ";\t"
//        << std::setw(10) << rls.ObservedDistanceTo(point, 100) - rls.DistanceTo(point) << ";\t"
//        << std::setw(10) << astroutils::RadToDeg(rls.ZenithAngleTo(point))
//        << std::endl;
//  std::cout << minute << " after epoch, " << asin(r[1]) * 180 / M_PI << ", "
//    << asin(r[1] / sqrt((1 - pow(r[0], 2)))) * 180 / M_PI << std::endl;
    }

    std::cout << "Real orbit (epoch, xbstar, xinclo, xnodeo, xecco, "
                     "xargpo, xmo, xno): ";
    std::cout << xbstar << "; "
    << xinclo << "; "
    << xnodeo << "; "
    << xecco << "; "
    << xargpo << "; "
    << xmo << "; "
    << xno << "; "
    << std::endl;

    double reg_error = 0.1;
    double xbstarR = 0.0,
        xincloR = astroutils::RandomizeValue(xinclo, xinclo * reg_error),
        xeccoR = astroutils::RandomizeValue(xecco, xecco * reg_error),
        xargpoR = astroutils::RandomizeValue(xargpo, xargpo * reg_error),
        xmoR = astroutils::RandomizeValue(xmo, xmo * reg_error),
        xnoR = astroutils::RandomizeValue(xno, xno * reg_error),
        xnodeoR = astroutils::RandomizeValue(xnodeo, xnodeo * reg_error);

    std::cout << "Calculated orbit (epoch, xbstar, xinclo, xnodeo, xecco, "
        "xargpo, xmo, xno): "
    << xbstarR << "; "
    << xincloR << "; "
    << xnodeoR << "; "
    << xeccoR << "; "
    << xargpoR << "; "
    << xmoR << "; "
    << xnoR << "; "
    << std::endl;

    std::cout << "Deviation: " << sqrt(pow(xbstar - xbstarR, 2)
                                  + pow(xinclo - xincloR, 2)
                                  + pow(xnodeo - xnodeoR, 2)
                                  + pow(xecco - xeccoR, 2)
                                  + pow(xargpo - xargpoR, 2)
                                  + pow(xmo - xmoR, 2)
                                  + pow(xno - xnoR, 2)) << std::endl;

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

    return std::map<std::string, std::vector<SightReport>>();
}
