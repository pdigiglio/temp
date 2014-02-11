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
Ising::Ising (void) {
} /* -----  end of method Ising::Ising (def. ctor)  ----- */

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

	E = energy();
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
	Ms = Mm = (long int) 0;
	M2 = (unsigned long) 0;

	for ( unsigned short int i = 0; i < L; i ++ ) {
		/* assign temporary variable */
		c = *( ckd + i );

		for ( j = 0; j < L; j ++ ) {
			/* if spin had not been visited yet */
			if ( *( c ) == ckd_status )
				M2 += Ising::cluster( i, j );

			/* update pointer */
			c ++;
		}
	}

	/* auxiliary variable to avoid resetting ckd[][] */
	ckd_status = !( ckd_status );

	/* update energy */
	E = Reticolo::energy();
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
} /* -----  end of method Ising::magnetization  ----- */
