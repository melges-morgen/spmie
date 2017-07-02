//
// Created by melge on 01.07.2015.
//

#ifndef SPMIE_TLEREADER_H
#define SPMIE_TLEREADER_H


#include <map>
#include "radarstation.h"
#include "orbitcalculator.h"

class TLEFormatException : std::exception
{
  public:
    TLEFormatException(std::string wrong_string);

    virtual const char *what() const throw();

  private:
    std::string wrong_tle_string_;
    std::string error_message;
};

class TLEReader
{
  public:
    static std::map<int, Orbit> ReadSatellitesFromFile(
            std::string filename);

  protected:
    static Orbit ParseTLEString(
            const std::string &tle_string_header,
            const std::string &tle_string_first,
            const std::string &tle_string_second);
};


#endif //SPMIE_TLEREADER_H
