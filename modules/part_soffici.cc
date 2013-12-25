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
		*x = (double) i / N;
		/* coordinata traslata */
		*y = *x + .5 / N;

		for ( unsigned short int k = 0; k < N; k ++ ) { /* 3D */
			/* calcolo la coordinata z */
			*( x + 2 ) = (double) k / N;
			*( y + 2 ) = *( x + 2 ) + .5 / N;

			for ( j = 0; j < N; j ++ ) {
				/* coordinata y */
				*( x + 1 ) = (double) j / N;
				*( y + 1 ) = *( x + 1 ) + .5 / N;
				
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
	double r_ij[D];
	register double r, mr = 0., pt;
	/* temporary variable for force */
	register double f;
	/* temporary particle pointers */
	struct ptcl *pi, *pj;

	/* run over all particles */
	register unsigned int j;
	register unsigned short int d;
	/* internal energy */
	register double internal = (double) 0;
	/* kinetic energy, acceleration */
	double kinetic = (double) 0, a;

	for ( unsigned int i = 0; i < Particella::nMax; i ++ ) {
		pi = p + i;

		for ( j = i + 1; j < Particella::nMax; j ++ ) {
			pj = p + j;

			/* evaluate distance */
			for ( d = 0; d < D; d ++ ) {
				/* r_ij = r_j - r_i */
				*( r_ij + d ) = *( (*pj).x + d ) - *( (*pi).x + d );
				
				/* rescale distance */
				*( r_ij + d ) -= (double) round( *( r_ij + d ) );
			}

			/* take distance between i-th and j-th particle */
			r = sqrt( Particella::sp( r_ij ) );
			mr += r;

			/* save internal energy */
			internal += Soft::V( r );

			/* assign force */
			f = Soft::F( r );
			/* update temporary pressure */
			pt -= f * r;
			/* scale force */
			f /= r;
			
			/* assign accelerations */
			for ( d = 0; d < D; d ++ ) {
				*( (*pi).a + d ) -= f * *( r_ij + d );
				*( (*pj).a + d ) += f * *( r_ij + d );
			}
		}

		for ( d = 0; d < D; d ++ ) {
			/* assign temporary variable */
			a = (double) *( (*pi).a + d ) * dt / 2;

			/* evolve */
			*( (*pi).v + d ) += a;

			/* kinetic energy */
			kinetic += *( (*pi).v + d ) * *( (*pi).v + d );
			
			*( (*pi).x + d ) += *( (*pi).v + d ) * dt + a * dt;
			*( (*pi).v + d ) += a;

			/* reset acceleration */
			*( (*pi).a + d ) = (double) 0;
		}
	}

	/* scale kinetic energy */
	kinetic /= (double) 2;

	/* update pressure */
	pr += pt / kinetic;

	/* assign kinetic and internal energies per particle */
	K = kinetic / Particella::nMax;
	U = internal / Particella::nMax;

	return (double) mr / e;
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
						"] Kinetic energy per particle shifted from %g to %g.\n",
						__func__, K, K_new );

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
		double scale = sqrt( D * kT / ( 2 * K ) );

		Soft::scale_velocity( scale );

		double K_new = D * kT / 2;

		fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET ": "
						ANSI_YELLOW "%s" ANSI_RESET
						"] Kinetic energy per particle shifted from %g to %g.\n",
						__func__, K, K_new );

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
	/* rescale velocities */
	register unsigned int i;
	register unsigned short int d;

	struct ptcl *pi;
	/* run over all particles */
	for ( i = 0; i < Particella::nMax; i ++ ) {
		pi = p + i;
	
		for ( d = 0; d < D; d ++ )
			*( (*pi).v + d ) = *( (*pi).v + d ) * scale;
	}
} /* -----  end of method Soft::scale_velocity  ----- */

