#include <gsl_errno.h>


inline BASE *
FUNCTION(gsl_vector,ptr)(const TYPE(gsl_vector) * v, const size_t i)
{
  if (gsl_check_range) 
    {
      if (i >= v->size) /* size_t is unsigned, can't be negative */
	{
	  GSL_ERROR_RETURN("index out of range", GSL_EINVAL, 0) ;
	}
    }

  return (BASE *) (v->data + i) ;
}


inline BASE
FUNCTION(gsl_vector,get)(const TYPE(gsl_vector) * v, const size_t i)
{
  if (gsl_check_range) 
    {
      if (i >= v->size) /* size_t is unsigned, can't be negative */
	{
	  const BASE zero = ZERO ;
	  GSL_ERROR_RETURN("index out of range", GSL_EINVAL, zero) ;
	}
    }

  return /* v->data[i]; */
  *(BASE *)(v->data + MULTIPLICITY*i);
}


inline void
FUNCTION(gsl_vector,set)(TYPE(gsl_vector) * v, const size_t i, BASE x)
{
  if (gsl_check_range) 
    {
      if (i >= v->size) /* size_t is unsigned, can't be negative */
	{
	  GSL_ERROR_RETURN_NOTHING("index out of range", GSL_EINVAL) ;
	}
    }

  /* v->data[i] = x; */
  *(BASE *)(v->data + MULTIPLICITY*i) = x;
}
