//
// Created by Морген Матвей on 12.06.15.
//

#include "radarstation.h"

double RadarStation::DistanceTo(GeoPoint &other)
{
    return GeoPoint::DistanceTo(other);
}

RadarStation::RadarStation(double latitude, double longitude, double altitude,
                           double view_bisector_zenith, double zenith_angle,
                           double view_bisector_azimuth, double azimuth_angle)
        : GeoPoint(latitude, longitude, altitude)
{

}

double RadarStation::ZenithAngleTo(GeoPoint &other)
{
    double r_rs = altitude_ + astroutils::kEarthRadius,
            etta_rs = astroutils::DegToRad(latitude_),
            phi_rs = astroutils::DegToRad(longitude_);
    double z_coord_rs = r_rs * sin(etta_rs),
            y_coord_rs = r_rs * cos(etta_rs) * sin(phi_rs),
            x_coord_rs = r_rs * cos(etta_rs) * cos(phi_rs);

    double r_s = other.GetAltitude() + astroutils::kEarthRadius,
            etta_s = astroutils::DegToRad(other.GetLatitude()),
            phi_s = astroutils::DegToRad(other.GetLongitude());
    double z_coord_s = r_s * sin(etta_s),
            y_coord_s = r_s * cos(etta_s) * sin(phi_s),
            x_coord_s = r_s * cos(etta_s) * cos(phi_s);

    double numenator = (x_coord_s - x_coord_rs) * x_coord_rs +
                       (y_coord_s - y_coord_rs) * y_coord_rs +
                       (z_coord_s - z_coord_rs) * z_coord_rs;

    double denominator =
            ::std::sqrt(
                    std::pow(x_coord_s - x_coord_rs, 2) +
                    std::pow(y_coord_s - y_coord_rs, 2) +
                    std::pow(z_coord_s - z_coord_rs, 2)) *
            ::std::sqrt(
                    std::pow(x_coord_rs, 2) +
                    std::pow(y_coord_rs, 2) +
                    std::pow(z_coord_rs, 2));

    return ::std::asin(std::abs(numenator) / denominator);
}

double RadarStation::AzimuthAngleTo(GeoPoint &other)
{
    return 0;
}
