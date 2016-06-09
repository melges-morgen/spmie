//
// Created by Морген Матвей on 12.06.15.
//

#include "radarstation.h"
#include "librsim/nequick/nequick.h"
RadarStation::RadarStation() : GeoPoint()
{
}

RadarStation::RadarStation(std::string radar_name, double latitude, double longitude,
                           double altitude, double view_bisector_zenith,
                           double zenith_angle, double view_bisector_azimuth,
                           double azimuth_angle, double frequency, double local_flux)
        : GeoPoint(latitude, longitude, altitude),
          name_(radar_name),
          view_bisector_zenith_(view_bisector_zenith),
          zenith_angle_(zenith_angle),
          view_bisector_azimuth_(view_bisector_azimuth),
          azimuth_angle_(azimuth_angle),
          frequency_(frequency),
          local_flux_(local_flux)
{

}

double RadarStation::ObservedDistanceTo(OrbitPoint &distance_point,
                                        double sigma)
{
    if(!IsInSigh(distance_point))
        return -1;

    time_t orbit_point_time = distance_point.GetTime();
    struct tm *time_info = gmtime(&orbit_point_time);

    int orbit_year = time_info->tm_year + 1900;
    double orbit_part_of_day = time_info->tm_hour / 24;

    double observed = DistanceTo(distance_point) + nequick::ElectronContent(
        GetLatitude(), GetLongitude(), GetAltitude(),
        distance_point.GetLatitude(), distance_point.GetLongitude(),
        distance_point.GetAltitude(),
        GetLocalFlux(), orbit_year, time_info->tm_mon,
        orbit_part_of_day,
        GetWorkFrequency()
    );

    return astroutils::RandomizeValue(observed, sigma);
}

double RadarStation::ZenithAngleTo(GeoPoint &other, double sigma)
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
            std::sqrt(
                    std::pow(x_coord_s - x_coord_rs, 2) +
                    std::pow(y_coord_s - y_coord_rs, 2) +
                    std::pow(z_coord_s - z_coord_rs, 2)) *
            std::sqrt(
                    std::pow(x_coord_rs, 2) +
                    std::pow(y_coord_rs, 2) +
                    std::pow(z_coord_rs, 2));

    return astroutils::RandomizeValue(
        std::asin(numenator / denominator), sigma
    );
}

double RadarStation::AzimuthAngleTo(GeoPoint &other, double sigma)
{
    double latitude1_rad = astroutils::DegToRad(latitude_);
    double latitude2_rad = astroutils::DegToRad(other.GetLatitude());
    double longitude1_rad = astroutils::DegToRad(longitude_);
    double longitude2_rad = astroutils::DegToRad(other.GetLongitude());

    double cl1 = cos(latitude1_rad);
    double cl2 = cos(latitude2_rad);
    double sl1 = sin(latitude1_rad);
    double sl2 = sin(latitude2_rad);

    double delta = longitude2_rad - longitude1_rad;
    double cdelta = cos(delta);
    double sdelta = sin(delta);

    double x = (cl1 * sl2) - (sl1 * cl2 * cdelta);
    double y = sdelta * cl2;
    double z = atan(-y / x);

    if (x < 0)
        z = z + M_PI;

    double z2 = remainder(z + M_PI,  2 * M_PI)- M_PI;
    double anglerad2 = z2 - ((2 * M_PI) * floor((z2 / (2 * M_PI))));
    return astroutils::RandomizeValue(anglerad2, sigma);
}

bool RadarStation::IsInSigh(GeoPoint &point)
{
    // Check that point in sight
    if(AzimuthAngleTo(point) - view_bisector_azimuth_ > azimuth_angle_)
        return false;
    double zenit_angle = ZenithAngleTo(point);
    return std::abs(ZenithAngleTo(point) - view_bisector_zenith_)
           <= zenith_angle_
           && zenit_angle > 0;
}
