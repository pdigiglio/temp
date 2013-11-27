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

/* 
 * ===  FUNCTION  ===================================================
 *         Name:  main
 *  Description:  
 * ==================================================================
 */
int
main ( int argc, char *argv[] ) {
	/* inizializzo il seme dei numeri casuali */
	srand( time(NULL) );
	
	/* take initial time */
	unsigned int begin = clock();

	Sistema s;

	/* termalizzo */
	unsigned short int term = 3 * s.nMax;
	register unsigned short j;
	for ( j = 0; j < s.nMax; j ++ )
		s.evolve();

	/* output-file name */
	char f_file_name[] = "histo.dat"; 
	FILE *f = fopen( f_file_name, "w" );
	if ( f == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				f_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	for ( unsigned int k = 0 ; k < 1500; k ++ ) {
		for ( j = 0; j < s.nMax; j ++ ) {
//			printf( ANSI_RED "%hu" ANSI_RESET "\n", j );
			s.evolve();
		}
		
		/* raccolgo le misure delle velocità */
		for ( j = 0; j < s.nMax; j ++ ) {
			fprintf( f, "%f\n", s.get_velocity( j ) );
		}

	}

	if( fclose(f) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				f_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}


	/* print execution time */
	print_exe_time( begin, __func__ );

	exit(EXIT_SUCCESS);
} /* ----------  end of function main  ---------- */
