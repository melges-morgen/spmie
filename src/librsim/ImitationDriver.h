//
// Created by melge on 01.07.2015.
//

#ifndef SPMIE_IMMITATIONDRIVER_H
#define SPMIE_IMMITATIONDRIVER_H


#include <bits/stl_map.h>
#include "astro/radarstation.h"
#include "astro/orbitcalculator.h"

class ImitationDriver
{

  private:
    std::map<RadarStation> radars_maps_;
    std::map<Orbit> sattelites_map_;
};


#endif //SPMIE_IMMITATIONDRIVER_H
