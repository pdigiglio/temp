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
#define ACD	15
#define BIN	10

int
main ( int argc, char *argv[] ) {
	/* controllo che argomenti da linea di comando */ 
	if ( argc > 3 ) {
		fprintf( stderr, " > Troppi argomenti!\n");

		/* esco dal programma */
		exit( EXIT_FAILURE );
	} else if ( argc < 2 ) {
		fprintf( stderr, " > Manca il nome del file!\n");

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

		/* assegno puntatore temporaneo */
		fscanf( pFile, "%Lg\n", f + l );
	}

	/* chiudo il file di input */
	if( fclose( pFile ) == EOF ) {
		fprintf ( stderr, " > Impossibile chiudere il file '%s'; %s\n",
				argv[1], strerror(errno) );
		exit (EXIT_FAILURE);
	}

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
		/* assegno i puntatori temporanei */
		aptr = a_co + t;
		eptr = a_co_err + t;

		/* azzero media e errore */
		*aptr = (long double) 0;
		*eptr = (long double) 0;

		/* stampo la coordinata temporale */
		fprintf( oFile, "%hu\t", t );
		
		for ( unsigned short int j = 0; j < BIN; j ++ ) {
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


			/* normalizzo */
			mp /= (long double) step;

			for ( s = start; s < stop - t; s ++ )
				ac += ( *( f + s ) - mp ) * ( *( f + s + t ) - mp );


			/* normalizzo */
			ac /= (long double) ( s - start );
			
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
