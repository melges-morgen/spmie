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
    RadarStation(double latitude, double longitude, double altitude,
                 double view_bisector_zenith, double zenith_angle,
                 double view_bisector_azimuth, double azimuth_angle,
                 double frequency);

    inline double GetWorkFrequency() {
        return frequency_;
    }

    bool IsInSigh(GeoPoint &point);

    double ObservedDistanceTo(OrbitPoint &distance_point);

    double ZenithAngleTo(GeoPoint &other);

    double AzimuthAngleTo(GeoPoint &other);

  private:
    double view_bisector_zenith_;
    double zenith_angle_;

    double view_bisector_azimuth_;
    double azimuth_angle_;

    double frequency_;
};

#endif //SPMIE_RADARSTATION_H
