//
// Created by melge on 01.07.2015.
//

#include <iostream>
#include <iomanip>
#include<gsl/gsl_sf_bessel.h>
#include "ImitationDriver.h"

std::map<std::string, std::vector<SightReport>>
ImitationDriver::RunImitation(
        time_t from, time_t to, time_t step)
{
    std::map<std::string, std::vector<SightReport>> imitation_result;

    for(time_t current_time = from; current_time <= to; current_time += step)
    {
        std::map<std::string, SightReport> report =
                GetSightReport(GetSatellitesPositions(current_time));
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
        satellites_positions.push_back(
                it->second.GetTrajectoryPoint(target_time)
        );
    }

    return satellites_positions;
}

std::map<std::string, SightReport> ImitationDriver::GetSightReport(
        std::list<OrbitPoint> &&satellites_positions)
{
    std::map<std::string, SightReport> all_radar_report;
    for(auto radar_iterator =
            radars_maps_.begin();
        radar_iterator != radars_maps_.end(); ++radar_iterator)
    {
        RadarStation radar = radar_iterator->second;
        SightReport sight_report(0, radar.GetName());

        int id = 0;
        for(auto point_iterator = satellites_positions.begin();
            point_iterator != satellites_positions.end();
            ++point_iterator, ++id)
        {
            double distance = radar.ObservedDistanceTo(*point_iterator);
            double azimuth = radar.AzimuthAngleTo(*point_iterator);
            double zenith = radar.ZenithAngleTo(*point_iterator);

            SightObject observed_object(id, point_iterator->GetTime(),
                                        distance, zenith, azimuth);
            sight_report.AddObservedObject(observed_object);
        }

        all_radar_report[radar.GetName()] = sight_report;
    }

    return all_radar_report;
}




