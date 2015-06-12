#include "coordpoints.h"

#include <cmath>

#include "astroutils.h"

GeoPoint::GeoPoint()
{

}

GeoPoint::GeoPoint(double latitude, double longitude, double altitude)
        : latitude_(latitude),
          longitude_(longitude),
          altitude_(altitude)
{

}

void GeoPoint::SetLatitude(double latitude)
{
    latitude_ = latitude;
}

void GeoPoint::SetLongitude(double longitude)
{
    longitude_ = longitude;
}

void GeoPoint::SetAltitude(double altitude)
{
    altitude_ = altitude;
}

double GeoPoint::DistanceTo(GeoPoint &other)
{
    double r1 = altitude_ + astroutils::kEarthRadius,
            etta1 = astroutils::DegToRad(latitude_),
            phi1 = astroutils::DegToRad(longitude_);
    double z_coord1 = r1 * sin(etta1),
            y_coord1 = r1 * cos(etta1) * sin(phi1),
            x_coord1 = r1 * cos(etta1) * cos(phi1);

    double r2 = other.altitude_ + astroutils::kEarthRadius,
            etta2 = astroutils::DegToRad(other.latitude_),
            phi2 = astroutils::DegToRad(other.longitude_);
    double z_coord2 = r2 * sin(etta2),
            y_coord2 = r2 * cos(etta2) * sin(phi2),
            x_coord2 = r2 * cos(etta2) * cos(phi2);


    double coord_sqr = pow(x_coord1 - x_coord2, 2) +
                       pow(y_coord1 - y_coord2, 2) +
                       pow(z_coord1 - z_coord2, 2);

    return sqrt(coord_sqr);
}

OrbitPoint::OrbitPoint()
{

}

OrbitPoint::OrbitPoint(double x_coord, double y_coord, double z_coord,
                       time_t timestamp)
        : x_coord_(x_coord),
          y_coord_(y_coord),
          z_coord_(z_coord),
          timestamp_(timestamp),
          invalid_(false)
{
    double r_v = sqrt(pow(x_coord_, 2) + pow(y_coord_, 2) + pow(z_coord_, 2));
    earth_rotated_angle_ =
            astroutils::kEarthRotationSpeed *
            astroutils::SecondsSinceMidnight(timestamp_);

    zenith_ = asin(z_coord_ / r_v);
    azimuth_ = remainder(atan(y_coord_ / x_coord_), 2 * M_PI);
    r_length_ = r_v * 1000;

    latitude_ = zenith_ * 180 / M_PI;
    longitude_ = (azimuth_ - earth_rotated_angle_) * 180 / M_PI;
    altitude_ = r_length_ - astroutils::kEarthRadius;

    if (x_coord_ < 0)
        longitude_ += 180;
    if (longitude_ > 180)
        longitude_ -= 360;
    if (longitude_ < -180)
        longitude_ += 360;
}

OrbitPoint::OrbitPoint(const OrbitPoint &other)
        : x_coord_(other.x_coord_),
          y_coord_(other.y_coord_),
          z_coord_(other.z_coord_),
          zenith_(other.zenith_),
          azimuth_(other.azimuth_),
          r_length_(other.r_length_),
          timestamp_(other.timestamp_),
          invalid_(other.invalid_),
          earth_rotated_angle_(other.earth_rotated_angle_) { }

OrbitPoint &OrbitPoint::operator=(const OrbitPoint &other)
{
    x_coord_ = other.x_coord_;
    y_coord_ = other.y_coord_;
    z_coord_ = other.z_coord_;
    zenith_ = other.zenith_;
    azimuth_ = other.azimuth_;
    r_length_ = other.r_length_;
    timestamp_ = other.timestamp_;
    invalid_ = other.invalid_;
    earth_rotated_angle_ = other.earth_rotated_angle_;

    return *this;
}

double RadarStation::DistanceTo(GeoPoint &other)
{
    return GeoPoint::DistanceTo(other);
}

RadarStation::RadarStation(double latitude, double longitude, double altitude,
                           double view_angle) : GeoPoint(latitude, longitude,
                                                         altitude)
{

}

double RadarStation::AngleTo(GeoPoint &other)
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

    return std::asin(std::abs(numenator) / denominator);
}
