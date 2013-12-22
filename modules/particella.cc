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

/*
 * ------------------------------------------------------------------
 *       Class: Particella
 *      Method: print_x
 * Description: 
 * ------------------------------------------------------------------
 */
void
Particella::print_x ( void ) {
	/* pointer to first particle structure */
	struct ptcl *ptr = p;

	for ( unsigned int n = 0; n < Particella::nMax; n ++ ) {
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
} /* -----  end of method Particella::print_x  ----- */
