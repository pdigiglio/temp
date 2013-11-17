#include "part_libere.h"
#include "colors.h"

#include <stdio.h>
#include <stdlib.h>
//#include <iostream>

#include <math.h>

//#include "TH1F.h"
//#include "TF1.h"

/* serve per settare il titolo degli assi */
//#include "TH1.h"
//#include "TCanvas.h"
//#include "TStyle.h"
//#include "TROOT.h"
//#include "TString.h"
//#include "TGraph.h"
//#include "TGraphErrors.h"

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

	/* inizializzo posizione e velocità delle particelle */
	float x[3], tmp;
	for ( unsigned short int i = 0; i < N; i ++ ) {
		x[0] = (float) i / N;
		/* coordinata z */
		for ( unsigned short int k = 0; k < N; k ++ ) {
			x[2] = (float) k / N;
			/* coordinata y */
			for ( unsigned short int j = 0; j < N; j ++ ) {
				x[1] = (float) j / N;
			
				tmp = 0;
				/* variabile ausiliaria per l'indice di particella */
				unsigned int idx = i + j + k;
				for ( unsigned short int d = 0; d < D; d ++ ) {
					/* assegno la coordinata d-esima */
					*( (*( p + idx )).x + d ) = *( x + d );
					/* assegno la velocità d-esima */
					*( (*( p + idx )).v + d ) = (float) 2 * rand() / RAND_MAX - 1;

					/* assegno la coordinata traslata */
					*( (*( p + idx + nMax / 2 )).x + d ) = *( x + d ) + (float) 1 / ( 2 * N );
					/* assegno la velocità (cambiata di segno) */
					*( (*( p + idx + nMax / 2 )).v + d ) = - *( (*( p + idx )).v + d );

					/* calcolo l'energia cinetica */
					tmp += *( (*( p + idx )).v + d ) * *( (*( p + idx )).v + d );
				}

//				(*histo).Fill( tmp );
				K += tmp;
			}
		}
	}

	fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET
			"] Initial kinetic energy: %g\n", K );

	/* alloco la memoria per i tempi di collisione */
	ct = (float **) malloc ( ( nMax - 1 ) * sizeof(float *) );
	if ( ct == NULL ) {
		fprintf ( stderr, "[" ANSI_RED "error" ANSI_RESET ": "
				ANSI_YELLOW "%s" ANSI_RESET 
				"] Dynamic memory allocation failed!\n"
				" >> Line %u of file '%s'\n",
				__func__, __LINE__, __FILE__ );
		exit (EXIT_FAILURE);
	}
	/* creo la matrice triangolare */
	for ( unsigned int n = 0; n < nMax; n ++ ) {
		*( ct + n ) = (float *) malloc ( ( nMax - n ) * sizeof(float) );
		if ( *( ct + n ) == NULL ) {
			fprintf ( stderr, "[" ANSI_RED "error" ANSI_RESET ": "
					ANSI_YELLOW "%s" ANSI_RESET 
					"] Dynamic memory allocation failed!\n"
					" >> Line %u of file '%s'\n",
					__func__, __LINE__, __FILE__ );
			exit (EXIT_FAILURE);
		}
	}

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
	for ( unsigned int s = 0; s < nMax / 2; s ++ ) {
//		printf( "%hu\t", s );
		for ( unsigned short int d = 0; d < D; d ++ ) {
//			printf( "%hu\t%f\t", d, *( (*( p + s )).v + d ) );
			tmp[d] += *( (*( p + s )).v + d );
			/* aggiungo questo per evitare che la variabile si saturi */
			tmp[d] += *( (*( p + s + nMax / 2 )).v + d );
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

