#include "part_libere.h"

#include <math.h>

/* Output text colors (i.e. for error messages) */
#define ANSI_RED		"\x1b[31m"
#define ANSI_GREEN		"\x1b[32m"
#define ANSI_YELLOW		"\x1b[33m"
#define ANSI_BLUE		"\x1b[34m"
#define ANSI_MAGENTA	"\x1b[35m"
#define ANSI_CYAN		"\x1b[36m"
#define ANSI_RESET		"\x1b[0m"
#define ANSI_BOLD		"\033[1m"

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: Sistema
 * Description: [ctor]
 * ------------------------------------------------------------------
 */
Sistema::Sistema (void) {
	/* lato minimo dei cubi sui vertici dei quali dispongo le sfere */
	float l = 4 * S / sqrt(3);
	/* calcolo il massimo numero di particelle che il cubo contiene */
	unsigned short int n = (unsigned short) L / l, m = n;
	if ( (float) L / l - n ) {
		/* riduco il numero dei cubi "sfasati" */
		m --;
		if ( (float) L / l - n < 2 * S )
			/* riduco il numero dei cubi "non sfasati" */
			n --;
	}

	/* controllo che il numero di particelle non ecceda la capienza */
	unsigned int nMax = pow( n, 3 ) + pow( m, 3 );
	if ( (unsigned) N > nMax ) {
		fprintf( stderr, "[" ANSI_RED "Fatale" ANSI_RESET "]"
				" Superata capienza massima sistema: %u / %u\n",
				(unsigned) N, (unsigned) nMax );
		exit(EXIT_FAILURE);
	}

	/* stampo alcune informazioni a schermo */
	fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET "] "
			"Siti 1: %hu, Siti 2: %hu\n", n, m );
	/* calcolo il rapporto (intero) tra la capienza e le particelle */
	unsigned short step = nMax / N;
	fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET "] "
			"Rapporto capienza (%u)/particelle (%u): %hu\n",
			(unsigned) nMax, (unsigned) N, step );

	/*---------------------------------------------------------------
	 *  Assegno posizioni e velocità
	 *-------------------------------------------------------------*/

	/* scelgo una terna di numeri casuali */
	unsigned short int j[3];
	for ( unsigned short int d = 0; d < D; d ++ ) {
		*( j + d ) = (unsigned short) rand();
//		printf( "%hu\t%hu\n", d, *( j + d ) );
	}

	/* azzero l'energia cinetica del sistema */
	K = (float) 0;
	/* XXX Se 'N' è dispari non funziona! */
	for ( unsigned short int s = 0; s < (unsigned) N / 2; s ++ ) {
		printf( "%hu\t", s );
		for ( unsigned short int d = 0; d < D; d ++ ) {
			/* assegno la velocità */
			*( (*( p + s )).v + d ) = (float) 2 * rand() / RAND_MAX - 1;
			/* variabile ausiliaria */
			unsigned short int z = N / 2 + ( s + *( j + d ) ) % ( N / 2 );
			/* assegno la stessa velocità (segno invertito) */
			*( (*( p + z )).v + d ) = - *( (*( p + s )).v + d );

			printf( "%hu\t%f\t", d, *( (*( p + s )).v + d ) );
			K += 2 * *( (*( p + s )).v + d ) * *( (*( p + s )).v + d );
		}
		printf( "\n" );
	}

	printf( "\n");
	float tmp[3] = {};
	for ( unsigned short int s = 0; s < N; s ++ ) {
		printf( "%hu\t", s );
		for ( unsigned short int d = 0; d < D; d ++ ) {
			printf( "%hu\t%f\t", d, *( (*( p + s )).v + d ) );
			tmp[d] += *( (*( p + s )).v + d );
		}
		printf( "\n" );
	}
	printf( "Velocità totale (cdm): (%f, %f, %f)\n", tmp[0], tmp[1], tmp[2] );
}/* -----  end of method Sistema::Sistema (ctor)  ----- */

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

