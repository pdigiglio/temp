//#include "particella.h"

#include "colors.h"
#include "round.h"

#include "part_libere.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: Sistema
 * Description: [ctor]
 * ------------------------------------------------------------------
 */
Sistema::Sistema ( void ) {
	/* controllo di non aver inserito troppe particelle nella scatola */
	if ( 2 * S > sqrt( D ) / N ) {
		fprintf ( stderr, "[" ANSI_RED "error" ANSI_RESET ": "
				ANSI_YELLOW "%s" ANSI_RESET 
				"] Sphere radius too high!\n"
				" >> Line %u of file '%s'\n",
				__func__, __LINE__, __FILE__ );
		exit (EXIT_FAILURE);

	}

	/* alloco la memoria per le particelle */
	p = (struct ptcl *) malloc( nMax * sizeof( struct ptcl ) );
	if ( p == NULL ) {
		fprintf ( stderr, "[" ANSI_RED "error" ANSI_RESET ": "
				ANSI_YELLOW "%s" ANSI_RESET 
				"] Dynamic memory allocation failed!\n"
				" >> Line %u of file '%s'\n",
				__func__, __LINE__, __FILE__ );
		exit (EXIT_FAILURE);
	}

	/* stampo a schermo informazioni (n. particelle, raggio) */
	fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET "] "
			"Number :: radius of particles (with N = %u) >> %lu :: %1.1g L\n",
			(unsigned) N, (unsigned long) nMax, S );

	/* alloco la memoria per i tempi di collisione */
	ct = (double **) malloc ( nMax * sizeof(double *) );
	if ( ct == NULL ) {
		fprintf ( stderr, "[" ANSI_RED "error" ANSI_RESET ": "
				ANSI_YELLOW "%s" ANSI_RESET 
				"] Dynamic memory allocation failed!\n"
				" >> Line %u of file '%s'\n",
				__func__, __LINE__, __FILE__ );
		exit (EXIT_FAILURE);
	}

	/* inizializzo posizioni delle particelle */
	double x[3], y[3];
	/* energia cinetica */
	register double kinetic;
	/* indice di particella (pari/dispari) */
	register unsigned int q, n = 0, m;
	register unsigned short j;
	/* temporary pointers */
	struct ptcl *pn, *pm;
	double **cn, **cm;
//	double **l = list;

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
					/* assegno la coordinata d-esima */
					*( (*pn).x + d ) = *( x + d );
					/* assegno la velocità d-esima */
					*( (*pn).v + d ) = (double) 2 * rand() / RAND_MAX;
					(*pn).t_last = (double) 0;

					/* assegno la coordinata traslata */
					*( (*pm).x + d ) = *( y + d );
					/* assegno la velocità (cambiata di segno) */
					*( (*pm).v + d ) = - *( (*pn).v + d );
					(*pm).t_last = (double) 0;

					/* calcolo l'energia cinetica */
					kinetic += *( (*pn).v + d ) * *( (*pn).v + d );
				}
//				fprintf( stderr, "\n" );

				/* memoria per i tempi di collisione (n = m - 1) */
				*( ct + n ) = (double *) malloc ( n * sizeof(double) );
				*( ct + m ) = (double *) malloc ( m * sizeof(double) );
				if ( ! ( *( ct + n ) && *( ct + n ) ) ) {
					fprintf ( stderr, "[" ANSI_RED "error" ANSI_RESET ": "
							ANSI_YELLOW "%s" ANSI_RESET 
							"] Dynamic memory allocation failed!\n"
							" >> Line %u of file '%s'\n",
							__func__, __LINE__, __FILE__ );
					exit (EXIT_FAILURE);
				}

				/* calcolo i tempi di collisione */
				cn = ct + n;
				cm = ct + m;
				for ( q = 0; q < n; q ++ ) {
					*( *cn + q ) = crash_time( q, n );

					/* assign list pointers */
//					*( l ++ ) = *cn + q;

//					fprintf( stderr, "(%u, %u) %g\n", n, q, *( *( ct + n ) + q ) );
					
					*( *cm + q ) = crash_time( q, m );
//					*( l ++ ) = *cm + q;

//					fprintf( stderr, "(%u, %u) %g\n", m, q, *( *( ct + m ) + q ) );
				}
				*( *cm + q ) = crash_time( q, m);
