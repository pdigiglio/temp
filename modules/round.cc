#include "round.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* 
 * ===  FUNCTION  =============================================================
 *         Name:  round
 *  Description:  rounds 'val' and 'err' to the same signicant figures
 * ============================================================================
 */
void
round ( long double val, long double err ) {
	if (  isnan( err ) ) {
		printf( "%Lf\t%Lf", val, err );
		return;
	}

	/* dichiaro una variabile per l'esponente */
	short int exp = (short) log10( fabs( err ) );

	/* controllo che l'approssimazione sia corretta */
	while ( !( err / pow( 10, exp ) >= 1 && err / pow( 10, exp ) < 10 ) ) {
		if ( err / pow( 10, exp ) <= 1 ) exp --;
		else exp ++;
	}

	/* controllo le cifre decimali da tenere */
	if ( err / pow( 10, exp ) < 3 )
		exp --;

	printf( "%Lf\t%Lf",
				floorl( val / powl(10., exp) + 0.5) * powl(10., exp),
				floorl( err / powl(10., exp) + 0.5) * powl(10., exp)
			);
}		/* -----  end of function round  ----- */
