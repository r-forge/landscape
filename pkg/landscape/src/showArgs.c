#include <R.h>
#include <Rinternals.h>
#include "Rdefines.h"
#include "R_ext/Rdynload.h"
#include <Rmath.h>

#include <stdio.h>
#include <stdlib.h>

#include <R_ext/PrtUtil.h>
     
SEXP showArgs(SEXP args)  {
	args = CDR(args); /* skip 'name' */
	for(int i = 0; args != R_NilValue; i++, args=CDR(args)) {
		const char *name = isNull(TAG(args)) ? "" : CHAR(PRINTNAME(TAG(args)));
        SEXP el = CAR(args);
        if (length(el) == 0) {
            Rprintf("[%d] '%s' R type, length 0\n", i+1, name);
            continue;
        }
        switch(TYPEOF(el)) {
        case REALSXP:
            Rprintf("[%d] '%s' %f\n", i+1, name, REAL(el)[0]);
             break;
        case LGLSXP:
        case INTSXP:
            Rprintf("[%d] '%s' %d\n", i+1, name, INTEGER(el)[0]);
            break;
        case CPLXSXP: {
				Rcomplex cpl = COMPLEX(el)[0];
				Rprintf("[%d] '%s' %f + %fi\n", i+1, name, cpl.r, cpl.i);
			}
            break;
        case STRSXP:
            Rprintf("[%d] '%s' %s\n", i+1, name, CHAR(STRING_ELT(el, 0)));
            break;
        default:
            Rprintf("[%d] '%s' R type\n", i+1, name);
        }
	}
    return(R_NilValue);
}	