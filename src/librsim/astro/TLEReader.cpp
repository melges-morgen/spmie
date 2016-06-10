//
// Created by melge on 01.07.2015.
//

#include <fstream>
#include <iostream>

#include <assert.h>
#include <string.h>

#include "TLEReader.h"

std::map<int, Orbit> TLEReader::ReadSatellitesFromFile(
        std::string filename) throw(TLEFormatException)
{
    std::map<int, Orbit> produced_map;
    std::ifstream input_stream(filename);
    if(input_stream.fail())
        throw std::runtime_error(strerror(errno));

    std::string satellite_header;
    while (std::getline(input_stream, satellite_header))
    {
        if (satellite_header.empty())
            continue;

        std::string first_tle_line;
        if (satellite_header.length() > 24) // This is not header
        {
            first_tle_line = satellite_header; // Possibly it is first data line
            satellite_header = "No name";
        }
        else if (!std::getline(input_stream, first_tle_line))
            throw TLEFormatException("Unexpected end of file");

        std::string second_tle_line;
        if (!std::getline(input_stream, second_tle_line))
            throw TLEFormatException("Unexpected end of file");

        // Check that both lines are starts from correct symbol
        if (first_tle_line[0] != '1')
            throw TLEFormatException(first_tle_line);
        if (second_tle_line[0] != '2')
            throw TLEFormatException(second_tle_line);

        Orbit parsed_satellite = ParseTLEString(
                satellite_header, first_tle_line, second_tle_line
        );
        produced_map[parsed_satellite.GetSatelliteNumber()] = parsed_satellite;
    }

    input_stream.close();

    return produced_map;
}

Orbit TLEReader::ParseTLEString(const std::string &tle_string_header,
                                const std::string &tle_string_first,
                                const std::string &tle_string_second)
{
    assert(tle_string_first[0] == '1');
    assert(tle_string_second[0] == '2');

    int satellite_number = atoi(tle_string_first.substr(2, 5).c_str());
    int epoch_year = atoi(tle_string_first.substr(18, 2).c_str()) + 2000;
    double epoch_day = atof(tle_string_first.substr(20, 12).c_str());
    double drag_coefficient = 0;//atof(tle_string_first.substr(53, 8).c_str());

    double inclination_angle = astroutils::DegToRad(
            atof(tle_string_second.substr(8, 8).c_str())
    );
    double ascending_node = astroutils::DegToRad(
            atof(tle_string_second.substr(17, 8).c_str())
    );
    double eccentricity = atof(tle_string_second.substr(26, 7).c_str())
                          / 10000000;
    double apsis_argument = atof(tle_string_second.substr(34, 8).c_str());
    double mean_anomaly = astroutils::DegToRad(
            atof(tle_string_second.substr(43, 8).c_str())
    );
    double mean_motion = atof(tle_string_second.substr(52, 11).c_str())
                         / (1440.0 / M_PI * 2);

    return Orbit(satellite_number,
                 astroutils::ConvertAstroTimeToUnix(
                         (epoch_year - 1950) * 365 + epoch_day),
                 drag_coefficient, inclination_angle, ascending_node,
                 eccentricity, apsis_argument, mean_anomaly, mean_motion,
                 tle_string_header

    );
}

TLEFormatException::TLEFormatException(std::string wrong_string)
{
    wrong_tle_string_ = wrong_string;
    error_message = "Error in TLE file: " + wrong_tle_string_;
}

const char *TLEFormatException::what() const throw()
{
    return error_message.c_str();
}
