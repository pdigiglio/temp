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
round ( long double val, long double err, FILE *stream ) {
	if (  isnan( err * val ) ) {
		fprintf( stream, ANSI_RED "%Lf\t%Lf" ANSI_RESET, val, err );
		return;
	}

	/* dichiaro una variabile per l'esponente */
	short int exp = (short) log10( fabs( err ) );

	/* controllo che l'approssimazione sia corretta */
	long double tmp = err / powl( 10, exp );
	while ( !( tmp >= 1. && tmp < 10. ) ) {
		if ( tmp <= 1 ) exp --;
		else exp ++;

//		printf( "%hd\n", exp );
		tmp = err / powl( 10, exp );
	}

//	printf( "%Lg, %Lg, %hd\n", err, tmp, exp );

	/* controllo le cifre decimali da tenere */
	if ( tmp < 3. )
		tmp = err / powl( 10, -- exp );

//	printf( "%hd\n", exp);

	fprintf( stream, "%.16Lg\t%.16Lg",
				roundl( val / powl(10., exp) ) * powl(10., exp),
				roundl( tmp ) * powl(10., exp)
			);

//	return exp;
}		/* -----  end of function round  ----- */

/* 
 * ===  FUNCTION  =============================================================
 *         Name:  round
 *  Description:  rounds 'val' and 'err' to the same signicant figures
 *  			  (double version)
 * ============================================================================
 */
void
round ( double val, double err, FILE *stream ) {
	if (  isnan( err * val ) ) {
		fprintf( stream, ANSI_RED "%f\t%f" ANSI_RESET, val, err );
		return;
	}

	/* dichiaro una variabile per l'esponente */
	short int exp = (short) log10( fabs( err ) );

	/* controllo che l'approssimazione sia corretta */
	double tmp = err / pow( 10, exp );
	while ( !( tmp >= 1 && tmp < 10 ) ) {
		if ( tmp <= 1 ) exp --;
		else exp ++;

		tmp = err / pow( 10, exp );
	}

	/* controllo le cifre decimali da tenere */
	if ( tmp < 3 )
		tmp = err / pow( 10, -- exp );

	fprintf( stream, "%g\t%g",
				round( val / pow(10., exp) ) * pow(10., exp),
				round( tmp ) * pow(10., exp)
			);
}		/* -----  end of function round  ----- */
