#include <ctime>
#include <vector>

class OrbitPoint {

  public:
    double GetLatitude();

    double GetLongitude();

    double GetAltitude();

    double GetX();

    double GetY();

    double GetZ();

  private:
    double x_coord;
    double y_coord;
    double z_coord;

    double zenit;
    double azimuth;
    double high;

    /**
     * Time (in unix time format) when sattelite goes the orbit point.
     */
    time_t timestamp;
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

    OrbitPoint GetTrajectoryPoint(double second_since_epoch);

    std::vector<OrbitPoint> GetTrajectoryPoints(double start_time, 
        double end_time, time_t time_step);


};
