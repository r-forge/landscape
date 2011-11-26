/* Robert Hijmans, November 2011 */

#include <R.h>
#include <Rinternals.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Rmath.h>
#include "Rdefines.h"
#include "R_ext/Rdynload.h"


SEXP test(SEXP x) {
	SEXP res;
	int *xres;

	PROTECT(res = allocVector( INTSXP, 1) );
	xres = INTEGER(res);
	xres[0] = 99;
	return(res);
}


