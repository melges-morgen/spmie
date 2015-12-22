#include "coordpoints.h"

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
    double coord_sqr = pow(GetX() - other.GetX(), 2) +
                       pow(GetY() - other.GetY(), 2) +
                       pow(GetZ() - other.GetZ(), 2);

    return sqrt(coord_sqr);
}

double GeoPoint::GetX()
{
    double r = altitude_ + astroutils::kEarthRadius;
    double phi = astroutils::DegToRad(latitude_);
    double lambda = astroutils::DegToRad(longitude_);

    return r * cos(phi) * cos(lambda);
}

double GeoPoint::GetY()
{
    double r = altitude_ + astroutils::kEarthRadius;
    double phi = astroutils::DegToRad(latitude_);
    double lambda = astroutils::DegToRad(longitude_);

    return r * cos(phi) * sin(lambda);
}

double GeoPoint::GetZ()
{
    double r = altitude_ + astroutils::kEarthRadius;
    double phi = astroutils::DegToRad(latitude_);

    return r * sin(phi);
}

OrbitPoint::OrbitPoint()
{

}


OrbitPoint::OrbitPoint(double x_coord, double y_coord, double z_coord,
                       time_t timestamp)
        : x_inertial_coord_(x_coord),
          y_inertial_coord_(y_coord),
          z_inertial_coord_(z_coord),
          timestamp_(timestamp),
          invalid_(false)
{
    double r_v = sqrt(pow(x_inertial_coord_, 2) + pow(y_inertial_coord_, 2) + pow(
            z_inertial_coord_, 2));
    earth_rotated_angle_ =
            astroutils::kEarthRotationSpeed *
            astroutils::SecondsSinceMidnight(timestamp_);

    zenith_ = asin(z_inertial_coord_ / r_v);
    azimuth_ = remainder(atan(y_inertial_coord_ / x_inertial_coord_), 2 * M_PI);
    r_length_ = r_v * 1000;

    latitude_ = zenith_ * 180 / M_PI;
    longitude_ = (azimuth_ - earth_rotated_angle_) * 180 / M_PI;
    altitude_ = r_length_ - astroutils::kEarthRadius;

    if (x_inertial_coord_ < 0)
        longitude_ += 180;
    if (longitude_ > 180)
        longitude_ -= 360;
    if (longitude_ < -180)
        longitude_ += 360;
}

OrbitPoint::OrbitPoint(const OrbitPoint &other)
        : x_inertial_coord_(other.x_inertial_coord_),
          y_inertial_coord_(other.y_inertial_coord_),
          z_inertial_coord_(other.z_inertial_coord_),
          zenith_(other.zenith_),
          azimuth_(other.azimuth_),
          r_length_(other.r_length_),
          timestamp_(other.timestamp_),
          invalid_(other.invalid_),
          earth_rotated_angle_(other.earth_rotated_angle_) { }

OrbitPoint &OrbitPoint::operator=(const OrbitPoint &other)
{
    x_inertial_coord_ = other.x_inertial_coord_;
    y_inertial_coord_ = other.y_inertial_coord_;
    z_inertial_coord_ = other.z_inertial_coord_;
    zenith_ = other.zenith_;
    azimuth_ = other.azimuth_;
    r_length_ = other.r_length_;
    timestamp_ = other.timestamp_;
    invalid_ = other.invalid_;
    earth_rotated_angle_ = other.earth_rotated_angle_;

    return *this;
}
