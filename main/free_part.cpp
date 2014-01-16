/*
 * ==================================================================
 *
 *       Filename:  part_libere.cpp
 *
 *    Description:  Programma per simulare un sistema di particelle
 *    				sferiche che interagiscono col potenziale di sfera
 *    				dura in un volume cubico di lato L finito.
 *
 *        Version:  1.0
 *        Created:  07/11/2013 22:24:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  P. Di Giglio, p.digiglio91@campus.unimib.it
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
#include "part_libere.h"
#include "info_time.h"
#include "round.h"

/* cycle to termalize system */
#define TERM	10

/* 
 * ===  FUNCTION  ===================================================
 *         Name:  main
 *  Description:  
 * ==================================================================
 */
int
main ( /* int argc, char *argv[] */ ) {
	/* inizializzo il seme dei numeri casuali */
//	srand( time(NULL) );
	
	/* take initial time */
	unsigned int begin = clock();

	Sistema s;
//	printf( "%.16g\n", s.get_KT() );

//	return 0;

	/* termalizzo */
	register unsigned int j, k;
	for ( k = 0; k < TERM; k ++ ) {
		for ( j = 0; j < s.nMax; j ++ ) {
//			printf( "%d\n\n", j );
			s.evolve();
		}
	}

//	return 0;
	
	/* reset pressure */
	s.reset_pr();

	fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET ": "
			ANSI_YELLOW "%s" ANSI_RESET
			"] Termalization executed (%lu collisions).\n",
			__func__, TERM * s.nMax );

	
	/*-------------------------------------------------------------------
	 *  Misure
	 *-----------------------------------------------------------------*/
	/* speeds histogram */
	char speed_file_name[] = "speed.dat";
	FILE *speed = fopen( speed_file_name, "w" );
	if ( speed == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				speed_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	/* collision delta time histogram */
	char dt_file_name[] = "delta_t.dat";
	FILE *dt = fopen( dt_file_name, "w" );
	if ( dt == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				dt_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	/* single collision delta time histogram */
	char sdt_file_name[] = "single_delta_t.dat";
	FILE *sdt = fopen( sdt_file_name, "w" );
	if ( sdt == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				sdt_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	/* free path */
	char fp_file_name[] = "free_path.dat";
	FILE *fp = fopen( fp_file_name, "w" );
	if ( fp == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				fp_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	/* pressure histogram (double pick) */
	char dp_file_name[] = "dp_measures.dat";
	FILE *dp = fopen( dp_file_name, "w" );
	if ( dp == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				dp_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	printf( "#start\n" );
	s.save_coordinates();
	double p[2] = {}, ptemp;
	double t;
	for ( k = 0 ; k < 10000; k ++ ) {

		/* save time before cycle */
		t = s.get_time();

		/* evolve system and print collision delta times */
		for ( j = 0; j < s.nMax; j ++ ) {
			fprintf( dt, "%.16g\n", s.evolve() );
			/* free path */
			s.print_fp( fp );
			/* single collision time */
			s.print_ct( sdt );
		}

		/* take velocity measures */
		for ( j = 0; j < s.nMax; j ++ )
			fprintf( speed, "%.16g\n", s.get_velocity(j) );

		/* measure pressure */
		ptemp = s.get_pr() / ( s.get_time() - t );
		fprintf( dp, "%.16g\n", ptemp);

		*p += ptemp;
		*( p + 1 ) += ptemp * ptemp;

		/* reset pressure */
		s.reset_pr();

//		s.mass_center_speed();
	}

	if( fclose(speed) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				speed_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	if( fclose(dt) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				dt_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	if( fclose(dp) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				dp_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	if( fclose(sdt) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				sdt_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	if( fclose(fp) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				fp_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	/* normalize pressure */
	*p = (double) *p / k;
	*( p + 1 ) = (double) *( p + 1 ) / k;
	*( p + 1 ) -= *p * *p;
	*( p + 1 ) = sqrt( *( p + 1 ) / ( k - 1 ) );
	for ( j = 0; j < 2; j ++ )
		*( p + j ) = (double) s.S * *( p + j ) / ( 2 * s.get_K() );

	/* output-file name */
	char press_file_name[] = "pressure.dat"; 
	FILE *press = fopen( press_file_name, "a" );
	if ( press == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				press_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}
	
	fprintf( press, "%g\t", (double) E );
	/* prints rounded value in 'press' file */
	round( *p, *( p + 1 ), press );
	fprintf( press, "\n" );

	if( fclose(press) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				press_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	/* print execution time */
	print_exe_time( begin, __func__ );

	return 0;
} /* ----------  end of function main  ---------- */
