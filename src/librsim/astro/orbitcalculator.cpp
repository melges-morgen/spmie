#include "orbitcalculator.h"

#include "astroutils.h"

Orbit::Orbit()
{

}

Orbit::Orbit(
        int satellite_number, time_t epoch, double drag_coefficient,
        double inclination_angle, double ascending_node,
        double eccentricity,
        double apsis_argument, double mean_anomaly,
        double mean_motion,
        std::string satellite_name
) : epoch_time_(epoch),
    satellite_name_(satellite_name),
    drag_coefficient_(drag_coefficient),
    inclination_angle_(inclination_angle),
    ascending_node_(ascending_node),
    eccentricity_(eccentricity),
    apsis_argument_(apsis_argument),
    mean_anomaly_(mean_anomaly),
    mean_motion_(mean_motion)
{
    // Calculate number of days since astronomic time epoch (1 jan 1950)
    double epoch_days =
            (epoch - astroutils::kAstroEpochStart) / astroutils::kSecondsInDay;
    last_error_ = sgp4init(wgs84, satellite_number, epoch_days,
                           drag_coefficient,
                           eccentricity, apsis_argument,
                           inclination_angle, mean_anomaly,
                           mean_motion, ascending_node, orbit_param_);
}

OrbitPoint Orbit::GetTrajectoryPoint(time_t target_time)
{
    double minute_since_epoch = (target_time - epoch_time_) / 60;
    double r[3], v[3];
    last_error_ = sgp4(wgs84, orbit_param_, minute_since_epoch, r, v);
    if (last_error_ != 0)
        return OrbitPoint();

    return OrbitPoint(r[0], r[1], r[2], target_time);
}

std::vector<OrbitPoint> Orbit::GetTrajectoryPoints(
        time_t start_time,
        time_t end_time,
        time_t time_step
)
{
    unsigned long points_number =
            (unsigned long) ((end_time - start_time) / time_step);
    std::vector<OrbitPoint> result_vector(points_number);

    for (int i = 0; i < points_number; ++i)
        result_vector[i] = GetTrajectoryPoint(start_time + i * time_step);

    return result_vector;
}

