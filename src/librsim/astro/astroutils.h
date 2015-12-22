#ifndef ASTROUTILS_H_
#define ASTROUTILS_H_

#include <ctime>
#include <cmath>
#include <chrono>
#include <random>

#ifndef M_PI
#define M_PI 3.1415
#endif

namespace astroutils
{
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
   * Random generator based on time seed
   */
  static std::default_random_engine generator(
      (unsigned long) std::chrono::system_clock::now()
          .time_since_epoch()
          .count()
  );

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
      return lrint(days * kSecondsInDay) + kAstroEpochStart;
  }

  inline time_t SecondsSinceMidnight(time_t timestamp)
  {
      return timestamp % kSecondsInDay;
  }

  inline double DegToRad(double degrees)
  {
      return degrees * M_PI / 180;
  }

  inline double RadToDeg(double radians)
  {
      return radians * 180 / M_PI;
  }

  double RandomizeValue(double expected, double sigma);
}

#endif // ASTROUTILS_H_
