#ifndef ORBITCALCULATOR_H_
#define ORBITCALCULATOR_H_

#include <vector>
#include <ctime>

#include "coordpoints.h"
#include "sgp_sdp_model/sgp4unit.h"

/**
 * Class represents satellite and his orbit parameters.
 */
class Orbit
{
  public: 
    Orbit();

    /**
     * Function returns number of satellite
     */
    inline int GetSatelliteNumber() { return (int) orbit_param_.satnum; }

    /**
     * Construct object represented satellite orbit.
     * All values in radians.
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
    Orbit(int satellite_number, time_t epoch, double drag_coefficient,
        double inclination_angle, double ascending_node, double eccentricity,
        double apsis_argument, double mean_anomaly, double mean_motion);

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
