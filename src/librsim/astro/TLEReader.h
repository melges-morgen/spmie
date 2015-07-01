//
// Created by melge on 01.07.2015.
//

#ifndef SPMIE_TLEREADER_H
#define SPMIE_TLEREADER_H


#include <map>
#include "radarstation.h"

class TLEReader
{
  public:
    std::map<std::string, RadarStation> ReadRadarsFromFile(std::string file);
};


#endif //SPMIE_TLEREADER_H
