/*
 * ==================================================================
 *
 *       Filename:  ising.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/02/2014 21:46:37
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  P. Di Giglio (), p.digiglio91@gmail.com
 *   Organization:  
 *
 * ==================================================================
 */

#include "reticolo.h"
#include "ising.h"

#include <stdio.h>
#include <stdlib.h>

#include <math.h>


/*
 * ------------------------------------------------------------------
 *       Class: Ising
 *      Method: Ising
 * Description: [default ctor]
 * ------------------------------------------------------------------
 */
Ising::Ising ( void ) {
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
			( *xptr ).s = (spin) ( 2 * ( rand() % 2 ) - 1 );

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
	E = Ising::energy();

	fprintf( stderr, "[Ising: %s]\n", __func__ );
} /* -----  end of method Ising::Ising (def. ctor)  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Ising
 *      Method: ~Ising
 * Description: destructor
 * ------------------------------------------------------------------
 */
Ising::~Ising (void) {
	fprintf( stderr, "[Ising: %s]\n", __func__ );
} /* -----  end of method Ising::~Ising (dtor)  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Ising
 *      Method: sweep
 * Description: Update system using Metropolis
 * ------------------------------------------------------------------
 */
void
Ising::sweep ( void ) {
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
			delta = 2 * Ising::single_E( i, j );

			/* (maybe) reverse spin */
			if ( (long double) rand() / RAND_MAX < expl( (long double) B * delta ) )
				( *xptr ).s = - ( *xptr ).s;

			/* update magnetization */
			Ms += ( *xptr ).s;

			/* update (column) Sito index */
			xptr ++;
		}
	}

	E = Ising::energy();
} /* -----  end of method Ising::sweep  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Ising
 *      Method: Sweep
 * Description: Update using Swendsen-Wang multi-cluster algorithm
 * ------------------------------------------------------------------
 */
void
Ising::Sweep ( void ) {
	/* ckd temporary pointer */
	register bool *c = NULL;
	register unsigned short int j;

	/* reset magnetization (sweep, max) */
	Ms = (long int) 0;
	Mm = (long int) 0;

	for ( unsigned short int i = 0; i < L; i ++ ) {
		/* assign temporary variable */
		c = *( ckd + i );

		for ( j = 0; j < L; j ++ ) {
			/* if spin had not been visited yet */
			if ( *( c ) == ckd_status )
				Ising::cluster( i, j );

			/* update pointer */
			c ++;
		}
	}

	/* auxiliary variable to avoid resetting ckd[][] */
	ckd_status = !( ckd_status );

	/* update energy */
	E = Ising::energy();

	/* uncomment this if you dont't update it in 'cluster' method */
//	Ms = Ising::magnetization();
} /* -----  end of method Ising::Sweep  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Ising
 *      Method: cluster
 * Description: 
 * ------------------------------------------------------------------
 */
