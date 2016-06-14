//
// Created by melges on 21.12.15.
//

#ifndef SPMIE_SIGHTOBJECT_H
#define SPMIE_SIGHTOBJECT_H

#include <time.h>
#include "radarstation.h"

/**
 * Class for store latched information about observation of one object.
 *
 * @author Morgen Matvey <melges.morgen@gmail.com>
 */
class SightObject
{
  public:
    SightObject();

    SightObject(int object_id, time_t observation_time, double distance_to,
                double zenith_ange, double azimuth_angle);

    SightObject(RadarStation &radar_station,
                OrbitPoint &observe_point, time_t observation_time);

    bool operator<(const SightObject &right);

    inline int GetObjectId()
    {
        return object_id_;
    }

    /**
     * Returns time when this observation was made.
     */
    inline time_t GetObservationTime() const
    {
        return observation_time_;
    }

    /**
     * Returns observed distance to the object.
     */
    inline double GetDistanceTo() const
    {
        return distance_to_;
    }

    /**
     * Returns zenith angle of the observations direction.
     */
    inline double GetZenitAngle() const
    {
        return zenith_angle_;
    }

    /**
     * Returns azimuth angle of the observations direction.
     */
    inline double GetAzimuthAngle() const
    {
        return azimuth_angle_;
    }

    inline double DistanceDelta()
    {
        return real_distance_to_ - distance_to_;
    }

    inline double ZenithDelta()
    {
        return real_zenith_angle_ - zenith_angle_;
    }

    inline double AzimuthDelta()
    {
        return real_azimuth_angle_ - azimuth_angle_;
    }

    inline bool IsValidSightObject()
    {
        return distance_to_ != -1;
    }

  private:
    /*
     * Object id
     */
    int object_id_;

    /*
     * Time of observation
     */
    time_t observation_time_;

    /*
     * Observed distance to the object
     */
    double distance_to_;

    /*
     * Zenith angle to the observed object
     */
    double zenith_angle_;

    /*
     * Azimuth angle to the observed object
     */
    double azimuth_angle_;

    /*
     * Observed distance to the object
     */
    double real_distance_to_;

    /*
     * Zenith angle to the observed object
     */
    double real_zenith_angle_;

    /*
     * Azimuth angle to the observed object
     */
    double real_azimuth_angle_;
};


#endif //SPMIE_SIGHTOBJECT_H
