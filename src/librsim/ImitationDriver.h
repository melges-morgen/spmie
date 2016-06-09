//
// Created by melge on 01.07.2015.
//

#ifndef SPMIE_IMMITATIONDRIVER_H
#define SPMIE_IMMITATIONDRIVER_H


#include <map>
#include <list>

#include "astro/radarstation.h"
#include "astro/orbitcalculator.h"
#include "SightReport.h"

class ImitationDriver
{
  public:
    std::map<std::string, std::vector<SightReport>>
        RunImitation(
                time_t from_time, time_t to_time, time_t step = 15);

    inline void addRadarStation(std::string &name,
                                RadarStation &radar_station)
    {
        radars_maps_[name] = radar_station;
    }
    
    inline void addSatellite(int satellite_number,
                             Orbit &satellite_orbit)
    {
        satellites_map_[satellite_number] = satellite_orbit;
    }
    
  private:
    std::map<std::string, RadarStation> radars_maps_;
    std::map<int, Orbit> satellites_map_;

    std::list<OrbitPoint> GetSatellitesPositions(time_t target_time);

    std::map<std::string, SightReport> GetSightReport(
            std::list<OrbitPoint> &&satellites_positions);
};


#endif //SPMIE_IMMITATIONDRIVER_H