#include "colors.h"
#include "round.h"

#include "part_soffici.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <string.h>
#include <errno.h>


/*
 * ------------------------------------------------------------------
 *       Class: Soft
 *      Method: Soft
 * Description: [ctor]
 * ------------------------------------------------------------------
 */
Soft::Soft (void) {


	/* inizializzo posizioni delle particelle */
	double x[3], y[3];
	/* energia cinetica */
	register double kinetic;
	/* indice di particella (pari/dispari) */
	register unsigned int q, n = 0, m;
	register unsigned short j;
	/* temporary pointers */
	struct ptcl *pn, *pm;

	for ( unsigned short int i = 0; i < N; i ++ ) {
		/* calcolo la coordinata x */
		*x = (double) i / N;
		/* coordinata traslata */
		*y = *x + .5 / N;

		for ( unsigned short int k = 0; k < N; k ++ ) { /* 3D */
			/* calcolo la coordinata z */
			*( x + 2 ) = (double) k / N;
			*( y + 2 ) = *( x + 2 ) + .5 / N;

			for ( j = 0; j < N; j ++ ) {
				/* coordinata y */
				*( x + 1 ) = (double) j / N;
				*( y + 1 ) = *( x + 1 ) + .5 / N;
				
				/* aggiorno l'indice della particella (dispari) */
				m = n + 1;
				/* azzero l'energia cinetica delle particella */
				kinetic = (double) 0;
				pn = p + n;
				pm = p + m;
				/* assegno le posizioni e le velocità */
				for ( unsigned short int d = 0; d < D; d ++ ) {
					/* assegno la coordinata d-esima */
					*( (*pn).x + d ) = *( x + d );
					/* assegno la velocità d-esima */
					*( (*pn).v + d ) = (double) 2 * rand() / RAND_MAX;
					(*pn).t_last = (double) 0;

					/* assegno la coordinata traslata */
					*( (*pm).x + d ) = *( y + d );
					/* assegno la velocità (cambiata di segno) */
					*( (*pm).v + d ) = - *( (*pn).v + d );
					(*pm).t_last = (double) 0;

					/* calcolo l'energia cinetica */
					kinetic += *( (*pn).v + d ) * *( (*pn).v + d );
				}
//				fprintf( stderr, "\n" );

				/* aggiorno l'indice della particella (pari) */
				n += 2;

//				printf( "%g\n%g\n", kinetic, kinetic );
				K += kinetic;
			}
		}
	}

	fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET
			"] Initial kinetic energy: %g\n", K );

//	exit( EXIT_SUCCESS );
	/* stampa la velocità del centro di massa */
//	Sistema::mass_center_speed();
} /* -----  end of method Soft::Soft (ctor)  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Soft
 *      Method: ~Soft
 * Description: destructor
 * ------------------------------------------------------------------
 */
Soft::~Soft (void) {
} /* -----  end of method Soft::~Soft (dtor)  ----- */
