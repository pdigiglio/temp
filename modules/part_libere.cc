#include "part_libere.h"
#include "colors.h"

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
//	/* controllo che le particelle stiano nel volume */
////	Sistema::capacity_check();
//
//	/* scelgo una terna di numeri casuali */
//	unsigned short int j[3];
//	for ( unsigned short int d = 0; d < D; d ++ ) {
//		*( j + d ) = (unsigned short) rand();
////		printf( "%hu\t%hu\n", d, *( j + d ) );
//	}
//
//	/* azzero l'energia cinetica del sistema */
//	K = (float) 0;
//	/* XXX Se 'N' è dispari non funziona! */
//	for ( unsigned short int s = 0; s < (unsigned) N / 2; s ++ ) {
////		printf( "%hu\t", s );
//		for ( unsigned short int d = 0; d < D; d ++ ) {
//			/* assegno la velocità */
//			*( (*( p + s )).v + d ) = (float) 2 * rand() / RAND_MAX - 1;
//			/* variabile ausiliaria */
//			unsigned short int z = N / 2 + ( s + *( j + d ) ) % ( N / 2 );
//			/* assegno la stessa velocità (segno invertito) */
//			*( (*( p + z )).v + d ) = - *( (*( p + s )).v + d );
//
////			printf( "%hu\t%f\t", d, *( (*( p + s )).v + d ) );
//			K += 2 * *( (*( p + s )).v + d ) * *( (*( p + s )).v + d );
//		}
////		printf( "\n" );
//	}
//	
////	utile per il debugging
//	/*
//	printf( "\n");
//	float tmp[3] = {};
//	for ( unsigned short int s = 0; s < N; s ++ ) {
//		printf( "%hu\t", s );
//		for ( unsigned short int d = 0; d < D; d ++ ) {
//			printf( "%hu\t%f\t", d, *( (*( p + s )).v + d ) );
//			tmp[d] += *( (*( p + s )).v + d );
//		}
//		printf( "\n" );
//	}
//	printf( "Velocità totale (cdm): (%f, %f, %f)\n", tmp[0], tmp[1], tmp[2] );
//	*/
}/* -----  end of method Sistema::Sistema (ctor)  ----- */

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