//				*( l ++ ) = *cm + q;
//				printf( "%zu\t%lu\n", l - list, e );

//				fprintf( stderr, "(%u, %u) %g\n", m, q, *( *( ct + m ) + q ) );

				/* aggiorno l'indice della particella (pari) */
				n += 2;

//				printf( "%g\n%g\n", kinetic, kinetic );
				K += kinetic;
			}
		}
	}

	/* creo l'array delle colonne */
//	unsigned long int v = 0;
//	unsigned long t1 = 9, t2 = 9;
//	while ( v < e ) {
//		c = (unsigned *) realloc( c, ( ++ L ) * sizeof(unsigned) );
//		if ( c == NULL ) {
//			fprintf ( stderr, "[" ANSI_RED "error" ANSI_RESET ": "
//					ANSI_YELLOW "%s" ANSI_RESET 
//					"] Dynamic memory allocation failed!\n"
//					" >> Line %u of file '%s'\n",
//					__func__, __LINE__, __FILE__ );
//			exit (EXIT_FAILURE);
//		}
//
//		*( c + L - 1 ) = v + 1;
//
//		/* aggiorno il termine generale */
//		t1 = t1 * 4;
//		t2 = t2 * 2;
//		v = t1 - t2;
//	}

	fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET
			"] Initial kinetic energy: %g\n", K );

//	exit( EXIT_SUCCESS );
	/* stampa la velocità del centro di massa */
//	Sistema::mass_center_speed();
}/* -----  end of method Sistema::Sistema (ctor)  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: mass_center_speed
 * Description: find mass center system speed. useful for debugging
 * ------------------------------------------------------------------
 */
void
Sistema::mass_center_speed ( void ) {
	/* temporary variables */
	double tmp[3] = {};

	/* evaluate speed sum */
	struct ptcl *ptr;
	for ( unsigned int s = 0; s < Sistema::nMax; s ++ ) {
		ptr = p + s;
		for ( unsigned short int d = 0; d < D; d ++ )
			*( tmp + d ) += *( (*ptr).v + d );
	}

	/* normalize */
	for ( unsigned short int d = 0; d < D; d ++ )
		*( tmp + d ) = (double) *( tmp + d ) / Sistema::nMax;

	fprintf( stderr, "[" ANSI_CYAN "info" ANSI_RESET
			"] Center of mass velocity: (%g, %g, %g)\n", tmp[0], tmp[1], tmp[2] );
} /* -----  end of method Sistema::mass_center_speed  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: netx_crash
 * Description: 
 * ------------------------------------------------------------------
 */
double
Sistema::next_crash ( void ) {
	/* initial values for colliding particles */
	i0 = 1; j0 = 0;
	/* initial value for collision time */
	double minimum = **( ct + 1 );

	/* seek for minimum */
	register unsigned int m;
	double *ptr;
	for ( unsigned int n = 2; n < Sistema::nMax; n ++ ) {
		ptr = *( ct + n );

		for ( m = 0; m < n; m ++ )
			if ( minimum > *( ptr + m ) ) {
				minimum = *( ptr + m );
				i0 = n; j0 = m;
			}
	}



//	fprintf( stderr, "[] Minimo: %g. Particelle %u - %u\n", minimum, i0, j0 );
	return minimum;

//	unsigned long int 

	/* XXX for shell sort */
//	j0 = nMax + 1;
//	for ( i0 = nMax; j0 > i0; i0 -- ) {
//		j0 = (size_t) ( *( ct + i0 ) - *list );
//		printf( "%d, %u, %g\n", i0, j0, **list );
//	}
//
//
//	/* return lower collision time */
//	return ** list;
} /* -----  end of method Sistema::netx_crash  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: evolve
 * Description: 
 * ------------------------------------------------------------------
 */
