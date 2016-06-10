#include <getopt.h>
#include <iostream>
#include <cstring>


#include "librsim/ImitationDriver.h"
#include "librsim/astro/TLEReader.h"

#include <gsl/gsl_rng.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit_nlin.h>

/*static const char *opt_string = "h";

static const struct option long_opts[] =
    {
        {"start-latitude",  required_argument, NULL, 1},
        {"start-longitude", required_argument, NULL, 2},
        {"end-latitude",    required_argument, NULL, 3},
        {"end-longitude",   required_argument, NULL, 4}
    };*/
//int calculate

double delta(SightObject &object1, SightObject &object2)
{
    double d = fabs(object1.GetDistanceTo()) - fabs(object2.GetDistanceTo());
    double z = fabs(object1.GetZenitAngle()) - fabs(object2.GetZenitAngle());
    double a = fabs(object1.GetAzimuthAngle()) - fabs(object2.GetAzimuthAngle());

    return d;
}

struct data {
    const size_t result_size;
    time_t start_time;
    time_t end_time;
    time_t epoch;
    RadarStation *radar_station;
    std::list<SightObject> *original_result_objects;
};

int simulate_f(const gsl_vector * x, void *data,
             gsl_vector * f)
{
    time_t start_time = ((struct data *) data)->start_time;
    time_t end_time = ((struct data *) data)->end_time;
    time_t epoch = ((struct data *) data)->epoch;
    RadarStation *radar = ((struct data *) data)->radar_station;
    std::list<SightObject> *original_result_objects =
        ((struct data *) data)->original_result_objects;
    const size_t result_size = original_result_objects->size();

    double drag_coefficient = 0;//gsl_vector_get(x, 0);
    double inclination_angle = gsl_vector_get(x, 0);
    double ascending_node = gsl_vector_get(x, 1);
    double eccentricity = gsl_vector_get(x, 2);
    double apsis_argument = gsl_vector_get(x, 3);
    double mean_anomaly = gsl_vector_get(x, 4);
    double mean_motion = gsl_vector_get(x, 5);

    Orbit orbit(0, epoch, drag_coefficient, inclination_angle, ascending_node,
                eccentricity, apsis_argument, mean_anomaly, mean_motion);

    radar->SetSigma(0);
    radar->SetLocalFlux(gsl_vector_get(x, 6));

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
                continue;
            else
            {
                auto observe = *observation_iterator;
                result_objects.push_back(observe.GetObservedObjects()[0]);
            }
        }
    }

    if(result_objects.size() < 1)
        return GSL_CONTINUE;

    size_t i = 0;
    for (auto it = result_objects.begin(),
             it_o = original_result_objects->begin();
         it != result_objects.end()
         && it_o != original_result_objects->end()
         && i < result_size; ++it, ++it_o, ++i)
    {
        auto current = *it;
        auto original = *it_o;
        double d_delta = delta(current, original);

        gsl_vector_set(f, i, d_delta);
    }

    return GSL_CONTINUE;
}

int simulate(const gsl_vector * x, void *data,
             gsl_vector * f)
{
    time_t start_time = ((struct data *) data)->start_time;
    time_t end_time = ((struct data *) data)->end_time;
    time_t epoch = ((struct data *) data)->epoch;
    RadarStation *radar = ((struct data *) data)->radar_station;
    std::list<SightObject> *original_result_objects =
        ((struct data *) data)->original_result_objects;
    const size_t result_size = original_result_objects->size();

    double drag_coefficient = 0;//gsl_vector_get(x, 0);
    double inclination_angle = gsl_vector_get(x, 0);
    double ascending_node = gsl_vector_get(x, 1);
    double eccentricity = gsl_vector_get(x, 2);
    double apsis_argument = gsl_vector_get(x, 3);
    double mean_anomaly = gsl_vector_get(x, 4);
    double mean_motion = gsl_vector_get(x, 5);

    Orbit orbit(0, epoch, drag_coefficient, inclination_angle, ascending_node,
                eccentricity, apsis_argument, mean_anomaly, mean_motion);

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
                return GSL_CONTINUE;
            else
            {
                auto observe = *observation_iterator;
                result_objects.push_back(observe.GetObservedObjects()[0]);
            }
        }
    }

    size_t i = 0;
    for (auto it = result_objects.begin(),
             it_o = original_result_objects->begin();
         it != result_objects.end()
         && it_o != original_result_objects->end()
         && i < result_size; ++it, ++it_o, ++i)
    {
        auto current = *it;
        auto original = *it_o;
        double d_delta = delta(current, original);
        gsl_vector_set(f, i, d_delta);
    }

    return GSL_CONTINUE;
}

