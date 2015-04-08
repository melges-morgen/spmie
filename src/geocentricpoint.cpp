#include "geocentricpoint.h"

#include "astroutils.h"

GeocentricPoint::GeocentricPoint()
{
  
}

GeocentricPoint::GeocentricPoint(double x_coord, double y_coord, double z_coord, 
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
    
  zenit_ = asin(z_coord_ / r_v);
  azimuth_ = remainder(atan(y_coord_ / x_coord_), 2 * M_PI);
  r_length_ = r_v * 1000;

  latitude_ = zenit_ * 180 / M_PI;
  longitude_ = (azimuth_ - earth_rotated_angle_) * 180 / M_PI;
  altitude_ = r_length_ - astroutils::kEarthRadius;

  if(x_coord_ < 0)
    longitude_ += 180;
  if(longitude_ > 180) 
    longitude_ -= 360;
  if(longitude_ < -180)
    longitude_ += 360;
}

GeocentricPoint::GeocentricPoint(const GeocentricPoint &other) 
  : x_coord_(other.x_coord_),
    y_coord_(other.y_coord_),
    z_coord_(other.z_coord_),
    zenit_(other.zenit_),
    azimuth_(other.azimuth_),
    r_length_(other.r_length_),
    timestamp_(other.timestamp_),
    invalid_(other.invalid_),
    earth_rotated_angle_(other.earth_rotated_angle_) {}

GeocentricPoint &GeocentricPoint::operator=(const GeocentricPoint &other)
{
  x_coord_ = other.x_coord_;
  y_coord_ = other.y_coord_;
  z_coord_ = other.z_coord_;
  zenit_ = other.zenit_;
  azimuth_ = other.azimuth_;
  r_length_ = other.r_length_;
  timestamp_ = other.timestamp_;
  invalid_ = other.invalid_;
  earth_rotated_angle_ = other.earth_rotated_angle_;

  return *this;
}

double GeocentricPoint::DestanceTo(GeocentricPoint &other)
{
  double coord_sqr = pow(x_coord_ - other.x_coord_, 2) + 
    pow(y_coord_ - other.y_coord_, 2) + pow(z_coord_ - other.z_coord_, 2);

  return sqrt(coord_sqr);
}

