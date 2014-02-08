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

#define LIFE 20000
#define TERM 500

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

	/* build up system */
	Reticolo r;

	/* termalyze system */
	for ( unsigned short int j = 0; j < TERM; j ++ ) {
//		fprintf( stderr, "#Sweep %u\n\n", j );
		r.Sweep();
	}

//	r.print_lattice();

	r.correlator();
	r.print_correlator();

	return 0;

	double e = (double) 0, ee = (double) 0;
	double tmp;

	double ms = (double) 0, mse = (double) 0;
	double tmps;
	
	double mm = (double) 0, mme = (double) 0;
	double tmpm;

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

	char su_file_name[] = "suscettività.dat"; /* output-file name */
	FILE *su = fopen( su_file_name, "w" );
	if ( su == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				su_file_name, strerror(errno) );
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

		/* magnetization (sweep) */
		tmps = (double) fabs( r.get_Ms() ) / r.L2;
		tmpm = (double) r.get_Mm() / r.L2;
		fprintf( mag, "%.16g\t%.16g\n", tmps, tmpm );
		fprintf( su, "%.16g\n", r.get_M2() / r.L2 );

		ms += tmps;
		mse += tmps * tmps;

		mm += tmpm;
		mme += tmpm * tmpm;

		/* energy */
		tmp = (double) r.get_E() / r.L2;
		fprintf( energy, "%g\n", tmp );
	
		e += tmp;
		ee += tmp * tmp;
		
		r.Sweep();
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

	if( fclose(su) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				su_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	/*-------------------------------------------------------------------
	 *  ENERGY
	 *-----------------------------------------------------------------*/
	e /= (double) LIFE;
	ee /= (double) LIFE;
	ee -= e*e;

	char ene_file_name[] = "energy_plot_c.dat"; /* output-file name */
	FILE *ene = fopen( ene_file_name, "a" );

	if ( ene == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				ene_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}
	
	fprintf( ene, "#B\tenergy\tSDOM\n");
	fprintf( ene, "%g\t", (double) B );
	round( e, sqrt( ee / LIFE), ene );
	fprintf( ene, "\n" );

	if( fclose(ene) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				ene_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	/*-------------------------------------------------------------------
	 *  MAGNETIZATION
	 *-----------------------------------------------------------------*/
	ms /= (double) LIFE;
	mse /= (double) LIFE;
	mse -= ms * ms;
	
	mm /= (double) LIFE;
	mme /= (double) LIFE;
	mme -= mm * mm;

	char m_file_name[] = "magnetizzazione_plot.dat"; /* output-file name */
	FILE *m = fopen( m_file_name, "a" );

	if ( m == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				m_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	fprintf( m, "#B\tm (sweep)\tSDOM\tm (max)\tSDOM\n" );
	fprintf( m, "%g\t", (double) B );

	round( ms, sqrt( mse / LIFE ), m );
	fprintf( m, "\t" );
	
	round( mm, sqrt( mme / LIFE ), m );
	fprintf( m, "\n" );

	if( fclose(m) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				m_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	print_exe_time( start, __func__ );

	return 0;
}				/* ----------  end of function main  ---------- */