int expb_df (const gsl_vector * x, void *data,
         gsl_matrix * J)
{
    time_t start_time = ((struct data *) data)->start_time;
    time_t end_time = ((struct data *) data)->end_time;
    time_t epoch = ((struct data *) data)->epoch;
    RadarStation *radar = ((struct data *) data)->radar_station;
    std::list<SightObject> *original_result_objects =
        ((struct data *) data)->original_result_objects;
    const size_t result_size = original_result_objects->size();

    double drag_coefficient = 0;//gsl_vector_get(x, 0);
    double inclination_angle = gsl_vector_get(x, 0);
    double ascending_node = gsl_vector_get(x, 1);
    double eccentricity = gsl_vector_get(x, 2);
    double apsis_argument = gsl_vector_get(x, 3);
    double mean_anomaly = gsl_vector_get(x, 4);
    double mean_motion = gsl_vector_get(x, 5);

    size_t i;

    for (i = 0; i < result_size; i++)
    {
        /* Jacobian matrix J(i,j) = dfi / dxj, */
        /* where fi = (Yi - yi)/sigma[i],      */
        /*       Yi = A * exp(-lambda * i) + b  */
        /* and the xj are the parameters (A,lambda,b) */

        gsl_matrix_set (J, i, 0, 1/inclination_angle);
        gsl_matrix_set (J, i, 1, 1/ascending_node);
        gsl_matrix_set (J, i, 2, 1/eccentricity);
        gsl_matrix_set (J, i, 3, 1/apsis_argument);
        gsl_matrix_set (J, i, 4, 1/mean_anomaly);
        gsl_matrix_set (J, i, 5, 1/mean_motion);

    }
    return GSL_SUCCESS;
}

