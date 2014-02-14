/*
 * ==================================================================
 *
 *       Filename:  cut.c
 *
 *    Description:  Programmino per approssimare automaticamente i 
 *    				dati contenuti nei file prodotti da esecuzioni di
 *    				altri programmi. Il programma è pensato per essere
 *    				riscritto a seconda del tipo di tabella che bi-
 *    				arrotondare sogna
 *
 *        Version:  1.0
 *        Created:  19/09/2013 08:35:38
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

#include <math.h>

#include <string.h>
#include <errno.h>

#include "round.h"

int
main ( int argc, char *argv[] ) {
	/* nome del file input */
	char *in_file_name;
	
	/* controllo i parametri da riga di comando */
	if ( argc == 2 )
		in_file_name = argv[1];
	else { /* se sono sbagliati */
		if ( argc > 2 )
			fprintf( stderr, " > Troppi argomenti!\n");
		else if ( argc < 2 )
			fprintf( stderr, " > Manca il nome del file!\n");

		/* esco */
		exit( EXIT_FAILURE );
	}

	/* puntatore al file dati in input */
	FILE *in = fopen( in_file_name, "r" );
	if ( in == NULL ) {
		fprintf ( stderr, "Impossibile aprire il file '%s'; %s\n",
				in_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	/*---------------------------------------------------------------
	 *  XXX Questa parte va adattata al formato del file input
	 *-------------------------------------------------------------*/

	/* altre colonne: valore e errore */
	double val, err;
	/* finché il file non finisce */
	while ( !( feof( in ) ) ) {
		/* stampo le prime colonne di interi */
		for ( unsigned short int t = 0; t < 1; t ++ ) {
			fscanf( in, "%lg\t", &val );
			printf( "%lg\t", val );
		}

		/* arrotondo le altre tre colonne */
		for ( unsigned short int t = 0; t < 2; t ++ ) {
			fscanf( in, "%lg\t%lg\t", &val, &err );
			round( val, err );
			printf( "\t" );
		}

		for ( unsigned short int t = 0; t < 3; t ++ ) {
			fscanf( in, "%lg\t", &val );
			printf( "%.10lg\t", val );
		}

		/* vado a capo */
		printf("\n");
	}

	if( fclose(in) == EOF ) { /* close input file */
		fprintf ( stderr, "Impossibile chiudere il file '%s'; %s\n",
				in_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}				/* ----------  end of function main  ---------- */
