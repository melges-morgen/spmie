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

bool SightObject::operator<(const SightObject &right)
{
    return this->object_id_ < right.object_id_;
}


