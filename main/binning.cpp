/*
 * ==================================================================
 *
 *       Filename:  binning.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  31/01/2014 12:31:47
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

#include "round.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int
main ( int argc, char *argv[] ) {
	/* ordine della suddivisione */
	unsigned short int ord = 30;
	
	/* controllo che argomenti da linea di comando */ 
	if ( argc == 3 )
		ord = atoi( argv[2] );
	else if ( argc == 2 )
		fprintf( stderr, " > Dominio bin impostata di default a %hu\n", ord );
	else { /* se sono sbagliati */
		if ( argc > 3 )
			fprintf( stderr, " > Troppi argomenti!\n");
		else if ( argc < 2 )
			fprintf( stderr, " > Manca il nome del file!\n");

		fprintf( stderr, "[uso] ./binning {nome file} {dim. bin}\n");
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
	 *  CALCOLO MEDIE ED ERRORI
	 * -------------------------------------------------------------*/

	/* valori delle misure */
	long double *f = NULL;


	/* acquisisco i valori e calcolo la media */
	unsigned int l;
	for ( l = 0; !( feof(pFile) ); l ++ ) {
		/* alloco la memoria per i valori in input */
		f = (long double *) realloc( f, (l + 1) * sizeof(long double) );
		if ( f == NULL ) {
			fprintf ( stderr, "\ndynamic memory reallocation failed\n" );
			exit (EXIT_FAILURE);
		}
		
		/* acquisisco i dati */
		fscanf( pFile, "%Lg\n", f + l );
	}

	/* chiudo il file di input */
	if( fclose( pFile ) == EOF ) {
		fprintf ( stderr, " > Impossibile chiudere il file '%s'; %s\n",
				argv[1], strerror(errno) );
		exit (EXIT_FAILURE);
	}

	/*---------------------------------------------------------------
	 *  BINNING
	 *-------------------------------------------------------------*/
	/* output-file name */
	char bin_file_name[] = "binning.dat";
	FILE *bin = fopen( bin_file_name, "w" );
	if ( bin == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				bin_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}
	
	/* output stream */
	FILE *stream = stdout;
	/* make header */
	fprintf( stream, "#b-size\tmean\tround_var\tvar\tmean\tround_sdom\tsdom\n" );

	long double temp;

	/* media ed errore (inizializzate a zero) */
	long double mean = (long double) 0;
	long double err =  (long double) 0;
	
	unsigned int bins;
	register unsigned int i;
	register long double *fptr = NULL;

	for ( unsigned int k = 1; k <= ord; k ++ ) {
		/* bin size */
		bins = (unsigned) l / k;
		/* assign temporary pointer */
		fptr = f;

		/* reset mean and error */
		mean = (long double) 0;
		err = (long double) 0;

		for ( unsigned int j = 0; j < bins; j ++ ) {
			/* reset temporary variable */
			temp = (long double) 0;

			/* evaluate bin */
			for ( i = 0; i < k; i ++ ) {
				temp += *fptr;
				++ fptr;
			}

			/* normalize bin */
			temp /= (long double) k;

			/* update mean and error */
			mean += temp;
			err += temp * temp;
		}

		/* normalize mean and errors */
		mean /= (long double) bins;

		err /= (long double) bins;
		err -= (long double) mean * mean;
		err = (long double) sqrtl( err );

		/* stampo la dimensione del bin */
		fprintf( stream, "%u\t", k );

		/* stampo nel file media e varianza */
		round( mean, err, stream );
		fprintf( stream, "\t%Lg\t", err );

		/* stampo nel file media e sdom */
		err /= (long double) sqrtl( bins );
		round( mean, err, stream );
		fprintf( stream, "\t%Lg\n", err );
	}

	if( fclose(bin) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				bin_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}				/* ----------  end of function main  ---------- */
