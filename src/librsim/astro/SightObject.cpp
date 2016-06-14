//
// Created by melges on 21.12.15.
//

#include "SightObject.h"

SightObject::SightObject()
        : object_id_(-1),
          observation_time_(-1),
          distance_to_(-1),
          zenith_angle_(-1),
          azimuth_angle_(-1)
{

}

SightObject::SightObject(int object_id, time_t observation_time,
                         double distance_to, double zenith_ange,
                         double azimuth_angle)
    : object_id_(object_id),
      observation_time_(observation_time),
      distance_to_(distance_to),
      zenith_angle_(zenith_ange),
      azimuth_angle_(azimuth_angle)
{
}

SightObject::SightObject(RadarStation &radar_station,
                         OrbitPoint &observe_point, time_t observation_time)
        : object_id_(0), // TODO: Add mechanism to ident differente objects
          observation_time_(observation_time),
          distance_to_(radar_station.ObservedDistanceTo(observe_point)),
          zenith_angle_(radar_station.ZenithAngleTo(observe_point)),
          azimuth_angle_(radar_station.AzimuthAngleTo(observe_point)),
          real_distance_to_(radar_station.DistanceTo(observe_point)),
          real_azimuth_angle_(radar_station.TrueAzimuthAngleTo(observe_point)),
          real_zenith_angle_(radar_station.TrueZenithAngleTo(observe_point))
{
}

bool SightObject::operator<(const SightObject &right)
{
    return this->object_id_ < right.object_id_;
}




