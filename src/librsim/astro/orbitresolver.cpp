//
// Created by melges on 12.06.16.
//

#include "orbitresolver.h"
#include "radarstation.h"

#include <gsl/gsl_multifit_nlin.h>
#include <librsim/astro/orbitcalculator.h>

Orbit orbitresolver::RestoreOrbitConsideringFlux(
    std::list<SightObject> objects_observations, RadarStation observer)
{
    size_t parameter_number = 8;
    double init_parameters[parameter_number] = {

    };

    const gsl_multifit_fdfsolver_type *solver_type =
        gsl_multifit_fdfsolver_lmder;
    gsl_multifit_fdfsolver *solver = gsl_multifit_fdfsolver_alloc(
        solver_type, objects_observations.size(), parameter_number);


}