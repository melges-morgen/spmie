#ifndef ORBITCALCULATOR_H_
#define ORBITCALCULATOR_H_

#include <vector>
#include <ctime>
#include <string>

#include "coordpoints.h"
#include "sgp_sdp_model/sgp4unit.h"

/**
 * Class represents satellite and his orbit parameters.
 */
class Orbit
{
  public: 
    Orbit();

    Orbit(elsetrec orbit_param, elsetrec raw_orbit_param,
              std::string satellite_name);

    /**
     * Construct object representing satellite orbit.
     * All values in radians.
     *
     * @param satellite_number satellite number in NORAD database.
     * @param epoch epoch time in seconds from unix epoch (epoch in timestamp)
     * @param drag_coefficient drag coefficient kg/m^2
     * @param inclination_angle inclination angle to the Earth equator in deg.
     * @param ascending_node right ascension of ascending node.
     * @param eccentricity orbits eccentricity.
     * @param apsis_argument argument of apsis in deg.
     * @param mean_anomaly mean anomaly in deg.
     * @param mean_motion mean motion rad/s
     */
    Orbit(int satellite_number, Orbit &other_orbit, double drag_coefficient,
          double inclination_angle, double ascending_node,
          double eccentricity,
          double apsis_argument, double mean_anomaly,
          double mean_motion,
          std::string satellite_name = "Not provided");

    /**
     * Function returns number of satellite
     */
    inline int GetSatelliteNumber() { return (int) orbit_param_.satnum; }

    const std::string &GetSatelliteName() const
    {
        return satellite_name_;
    }

    OrbitPoint GetTrajectoryPoint(time_t target_time);

    std::vector<OrbitPoint> GetTrajectoryPoints(time_t start_time,
        time_t end_time, time_t time_step);

    int GetLastError_() const
    {
        return last_error_;
    }

    time_t GetEpochTime() const
    {
        return epoch_time_;
    }

    const std::string &GetSatelliteName_() const
    {
        return satellite_name_;
    }

    double GetDragCoefficient() const
    {
        return drag_coefficient_;
    }

    double GetInclinationAngle() const
    {
        return inclination_angle_;
    }

    double GetAscendingNode() const
    {
        return ascending_node_;
    }

    double GetEccentricity() const
    {
        return eccentricity_;
    }

    double GetApsisArgument() const
    {
        return apsis_argument_;
    }

    double GetMeanAnomaly() const
    {
        return mean_anomaly_;
    }

    double GetMeanMotion() const
    {
        return mean_motion_;
    }

    inline double GetOriginalEpochValue()
    {
        return original_epoch_value_;
    }

    inline elsetrec GetOrbitParam()
    {
        return orbit_param_;
    }

    inline void SetEpoch(time_t epoch)
    {
        epoch_time_ = epoch;
    }

    void SetOriginalEpochValue(double original_epoch_value);

  private:
    elsetrec orbit_param_;
    elsetrec raw_orbit_param_;

    int last_error_;

    /**
     * Epoch time in unix timestamp format
     */
    time_t epoch_time_;

    std::string satellite_name_;

    double drag_coefficient_;
    double inclination_angle_;
    double ascending_node_;
    double eccentricity_;
    double apsis_argument_;
    double mean_anomaly_;
    double mean_motion_;
    double original_epoch_value_;
};

#endif // ORBITCALCULATOR_H_
