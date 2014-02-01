/*
 * ==================================================================
 *
 *       Filename:  susc.cpp
 *
 *    Description:  Routine to evaluate susceptivity from magnetization
 *    				data file(s).
 *
 *        Version:  1.0
 *        Created:  31/01/2014 16:55:06
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

#include "colors.h"
#include "side.h"

#include <math.h>

#include <string.h>
#include <errno.h>

/* 
 * ===  FUNCTION  ===================================================
 *         Name:  main
 *  Description:  
 * ==================================================================
 */
int
main ( int argc, char *argv[] ) {
	/* check command line arguments */
	if ( argc != 2 ) {
		if ( argc < 2 )
			fprintf( stderr, "Too few cmd line arguments!\n" );
		else if ( argc > 2 )
			fprintf( stderr, "Too few cmd line arguments!\n" );

		exit( EXIT_FAILURE );	
	}

	/* input-file pointer */
	FILE *input = fopen( argv[1], "r" );
	if ( input == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				argv[1], strerror(errno) );
		exit (EXIT_FAILURE);
	}

	long double *data = NULL, *dataptr = NULL;
	long double mean = (long double) 0;

	unsigned int l;
	for ( l = 0; !( feof(input) ); l ++ ) {
		/* allocate memory for data */
		data = (long double *) realloc ( data, ( l + 1 ) * sizeof( long double ) );
		if ( data == NULL ) {
			fprintf ( stderr, "[" ANSI_RED "%s" ANSI_RESET ": "
					ANSI_YELLOW "realloc" ANSI_RESET
					"] Dynamic memory allocation failed.\n", __func__ );
			exit (EXIT_FAILURE);
		}

		/* assign temporary pointer */
		dataptr = data + l;

		/* read data */
		fscanf( input, "%Lg\n", dataptr );

		/* update mean */
		mean += *( dataptr );
	}

	if( fclose(input) == EOF ) { /* close input file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				argv[1], strerror(errno) );
		exit (EXIT_FAILURE);
	}

	/* normalize mean */
	mean /= (long double) l;

	fprintf( stderr, "media: %Lg\n", mean );

	long double tmp;
	for ( unsigned int j = 0; j < l; j ++ ) {
		tmp = *( data + j ) - mean;
		tmp *= (long double) L;

		printf( "%.16Lg\n", tmp * tmp );
	}
	
	exit(EXIT_SUCCESS);
}				/* ----------  end of function main  ---------- */
