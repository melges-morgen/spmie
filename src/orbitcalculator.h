#ifndef ORBITCALCULATOR_H_
#define ORBITCALCULATOR_H_

#include <vector>
#include <ctime>

#include "coordpoints.h"

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
