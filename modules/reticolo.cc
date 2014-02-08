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
	/* assign spins */
	register Sito *xptr = NULL;
	register unsigned short int a;

	const signed short int *sptr = NULL;
	unsigned short int *nn = NULL;

	register unsigned int j;
	for ( unsigned int i = 0; i < L; i ++ ) {
		/* assign temporary pointer */
		xptr = *( x + i );

		for ( j = 0; j < L; j ++ ) {
			/* assign spin */
			( *xptr ).s = 2; //rand_init_val();

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
 *      Method: sweep
 * Description: Update system using Metropolis
 * ------------------------------------------------------------------
 */
void
Reticolo::sweep ( void ) {
	/* energy difference */
	short int delta;
	/* reset magnetization */
	Ms = 0;

	register Sito *xptr = NULL;
	register unsigned short int j;
	for ( unsigned short int i = 0; i < L; i ++ ) {
		/* assign row spin */
		xptr = *( x + i );

		for ( j = 0; j < L; j ++ ) {
			/* (-) energy difference */
			delta = 2 * Reticolo::single_E( i, j );

			/* (maybe) reverse spin */
			if ( (long double) rand() / RAND_MAX < expl( (long double) B * delta ) )
				( *xptr ).s = - ( *xptr ).s;

			/* update magnetization */
			Ms += ( *xptr ).s;

			/* update (column) Sito index */
			xptr ++;
		}
	}

	E = energy();
} /* -----  end of method Reticolo::sweep  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Reticolo
 *      Method: Sweep
 * Description: 
 * ------------------------------------------------------------------
 */
void
Reticolo::Sweep ( void ) {
	/* ckd temporary pointer */
	register bool *c = NULL;
	register unsigned short int j;

	/* reset magnetization (sweep, max) */
	Ms = Mm = (long int) 0;
	M2 = (unsigned long) 0;

	for ( unsigned short int i = 0; i < L; i ++ ) {
		c = *( ckd + i );

		for ( j = 0; j < L; j ++ ) {
			/* if spin had not been visited yet */
			if ( *( c ) == ckd_status )
				M2 += Reticolo::cluster( i, j );

			/* update pointer */
			c ++;
		}
	}

	/* auxiliary variable to avoid resetting ckd[][] */
	ckd_status = !( ckd_status );

	/* update energy */
	E = Reticolo::energy();
	Ms = Reticolo::magnetization();
} /* -----  end of method Reticolo::Sweep  ----- */

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
 *      Method: cluster
 * Description: 
 * ------------------------------------------------------------------
 */
unsigned long int
Reticolo::cluster ( unsigned int i, unsigned int j ) {
	/* flip spin with 50 % probability (flip = {+,-}) */
	spin flip = (spin) ( 2 * ( rand() % 2 ) - 1 );

	/* assign initial cluster site */
	**stack = i;
	*( *stack + 1 ) = j;

	/* update 'ckd' */
	*( *( ckd + i ) + j ) = !*( *( ckd + i ) + j );

	/* first free element of the stack */
	tail = 1;

//	fprintf( stderr, "\n" );

	/* counter */
	unsigned short int a;
	/* indexes */
	unsigned short int n, m;
	/* cluster size */
	unsigned long int size = (long int) 0;

	/* temporary pointers */
	unsigned short int *sptr = NULL;
	unsigned short int *nnptr = NULL;
	bool *cptr = NULL;
	Sito *xptr = NULL;

	do {
		/* take/free last stack element */
		sptr = *( stack + ( -- tail ) );
		i = *( sptr ); j = *( ++ sptr );

		/* assign (spin) temporary pointer */
		xptr = *( x + i ) + j;

//		fprintf( stdout, "(%u, %u)\n", i, j );

		/* sweep over nearest neighbours */
		for ( a = 0; a < 4; a ++ ) {
			/* assign nearest neighbour coordinate */
			nnptr = *( ( *xptr ).nn + a );

			/* assign neighbours coordinates */
			n = (unsigned) *( nnptr );
			m = (unsigned) *( ++ nnptr );

			/* assign (check bool) temporary variable */
			cptr = *( ckd + n ) + m;

			/* if spin does not belong to a cluster */
			if ( *cptr == ckd_status ) {

				/* if spin are parallel */
				if ( ( *xptr ).s == ( *( *( x + n ) + m ) ).s ) {

					/* check wether activate bond or not */
					if ( (long double) rand() / RAND_MAX <= EB ) {
						/* assign (stack) temporary pinter */
						sptr = *( stack + tail );

						/* push ( n, m ) into stack */
						*( sptr ) = n; *( ++ sptr ) = m;

						/* TikZ */
//						fprintf( stderr, "\\draw (%u, %u) -- (%u, %u);\n", i, j, n, m );

						/* increase 'tail' */
						tail ++;

						/* update 'ckd' */
						*cptr = !( *cptr );
					}
				}
			}
		}

		/* update size */
		size ++;
		/* update magnetization (sweep) */
//		Ms += (long int) ( *xptr ).s;

		/* flip spin */
		( *xptr ).s *= flip;
	} while ( tail != head );

	/* uprate magnetization (max) */
	Mm = ( size > Mm ? size : Mm );

	return size * size;
} /* -----  end of method Reticolo::cluster  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Reticolo
 *      Method: magnetization
 * Description: 
 * ------------------------------------------------------------------
 */
long int
Reticolo::magnetization ( void ) {
	/* reset magnetization */
	Ms = (long int) 0;

	register Sito *xptr = NULL;
	register unsigned short int j;
	for ( unsigned short int i = 0; i < L; i ++ ) {
		/* assign temporary pointer */
		xptr = *( x + i );

		for ( j = 0; j < L; j ++ ) {
			Ms += (long int) ( *xptr ).s;
			xptr ++;
		}
	}

	return Ms;
} /* -----  end of method Reticolo::magnetization  ----- */

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
	double norm = *corr;
	for ( unsigned short int i = 0; i < L / 2; i ++ ) {
		fprintf( stream, "%.15g\t", (double) *( corr + i ) / norm );
	}
	fprintf( stream, "\n" );
} /* -----  end of method Reticolo::print_correlator  ----- */

