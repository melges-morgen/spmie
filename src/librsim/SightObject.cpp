//
// Created by melges on 21.12.15.
//

#include "SightObject.h"

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

bool SightObject::operator<(const SightObject &right)
{
    return this->object_id_ < right.object_id_;
}
