/*
 * ==================================================================
 *
 *       Filename:  potts.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/02/2014 16:29:57
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  P. Di Giglio (), p.digiglio91@gmail.com
 *   Organization:  
 *
 * ==================================================================
 */

#include "reticolo.h"
#include "potts.h"

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

/*
 * ------------------------------------------------------------------
 *       Class: Potts
 *      Method: single_E
 * Description: 
 * ------------------------------------------------------------------
 */
signed short int
Potts::single_E ( unsigned int i, unsigned int j ) {
	/* energy, temporary Sito ptr */
	short unsigned int temp = 0;
	Sito *xptr = *( x + i ) + j;

	/* spin in site (i,j) */
	spin s0 = Reticolo::S( *( (*xptr).nn ) );

	/* sweep over nearest neighbours */
	for ( unsigned short int a = 0; a < 4; a ++ ) {
//		printf( "spin: %u ", Reticolo::S( *( (*xptr).nn + a ) ) );
		if ( s0 == Reticolo::S( *( (*xptr).nn + a ) ) )
			temp ++;
	}
//	printf( "\n" );

	return (signed short) - temp;
} /* -----  end of method Potts::single_E  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Potts
 *      Method: delta_E
 * Description: 
 * ------------------------------------------------------------------
 */
signed short int
Potts::delta_E ( const Sito *p, spin s_new ) {
	/* nearest neighbours in state s = 0,...,Q */
	unsigned short int nn[Q] = {};
	
	/* sweep over nearest neighbours */
	for ( unsigned short int a = 0; a < Reticolo::D2; a ++ )
		( *( nn + Reticolo::S( *( (*p).nn + a ) ) ) ) ++;

	return *( nn + (*p).s ) - *( nn + s_new );
} /* -----  end of method Potts::delta_E  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Potts
 *      Method: sweep
 * Description: 
 * ------------------------------------------------------------------
 */
void
Potts::sweep ( void ) {
	/* reset magnetization */
//	Ms = 0;

	register unsigned short int test;
	register spin sn;

	register Sito *xptr = NULL;
	register unsigned short int j;
	for ( unsigned short int i = 0; i < L; i ++ ) {
		/* assign row spin */
		xptr = *( x + i );

		for ( j = 0; j < L; j ++ ) {
			/* new state proposed */
			test = rand() % 2;
			sn = ( (*xptr).s + ( ++ test ) ) % Q;

			/* (maybe) change spin */
			if ( (long double) rand() / RAND_MAX < expl( (long double) - B * Potts::delta_E( xptr, sn ) ) )
				( *xptr ).s = sn;

			/* update magnetization */
//			Ms += ( *xptr ).s;

			/* update (column) Sito index */
			xptr ++;
		}
	}

	E = energy();
} /* -----  end of method Potts::sweep  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Potts
 *      Method: energy
 * Description: 
 * ------------------------------------------------------------------
 */
long int
Potts::energy ( void ) {
	/* energy temporary variable */
	long int ene = 0;

	register short int j;
	/* cycle over even sites only */
	short int i1 = 1;
	for ( unsigned short int i = 0; i < L; i += 2 ) {
		for ( j = 0; j < L; j ++ ) {
			ene += Potts::single_E( i, j );
//			printf( "%u %u %d\n", i, j, Potts::single_E(i, j) );
			/* translater lattice */
			ene += Potts::single_E( i1, ++ j );
//			printf( "%u %u %d\n", i, j, Potts::single_E(i, j) );
		}

		i1 += 2;
	}

	return ene;
} /* -----  end of method Potts::energy  ----- */
