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
} /* -----  end of method Reticolo::Reticolo (ctor)  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Reticolo
 *      Method: ~Reticolo
 * Description: destructor
 * ------------------------------------------------------------------
 */
Reticolo::~Reticolo (void) {
} /* -----  end of method Reticolo::~Reticolo (dtor)  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Reticolo
 *      Method: sweep
 * Description: 
 * ------------------------------------------------------------------
 */
void
Reticolo::sweep ( void ) {
	/* energy difference */
	short int delta;
	/* reset magnetization */
	M = (double) 0;

	register unsigned short int j;
	for ( unsigned short int i = 0; i < L; i ++ )
		for ( j = 0; j < L; j ++ ) {
			if ( rand() % 2 != *( *( x + i ) + j ) ) {
				/* energy difference */
				delta = - 2 * Reticolo::single_E( i, j );

				/* update */
				if ( (long double) rand() / RAND_MAX <= expl( B * delta ) ) {
					/* reverse spin */
					*( *( x + i ) + j ) = ! *( *( x + i ) + j );
					/* update energy */
					E += delta;
				}
			}

			/* update magnetization */
			M += (double) *( *( x + i ) + j );
		}

	/* update mean */
	*m += M;
	*( m + 1 ) += M * M;
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
	/* auxiliary variable to avoid resetting ckd[][] */
	ckd_status = (bool) ( t % 2 );

	register unsigned short int j;
	for ( unsigned short int i = 0; i < L; i ++ ) {
		for ( j = 0; j < L; j ++ ) {

			/* if spin had not been visited yet */
			if ( *( *( ckd + i ) + j ) == ckd_status ) {
				Reticolo::cluster( i, j );
			}

		}
	}

	/* update time (sweep) counter */
	t ++;
} /* -----  end of method Reticolo::Sweep  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Reticolo
 *      Method: cluster
 * Description: 
 * ------------------------------------------------------------------
 */
void
Reticolo::cluster ( unsigned int i, unsigned int j ) {
	/* flip spin with 50 % probability */
	bool flip = (bool) ( rand() % 2 );

	short int n, m;

	/* assign initial cluster site */
	**stack = i;
	*( *stack + 1 ) = j;

	/* update 'ckd' */
	*( *( ckd + i ) + j ) = !*( *( ckd + i ) + j );

	/* firs free element of the stack */
	tail = 1;

	fprintf( stderr, "\n" );

	unsigned short int a;
	do {
		/* free last stack element */
		tail --;
		i = **( stack + tail );
		j = *( *( stack + tail ) + 1 );

//		fprintf( stdout, "(%u, %u)\n", i, j );

		/* sweep over nearest neighbours */
		for ( a = 0; a < 4; a ++ ) {
			/* assign neighbours coordinates */
			n = (unsigned) ( (signed) L + i + *( *( s + a ) ) ) % L;
			m = (unsigned) ( (signed) L + j + *( *( s + a ) + 1 ) ) % L;

			/* if spin does not belong to a cluster */
			if ( *( *( ckd + n ) + m ) == ckd_status ) {

				/* if spin are anti-parallel */
				if ( *( *( x + i ) + j ) ^ *( *( x + n ) + m ) ) {

					/* check wether activate bond or not */
					if ( (long double) rand() / RAND_MAX <= EB ) {
						/* push ( n, m ) into stack */
						**( stack + tail ) = n;
						*( *(stack + tail ) + 1 ) = m;

						/* TikZ */
						printf( "\\draw (%u, %u) -- (%u, %u);\n", i, j, n, m );
						/* increase 'tail' */
						tail ++;

						/* update 'ckd' */
						*( *( ckd + n ) + m ) = !*( *( ckd + n ) + m );
					}
				}
			}
		}

		/* flip spin */
		*( *( x + i ) + j ) ^= flip;
	} while ( tail != head );

} /* -----  end of method Reticolo::cluster  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Reticolo
 *      Method: print_lattice
 * Description: 
 * ------------------------------------------------------------------
 */
void
Reticolo::print_lattice ( void ) {
	register unsigned short int j;
	for ( unsigned short int i = 0; i < L; i ++ ) {
		for ( j = 0; j < L; j ++ ) {
			if ( *( *( x + i ) + j ) ) {
				fprintf( stderr, "%u/%u, ", i, j );
			}
		}
	}
} /* -----  end of method Reticolo::print_lattice  ----- */

