#include "colors.h"
#include "round.h"

#include "part_soffici.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <string.h>
#include <errno.h>


/*
 * ------------------------------------------------------------------
 *       Class: Soft
 *      Method: Soft
 * Description: [ctor]
 * ------------------------------------------------------------------
 */
Soft::Soft (void) {
	/* stampo a schermo informazioni (n. particelle, raggio) */
	fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET "] "
			"Number :: radius of particles (with N = %u) >> %lu :: %.2g sigma\n",
			(unsigned) N, (unsigned long) Particella::nMax, (double) Soft::L );
	
	/* inizializzo posizioni delle particelle */
	double x[3], y[3];

	/* energia cinetica */
	register double kinetic;

	/* indice di particella (pari/dispari) */
	register unsigned int n = 0, m;
	register unsigned short j;
	
	/* temporary pointers */
	struct ptcl *pn, *pm;

	for ( unsigned short int i = 0; i < N; i ++ ) {
		/* calcolo la coordinata x */
		*x = (double) Soft::L * i / N;
		/* coordinata traslata */
		*y = *x + Soft::L * .5 / N;

		for ( unsigned short int k = 0; k < N; k ++ ) { /* 3D */
			/* calcolo la coordinata z */
			*( x + 2 ) = (double) Soft::L * k / N;
			*( y + 2 ) = *( x + 2 ) + Soft::L * .5 / N;

			for ( j = 0; j < N; j ++ ) {
				/* coordinata y */
				*( x + 1 ) = (double) Soft::L * j / N;
				*( y + 1 ) = *( x + 1 ) + Soft::L * .5 / N;
				
				/* aggiorno l'indice della particella (dispari) */
				m = n + 1;
				/* azzero l'energia cinetica delle particella */
				kinetic = (double) 0;

				pn = p + n;
				pm = p + m;
				
				/* assegno le posizioni e le velocità */
				for ( unsigned short int d = 0; d < D; d ++ ) {
					/* assegno la velocità d-esima */
					*( (*pn).v + d ) = (double) 2 * rand() / RAND_MAX;
					/* assegno la coordinata d-esima */
					*( (*pn).x + d ) = *( x + d );
					/* evolvo la coordinata */
					*( (*pn).x + d ) += *( (*pn).v + d ) * dt;
					/* accelerazione */
					*( (*pn).a + d ) = (double) 0;
					(*pn).t_last = (double) 0;

					/* assegno la velocità (cambiata di segno) */
					*( (*pm).v + d ) = - *( (*pn).v + d );
					/* assegno la coordinata traslata */
					*( (*pm).x + d ) = *( y + d );
					/* evolvo la coordinata */
					*( (*pm).x + d ) += *( (*pm).v + d ) * dt;
					*( (*pm).a + d ) = (double) 0;
					(*pm).t_last = (double) 0;

					/* calcolo l'energia cinetica */
					kinetic += *( (*pn).v + d ) * *( (*pn).v + d );
				}
//				fprintf( stderr, "\n" );

				/* aggiorno l'indice della particella (pari) */
				n += 2;

//				printf( "%g\n%g\n", kinetic, kinetic );
				K += kinetic;
			}
		}
	}

	fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET ": "
					ANSI_YELLOW "%s" ANSI_RESET "] constructed.\n", __func__ );

	fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET
			"] Initial kinetic energy: %g\n", K );

//	exit( EXIT_SUCCESS );
	/* stampa la velocità del centro di massa */
//	Sistema::mass_center_speed();
} /* -----  end of method Soft::Soft (ctor)  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Soft
 *      Method: evolve
 * Description: 
 * ------------------------------------------------------------------
 */
