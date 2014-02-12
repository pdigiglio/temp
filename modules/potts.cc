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
 *      Method: Potts
 * Description: [default ctor]
 * ------------------------------------------------------------------
 */
Potts::Potts (void) {
	/* temporary pointer */
	long double *mptr = NULL;
	/* temporary angle */
	long double theta;

	/* assign possible values of magnetization */
	for ( unsigned short int q = 0; q < Q; q ++ ) {
		/* assign temporary pointer */
		mptr = *( mag + q );
		/* assign angle */
		theta = (long double) 2 * M_PI * q / Q;

		/* assign values */
		*mptr = cosl( theta );
		*( ++ mptr ) = sinl ( theta );
	}
} /* -----  end of method Potts::Potts (def. ctor)  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Potts
 *      Method: single_E
 * Description: 
 * ------------------------------------------------------------------
 */
signed short int
Potts::single_E ( unsigned int i, unsigned int j ) {
//	fprintf ( stderr, "ene\n" );
	/* energy, temporary Sito ptr */
	short unsigned int temp = 0;
	Sito *xptr = *( x + i ) + j;

	/* spin in site (i,j) */
	spin s0 = (*xptr).s;

	/* sweep over nearest neighbours */
	for ( unsigned short int a = 0; a < Reticolo::D2; a ++ )
		if ( s0 == Reticolo::S( *( (*xptr).nn + a ) ) )
			temp ++;

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
Potts::delta_E ( const Sito *xptr, spin s_new ) {
	/* nearest neighbours in state s = 0,...,Q */
	unsigned short int nn[Q] = {};
	
	/* sweep over nearest neighbours */
	for ( unsigned short int a = 0; a < Reticolo::D2; a ++ )
		( *( nn + Reticolo::S( *( (*xptr).nn + a ) ) ) ) ++;

	return *( nn + (*xptr).s ) - *( nn + s_new );
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
 *      Method: Sweep
 * Description: Update using Swendsen-Wang multi-cluster algorithm
 * ------------------------------------------------------------------
 */
void
Potts::Sweep ( void ) {
	/* ckd temporary pointer */
	register bool *c = NULL;
	register unsigned short int j;

	/* reset magnetization (sweep, max) */
//	Ms = Mm = (long int) 0;
//	M2 = (unsigned long) 0;

	for ( unsigned short int i = 0; i < L; i ++ ) {
		/* assign temporary variable */
		c = *( ckd + i );

		for ( j = 0; j < L; j ++ ) {
			/* if spin had not been visited yet */
			if ( *( c ) == ckd_status )
				Potts::cluster( i, j );

			/* update pointer */
			c ++;
		}
	}

	/* auxiliary variable to avoid resetting ckd[][] */
	ckd_status = !( ckd_status );

	/* update energy */
	E = Potts::energy();
	/* uncomment this if you dont't update it in 'cluster' method */
//	Ms = Ising::magnetization();
} /* -----  end of method Potts::Sweep  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Potts
 *      Method: cluster
 * Description: 
 * ------------------------------------------------------------------
 */
unsigned long int
Potts::cluster ( unsigned int i, unsigned int j ) {
	/* new spin value (flip = { 0,1,...,Q }) */
	spin flip = (spin) ( rand() % Q );


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

	/* nearest neighbours counter */
	unsigned short int a;
	/* position indexes */
	unsigned short int n, m;
	/* cluster size */
	unsigned long int size = (long int) 0;

	/* temporary pointers */
	unsigned short int *sptr = NULL;
	unsigned short int *nnptr = NULL;
	long double *magptr = NULL;
	bool *cptr = NULL;
	Sito *xptr = NULL;

	/*---------------------------------------------------------------
	 *  CREATE CLUSTER
	 *-------------------------------------------------------------*/

	do {
		/* take/free last stack element */
		sptr = *( stack + ( -- tail ) );
		i = *( sptr ); j = *( ++ sptr );

		/* assign (spin) temporary pointer */
		xptr = *( x + i ) + j;

		/* assign temporary magnetization pointer */
		magptr = *( mag + (*xptr).s );

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
		*( Ms ) += *magptr;
		*( Ms + 1 ) += *( ++ magptr );

		/* flip spin */
		( *xptr ).s = flip;
	} while ( tail != head );

	/* update magnetization (max) */
//	Mm = ( size > Mm ? size : Mm );

	return size * size;
} /* -----  end of method Potts::cluster  ----- */

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
