//
// Created by Морген Матвей on 12.06.15.
//

#ifndef SPMIE_RADARSTATION_H
#define SPMIE_RADARSTATION_H

#include <cmath>

#include "coordpoints.h"
#include "astroutils.h"

class RadarStation : public GeoPoint
{
  public:
    RadarStation();

    RadarStation(std::string radar_name, double latitude, double longitude,
                 double altitude, double view_bisector_zenith,
                 double zenith_angle, double view_bisector_azimuth,
                 double azimuth_angle, double frequency, double local_flux);


    inline const std::string &GetName() const
    {
        return name_;
    }

    inline const double &GetWorkFrequency()
    {
        return frequency_;
    }

    inline const double &GetLocalFlux() const
    {
        return local_flux_;
    }

    inline void SetLocalFlux(double local_flux_)
    {
        RadarStation::local_flux_ = local_flux_;
    }

    bool IsInSigh(GeoPoint &point);

    double ObservedDistanceTo(OrbitPoint &distance_point, double sigma = 0);

    double ZenithAngleTo(GeoPoint &other, double sigma = 0);

    double AzimuthAngleTo(GeoPoint &other, double sigma = 0);

  private:
    std::string name_;

    double view_bisector_zenith_;
    double zenith_angle_;

    double view_bisector_azimuth_;
    double azimuth_angle_;

    double frequency_;
    double local_flux_;
};

#endif //SPMIE_RADARSTATION_H
