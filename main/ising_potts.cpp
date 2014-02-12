/*
 * ==================================================================
 *
 *       Filename:  ising.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  25/12/2013 20:40:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  P. Di Giglio (), p.digiglio91@gmail.com
 *   Organization:  
 *
 * ==================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#include <string.h>
#include <errno.h>

#include "reticolo.h"
#include "ising.h"
#include "potts.h"
#include "colors.h"
#include "info_time.h"
#include "round.h"

#define LIFE 10000
#define TERM 2000

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int
main ( void ) {
	srand( time( NULL ) );

	unsigned int start = clock();

	Potts R;

	/*---------------------------------------------------------------
	 *  TERMALIZATION
	 *-------------------------------------------------------------*/
	const long double *Ms = NULL;

	register unsigned j;
	for ( j = 0; j < TERM; j ++ ) {
		R.Sweep();
//		fprintf( stdout, "%.16g\n", (double) R.get_E() / R.L2 );

		/* get magnetization */
		Ms = R.get_Ms();
		fprintf( stdout, "%.16Lg\t%.16Lg\n", (long double) *( Ms ) / R.L2, (long double) R.get_Ms()[1] / R.L2 );
	}

	print_exe_time( start, __func__ );

	return 0;

	/* build up system */
	Ising r;

	/*---------------------------------------------------------------
	 *  TERMALIZATION
	 *-------------------------------------------------------------*/
	for ( unsigned short int j = 0; j < TERM; j ++ )
		r.sweep();

	/*---------------------------------------------------------------
	 *  MEASURES
	 *-------------------------------------------------------------*/

	/* output-file name */
	char energy_file_name[] = "energia.dat";
	FILE *energy = fopen( energy_file_name, "w" );
	if ( energy == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				energy_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	/* output-file name */
	char mags_file_name[] = "mag_sweep.dat";
	FILE *mags = fopen( mags_file_name, "w" );
	if ( mags == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				mags_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	/* output-file name */
	char magsa_file_name[] = "mag_sweep_abs.dat";
	FILE *magsa = fopen( magsa_file_name, "w" );
	if ( magsa == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				magsa_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	/* output-file name */
//	char magc_file_name[] = "mag_cluster.dat";
//	FILE *magc = fopen( magc_file_name, "w" );
//	if ( magc == NULL ) {
//		fprintf ( stderr, "couldn't open file '%s'; %s\n",
//				magc_file_name, strerror(errno) );
//		exit (EXIT_FAILURE);
//	}

	/* output-file name */
//	char su_file_name[] = "sus.dat";
//	FILE *su = fopen( su_file_name, "w" );
//	if ( su == NULL ) {
//		fprintf ( stderr, "couldn't open file '%s'; %s\n",
//				su_file_name, strerror(errno) );
//		exit (EXIT_FAILURE);
//	}

	/* output-file name */
	char corr_file_name[] = "corr.dat";
	FILE *corr = fopen( corr_file_name, "w" );
	if ( corr == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				corr_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	float pct = (float) 0;
	for ( unsigned int j = 0; j < LIFE; j ++ ) {
		/*-----------------------------------------------------------
		 *  PROGRESS BAR
		 *---------------------------------------------------------*/
		pct = (double) 100 * ( (double) j / LIFE );
		/* print percentage */
		fprintf( stderr, "Step n. %u of %u. Completed %.2f %%\r", j, LIFE, pct );

		/* evolve system */
		r.sweep();

		/* magnetization (sweep) */
		fprintf( mags, "%.16g\n", (double) r.get_Ms() / r.L2 );
		/* magnetization (sweep) abs */
		fprintf( magsa, "%.16g\n", (double) fabsl( r.get_Ms() ) / r.L2 );
		/* magnetization (cluster) */
//		fprintf( magc, "%.16g\n", (double) r.get_Mm() / r.L2 );
		/* susceptivity */
//		fprintf( su, "%.16g\n", (double) r.get_M2() / r.L2 );
		/* energy */
		fprintf( energy, "%g\n", (double) r.get_E() / r.L2 );
	
		r.correlator();
		r.print_correlator( corr );
	}

	if( fclose(energy) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				energy_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	if( fclose(mags) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				mags_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

//	if( fclose(su) == EOF ) { /* close output file */
//		fprintf ( stderr, "couldn't close file '%s'; %s\n",
//				su_file_name, strerror(errno) );
//		exit (EXIT_FAILURE);
//	}
//
//	if( fclose(magc) == EOF ) { /* close output file */
//		fprintf ( stderr, "couldn't close file '%s'; %s\n",
//				magc_file_name, strerror(errno) );
//		exit (EXIT_FAILURE);
//	}

	if( fclose(magsa) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				magsa_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	if( fclose(corr) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				corr_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	print_exe_time( start, __func__ );

	return 0;
}				/* ----------  end of function main  ---------- */
