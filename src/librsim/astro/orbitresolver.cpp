//
// Created by melges on 12.06.16.
//

#include "orbitresolver.h"
#include "radarstation.h"

#include <gsl/gsl_multifit_nlin.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_blas.h>

#include <librsim/astro/orbitcalculator.h>
#include <librsim/ImitationDriver.h>

int orbitresolver::CalculationFunctionWithFlux(const gsl_vector * x, void *data,
                                gsl_vector * f)
{
    restore_function_parameters *parameters =
            (restore_function_parameters *) data;
    const size_t result_size = parameters->objects_observations->size();
    time_t start_time =
            parameters->objects_observations->begin()->GetObservationTime();
    time_t end_time =
            parameters->objects_observations->back().GetObservationTime();
    time_t epoch =
            parameters->real_orbit->GetEpochTime();
    std::list<SightObject> *objects_observations =
            parameters->objects_observations;
    RadarStation *radar = parameters->observer;

    double drag_coefficient = gsl_vector_get(x, 0);
    double inclination_angle = gsl_vector_get(x, 1);
    double ascending_node = gsl_vector_get(x, 2);
    double eccentricity = gsl_vector_get(x, 3);
    double apsis_argument = gsl_vector_get(x, 4);
    double mean_anomaly = gsl_vector_get(x, 5);
    double mean_motion = gsl_vector_get(x, 6);

    Orbit orbit(0, *parameters->real_orbit,
                drag_coefficient, inclination_angle, ascending_node,
                eccentricity, apsis_argument, mean_anomaly, mean_motion);
    orbit.SetEpoch(parameters->real_orbit->GetEpochTime());

    radar->SetSigma(0);
//    radar->SetLocalFlux(gsl_vector_get(x, 7));
    radar->SetLocalFlux(parameters->observer->GetLocalFlux());

    ImitationDriver driver;
    driver.addRadarStation(*radar);
    driver.addSatellite(orbit);

    auto simulation_report = driver.RunImitation(start_time, end_time);
    std::list<SightObject> result_objects;
    for(auto it = simulation_report.begin(); it != simulation_report.end();
        ++it)
    {
        auto observation_report = it->second;
        for(auto observation_iterator = observation_report.begin();
            observation_iterator != observation_report.end();
            ++observation_iterator) {
            if(observation_iterator->GetObservedObjects().size() < 1)
                result_objects.push_back(SightObject(0, 0, 0, 0, 0));
            else
            {
                auto observe = *observation_iterator;
                result_objects.push_back(observe.GetObservedObjects()[0]);
            }
        }
    }

    size_t i = 0;
    for (auto it = result_objects.begin(),
                 it2 = objects_observations->begin();
         it != result_objects.end() && it2 != objects_observations->end()
         && i < result_size; ++it, ++it2, ++i)
    {
        double delta = pow(it->GetDistanceTo()
                       - it2->GetDistanceTo(), 2);
        //std::cout << "Delta : " << delta << std::endl;
        gsl_vector_set(f, i, delta);
    }

    return GSL_SUCCESS;
}

