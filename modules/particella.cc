#include "parameters.h"
#include "particella.h"

#include <stdio.h>

/*
 * ------------------------------------------------------------------
 *       Class: Particella
 *      Method: sp
 * Description: Scalar product between two vectors, a and b, in D
 * 				dimensions
 * ------------------------------------------------------------------
 */
double
Particella::sp ( const double *a, const double *b ) {
	double tmp = (double) 0;
	for ( unsigned short int d = 0; d < D; d ++ ) {
		tmp += *( a + d ) * *( b + d );
//		printf( "%g * %g ", *( a + d ), *( b + d ) );
	}
//	printf( "\n" );

	return tmp;
} /* -----  end of method Particella::sp  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Particella
 *      Method: sp
 * Description: The same as above. If only 'a' is given, it returns
 * 				his square modulus
 * ------------------------------------------------------------------
 */
double
Particella::sp ( const double *a ) {
	double tmp = (double) 0;
	for ( unsigned short int d = 0; d < D; d ++ )
		tmp += *( a + d ) * *( a + d );

	return tmp;
} /* -----  end of method Particella::sp  ----- */
