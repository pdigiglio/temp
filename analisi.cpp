/*
 * ==================================================================
 *
 *       Filename:  analisi.cpp
 *
 *  Description:  [sintassi] Il programma dev'essere lanciato con la
 *  			 sintassi "./analisi {file(.dat)} {ord}". Il {file}
 *  			 deve contenere tre colonne di cui le prime due sepa-
 *  			 rate da una virgola.
 *
 *  			  [output] Il programma suddivide i dati del file in
 *  			 cluster di dimensione {ord} e ne fa la media. Poi
 *  			 calcola varianza e deviazione standard stampandole a
 *  			 schermo. Nel frattempo crea un file contenente gli
 *  			 autocorrelatori.
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

#include "round.h"

/* numero di colonne nel file in input */
#define C	1

int
main ( int argc, char *argv[] ) {
	/* ordine della suddivisione */
	unsigned short int ord = 1;
	
	/* controllo che argomenti da linea di comando */ 
	if ( argc == 3 )
		ord = atoi( argv[2] );
	else if ( argc == 2 )
		fprintf( stderr, " > Dimensione bin impostata di default a %hu\n", ord );
	else { /* se sono sbagliati */
		if ( argc > 3 )
			fprintf( stderr, " > Troppi argomenti!\n");
		else if ( argc < 2 )
			fprintf( stderr, " > Manca il nome del file!\n");

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
	 *  CALCOLO MEDIE ED ERRORI
	 * -------------------------------------------------------------*/

	/* valori delle misure */
	double **f = NULL;
	double temp;

	/* media ed errore (inizializzate a zero) */
	double mean[C] = {}, err[C] =  {};

	/* acquisisco i valori e calcolo la media */
	unsigned int l, i;
	for ( l = 0; !( feof(pFile) ); l ++ ) {	
		/* alloco la memoria per i valori in input */
		f = (double **) realloc( f, (l + 1) * sizeof(double *) );
		if ( f == NULL ) {
			fprintf ( stderr, "\ndynamic memory reallocation failed\n" );
			exit (EXIT_FAILURE);
		}

		f[l] = (double *) malloc( C * sizeof(double) );
		if ( f[l] == NULL ) {
			fprintf ( stderr, "\ndynamic memory allocation failed\n" );
			exit (EXIT_FAILURE);
		}
		
		/* azzero i valori di '**f' */
		for ( unsigned short int j = 0; j < C; j ++ )
			f[l][j] = (double) 0;

		/* calcolo i cluster */
		for ( i = 0; i < ord && !( feof(pFile) ); i ++ ) {
			signed short int j;

			/* acquisisco prime colonne */
			for ( j = 0; j < C - 1; j ++ ) {
				fscanf(pFile, "%lf, ", &temp);
				f[l][j] += temp;
			}

			/* ultima colonna */
			fscanf(pFile, "%lf\n", &temp);
			f[l][j] += temp;
		}
		
//		printf( "%u\t", l );
		/* normalizzo i cluster ed aggiorno le medie */
		for ( unsigned short int j = 0; j < C; j ++ ) {
			f[l][j] = (double) f[l][j] / i;
//			printf("%G\t", f[l][j]);
			mean[j] += f[l][j];
			err[j] += pow( f[l][j], (double) 2);
		}
//		printf("\n");
	}

	/* chiudo il file di input */
	if( fclose( pFile ) == EOF ) {
		fprintf ( stderr, " > Impossibile chiudere il file '%s'; %s\n",
				argv[1], strerror(errno) );
		exit (EXIT_FAILURE);
	}


	FILE *stream = stdout;
//	fprintf( stream, "%u\t", ord);
	for ( unsigned short int j = 0; j < C; j ++ ) {
		/* normalizzo la media */
		mean[j] = (double) mean[j] / l;

		/* calcolo la varianza */
		err[j] = err[j] / l;
		err[j] = (double) sqrt( err[j] - pow( mean[j], (double) 2) );

		/* stampo nel file varianza e sdom */
		round( *( mean + j), *( err + j ) / sqrt( l ), stream );
	}
	fprintf( stream, "\n" );

	exit( EXIT_SUCCESS );
}				/* ----------  end of function main  ---------- */