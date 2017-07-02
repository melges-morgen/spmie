#ifndef ASTROUTILS_H_
#define ASTROUTILS_H_

#include <ctime>
#include <cmath>

namespace astroutils {
  /**
   * Earth rotation speed in radians per second.
   */
  const double kEarthRotationSpeed = 7.2921158553E-5;

  /**
   * Earth radius in m
   */
  const double kEarthRadius = 6378.137 * 1000;


  /**
   * Shift in seconds between of astronomic epoch start and unix epoch start,
   * unix epoch start at 0 second.
   */
  const time_t kAstroEpochStart = -631152000;

  /**
   * Seconds in day
   */
  const int kSecondsInDay = 86164;

  /**
   * Convert unix time (seconds since unix epoch) 
   * to astronomic time (in days from 1 jan 1950).
   */
  inline double ConvertUnixTimeToAstro(time_t timestamp) 
  { 
    return (timestamp - kAstroEpochStart) / kSecondsInDay; 
  }

  inline time_t ConvertAstroTimeToUnix(double days)
  {
    return (time_t) lrint(days * kSecondsInDay) + kAstroEpochStart;
  }

  inline time_t SecondsSinceMidnight(time_t timestamp) 
  {
    return timestamp % kSecondsInDay;
  }

  inline double DegToRad(double degrees)
  {
    return degrees * M_PI / 180;
  }
}

#endif // ASTROUTILS_H_
