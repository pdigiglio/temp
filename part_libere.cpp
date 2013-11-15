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

#include "part_libere.cc"
#include "info_time.cc"

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

	unsigned int begin, end;

	begin = clock();
	unsigned int a;
	for ( unsigned int j = 0; j < 1000000; j ++ )
		for ( unsigned k = 0; k < 1000; k ++ )
			a = j * k;

	Sistema s;
	end = clock();

	print_exe_time( begin, end );
//	fprintf( stderr, "a: %u; %g\n", a, (double) ( end - begin ) / CLOCKS_PER_SEC );

	exit(EXIT_SUCCESS);
} /* ----------  end of function main  ---------- */
