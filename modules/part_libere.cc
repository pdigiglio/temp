#include "part_libere.h"
#include "colors.h"
#include "round.h"

#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
//#include <iostream>

#include <math.h>

#include "TH1F.h"
#include "TF1.h"

/* serve per settare il titolo degli assi */
#include "TH1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TString.h"
#include "TGraph.h"
#include "TGraphErrors.h"

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: Sistema
 * Description: [ctor]
 * ------------------------------------------------------------------
 */
Sistema::Sistema ( void ) {
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
	register unsigned int n = 0, m;
	register unsigned short j;
	/* temporary pointers */
	struct ptcl *pn, *pm;
	double **cn, **cm;

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

					/* assegno la coordinata traslata */
					*( (*pm).x + d ) = *( y + d );
					/* assegno la velocità (cambiata di segno) */
					*( (*pm).v + d ) = - *( (*pn).v + d );

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
				unsigned int q;
				cn = ct + n;
				cm = ct + m;
				for ( q = 0; q < n; q ++ ) {
					*( *cn + q ) = crash_time( q, n );
//					fprintf( stderr, "(%u, %u) %g\n", n, q, *( *( ct + n ) + q ) );
					*( *cm + q ) = crash_time( q, m );
//					fprintf( stderr, "(%u, %u) %g\n", m, q, *( *( ct + m ) + q ) );
				}
				*( *( ct + m ) + q ) = crash_time( q, m);
//				fprintf( stderr, "(%u, %u) %g\n", m, q, *( *( ct + m ) + q ) );

				/* aggiorno l'indice della particella (pari) */
				n += 2;

//				printf( "%g\n%g\n", kinetic, kinetic );
//				(*histo).Fill( tmp );
				K += kinetic;
			}
		}
	}

	fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET
			"] Initial kinetic energy: %g\n", K );

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
	printf( "\n");
	double tmp[3] = {};
	for ( unsigned int s = 0; s < nMax; s ++ ) {
//		printf( "%hu\t", s );
		for ( unsigned short int d = 0; d < D; d ++ ) {
//			printf( "%hu\t%f\t", d, *( (*( p + s )).v + d ) );
			tmp[d] += *( (*( p + s )).v + d );
			/* aggiungo questo per evitare che la variabile si saturi */
//			tmp[d] += *( (*( p + s + nMax / 2 )).v + d );
		}
//		printf( "\n" );
	}
	fprintf( stderr, "[" ANSI_CYAN "info" ANSI_RESET
			"] Center of mass velocity: (%g, %g, %g)\n", tmp[0], tmp[1], tmp[2] );
} /* -----  end of method Sistema::mass_center_speed  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: sp
 * Description: Scalar product between two vectors, a and b, in D
 * 				dimensions
 * ------------------------------------------------------------------
 */
double
Sistema::sp ( const double *a, const double *b ) {
	double tmp = (double) 0;
	for ( unsigned short int d = 0; d < D; d ++ ) {
		tmp += *( a + d ) * *( b + d );
//		printf( "%g * %g ", *( a + d ), *( b + d ) );
	}
//	printf( "\n" );

	return tmp;
} /* -----  end of method Sistema::sp  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: sp
 * Description: The same as above. If only 'a' is given, it returns
 * 				his square modulus
 * ------------------------------------------------------------------
 */
double
Sistema::sp ( const double *a ) {
	double tmp = (double) 0;
	for ( unsigned short int d = 0; d < D; d ++ )
		tmp += *( a + d ) * *( a + d );

	return tmp;
} /* -----  end of method Sistema::sp  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: shell_sort
 * Description: 
 * ------------------------------------------------------------------
 */
void
Sistema::shell_sort ( double **a = list , unsigned long int n = nMax ) {

	/* TODO find a way to evaluate this array */
    static unsigned int c[] = { 4356424, 1355339, 543749, 213331, 84801, 27901,
                    11969, 4711, 1968, 815, 277, 97, 31, 7, 3, 1 };

	/* TODO properly comment this code */
	register unsigned int i, j;
	unsigned int h;

	double *tmp;
    for ( unsigned short int k = 0; k < 16; k ++ ) {
		h = *( c + k );
		for ( i = h; i < n; i ++ ) {
			tmp = *( a + i );

			for ( j = i; j >= h && **( a + j - h ) > *tmp; j -= h )
                *( a + j ) = *( a + j - h );

            *( a + j ) = tmp;
        }
    }
} /* -----  end of method Sistema::shell_sort  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: netx_crash
 * Description: 
 * ------------------------------------------------------------------
 */
double
Sistema::next_crash ( void ) {
	/* XXX */
	i0 = 1; j0 = 0;
	double minimum = ct[1][0];
	for ( unsigned int n = 2; n < nMax; n ++ )
		for ( unsigned int m = 0; m < n; m ++ )
			if ( minimum > ct[n][m] ) {
				minimum = ct[n][m];
				i0 = n; j0 = m;
			}

//	fprintf( stderr, "[] Minimo: %g. Particelle %u - %u\n", minimum, i0, j0 );
	return minimum;
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
	/* take colliding time */
	double t = Sistema::next_crash() - tm;

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

	/* update colliding particles velocities */
	Sistema::exchange();
	/* evaluate new collision times */
	Sistema::update_crash_times( t + tm );

//	Sistema::mass_center_speed();

	/* update system time */
	tm += t;
	/* returns collision delta time */
	return t;
} /* -----  end of method Sistema::evolve  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: get_velocity
 * Description: 
 * ------------------------------------------------------------------
 */
double
Sistema::get_velocity ( unsigned int n, unsigned short int d ) {
	return *( (*( p + n )). v + d );
} /* -----  end of method Sistema::get_velocity  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: update_crash_times
 * Description: 
 * ------------------------------------------------------------------
 */
