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
#define TERM	25
#define	LIFE	1500
#define STEP	150

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

	/* declare system class */
	Soft s;

	/* termalization */
	register unsigned int j;
	for ( unsigned short int k = 0; k < TERM; k ++ )
		for ( j = 0; j < STEP; j ++ ) {
			s.evolve();
			s.set_energy( -2.98 );
		}

	/* reset variables */
	s.reset_pr();
	s.reset_R2M();
	
	/* save coordinates after termalization */
	s.save_coordinates();
	
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

	double pct = (double) 0;

	fprintf( stdout, "#time\tkinetic\tinternal\tkt\n" );
	double K = 0., Ke = 0., u = 0., ue = 0., kt = 0., kte = 0., tmp = 0.;
	for ( unsigned short int k = 0; k < LIFE; k ++ ) {

		/* aggiorno le variabili di controllo */
		pct = (double) 100 * ( (double) k / LIFE );
		/* stampo la percentuale */
		fprintf( stderr, "Step n. %u of %u. Completed %.2f %%\r", k, LIFE, pct );
		
		for ( j = 0; j < STEP; j ++ ) {
			s.evolve();

			/* get R2M */
//			fprintf( r, "%g\t%.16g\n", ( k * LIFE + j ) * dt, (double) s.get_R2M() / ( s.L * s.L ) );
			/* reset R2M */
//			s.reset_R2M();

			fprintf( stdout, "%g\t%.16g\t%.16g\t%.16g\n", s.get_time(), s.get_K(), s.get_U(), s.get_KT() );
			
			tmp = s.get_K();
			K += tmp;
			Ke += tmp * tmp;

			tmp = s.get_U();
			u += tmp;
			ue += tmp * tmp;

			tmp = s.get_KT();
			kt += tmp;
			kte += tmp * tmp;
		}

		/* speed distribution */
//		for ( j = 0; j < Particella::nMax; j ++ )
//			fprintf( speed, "%.16g\n", s.get_velocity( j ) );

		/* get pressure */
//		fprintf( p, "%.16g\n", (double) s.get_pr() / STEP );
		/* reset pressure */
//		s.reset_pr();


		/* reset temperature */
//		s.set_kT( 1.22 );
	}

	/* output-file name */
	char tl_file_name[] = "limite_termodinamico.dat";
	FILE *tl = fopen( tl_file_name, "a" );

	if ( tl == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				tl_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	fprintf( tl, "%lu\t", Particella::nMax );
	
	K /= (double) LIFE * STEP;
	Ke /= (double) LIFE * STEP;
	Ke -= (double) K * K;
	Ke /= sqrt((double) LIFE * STEP );
	round( K, Ke, tl );
	fprintf( tl, "\t" );

	u /= (double) LIFE * STEP;
	ue /= (double) LIFE * STEP;
	ue -= (double) u * u;
	ue /= sqrt((double) LIFE * STEP );
	round( u, ue, tl );
	fprintf( tl, "\t" );

	kt /= (double) LIFE * STEP;
	kte /= (double) LIFE * STEP;
	kte -= (double) kt * kt;
	kte /= sqrt((double) LIFE * STEP );
	round( kt, kte, tl );
	fprintf( tl, "\n" );
	
	if( fclose(tl) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				tl_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
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
