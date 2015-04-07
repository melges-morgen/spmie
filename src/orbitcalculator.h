#ifndef ORBITCALCULATOR_H_
#define ORBITCALCULATOR_H_

#include <ctime>
#include <vector>

#include "sgp_sdp_model/sgp4unit.h"

class OrbitPoint {

  public:
    OrbitPoint();

    OrbitPoint(double x_coord, double y_coord, double z_coord, 
        time_t timetamp);

    OrbitPoint(const OrbitPoint &); 

    OrbitPoint & operator = (const OrbitPoint &);

    inline double GetX() { return x_coord_; }

    inline double GetY() { return y_coord_; }

    inline double GetZ() { return z_coord_; }
    
    inline double GetLatitude() { return latitude_; }

    inline double GetLongitude() { return longitude_; }

    inline double GetAltitude() { return altitude_; }

  private:
    /**
     * X coord in inertial cartesian coordinate system.
     */
    double x_coord_;
    
    /**
     * Y coord in inertial cartesian coordinate system.
     */
    double y_coord_;

    /**
     * Z coord in inertial cartesian coordinate system.
     */
    double z_coord_;

    /**
     * Zenit angle (tetta), but from the ordinate (Y Axis)
     */
    double zenit_;

    /**
     * Azimuth angle (phi), from the abscissas (X Axis)
     */
    double azimuth_;

    /**
     * Length of radius vector.
     */
    double r_length_;

    /**
     * Latitude in degrees
     */
    double latitude_;

    /**
     * Longitude in degrees
     */
    double longitude_;

    /**
     * High under sea level.
     */
    double altitude_;

    /**
     * Time (in unix time format) when sattelite goes the orbit point.
     */
    time_t timestamp_;

    bool invalid_ = true;

    double earth_rotated_angle_;
};



class Orbit
{
  public: 
    Orbit();

    /**
     * Construct object represented sattelite orbit. 
     * All values in radians.
     * @param sattelite_number sattelite number in NORAD database.
     * @param epoch epoch time in seconds from unix epoch (epoch in timestamp)
     * @param dgar_coefficent drag coefficent kg/m^2
     * @param inclination_angle inclination angle to the Earth equator.
     * @param ascending_node right ascension of ascending node.
     * @param excentrisitet orbits excentrisitet.
     * @param pericenter_argument argument of pericenter.
     * @param mean_anomaly mean anomaly.
     * @param mean_motion mean motion rad/s
     */
    Orbit(int satellite_number, time_t epoch, double drag_coefficent,
        double inclination_angle, double ascending_node, double excentrisitet, 
        double pericenter_argument, double mean_anomaly, double mean_motion);

    OrbitPoint GetTrajectoryPoint(time_t second_since_epoch);

    std::vector<OrbitPoint> GetTrajectoryPoints(time_t start_time, 
        time_t end_time, time_t time_step);

    

  private:
    elsetrec orbit_param_;

    int last_error_;

    /**
     * Epoch time in unix timestamp format
     */
    time_t epoch_time_;
};

#endif // ORBITCALCULATOR_H_
