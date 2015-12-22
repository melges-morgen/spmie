//
// Created by melges on 20.12.15.
//

#include "SightReport.h"

SightReport::SightReport(time_t observation_time, const std::string &radar_name,
                         const std::vector<SightObject> &observed_objects)
    : observation_time_(observation_time),
      radar_name_(radar_name),
      observed_objects_(observed_objects)
{
}

SightReport::SightReport(time_t observation_time,
                         const std::string &radar_name)
    : SightReport(observation_time, radar_name, std::vector<SightObject>())
{
}
