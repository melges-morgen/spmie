#include "orbitcalculator.h"

#include "astroutils.h"

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

OrbitPoint::OrbitPoint(const OrbitPoint &other) 
  : x_coord_(other.x_coord_),
    y_coord_(other.y_coord_),
    z_coord_(other.z_coord_),
    zenit_(other.zenit_),
    azimuth_(other.azimuth_),
    r_length_(other.r_length_),
    timestamp_(other.timestamp_),
    invalid_(other.invalid_),
    earth_rotated_angle_(other.earth_rotated_angle_) {}

OrbitPoint &OrbitPoint::operator=(const OrbitPoint &other)
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

Orbit::Orbit() 
{
  
}

Orbit::Orbit(int satellite_number, time_t epoch, double drag_coefficent, 
    double inclination_angle, double ascending_node, double excentrisitet,
    double pericenter_argument, double mean_anomaly, double mean_motion)
  : epoch_time_(epoch)
{
  // Calculate number of days since astronomic time epoch (1 jan 1950)
  double epoch_days = 
    (epoch - astroutils::kAstroEpochStart) / astroutils::kSecondsInDay;
  last_error_ = sgp4init(wgs84, satellite_number, epoch_days, drag_coefficent,
      excentrisitet, pericenter_argument, inclination_angle, mean_anomaly, 
      mean_motion, ascending_node, orbit_param_);
}

OrbitPoint Orbit::GetTrajectoryPoint(time_t second_since_epoch)
{
  double minute_since_epoch = second_since_epoch / 60;
  double r[3], v[3];
  last_error_ = sgp4(wgs84, orbit_param_, minute_since_epoch, r, v);
  if(last_error_ != 0)
    return OrbitPoint();

  return OrbitPoint(r[0], r[1], r[2], epoch_time_ + second_since_epoch);
}

std::vector<OrbitPoint> Orbit::GetTrajectoryPoints(time_t start_time, 
        time_t end_time, time_t time_step)
{
  int points_number = 
    (unsigned int) (end_time - start_time) / time_step;
  std::vector<OrbitPoint> result_vector(points_number);

  for(int i = 0; i < points_number; ++i)
    result_vector[i] = GetTrajectoryPoint(start_time + i*time_step);
 
  return result_vector;
}

