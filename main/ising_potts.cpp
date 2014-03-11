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
#define TERM 500

/* 
 * ===  FUNCTION  ===================================================
 *         Name:  main
 *  Description:  
 * ==================================================================
 */
int
main ( void ) {
	/* random number generator seed */
//	srand( time( NULL ) );

	/* take initial time */
	unsigned int start = clock();

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
	char magc_file_name[] = "mag_cluster.dat";
	FILE *magc = fopen( magc_file_name, "w" );
	if ( magc == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				magc_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	/* output-file name */
	char corr_file_name[] = "corr.dat";
	FILE *corr = fopen( corr_file_name, "w" );
	if ( corr == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				corr_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	/* output-file name */
	char sus_imp_file_name[] = "sus_improved.dat";
	FILE *sus_imp = fopen( sus_imp_file_name, "w" );
	if ( sus_imp == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				sus_imp_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	Potts R;

	/*---------------------------------------------------------------
	 *  TERMALIZATION
	 *-------------------------------------------------------------*/
	register unsigned int j = 0;
	for ( j = 0; j < TERM; j ++ )
		R.Sweep();

	/*---------------------------------------------------------------
	 *  MEASURES
	 *-------------------------------------------------------------*/

	float pct = (float) 0;
	long double m1, m2;
	for ( unsigned int j = 0; j < LIFE; j ++ ) {
		/*-----------------------------------------------------------
		 *  PROGRESS BAR
		 *---------------------------------------------------------*/
		pct = (double) 100 * ( (double) j / LIFE );
		/* print percentage */
		fprintf( stderr, "Step n. %u of %u. Completed %.2f %%\r", j, LIFE, pct );

		/* evolve system */
		R.Sweep();

		/* magnetization (sweep) */
		m1 = *( R.get_Ms() );
		m2 = R.get_Ms()[1];

		fprintf( mags, "%.16Lg\t%.16Lg\n", (long double) m1 / R.L2, (long double) m2 / R.L2 );

		/* magnetization (sweep) abs */
		fprintf( magsa, "%.16g\n", (double) sqrt( m1 * m1 + m2 * m2 ) / R.L2 );
		
		/* magnetization (cluster) */
		fprintf( magc, "%.16g\n", (double) R.get_Mm() / R.L2 );

		/* susceptivity (improved) */
		fprintf( sus_imp, "%.16g\n", (double) R.get_M2() / R.L2 );

		/* energy */
		fprintf( energy, "%.16g\n", (double) R.get_E() / R.L2 );

		/* get magnetization */

		R.correlator();
		R.print_correlator( corr );
	}

//	/* build up system */
//	Ising r;
//
//	/*---------------------------------------------------------------
//	 *  TERMALIZATION
//	 *-------------------------------------------------------------*/
//	for ( unsigned short int j = 0; j < TERM; j ++ )
//		r.Sweep();
//
//	/*---------------------------------------------------------------
//	 *  MEASURES
//	 *-------------------------------------------------------------*/
//
//	float pct = (float) 0;
//	for ( unsigned int j = 0; j < LIFE; j ++ ) {
//		/*------------------------------------------------------------
//		 *  PROGRESS BAR
//		 *---------------------------------------------------------*/
//		pct = (double) 100 * ( (double) j / LIFE );
//		/* print percentage */
//		fprintf( stderr, "Step n. %u of %u. Completed %.2f %%\r", j, LIFE, pct );
//
//		/* evolve system */
//		r.Sweep();
//
//		/* magnetization (sweep) */
//		fprintf( mags, "%.16g\n", (double) r.get_Ms() / r.L2 );
//		/* magnetization (sweep) abs */
//		fprintf( magsa, "%.16g\n", (double) fabsl( r.get_Ms() ) / r.L2 );
//		/* magnetization (cluster) */
//		fprintf( magc, "%.16g\n", (double) r.get_Mm() / r.L2 );
//		/* energy */
//		fprintf( energy, "%.16g\n", (double) r.get_E() / r.L2 );
//	
//		r.correlator();
//		r.print_correlator( corr );
//	}

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

	if( fclose(magc) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				magc_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

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

	if( fclose(sus_imp) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				sus_imp_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	print_exe_time( start, __func__ );
	return 0;
}				/* ----------  end of function main  ---------- */