double
Sistema::evolve ( void ) {
	/* sort collision times */
//	Sistema::shell_sort();

	/* take colliding time */
	double t0 = Sistema::next_crash();
	double t = t0 - tm;

//	fprintf( stderr, "Distanza (1): %g, Sigma: %g\n", Sistema::distance( i0, j0 ), S );


	/* temporary particle pointer */
	struct ptcl *ptr;
	/* evolve particle positions */
	register unsigned short int d;
	for ( unsigned int n = 0; n < nMax; n ++ ) {
		ptr = p + n;
		for ( d = 0; d < D; d ++ ) {
			*( (*ptr).x + d ) += t * *( (*ptr).v + d );
			*( (*ptr).x + d ) -= floorf( *( (*ptr).x + d ) );
		}
	}

//	fprintf( stderr, "Distanza(2): %g, Sigma: %g\n", Sistema::distance( i0, j0 ), S );
//	fprintf( stderr, "(t: %g) - (tm: %g) = %g (%u, %u)\n", t0, tm, t, i0, j0 );
//	
//	for ( d = 0; d < D; d ++ ) {
//		fprintf( stderr, "v_{%u}(%hu) = %g\n", i0, d, *( (*( p + i0)).v + d ) );
//		fprintf( stderr, "v_{%u}(%hu) = %g\n", j0, d, *( (*( p + j0)).v + d ) );
//	}
//	if ( t < 0 ) {
////		fprintf( stderr, "tempo negativo: %g (%u, %u)\n", t, i0, j0 );
//		exit(1);
//	}
	/* update system time */
	tm += t;

	/* collision time for (single) particles */
	ti0 = fabs( tm - (*( p + i0 )).t_last );
	tj0 = fabs( tm - (*( p + j0 )).t_last );
	
	/* update last collision for single particles */
	(*( p + i0 )).t_last += ti0;
	(*( p + j0 )).t_last += tj0;

	/* free path */
	li0 = ti0 * sqrt( Particella::sp( (*(p + i0)).v ) );
	lj0 = tj0 * sqrt( Particella::sp( (*(p + j0)).v ) );

	/* update colliding particles velocities */
	Sistema::exchange();
	/* evaluate new collision times */
	Sistema::update_crash_times( tm );

//	Sistema::mass_center_speed();

	/* returns collision delta time */
	return t;
} /* -----  end of method Sistema::evolve  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: update_crash_times
 * Description: 
 * ------------------------------------------------------------------
 */
void
Sistema::update_crash_times ( double t0 = (double) 0 ) {
	/* temporary pointer */
	double **pt = ct + i0;
	register unsigned int j;

	for ( j = 0; j < i0; j ++ )
		*( *pt + j ) = t0 + Sistema::crash_time( i0, j );

	for ( j = i0 + 1; j < nMax; j ++ )
		*( *( ct + j ) + i0 ) = t0 + Sistema::crash_time( i0, j );

	/* reassign pointer */
	pt = ct + j0;
	for ( j = 0; j < j0; j ++ )
		*( *pt + j ) = t0 + Sistema::crash_time( j0, j );

	for ( j = j0 + 1; j < nMax; j ++ )
		*( *( ct + j ) + j0 ) = t0 + Sistema::crash_time( j0, j );
} /* -----  end of method Sistema::update_crash_times  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: print_x
 * Description: 
 * ------------------------------------------------------------------
 */
void
Sistema::print_x ( void ) {
	/* pointer to first particle structure */
	struct ptcl *ptr = p;

	for ( unsigned int n = 0; n < Sistema::nMax; n ++ ) {
		/* print particle number */
		fprintf( stdout, "%u\t", n );
		/* print coordinate values */
		for ( unsigned short int d = 0; d < D; d ++ )
			fprintf( stdout, "%f\t", *( (*ptr).x + d ) );

		/* break line */
		fprintf( stdout, "\n" );
		/* update pointer */
		ptr ++;
	}
} /* -----  end of method Sistema::print_x  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: crash_time
 * Description: 
 * ------------------------------------------------------------------
 */