Orbit orbitresolver::RestoreOrbitConsideringFlux(
        Orbit real_orbit, std::list<SightObject> objects_observations,
        RadarStation observer)
{
    size_t parameters_number = 8;
    double init_parameters[parameters_number] = {
            real_orbit.GetDragCoefficient(),
            real_orbit.GetInclinationAngle(),
            real_orbit.GetAscendingNode(),
            real_orbit.GetEccentricity(),
            real_orbit.GetApsisArgument(),
            real_orbit.GetMeanAnomaly(),
            real_orbit.GetMeanMotion(),
            observer.GetLocalFlux()
    };

    restore_function_parameters internal_parameters = {
            &objects_observations,
            &observer,
            &real_orbit
    };

    const gsl_multifit_fdfsolver_type *solver_type =
            gsl_multifit_fdfsolver_lmder;
    gsl_multifit_fdfsolver *solver = gsl_multifit_fdfsolver_alloc(
            solver_type, objects_observations.size(), parameters_number);

    gsl_vector_view parameter_vector = gsl_vector_view_array(
            init_parameters, parameters_number);

    const double xtol = 1e-4;
    const double gtol = 1e-4;
    const double ftol = 1e-4;

    gsl_rng * r;
    gsl_vector *res_f;
    double chi, chi0;

    gsl_rng_env_setup();
    const gsl_rng_type * type = gsl_rng_default;
    r = gsl_rng_alloc (type);

    gsl_multifit_function_fdf f;
    f.f = &CalculationFunctionWithFlux;
    f.df = nullptr; /* set to NULL for finite-difference Jacobian */
    f.n = objects_observations.size();
    f.p = parameters_number;
    f.params = &internal_parameters;

    gsl_multifit_fdfsolver_set (solver, &f, &parameter_vector.vector);

    res_f = gsl_multifit_fdfsolver_residual(solver);
    chi0 = gsl_blas_dnrm2(res_f);
    int info;
    int status =
            gsl_multifit_fdfsolver_driver(solver, 20, xtol, gtol, ftol, &info);
    chi = gsl_blas_dnrm2(res_f);

#define FIT(i) gsl_vector_get(solver->x, i)

    fprintf(stderr, "summary from method '%s'\n",
            gsl_multifit_fdfsolver_name(solver));
    fprintf(stderr, "number of iterations: %zu\n",
            gsl_multifit_fdfsolver_niter(solver));
    fprintf(stderr, "function evaluations: %zu\n", f.nevalf);
    fprintf(stderr, "Jacobian evaluations: %zu\n", f.nevaldf);
    fprintf(stderr, "reason for stopping: %s\n",
            (info == 1) ? "small step size" : "small gradient");
    fprintf(stderr, "initial |f(x)| = %g\n", chi0);
    fprintf(stderr, "final   |f(x)| = %g\n", chi);

    {
        double dof = objects_observations.size() - parameters_number;
        double c = GSL_MAX_DBL(1, chi / sqrt(dof));

        fprintf(stderr, "chisq/dof = %g\n",  pow(chi, 2.0) / dof);

        fprintf (stderr, "Drag Coefficient      = %.5f\n", FIT(0));
        fprintf (stderr, "Inclination Angle = %.5f\n", FIT(1));
        fprintf (stderr, "Ascending Node      = %.5f\n", FIT(2));
        fprintf (stderr, "Eccentricity      = %.5f\n", FIT(3));
        fprintf (stderr, "Apsis Argument Node      = %.5f\n", FIT(4));
        fprintf (stderr, "Mean Anomaly      = %.5f\n", FIT(5));
        fprintf (stderr, "Mean Motion      = %.5f\n", FIT(6));
        fprintf (stderr, "Flux      = %.5f\n", FIT(7));
    }

    fprintf (stderr, "status = %s\n", gsl_strerror (status));

    gsl_multifit_fdfsolver_free (solver);
    gsl_rng_free (r);

    return Orbit(0, real_orbit,
                 gsl_vector_get(solver->x, 0),
                 gsl_vector_get(solver->x, 1),
                 gsl_vector_get(solver->x, 2),
                 gsl_vector_get(solver->x, 3),
                 gsl_vector_get(solver->x, 4),
                 gsl_vector_get(solver->x, 5),
                 gsl_vector_get(solver->x, 6)
    );
}

int orbitresolver::CalculationFunctionNoFlux(const gsl_vector * x, void *data,
                                             gsl_vector * f)
{
    restore_function_parameters *parameters =
            (restore_function_parameters *) data;
    const size_t result_size = parameters->objects_observations->size();
    time_t start_time =
            parameters->objects_observations->begin()->GetObservationTime();
    time_t end_time =
            parameters->objects_observations->back().GetObservationTime();
    std::list<SightObject> *objects_observations =
            parameters->objects_observations;
    RadarStation *radar = parameters->observer;

    double drag_coefficient = gsl_vector_get(x, 0);
    double inclination_angle = gsl_vector_get(x, 1);
    double ascending_node = gsl_vector_get(x, 2);
    double eccentricity = gsl_vector_get(x, 3);
    double apsis_argument = gsl_vector_get(x, 4);
    double mean_anomaly = gsl_vector_get(x, 5);
    double mean_motion = gsl_vector_get(x, 6);

    Orbit orbit(0, *parameters->real_orbit,
                drag_coefficient, inclination_angle, ascending_node,
                eccentricity, apsis_argument, mean_anomaly, mean_motion);
    orbit.SetEpoch(parameters->real_orbit->GetEpochTime());

    radar->SetSigma(0);
    radar->SetLocalFlux(0);

    ImitationDriver driver;
    driver.addRadarStation(*radar);
    driver.addSatellite(orbit);

    auto simulation_report = driver.RunImitation(start_time, end_time);
    std::list<SightObject> result_objects;
    for(auto it = simulation_report.begin(); it != simulation_report.end();
        ++it)
    {
        auto observation_report = it->second;
        for(auto observation_iterator = observation_report.begin();
            observation_iterator != observation_report.end();
            ++observation_iterator) {
            if(observation_iterator->GetObservedObjects().size() < 1)
                result_objects.push_back(SightObject(0, 0, 0, 0, 0));
            else
            {
                auto observe = *observation_iterator;
                result_objects.push_back(observe.GetObservedObjects()[0]);
            }
        }
    }

    size_t i = 0;
    for (auto it = result_objects.begin(),
                 it2 = objects_observations->begin();
         it != result_objects.end() && it2 != objects_observations->end()
         && i < result_size; ++it, ++it2, ++i)
    {
        double delta = pow(it->GetDistanceTo()
                       - it2->GetDistanceTo(), 2);
        //std::cout << "Delta : " << delta << std::endl;
        gsl_vector_set(f, i, delta);
    }

    return GSL_SUCCESS;
}

