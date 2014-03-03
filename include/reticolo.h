
#ifndef  reticolo_INC
#define  reticolo_INC

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "beta.h"
#include "side.h"

/* dimension */
#define D	2


/* short-cut to extend do Potts model */
typedef signed short int spin;

/*
 * ==================================================================
 *        Class: Reticolo
 *  Description: 
 * ==================================================================
 */
class Reticolo {
	public:
		Reticolo (void); /* ctor */
		virtual ~Reticolo (void); /* dtor */

		/* return value of 'Reticolo::E' */
		double get_E ( void );

		void print_lattice ( void );
		void print_correlator ( FILE *stream = stdout );

		static const long int L2 = L * L;
		static const short unsigned int D2 = 2 * D;
		static const short unsigned int Q_1 = (short unsigned) Q - 1;
		
	protected:

		struct sito {
			/* nearest neighbours */
			unsigned short int nn[2 * D][D];
			/* spin value */
			spin s;
		} x[L][L];
		typedef struct sito Sito;

		/* auxiliary array to check nearest neighbours */
		const signed short int s[D2][D] = {
			{ 0, 1 }, { 1, 0 },
			{ 0, -1 }, { -1, 0 }
		};

		/* controls if a site has been already checked */
		bool ckd[L][L] = {};
		bool ckd_status = (bool) 0;

		/* cluster sites stack */
		const unsigned int head = 0;
		unsigned int tail = 0;
		/* stack contains at most L * L elements */
		unsigned short int stack[L2][D];

		/* correlator */
		double corr[L] = {};
		
		/* energy initialized in ctor */
		long int E = (long int) 0;

		/* returns spin in *pos */
		spin S ( const unsigned short int *p = NULL );
}; /* -----  end of class Reticolo  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Reticolo
 *      Method: get_E
 * Description: get energy (divide by L2 to have density!)
 * ------------------------------------------------------------------
 */
inline double
Reticolo::get_E ( void ) {
	return (double) Reticolo::E;
} /* -----  end of method Reticolo::get_E  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Reticolo
 *      Method: S
 * Description: return spin of site in ( p[0], p[1] ).
 * ------------------------------------------------------------------
 */
inline spin
Reticolo::S ( const unsigned short int *p ) {
	return (*( *( x + *p ) + *( ++ p ) )).s;
} /* -----  end of method Reticolo::S  ----- */

#endif   /* ----- #ifndef reticolo_INC  ----- */
