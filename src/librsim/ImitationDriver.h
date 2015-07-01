//
// Created by melge on 01.07.2015.
//

#ifndef SPMIE_IMMITATIONDRIVER_H
#define SPMIE_IMMITATIONDRIVER_H


#include <map>
#include "astro/radarstation.h"
#include "astro/orbitcalculator.h"

class ImitationDriver
{
  public:
    void RunImitation();
  private:
    std::map<std::string, RadarStation> radars_maps_;
    std::map<std::string, Orbit> sattelites_map_;
};


#endif //SPMIE_IMMITATIONDRIVER_H