/*
 * ==================================================================
 *
 *       Filename:  corr_length.cpp
 *
 *    Description:  Routine to bin correlator length measures
 *
 *        Version:  1.0
 *        Created:  07/09/2013 11:02:39
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

#include <errno.h>
#include <string.h>

#include "side.h"
#include "round.h"

/* columns */
const signed long int C = L / 2;

int
main ( int argc, char *argv[] ) {
	/* ordine della suddivisione */
	unsigned short int ord = 1;

	/* controllo che argomenti da linea di comando */ 
	if ( argc == 3 )
		ord = atoi( argv[2] );
	else if ( argc == 2 )
		fprintf( stderr, " > Dimensione bin impostata di default a %hu\n", ord );
	else {
		if ( argc == 1 )
			fprintf( stderr, " > Manca il nome del file!\n");
		else /* se sono sbagliati */
			fprintf( stderr, " > Troppi argomenti!\n");

		fprintf( stderr, "[uso] ./analisi {nome file} {dim. bin}\n");
		/* esco dal programma */
		exit( EXIT_FAILURE );
	}

	/* (apro il) file contenente i dati */
	FILE *pFile = fopen( argv[1], "r" );
	if ( pFile == NULL ) {
		fprintf( stderr, " > Impossibile aprire il file '%s': %s\n",
				argv[1], strerror(errno) );
		exit(EXIT_FAILURE);
	}

	/* ---------------------------------------------------------------
	 *  BINNING
	 * -------------------------------------------------------------*/

	/* valori delle misure */
	long double *f[C] = {};
	register long double **fptr = NULL;
	long double tmp;

	/* media ed errore (inizializzate a zero) */
	long double mean[C] = {}; //(long double) 0;
	long double err[C] = {}; // (long double) 0;

	/* acquisisco i valori e calcolo la media */
	unsigned int l, i;
	for ( l = 0; !( feof( pFile ) ); l ++ ) {
		/* assegno puntatore temporaneo */
		fptr = f;

		/* alloco la memoria per i valori in input */
		for ( unsigned short int c = 0; c < C; c ++ ) {
			*fptr = (long double *) realloc( *fptr, ( l + 1 ) * sizeof( long double ) );
			if ( *fptr == NULL ) {
				fprintf ( stderr, "\ndynamic memory reallocation failed\n" );
				exit (EXIT_FAILURE);
			}

			/* reset variable */
			*( *fptr + l ) = (long double) 0;
			/* update pointer */
			fptr ++;
		}

		/* acquisisco i dati */
		for ( i = 0; i < ord && !( feof(pFile) ); i ++ ) {
			/* assegno puntatore temporaneo */
			fptr = f;

			/* prime C - 1 colonne */
			for ( unsigned short int c = 0; c < C - 1; c ++ ) {
				/* leggo il valore */
				fscanf( pFile, "%Lg\t", &tmp );
				/* aggiorno il bin */
				*( *fptr + l ) += tmp;
				/* aggiorno il puntatore */
				fptr ++;
			}

			/* ultima colonna */
			fscanf( pFile, "%Lg\n", &tmp );
			*( *fptr + l ) += tmp;
		}
		
		/* assegno puntatore temporaneo */
		fptr = f;
		/* normalizzo i cluster */
		for ( unsigned short int c = 0; c < C; c ++ ) {
			*( *fptr + l ) /= (long double) i;
		
			/* aggiorno le medie */
			tmp = *( *fptr + l );

			*( mean + c ) += tmp;
			*( err + c ) += tmp * tmp;

			fptr ++;
		}
	}

	/* chiudo il file di input */
	if( fclose( pFile ) == EOF ) {
		fprintf ( stderr, " > Impossibile chiudere il file '%s'; %s\n",
				argv[1], strerror(errno) );
		exit (EXIT_FAILURE);
	}

	/* binning output file */
	char out_file_name[] = "cl_binning.dat";
	FILE *out_stream = fopen( out_file_name, "a" );
	if ( out_stream == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				out_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	/* header */
	fprintf( out_stream, "#ord\t" );
	for ( unsigned short int c = 0; c < C; c ++ )
		fprintf( out_stream, "mean\tvar\tmean\tSDOM\t" );
	
	fprintf( out_stream, "\n" );

	/* bin size */
	fprintf( out_stream, "%u\t", ord);

	/* scrivo i bin nel file di output */
	for ( unsigned short int c = 0; c < C; c ++ ) {
		/* normalizzo la media */
		*( mean + c ) /= (long double) l;

		/* calcolo la varianza */
		*( err + c ) /= (long double) l;
		*( err + c ) -= *( mean + c ) * *( mean + c );
		*( err + c ) = (long double) sqrtl( *( err + c ) );

		/* stampo nel file media e varianza */
//		round( *( mean + c ), *( err + c ), out_stream );
		fprintf( out_stream, "%Lg\t%Lg", *( mean + c ), *( err + c ) );
		fprintf( out_stream, "\t" );

		/* stampo nel file media e sdom */
//		round( *( mean + c ), *( err + c ) / sqrtl( l ), out_stream );
		fprintf( out_stream, "%Lg\t%Lg", *( mean + c ), *( err + c ) / sqrtl( l ) );
		fprintf( out_stream, "\t" );
	}

	fprintf( out_stream, "\n" );

	if( fclose(out_stream) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				out_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	exit( EXIT_SUCCESS );
}				/* ----------  end of function main  ---------- */
