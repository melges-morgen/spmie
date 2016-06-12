//
// Created by melges on 12.06.16.
//

#ifndef SPMIE_ORBITRESOLVER_H
#define SPMIE_ORBITRESOLVER_H

#include <list>

#include "orbitcalculator.h"
#include "SightObject.h"
#include "radarstation.h"

namespace orbitresolver
{
  typedef struct {
      std::list *objects_observations;
      RadarStation *observer;
  } restore_function_parameters;

  Orbit RestoreOrbitConsideringFlux(
      std::list<SightObject> objects_observations, RadarStation observer);
};


#endif //SPMIE_ORBITRESOLVER_H
