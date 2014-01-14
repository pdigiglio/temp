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

/*
 * ------------------------------------------------------------------
 *       Class: Particella
 *      Method: save_coordinates
 * Description: 
 * ------------------------------------------------------------------
 */
void
Particella::save_coordinates ( void ) {
	struct ptcl *ptr;

	register unsigned short d;
	for ( unsigned int n = 0; n < Particella::nMax; n ++ ) {
		ptr = p + n;
		for ( d = 0; d < D; d ++ )
			*( *( r + n ) + d ) = *( (*ptr).x + d );
	}

} /* -----  end of method Particella::save_coordinates  ----- */
