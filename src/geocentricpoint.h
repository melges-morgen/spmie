#ifndef GEOCENTRICPOINT_H_
#define GEOCENTRICPOINT_H_

#include <ctime>

#include "sgp_sdp_model/sgp4unit.h"

class GeocentricPoint {

  public:
    GeocentricPoint();

    GeocentricPoint(double x_coord, double y_coord, double z_coord, 
        time_t timetamp);

    GeocentricPoint(const GeocentricPoint &); 

    GeocentricPoint & operator = (const GeocentricPoint &);

    double DestanceTo(GeocentricPoint &other);

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

class 

#endif // GEOCENTRICPOINT_H_
