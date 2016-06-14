//
// Created by melges on 12.06.16.
//

#ifndef SPMIE_ORBITRESOLVER_H
#define SPMIE_ORBITRESOLVER_H

#include <list>
#include <gsl/gsl_vector.h>

#include "orbitcalculator.h"
#include "SightObject.h"
#include "radarstation.h"

namespace orbitresolver
{
    typedef struct
    {
        std::list<SightObject> *objects_observations;
        RadarStation *observer;
        Orbit *real_orbit;
    } restore_function_parameters;

    Orbit RestoreOrbitConsideringFlux(Orbit real_orbit,
                                      std::list<SightObject> objects_observations,
                                      RadarStation observer);

    Orbit RestoreOrbitNoFlux(Orbit real_orbit,
                             std::list<SightObject> objects_observations,
                             RadarStation observer);

    int CalculationFunctionWithFlux(const gsl_vector * x, void *data,
                                    gsl_vector * f);

    int CalculationFunctionNoFlux(const gsl_vector * x, void *data,
                                    gsl_vector * f);
};


#endif //SPMIE_ORBITRESOLVER_H