void
Sistema::update_crash_times ( double t0 = (double) 0 ) {
//	register unsigned int j;
//	double **pt;
//	for ( unsigned int i = 1; i < nMax; i ++ ) {
//		pt = ct + i;
//		for ( j = 0; j < i; j ++ )
//			*( *pt + j ) = Sistema::crash_time( i, j );
//	}

	double **pt = ct + i0;
	register unsigned int j;

	for ( j = 0; j < i0; j ++ )
		*( *pt + j ) = t0 + Sistema::crash_time( i0, j );

	for ( j = i0 + 1; j < nMax; j ++ )
		*( *( ct + j ) + i0 ) = t0 + Sistema::crash_time( i0, j );


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

	for ( unsigned int n = 0; n < nMax; n ++ ) {
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
void
Sistema::mct ( void ) {
	*tau = (double) *tau / crash;
	*( tau + 1 ) = (double) *( tau + 1 ) / crash;
	*( tau + 1 ) -= *tau * *tau;

	printf( "%Lf\t%Lf\n", tau[0], sqrtl( tau[1] / crash ) );
	round( *tau, sqrtl( *( tau + 1 )  / crash ) );
	printf( "\n" );
//
	
	return;
} /* -----  end of method Sistema::mct  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: get_time
 * Description: 
 * ------------------------------------------------------------------
 */
double
Sistema::get_time ( void ) {
	return tm;
} /* -----  end of method Sistema::get_time  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: exchange
 * Description: 
 * ------------------------------------------------------------------
 */
void
Sistema::exchange ( /* unsigned int i, unsigned int j */ ) {
	unsigned int i = i0, j = j0;

	double R_ij[D], v_ij[D];

	/* assign initial values */
	for ( unsigned short int d = 0; d < D; d ++ ) {
		/* positions */
		*( R_ij + d ) = *( (*( p + i )).x + d ) - *( (*( p + j )).x + d );
		/* impose cyclic conditions */
		*( R_ij + d ) -= (double) round( *( R_ij + d ) );
	
		/* velocity components */
		*( v_ij + d ) = *( (*( p + i )).v + d ) - *( (*( p + j )).v + d );
	}

	double dv = Sistema::sp ( v_ij, R_ij ) / ( S * S );
//	printf( "%f %f\n", S, sqrt( Sistema::sp(R_ij) ) );
	
	/* evaluate pression */
	pr += fabs( dv );
//	*( press + 1 ) += dv * dv;

	for ( unsigned short int d = 0; d < D; d ++ ) {
//		fprintf( stderr, "(%u) v_%hu = %f\t", i, d, *( (*( p + i )).v + d ));
		
		*( (*( p + i )).v + d ) -= dv * *( R_ij + d );

//		fprintf( stderr, "(%u) v_%hu = %f\t", i, d, *( (*( p + i )).v + d ));
//		fprintf( stderr, "(%u) v_%hu = %f\t", j, d, *( (*( p + j )).v + d ));
		
		*( (*( p + j )).v + d ) += dv * *( R_ij + d );

//		fprintf( stderr, "(%u) v_%hu = %f\n", j, d, *( (*( p + j )).v + d ));
	}
} /* -----  end of method Sistema::exchange  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: get_pr
 * Description: 
 * ------------------------------------------------------------------
 */
double
Sistema::get_pr ( void ) {
	return pr;
} /* -----  end of method Sistema::get_pr  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: reset_pr
 * Description: 
 * ------------------------------------------------------------------
 */
void
Sistema::reset_pr ( double val ) {
	pr = (double) val;
} /* -----  end of method Sistema::reset_pr  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: Sistema
 * Description: [copy ctor]
 * ------------------------------------------------------------------
 */
//Sistema::Sistema (const Sistema &other) {
//} /* -----  end of method Sistema::Sistema (copy ctor)  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: time_reset
 * Description: 
 * ------------------------------------------------------------------
 */
void
Sistema::time_reset ( void ) {
	double **ptr;
	register unsigned int j;
	for ( unsigned int i = 1; i < nMax; i ++ ) {
		ptr = ct + i;
		for ( j = 0; j < i; j ++ )
			*( *ptr + j ) -= tm;
	}

	tm = (double) 0;
} /* -----  end of method Sistema::time_reset  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: ~Sistema
 * Description: destructor
 * ------------------------------------------------------------------
 */
Sistema::~Sistema (void) {
} /* -----  end of method Sistema::~Sistema (dtor)  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: get_KT
 * Description: 
 * ------------------------------------------------------------------
 */
double
Sistema::get_KT ( void ) {
	return (double) 2 * K / ( D * nMax );
} /* -----  end of method Sistema::get_KT  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: get_K
 * Description: 
 * ------------------------------------------------------------------
 */
double
Sistema::get_K ( void ) {
	return K;
} /* -----  end of method Sistema::get_K  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: pression
 * Description: 
 * ------------------------------------------------------------------
 */
void
Sistema::pression ( void ) {
//	*( press ) = *( press ) / tm;
//	*( press + 1 ) = sqrt ( *( press + 1 ) / tm - *press * *press );
//
//	for ( unsigned short int k = 0; k < 2; k ++ )
//		*( press + k ) = S * *( press + k ) / ( 2. * K );
//
//	press[0] ++;
//
//	printf( "Pressione: %g +- %g\n", press[0], press[1] / sqrt( tm ) );
} /* -----  end of method Sistema::pression  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: operator '='
 * Description: assignment operator
 * ------------------------------------------------------------------
 */
//Sistema&
//Sistema::operator = (const Sistema &other) {
//	if ( this != &other ) {
//	}
//	return *this;
//} /* -----  end of method Sistema::operator =  (ass. op.)  ----- */

