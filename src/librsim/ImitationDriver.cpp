//
// Created by melge on 01.07.2015.
//

#include <iostream>
#include <iomanip>
#include "ImitationDriver.h"

std::map<std::string, std::vector<SightReport>>
ImitationDriver::RunImitation(
        time_t from, time_t to, time_t step)
{
    std::map<std::string, std::vector<SightReport>> imitation_result;

    for(time_t current_time = from; current_time <= to; current_time += step)
    {
        std::map<std::string, SightReport> report =
                GetAllRadarSightReports(
                        GetSatellitesPositions(current_time), current_time
                );
        for(auto it = report.begin(); it != report.end(); ++it)
        {
            imitation_result[it->first].push_back(it->second);
        }
    }

    return imitation_result;
}

std::list<OrbitPoint>
ImitationDriver::GetSatellitesPositions(time_t target_time)
{
    std::list<OrbitPoint> satellites_positions;
    for(auto it = satellites_map_.begin();
        it != satellites_map_.end(); ++it)
    {
        auto point = it->second.GetTrajectoryPoint(target_time);
        satellites_positions.push_front(point);
    }

    return satellites_positions;
}

std::map<std::string, SightReport> ImitationDriver::GetAllRadarSightReports(
        std::list<OrbitPoint> &&satellites_positions, time_t observation_time)
{
    std::map<std::string, SightReport> all_radar_report;
    for(auto radar_iterator =
            radars_maps_.begin();
        radar_iterator != radars_maps_.end(); ++radar_iterator)
    {
        RadarStation radar = radar_iterator->second;
        SightReport sight_report(observation_time, radar.GetName());

        int id = 0;
        for(auto point_iterator = satellites_positions.begin();
            point_iterator != satellites_positions.end();
            ++point_iterator, ++id)
        {
            if(observation_time == 1465440129)
                printf("Bad time");
            double distance = radar.ObservedDistanceTo(*point_iterator);
            double azimuth = radar.AzimuthAngleTo(*point_iterator);
            double zenith = radar.ZenithAngleTo(*point_iterator);

            if(distance != -1)
            {
                SightObject observed_object(id, observation_time,
                                            distance, zenith, azimuth);
                sight_report.AddObservedObject(observed_object);
            }
        }

        all_radar_report[radar.GetName()] = sight_report;
    }

    return all_radar_report;
}




