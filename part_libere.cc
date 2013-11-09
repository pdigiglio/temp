#include "part_libere.h"

#include <math.h>

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: Sistema
 * Description: [ctor]
 * ------------------------------------------------------------------
 */
Sistema::Sistema (void) {
	/* scelgo una terna di numeri casuali */
	unsigned short int j[3];
	for ( unsigned short int d = 0; d < D; d ++ ) {
		*( j + d ) = (unsigned short) rand();
//		printf( "%hu\t%hu\n", d, *( j + d ) );
	}

	/* lato minimo dei cubi sui vertici dei quali dispongo le sfere */
	float l = 4 * S / sqrt(3);
	/* calcolo le particelle che entrano nel cubo */
	unsigned short int n = (unsigned short) L / l, m = n;
	if ( (float) L / l - n ) {
		m --;
		if ( (float) L / l - n < 2 * S )
			n --;
	}

	/* controllo che il numero di particelle non ecceda la capienza */
	if ( (unsigned) N > n * m ) {
		fprintf( stderr, "[Fatale]"
				" Superata capienza massima sistema %hu\n",
				n * m );
		exit(EXIT_FAILURE);
	}

	/* calcolo il rapporto (intero) tra la capienza e le particelle */
	unsigned short step = n * m / N;
	printf( "Rapporto capienza (%u)/particelle (%hu): %hu\n",
			(unsigned) n * m, (unsigned short) N, step );

	printf( "Siti 1: %hu, Siti 2: %hu\n", n, m );

	/* assegno le velocità */
	float tmp; /* modulo delle velocità */
	for ( unsigned short int n = 0; n < N; n ++ ) {
		tmp = 0.;
		for ( unsigned short int d = 0; d < D; d ++ ) {
			*( (*( p + n )).v + d ) = 2 * ( (float) rand() / RAND_MAX - 1 / 2 );
//			printf( "%hu\t%f\n", d, *( (*( p + n )).v + d ) );
			tmp += *( (*( p + n )).v + d ) * *( (*( p + n )).v + d );
		}
		printf( "%hu\t%f\n", n, tmp );
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

