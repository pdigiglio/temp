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

/* auto-correlator domain */
#define ACD	30
#define BIN	1

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
	long double *f = NULL, *fptr = NULL;
	long double temp;

	/* media ed errore (inizializzate a zero) */
	long double mean = (long double) 0;
	long double err =  (long double) 0;

	/* binning output file */
	char out_file_name[] = "analisi.dat";
	FILE *out_stream = fopen( out_file_name, "w" );
	if ( out_stream == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				out_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	/* acquisisco i valori e calcolo la media */
	unsigned int l, i;
	for ( l = 0; !( feof(pFile) ); l ++ ) {
		/* alloco la memoria per i valori in input */
		f = (long double *) realloc( f, (l + 1) * sizeof(long double) );
		if ( f == NULL ) {
			fprintf ( stderr, "\ndynamic memory reallocation failed\n" );
			exit (EXIT_FAILURE);
		}

		/* assegno puntatore temporaneo */
		fptr = f + l;

		/* azzero i valori */
		*fptr = (long double) 0;

		/* acquisisco i dati */
		for ( i = 0; i < ord && !( feof(pFile) ); i ++ ) {
			fscanf(pFile, "%Lf\n, ", &temp);
			*fptr += temp;
		}
		
//		fprintf( out_stream, "%u\t", l );
		/* normalizzo i cluster */
		*fptr /= (long double) i;
		fprintf( out_stream, "%LG\n", *fptr );
			
		/* aggiorno le medie */
		temp = *fptr;

		mean += temp;
		err += temp * temp;
	}

	/* chiudo il file di input */
	if( fclose( pFile ) == EOF ) {
		fprintf ( stderr, " > Impossibile chiudere il file '%s'; %s\n",
				argv[1], strerror(errno) );
		exit (EXIT_FAILURE);
	}

	if( fclose(out_stream) == EOF ) { /* close output file */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				out_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	FILE *stream = stdout;
	fprintf( stream, "#bin-size\t" );

	/* head */
	fprintf( stream, "#ord\nmean\tvariance\tmean\tsdom\n" );

	/* bin size */
	fprintf( stream, "%u\t", ord);

	/* normalizzo la media */
	mean /= (long double) l;

	/* calcolo la varianza */
	err /= (long double) l;
	err -= mean * mean;
	err = (long double) sqrtl( err );

	/* stampo nel file media e varianza */
	round( mean, err, stream );
	fprintf( stream, "\t" );

	/* stampo nel file media e sdom */
	round( mean, err / sqrtl( l ), stream );
	fprintf( stream, "\n" );
	

	/*-------------------------------------------------------------------
	 *  AUTOCORRELATORI
	 *-----------------------------------------------------------------*/

	long double a_co[ACD], a_co_err[ACD];
	register long double ac, mp;
	long double *aptr = NULL, *eptr = NULL;

	/* coefficiente di normalizzazione */
	long double norm = (long double) 1;

	unsigned int step = (unsigned) l / BIN;
	unsigned int start, stop;

	/* apro un file di output (lo creo) */
	char oFile_name[] = "ac.dat"; /* output-file name */
	FILE *oFile = fopen( oFile_name , "w" );

	if ( oFile == NULL ) {
		fprintf ( stderr, " > Impossibile aprire il file '%s'; %s\n",
				oFile_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	/* calcolo gli autocorrelatori */
	for ( unsigned short int t = 0; t < ACD; t ++ ) {
		fprintf( stderr, "t: %u\n", t );
		/* assegno i puntatori temporanei */
		aptr = a_co + t;
		eptr = a_co_err + t;
		fprintf( stderr, "assign\n" );

		/* azzero media e errore */
		*aptr = (long double) 0;
		*eptr = (long double) 0;

		/* stampo la coordinata temporale */
		fprintf( oFile, "%hu\t", t );
		
		for ( unsigned short int j = 0; j < BIN; j ++ ) {
			fprintf( stderr, "j: %u\n", j );
			/* azzero auto-correlatore */
			ac = (long double) 0;
			/* azzero media parziale */
			mp = (long double) 0;

			start = j * step;
			stop = ( j + 1 ) * step;

			/* calcolo la media parziale */
			unsigned int s;
			for ( s = start; s < stop; s ++ )
				mp += *( f + s );

			fprintf( stdout, "media: %Lg\n", mp );

			/* normalizzo */
			mp /= (long double) step;

			for ( s = start; s < stop - t; s ++ )
				ac += *( f + s ) * *( f + s + t );


			/* normalizzo */
			ac /= (long double) ( s - start );
			fprintf( stdout, "auto: %Lg div: %d \n", ac, (int) ( s - start ) );
			
			ac -= mp * mp;

			/* aggiorno l'autocorrelatore */
			*aptr += ac;
			*eptr += ac * ac;
		}

		/* normalizzo l'autocorrelatore */
		*aptr /= (long double) BIN;
		*eptr /= (long double) BIN;
		*eptr -= *aptr * *aptr;
		*eptr /= (long double) ( BIN - 1 );

		/* salvo il primo valore per normalizzare gli altri */
		if ( t == 0 )
			norm = *aptr;
		
		/* stampo i valori normalizzati */
		fprintf( oFile, "%Lg\t%Lg\n", *aptr / norm, sqrtl( *eptr ) / norm );
	}

	/* chiudo il file di output */
	if( fclose(oFile) == EOF ) {
		fprintf ( stderr, " > Impossibile chiudere '%s'; %s\n",
				oFile_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	exit( EXIT_SUCCESS );
}				/* ----------  end of function main  ---------- */
