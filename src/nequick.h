extern "C" 
{
  /**  
   * h:     height (km)
   * alat:  gg. latitude  (degrees N)
   * along: gg. longitude (degrees E)
   * mth:   month (1 .. 12)
   * flx:   10.7 cm solar radio flux (flux units)
   * UT:    Universal Time (hours)
   */

  double nequick_(double *, double *, double *, int *, 
      double *, double *);

  /**
   * alat1: latitude of start point (degrees N)
   * along1: longitude of start point (degrees E)
   * h1: height of start point (km)
   * alat2: latitude of end point (degrees N)
   * along2: longitude of end point (degrees E)
   * h2: height of end point (km)
   * y: year
   * m: month
   * UT: time of day (hour)
   * flux: solar activity 10.7 cm radio flux
   */
  double nequickintegral_(double *, double *, double*, 
      double *, double *, double *,
      int *, int *, double *, double *);
}

namespace nequick 
{
  double NeQuick(double &height, double &latitude, double &longitude,
      double &flux, int &month, double &time_of_day);

  double ElectronContent(
      double &start_latitude, double &start_longitude, double &start_height, 
      double &end_latitude, double &end_longitude, double &end_height,
      double &flux, int &year, int &month, double &time_of_day);
}


