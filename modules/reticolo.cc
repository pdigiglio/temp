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

