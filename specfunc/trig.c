/* Author:  G. Jungman
 * RCS:     $Id$
 */
#include <stdio.h>
#include <math.h>
#include <gsl_math.h>
#include <gsl_errno.h>
#include "gsl_sf_log.h"
#include "gsl_sf_trig.h"


/*-*-*-*-*-*-*-*-*-*-*-* (semi)Private Implementations *-*-*-*-*-*-*-*-*-*-*-*/

int gsl_sf_complex_sin_impl(const double zr, const double zi, double * szr, double * szi)
{
  if(fabs(zi) < GSL_LOG_DBL_MAX) {
    double ex = exp(zi);
    double sh = 0.5*(ex-1./ex);
    double ch = 0.5*(ex+1./ex);
    *szr = sin(zr)*ch;
    *szi = cos(zr)*sh;
    return GSL_SUCCESS;
  }
  else {
    return GSL_EOVRFLW;
  }
}

int gsl_sf_complex_logsin_impl(const double zr, const double zi, double * lszr, double * lszi)
{
  if(zi > 60.) {
    *lszr = -M_LN2 + zi;
    *lszi =  0.5*M_PI - zr;
  }
  else if(zi < -60.) {
    *lszr = -M_LN2 - zi;
    *lszi = -0.5*M_PI + zr; 
  }
  else {
    double sin_r, sin_i;
    int status;
    gsl_sf_complex_sin_impl(zr, zi, &sin_r, &sin_i);
    status = gsl_sf_complex_log_impl(sin_r, sin_i, lszr, lszi);
    if(status == GSL_EDOM) {
      return GSL_EDOM;
    }
  }
  return gsl_sf_angle_restrict_symm_impl(lszi, GSL_MACH_EPS * 1.e5);
}

int gsl_sf_complex_cos_impl(const double zr, const double zi, double * czr, double * czi)
{
  if(fabs(zi) < GSL_LOG_DBL_MAX) {
    double ex = exp(zi);
    double sh = 0.5*(ex-1./ex);
    double ch = 0.5*(ex+1./ex);
    *czr =  cos(zr)*ch;
    *czi = -sin(zr)*sh;
    return GSL_SUCCESS;
  }
  else {
    return GSL_EOVRFLW;
  }
}

/*
inline int gsl_sf_sincos_impl(const double theta, double * s, double * c)
{
  double tan_half = tan(0.5 * theta);
  double den = 1. + tan_half*tan_half;
  double cos_theta = (tan_half*tan_half - 1.) / den;
  double sin_theta = 2. * tan_half / den;
}
*/

int gsl_sf_polar_to_rect_impl(const double r, const double theta, double * x, double * y)
{
  if(fabs(theta) == M_PI) {
    *x = r;
    *y = 0.;
  }
  else {
    double tan_half = tan(0.5 * theta);
    double den = 1. + tan_half*tan_half;
    double cos_theta = (tan_half*tan_half - 1.) / den;
    double sin_theta = 2. * tan_half / den;
    *x = r * cos_theta;
    *y = r * sin_theta;
  }
  return GSL_SUCCESS;
}

int gsl_sf_rect_to_polar_impl(const double x, const double y, double * r, double * theta)
{
  *r = hypot(x, y);
  if(*r > 0.) {
    *theta = atan2(y, x);
    return GSL_SUCCESS;
  }
  else {
    *theta = 0.;
    return GSL_EDOM;
  }
}

int gsl_sf_angle_restrict_symm_impl(double * theta, const double precision)
{
/*
  int status;
  double x;
  
  if(fabs(*theta) * GSL_MACH_EPS >  precision) {
    status = GSL_ELOSS;
  }
  else {
    status = GSL_SUCCESS;
  }
  
  x = *theta/(2.*M_PI);
  *theta = (x - floor(x)) * 2.*M_PI;
  if(*theta > M_PI) *theta -= 2.*M_PI;
  return status;
*/
  const double P1 = 4.0 * 7.85398125648498535156e-1;
  const double P2 = 4.0 * 3.77489470793079817668e-8;
  const double P3 = 4.0 * 2.69515142907905952645e-15;
  double t = *theta;
  double y = floor(t/M_PI);
  *theta = ((t - y*P1) - y*P2) - y*P3;
  if(t > 1.0/GSL_SQRT_MACH_EPS)
    return GSL_ELOSS;
  else
    return GSL_SUCCESS;
}

int gsl_sf_angle_restrict_pos_impl(double * theta, const double precision)
{
  int status;
  double x;

  if(fabs(*theta) * GSL_MACH_EPS >  precision) {
    status = GSL_ELOSS;
  }
  else {
    status = GSL_SUCCESS;
  }
  
  x = *theta/(2.*M_PI);
  *theta = (x - floor(x)) * 2.*M_PI;
  return status;
}


/*-*-*-*-*-*-*-*-*-*-*-* Functions w/ Error Handling *-*-*-*-*-*-*-*-*-*-*-*/

int gsl_sf_complex_sin_e(const double zr, const double zi, double * szr, double * szi)
{
  int status = gsl_sf_complex_sin_impl(zr, zi, szr, szi);
  if(status != GSL_SUCCESS) {
    GSL_ERROR("gsl_sf_complex_sin_e", status);
  }
  return status;
}

int gsl_sf_complex_logsin_e(const double zr, const double zi, double * lszr, double * lszi)
{
  int status = gsl_sf_complex_logsin_impl(zr, zi, lszr, lszi);
  if(status != GSL_SUCCESS) {
    GSL_ERROR("gsl_sf_complex_logsin_e", status);
  }
  return status;
}

int gsl_sf_complex_cos_e(const double zr, const double zi, double * czr, double * czi)
{
  int status = gsl_sf_complex_cos_impl(zr, zi, czr, czi);
  if(status != GSL_SUCCESS) {
    GSL_ERROR("gsl_sf_complex_cos_e", status);
  }
  return status;
}

int gsl_sf_polar_to_rect_e(const double r, const double theta, double * x, double * y)
{
  int status = gsl_sf_polar_to_rect_impl(r, theta, x, y);
  if(status != GSL_SUCCESS) {
    GSL_ERROR("gsl_sf_polar_to_rect_e", status);
  }
  return status;
}

int gsl_sf_rect_to_polar_e(const double x, const double y, double * r, double * theta)
{
  int status = gsl_sf_rect_to_polar_impl(x, y, r, theta);
  if(status != GSL_SUCCESS) {
    GSL_ERROR("gsl_sf_rect_to_polar_e", status);
  }
  return status;
}

int gsl_sf_angle_restrict_symm_e(double * theta, const double precision)
{
  int status = gsl_sf_angle_restrict_symm_impl(theta, precision);
  if(status != GSL_SUCCESS) {
    GSL_ERROR("gsl_sf_angle_restrict_symm_e", status);
  }
  return status;
}

int gsl_sf_angle_restrict_pos_e(double * theta, const double precision)
{
  int status = gsl_sf_angle_restrict_pos_impl(theta, precision);
  if(status != GSL_SUCCESS) {
    GSL_ERROR("gsl_sf_angle_restrict_pos_e", status);
  }
  return status;
}
