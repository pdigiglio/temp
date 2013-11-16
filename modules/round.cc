#include "round.h"
#include "colors.h"

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
		fprintf( stdout, ANSI_RED "%Lf\t%Lf" ANSI_RESET, val, err );
		return;
	}

	/* dichiaro una variabile per l'esponente */
	short int exp = (short) log10( fabs( err ) );

	/* controllo che l'approssimazione sia corretta */
	long double tmp = err / pow( 10, exp );
	while ( !( tmp >= 1 && tmp < 10 ) ) {
		if ( err / pow( 10, exp ) <= 1 ) exp --;
		else exp ++;
	}

	/* controllo le cifre decimali da tenere */
	if ( tmp < 3 )
		exp --;

	printf( "%Lf\t%Lf",
				floorl( val / powl(10., exp) + 0.5) * powl(10., exp),
				floorl( tmp + 0.5) * powl(10., exp)
			);
}		/* -----  end of function round  ----- */
