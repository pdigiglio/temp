/*
 * ==================================================================
 *
 *       Filename:  reticolo.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  25/12/2013 20:40:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  P. Di Giglio (), p.digiglio91@gmail.com
 *   Organization:  
 *
 * ==================================================================
 */

#include "reticolo.h"

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

/*
 * ------------------------------------------------------------------
 *       Class: Reticolo
 *      Method: Reticolo
 * Description: [ctor]
 * ------------------------------------------------------------------
 */
Reticolo::Reticolo (void) {
	register Sito *xptr = NULL;
	register unsigned short int a;

	const signed short int *sptr = NULL;
	unsigned short int *nn = NULL;

	register unsigned short int j;
	for ( unsigned short int i = 0; i < L; i ++ ) {
		/* assign temporary pointer */
		xptr = *( x + i );

		for ( j = 0; j < L; j ++ ) {
			/* assign spin */
			( *xptr ).s = rand_init_val();

			/* assign nearest neighbours */
			for ( a = 0; a < 4; a ++ ) {	
				/* assign (displacement) temporary pointer */
				sptr = *( s + a );
				nn = *( ( *xptr ).nn + a );

				/* assign neighbours coordinates */
				*nn = (unsigned) ( (signed) L + i + *( sptr ) ) % L;
				*( ++ nn ) = (unsigned) ( (signed) L + j + *( ++ sptr ) ) % L;
			}

			/* update pointer Sito */
			xptr ++;
		}
	}

	/* assign energy */
	E = Reticolo::energy();

	fprintf( stderr, "[Reticolo: %s]\n", __func__ );
} /* -----  end of method Reticolo::Reticolo (ctor)  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Reticolo
 *      Method: ~Reticolo
 * Description: destructor
 * ------------------------------------------------------------------
 */
Reticolo::~Reticolo (void) {
	fprintf( stderr, "[Reticolo: %s]\n", __func__ );
} /* -----  end of method Reticolo::~Reticolo (dtor)  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Reticolo
 *      Method: energy
 * Description: 
 * ------------------------------------------------------------------
 */
long int
Reticolo::energy ( void ) {
	/* energy temporary variable */
	long int ene = 0;

	register short int j;
	/* cycle over even sites only */
	short int i1 = 1;
	for ( unsigned short int i = 0; i < L; i += 2 ) {
		for ( j = 0; j < L; j ++ ) {
			ene += Reticolo::single_E( i, j );
			/* translater lattice */
			ene += Reticolo::single_E( i1, ++ j );
		}

		i1 += 2;
	}

	return ene;
} /* -----  end of method Reticolo::energy  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Reticolo
 *      Method: correlator
 * Description: 
 * ------------------------------------------------------------------
 */
void
Reticolo::correlator ( void ) {
	/* temporary pointer */
	register Sito *xptr = NULL;

	/* temporary variables (mean over rows and columns) */
	short int r[L] = {}, c[L] = {};
	register short int *cptr = NULL;
	short int *rptr = NULL;

	/* sweep all over lattice */
	register unsigned short int j;
	for ( unsigned short int i = 0; i < L; i ++ ) {
		/* reset correlator */
		*( corr + i ) = (double) 0;

		/* assign temporary site pointer */
		xptr = *( x + i );

		/* assign array pointers */
		rptr = r + i;
		cptr = c;

		/* evaluate means over columns and rows */
		for ( j = 0; j < L; j ++ ) {
			/* update means */
			*rptr += ( *xptr ).s;
			*cptr += ( *xptr ).s;

			/* update pointers */
			xptr ++;
			cptr ++;
		}
	}

	/* correlator pointer */
	register double *sptr = NULL;
	/* evaluate correlators */
	for ( unsigned short int i = 0; i < L; i ++ ) {
		/* set correlator pointer to t = 0 */
		sptr = corr;

		/* reuse old pointers */
		rptr = r + i;
		cptr = c + i;

		for ( j = 0; j < L - i; j ++ ) {
			/* rows */
			*sptr += *( rptr ) * *( rptr + j );
			/* columns */
			*sptr += *( cptr ) * *( cptr + j );

			/* update pointer */
			sptr ++;
		}

		for ( j = 0; j < i; j ++ ) {
			/* rows */
			*sptr += *( rptr ) * *( r + j );
			/* columns */
			*sptr += *( cptr ) * *( c + j );

			/* update pointer */
			sptr ++;
		}
	}

} /* -----  end of method Reticolo::correlator  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Reticolo
 *      Method: print_lattice
 * Description: 
 * ------------------------------------------------------------------
 */
void
Reticolo::print_lattice ( void ) {
	FILE *stream = stderr;
	register signed short int j;
	for ( unsigned short int i = 0; i < L; i ++ ) {
		for ( j = 0; j < L; j ++ ) {
//		for ( j = L; j > 0; j -- ) {
//			fprintf( stream, "%u ", ( *( *( x + i ) + j ) ).s );

			if ( (*( *( x + i ) + j )).s == 1 ) /* TiKz */
				fprintf( stream, "%u/%u, ", i, j );
		}
//		fprintf( stream, "\n" );
	}
} /* -----  end of method Reticolo::print_lattice  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Reticolo
 *      Method: single_E
 * Description: evaluate single-spin energy
 * ------------------------------------------------------------------
 */
short int
Reticolo::single_E ( unsigned int i, unsigned int j ) {
	/* energy, temporary Sito ptr */
	short int temp = 0;
	Sito *xptr = *( x + i ) + j;

	/* sweep over nearest neighbours */
	for ( unsigned short int a = 0; a < 4; a ++ )
		temp += Reticolo::S( *( (*xptr).nn + a ) );

	/* multiply by (i,j)-spin */
	temp *= ( *xptr ).s;

	return - temp;
} /* -----  end of method Reticolo::single_E  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Reticolo
 *      Method: print_correlator
 * Description: 
 * ------------------------------------------------------------------
 */
void
Reticolo::print_correlator ( FILE *stream ) {
	/* normalization */
	double norm = *corr;

	/* pointer/counter stopper */
	double *stop = corr + L / 2;
	for ( register double *cptr = corr; cptr != stop; cptr ++ )
		fprintf( stream, "%.15g\t", (double) *cptr / norm );

	fprintf( stream, "\n" );
} /* -----  end of method Reticolo::print_correlator  ----- */