double
Soft::evolve ( void ) {
	/* vector radius connecting particles, modulus, mean modulus */
	double r_ij[D], rtemp, rmod, rmean = (double) 0;
	double *rptr = NULL;

	/* temporary variable for force, pressure (temporary) */
	double f, pt = (double) 0;

	/* temporary particle pointers */
	struct ptcl *pi = NULL, *pstop = p + Particella::nMax;
	register struct ptcl *pj = NULL;
	register double *tmp1 = NULL, *tmp2 = NULL;
	double *ri = NULL;

	/* internal energy */
	register double internal = (double) 0;

	/* kinetic energy, acceleration */
	double kinetic = (double) 0, a, *pivd = NULL;

	for ( unsigned int i = 0; i < Particella::nMax; i ++ ) {
		pi = p + i;
		ri = *( r + i );

		for ( pj = pi + 1; pj != pstop; pj ++ ) {
			/* assign temporary pointers */
			tmp1 = ( *pj ).x;
			tmp2 = ( *pi ).x;

			/* evaluate distance */
			for ( rptr = r_ij; rptr != r_ij + D; rptr ++ ) {
				/* r_ij = r_j - r_i */
				*( rptr ) = *( tmp1 ++ ) - *( tmp2 ++ );
				
				/* rescale distance within the box */
				*( rptr ) -= (double) Soft::L * round( *( rptr ) / Soft::L );
			}

			/* take distance between i-th and j-th particle */
			rmod = sqrt( Particella::sp( r_ij ) );
			rmean += rmod;

			/* save internal energy */
			internal += Soft::V( rmod );

			/* assign force */
			f = Soft::F( rmod );
			/* update temporary pressure */
			pt += f * rmod;
			/* scale force */
			f /= rmod;

			/* re-assign temporary pointers */
			tmp1 = ( *pj ).a;
			tmp2 = ( *pi ).a;
			
			/* assign accelerations */
			for ( rptr = r_ij; rptr != r_ij + D; rptr ++ ) {
				*( tmp2 ++ ) -= f * *( rptr );
				*( tmp1 ++ ) += f * *( rptr );
			}
		}

		/* re-assign temporary pointers */
		tmp1 = ( *pi ).a;
		tmp2 = ( *pi ).x;

		for ( unsigned short int d = 0; d < D; d ++ ) {
			/* assign temporary variables */
			a = (double) *( tmp1 ) * dt / 2;
			pivd = (*pi).v + d;

			/* evolve */
			*pivd += a;

			/* kinetic energy */
			kinetic += *( pivd ) * *( pivd );
			
			/* move particle */
			*( tmp2 ) += *( pivd ) * dt + a * dt;

			/* evaluate mean square modulus */
			rtemp = *( tmp2 ++ ) - *( ri + d );
			rtemp -= (double) Soft::L * round( rtemp / Soft::L );
			R2M += rtemp * rtemp;

			*pivd += a;

			/* reset acceleration */
			*( tmp1 ++ ) = (double) 0;
		}
	}

	/* update pressure */
	pr += pt / kinetic;

	/* assign kinetic and internal energies per particle */
	K = (double) kinetic / ( 2 * Particella::nMax );
	U = (double) internal / Particella::nMax;

	/* update system time */
	tm += dt;

	return (double) rmean / e;
} /* -----  end of method Soft::evolve  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Soft
 *      Method: evolve_euler
 * Description: 
 * ------------------------------------------------------------------
 */
