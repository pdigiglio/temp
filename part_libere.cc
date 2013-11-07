#include "part_libere.h"

/* definisce le macro contenenti i valori massimi dei tipi */
#include <limits.h>

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

