#include "colors.h"
#include "round.h"

#include "particella.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <string.h>
#include <errno.h>

/*
 * ------------------------------------------------------------------
 *       Class: Particella
 *      Method: Particella
 * Description: [ctor]
 * ------------------------------------------------------------------
 */
Particella::Particella (void) {
	/* controllo di non aver inserito troppe particelle nella scatola */
	if ( 2 * S > sqrt( D ) / N ) {
		fprintf ( stderr, "[" ANSI_RED "error" ANSI_RESET ": "
				ANSI_YELLOW "%s" ANSI_RESET 
				"] Sphere radius too high!\n"
				" >> Line %u of file '%s'\n",
				__func__, __LINE__, __FILE__ );
		exit (EXIT_FAILURE);

	}

	/* stampo a schermo informazioni (n. particelle, raggio) */
	fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET "] "
			"Number :: radius of particles (with N = %u) >> %lu :: %1.1g L\n",
			(unsigned) N, (unsigned long) nMax, S );


	fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET ": "
					ANSI_YELLOW "%s" ANSI_RESET "] constructed.\n", __func__ );
} /* -----  end of method Particella::Particella (ctor)  ----- */

///*
// * ------------------------------------------------------------------
// *       Class: Particella
// *      Method: Particella
// * Description: [copy ctor]
// * ------------------------------------------------------------------
// */
//Particella::Particella (const Particella &other) {
//} /* -----  end of method Particella::Particella (copy ctor)  ----- */
//
///*
// * ------------------------------------------------------------------
// *       Class: Particella
// *      Method: ~Particella
// * Description: destructor
// * ------------------------------------------------------------------
// */
//Particella::~Particella (void) {
//} /* -----  end of method Particella::~Particella (dtor)  ----- */
//
///*
// * ------------------------------------------------------------------
// *       Class: Particella
// *      Method: operator '='
// * Description: assignment operator
// * ------------------------------------------------------------------
// */
//Particella&
//Particella::operator = (const Particella &other) {
//	if ( this != &other ) {
//	}
//	return *this;
//} /* -----  end of method Particella::operator =  (ass. op.)  ----- */
//
