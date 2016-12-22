//
// Created by melge on 01.07.2015.
//

#ifndef SPMIE_IMMITATIONDRIVER_H
#define SPMIE_IMMITATIONDRIVER_H


#include <map>
#include <list>

#include "astro/radarstation.h"
#include "astro/orbitcalculator.h"
#include "librsim/astro/SightReport.h"

class ImitationDriver
{
  public:
    std::map<std::string, std::vector<SightReport>>
        RunImitation(
                time_t from_time, time_t to_time, time_t step = 15);

    inline void addRadarStation(RadarStation &radar_station)
    {
        radars_maps_[radar_station.GetName()] = radar_station;
    }
    
    inline void addSatellite(Orbit &satellite_orbit)
    {
        satellites_map_[satellite_orbit.GetSatelliteNumber()] = satellite_orbit;
    }
    
  private:
    std::map<std::string, RadarStation> radars_maps_;
    std::map<int, Orbit> satellites_map_;

    std::list<OrbitPoint> GetSatellitesPositions(time_t target_time);

    std::map<std::string, SightReport> GetAllRadarSightReports(
            std::list<OrbitPoint> &&satellites_positions,
            time_t observation_time);
};


#endif //SPMIE_IMMITATIONDRIVER_H