Orbit fit_f(std::list<SightObject> true_objects, Orbit true_orbit,
          RadarStation radarStation)
{
    const size_t n = true_objects.size();

    struct data d = {
            n,
            true_objects.begin()->GetObservationTime(),
            true_objects.back().GetObservationTime(),
            true_orbit.GetEpochTime(),
            &radarStation,
            &true_objects
    };

    const gsl_multifit_fdfsolver_type *T = gsl_multifit_fdfsolver_lmsder;
    gsl_multifit_fdfsolver *s;
    int status, info;
    size_t i;
    const size_t p = 7;

    gsl_matrix *J = gsl_matrix_alloc(n, p);
    gsl_matrix *covar = gsl_matrix_alloc (p, p);
    double y[n], weights[n];
    for(int i = 0; i < n; ++i)
        weights[i] = 1;
    memset(weights, 0, sizeof(weights));
    gsl_multifit_function_fdf f;

    double x_init[p] = {
            //true_orbit.GetDragCoefficient(),
            true_orbit.GetInclinationAngle(),
            true_orbit.GetAscendingNode(),
            true_orbit.GetEccentricity(),
            true_orbit.GetApsisArgument(),
            true_orbit.GetMeanAnomaly(),
            true_orbit.GetMeanMotion(),
            0.0
    };
    gsl_vector_view x = gsl_vector_view_array (x_init, p);
    gsl_vector_view w = gsl_vector_view_array(weights, n);

    const gsl_rng_type * type;
    gsl_rng * r;
    gsl_vector *res_f;
    double chi, chi0;

    const double xtol = 10;
    const double gtol = 10;
    const double ftol = 10;

    gsl_rng_env_setup();

    type = gsl_rng_default;
    r = gsl_rng_alloc (type);

    f.f = &simulate_f;
    f.df = NULL;   /* set to NULL for finite-difference Jacobian */
    f.n = n;
    f.p = p;
    f.params = &d;

    s = gsl_multifit_fdfsolver_alloc (T, n, p);
    /* initialize solver with starting point and weights */
    gsl_multifit_fdfsolver_wset (s, &f, &x.vector, &w.vector);

    /* compute initial residual norm */
    res_f = gsl_multifit_fdfsolver_residual(s);
    chi0 = gsl_blas_dnrm2(res_f);

    /* solve the system with a maximum of 20 iterations */
    status = gsl_multifit_fdfsolver_driver(s, 20, xtol, gtol, ftol, &info);

    gsl_multifit_fdfsolver_jac(s, J);
    gsl_multifit_covar (J, 0.0, covar);

    /* compute final residual norm */
    chi = gsl_blas_dnrm2(res_f);
#define FIT(i) gsl_vector_get(s->x, i)
#define ERR(i) sqrt(gsl_matrix_get(covar,i,i))

    fprintf(stderr, "summary from method '%s'\n",
            gsl_multifit_fdfsolver_name(s));
    fprintf(stderr, "number of iterations: %zu\n",
            gsl_multifit_fdfsolver_niter(s));
    fprintf(stderr, "function evaluations: %zu\n", f.nevalf);
    fprintf(stderr, "Jacobian evaluations: %zu\n", f.nevaldf);
    fprintf(stderr, "reason for stopping: %s\n",
            (info == 1) ? "small step size" : "small gradient");
    fprintf(stderr, "initial |f(x)| = %g\n", chi0);
    fprintf(stderr, "final   |f(x)| = %g\n", chi);

    {
        double dof = n - p;
        double c = GSL_MAX_DBL(1, chi / sqrt(dof));

        fprintf(stderr, "chisq/dof = %g\n",  pow(chi, 2.0) / dof);

        //fprintf (stderr, "drag_coefficient_      = %.5f +/- %.5f\n", FIT(0), c*ERR(0));
        fprintf (stderr, "inclination_angle_ = %.5f +/- %.5f\n", FIT(0), c*ERR(0));
        fprintf (stderr, "ascending_node_      = %.5f +/- %.5f\n", FIT(1), c*ERR(1));
        fprintf (stderr, "eccentricity_      = %.5f +/- %.5f\n", FIT(2), c*ERR(2));
        fprintf (stderr, "apsis_argument_      = %.5f +/- %.5f\n", FIT(3), c*ERR(3));
        fprintf (stderr, "mean_anomaly_      = %.5f +/- %.5f\n", FIT(4), c*ERR(4));
        fprintf (stderr, "mean_motion_      = %.5f +/- %.5f\n", FIT(5), c*ERR(5));
        fprintf (stderr, "flux_      = %.5f +/- %.5f\n", FIT(6), c*ERR(6));
    }

    fprintf (stderr, "status = %s\n", gsl_strerror (status));

    gsl_multifit_fdfsolver_free (s);
    gsl_matrix_free (covar);
    gsl_matrix_free (J);
    gsl_rng_free (r);

    return Orbit();
}