Orbit orbitresolver::RestoreOrbitNoFlux(
        Orbit real_orbit, std::list<SightObject> objects_observations,
        RadarStation observer)
{
    size_t parameters_number = 7;
    double init_parameters[parameters_number] = {
            real_orbit.GetDragCoefficient(),
            real_orbit.GetInclinationAngle(),
            real_orbit.GetAscendingNode(),
            real_orbit.GetEccentricity(),
            real_orbit.GetApsisArgument(),
            real_orbit.GetMeanAnomaly(),
            real_orbit.GetMeanMotion(),
    };

    restore_function_parameters internal_parameters = {
            &objects_observations,
            &observer,
            &real_orbit
    };

    const gsl_multifit_fdfsolver_type *solver_type =
            gsl_multifit_fdfsolver_lmder;
    gsl_multifit_fdfsolver *solver = gsl_multifit_fdfsolver_alloc(
            solver_type, objects_observations.size(), parameters_number);

    gsl_vector_view parameter_vector = gsl_vector_view_array(
            init_parameters, parameters_number);

    const double xtol = 1e-4;
    const double gtol = 1e-4;
    const double ftol = 1e-4;

    gsl_rng * r;
    gsl_vector *res_f;
    double chi, chi0;

    gsl_rng_env_setup();
    const gsl_rng_type * type = gsl_rng_default;
    r = gsl_rng_alloc (type);

    gsl_multifit_function_fdf f;
    f.f = &CalculationFunctionNoFlux;
    f.df = nullptr; /* set to NULL for finite-difference Jacobian */
    f.n = objects_observations.size();
    f.p = parameters_number;
    f.params = &internal_parameters;

    gsl_multifit_fdfsolver_set (solver, &f, &parameter_vector.vector);

    res_f = gsl_multifit_fdfsolver_residual(solver);
    chi0 = gsl_blas_dnrm2(res_f);
    int info;
    int status =
            gsl_multifit_fdfsolver_driver(solver, 20, xtol, gtol, ftol, &info);
    chi = gsl_blas_dnrm2(res_f);

#define FIT(i) gsl_vector_get(solver->x, i)

    fprintf(stderr, "summary from method '%s'\n",
            gsl_multifit_fdfsolver_name(solver));
    fprintf(stderr, "number of iterations: %zu\n",
            gsl_multifit_fdfsolver_niter(solver));
    fprintf(stderr, "function evaluations: %zu\n", f.nevalf);
    fprintf(stderr, "Jacobian evaluations: %zu\n", f.nevaldf);
    fprintf(stderr, "reason for stopping: %s\n",
            (info == 1) ? "small step size" : "small gradient");
    fprintf(stderr, "initial |f(x)| = %g\n", chi0);
    fprintf(stderr, "final   |f(x)| = %g\n", chi);

    {
        double dof = objects_observations.size() - parameters_number;
        double c = GSL_MAX_DBL(1, chi / sqrt(dof));

        fprintf(stderr, "chisq/dof = %g\n",  pow(chi, 2.0) / dof);

        fprintf (stderr, "Drag Coefficient      = %.5f\n", FIT(0));
        fprintf (stderr, "Inclination Angle = %.5f\n", FIT(1));
        fprintf (stderr, "Ascending Node      = %.5f\n", FIT(2));
        fprintf (stderr, "Eccentricity      = %.5f\n", FIT(3));
        fprintf (stderr, "Apsis Argument Node      = %.5f\n", FIT(4));
        fprintf (stderr, "Mean Anomaly      = %.5f\n", FIT(5));
        fprintf (stderr, "Mean Motion      = %.5f\n", FIT(6));
    }

    fprintf (stderr, "status = %s\n", gsl_strerror (status));

    gsl_multifit_fdfsolver_free (solver);
    gsl_rng_free (r);

    return Orbit(0, real_orbit,
                 gsl_vector_get(solver->x, 0),
                 gsl_vector_get(solver->x, 1),
                 gsl_vector_get(solver->x, 2),
                 gsl_vector_get(solver->x, 3),
                 gsl_vector_get(solver->x, 4),
                 gsl_vector_get(solver->x, 5),
                 gsl_vector_get(solver->x, 6)
    );
}
