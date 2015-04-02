#include <vector>

class OrbitPoint {

  private:
    double x_coord;
    double y_coord;
    double z_coord;

    double zenit;
    double azimuth;
    double high;

    long timestamp;
};



class Orbit
{
  public:
    Orbit();

    Orbit(int satellite_number, double epoch, double drag_coefficent,
        double inclination_angle, double ascending_node, double excentrisitet, 
        double pericenter_argument, double mean_anomaly, double mean_motion);

    OrbitPoint GetTrajectoryPoint(double second_since_epoch);

    std::vector<OrbitPoint> GetTrajectoryPoints(double start_time, 
        double end_time, double time_step);


};
