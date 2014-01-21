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
			/* assign spin ( 1 or -1 ) */
			( *xptr ).s = 2 * ( rand() % 2 ) - 1;

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
	M = 0;

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
			M += ( *xptr ).s;
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
//void
//Reticolo::Sweep ( void ) {
//	/* ckd temporary pointer */
//	bool *c = NULL;
//
//	register unsigned short int j;
//	for ( unsigned short int i = 0; i < L; i ++ ) {
//		c = *( ckd + i );
//
//		for ( j = 0; j < L; j ++ ) {
//			/* if spin had not been visited yet */
//			if ( *( c + j ) == ckd_status )
//				Reticolo::cluster( i, j );
//		}
//	}
//
//	/* auxiliary variable to avoid resetting ckd[][] */
//	ckd_status = !( ckd_status );
//
//	/* update energy */
//	E = Reticolo::energy();
//} /* -----  end of method Reticolo::Sweep  ----- */
//
///*
// * ------------------------------------------------------------------
// *       Class: Reticolo
// *      Method: cluster
// * Description: 
// * ------------------------------------------------------------------
// */
//void
//Reticolo::cluster ( unsigned int i, unsigned int j ) {
//	/* flip spin with 50 % probability */
//	bool flip = (bool) ( rand() % 2 );
//
//
//	/* assign initial cluster site */
//	**stack = i;
//	*( *stack + 1 ) = j;
//
//	/* update 'ckd' */
//	*( *( ckd + i ) + j ) = !*( *( ckd + i ) + j );
//
//	/* firs free element of the stack */
//	tail = 1;
//
////	fprintf( stderr, "\n" );
//
//	/* counter */
//	unsigned short int a;
//	/* indexes */
//	unsigned short int n, m;
//
//	/* temporary pointers */
//	unsigned short int *sptr = NULL;
//	const signed short int *aptr = NULL;
//	bool *cptr = NULL;
//	Sito *xptr = NULL;
//
//	/* temporary magnetization */
////	int magnetization = 0;
//
//	do {
//		/* take/free last stack element */
//		sptr = *( stack + ( -- tail ) );
//		i = *( sptr ); j = *( ++ sptr );
//
//		/* assign (spin) temporary pointer */
//		xptr = *( x + i ) + j;
//
////		fprintf( stdout, "(%u, %u)\n", i, j );
//
//		/* sweep over nearest neighbours */
//		for ( a = 0; a < 4; a ++ ) {
//			/* assign (displacement) temporary pointer */
//			aptr = *( s + a );
//
//			/* assign neighbours coordinates */
//			n = (unsigned) ( (signed) L + i + *( aptr ) ) % L;
//			m = (unsigned) ( (signed) L + j + *( ++ aptr ) ) % L;
//
//			/* assign (check bool) temporary variable */
//			cptr = *( ckd + n ) + m;
//
//			/* if spin does not belong to a cluster */
//			if ( *cptr == ckd_status ) {
//
//				/* if spin are parallel */
//				if ( *( xptr ) == *( *( x + n ) + m ) ) {
//
//					/* check wether activate bond or not */
//					if ( (long double) rand() / RAND_MAX <= EB ) {
//						/* assign (stack) temporary pinter */
//						sptr = *( stack + tail );
//
//						/* push ( n, m ) into stack */
//						*( sptr ) = n; *( ++ sptr ) = m;
//
//						/* TikZ */
////						printf( "\\draw (%u, %u) -- (%u, %u);\n", i, j, n, m );
//
//						/* increase 'tail' */
//						tail ++;
//
//						/* update 'ckd' */
//						*cptr = !( *cptr );
//					}
//				}
//			}
//		}
//
//		/* flip spin */
//		*( xptr ) ^= flip;
//	} while ( tail != head );
//
//} /* -----  end of method Reticolo::cluster  ----- */
//
///*
// * ------------------------------------------------------------------
// *       Class: Reticolo
// *      Method: print_lattice
// * Description: 
// * ------------------------------------------------------------------
// */
//void
//Reticolo::print_lattice ( void ) {
//	FILE *stream = stderr;
//	register signed short int j;
//	for ( unsigned short int i = 0; i < L; i ++ ) {
//		for ( j = L; j > 0; j -- ) {
//			fprintf( stream, "%u ", *( *( x + i ) + j ) );
////			if ( *( *( x + i ) + j ) ) { /* TiKz */
////				fprintf( stderr, "%u/%u, ", i, j );
////			}
//		}
//		fprintf( stream, "\n" );
//	}
//} /* -----  end of method Reticolo::print_lattice  ----- */
//
