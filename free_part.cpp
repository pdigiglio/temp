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
#include <time.h>
#include <math.h>

#include "colors.h"
#include "part_libere.h"
#include "info_time.h"
#include "round.h"

#include "TH1D.h"

#define TERM	3

/* 
 * ===  FUNCTION  ===================================================
 *         Name:  main
 *  Description:  
 * ==================================================================
 */
int
main ( /* int argc, char *argv[] */ ) {
	/* inizializzo il seme dei numeri casuali */
	srand( time(NULL) );
	
	/* take initial time */
	unsigned int begin = clock();

	Sistema s;

	/* termalizzo */
	register unsigned int j;

	for ( unsigned int k = 0; k < 1; k ++ ) {
		for ( j = 0; j < s.nMax; j ++ ) {
//			fprintf ( stderr, ANSI_RED "%lu\n" ANSI_RESET, k * s.nMax + j );
			s.evolve();
//			s.time_reset();
		}

//		s.time_reset();
	}

	fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET ": "
			ANSI_YELLOW "%s" ANSI_RESET
			"] Termalization executed (%lu collisions).\n",
			__func__, TERM * s.nMax );
			
//	TH1D *histo = new TH1D( "Dati", "Speed distribution (x axis)", 100, -3., 3. );
//	TCanvas *c = new TCanvas();
//
//	/* output-file name */
//	char f_file_name[] = "histo.dat"; 
//	FILE *f = fopen( f_file_name, "w" );
//	if ( f == NULL ) {
//		fprintf ( stderr, "couldn't open file '%s'; %s\n",
//				f_file_name, strerror(errno) );
//		exit (EXIT_FAILURE);
//	}
//
//	for ( unsigned int k = 0 ; k < 1000; k ++ ) {
//		for ( j = 0; j < s.nMax; j ++ ) {
////			printf( ANSI_RED "%hu" ANSI_RESET "\n", j );
//			s.evolve();
//		}
//		
//		/* raccolgo le misure delle velocità */
//		for ( j = 0; j < s.nMax; j ++ ) {
//////			fprintf( f, "%f\n", s.get_velocity( j, 2 ) );
//			(*histo).Fill( s.get_velocity(j) );
//		}
//
//		s.time_reset();
////
//	}
//
//	if( fclose(f) == EOF ) { /* close output file */
//		fprintf ( stderr, "couldn't close file '%s'; %s\n",
//				f_file_name, strerror(errno) );
//		exit (EXIT_FAILURE);
//	}
//
//	(*histo).Draw();
//	(*c).Print( "New_histo.gif", "gif" );
//
//	s.pression();
//	s.mct();

	/* print execution time */
	print_exe_time( begin, __func__ );

	exit(EXIT_SUCCESS);
} /* ----------  end of function main  ---------- */
