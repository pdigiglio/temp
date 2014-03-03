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
	/*---------------------------------------------------------------
	 *  INITIALIZE LATTICE
	 *-------------------------------------------------------------*/
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
			( *xptr ).s = Potts::rand_init_val();

			/* assign nearest neighbours */
			for ( a = 0; a < Reticolo::D2; a ++ ) {	
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

	
	/*---------------------------------------------------------------
	 *  INITIALIZE ENERGY AND MAGNETIZATION
	 *-------------------------------------------------------------*/

	/* assign energy */
	E = Potts::energy();
	
	/* temporary pointer */
	long double *mptr = NULL;
	/* temporary angle */
	long double theta_0 = (long double) 2 * M_PI / Q;
	long double theta = (long double) 0;

	/* assign possible values of magnetization */
	for ( unsigned short int q = 0; q < Q; q ++ ) {
		/* assign temporary pointer */
		mptr = *( mag + q );

		/* assign values */
		*mptr = cosl( theta );
		*( ++ mptr ) = sinl ( theta );

		/* update angle */
		theta += (long double) theta_0;
	}

	fprintf( stderr, "[Potts: %s]\n", __func__ );
} /* -----  end of method Potts::Potts (def. ctor)  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Potts
 *      Method: ~Potts
 * Description: destructor
 * ------------------------------------------------------------------
 */
Potts::~Potts (void) {
	fprintf( stderr, "[Potts: %s]\n", __func__ );
} /* -----  end of method Potts::~Potts (dtor)  ----- */

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
	/* reset magnetization */
	*Ms = *( Ms + 1 ) = (long double) 0;

	/* temporary variables for new state */
	register spin sn;

	/* site pointer */
	register Sito *xptr = NULL;
	/* control variable */
	Sito *xstop = NULL;

	for ( unsigned short int i = 0; i < L; i ++ ) {
		/* assign row spin */
		xptr = *( x + i );
		/* assign cycle stop */
		xstop = xptr + L;

		for ( ; xptr != xstop; xptr ++ ) {
			/* new state proposed */
			sn = (*xptr).s + ( rand() % Potts::Q_1 );
			sn = ( ++ sn ) % Q;

			/* (maybe) change spin */
			if ( (long double) rand() / RAND_MAX < expl( (long double) - B * Potts::delta_E( xptr, sn ) ) )
				( *xptr ).s = sn;

			/* update magnetization */
			Potts::update_Ms( xptr );
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

	/* reset magnetization */
	*( Ms ) = *( Ms + 1 ) = (long double) 0;
	M2 = (long double) 0;
	Mm = (long double) 0;

	for ( unsigned short int i = 0; i < L; i ++ ) {
		/* assign temporary variable */
		c = *( ckd + i );

		for ( j = 0; j < L; j ++ ) {
			/* if spin had not been visited yet */
			if ( *( c ) == ckd_status )
				M2 += Potts::cluster( i, j );

			/* update pointer */
			c ++;
		}
	}

	/* auxiliary variable to avoid resetting ckd[][] */
	ckd_status = !( ckd_status );

	/* update energy */
	E = Potts::energy();
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
	bool *cptr = *( ckd + i ) + j;
	*cptr= !( *cptr );

	/* first free element of the stack */
	tail = 1;


	/*---------------------------------------------------------------
	 *  AUXILIARY VARIABLES
	 *-------------------------------------------------------------*/

	/* nearest neighbours counter */
	register unsigned short int a;
	/* position indexes */
	unsigned short int n, m;
	/* cluster size */
	unsigned long int size = (long int) 0;

	/* temporary pointers */
	unsigned short int *sptr = NULL;
	register unsigned short int *nnptr = NULL;
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

		/* sweep over nearest neighbours */
		for ( a = 0; a < Reticolo::D2; a ++ ) {
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
					if ( (long double) rand() / RAND_MAX <= Potts::EB ) {
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
		Potts::update_Ms( xptr );

		/* flip spin */
		( *xptr ).s = flip;
	} while ( tail != head );

	/* update magnetization (max) */
	Mm = ( size > Mm ? size : Mm );

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
			/* translater lattice */
			ene += Potts::single_E( i1, ++ j );
		}

		i1 += 2;
	}

	return ene;
} /* -----  end of method Potts::energy  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Potts
 *      Method: correlator
 * Description: 
 * ------------------------------------------------------------------
 */
void
Potts::correlator ( void ) {
	/* temporary pointer */
	register Sito *xptr = NULL;

	/* temporary variables (mean over rows and columns) */
	double r[L][2] = {}, c[L][2] = {};
	register double *cptr = NULL;
	register long double *mptr = NULL;
	double *rptr = NULL;

	/* sweep all over lattice */
	register unsigned short int j;
	for ( unsigned short int i = 0; i < L; i ++ ) {
		/* reset correlator */
		*( corr + i ) = (double) 0;

		/* assign site pointer */
		xptr = *( x + i );

		/* assign rows array pointer */
		rptr = *( r + i );

		/* evaluate means over columns and rows */
		for ( j = 0; j < L; j ++ ) {
			/* assign columns array pointer */
			cptr = *( c + j );

			/* assign magnetization pointer */
			mptr = *( mag + ( *xptr ).s );
			
			/*-------------------------------------------------------
			 *  UPDATE MEANS
			 *-----------------------------------------------------*/
			
			/* row correlator */
			*rptr += *mptr; // real pt
			/* take (i,j)-th site in += j-th column correlator */ 
			*cptr += *mptr; // real pt

			/* row correlator */
			*( rptr + 1 ) += *( ++ mptr ); // cplx pt
			/* take (i,j)-th site in += j-th column correlator */ 
			*( ++cptr ) += *mptr; // cplx pt

			/* update site ptr */
			xptr ++;
		}
	}

	/* correlator pointer */
	register double *sptr = NULL;

	/* auxiliary pointers */
	register double *ptmp = NULL;
	double *cptr1 = NULL, *rptr1 = NULL;
	
	/* evaluate correlators */
	for ( unsigned short int i = 0; i < L; i ++ ) {
		/* set correlator pointer to t = 0 */
		sptr = corr;

		/* reuse old pointers */
		rptr = *( r + i );
		rptr1 = rptr + 1;

		cptr = *( c + i );
		cptr1 = cptr + 1;

		for ( j = 0; j < L - i; j ++ ) {
			/* rows */
			ptmp = *( r + i + j );

			*sptr += *( rptr ) * *( ptmp );
			*sptr += *( rptr1 ) * *( ++ ptmp );

			/* columns */
			ptmp = *( c + i + j );

			*sptr += *( cptr ) * *( ptmp );
			*sptr += *( cptr1 ) * *( ++ ptmp );

			/* update pointer */
			sptr ++;
		}

		for ( j = 0; j < i; j ++ ) {
			/* rows */
			ptmp = *( r + j );

			*sptr += *( rptr ) * *( ptmp );
			*sptr += *( rptr1 ) * *( ++ ptmp );

			/* columns */
			ptmp = *( c + j );

			*sptr += *( cptr ) * *( ptmp );
			*sptr += *( cptr1 ) * *( ++ ptmp );

			/* update pointer */
			sptr ++;
		}
	}
} /* -----  end of method Potts::correlator  ----- */
