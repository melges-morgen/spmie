#include "nequick.h"

double nequick::NeQuick(double &height, double &latitude, double &longitude, 
    double &flux, int &month, double &time_of_day) 
{
  return nequick_(&height, &latitude, &longitude, &month, &flux, &time_of_day);
}

double nequick::ElectronContent(
    double &start_latitude, double &start_longitude, double &start_height, 
    double &end_latitude, double &end_longitude, double &end_height,
    double &flux, int &year, int &month, double &time_of_day) 
{
  return nequickintegral_(&start_latitude, &start_longitude, &start_height,
      &end_latitude, &end_longitude, &end_height, 
      &year, &month, &time_of_day, &flux);
}

