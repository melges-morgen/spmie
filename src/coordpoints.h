#ifndef COORDPOINTS_H_
#define COORDPOINTS_H_

#include <ctime>

#include "sgp_sdp_model/sgp4unit.h"

/**
 * Class for store coordinates of object or smth. placed on Earth and rotated
 * with, in standard geo coordinate system.
 */
class GeoPoint
{
  public:
    GeoPoint();

    GeoPoint(double latitude, double longitude, double altitude);

    virtual void SetLatitude(double latitude);

    virtual void SetLongitude(double longitude);

    virtual void SetAltitude(double altitude);

    virtual double DistanceTo(GeoPoint &other);

    inline double & GetLatitude() { return latitude_; }

    inline double & GetLongitude() { return longitude_; }

    inline double & GetAltitude() { return altitude_; }

  protected:
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

};

/**
 * Class for store coordinates of object or smth. placed on orbit near Earth.
 * Inertial cartesian coordinate system is using instead of geographical.
 * This class consider time when object was at specified point and convert
 * coordinates from inertial system to geographical. Latitude and longitude mean
 * coordinates of sub-satellite point on sea level at specified time and
 * altitude mean length of normal from sub-satellite point to object. Earth
 * rotation is considered for calculate latitude and longitude.
 */
class OrbitPoint : public GeoPoint
{
  public:
    OrbitPoint();

    OrbitPoint(double x_coord, double y_coord, double z_coord,
               time_t timestamp);

    OrbitPoint(const OrbitPoint &);

    OrbitPoint &operator=(const OrbitPoint &);

    inline double GetX() { return x_coord_; }

    inline double GetY() { return y_coord_; }

    inline double GetZ() { return z_coord_; }


  protected:
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
     * Zenith angle (tetta), but from the ordinate (Y Axis)
     */
    double zenith_;

    /**
     * Azimuth angle (phi), from the abscissas (X Axis)
     */
    double azimuth_;

    /**
     * Length of radius vector.
     */
    double r_length_;

    /**
     * Time (in unix time format) when sattelite goes the orbit point.
     */
    time_t timestamp_;

    bool invalid_ = true;


  private:
    double earth_rotated_angle_;
};

class RadarStation : public GeoPoint
{
  public:
    RadarStation(double latitude, double longitude, double altitude,
                 double view_angle);


    virtual double DistanceTo(GeoPoint &other) override;

    double AngleTo(GeoPoint &other);
};

#endif // GEOCENTRICPOINT_H_