unsigned long int
Ising::cluster ( unsigned int i, unsigned int j ) {
	/* flip spin with 50 % probability (flip = {+,-}) */
	spin flip = (spin) ( 2 * ( rand() % 2 ) - 1 );


	/*---------------------------------------------------------------
	 *  INITIALIZE CLUSTER SCTRUCTURES
	 *-------------------------------------------------------------*/
	/* assign initial cluster site */
	**stack = i;
	*( *stack + 1 ) = j;

	/* update 'ckd' */
	bool *cptr = *( ckd + i ) + j;
	*( cptr ) = !*( cptr );

	/* first free element of the stack */
	tail = 1;


	/*---------------------------------------------------------------
	 *  CONSTRUCT CLUSTER
	 *-------------------------------------------------------------*/
	/* counter */
	unsigned short int a;
	/* indexes */
	unsigned short int n, m;
	/* cluster size */
	unsigned long int size = (long int) 0;

	/* temporary pointers */
	unsigned short int *sptr = NULL;
	unsigned short int *nnptr = NULL;
	Sito *xptr = NULL;

	do {
		/* take/free last stack element */
		sptr = *( stack + ( -- tail ) );
		i = *( sptr ); j = *( ++ sptr );

		/* assign (spin) temporary pointer */
		xptr = *( x + i ) + j;

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
					if ( (long double) rand() / RAND_MAX <= Ising::EB ) {
						/* assign (stack) temporary ptr */
						sptr = *( stack + tail );

						/* push ( n, m ) into stack */
						*( sptr ) = n; *( ++ sptr ) = m;

						/* increase tail */
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
		Ms += (long int) ( *xptr ).s;

		/* flip spin */
		( *xptr ).s *= flip;
	} while ( tail != head );

	/* update magnetization (max) */
	Mm = ( size > Mm ? size : Mm );

	return size * size;
} /* -----  end of method Ising::cluster  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Ising
 *      Method: magnetization
 * Description: 
 * ------------------------------------------------------------------
 */
long int
Ising::magnetization ( void ) {
	/* reset magnetization */
	Ms = (long int) 0;

	register Sito *xptr = NULL;
	Sito *stop = NULL;
	for ( unsigned short int i = 0; i < L; i ++ ) {
		/* assign (stop) temporary pointer */
		stop = *( x + i ) + L;

		for ( xptr = *( x + i ); xptr != stop; xptr ++ )
			Ms += (long int) ( *xptr ).s;
	}

	return Ms;
} /* -----  end of method Ising::magnetization  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Ising
 *      Method: energy
 * Description: 
 * ------------------------------------------------------------------
 */
long int
Ising::energy ( void ) {
	/* energy temporary variable */
	long int ene = 0;

	register short int j;
	/* cycle over even sites only */
	short int i1 = 1;
	for ( unsigned short int i = 0; i < L; i += 2 ) {
		for ( j = 0; j < L; j ++ ) {
			ene += Ising::single_E( i, j );
			/* translater lattice */
			ene += Ising::single_E( i1, ++ j );
		}

		i1 += 2;
	}

	return ene;
} /* -----  end of method Ising::energy  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Ising
 *      Method: single_E
 * Description: evaluate single-spin energy
 * ------------------------------------------------------------------
 */
short int
Ising::single_E ( unsigned int i, unsigned int j ) {
	/* energy, temporary Sito ptr */
	short int temp = 0;
	Sito *xptr = *( x + i ) + j;

	/* sweep over nearest neighbours */
	for ( unsigned short int a = 0; a < 4; a ++ )
		temp += Reticolo::S( *( (*xptr).nn + a ) );

	/* multiply by (i,j)-spin */
	temp *= ( *xptr ).s;

	return - temp;
} /* -----  end of method Ising::single_E  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Ising
 *      Method: correlator
 * Description: 
 * ------------------------------------------------------------------
 */
void
Ising::correlator ( void ) {
	/* temporary pointer */
	register Sito *xptr = NULL;
	Sito *stop = NULL;

	/* temporary variables (mean over rows and columns) */
	short int r[L] = {}, c[L] = {};
	register short int *cptr = NULL;
	short int *rptr = NULL;

	/* sweep all over lattice */
	for ( unsigned short int i = 0; i < L; i ++ ) {
		/* reset correlator */
		*( corr + i ) = (double) 0;

		/* assign array pointers */
		rptr = r + i;
		cptr = c;

		/* assign (stop) temporary site pointer */
		stop = *( x + i ) + L;
		/* evaluate means over columns and rows */
		for ( xptr = *( x + i ); xptr != stop; xptr ++ ) {
			/* update means */
			*rptr += ( *xptr ).s;

			/* take (i,j)-th site in += j-th column correlator */ 
			*cptr += ( *xptr ).s;
			/* update column correlator pointer */
			cptr ++;
		}
	}

	/* correlator pointer */
	register double *sptr = NULL;
	register unsigned short int j;
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

} /* -----  end of method Ising::correlator  ----- */
