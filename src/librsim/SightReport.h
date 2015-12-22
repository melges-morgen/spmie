//
// Created by melges on 20.12.15.
//

#ifndef SPMIE_SIGHTREPORT_H
#define SPMIE_SIGHTREPORT_H

#include <time.h>
#include <string>
#include <vector>
#include "SightObject.h"

/**
 * Class contains information about objects observed at a given time.
 *
 * @author Morgen Matvey <melges.morgen@gmail.com>
 */
class SightReport
{
  public:
    /**
     * Construct report for specified time and radar.
     */
    SightReport(time_t observation_time, const std::string &radar_name,
                const std::vector<SightObject> &observed_objects);

    /**
     * Construct empty report for specified time and radar.
     */
    SightReport(time_t observation_time_, const std::string &radar_name_);

    inline void AddObservedObject(SightObject &object)
    {
        observed_objects_.push_back(object);
    }

    inline time_t GetObservationTime() const
    {
        return observation_time_;
    }

    inline const std::string &GetRadarName() const
    {
        return radar_name_;
    }

    const std::vector<SightObject> &GetObservedObjects() const
    {
        return observed_objects_;
    }


  private:
    /*
     * Time of observations
     */
    time_t observation_time_;

    /*
     * String identification of radar which has produced this report
     */
    std::string radar_name_;

    /*
     * Array of all observed object at given time.
     */
    std::vector<SightObject> observed_objects_;
};


#endif //SPMIE_SIGHTREPORT_H