void
Soft::evolve_euler ( void ) {
	/* vector radius connecting particles, modulus */
	double r_ij[D], r;
	/* temporary variable for force and acceleration */
	double f;
	/* temporary particle pointers */
	struct ptcl *pi, *pj;

	/* run over all particles */
	register unsigned int j;
	register unsigned short int d;
	/* kinetic energy */
	register double internal = (double) 0, kinetic = (double) 0;

	for ( unsigned int i = 0; i < Particella::nMax; i ++ ) {
		pi = p + i;

		for ( j = i + 1; j < Particella::nMax; j ++ ) {
			pj = p + j;

			/* evaluate distance */
			for ( d = 0; d < D; d ++ ) {
				/* r_ij = r_j - r_i */
//				fprintf( stderr, "%u: %g\t%u: %g\n",i, *( (*pi).x + d ), j, *( (*pj).x + d ) );
				*( r_ij + d ) = *( (*pj).x + d ) - *( (*pi).x + d );
				
				/* rescale distance */
				*( r_ij + d ) -= (double) round( *( r_ij + d ) );
//				fprintf( stderr, "%u - %u: %g\n", i, j, *( r_ij + d ));
			}

			/* take distance between i-th and j-th particle */
			r = sqrt( Particella::sp( r_ij ) );

			/* save internal energy */
			internal += Soft::V( r );

			/* assign force */
			f = Soft::F( r ) / r;
			
			/* assign accelerations */
			for ( d = 0; d < D; d ++ ) {
				
//				fprintf( stderr, "acc(%u): %g\n", i, - f * *( r_ij + d ));

				*( (*pi).a + d ) += f * *( r_ij + d );
				*( (*pj).a + d ) -= f * *( r_ij + d );
			}
			
//			fprintf( stderr, "acc(%u): %g\n", i, *( (*pi).a + 0 )  );
		}

		for ( d = 0; d < D; d ++ ) {
			/* kinetic energy */
			kinetic += *( (*pi).v + d ) * *( (*pi).v + d );
			
			*( (*pi).x + d ) += *( (*pi).v + d ) * dt;
			*( (*pi).v + d ) += *( (*pi).a + d ) * dt;

			/* reset acceleration */
			*( (*pi).a + d ) = (double) 0;
		}
	}

	K = .5 * kinetic / Particella::nMax;
	U = internal / Particella::nMax;

	printf( "%g\t%g\n", K, U );
} /* -----  end of method Soft::evolve_euler  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Soft
 *      Method: set_energy
 * Description: Rescales kinetic energy. Current kinetic and internal 
 * 				energies per particle are supposed to be known.
 * ------------------------------------------------------------------
 */
void
Soft::set_energy ( double ene ) {
	double K_new = ene - U;

	/* check if 'k_new' is non-negative */
	if ( K_new >= (double) 0 ) {
		double scale = sqrt( K_new / K );

		Soft::scale_velocity( scale );

		fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET ": "
						ANSI_YELLOW "%s" ANSI_RESET
						"] Energy per particle shifted from %g to %g.\n",
						__func__, Particella::K + Particella::U, ene );

		/* reassign kinetic energy */
		K = K_new;
	} else {
		fprintf( stderr, "[" ANSI_RED "warning" ANSI_RESET ": "
						ANSI_YELLOW "%s" ANSI_RESET
						"] New kinetic per particle is negative: try "
						"raising energy %g to >= %g.\n",
						__func__, ene, U );

		exit( EXIT_FAILURE );
	}
} /* -----  end of method Soft::set_energy  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Soft
 *      Method: set_kT
 * Description: 
 * ------------------------------------------------------------------
 */
void
Soft::set_kT ( double kT ) {
	/* check if new 'kT' is greater than 0 */
	if ( kT >= (double) 0 ) {
		double K_new = D * kT / 2;
		double scale = sqrt( K_new / Particella::K );

		Soft::scale_velocity( scale );

		fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET ": "
						ANSI_YELLOW "%s" ANSI_RESET
						"] KT shifted from %g to %g.\n",
						__func__, (double) 2 * Particella::K / D, kT );

		/* reassign kinetic energy */
		K = K_new;
	} else {
		fprintf( stderr, "[" ANSI_RED "warning" ANSI_RESET ": "
						ANSI_YELLOW "%s" ANSI_RESET
						"] New kT is negative!\n", __func__ );

		exit( EXIT_FAILURE );
	}

} /* -----  end of method Soft::set_kT  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Soft
 *      Method: scale_velocity
 * Description: 
 * ------------------------------------------------------------------
 */
void
Soft::scale_velocity ( double scale ) {
	/* temporary pointers */
	register double *vptr = NULL;
	double *vstop = NULL;

	/* run over all particles */
	register unsigned int i;
	for ( i = 0; i < Particella::nMax; i ++ ) {
		/* assign control cycle variables */
		vptr = ( *( p + i ) ).v;
		vstop = vptr + D;
	
		for ( ; vptr != vstop; vptr ++ )
			*( vptr ) *= scale;
	}
} /* -----  end of method Soft::scale_velocity  ----- */

