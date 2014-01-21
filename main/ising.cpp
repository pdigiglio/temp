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
#include "colors.h"
#include "info_time.h"
#include "round.h"

#define LIFE 1000
#define TERM 250

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int
main ( void ) {
//	srand( time( NULL ) );

	unsigned int start = clock();

	/* build up system */
	Reticolo r;

	/* termalyze system */
//	for ( unsigned short int j = 0; j < TERM; j ++ )
//		r.sweep();

	double e = (double) 0, ee = (double) 0;
	double tmp;

	/* output-file name */
	char energy_file_name[] = "energia.dat";
	FILE *energy = fopen( energy_file_name, "w" );
	if ( energy == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				energy_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	/* output-file name */
	char mag_file_name[] = "magnetizzazione.dat";
	FILE *mag = fopen( mag_file_name, "w" );
	if ( mag == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				mag_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	float pct = (float) 0;
	for ( unsigned int j = 0; j < LIFE; j ++ ) {
		/*-------------------------------------------------------------------
		 *  PROGRESS BAR
		 *-----------------------------------------------------------------*/
		pct = (double) 100 * ( (double) j / LIFE );
		/* stampo la percentuale */
		fprintf( stderr, "Step n. %u of %u. Completed %.2f %%\r", j, LIFE, pct );

		fprintf( mag, "%g\n", (double) r.get_M() / r.L2 );
//		fprintf( stderr, ANSI_RED "Sweep %u\n" ANSI_RESET, j );
		tmp = (double) r.get_E() / r.L2;
		fprintf( energy, "%g\n", tmp );
		
		r.sweep();

		e += tmp;
		ee += tmp * tmp;
	}

	if( fclose(energy) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				energy_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	if( fclose(mag) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				mag_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	e /= (double) LIFE;
	ee /= (double) LIFE;
	ee -= e*e;


	char ene_file_name[] = "energy_plot.dat"; /* output-file name */
	FILE *ene = fopen( ene_file_name, "a" );

	if ( ene == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				ene_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}
	
	fprintf( ene, "%g\t", (double) B );
	round( e, sqrt( ee / LIFE), ene );
	fprintf( ene, "\n" );

	if( fclose(ene) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				ene_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

//	printf( "%g %g\n", e, ee );

//	r.print_lattice();

	print_exe_time( start, __func__ );

	return 0;
}				/* ----------  end of function main  ---------- */
