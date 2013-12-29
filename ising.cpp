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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int
main ( void ) {
	srand( time( NULL ) );

	Reticolo r;

//	printf( "%hu\n", (unsigned short) sizeof( unsigned short int ) );
//	unsigned short int b = 2;
//
//	unsigned short t[16] = { 1 };
//
//	unsigned short int tmp = *t;
//	for ( unsigned short int i = 1; i < 16; i ++ ) {
//		tmp *= 2;
//		*( t + i ) = tmp;
//	}
//
//
//	fprintf( stdout, "%hu _10 = ", b );
//	for ( short int i = 15; i >= 0; i -- ) {
//		fprintf( stdout, "%hu", (bool) ( b & *( t + i ) ) );
//	}
//	fprintf( stdout, " _2\n" );
	

//	for ( unsigned int i = 0; i < L * L ; i ++ ) {
//		printf( "%u, %u: %hd\n", i / L, i % L, r.single_E( i / L, i % L ));
//	}

	for ( unsigned int j = 0; j < 2; j ++ ) {
//		printf( "%g\n", (double) r.get_M() );
		r.Sweep();
	}

//	printf( "\n" );
//	for ( unsigned int i = 0; i < L * L ; i ++ ) {
//		printf( "%u, %u: %hd\n", i / L, i % L, r.single_E( i / L, i % L ));
//	}
//
//	printf( "Mag: %g\n", r.get_M() );
	return 0;
}				/* ----------  end of function main  ---------- */
