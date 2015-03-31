#include <stdlib.h>
#include <getopt.h>
#include <math.h>

#include <iostream>

#include "sgp_sdp_model/sgp4unit.h"


static const char *opt_string = "h";

static const struct option long_opts[] = 
{
  {"start-latitude", required_argument, NULL, 1},
  {"start-longitude", required_argument, NULL, 2},
  {"end-latitude", required_argument, NULL, 3},
  {"end-longitude", required_argument, NULL, 4}
};

int main(int argc, char **argv)
{
  double alat1 = 55.930825, along1 = 37.522267, h1 = 0.5, 
         alat2 = 55.926840, along2 = 37.530721, h2 = 300.0, UT = 16.5,
         flux = 100.0;
  int y = 2015, month = 3;

  double deg2rad = M_PI / 180, xpdotp = 1440.0 / M_PI * 2;

  int sputnik_number = 39765;
  double epoch = (2014 - 1950)*365 + 264.51, xbstar = 0.0, xecco = 0.018110,
         xargpo = 317.1560 * deg2rad, xinclo = 82.4519 * deg2rad, 
         xmo = 42.8029 * deg2rad,
         xno = 12.429 / xpdotp, xnodeo = 349.4209 * deg2rad;


  elsetrec orbit_param;
  sgp4init(wgs84, sputnik_number, epoch, xbstar, xecco, xargpo, 
      xinclo, xmo, xno, xnodeo,
      orbit_param);

  double r[3], v[3];

  
  for(int minute = 0; minute < 480; minute +=6) {
    sgp4(wgs84, orbit_param, minute, r, v);
    //std::cout << r[1] << " " << r[1] << " " << r[2] << " - "
    //  << v[0] << " " << v[1] << " " << v[2] << std::endl;
    std::cout << minute << " after epoch, " << asin(r[1]) * 180 / M_PI << ", "
      << asin(r[1] / sqrt((1 - pow(r[0], 2)))) * 180 / M_PI << std::endl;
  }
  
//  std::cout << nequickintegral_(&alat1, &along1, &h1,
//                                &alat2, &along2, &h2,
//                                &y, &month, &UT, &flux) 
//            << std::endl;

  return 0;
}
