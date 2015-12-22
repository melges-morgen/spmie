//
// Created by melge on 01.07.2015.
//

#ifndef SPMIE_IMMITATIONDRIVER_H
#define SPMIE_IMMITATIONDRIVER_H


#include <map>
#include "astro/radarstation.h"
#include "astro/orbitcalculator.h"
#include "SightReport.h"

class ImitationDriver
{
  public:
    std::map<std::string, std::vector<SightReport>>
        RunImitation(time_t from, time_t to);

    inline void addRadarStation(std::string &name,
                                RadarStation &radar_station)
    {
        radars_maps_[name] = radar_station;
    }
    
    inline void addSattelite(std::string &name, 
                             Orbit &satellite_orbit)
    {
        satellites_map_[name] = satellite_orbit;
    }
    
  private:
    std::map<std::string, RadarStation> radars_maps_;
    std::map<std::string, Orbit> satellites_map_;
};


#endif //SPMIE_IMMITATIONDRIVER_H