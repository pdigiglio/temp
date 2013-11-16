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

//#include "colors.h"
#include "part_libere.h"
#include "info_time.h"

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

	bool x[2];
	begin = clock();
	unsigned int a;
	for ( unsigned int j = 0; j < 1000000; j ++ )
		for ( unsigned k = 0; k < 100000; k ++ )
			x[ k % 2 ] = j * k;

	Sistema s;

	/* prints execution time to stderr */
	print_exe_time( begin );

	exit(EXIT_SUCCESS);
} /* ----------  end of function main  ---------- */
