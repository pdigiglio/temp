/*
 * ==================================================================
 *
 *       Filename:  lennard_jones.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  18/12/2013 15:47:05
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

#include <string.h>
#include <errno.h>
#include <time.h>

#include <math.h>

#include "colors.h"
#include "eta.h"
#include "part_soffici.h"
#include "info_time.h"
#include "round.h"

/* cycle to termalize system */
#define TERM	10
#define	LIFE	100

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int
main ( void ) {
	/* inizializzo il seme dei numeri casuali */
//	srand( time(NULL) );
	
	/* take initial time */
	unsigned int begin = clock();

	Soft s;

	register unsigned int j;

	for ( unsigned short int k = 0; k < TERM; k ++ ) {
		for ( j = 0; j < Particella::nMax; j ++ ) {
			s.evolve();
			s.set_kT( 1.2 );
	//		s.set_energy( -100 );
		}
//		s.set_kT( 1.2 );
	}

	s.reset_pr();
	
	char r_file_name[] = "radius.dat"; /* output-file name */
	FILE *r = fopen( r_file_name, "w" );

	if ( r == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				r_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	char speed_file_name[] = "speed.dat"; /* output-file name */
	FILE *speed = fopen( speed_file_name, "w" );

	if ( speed == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				speed_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	char p_file_name[] = "press_measures.dat"; /* output-file name */
	FILE *p = fopen( p_file_name, "w" );

	if ( p == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				p_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	double press = (double) 0, epress = (double) 0;
	double pct = (double) 0;
	for ( unsigned short int k = 0; k < LIFE; k ++ ) {
		/* aggiorno le variabili di controllo */
		pct = (double) 100 * ( (double) k / LIFE );
		/* stampo la percentuale */
		fprintf( stderr, "Step n. %u of %u. Completed %.4g %%\r", k, LIFE, pct );
		
		for ( j = 0; j < Particella::nMax; j ++ ) {
			fprintf( r, "%.16g\n", s.evolve() );
			fprintf( stdout, "%.16g\t%.16g\n", s.get_K(), s.get_U() );
		}

		/* speed distribution */
		for ( j = 0; j < Particella::nMax; j ++ )
			fprintf( speed, "%.16g\n", s.get_velocity( j ) );

		fprintf( p, "%.16g\n", s.get_pr() );

		s.reset_pr();
	}

	if( fclose(r) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				r_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	if( fclose(speed) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				speed_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	if( fclose(p) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				p_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	/* print execution time */
	print_exe_time( begin, __func__ );

	return 0;
}				/* ----------  end of function main  ---------- */