double
Sistema::crash_time ( unsigned int i, unsigned int j ) {
	/* temporary variables for vector differences */
	double R_ij[D], v_ij[D];

	/* assign initial values */
	for ( unsigned short int d = 0; d < D; d ++ ) {
		/* positions */
		*( R_ij + d ) = *( (*( p + i )).x + d ) - *( (*( p + j )).x + d );
		/* impose cyclic conditions */
		*( R_ij + d ) -= round( *( R_ij + d ) );
	
		/* velocity components */
		*( v_ij + d ) = *( (*( p + i )).v + d ) - *( (*( p + j )).v + d );
	}

	/* minimum found time (initially infinite) */
	double time = HUGE_VAL;

	/* auxiliary variable: translation for coordinates */
	signed short int t[D];
	double r_ij[D];

	/* cycle over all possible particle image */
	for ( t[0] = -1; t[0] < 2; t[0] ++ ) {
		*r_ij = *R_ij + (double) *t;

		for (  t[2] = -1; t[2] < 2; t[2] ++ ) { /* 3D */
			*( r_ij + 2 ) = *( R_ij + 2 ) + (double) *( t + 2 );

			for ( t[1] = -1; t[1] < 2; t[1] ++ ) {
				*( r_ij + 1 ) = *( R_ij + 1 ) + (double) *( t + 1 );

				/* evaluate collision time */
				double tmp = sp( r_ij, v_ij );
				if ( tmp < (double) 0 ) {
					/* begin evaluating discriminant */
					double delta = tmp * tmp;
					/* take square modulus of velocity difference */
					double v2 = sp( v_ij );
					/* evaluate discriminant */
					delta -= v2 * ( sp( r_ij ) - S * S );

					if ( delta >= (double) 0  ) {
						/* collision time */
						delta = -( tmp + sqrt( delta ) ) / v2;
//						printf( "delta2: %f\n", delta );

						if ( time > delta ) 
							time = delta;
					}
				}
			}
		}
	}

	return time;
} /* -----  end of method Sistema::crash_time  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: mct
 * Description: mean collision time
 * ------------------------------------------------------------------
 */
//void
//Sistema::mct ( void ) {
//	*tau = (double) *tau / crash;
//	*( tau + 1 ) = (double) *( tau + 1 ) / crash;
//	*( tau + 1 ) -= *tau * *tau;
//
//	printf( "%Lf\t%Lf\n", tau[0], sqrtl( tau[1] / crash ) );
//	round( *tau, sqrtl( *( tau + 1 )  / crash ) );
//	printf( "\n" );
////
//	
//	return;
//} /* -----  end of method Sistema::mct  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: exchange
 * Description: exchange colliding particles velocities (i0, j0).
 * ------------------------------------------------------------------
 */
void
Sistema::exchange ( void ) {
	/* auxiliary variables for position and velocity difference */
	double R_ij[D], v_ij[D];
	/* temporary particle pointers */
	struct ptcl *pi = p + i0, *pj = p + j0;

	/* assign initial values */
	for ( unsigned short int d = 0; d < D; d ++ ) {
		/* positions */
		*( R_ij + d ) = *( (*pi).x + d ) - *( (*pj).x + d );
		/* impose cyclic conditions */
		*( R_ij + d ) -= (double) round( *( R_ij + d ) );
	
		/* velocity components */
		*( v_ij + d ) = *( (*pi).v + d ) - *( (*pj).v + d );
	}

	double dv = Particella::sp ( v_ij, R_ij ) / ( S * S );
//	printf( "%f %f\n", S, sqrt( Particella::sp(R_ij) ) );
	
	/* evaluate pression */
	pr += fabs( dv );

	/* exchange velocity components */
	for ( unsigned short int d = 0; d < D; d ++ ) {
//		fprintf( stderr, "(%u) v_%hu = %f\t", i, d, *( (*pi).v + d ));
		
		*( (*pi).v + d ) -= dv * *( R_ij + d );

//		fprintf( stderr, "(%u) v_%hu = %f\t", i, d, *( (*pi).v + d ));
//		fprintf( stderr, "(%u) v_%hu = %f\t", j, d, *( (*pj).v + d ));
		
		*( (*pj).v + d ) += dv * *( R_ij + d );

//		fprintf( stderr, "(%u) v_%hu = %f\n", j, d, *( (*pj).v + d ));
	}
} /* -----  end of method Sistema::exchange  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: reset_time
 * Description: 
 * ------------------------------------------------------------------
 */
void
Sistema::reset_time ( void ) {
	double **ptr;
	register unsigned int j;
	for ( unsigned int i = 1; i < nMax; i ++ ) {
		ptr = ct + i;
		for ( j = 0; j < i; j ++ )
			*( *ptr + j ) -= tm;
	}

	tm = (double) 0;
} /* -----  end of method Sistema::reset_time  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: ~Sistema
 * Description: destructor
 * ------------------------------------------------------------------
 */
Sistema::~Sistema (void) {
	fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET ": "
			ANSI_YELLOW "%s" ANSI_RESET "] destructed.\n", __func__ );
} /* -----  end of method Sistema::~Sistema (dtor)  ----- */
