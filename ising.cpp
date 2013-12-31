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

#include "reticolo.h"
#include "colors.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int
main ( void ) {
//	srand( time( NULL ) );

	Reticolo r;

//	for ( unsigned int i = 0; i < L * L ; i ++ ) {
//		printf( "%u, %u: %hd\n", i / L, i % L, r.single_E( i / L, i % L ));
//	}

	for ( unsigned int j = 0; j < 110; j ++ ) {
//		printf( "%g\n", (double) r.get_M() );
		fprintf( stderr, ANSI_RED "Sweep %u\n" ANSI_RESET, j );
		r.Sweep();
	}

	r.print_lattice();

//	printf( "\n" );
//	for ( unsigned int i = 0; i < L * L ; i ++ ) {
//		printf( "%u, %u: %hd\n", i / L, i % L, r.single_E( i / L, i % L ));
//	}
//
//	printf( "Mag: %g\n", r.get_M() );
	return 0;
}				/* ----------  end of function main  ---------- */
