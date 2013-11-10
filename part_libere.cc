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
	for ( unsigned short int n = 0; n < N; n ++ ) {
		for ( unsigned short int d = 0; d < D; d ++ ) {
			*( (*( p + n )).v + d ) = 2 * ( (float) rand() / RAND_MAX - 1 / 2 );
//			printf( "%hu\t%f\n", d, *( (*( p + n )).v + d ) );
			K += *( (*( p + n )).v + d ) * *( (*( p + n )).v + d );
		}
	}
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

