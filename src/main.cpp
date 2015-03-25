#include <stdlib.h>
#include <getopt.h>

#include <iostream>




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
  
  std::cout << nequickintegral_(&alat1, &along1, &h1,
                                &alat2, &along2, &h2,
                                &y, &month, &UT, &flux) 
            << std::endl;

  return 0;
}