Orbit fit(std::list<SightObject> &true_objects, Orbit &true_orbit,
          RadarStation &radarStation)
{
    const size_t n = true_objects.size();

    struct data d = {
            n,
            true_objects.begin()->GetObservationTime(),
            true_objects.back().GetObservationTime(),
            true_orbit.GetEpochTime(),
            &radarStation,
            &true_objects
    };

    const gsl_multifit_fdfsolver_type *T = gsl_multifit_fdfsolver_lmsder;
    gsl_multifit_fdfsolver *s;
    int status, info;
    size_t i;
    const size_t p = 6;

    gsl_matrix *J = gsl_matrix_alloc(n, p);
    gsl_matrix *covar = gsl_matrix_alloc (p, p);
    double y[n], weights[n];
    gsl_multifit_function_fdf f;

    double x_init[p] = {
            //true_orbit.GetDragCoefficient(),
            true_orbit.GetInclinationAngle(),
            true_orbit.GetAscendingNode(),
            true_orbit.GetEccentricity(),
            true_orbit.GetApsisArgument(),
            true_orbit.GetMeanAnomaly(),
            true_orbit.GetMeanMotion()
    };
    gsl_vector_view x = gsl_vector_view_array (x_init, p);
    gsl_vector_view w = gsl_vector_view_array(weights, n);

    const gsl_rng_type * type;
    gsl_rng * r;
    gsl_vector *res_f;
    double chi, chi0;

    const double xtol = 10;
    const double gtol = 10;
    const double ftol = 10;

    gsl_rng_env_setup();

    type = gsl_rng_default;
    r = gsl_rng_alloc (type);

    f.f = &simulate;
    f.df = &expb_df;   /* set to NULL for finite-difference Jacobian */
    f.n = n;
    f.p = p;
    f.params = &d;

    s = gsl_multifit_fdfsolver_alloc (T, n, p);
    /* initialize solver with starting point and weights */
    gsl_multifit_fdfsolver_wset (s, &f, &x.vector, &w.vector);

    /* compute initial residual norm */
    res_f = gsl_multifit_fdfsolver_residual(s);
    chi0 = gsl_blas_dnrm2(res_f);

    /* solve the system with a maximum of 20 iterations */
    status = gsl_multifit_fdfsolver_driver(s, 20, xtol, gtol, ftol, &info);

    gsl_multifit_fdfsolver_jac(s, J);
    gsl_multifit_covar (J, 0.0, covar);

    /* compute final residual norm */
    chi = gsl_blas_dnrm2(res_f);
#define FIT(i) gsl_vector_get(s->x, i)
#define ERR(i) sqrt(gsl_matrix_get(covar,i,i))

    fprintf(stderr, "summary from method '%s'\n",
            gsl_multifit_fdfsolver_name(s));
    fprintf(stderr, "number of iterations: %zu\n",
            gsl_multifit_fdfsolver_niter(s));
    fprintf(stderr, "function evaluations: %zu\n", f.nevalf);
    fprintf(stderr, "Jacobian evaluations: %zu\n", f.nevaldf);
    fprintf(stderr, "reason for stopping: %s\n",
            (info == 1) ? "small step size" : "small gradient");
    fprintf(stderr, "initial |f(x)| = %g\n", chi0);
    fprintf(stderr, "final   |f(x)| = %g\n", chi);

    {
        double dof = n - p;
        double c = GSL_MAX_DBL(1, chi / sqrt(dof));

        fprintf(stderr, "chisq/dof = %g\n",  pow(chi, 2.0) / dof);

        //fprintf (stderr, "drag_coefficient_      = %.5f +/- %.5f\n", FIT(0), c*ERR(0));
        fprintf (stderr, "inclination_angle_ = %.5f +/- %.5f\n", FIT(0), c*ERR(0));
        fprintf (stderr, "ascending_node_      = %.5f +/- %.5f\n", FIT(1), c*ERR(1));
        fprintf (stderr, "eccentricity_      = %.5f +/- %.5f\n", FIT(2), c*ERR(2));
        fprintf (stderr, "apsis_argument_      = %.5f +/- %.5f\n", FIT(3), c*ERR(3));
        fprintf (stderr, "mean_anomaly_      = %.5f +/- %.5f\n", FIT(4), c*ERR(4));
        fprintf (stderr, "mean_motion_      = %.5f +/- %.5f\n", FIT(5), c*ERR(5));
    }

    fprintf (stderr, "status = %s\n", gsl_strerror (status));

    gsl_multifit_fdfsolver_free (s);
    gsl_matrix_free (covar);
    gsl_matrix_free (J);
    gsl_rng_free (r);

    return Orbit();
}

int main(int argc, char **argv)
{
    //setlocale(LC_ALL, "Russian");
    //ImitationDriver imitationDriver = ImitationDriver();
    //imitationDriver.RunImitation(10, 1100, 0);
    try {
        std::map<int, Orbit> orbits =
            TLEReader::ReadSatellitesFromFile("data/visual.tle");
        ImitationDriver imitation_driver;
        imitation_driver.addSatellite(orbits[25544]);

        RadarStation radar("Experemntal Radar", 8.95, 50.67, 500,
                           astroutils::DegToRad(45), astroutils::DegToRad(45),
                           astroutils::DegToRad(70), astroutils::DegToRad(135),
                           70000000, 178
        );
        imitation_driver.addRadarStation(radar);

        auto simulation_report =
                imitation_driver.RunImitation(1465416819, 1465576329);

        std::list<SightObject> result_objects;
        for(auto it = simulation_report.begin(); it != simulation_report.end();
                ++it)
        {
            auto observation_report = it->second;
            for(auto observation_iterator = observation_report.begin();
                    observation_iterator != observation_report.end();
                    ++observation_iterator) {
                if(observation_iterator->GetObservedObjects().size() < 1)
                    continue;
                auto observe = *observation_iterator;
                result_objects.push_back(observe.GetObservedObjects()[0]);
            }
        }

        if(result_objects.size() < 1)
        {
            std::cout << "No observation" << std::endl;
            exit(EXIT_FAILURE);
        }

        fit_f(result_objects, orbits[25544], radar);
        fit(result_objects, orbits[25544], radar);

    } catch (TLEFormatException &e) {
        std::cout << e.what();
    }

    exit(EXIT_SUCCESS);
}
