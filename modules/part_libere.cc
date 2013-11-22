#include "part_libere.h"
#include "colors.h"

#include <stdio.h>
#include <stdlib.h>
//#include <iostream>

#include <math.h>

//#include "TH1F.h"
//#include "TF1.h"

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
	p = (struct ptcl *) malloc ( nMax * sizeof( struct ptcl ) );
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

				/* memoria per i tempi di collisione */
				if ( n ) {
					*( ct + n  - 1 ) = (float *) malloc ( n * sizeof(float) );
//					printf( "i: %u\tlen: %u\n", n - 1, n );
					if ( *( ct + n - 1 ) == NULL ) {
						fprintf ( stderr, "[" ANSI_RED "error" ANSI_RESET ": "
								ANSI_YELLOW "%s" ANSI_RESET 
								"] Dynamic memory allocation failed!\n"
								" >> Line %u of file '%s'\n",
								__func__, __LINE__, __FILE__ );
						exit (EXIT_FAILURE);
					}
				}			
				
				/* aggiorno l'indice della particella (dispari) */
				m = n + 1;
				/* memoria per i tempi di collisione (n = m - 1) */
				*( ct + n ) = (float *) malloc ( m * sizeof(float) );
//				printf( "i: %u\tlen: %u\n", n, m );
				if ( *( ct + n ) == NULL ) {
					fprintf ( stderr, "[" ANSI_RED "error" ANSI_RESET ": "
							ANSI_YELLOW "%s" ANSI_RESET 
							"] Dynamic memory allocation failed!\n"
							" >> Line %u of file '%s'\n",
							__func__, __LINE__, __FILE__ );
					exit (EXIT_FAILURE);
				}

				/* azzero l'energia cinetica delle particella */
				kinetic = (float) 0;

				/* assegno le posizioni e le velocità */
				for ( unsigned short int d = 0; d < D; d ++ ) {
					/* assegno la coordinata d-esima */
					*( (*( p + n )).x + d ) = *( x + d );
//					fprintf( stderr, "%g\t", *( (*( p + n )).x + d ) );
					/* assegno la velocità d-esima */
					*( (*( p + n )).v + d ) = (float) 2 * rand() / RAND_MAX - 1;

					/* assegno la coordinata traslata */
					*( (*( p + m )).x + d ) = *( y + d );
					/* assegno la velocità (cambiata di segno) */
					*( (*( p + m )).v + d ) = - *( (*( p + n )).v + d );

					/* calcolo l'energia cinetica */
					kinetic += *( (*( p + n )).v + d ) * *( (*( p + n )).v + d );
				}
//				fprintf( stderr, "\n" );

				/* aggiorno l'indice della particella (pari) */
				n += 2;

//				printf( "Kinetic: %g\n", kinetic );
//				(*histo).Fill( tmp );
				K += kinetic;
			}
		}
	}

	fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET
			"] Initial kinetic energy: %g\n", K );

	printf( "%f\n", crash_time( 3, 6 ) );
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
			"] Center of mass velocity: (%f, %f, %f)\n", tmp[0], tmp[1], tmp[2] );
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
 *      Method: crash_time
 * Description: 
 * ------------------------------------------------------------------
 */
float
Sistema::crash_time ( unsigned int i, unsigned int j ) {
	/* temporary variables for vector differences */
	float r_ij[D], v_ij[D];
	/* minimum found time */
	float time = NAN;

	/* auxiliary variable: translation for coordinates */
	float t[D];
	/* cycle over all possible particle image */
	for ( *t = -1; *t < 2; *t += 1 )
		for ( *( t + 2 ) = -1; *( t + 2 ) < 2; *( t + 2 ) += 1 )
			for ( *( t + 1 ) = -1; *( t + 1 ) < 2; *( t + 1 ) += 1 ) {

				/* assign vector differences */
				for ( unsigned short int d = 0; d < D; d ++ ) {
					*( r_ij + d ) = *( (*( p + i )).x + d ) - *( (*( p + j )).x + d ) + *( t + d );
					*( v_ij + d ) = *( (*( p + i )).v + d ) - *( (*( p + j )).v + d );
//					fprintf( stderr, "r_ij:%g\tv_ij: %g\n", *( r_ij + d ), *( v_ij + d ) );
				}

				/* evaluate collision time */
				float tmp = sp( r_ij, v_ij );
				if ( tmp < (float) 0 ) {
					/* begin evaluating discriminant */
					float delta = tmp * tmp;
					/* take square modulus of velocity difference */
					float v2 = sp( v_ij );
					/* evaluate discriminant */
					delta -= v2 * ( sp( r_ij ) - S * S );

					if ( delta > (float) 0  ) {
						/* collision time */
						delta = -( tmp - sqrt( delta ) ) / v2;
//						printf( "delta2: %f\n", delta );

						if ( isnan( time ) || time > delta ) 
							time = delta;
					}
				}
			}

	return time;
} /* -----  end of method Sistema::crash_time  ----- */

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
 *      Method: ~Sistema
 * Description: destructor
 * ------------------------------------------------------------------
 */
Sistema::~Sistema (void) {
} /* -----  end of method Sistema::~Sistema (dtor)  ----- */

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

