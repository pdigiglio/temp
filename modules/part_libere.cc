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

//	TH1 *histo = new TH1("histogram", "Scalar speed distribution", 100, -1. , 1.);

	/* alloco la memoria per i tempi di collisione */
	ct = (float **) malloc ( nMax * sizeof(float *) );
	if ( ct == NULL ) {
		fprintf ( stderr, "[" ANSI_RED "error" ANSI_RESET ": "
				ANSI_YELLOW "%s" ANSI_RESET 
				"] Dynamic memory allocation failed!\n"
				" >> Line %u of file '%s'\n",
				__func__, __LINE__, __FILE__ );
		exit (EXIT_FAILURE);
	}

	/* inizializzo posizioni delle particelle */
	float x[3], y[3];
	/* energia cinetica */
	register float kinetic;
	/* indice di particella (pari/dispari) */
	unsigned int n = 0, m;
	/* temporary pointers */
	struct ptcl *pn, *pm;

	for ( unsigned short int i = 0; i < N; i ++ ) {
		/* calcolo la coordinata x */
		*x = (float) i / N;
		/* coordinata traslata */
		*y = *x + .5 / N;

		for ( unsigned short int k = 0; k < N; k ++ ) {
			/* calcolo la coordinata z */
			*( x + 2 ) = (float) k / N;
			*( y + 2 ) = *( x + 2 ) + .5 / N;

			for ( register unsigned short int j = 0; j < N; j ++ ) {
				/* coordinata y */
				*( x + 1 ) = (float) j / N;
				*( y + 1 ) = *( x + 1 ) + .5 / N;
				
				/* aggiorno l'indice della particella (dispari) */
				m = n + 1;
				/* azzero l'energia cinetica delle particella */
				kinetic = (float) 0;
				pn = p + n;
				pm = p + m;
				/* assegno le posizioni e le velocità */
				for ( unsigned short int d = 0; d < D; d ++ ) {
					/* assegno la coordinata d-esima */
					*( (*pn).x + d ) = *( x + d );
					/* assegno la velocità d-esima */
					*( (*pn).v + d ) = (float) 2 * rand() / RAND_MAX;

					/* assegno la coordinata traslata */
					*( (*pm).x + d ) = *( y + d );
					/* assegno la velocità (cambiata di segno) */
					*( (*pm).v + d ) = - *( (*pn).v + d );

					/* calcolo l'energia cinetica */
					kinetic += *( (*pn).v + d ) * *( (*pn).v + d );
				}
//				fprintf( stderr, "\n" );

				/* memoria per i tempi di collisione (n = m - 1) */
				*( ct + n ) = (float *) malloc ( n * sizeof(float) );
				*( ct + m ) = (float *) malloc ( m * sizeof(float) );
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
				for ( q = 0; q < n; q ++ ) {
					*( *( ct + n ) + q ) = crash_time( q, n);
//					fprintf( stderr, "(%u, %u) %g\n", n, q, *( *( ct + n ) + q ) );
					*( *( ct + m ) + q ) = crash_time( q, m);
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
	float tmp[3] = {};
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
			"] Center of mass velocity: (%1.6g, %1.6g, %1.6g)\n", tmp[0], tmp[1], tmp[2] );
} /* -----  end of method Sistema::mass_center_speed  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: capacity_check
 * Description: Checks if particles can be contained in volume. The 
 * 				function works quite fine if 'S/L' is not too big.
 * ------------------------------------------------------------------
 */
void
Sistema::capacity_check ( void ) {
//	/* se il diametro di una sfera è maggiore del lato */
//	if ( (float) L < 2 * S ) {
//		/* stampo l'errore */
//		fprintf( stderr, "[" ANSI_MAGENTA "%s" ANSI_RESET ": "
//				ANSI_RED "Errore" ANSI_RESET "] "
//				"Superata capienza massima sistema: ridurre raggio.\n",
//				__FUNCTION__ );
//		/* termino 'esecuzione */
//		exit(EXIT_FAILURE);
//	}
//
//	unsigned int count = 0;
//	float l = (float) 
////	for ( float x = 0; x + 2 * S < L; x +=  )
////	/* calcolo meglio la capienza */
////	if ( (float) L / l - n ) {
////		/* riduco il numero dei cubi "sfasati" */
////		m --;
////		if ( (float) L / l - n < 2 * S )
////			/* riduco il numero dei cubi "non sfasati" */
////			n --;
////	}
//
//	/* lato minimo dei cubi sui vertici dei quali dispongo le sfere */
//	float l = (float) 4 * S / sqrt(3);	/* controllo che il numero di particelle non ecceda la capienza */
//	
//	nMax = pow( n, 3 ) + pow( m, 3 );
//	if ( (unsigned) N > nMax ) {
//		fprintf( stderr, "[" ANSI_MAGENTA "%s" ANSI_RESET ": "
//				ANSI_RED "Errore" ANSI_RESET "]"
//				" Superata capienza massima sistema: %u / %u.\n",
//				__PRETTY_FUNCTION__, (unsigned) N, (unsigned) nMax );
//		exit(EXIT_FAILURE);
//	}
//
//	/* calcolo il rapporto (intero) tra la capienza e le particelle */
//	step = nMax / N;
//
//	/* stampo alcune informazioni a schermo */
//	fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET "] "
//			"Siti 1: %hu, Siti 2: %hu\n", n, m );
//	fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET "] "
//			"Rapporto capienza (%u)/particelle (%u): %hu\n",
//			(unsigned) nMax, (unsigned) N, step );
} /* -----  end of method Sistema::capacity_check  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: sp
 * Description: Scalar product between two vectors, a and b, in D
 * 				dimensions
 * ------------------------------------------------------------------
 */
float
Sistema::sp ( const float *a, const float *b ) {
	float tmp = (float) 0;
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
float
Sistema::sp ( float *a ) {
	float tmp = (float) 0;
	for ( unsigned short int d = 0; d < D; d ++ )
		tmp += *( a + d ) * *( a + d );

	return tmp;
} /* -----  end of method Sistema::sp  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: netx_crash
 * Description: 
 * ------------------------------------------------------------------
 */
float
Sistema::next_crash ( void ) {
	/* XXX */
	i0 = 1; j0 = 0;
	float minimum = ct[1][0];
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
void
Sistema::evolve ( void ) {
	/* take colliding time */
	float time = next_crash();

//	histo = new TH1F("histogram", "Scalar speed distribution\n(crash)", 100, -1. , 1.);

	/* evolve particle positions */
	float t = time - tm;
//	printf( "\ndt: %f, t %f, dt / t: %f\n", t, time, t / time );
	for ( unsigned int n = 0; n < nMax; n ++ ) {
//		if ( !( crash % ( 2 * nMax ) ) && crash )
//			histo -> Fill( p[n].v[0] );
		for ( unsigned short int d = 0; d < D; d ++ ) {
			p[n].x[d] += t * p[n].v[d];
			p[n].x[d] -= floor( p[n].x[d] );
		}
	}

//	histo->Draw();
//	c->Print( "nome.svg", "svg");

	/* update mean collision time */
	*tau += t;
	*( tau + 1 ) += t * t;

	/* update crash number*/
	crash ++;

	/* update colliding particles velocities */
	Sistema::exchange();
	/* evaluate new collision times */
	Sistema::update_crash_times( time );

	/* update system time */
	tm += t;

//	Sistema::mass_center_speed();
} /* -----  end of method Sistema::evolve  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: get_velocity
 * Description: 
 * ------------------------------------------------------------------
 */
float
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
Sistema::update_crash_times ( float t0 = (float) 0 ) {
//	register unsigned int j;
//	float **pt;
//	for ( unsigned int i = 1; i < nMax; i ++ ) {
//		pt = ct + i;
//		for ( j = 0; j < i; j ++ )
//			*( *pt + j ) = Sistema::crash_time( i, j );
//	}

	float **pt = ct + i0;
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
float
Sistema::crash_time ( unsigned int i, unsigned int j ) {
	/* temporary variables for vector differences */
	float R_ij[D], v_ij[D];
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
	float time = HUGE_VAL;

	/* auxiliary variable: translation for coordinates */
	signed short int t[D];
	float r_ij[D];

	/* cycle over all possible particle image */
	for ( t[0] = -1; t[0] < 2; t[0] ++ ) {
		*r_ij = *R_ij + (float) *t;

		for (  t[2] = -1; t[2] < 2; t[2] ++ ) {
			*( r_ij + 2 ) = *( R_ij + 2 ) + (float) *( t + 2 );

			for ( t[1] = -1; t[1] < 2; t[1] ++ ) {
				*( r_ij + 1 ) = *( R_ij + 1 ) + (float) *( t + 1 );

				/* evaluate collision time */
				float tmp = sp( r_ij, v_ij );
				if ( tmp < (float) 0 ) {
					/* begin evaluating discriminant */
					float delta = tmp * tmp;
					/* take square modulus of velocity difference */
					float v2 = sp( v_ij );
					/* evaluate discriminant */
					delta -= v2 * ( sp( r_ij ) - S * S );

					if ( delta >= (float) 0  ) {
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
	*tau = (float) *tau / crash;
	*( tau + 1 ) = (float) *( tau + 1 ) / crash;
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
 *      Method: exchange
 * Description: 
 * ------------------------------------------------------------------
 */
void
Sistema::exchange ( /* unsigned int i, unsigned int j */ ) {
	unsigned int i = i0, j = j0;

	/* now distance from i0 to j0 is S (\sigma, sphere diameter) */
	float R_ij[D], v_ij[D];
	/* assign initial values */
	for ( unsigned short int d = 0; d < D; d ++ ) {
		/* positions */
		*( R_ij + d ) = *( (*( p + i )).x + d ) - *( (*( p + j )).x + d );
		/* impose cyclic conditions */
		*( R_ij + d ) -= round( *( R_ij + d ) );
	
		/* velocity components */
		*( v_ij + d ) = *( (*( p + i )).v + d ) - *( (*( p + j )).v + d );
	}

	float dv = Sistema::sp ( v_ij, R_ij ) / ( S * S );
	
	/* evaluate pression */
	*press += fabs( dv );
	*( press + 1 ) += dv * dv;

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
	float **ptr;
	register unsigned int j;
	for ( unsigned int i = 1; i < nMax; i ++ ) {
		ptr = ct + i;
		for ( j = 0; j < i; j ++ ) {
			*( *ptr + j ) -= tm;
		}
	}

	tm = (float) 0;
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
 *      Method: pression
 * Description: 
 * ------------------------------------------------------------------
 */
void
Sistema::pression ( void ) {
	*( press ) = *( press ) / tm;
	*( press + 1 ) = sqrt ( *( press + 1 ) / tm - *press * *press );

	for ( unsigned short int k = 0; k < 2; k ++ )
		*( press + k ) = S * *( press + k ) / ( 2. * K );

	press[0] ++;

	printf( "Pressione: %g +- %g\n", press[0], press[1] / sqrt( tm ) );
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

